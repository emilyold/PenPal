#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "../../util/web.h"
#include "../../util/hashtable.h"
#include "../../util/file.h"
#include "../../util/indexer_utils.h"

/*
 * CopyListData -- makes a copy of a singly linked list of DocumentNodes
 *
 * A new list is malloced and created to hold the exact same data as
 * the list for which the provided DocumentNode is the head
 */
DocumentNode *CopyListData(DocumentNode *head){
	/* check if there is anything to copy */
	if ( head == NULL){
		return NULL;
	}

	DocumentNode *copy = malloc(sizeof(DocumentNode)); // head of the new list
	DocumentNode *temp = NULL;	// iterator for new list
	DocumentNode *curr = head;	// iterator for old list
	int counter = 1;

	/* traverse down the list */
	while(curr != NULL){
		/* if the node being looked at is the head, initialize the data in the head of the new list */
		if(counter == 1){
			copy->doc_id = curr->doc_id;
			copy->freq = curr->freq;
			copy->next = NULL;
			temp = copy;
			counter++;
		}
		/* otherwise keep adding to the new list */
		else{
			temp->next = malloc(sizeof(DocumentNode));
			temp = temp->next;
			temp->doc_id = curr->doc_id;
			temp->freq = curr->freq;
			temp->next = NULL;
		}
		curr = curr->next;
	}
	return copy;
}

/*
 * GetDocList -- returns the list of DocumentNodes associated with the provided 
 * 				 word stored in the provided HashTable
 * 
 * Looks up the given word in the given HashTable and returns
 * the list of DocumentNodes associated with it if it exists
 */
DocumentNode *GetDocList(HashTable *ht, char *word){
	/* check if there is anything in the HashTable where this word would be hashed */
	if (lookUp(ht, word) == NULL){
		return NULL;
	}
	else{
		HashTableNode *htnode = lookUp(ht, word);
		WordNode *currentWord = htnode->data;

		/* if the word matches the first word in the list, return its document list */
		if(strcmp(currentWord->word, word) == 0){
			return CopyListData(currentWord->page);
		}

		/* otherwise traverse down the WordNode list to see if the word has been discovered before */
		else{
			while(currentWord->next != NULL){
				/* if the word has been previously discovered, return its document list */
				if(strcmp(currentWord->word, word) == 0){
					return CopyListData(currentWord->page);
				}
				else{
					currentWord = currentWord->next;
				}
			}
			
			/* if the word is the same as the last word in the list, return its document list */
			if(strcmp(currentWord->word, word) == 0){
				return CopyListData(currentWord->page);
			}
			/* otherwise, the word is not in the HashTable */	
			else{
				return NULL;
			}
		}
	}
}

/*
 * ANDUpdate -- performs the AND operation on two Document Node lists
 *
 * Returns an updated new list containing the intersection of the DocumentNodes
 * found in each list. The DocumentNodes with the same docIDs in the two lists
 * will be represented once in the new list in a DocumentNode with the same docID,
 * and a combined frequency.
 */
DocumentNode *ANDUpdate(DocumentNode *first, DocumentNode *second){
	//printf("performing ANDUpdate\n");
	DocumentNode *currentOuter = first;
	DocumentNode *match = malloc(sizeof(DocumentNode)); // new list to be returned
	DocumentNode *temp = NULL; // iterator for new list
	int matches = 0;

	/* check if either of the lists have data */
	if(first == NULL && second == NULL){
		free(match);
		return NULL;
	}
	/* check if only the first list contains data */
	else if (first != NULL && second == NULL){
		free(match);
		return NULL;
	}
	/* check if only the second list contains data */
	else if (first == NULL && second != NULL){
		free(match);
		return NULL;
	}
	else{
		/* compare each element of the first list with each element of the second */
		while(currentOuter != NULL){
			DocumentNode *currentInner = second;
			while(currentInner != NULL){
				//printf("comparing document %d and document %d\n", currentOuter->doc_id, currentInner->doc_id);
				/* if two elements represent the same document, add the document data
					to the new list with an updated frequency */
				if (currentOuter->doc_id == currentInner->doc_id){

					matches++;
					/* if this is the first match to be found, initialize the head of the new list */
					if(matches == 1){
						//printf("this is the first match\n");
						match->doc_id = currentOuter->doc_id;
						match->freq = currentOuter->freq + currentInner->freq;
						match->next = NULL;
						temp = match;
						//printf("new node docID: %d\n", temp->doc_id);
					}
					/* otherwise just append to the list */
					else {
						temp->next = malloc(sizeof(DocumentNode));
						temp = temp->next;
						temp->doc_id = currentOuter->doc_id;
						temp->freq = currentOuter->freq + currentInner->freq;
						temp->next = NULL;
						//printf("new node docID: %d\n", temp->doc_id);
					}
				
				}
				currentInner = currentInner->next;
			}
			currentOuter = currentOuter->next;
		}
		if (matches == 0){
			free(match);
			return NULL;
		}
		else{
			return match;
		}
		
	}
	
}

