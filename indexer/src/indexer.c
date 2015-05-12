/* ========================================================================== */
/* File: indexer.c - Tiny Search Engine indexer
 *
 * Author: Emily Old
 * Date: May 2015
 *
 * Input without testing: [TARGET_DIRECTORY] [RESULTS FILENAME]
 * Input with testing: [TARGET_DIRECTORY] [RESULTS FILENAME] [RESULTS FILENAME] [REWRITTEN FILENAME]
 *
 * Command line options: None
 *
 * Output without testing: The results file is updated with each line in the format:
 *						   [word] [# of documents containing the word] [doc ID] [freqency in doc]... 
 *
 * Special Considerations: Assumes the files and directory provided already exist.
 *						   Assumes the files in the directory are named with unique integers
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
// ---------------- Local includes  e.g., "file.h"
#include "web.h"  						  // webpage functionality
#include "../../crawler/src/hashtable.h"  // hashtable functionality
#include "file.h"                         // checking file functionality

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types


typedef struct DocumentNode {
  struct DocumentNode *next;         // pointer to the next member of the list.
  int doc_id;                        // document identifier
  int freq;                          // number of occurrences of the word
} DocumentNode;

typedef struct WordNode {
  struct WordNode *next;            // pointer to the next word (for collisions)
  char *word;                       // the word
  DocumentNode *page;               // pointer to the first element of the page list.
} WordNode;

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */



char *LoadDocument(char *fileName){
	int lineNum = 1;
	FILE *fp;
	
	char line[BUFSIZ];

    fp = fopen(fileName, "r");
    if (fp == NULL){
    	printf("Error opening file %s", fileName);
    }

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    char *html = malloc(sizeof(char)*len + 1);
    fseek(fp, 0, SEEK_SET);
    while ( fgets(line, BUFSIZ, fp) != NULL){
		if (lineNum > 2){
			strcat(html, line);
		}
		lineNum ++;
	}

    fclose(fp);
    
    return html;
}

static int UpdateDocList(WordNode *word, int documentId){
	DocumentNode *currentDoc = word->page;

	if(currentDoc->doc_id == documentId){
		currentDoc->freq++;
		return 1;
	}
	else{
		while(currentDoc->next != NULL){
			if (currentDoc->doc_id == documentId){
				currentDoc->freq++;
				return 1;
			}
			else{
				currentDoc = currentDoc->next;
			}
					
		}
		if(currentDoc->doc_id == documentId){
			currentDoc->freq++;
			return 1;
		}
		else{
			currentDoc->next = malloc(sizeof(DocumentNode));
			DocumentNode *newDoc = currentDoc->next;
			newDoc->doc_id = documentId;
			newDoc->freq = 1;
			newDoc->next = NULL;
			return 1;
		}
	}
	return 0;	
}


int UpdateIndex(char *word, int documentId, HashTable *index){
	if (lookUp(index, word) == NULL){
		WordNode *newWord = malloc(sizeof(WordNode));
		newWord->next = NULL;
		newWord->word = malloc(sizeof(char)*50);
		strcpy(newWord->word, word);
		newWord->page = malloc(sizeof(DocumentNode));
		newWord->page->next = NULL;
		newWord->page->doc_id = documentId;
		newWord->page->freq = 1;
		addToHashTable(index, newWord, newWord->word);
		return 1;
	}

	else{
		HashTableNode *htnode = lookUp(index, word);
		WordNode *currentWord = htnode->data;
		if(strcmp(currentWord->word, word) == 0){
			return UpdateDocList(currentWord, documentId);
		}

		else{
			while(currentWord->next != NULL){
				if(strcmp(currentWord->word, word) == 0){
					return UpdateDocList(currentWord, documentId);
				}
				else{
					currentWord = currentWord->next;
				}

			}
			
			if(strcmp(currentWord->word, word) == 0){
				UpdateDocList(currentWord, documentId);
			}	
			else{
				currentWord->next= malloc(sizeof(WordNode));
				WordNode *newWord = currentWord->next;
				newWord->word = malloc(sizeof(char)*50);
				strcpy(newWord->word, word);
				newWord->next = NULL;

				newWord->page = malloc(sizeof(DocumentNode));
				DocumentNode *newDoc = newWord->page;
				newDoc->doc_id = documentId;
				newDoc->freq = 1;
				newDoc->next = NULL;
				return 1;
			}			
		}
		
	}
	return 0;
}

long int GetDocumentId(char *fileName){
	char *ptr;
	long int docID = strtol(fileName, &ptr, 10);
	return docID;
}

void FreeIndex(HashTable *index){
	int i;
	for (i=0; i < MAX_HASH_SLOT; i++){
		HashTableNode *htnode = index->table[i];
		WordNode *toFree = htnode->data;
		while(toFree != NULL){
			WordNode *tmpword = toFree;
			DocumentNode *freeDoc = toFree->page;
			while(freeDoc != NULL){
				DocumentNode* tmpdoc = freeDoc;
				freeDoc = freeDoc->next;
				free(tmpdoc);
			}
			toFree = toFree->next;
			free(tmpword->word);
			free(tmpword);
		}
		free(htnode);
	}
}

