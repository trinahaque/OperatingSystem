/**************************************************************
* Class: CSC-415-01# Spring 2020
* Name: Trina Haque
* Student ID: 920863317
* Project: Assignment 1 – Simple Shell
*
* File: SimpleShell.c
*
* Description:
*
**************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
// #include <stdlib.h>


// argc refers to the number of arguments passed
// argv[ ] is a pointer array which points to each argument passed to the program

int main(){
// int main(int argc, char *argv[]){
    // create array of characters to store the token
    char str[1026]; 
    char *array[1026];
    int i = 0; 
    int token_number = 0; 
    int status;
    int exit_status;

    // infinite loop so user can entering input
    while(1){
        scanf("%[^\n]%*c", str); 
        // printf("User input is: %s\n", str); 

        // add an if statement for str == exit()
        
        // parsing strings
        char * token = strtok(str, " ");
        while (token != NULL){
            token_number++;
            array[i++] = token;
            // printf(" %s\n", token);
            token = strtok(NULL, " ");
        }

        for (i = 0; i < token_number; ++i){
            printf("%s\n", array[i]);
        }

        // printf("I am: %d\n", (int) getpid());

        // pid_t pid = fork();

        // printf("fork returned: %d\n", (int) pid);

        // // too many forks/process running can fail
        // if (pid < 0){
        //     perror("Fork failed");
        //     exit(1);
        // }
        // if (pid == 0){
        //     // printf("I am the child with pid %d\n", (int) getpid());
        //     // char *args[] = system(array);

        //     execvp(array[0], array);
        //     // this stops executing the child process
        //     // exit(0);
        //     // everything after this will be about parent
        // } 
        // else if (pid > 0){
        //     // printf("I am the parent\n");
        //     waitpid(pid, &status, 0);
        //     if ( WIFEXITED(status) ) 
        //     { 
        //         exit_status = WEXITSTATUS(status);         
        //         printf("\nChild %d, exited with %d\n",  pid, exit_status); 
        //     } 

        // }

    }
  

    

    /**
    1. Analyze the tokens
    2. Execute depending on the token
    3. In forking, check if a command is exit. If so, then exit the program
    **/

    
    //TODO
    // 1) Exit needs to exit
    // 2) Child needs to return the current exit status
    // 3) Handle EOF error
    // 4) Exit when there is no input
    
  
    
    

    
    return 0;
}