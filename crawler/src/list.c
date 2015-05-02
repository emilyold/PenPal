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
 * Given a pointer to a List struture, connect the list's
 * head and tail, and set their data equal to NULL;
 */
void initializeList(List *theList){
	/* set each node's data to null */
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
 * Return the first element of the list if it exists.
 * Otherwise return null.
 */
ListNode *pop(List *theList){
	/* check if there is anything in the list other than the head and tail */ 
	if(theList->head->next != tail){
		/* get the first listnode */
		ListNode *popped = theList->head->next;

		/* adjust pointers */
		theList->head->next = popped->next;
		popped->next->prev = theList->head;

		return popped;
	}
	else{
		return NULL;
	}
}

/*
 * Add a list node to the end of the list
 */
void appendToList(List *theList, void *page){
	
	ListNode *node = malloc(sizeof(ListNode));
	node->page = page;

	node->prev = theList->tail->prev;
	theList->tail->prev->next = node;
	node->next = theList->tail;
	theList->tail->prev = node;
	
}
	