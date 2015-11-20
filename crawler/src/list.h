/* ========================================================================== */
/* File: list.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * This file contains the definitions for a doubly-linked list of WebPages.
 *
 */
/* ========================================================================== */
#ifndef LIST_H
#define LIST_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality

// ---------------- Constants

// ---------------- Structures/Types

typedef struct ListNode {
    void *data;                           // the data for a given page
    struct ListNode *prev;                   // pointer to previous node
    struct ListNode *next;                   // pointer to next node
} ListNode;

typedef struct List {
    ListNode *head;                          // "beginning" of the list
    ListNode *tail;                          // "end" of the list
} List;

// ---------------- Public Variables

// ---------------- Prototypes/Macros

/*
 * initializeList
 * @theList: List structure that has been allocated
 *
 * Returns void
 */
void initializeList(List *theList);

/*
 * pop -- removes first element in a list
 * @theList: List structure that has been allocated
 * 
 * Returns pointer to the listNode structure that was popped
 */
ListNode *pop(List *theList);

/*
 * appendToList -- adds an element to the end of a list 
 * @theList: List structure that has been allocated
 * @page: element to be appended
 *
 * Returns void
 */
void appendToList(List *theList, void *data);

#endif // LIST_H
