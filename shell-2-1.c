#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_CMD_LINE_ARGS  128

int min(int a, int b) { return a < b ? a : b; }

void redirect_input(const char* filename) { 
    int fd = open (filename, O_RDONLY);
    if (fd == -1) {  
        perror("open filename in redirect_input failed");
        exit(-1);
    }
    dup2(fd, STDIN_FILENO);
}

void redirect_output(const char* filename, bool append) { 
    int flags = O_WRONLY;
    flags |= append ? O_APPEND : O_CREAT;

    int fd = open (filename, flags);
    if (fd == -1) { 
        perror("open failure in redirect_output");
        exit(-2);
    }
    dup2(fd, STDOUT_FILENO);
}
// break a string into its tokens, putting a \0 between each token
// save the beginning of each string in a string of char *'s (ptrs to chars)

int parse(char* p, char* argv[]) {
  char* filename;
  char in_or_out;
  bool append = false;
  int argc = 0;

  while (*p != '\0') { 
      while (*p != '\0' && isspace(*p)) { 
          *p++ = '\0';
      }
      if (*p == '<' || *p == '>') { 
          in_or_out = *p++;
          if (*p == '>') {++p; append = true; }   // >> means append to file
          while (*p != '\0' && isspace(*p)) {
              ++p;
          }
          filename = p;
          while (*p != '\0' && !isspace(*p)) { 
              ++p;
          }
          *p++ = '\0';
          in_or_out == '<' ? redirect_input(filename) : redirect_output(filename, append);
          continue; 
      }
      *argv++ = p;
      ++argc;
      while (*p != '\0' && !isspace(*p)) { 
        ++p; 
      }
  }
  *argv = NULL;
  return argc;
}

void execute_with_pipe(char *cmd1[], char *cmd2[]) {
  int pipefd[2];
  pid_t p1, p2;

  if (pipe(pipefd) < 0) {
    perror("pipe() failed");
    exit(EXIT_FAILURE);
  }
  p1 = fork();
  if (p1 < 0) {
    perror("fork() failed");
    exit(EXIT_FAILURE);
  }

  if (p1 == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    execvp(cmd1[0], cmd1);
    perror("execvp of cmd1 failed");
    exit(EXIT_FAILURE);
  } else {
    p2 = fork();
    if (p2 < 0) {
      perror("fork() failed");
      exit(EXIT_FAILURE);
    }

    if (p2 == 0) {
      close(pipefd[1]);
      dup2(pipefd[0], STDIN_FILENO);
      execvp(cmd2[0], cmd2);
      perror("execvp of cmd2 failed");
      exit(EXIT_FAILURE);
    } else {
      close(pipefd[0]);
      close(pipefd[1]);
      wait(NULL);
      wait(NULL);
    }
  }
}



int execute(char* input) {
  int i = 0;
  char* shell_argv[MAX_CMD_LINE_ARGS];
  memset(shell_argv, 0, MAX_CMD_LINE_ARGS * sizeof(char));

  int shell_argc = parse(input, shell_argv);
  // printf("after parse, what is input: %s\n", input);      // check parser
  // printf("argc is: %d\n", shell_argc);
  // while (shell_argc > 0) {
  //   printf("argc: %d: %s\n", i, shell_argv[i]);
  //   --shell_argc;
  //   ++i;
  // }

  // Initialize cmd1 and cmd2 for potential piping
  char* cmd1[MAX_CMD_LINE_ARGS] = {NULL};
  char* cmd2[MAX_CMD_LINE_ARGS] = {NULL};

  // Check for pipe and split the command
  int j = 0, k = 0;
  bool isPipe = false;
  for (i = 0; i < shell_argc; ++i) {
    if (strcmp(shell_argv[i], "|") == 0) {
      isPipe = true;
      continue;
    }
    if (!isPipe) {
      cmd1[j++] = shell_argv[i];
    } else {
      cmd2[k++] = shell_argv[i];
    }
  }

  int status = 0;
  pid_t pid = fork();
  
  if (pid < 0) { fprintf(stderr, "Fork() failed\n"); }  // send to stderr
  else if (pid == 0) { // child
    // If a pipe is detected, execute with pipe
    if (isPipe) {
      execute_with_pipe(cmd1, cmd2);
      exit(0);
    } 
    // Otherwise, execute normally
    else {
      int ret = 0;
      if ((ret = execvp(shell_argv[0], shell_argv)) < 0) {  // can do it arg by arg, ending in NULL
        exit(1);
      }
      exit(0);
    }
  }
  else { // parent -----  don't wait if you are creating a daemon (background) process
    while (wait(&status) != pid) { }
  }

  return 0;
}


