#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// extension file only handles cases when multiple 
// arguments are passed such as ls -la
// argv[0] = "./extension"
// argv[1] = "ls"
// argv[2] = "l"
int main(int argc, char *argv[]){
    char stringCommand[1026];

    if (argc > 1){
        // strcpy copies string
        // copies stringCommand to argv[1]
        strcpy(stringCommand, argv[1]);
    }

    // when there is multiple commands
    // ls -l -a
    // argc is len(argv)
    if (argc > 2){
        for (int i = 2; i < argc; i++){
            // char *strcat(char *dest, const char *src) appends the 
            //string pointed to by src to the end of the string pointed to by dest.
            strcat(stringCommand, " ");
            strcat(stringCommand, argv[i]);
            strcat(stringCommand, " ");
        }
        // system takes in the command and passes it to the host environment to be executed
        // by the command processor and returns after the command has been completed
        system(stringCommand);
        //returns -1 or error
        // returns command status if there is no error
    }
    return 0;

}