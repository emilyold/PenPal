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
#include "common.h"                          // common functionality
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
 *      2. set each element's url and next pointers to NULL
 */
void initializeHashTable(HashTable *ht){
    /* set url and next pointers to NULL */
    int i;
    for ( i = 0; i < MAX_HASH_SLOT; i++ ){
        ht->table[i] = malloc(sizeof(HashTableNode));
        ht->table[i]->url = NULL;
        ht->table[i]->next = NULL;
    }
}

/*
 * addToHashTable - add a url to a hash table
 * 
 * Assumptions:
 *      1. ht has been allocated
 * 
 * Psuedocode:
 *      1. find the Jenkins hash code for the given url
 *      2. check wither anything has been hashed with that code yet
 *      3. add the url to the hash table in a HashTableNode structure
 */
void addToHashTable(HashTable *ht, const char *url){
    unsigned long hashVal = JenkinsHash(url, MAX_HASH_SLOT);
    /* if nothing had been hashed to that index yet, create a new HashTableNode
     * and with the provided url in that spot */
    if ( ht->table[hashVal]->url == NULL ){
        //printf("\n%ld good", hashVal);
        ht->table[hashVal]->url = malloc(sizeof(char) * 1000);
        strcpy(ht->table[hashVal]->url, url);
    }
    /* if there is already a HashTableNode hashed to that index, traverse
     * the list of nodes hashed there, and add a new one with the provided url 
     * at the end of the list */
    else{
        //printf("\n%ld here", hashVal);
        /* get ready to traverse, or insert */
        //ht->table[hashVal]->next = malloc(sizeof(HashTableNode));
        HashTableNode *current = malloc(sizeof(HashTableNode));
        current = ht->table[hashVal];
       
        /* traverse the list */
        while ( current->next != NULL ){
            current = current->next;
        }

        /* insert the new node at the end of the list */
        current->next = malloc(sizeof(HashTableNode));
        HashTableNode *newNode = current->next;
        newNode->url = malloc(sizeof(char) * 1000);
        strcpy(newNode->url, url);
        newNode->next = NULL;       
    }
}

/* lookUpURL - check if a url is already in a hash table
 * 
 * Assumptions:
 *      1. ht has been allocated
 *     
 * Pseudocode:
 *      1. get the Jenkinds hash code for the provided url
 *      2. check if anything has been hashed with that code yet
 *      3. compare the url to the other that are hashed to the same spot if necessary
 *      4. return 1 if there is a match, and 0 otherwise
 */
int lookUpURL(HashTable *ht, const char *url){
    unsigned long hashVal = JenkinsHash(url, MAX_HASH_SLOT);
    /* if nothing has been hashed to that index, the url is not in the
     * table yet, so return 0 */
    if ( ht->table[hashVal]->url == NULL && ht->table[hashVal]->next == NULL){
        //printf("here");
        return 0;
    } 

    /* if there is already something hashed to that index, traverse down
     * the list of nodes and compare each node's urls to the provided one */   
    else{
        HashTableNode *current = ht->table[hashVal];

        /* return 1 if there is a matching url */
        if (strcmp(url, current->url) == 0){
            return 1;
        }
        while (current->next != NULL){
            if(strcmp(url, current->next->url) == 0){
                return 1;
            }
            else{
                current = current->next;
            }
        }
    }
    return 0;
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