int main(int argc, const char * argv[]) {
  char input[BUFSIZ];
  char last_input[BUFSIZ];  
  bool finished = false;

  memset(last_input, 0, BUFSIZ * sizeof(char));  
  while (!finished) {
    memset(input, 0, BUFSIZ * sizeof(char));

    printf("osh > ");
    fflush(stdout);

    if (strlen(input) > 0) {
      strncpy(last_input, input, min(strlen(input), BUFSIZ));
      memset(last_input, 0, BUFSIZ * sizeof(char));
    }

    if ((fgets(input, BUFSIZ, stdin)) == NULL) {   // or gets(input, BUFSIZ);
      fprintf(stderr, "no command entered\n");
      exit(1);
    }
    input[strlen(input) - 1] = '\0';          // wipe out newline at end of string
    // printf("input was: '%s'\n", input);
    // printf("last_input was: '%s'\n", last_input);
    if (strncmp(input, "exit", 4) == 0) {   // only compare first 4 letters
      finished = true;
    } else if (strncmp(input, "!!", 2) == 0) { // check for history command
  //todo add history 
        if (strlen(last_input) == 0) {
          printf("No commands in history.\n");
  } else {
    execute(last_input);
  }
  } else {
    strncpy(last_input, input, min(strlen(input), BUFSIZ));
    execute(input);
  }

  }
  
  printf("\t\t...exiting\n");
  return 0;
}


/*
robert1014@robert1014:~/os/shell$ ./shell-2-1
osh > cal 2023
                            2023
      January               February               March          
Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  
 1  2  3  4  5  6  7            1  2  3  4            1  2  3  4  
 8  9 10 11 12 13 14   5  6  7  8  9 10 11   5  6  7  8  9 10 11  
15 16 17 18 19 20 21  12 13 14 15 16 17 18  12 13 14 15 16 17 18  
22 23 24 25 26 27 28  19 20 21 22 23 24 25  19 20 21 22 23 24 25  
29 30 31              26 27 28              26 27 28 29 30 31     
                                                                  

       April                  May                   June          
Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  
                   1      1  2  3  4  5  6               1  2  3  
 2  3  4  5  6  7  8   7  8  9 10 11 12 13   4  5  6  7  8  9 10  
 9 10 11 12 13 14 15  14 15 16 17 18 19 20  11 12 13 14 15 16 17  
16 17 18 19 20 21 22  21 22 23 24 25 26 27  18 19 20 21 22 23 24  
23 24 25 26 27 28 29  28 29 30 31           25 26 27 28 29 30     
30                                                                

        July                 August              September        
Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  
                   1         1  2  3  4  5                  1  2  
 2  3  4  5  6  7  8   6  7  8  9 10 11 12   3  4  5  6  7  8  9  
 9 10 11 12 13 14 15  13 14 15 16 17 18 19  10 11 12 13 14 15 16  
16 17 18 19 20 21 22  20 21 22 23 24 25 26  17 18 19 20 21 22 23  
23 24 25 26 27 28 29  27 28 29 30 31        24 25 26 27 28 29 30  
30 31                                                             

      October               November              December        
Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  
 1  2  3  4  5  6  7            1  2  3  4                  1  2  
 8  9 10 11 12 13 14   5  6  7  8  9 10 11   3  4  5  6  7  8  9  
15 16 17 18 19 20 21  12 13 14 15 16 17 18  10 11 12 13 14 15 16  
22 23 24 25 26 27 28  19 20 21 22 23 24 25  17 18 19 20 21 22 23  
29 30 31              26 27 28 29 30        24 25 26 27 28 29 30  
                                            31                    
osh > cp output.txt test.txt
osh > ls
output.txt  shell-2-1  shell-2-1.c  test.  test.txt
osh > !!
output.txt  test.txt
osh > exit
                ...exiting
*/