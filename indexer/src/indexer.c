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
 * Special Considerations: 
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
// ---------------- Local includes  e.g., "file.h"
#include "web.h"  						  // webpage functionality
#include "hashtable.h"  // hashtable functionality
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

/*
* LoadDocument -- reads a file into a string which is returned
*/
char *LoadDocument(char *fileName){
	int lineNum = 1;
	FILE *fp;
	
	//char line[BUFSIZ];

    fp = fopen(fileName, "r");
    if (fp == NULL){
    	printf("Error opening file %s", fileName);
    }

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    char *html = malloc(len);
    fseek(fp, 0, SEEK_SET);
    int ch;
    int i = 0;
    while ( (ch = fgetc(fp)) != EOF){
		if (lineNum < 3 && ch == '\n'){
			lineNum++;
		}
		else if (lineNum >= 3){
			html[i] = ch;
			i++;
		}
	}

    fclose(fp);
    
    return html;
}

/*
 * Update a given WordNode's list of documents
 * If a document with the given document ID already exists, incredment its frequency
 * If the document ID is new, create a document node with a frequency of 1
 * Returns 1 if the list is updated successfully, and 0 otherwise;
 */
static int UpdateDocList(WordNode *word, int documentId){
	DocumentNode *currentDoc = word->page;

	/* if the first doc in the list is the same as the provided doc, update its frequency */
	if(currentDoc->doc_id == documentId){
		currentDoc->freq++;
		return 1;
	}
	/* otherwise traverse down the document list to see if the word has been discovered in the document before */
	else{
		while(currentDoc->next != NULL){
			/* if the document is already in the list, update its frequency */
			if (currentDoc->doc_id == documentId){
				currentDoc->freq++;
				return 1;
			}
			else{
				currentDoc = currentDoc->next;
			}
					
		}

		/* if the document Id matches the last doc in the list, update its frequency */
		if(currentDoc->doc_id == documentId){
			currentDoc->freq++;
			return 1;
		}
		/* otherwise, this is the first occurence of the word in this document
		   so append a new document node to the end of the list and set its frequency to 1 */
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

/*
 * Updates the provided hashtable given a word, and document ID
 * Returns 1 if update is successful, and 0 otherwise
 */
int UpdateIndex(char *word, int documentId, HashTable *index){
	/* if the word gets hashed to an empty sport in the table
	   create a new WordNode and the first DocumentNode in the list of
	   docs containing the word */
	if (lookUp(index, word) == NULL){
		/* new WordNode */
		WordNode *newWord = malloc(sizeof(WordNode));
		newWord->next = NULL;
		newWord->word = malloc(sizeof(char)*50);
		strcpy(newWord->word, word);

		/* new DocumentNode */
		newWord->page = malloc(sizeof(DocumentNode));
		newWord->page->next = NULL;
		newWord->page->doc_id = documentId;
		newWord->page->freq = 1;
		addToHashTable(index, newWord, newWord->word);
		return 1;
	}

	/* if the word is hashed to a spot that is already filled in 
	   the hashtable, update the list of WordNodes with the given word
	   and document ID */
	else{
		HashTableNode *htnode = lookUp(index, word);
		WordNode *currentWord = htnode->data;

		/* if the word matches the first word in the list, update its document list */
		if(strcmp(currentWord->word, word) == 0){
			return UpdateDocList(currentWord, documentId);
		}

		/* otherwise traverse down the WordNode list to see if the word has been discovered before */
		else{
			while(currentWord->next != NULL){
				/* if the word has been previously discovered, update its document list */
				if(strcmp(currentWord->word, word) == 0){
					return UpdateDocList(currentWord, documentId);
				}
				else{
					currentWord = currentWord->next;
				}

			}
			
			/* if the word is the same as the last word in the list, update its document list */
			if(strcmp(currentWord->word, word) == 0){
				UpdateDocList(currentWord, documentId);
			}
			/* otherwise create a new WordNode, and the first DocumentNode in the document list */	
			else{
				/* initialize WordNode */
				currentWord->next= malloc(sizeof(WordNode));
				WordNode *newWord = currentWord->next;
				newWord->word = malloc(sizeof(char)*50);
				strcpy(newWord->word, word);
				newWord->next = NULL;

				/* initialize DocumentNode */
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

/*
 * Convert's a file name to an integer document ID
 * Returns the document ID if the filename is an integer with no other characters
 * Returns -1 if the documentID is not a single integer
 */
long int GetDocumentId(char *fileName){
	char *ptr;
	long int docID = strtol(fileName, &ptr, 10);
	/* if the filename was successfully converted to an integer, return it */
	if(strlen(ptr) == 0){
		return docID;
	}
	/* otherwise return -1 */
	else{
		return -1;
	}
	
}

/* 
 * Frees memory from the Index used to keep track of words and document info 
 */
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

/*
 * Returns the length of a list of DocumentNodes starting from 
 * the node the function is passed.
 */
int DocListLength(DocumentNode *doc){
	/* traverse the list, keeping count of the number of nodes */
	int docCount = 0;
	while(doc != NULL){
		docCount++;
		doc = doc->next;
	}
	return docCount;
}

/*
 * Write the information stored in an index to the specified file.
 * Each line of the resulting file will begin with a word that was discovered
 * followed by the number of documents the word occured in. The preceding of pattern of numbers
 * will be: [docID] [frequency of word in that doc] [docId] [frequency of word in that doc]...
 *
 * Returns 1 if successful, and 0 otherwise.
 */
int SaveIndexToFile(HashTable *index, char *filePath){
	int status = 1;
	int i;
	char numFiles[5];
	char genString[10];
	FILE *fp;

	/* make sure the file opened successfully */
	fp = fopen(filePath, "w");
	if(fp == NULL){
		printf("Error opening file: %s", filePath);
		status = 0;
	}
	else{
		/* iterate through each index in the hashtable provided */
		for (i = 0; i < MAX_HASH_SLOT; i++){

			/* get the WordNode stored in the hashtable at index i if there is one */
			HashTableNode *htnode = index->table[i];
			if(htnode->data != NULL){
				WordNode *currentWord = htnode->data;
				
				/* traverse down the list of WordNodes hashed to that index */
				while(currentWord != NULL){
					/* find the number of files the word occurs in */
					DocumentNode *currentDoc = currentWord->page;
					int numDocs = DocListLength(currentDoc);
					sprintf(numFiles, " %d ", numDocs);

					/* write the first word, and the number of documents it was found in to the file */
					fputs(currentWord->word, fp);
					fputs(numFiles, fp);

					/* traverse down the list of documents the word was discovered in */
					while(currentDoc != NULL){
						/* create the appropriate string to be written to the file */
						if(currentDoc->next == NULL){
							sprintf(genString, "%d %d\n", currentDoc->doc_id, currentDoc->freq);
						}
						else{
							sprintf(genString, "%d %d ", currentDoc->doc_id, currentDoc->freq);
						}

						/* write the string containing a document ID and the frequency in the doc to the file */
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

/*
 * Reads in a file that is in the format like the files produced from 
 * the SaveIndexToFile function. A new index is created from the information
 * in the file.
 *
 * The new index is returned. 
 */ 
HashTable *ReadFile(char *file){
	HashTable *ht; // new index
	char line[BUFSIZ]; // where each line of the file will be stored
	const char delim[2] = " "; // delimiter used to parse each line of the file
	char *word; // pointer to current word being read of the file
	long int docID; // current document ID being looked at
	long int freq; // current frequency of the word in the current document
	FILE *fp; // pointer to file stream being read
	char *token; // section of the parsed line being processed

	/* make sure the file is opened properly */
	fp = fopen(file, "r");
	if(fp == NULL){
		printf("Error opening file: %s", file);
		exit(EXIT_FAILURE);
	}


	else{
		/* set up the index to store data */
		ht = malloc(sizeof(HashTable));
		initializeHashTable(ht);

		int counter; // token counter
		char *ptr;	 // pointer for strtol 
		/* read the file line by line */
		while (fgets(line, BUFSIZ, fp) != NULL){
			/* parse the current line and store the word as the first token */
			token = strtok(line, delim);
			word = token;
			counter = 1;

			/* look at each token of the line in order */
			while( token != NULL){
				/* ignore the word and the number of documents it is found in */
				if (counter == 1 || counter == 2){
					counter++;
				}

				/* if the token is in the position of a document ID, store it */
				else if (counter > 2 && (counter % 2) == 1){
					docID = strtol(token, &ptr, 10);
					counter ++;
				}

				/* if the token is in the position of a frequency, store it and update the index */
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

	/* argument checking */
	if (argc != 3 && argc != 5){
		printf("Indexer requires 3 arguments, or 5 for testing.\n");
		exit(EXIT_FAILURE);
	}
	if ( !IsDir(argv[1]) ){
		printf("%s is not a valid directory.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if ( IsFile(argv[2]) ){
		printf("The file %s already exists. Don't overwrite it.\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	if ( argc == 5 && IsFile(argv[3]) ){
		printf("The file %s already exists. Don't overwrite it.\n", argv[3]);
		exit(EXIT_FAILURE);
	}

	if ( argc == 5 && IsFile(argv[4]) ){
		printf("The file %s already exists. Don't overwrite it.\n", argv[4]);
		exit(EXIT_FAILURE);
	}

	if (argc == 5 && strcmp(argv[2], argv[4]) == 0){
		printf("The results file and the rewritten file cannot be the same.\n");
		exit(EXIT_FAILURE);
	}

	if (argc == 5 && strcmp(argv[2], argv[3]) != 0){
		printf("The second and third argument should be the same file for proper testing.\n");
		exit(EXIT_FAILURE);
	}

	/* if all the arguments chech out, get ready to build the index */
	Index = malloc(sizeof(HashTable));
	initializeHashTable(Index);

	/* make sure files were able to be gound in the directory */
	int numFiles = GetFilenamesInDir(argv[1], &filenames);
	if(numFiles < 0){
		printf("Couldn't get files from directory %s", argv[1]);
		exit(EXIT_FAILURE);
	}

	/* iterate through the files in the directory */
	for (i = 0; i < numFiles; i++){

		/* create a proper path to the file in the directory */
		int length = strlen(argv[1]);
		if(strcmp(&argv[1][length-1], "/") != 0){
			sprintf(fileName, "%s/%s", argv[1], filenames[i]);
		}
		else{
			sprintf(fileName, "%s%s", argv[1], filenames[i]);
		}

		/* store the contents of the file in a string */
		doc = LoadDocument(fileName);

		/* if the file name is not an integer, the data from that file will not be included in the index*/
		docID = (int)GetDocumentId(filenames[i]);
		if(docID == -1){
			printf("All files must be named with unique integers. The file %s has an invalid name and has not been processed. \n", filenames[i]);
		}
		else{
			/* normalize and add each word found in the document to the index */
			pos = 0;
			while ((pos = GetNextWord(doc, pos, &word)) > 0){
				NormalizeWord(word);
				//printf("%s\n", word);
				UpdateIndex(word, docID, Index);
				free(word);
			}
		}
		
		free(filenames[i]);
		free(doc);
	}

	/* write the data stored in the index to the file provided */
	SaveIndexToFile(Index, argv[2]);
	free(filenames);
	free(fileName);
	FreeIndex(Index);
	free(Index);

	/* if the user is performing testing... */
	if(argc == 5){
		/* create a new index from the file that was just written, and save the data to a different file */
		HashTable *newht = ReadFile(argv[3]);
		SaveIndexToFile(newht, argv[4]);
		FreeIndex(newht);
		free(newht);
	}

	return 1;
}

