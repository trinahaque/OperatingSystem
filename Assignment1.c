#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


// argc refers to the number of arguments passed
// argc is 2 if user types in ./assn1 prompt$
// argv[] if a pointer array which points to each argument passed to the program
int main(int argc, char *argv[]){
    char stringCommand[1026];
    int status;
    int child_exit_status;
    
    // while loop so user can keep typing commands until exiting
    while (1) {
    // if user types in prompt or something after ./assn1, it prints that for user
        if (argc > 1){
            printf("Type your command %s ", argv[1]);
        }
        // otherwise it displays $ to user to type in commands
        else {
            printf("Type your command $ ");
        }

        // it sets the user input into the variable stringCommand
        gets(stringCommand);
        
        // fork creates a new process
        pid_t pid = fork();
        printf("pid is %d\n", pid);

        if (pid < 0){
            perror("fork failed, no child was created");
            exit(1);
        }
        // fork returns 0 to the newly created child process
        else if (pid == 0){
            printf("hello I am here");
            // char *args[] = {"./extension", "ls", "-l", "a", NULL}
            // char *args[] is a string array
            // NULL specifies the end of the array
            // This line is passing on the commands to file extension 
            // and storing the return value in a string array
            char *args[] = {"./extension", stringCommand, NULL};
            // printf("args");
            // printf(“%s”, args[1]);

            // int execvp(const char *file, char *const argv[]);
            execvp(args[0], args);
        }
        
        // waitpid() and printing exit status
        // cpid gives child id
        pid_t cpid = waitpid(pid, &status, 0);
        // waitpid(pid, &status, 0);
        if (WIFEXITED(status)) { 
            child_exit_status = WIFEXITED(status);         
            printf("\nChild %d, exited with %d\n", cpid, child_exit_status); 
        } 

    }

    return 0;
}

/* TODO:
1. where is single command getting implemented?
2. strcat in extension
3. while loop issue
*/