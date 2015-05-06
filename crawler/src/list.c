/* ========================================================================== */
/* File: list.c - doubly linked list with two sentinels
 *
 * Author: Emily Old
 * Date: April 2015
 *
 * Input:
 *
 * Command line options:
 *
 * Output:
 *
 * Error Conditions:
 *
 * Special Considerations:
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include "common.h"

// ---------------- Local includes  e.g., "file.h"
#include "list.h"
#include <stdlib.h>
// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */

/*
 * initializeList -- creates empy doubly linked list with two sentinels
 *
 * Assumptions:
 * 		1. theList has been allocated
 *
 * Pseudocode:
 * 		1. allocate memory for the head and tail
 * 		2. set the head and tail data to NULL
 * 		3. adjust head pointers
 * 		4. adjust tail pointers
 */
void initializeList(List *theList){
	/* set each node's data to null */
	ListNode *head = malloc(sizeof(ListNode));
	ListNode *tail = malloc(sizeof(ListNode));

	theList->head = head;
	theList->tail = tail;
	theList->head->page = NULL;
	theList->tail->page = NULL;

	/* deal with the head */
	theList->head->prev = NULL;
	theList->head->next = theList->tail;
	

	/* deal with the tail */
	theList->tail->prev = theList->head;
	theList->tail->next = NULL;
}


/*
 * pop -- remove and return the first element of a list
 * 
 * Assumptions:
 * 		1. theList has been allocated
 * 
 * Pseudocode:
 * 		1. check if the list is empty
 *		2. if there is an element to be popped allocate memory 
 * 		3. adjust pointers to remove the element
 *		4. if it is empty return NULL;
 */
ListNode *pop(List *theList){
	/* check if there is anything in the list other than the head and tail */ 
	if(theList->head->next != theList->tail){
		/* get the first listnode */
		ListNode *popped = malloc(sizeof(ListNode));
		popped = theList->head->next;

		/* adjust pointers */
		theList->head->next = popped->next;
		popped->next->prev = theList->head;
		// free(popped->next);
		// free(popped->prev);

		return popped;
	}
	else{
		return NULL;
	}
}

/*
 * appendToList -- add an element to the end of a list
 *
 * Assumptions:
 * 		1. theList has been allocated
 *
 * Pseudocode:
 * 		1. allocate memory for the new element
 * 		2. adjust pointers at the end of the list 
 */
void appendToList(List *theList, void *page){
	
	ListNode *node = malloc(sizeof(ListNode));
	node->page = page;

	/* insert the new listNode directly before the tail */
	node->prev = theList->tail->prev;
	theList->tail->prev->next = node;
	node->next = theList->tail;
	theList->tail->prev = node;
	
}
	