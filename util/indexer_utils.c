#include <stdio.h>                           
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "web.h"  						  // webpage functionality
#include "hashtable.h"  	// hashtable functionality
#include "file.h"                         // checking file functionality


/*
 * Update a given WordNode's list of documents
 * If a document with the given document ID already exists, incredment its frequency
 * If the document ID is new, create a document node with a frequency of 1
 * Returns 1 if the list is updated successfully, and 0 otherwise;
 */
int UpdateDocList(WordNode *word, int documentId){
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
		newWord->word = malloc(sizeof(char)*100);
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
				newWord->word = malloc(sizeof(char)*100);
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
 * Frees memory from the Index used to keep track of words and document info 
 */
void FreeIndex(HashTable *index){
	int i;
	WordNode *toFree = NULL;
	for (i=0; i < MAX_HASH_SLOT; i++){
		if(index->table[i] != NULL){
			HashTableNode *htnode = index->table[i];
			if(htnode->data != NULL){
				toFree = htnode->data;
			}
		
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
}

/*
 * Reads in a file that is in the format like the files produced from 
 * the SaveIndexToFile function. A new index is created from the information
 * in the file.
 *
 * The new index is returned. 
 */ 
void ReadFile(char *file, HashTable *ht){
	//HashTable *ht; // new index
	char line[BUFSIZ*3]; // where each line of the file will be stored
	const char delim[2] = " \n"; // delimiter used to parse each line of the file
	char *word; // pointer to current word being read of the file
	long int docCount;
	long int docID; // current document ID being looked at
	long int freq; // current frequency of the word in the current document
	FILE *fp; // pointer to file stream being read
	char *token;// section of the parsed line being processed
	
	/* make sure the file is opened properly */
	fp = fopen(file, "r");
	if(fp == NULL){
		printf("Error opening file: %s", file);
		exit(EXIT_FAILURE);
	}

	else{
		/* set up the index to store data */

		int counter; // token counter
		char *ptr;	 // pointer for strtol 
		/* read the file line by line */
		while (fgets(line, BUFSIZ*3, fp) != NULL){

			/* parse the current line and store the word as the first token */
			token = strtok(line, delim);
			word = token;
			
			int i;
			for(i=0; i < strlen(word); i++){
				if (!isalpha(word[i])){
					printf("%s is not formatted correctly. Every line must begin with a word", file);
					exit(EXIT_FAILURE);
				}
			}
			counter = 1;

			/* look at each token of the line in order */
			while( token != NULL){
				

				/* get the doc count of the current word */
				if (counter == 2){
					docCount = strtol(token, &ptr, 10);
					/* make sure it is an integer */
					if(strlen(ptr) != 0 && strcmp(ptr, "\n") != 0){
						printf("%s is not formatted correctly. Every character after the word must be an integer.", file);
						exit(EXIT_FAILURE);
					}
				}

				/* if the token is in the position of a document ID, store it */
				else if (counter > 2 && (counter % 2) == 1){
					docID = strtol(token, &ptr, 10);
					/* make sure it is an integer */
					if(strlen(ptr) != 0 && strcmp(ptr, "\n") != 0){
						printf("%s is not formatted correctly. Every character after the word must be an integer.", file);
						exit(EXIT_FAILURE);
					}
				}

				/* if the token is in the position of a frequency, store it and update the index */
				else if( counter > 2 && (counter % 2) == 0){
					freq = strtol(token, &ptr, 10);
					/* make sure it is an integer */
					if(strlen(ptr) != 0 && strcmp(ptr, "\n") != 0){
						printf("%s is not formatted correctly. Every character int a line after the word must be an integer", file);
						exit(EXIT_FAILURE);
					}

					/* update the index for the current word, document Id, and frequency */
					long int i = 1;
					while(i<=freq){
						UpdateIndex(word, (int)docID, ht);
						i++;
					}
				}
				counter++;
				token = strtok(NULL, delim);
			}
			/* make sure the file is formatted correctly thus far */
			if ((counter % 2) == 0){
				printf("%s is not formatted correctly. Every line should have an even number of tokens.", file);
				exit(EXIT_FAILURE);
			}	
			else if (counter == 3){
				printf("%s is not formatted correctly. Every line should include at least one docID-freq pair.", file);
				exit(EXIT_FAILURE);
			}
			else if((counter - 3)/2 != docCount){
				printf("%s is not formatted correctly. Each word should have the same number of docID-freq pairs as there are documents.", file);
				exit(EXIT_FAILURE);
			}
		}
	}
	fclose(fp);
	
}