/*
 * ORUpdate -- performs the OR operator on two lists of DocumentNodes
 * 
 * Returns an updated new list containing the union of the DocumentNodes
 * found in each list. The DocumentNodes with the same docIDs in the two lists
 * will be represented once in the new list in a DocumentNode with the same docID,
 * and a combined frequency. The data from the non-matched elements will be copied to 
 * new elements in the new list.
 */
DocumentNode *ORUpdate(DocumentNode *first, DocumentNode *second){
	DocumentNode *currentOuter = first;
	DocumentNode *currentInner = second;
	DocumentNode *combo = malloc(sizeof(DocumentNode)); // new list
	DocumentNode *temp = NULL; // iterator for new list
	
	/* check if both lists are empty */
	if(first == NULL && second == NULL){
		free(combo);
		return NULL;
	}
	/* check if only the first list has elements */
	else if (first != NULL && second == NULL){
		free(combo);
		return CopyListData(first);
	}
	/* check if only the second list has elements */
	else if (first == NULL && second != NULL){
		free(combo);
		return CopyListData(second);
	}
	else{
		/* if neither list is empty, compare each element of the first with each element of the second */
		while(currentOuter != NULL){
			int matches = 0;
			currentInner = second;
			while(currentInner != NULL){

				/* if the two elements represent the same document, add an element 
					to the new list with an updated frequency */
				if (currentOuter->doc_id == currentInner->doc_id){
					matches++;
					/* if the new list has not been added to yet, initialize the head */
					if (temp == NULL){
						combo->doc_id = currentInner->doc_id;
						combo->freq = currentOuter->freq + currentInner->freq;
						combo->next = NULL;
						temp = combo;
					}
					/* otherwise keep adding to the new list */
					else if (temp != NULL){
						temp->next = malloc(sizeof(DocumentNode));
						temp = temp->next;
						temp->doc_id = currentInner->doc_id;
						temp->freq = currentInner->freq + currentOuter->freq;
						temp->next = NULL;
					}
				}
				currentInner = currentInner->next;
			}
			/* if no elements of the second list matched the current element of the first list,
				add an element with the same data as the current element of the first list to the new list */
			if (matches == 0 && temp != NULL){
				temp->next = malloc(sizeof(DocumentNode));
				temp = temp->next;
				temp->doc_id = currentOuter->doc_id;
				temp->freq = currentOuter->freq;
				temp->next = NULL;
			}
			else if(matches == 0 && temp == NULL){
				combo->doc_id = currentOuter->doc_id;
				combo->freq = currentOuter->freq;
				combo->next = NULL;
				temp = combo;
			}
			currentOuter = currentOuter->next;
		}

		/* compare the second list to the current list to find elements that have were not matches
			with the first list that need to be added to the new list */
		currentOuter = second;
		while(currentOuter != NULL){
			int matches = 0;
			currentInner = combo;
			while(currentInner != NULL){
				/* check for matches */
				if (currentInner->doc_id == currentOuter->doc_id ){
					matches++;
				}
				currentInner = currentInner->next;
			}
			/* if there are no matches, add the data from the current element of the second list
				to the new list */
			if (matches == 0 && temp != NULL){
				temp->next = malloc(sizeof(DocumentNode));
				temp = temp->next;
				temp->doc_id = currentOuter->doc_id;
				temp->freq = currentOuter->freq;
				temp->next = NULL;
			}
			else if(matches == 0 && temp == NULL){
				combo->doc_id = currentOuter->doc_id;
				combo->freq = currentOuter->freq;
				combo->next = NULL;
				temp = combo;
			}
			currentOuter = currentOuter->next;
		}
		
	}
	return combo;
}

/*
 * FreeDocList -- frees memory from a list of DocumentNodes
 *
 */
void FreeDocList(DocumentNode *head){
	/* traverse the list, freeing memory as we go */
	while(head != NULL){
		DocumentNode *tmp = head;
		head = head->next;
		free(tmp);
	}
}


/*
 * CreateUnsortedDocList -- returns a list of DocumentNodes created from a string of words and AND/OR operators
 *
 * Parses the input string and uses the ANDUpdate and ORUpdate functions appropriately to create 
 * a list of DocumentNodes that represent the documents that define the series of unions and intersections
 * provided and their combined frequencies.
 */
