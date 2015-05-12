/* ========================================================================== */
/* File: hashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author: Emily Old
 * Date: May 2015
 *
 * You should include in this file your functionality for the hashtable as
 * described in the assignment and lecture.
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen
#include <stdlib.h>
 #include <stdio.h>

// ---------------- Local includes  e.g., "file.h"
#include "hashtable.h"                       // hashtable functionality

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/*
 * initializeHashTable - sets all elements of each node in the table to NULL
 * 
 * Assumptions:
 *      1. ht has been allocated
 * 
 * Pseudocode:
 *      1. visit every index of the table
 *      2. set each element's data and next pointers to NULL
 */
void initializeHashTable(HashTable *ht){
    /* set data and next pointers to NULL */
    int i;
    for ( i = 0; i < MAX_HASH_SLOT; i++ ){
        ht->table[i] = malloc(sizeof(HashTableNode));
        ht->table[i]->data = NULL;
        ht->table[i]->next = NULL;
    }
}

/*
 * addToHashTable - add a data to a hash table
 * 
 * Assumptions:
 *      1. ht has been allocated
 *      2. user has looked up the hashkey already and knows what they are doing
 * 
 * Psuedocode:
 *      1. find the Jenkins hash code for the given data
 *      2. set the data in the table to the data provided
 */
void addToHashTable(HashTable *ht, void *data, const char *hashKey){
    unsigned long hashVal = JenkinsHash(hashKey, MAX_HASH_SLOT);
    ht->table[hashVal]->data = data;
}

/* lookUp - check if a data is already in a hash table
 * 
 * Assumptions:
 *      1. ht has been allocated
 *     
 * Pseudocode:
 *      1. get the Jenkinds hash code for the provided data
 *      2. check if anything has been hashed with that code yet
 *      3. compare the data to the other that are hashed to the same spot if necessary
 *      4. return 1 if there is a match, and 0 otherwise
 *
 * Returns NULL if there is nothing hashed to that spot in the table yet
 * Otherwise, returns the HashTableNode stored in that spot 
 */
HashTableNode *lookUp(HashTable *ht, const char *hashKey){
    unsigned long hashVal = JenkinsHash(hashKey, MAX_HASH_SLOT);
    /* if nothing has been hashed to that index, the data is not in the
     * table yet, so return NULL */
    if ( ht->table[hashVal]->data == NULL && ht->table[hashVal]->next == NULL){
        return NULL;
    } 

    /* if there is already something hashed to that index, return it */ 
    else{
        return ht->table[hashVal];
    }
}

unsigned long JenkinsHash(const char *str, unsigned long mod)
{
    size_t len = strlen(str);
    unsigned long hash, i;

    for(hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % mod;
}
