/* ========================================================================== */
/* File: hashtable.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * This file contains the definitions for a hashtable of urls.
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality

// ---------------- Constants
#define MAX_HASH_SLOT 1000                  // number of "buckets"

// ---------------- Structures/Types

typedef struct HashTableNode {
    char *url;                               // url previously seen
    struct HashTableNode *next;              // pointer to next node
} HashTableNode;

typedef struct HashTable {
    HashTableNode *table[MAX_HASH_SLOT];     // actual hashtable
} HashTable;

// ---------------- Public Variables

// ---------------- Prototypes/Macros

/*
 * jenkins_hash - Bob Jenkins' one_at_a_time hash function
 * @str: char buffer to hash
 * @mod: desired hash modulus
 *
 * Returns hash(str) % mod. Depends on str being null terminated.
 * Implementation details can be found at:
 *     http://www.burtleburtle.net/bob/hash/doobs.html
 */
unsigned long JenkinsHash(const char *str, unsigned long mod);

/*
* initializeHashTable
* @ht: HashTable struct that has been allocated
* 
* Returns void
*/
void initializeHashTable(HashTable *ht);

/*
 * addToHashTable
 * @ht: HashTable struct that has been allocated
 * @url: char buffer to be hashed
 *
 * Returns void
 */
void addToHashTable(HashTable *ht, const char *url);

/*
 * @ht: HashTable struct that has been allocated
 * @url: char buffer that needs to be looked up
 *
 * returns voide
 */
int lookUpURL(HashTable *ht, const char *url);

void freeHashTable(HashTable *ht);

#endif // HASHTABLE_H