DocumentNode *CreateUnsortedDocList(char *input, HashTable *ht){
	char *token;
	int counter = 1;
	char *copy = malloc(sizeof(char)*strlen(input)+1);
	strcpy(copy, input);
	DocumentNode *first = NULL;

	token = strtok(copy, " \n");
	while(token != NULL){
		
		DocumentNode *second = NULL;
		DocumentNode *tmp = NULL;
		if(counter == 1 && strcmp(token, "AND") != 0 && strcmp(token, "OR") != 0){
			NormalizeWord(token);
			first = GetDocList(ht, token);

			counter++;
		}
		else if (counter == 1 && (strcmp(token, "AND") == 0 || strcmp(token, "OR") != 0)){
			printf("Your input must start with a word that is not an operator.");
			free(copy);
			return NULL;
		}
		else if(counter > 1 && strcmp(token, "AND") != 0 && strcmp(token, "OR") != 0){
			NormalizeWord(token);
			second = GetDocList(ht, token);
			tmp = first;
			first = ANDUpdate(first, second);
			FreeDocList(tmp);
			FreeDocList(second);
			counter++;
		}
		else if( counter > 1 && strcmp(token, "OR") == 0){
			
			token = strtok(NULL, " \n");
			if(token != NULL && strcmp(token, "AND") != 0 && strcmp(token, "OR") != 0){
				char *ORparse = strstr(input, "OR");
			
				ORparse++;
				ORparse++;
				ORparse++;
				
				second = CreateUnsortedDocList(ORparse, ht);
				tmp = first;
				first = ORUpdate(first, second);
				FreeDocList(tmp);
				FreeDocList(second);
				free(copy);
				return first;
			}
			else{
				printf("The OR operator must be preceded by a word that is not an operator.\n");
				FreeDocList(first);
				FreeDocList(second);
				free(copy);
				return NULL;
			}
		}
		else if ( counter > 1 && strcmp(token, "AND") == 0){
			
			token = strtok(NULL, " \n");
			if (token != NULL && strcmp(token, "AND") != 0 && strcmp(token, "OR") != 0){
				
				NormalizeWord(token);
				second = GetDocList(ht, token);
				tmp = first;
				first = ANDUpdate(first, second);

				FreeDocList(tmp);
				FreeDocList(second);
				counter++;
			}
			else{
				printf("The AND operator must be preceded by a word that is not an operator.\n");
				FreeDocList(first);
				FreeDocList(second);
				free(copy);
				return NULL;
			}
		}
		token = strtok(NULL, " \n");
	}

	free(copy);
	
	return first;
}

/* 
 * SortDocList -- performs bubble sort on a singly linked list
 *
 * adapted from M. Oehm's post on: 
 * http://stackoverflow.com/questions/21388916/bubble-sort-singly-linked-list-in-c-with-pointers 
 *
 * sorts a singly linked list of DocumentNodes from largest frequency to smallest
 */
void SortDocList(DocumentNode **head){
    int done = 0;         // True if no swaps were made in a pass

    // Don't try to sort empty or single-node lists
    if (*head == NULL || (*head)->next == NULL) return;

    while (!done) {
        DocumentNode **pv = head;            // "source" of the pointer to the
                                            // current node in the list struct
        DocumentNode *nd = *head;            // local iterator pointer
       	DocumentNode *nx = (*head)->next;  // local next pointer

        done = 1;

        while (nx) {
            if (nd->freq < nx->freq) {
                nd->next = nx->next;
                nx->next = nd;
                *pv = nx;

                done = 0;
            }
            pv = &nd->next;
            nd = nx;
            nx = nx->next;
        }
    }
}

/*
 * ReportInfo -- prints information about the user's request to standard out
 *
 * Given a sorted list of DocumentNodes that hold data about files in the provided directory,
 * print the document ID and url of the elements of the list.
 */
int ReportInfo(DocumentNode *head, char *directory){
	/* check if the list is empty */
	if (head == NULL){
		printf("No websites satisfy this query.\n");
		return 1;
	}

	/* get ready to traverse the list and open files */
	DocumentNode *curr = head;
	int length = strlen(directory);
	char *fileName = malloc(sizeof(char)*(length+6));
	FILE *fp = NULL;

	/* traverse the list */
	while(curr != NULL){
		/* get the name of the current file */
		int docID = curr->doc_id;
		sprintf(fileName, "%s/%d", directory, docID);

		/* make sure the file opens properly */
		fp = fopen(fileName, "r");
		if (fp == NULL){
			printf("Error opening file %s -- unable to retrieve URL.\n", fileName);
		}
		else{
			/* find the length of the first line of the file so that enough memory can be allocated */
			int ch;
    		int count = 0;
    		for( ;; ){
       			ch = fgetc( fp );
        		if( ch == EOF || ch == '\n' ){
        			count++;
        			break;
        		}
        		count++;
    		}
    		/* store the first line of the file -- the url */
    		fseek(fp, 0, SEEK_SET);
    		char url[count+1];
			if (fgets(url, count, fp) != NULL){
				printf("Document ID: %d URL: %s\n", docID, url);
			}
			fclose(fp);
		}
		curr = curr->next;
	}
	free(fileName);
	return 1;
}


