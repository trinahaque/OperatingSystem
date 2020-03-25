#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// needs struct anytime passing in multiple data type
typedef struct fileInfo {
    char * filename;
    long start;
    long size;
} fileInfo, * fileInfo_p;


// open the file with correct size
// find words that are 6 chars or higher in that file
// add words to a hashmap
// thread function takes void pointer 
void * countWords(void * arg){
    char * delimeter = "\"\'.""''?:;-,—*($%)! \t\n\x0A\r";
    int file;
    // WHAT DOES THE BUFFER DO HERE?
    char * wordArray;
    fileInfo_p fileInfo = (fileInfo_p) arg;
    char * stringToken;
    char * saveptr;

    wordArray = malloc(fileInfo->size+4);
    // open the file
    file = open(fileInfo->filename, O_RDONLY);
    lseek(file, fileInfo->start, SEEK_SET);
    
    // texts from the file will be put into the wordArray
    long response = read(file, wordArray, fileInfo->size);
    // printf(wordArray);

    // null terminator
    wordArray[fileInfo->size] = 0;

    // segmentation error when I used **saveptr

   
    

 
    stringToken = strtok_r(wordArray, delimeter, &saveptr);
    // segmentation error when I try to print stringToken
    printf("%s",stringToken);
    
    // do{
    //     if (strlen(stringToken) > 5){
    //         printf("word is: %s\n", stringToken);
    //     }
    //     stringToken = strtok_r(NULL, delimeter, &saveptr);
    // }while(stringToken);
    
    
    // free is clearing up the memory
    free(wordArray);
    close(file);
    return NULL;
}


 // argc is numner of command line arguments
int main(int argc, char** argv){
    // processCount is number of threads
    // chunkInfo 
    // fd is file
    int numberOfThreads;
    int file;
    long fileSize;
    char * fileName;
    pthread_t * threadArray;
    fileInfo * fileArray;

    if (argc < 3){
        printf("Needs two arguments: filename and threadcount!\n");
        return 1;
    }

     // atoi converts char to int
    numberOfThreads = atoi(argv[2]);
    if (numberOfThreads < 1){
        printf("Need at least 1 thread!\n");
        return 2;
    }

    fileName = argv[1];
    
    // handles file
    // argv[1] gives us filename
    file = open(argv[1], O_RDONLY);
    lseek(file, 0, SEEK_END);
    fileSize = lseek(file, 0, SEEK_CUR);
    lseek(file, 0, SEEK_SET);
    printf("%d: size\n", fileSize);
    close(file);

    struct timespec startTime;
    struct timespec endTime;
    clock_gettime(CLOCK_REALTIME, &startTime);
    // allocate thread array
    // malloc returns a void pointer
    // void pointer can point to any particular type of data type we need
    fileArray = malloc (sizeof(fileInfo));
    // creates an array of p_thread id
    threadArray = malloc (sizeof(pthread_t) * numberOfThreads);

    for (int i = 0; i < numberOfThreads; i++){
        // need this to be a fileArray because it will create different pointers
        // and pass that info instead of changing the info of the same pointer
        fileArray[i].start = (fileSize / numberOfThreads) * i;
        if (i == numberOfThreads - 1){
            fileArray[i].filename = fileName;
            fileArray[i].size = fileSize - fileArray[i].start;
        }
        else {
            fileArray[i].filename = fileName;
            fileArray[i].size = (fileSize / numberOfThreads);
        }
        // fileArray[i] passing in info such as start and end of file it needs to process
        // &(fileArray[i]) sends struct fileInfo
        // 1st arg: passes p_thread id
        // 2nd arg: thread attachale or detachable
        // 3rd arg: function I am calling
        // 4th arg: variables I am passing onto the function
        // & before a variable makes it a pointer to that variable
        pthread_create(&(threadArray[i]), NULL, countWords, &(fileArray[i]));
    }

    // pthread_join makes the function to wait until the thread is done
    for(int i = 0; i < numberOfThreads; i++){
        pthread_join(threadArray[i], NULL);
    }



    clock_gettime(CLOCK_REALTIME, &endTime);
    time_t sec = endTime.tv_sec - startTime.tv_sec;
    long n_sec = endTime.tv_nsec - startTime.tv_nsec;
    if (endTime.tv_nsec < startTime.tv_nsec)
    {
    --sec;
    n_sec = n_sec + 1000000000L;
    }
    printf("Total Time was %ld.%09ld seconds\n", sec, n_sec);
    

    return 0;
}
  