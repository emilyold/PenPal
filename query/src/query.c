#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#include "../../util/web.h"
#include "../../util/hashtable.h"
#include "../../util/file.h"
#include "../../util/indexer_utils.h"
#include "operations.h"

#define MAX_INPUT 1000

static int running = 1;

/*
 * GetUserInput -- reads from standard in
 * 
 * Stores input from standard in the string provided
 * Returns 0 if all goes well, 1 if the user intput too many
 * characters, and 2 if EOF read.
 */
int GetUserInput(char *input){
	/* read characters until the new line character or EOF is reached */
	int i=0;
	int ch;
	while (1){
		ch = getchar();
    	if (ch == '\n'){
        	input[i] = '\0';
        	break;
    	}
    	else if (ch == EOF ){
    		return 2;
    	}
    	else{
    		input[i] = ch;	
    	}
    	i++;
	}

	/* check if two many characters were inputted */
	if(strlen(input) > MAX_INPUT){
		printf("Too many characters inputted\n");
		return 0;
	}

	/* clean up the string once complete */
	input[strlen(input)] = '\0';
	return 1;
}


/* handles SIGINT */
void intHandler(int dummy){
	running = 0;
}

int main(int argc, char *argv[]){
	/* argument checking */
	if (argc != 3){
		printf("The query engine required 2 arguments.\n");
		exit(EXIT_FAILURE);
	}
	if ( !IsFile(argv[1]) ){
		printf("%s is not a valid file.", argv[1]);
		exit(EXIT_FAILURE);
	}
	if ( !IsDir(argv[2]) ){
		printf("%s is not a valid directory", argv[2]);
		exit(EXIT_FAILURE);
	}

	/* HashTable to store data from the file provided */
	HashTable *ht = malloc(sizeof(HashTable));
	initializeHashTable(ht);
	printf("Reading in the file...\n");
	ReadFile(argv[1], ht);
	

 	

	signal(SIGINT, intHandler);
	while(running){
		DocumentNode *final = NULL;
		char input[MAX_INPUT + 1]; // character array to hold user input
		printf("Enter your words: ");

		/* make sure everything went well on the user's end */
		int ret = GetUserInput(input);
		if( ret == 0){
			continue;
		}
		else if(ret == 2){
			break;
		}

		/* handle the input */
		final = CreateUnsortedDocList(input, ht);
		SortDocList(&final);	
		ReportInfo(final, argv[2]);
		FreeDocList(final);
		
		
	}

	/* clean up */
	
	FreeIndex(ht);	
	free(ht);	
	

	
}
