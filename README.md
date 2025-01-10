Technologies Used: C

Project Description:
-Developed a Unix shell interface in C as part of a university course project, designed to execute user commands and manage processes within a Unix environment.
-Implemented core functionalities including input/output redirection, pipes for inter-process communication, and command execution in child processes using system calls such as fork(), exec(), wait(), dup2(), and pipe().
-Enhanced the shell with custom features like command history retrieval, enabling users to rerun previous commands with '!!', and built-in support for concurrent process execution with '&' operator.
-Integrated file redirection operations, allowing redirection of standard input and output to and from files using the dup2() system call, which facilitated complex command line operations involving multiple utilities.


Key Achievements:
-Successfully simulated advanced shell operations, increasing understanding and practical application of Unix system-level programming.
-Delivered a fully functional shell interface that supports multitasking and batch processing, demonstrating deep integration of Unix system concepts.
-Provided a robust platform for testing and development purposes, exemplified by comprehensive documentation and live demonstration examples included in the project repository.


Output:

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
