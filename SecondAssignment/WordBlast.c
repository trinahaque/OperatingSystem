#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

pthread_mutex_t lock;

// typedef struct node{
//     char * key;
//     int val;
//     struct node *next;
// } *node;

struct node{
    char * key;
    int val;
    struct node *next;
};

struct table{
    int size;
    struct node **list;
};

struct table *createTable(int size){
    struct table *t = (struct table*)malloc(sizeof(struct table));
    t->size = size;
    t->list = (struct node**)malloc(sizeof(struct node*)*size);
    int i;
    for(i=0;i<size;i++)
        t->list[i] = NULL;
    return t;
}


int hashCode(struct table *t, char *key) {
    unsigned long int hash_value = 0;
    unsigned int i;
    unsigned int key_len = strlen(key);

    // do several rounds of multiplication
    for (i = 0; i < key_len; ++i) {
        hash_value = hash_value * 37 + key[i];
    }

    // make sure value is 0 <= value < TABLE_SIZE
    hash_value = hash_value % t->size;

    return hash_value;
}
void insert(struct table *t, char * key, int val){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            temp->val = val;
            return;
        }
        temp = temp->next;
    }
    newNode->key = key;
    newNode->val = val;
    newNode->next = list;
    t->list[pos] = newNode;
}

int lookup(struct table *t, char * key){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            return temp->val;
        }
        temp = temp->next;
    }
    return -1;
}
// node lookup(struct table *t, char * key){
//     int pos = hashCode(t,key);
//     struct node *list = t->list[pos];
//     struct node *temp = list;
//     while(temp){
//         if(temp->key==key){
//             return temp;
//         }
//         temp = temp->next;
//     }
//     return NULL;
// }


// needs struct anytime passing in multiple data type
typedef struct fileInfo {
    char * filename;
    long start;
    long size;
    struct table *t;
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
    int totalWords;
    struct table *t = fileInfo->t;

    totalWords = 0;

    wordArray = malloc(fileInfo->size + 1);
    // open the file
    file = open(fileInfo->filename, O_RDONLY);
    lseek(file, fileInfo->start, SEEK_SET);
    
    // texts from the file will be put into the wordArray
    long response = read(file, wordArray, fileInfo->size);
    // printf(wordArray);

    // null terminator
    wordArray[fileInfo->size] = 0;

 
    stringToken = strtok_r(wordArray, delimeter, &saveptr);

    // struct table *t = createTable(5);
    

    do{
        if (strlen(stringToken) > 5){
            totalWords += 1;
            int tmp = lookup(t, stringToken);
            // node tmp = lookup(t, stringToken);
            if (tmp == -1){
                pthread_mutex_lock(&lock);
                insert(t, stringToken, 1);
                pthread_mutex_unlock(&lock);
            }
            else{
                pthread_mutex_lock(&lock);
                // insert(t, stringToken, tmp->val + 1);
                insert(t, stringToken, tmp + 1);
                // tmp->val += 1;
                pthread_mutex_unlock(&lock);
            }
            printf("word is: %s\n", stringToken);
        }
        stringToken = strtok_r(NULL, delimeter, &saveptr);
    }while(stringToken);

    // printf("total word is %d\n", totalWords);
    
    // free is clearing up the memory
    // free(hash_table);

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
    struct table *t = createTable(1000);
    // init_hash_table();

    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\n mutex init failed\n");
        return 1;
    }

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
    printf("%ld: size\n", fileSize);
    close(file);

    struct timespec startTime;
    struct timespec endTime;
    clock_gettime(CLOCK_REALTIME, &startTime);
    // allocate thread array
    // malloc returns a void pointer
    // void pointer can point to any particular type of data type we need
    fileArray = malloc (sizeof(fileInfo) * numberOfThreads);
    // creates an array of p_thread id
    threadArray = malloc (sizeof(pthread_t) * numberOfThreads);

    for (int i = 0; i < numberOfThreads; i++){
        // need this to be a fileArray because it will create different pointers
        // and pass that info instead of changing the info of the same pointer
        fileArray[i].start = (fileSize / numberOfThreads) * i;
        if (i == numberOfThreads - 1){
            fileArray[i].t = t;
            fileArray[i].filename = fileName;
            fileArray[i].size = fileSize - fileArray[i].start;
        }
        else {
            fileArray[i].t = t;
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
    // printf("hashtable size %d", t->size);
    // for (int i = 0; i < t-> size; i++){
    //     struct node *list = t->list[i]; // Array of node list
    //     struct node *temp = list; // list gives a single node 
    //     while(temp){
    //         printf("%s ",temp->key);
    //         printf("%d\n",temp->val);
    //         temp = temp->next;
    //     }
    // }

    clock_gettime(CLOCK_REALTIME, &endTime);
    time_t sec = endTime.tv_sec - startTime.tv_sec;
    long n_sec = endTime.tv_nsec - startTime.tv_nsec;
    if (endTime.tv_nsec < startTime.tv_nsec)
    {
    --sec;
    n_sec = n_sec + 1000000000L;
    }
    printf("Total Time was %ld.%09ld seconds\n", sec, n_sec);
    pthread_mutex_destroy(&lock);
    free(t);
    
    return 0;
}
  