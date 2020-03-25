// Used hashtable implementation from 
// https://github.com/engineer-man/youtube/blob/master/077/hashtable.c
// and https://www.youtube.com/watch?v=2Ti5yvumFTU

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


#define Table_Size 1000
#define Max_Key_Size 256


// creating the structure of each element in hashmap
typedef struct {
    char key[Max_Key_Size];
    int frequency;
} word;

word * hash_table[Table_Size];

// sets all the values of the hashmap to NULL
void init_hash_table(){
    for (int i = 0; i < Table_Size; i++){
        hash_table[i] = NULL;
    }
}

void print_hashtable(){
    for (int i = 0; i < Table_Size; i++){
        if (hash_table[i] == NULL){
            printf("\t%i\t---\n", i);
        }
        else{
            printf("\t%i\t%s\t%d\t\n", i, hash_table[i]->key, hash_table[i]->frequency);
        }
    }
}

// this function takes the key, generates a hashvalue, and returns that hashvalue
unsigned int hash(const char *key) {
    unsigned long int hash_value = 0;
    unsigned int i;
    unsigned int key_len = strlen(key);

    // do several rounds of multiplication
    for (i = 0; i < key_len; ++i) {
        hash_value = hash_value * 37 + key[i];
    }

    // make sure value is 0 <= value < TABLE_SIZE
    hash_value = hash_value % Table_Size;

    return hash_value;
}

bool hash_table_insert(word *p){
    if (p == NULL){
        return false;
    }
    int index = hash(p->key);
    if (hash_table[index] != NULL){
        return false;
    }
    p->frequency = 1;
    hash_table[index] = p;
    return true;
}


// check if a key is alreay in the table
word *hash_table_lookup(char *key){
    int index = hash(key);
    // first check if something is there
    // strncmp checks to see if the keys match
    if (hash_table[index] != NULL && strncmp(hash_table[index]->key, key, Table_Size)==0){
        return hash_table[index];
    }
    else{
        return NULL;
    }
}


int main(){
    // initializes the hashtable
    init_hash_table();
    word hello = {.key="hello", .frequency=0};
    hash_table_insert(&hello);

    word *tmp = hash_table_lookup("hello");
    if (tmp == NULL){
        printf("Not found!\n");
    }
    else{
        tmp->frequency += 1;
        printf("Found %s\t%d.\n", tmp->key, tmp->frequency);
    }

    // print_hashtable();
    return 0;
}