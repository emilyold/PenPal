#ifndef INDEXER_UTILS_H
#define INDEIXER_UTILS_H

#include "hashtable.h"
#include "web.h"
#include "file.h"

int UpdateDocList(WordNode *word, int documentId);

int UpdateIndex(char *word, int documentId, HashTable *index);

void FreeIndex(HashTable *index);

void ReadFile(char *file, HashTable *ht);

#endif // INDEXER_UTILS_H