int DocListLength(DocumentNode *doc){
	int docCount = 0;
	while(doc != NULL){
		docCount++;
		doc = doc->next;
	}
	return docCount;
}

int SaveIndexToFile(HashTable *index, char *filePath){
	int status = 1;
	int i;
	char numFiles[5];
	char genString[10];
	FILE *fp;
	fp = fopen(filePath, "w");

	if(fp == NULL){
		printf("Error opening file: %s", filePath);
		status = 0;
	}
	else{
		for (i = 0; i < MAX_HASH_SLOT; i++){
		
			HashTableNode *htnode = index->table[i];
			if(htnode->data != NULL){
				WordNode *currentWord = htnode->data;
			
				while(currentWord != NULL){
					DocumentNode *currentDoc = currentWord->page;
					int numDocs = DocListLength(currentDoc);
					sprintf(numFiles, " %d ", numDocs);
					fputs(currentWord->word, fp);
					fputs(numFiles, fp);
					while(currentDoc != NULL){
						if(currentDoc->next == NULL){
							sprintf(genString, "%d %d\n", currentDoc->doc_id, currentDoc->freq);
						}
						else{
							sprintf(genString, "%d %d ", currentDoc->doc_id, currentDoc->freq);
						}

						fputs(genString, fp);
						currentDoc = currentDoc->next;
					}
					currentWord = currentWord->next;
				}
			}
		}
	}
	
	fclose(fp);
	return status;
}

HashTable *ReadFile(char *file){
	HashTable *ht;
	char line[BUFSIZ];
	const char delim[2] = " ";
	char *word;
	long int docID;
	long int freq;
	FILE *fp;
	char *token;
	fp = fopen(file, "r");


	if(fp == NULL){
		printf("Error opening file: %s", file);
		exit(EXIT_FAILURE);
	}
	else{
		ht = malloc(sizeof(HashTable));
		initializeHashTable(ht);
		int counter;
		char *ptr;
		while (fgets(line, BUFSIZ, fp) != NULL){
			token = strtok(line, delim);
			word = token;
			counter = 1;
			while( token != NULL){
				if (counter == 1 || counter == 2){
					counter++;
				}
				else if (counter > 2 && (counter % 2) == 1){
					docID = strtol(token, &ptr, 10);
					counter ++;
				}
				else if( counter > 2 && (counter % 2) == 0){
					freq = strtol(token, &ptr, 10);
					long int i = 1;
					while(i<=freq){
						UpdateIndex(word, (int)docID, ht);
						i++;
					}
					counter++;
				}
				token = strtok(NULL, delim);
			}
		}
	}
	fclose(fp);
	return ht;
}

int main(int argc, char *argv[]){
	HashTable *Index; 
	
	int docID;
	int pos;
	char *word;
	char *doc;
	char **filenames;
	char *fileName = malloc(sizeof(char)*50);
	int i;

	if (argc != 3 && argc != 5){
		printf("Indexer requires 3 arguments, or 5 for testing");
		exit(EXIT_FAILURE);
	}
	if ( !IsDir(argv[1]) ){
		printf("%s is not a valid directory.", argv[1]);
		exit(EXIT_FAILURE);
	}

	if ( IsFile(argv[2]) ){
		printf("The file %s already exists. Don't overwrite it.", argv[2]);
		exit(EXIT_FAILURE);
	}

	if ( argc == 5 && IsFile(argv[3]) ){
		printf("The file %s already exists. Don't overwrite it.", argv[3]);
		exit(EXIT_FAILURE);
	}

	if ( argc == 5 && IsFile(argv[4]) ){
		printf("The file %s already exists. Don't overwrite it.", argv[4]);
		exit(EXIT_FAILURE);
	}

	Index = malloc(sizeof(HashTable));
	initializeHashTable(Index);
	int numFiles = GetFilenamesInDir(argv[1], &filenames);
	if(numFiles < 0){
		printf("Couldn't get files from directory %s", argv[1]);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < numFiles; i++){
		int length = strlen(argv[1]);
		if(strcmp(&argv[1][length-1], "/") != 0){
			sprintf(fileName, "%s/%s", argv[1], filenames[i]);
		}
		else{
			sprintf(fileName, "%s%s", argv[1], filenames[i]);
		}
		doc = LoadDocument(fileName);
		docID = (int)GetDocumentId(filenames[i]);
		free(filenames[i]);

		pos = 0;
		while ((pos = GetNextWord(doc, pos, &word)) > 0){
			NormalizeWord(word);
			UpdateIndex(word, docID, Index);
			free(word);
		}
		free(doc);
	}

	SaveIndexToFile(Index, argv[2]);
	free(filenames);
	free(fileName);
	FreeIndex(Index);
	free(Index);

	if(argc == 5){
		HashTable *newht = ReadFile(argv[3]);
		SaveIndexToFile(newht, argv[4]);
		FreeIndex(newht);
		free(newht);
	}

	return 1;
}

