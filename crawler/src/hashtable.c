/* ========================================================================== */
/* File: hashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author:
 * Date:
 *
 * You should include in this file your functionality for the hashtable as
 * described in the assignment and lecture.
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen
#include <stdlib.h>

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "hashtable.h"                       // hashtable functionality

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

void initializeHashTable(HashTable *ht){
    int i;
    for ( i = 0; i < MAX_HASH_SLOT; i++ ){
        ht->table[i] = malloc(sizeof(HashTableNode));
        ht->table[i]->url = NULL;
        ht->table[i]->next = NULL;
    }
}

void addToHashTable(HashTable *ht, const char *url){
    unsigned long hashVal = JenkinsHash(node->url, 5);
    if ( ht->table[hashVal] == NULL ){
        strcpy(ht->table[hashVal]->url, node->url);
    }
    else{
        HashTableNode *current = ht->table[hashVal]->next;
        while ( current != NULL ){
            current = current->next;
        }
        strcpy(current->url, url);
        current->next
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
