// Filename: Test cases for operations.h/c 
// Description: A unit test for the functions used by the query engine
// 

//
// Test Harness Spec:
// ------------------
//
//  It uses these files but they are not unit tested in this test harness:
//
//  void initializeHashTable(HashTable *ht)
//  void ReadFile(char *filename, HashTable *ht)
//  void FreeHashTable(HashTable *ht)
//  void FreeDocList(DcumentNode *head)
//
//  It tests the following functions:
//
//   DocumentNode *CopyListData(DocumentNode *head)
//   DocumentNode *ANDUpdate(DocumentNode *first, DocumentNode *second)
//   DocumentNode *ORUpdate(DocumentNode *first, DocumentNode *second)
//   DocumentNode *CreateUnsortedDocList(char *input, HashTable *ht)
//   void SortDocList(DocumentNode *head)
//
//  If any of the tests fail it prints status 
//  If all tests pass it prints status.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/operations.h"
#include "../../util/hashtable.h"
#include "../../util/web.h"
#include "../../util/indexer_utils.h"

// Useful MACROS for controlling the unit tests.

// each test should start by setting the result count to zero

#define START_TEST_CASE  int rs=0

// check a condition and if false print the test condition failed
// e.g., SHOULD_BE(dict->start == NULL)
// note: the construct "#x" below is the sstringification preprocessor operator that
//       converts the argument x into a character string constant

#define SHOULD_BE(x) if (!(x))  {rs=rs+1; \
    printf("Line %d test [%s] Failed\n", __LINE__,#x); \
  }


// return the result count at the end of a test

#define END_TEST_CASE return rs

//
// general macro for running a best
// e.g., RUN_TEST(TestDAdd1, "DAdd Test case 1");
// translates to:
// if (!TestDAdd1()) {
//     printf("Test %s passed\n","DAdd Test case 1");
// } else { 
//     printf("Test %s failed\n", "DAdd Test case 1");
//     cnt = cnt +1;
// }
//

#define RUN_TEST(x, y) if (!x()) {              \
    printf("Test %s passed\n", y);              \
} else {                                        \
    printf("Test %s failed\n", y);              \
    cnt = cnt + 1;                              \
}
    
HashTable *ht;

// Test case: CopyListData:1
// This test case calls the function on an empty list
int TestCopyListData1() {
  START_TEST_CASE;

  DocumentNode *first = NULL;
  DocumentNode *copy = CopyListData(first);
  DocumentNode *copytmp = copy;
  DocumentNode *firsttmp = first;

  while(copytmp != NULL && firsttmp != NULL){
    SHOULD_BE(copytmp->doc_id == firsttmp->doc_id);
    SHOULD_BE(copytmp->freq == firsttmp->freq);
    copytmp = copytmp->next;
    firsttmp = firsttmp->next;
  }
  SHOULD_BE(copytmp == NULL && firsttmp == NULL);

  FreeDocList(copy);
 
  END_TEST_CASE;
}

// Test case: CopyListData:2
// This test case calls the function on a list with three elements.
int TestCopyListData2() {
  START_TEST_CASE;
  DocumentNode *first = malloc(sizeof(DocumentNode));
  first->doc_id = 1;
  first->freq = 3;
  first->next = malloc(sizeof(DocumentNode));
  first->next->doc_id = 2;
  first->next->freq = 7;
  first->next->next = malloc(sizeof(DocumentNode));
  first->next->next->doc_id = 3;
  first->next->next->freq = 2;
  first->next->next->next = NULL;

  
  DocumentNode *copy = CopyListData(first);
  DocumentNode *copytmp = copy;
  DocumentNode *firsttmp = first;
  while(copytmp != NULL && firsttmp != NULL){
    SHOULD_BE(copytmp->doc_id == firsttmp->doc_id);
    SHOULD_BE(copytmp->freq == firsttmp->freq);
    copytmp = copytmp->next;
    firsttmp = firsttmp->next;
  }
  SHOULD_BE(copytmp == NULL && firsttmp == NULL);

  FreeDocList(copy);
  FreeDocList(first);

  END_TEST_CASE;
}


// Test case: ANDUpdate:1
// Calls the function on two lists that have two documents in common
int TestANDUpdate1() {
  START_TEST_CASE;
  DocumentNode *one = malloc(sizeof(DocumentNode));
  one->doc_id = 1;
  one->freq = 3;
  one->next = malloc(sizeof(DocumentNode));
  one->next->doc_id = 2;
  one->next->freq = 7;
  one->next->next = malloc(sizeof(DocumentNode));
  one->next->next->doc_id = 3;
  one->next->next->freq = 2;
  one->next->next->next = NULL;

  DocumentNode *two = malloc(sizeof(DocumentNode));
  two->doc_id = 1;
  two->freq = 3;
  two->next = malloc(sizeof(DocumentNode));
  two->next->doc_id = 2;
  two->next->freq = 7;
  two->next->next = malloc(sizeof(DocumentNode));
  two->next->next->doc_id = 4;
  two->next->next->freq = 2;
  two->next->next->next = NULL;

  DocumentNode *combo = ANDUpdate(one, two);
  SHOULD_BE(combo->doc_id == 1);
  SHOULD_BE(combo->freq == 6);
  SHOULD_BE(combo->next->doc_id == 2);
  SHOULD_BE(combo->next->freq == 14);
  SHOULD_BE(combo->next->next == NULL);

  FreeDocList(combo);
  FreeDocList(one);
  FreeDocList(two);

  END_TEST_CASE;
}

// Test case:ANDUpdate:2
// Calls the function with an empty list as the second parameter.
int TestANDUpdate2() {
  START_TEST_CASE;

  DocumentNode *one = malloc(sizeof(DocumentNode));
  one->doc_id = 1;
  one->freq = 3;
  one->next = malloc(sizeof(DocumentNode));
  one->next->doc_id = 2;
  one->next->freq = 7;
  one->next->next = malloc(sizeof(DocumentNode));
  one->next->next->doc_id = 3;
  one->next->next->freq = 2;
  one->next->next->next = NULL;

  DocumentNode *two = NULL;

  DocumentNode *combo = ANDUpdate(one, two);
  SHOULD_BE(combo == NULL);

  FreeDocList(one);
  FreeDocList(combo);

  END_TEST_CASE;

}


// Test case:ANDUpdate:3
// Calls the function with an empty list as the first parameter.
int TestANDUpdate3() {
  START_TEST_CASE;
  DocumentNode *one = NULL;

  DocumentNode *two = malloc(sizeof(DocumentNode));
  two->doc_id = 1;
  two->freq = 3;
  two->next = malloc(sizeof(DocumentNode));
  two->next->doc_id = 2;
  two->next->freq = 7;
  two->next->next = malloc(sizeof(DocumentNode));
  two->next->next->doc_id = 4;
  two->next->next->freq = 2;
  two->next->next->next = NULL;

  DocumentNode *combo = ANDUpdate(one, two);
  SHOULD_BE(combo == NULL);

  FreeDocList(two);
  FreeDocList(combo);

  END_TEST_CASE;
}

// Test case:ANDUpdate:4
// Calls the function with two empty lists as the parameters
int TestANDUpdate4(){
  START_TEST_CASE;
  DocumentNode *one = NULL;
  DocumentNode *two = NULL;

  DocumentNode *combo = ANDUpdate(one, two);
  SHOULD_BE(combo == NULL);

  END_TEST_CASE;
}

// Test case: ORUpdate:1
// Calls the function on two lists that have two documents in common
int TestORUpdate1() {
  START_TEST_CASE;
  DocumentNode *one = malloc(sizeof(DocumentNode));
  one->doc_id = 1;
  one->freq = 3;
  one->next = malloc(sizeof(DocumentNode));
  one->next->doc_id = 2;
  one->next->freq = 7;
  one->next->next = malloc(sizeof(DocumentNode));
  one->next->next->doc_id = 3;
  one->next->next->freq = 2;
  one->next->next->next = NULL;

  DocumentNode *two = malloc(sizeof(DocumentNode));
  two->doc_id = 1;
  two->freq = 3;
  two->next = malloc(sizeof(DocumentNode));
  two->next->doc_id = 2;
  two->next->freq = 7;
  two->next->next = malloc(sizeof(DocumentNode));
  two->next->next->doc_id = 4;
  two->next->next->freq = 2;
  two->next->next->next = NULL;

  DocumentNode *combo = ORUpdate(one, two);
  SHOULD_BE(combo->doc_id == 1);
  SHOULD_BE(combo->freq == 6);
  SHOULD_BE(combo->next->doc_id == 2);
  SHOULD_BE(combo->next->freq == 14);
  SHOULD_BE(combo->next->next->doc_id == 3);
  SHOULD_BE(combo->next->next->freq == 2);
  SHOULD_BE(combo->next->next->next->doc_id == 4);
  SHOULD_BE(combo->next->next->next->freq == 2);
  SHOULD_BE(combo->next->next->next->next == NULL);

  FreeDocList(one);
  FreeDocList(two);
  FreeDocList(combo);
  END_TEST_CASE;
}


// Test case:ORUpdate:2
// calls the function with an empty list as the second parameter
int TestORUpdate2() {
  START_TEST_CASE;
  DocumentNode *one = malloc(sizeof(DocumentNode));
  one->doc_id = 1;
  one->freq = 3;
  one->next = malloc(sizeof(DocumentNode));
  one->next->doc_id = 2;
  one->next->freq = 7;
  one->next->next = malloc(sizeof(DocumentNode));
  one->next->next->doc_id = 3;
  one->next->next->freq = 2;
  one->next->next->next = NULL;

  DocumentNode *two = NULL;

  DocumentNode *combo = ORUpdate(one, two);
  SHOULD_BE(combo->doc_id == one->doc_id);
  SHOULD_BE(combo->freq == one->freq);
  SHOULD_BE(combo->next->doc_id == one->next->doc_id);
  SHOULD_BE(combo->next->freq == one->next->freq);
  SHOULD_BE(combo->next->next->doc_id == one->next->next->doc_id);
  SHOULD_BE(combo->next->next->freq == one->next->next->freq);
  SHOULD_BE(combo->next->next->next == NULL);

  FreeDocList(one);
  FreeDocList(combo);
  END_TEST_CASE;
}


// Test case:ORUpdate:3
// calls the function with an empty list as the first parameter
int TestORUpdate3() {
  START_TEST_CASE;
  DocumentNode *one = NULL;

  DocumentNode *two = malloc(sizeof(DocumentNode));
  two->doc_id = 1;
  two->freq = 3;
  two->next = malloc(sizeof(DocumentNode));
  two->next->doc_id = 2;
  two->next->freq = 7;
  two->next->next = malloc(sizeof(DocumentNode));
  two->next->next->doc_id = 4;
  two->next->next->freq = 2;
  two->next->next->next = NULL;

  DocumentNode *combo = ORUpdate(one, two);
  SHOULD_BE(combo->doc_id == two->doc_id);
  SHOULD_BE(combo->freq == two->freq);
  SHOULD_BE(combo->next->doc_id == two->next->doc_id);
  SHOULD_BE(combo->next->freq == two->next->freq);
  SHOULD_BE(combo->next->next->doc_id == two->next->next->doc_id);
  SHOULD_BE(combo->next->next->freq == two->next->next->freq);
  SHOULD_BE(combo->next->next->next == NULL);

  FreeDocList(two);
  FreeDocList(combo);
  END_TEST_CASE;
}


// Test case:ORUpdate:4
// calls the function with two empy lists as the parameters
int TestORUpdate4(){
  START_TEST_CASE;
  DocumentNode *one = NULL;
  DocumentNode *two = NULL;

  DocumentNode *combo = ORUpdate(one, two);
  SHOULD_BE(combo == NULL);

  END_TEST_CASE;
}

// Test case:CreateUnsortedDocList:1
// calls the function with one word that occurs in one document as the input
int TestCreateUnsortedDocList1(){
  START_TEST_CASE;


  DocumentNode *list = CreateUnsortedDocList("timesaving", ht);

  SHOULD_BE(list->doc_id == 1076);
  SHOULD_BE(list->freq == 1);
  SHOULD_BE(list->next == NULL);

  FreeDocList(list);

  END_TEST_CASE;
}

// Test case:CreateUnsortedDocList:2
// calls the function with one word that occurs in two documents as the input
int TestCreateUnsortedDocList2(){
  START_TEST_CASE;
  
  DocumentNode *list = CreateUnsortedDocList("captivate", ht);

  SHOULD_BE(list->doc_id == 1367);
  SHOULD_BE(list->freq == 1);
  SHOULD_BE(list->next->doc_id == 560);
  SHOULD_BE(list->next->freq == 1);
  SHOULD_BE(list->next->next == NULL);

  FreeDocList(list);

  END_TEST_CASE;
}

// Test case:CreateUnsortedDocList:3
// calls the function with one word that is not in the hashtable
int TestCreateUnsortedDocList3(){
  START_TEST_CASE;
  
  DocumentNode *list = CreateUnsortedDocList("badword", ht);

  SHOULD_BE(list == NULL);

  FreeDocList(list);

  END_TEST_CASE;
}

// Test case:CreateUnsortedDocList:4
// calls the function with the AND operator and two words that have one document in common
int TestCreateUnsortedDocList4(){
  START_TEST_CASE;
  
  DocumentNode *list = CreateUnsortedDocList("captivate AND computer", ht);

  SHOULD_BE(list->doc_id == 560);
  SHOULD_BE(list->freq == 12);
  SHOULD_BE(list->next == NULL);

  FreeDocList(list);

  END_TEST_CASE;
}

// Test case:CreateUnsortedDocList:5
// calls the function with the OR operator and two words that have one word in common
int TestCreateUnsortedDocList5(){
  START_TEST_CASE;
  
  DocumentNode *list = CreateUnsortedDocList("captivate OR cavouras", ht);

  SHOULD_BE(list->doc_id == 1367);
  SHOULD_BE(list->freq == 1);
  SHOULD_BE(list->next->doc_id == 560);
  SHOULD_BE(list->next->freq == 2);
  SHOULD_BE(list->next->next == NULL);

  FreeDocList(list);

  END_TEST_CASE;
}

// Test case:CreateUnsortedDocList:5
// tests the function's ability to consider the precidence of AND over OR
int TestCreateUnsortedDocList6(){
  START_TEST_CASE;
  
  DocumentNode *list = CreateUnsortedDocList("captivate AND computer OR morphometry AND anterior", ht);

  SHOULD_BE(list->doc_id == 560);
  SHOULD_BE(list->freq == 12);
  SHOULD_BE(list->next->doc_id == 159);
  SHOULD_BE(list->next->freq == 4);
  SHOULD_BE(list->next->next == NULL);

  FreeDocList(list);

  END_TEST_CASE;
}

// Test case:SortDocList:1
// sorts list of three DocumentNodes 
int TestSortDocList(){
  START_TEST_CASE;

  DocumentNode *list = malloc(sizeof(DocumentNode));
  list->doc_id = 1;
  list->freq = 3;
  list->next = malloc(sizeof(DocumentNode));
  list->next->doc_id = 2;
  list->next->freq = 7;
  list->next->next = malloc(sizeof(DocumentNode));
  list->next->next->doc_id = 3;
  list->next->next->freq = 2;
  list->next->next->next = NULL;

  SortDocList(&list);

  SHOULD_BE(list->doc_id == 2);
  SHOULD_BE(list->freq == 7);
  SHOULD_BE(list->next->doc_id == 1);
  SHOULD_BE(list->next->freq == 3);
  SHOULD_BE(list->next->next->doc_id == 3);
  SHOULD_BE(list->next->next->freq == 2);
  SHOULD_BE(list->next->next->next == NULL);

  FreeDocList(list);

  END_TEST_CASE;

}



// This is the main test harness for the set of query engine functions. It tests the code
// in operations.c:
//
//  It uses these files but they are not unit tested in this test harness:
//
//  void initializeHashTable(HashTable *ht)
//  void ReadFile(char *filename, HashTable *ht)
//  void FreeHashTable(HashTable *ht)
//  void FreeDocList(DcumentNode *head)
//
//  It tests the following functions:
//
//   DocumentNode *CopyListData(DocumentNode *head)
//   DocumentNode *ANDUpdate(DocumentNode *first, DocumentNode *second)
//   DocumentNode *ORUpdate(DocumentNode *first, DocumentNode *second)
//   DocumentNode *CreateUnsortedDocList(char *input, HashTable *ht)
//   void SortDocList(DocumentNode *head)
//
//  If any of the tests fail it prints status 
//  If all tests pass it prints status.

int main(int argc, char** argv) {

  ht = malloc(sizeof(HashTable));
  initializeHashTable(ht);

  ReadFile("../../indexer/index2.dat.txt", ht);

  int cnt = 0;

  RUN_TEST(TestCopyListData1, "CopyListData Test case 1");
  RUN_TEST(TestCopyListData2, "CopyListData Test case 2");
  RUN_TEST(TestANDUpdate1, "ANDUpdate Test case 1");
  RUN_TEST(TestANDUpdate2, "ANDUpdate Test case 2");
  RUN_TEST(TestANDUpdate3, "ANDUpdate Test case 3");
  RUN_TEST(TestANDUpdate4, "ANDUpdate Test case 4");

  RUN_TEST(TestORUpdate1, "ORUpdate Test case 1");
  RUN_TEST(TestORUpdate2, "ORUpdate Test case 2");
  RUN_TEST(TestORUpdate3, "ORUpdate Test case 3");
  RUN_TEST(TestORUpdate4, "ORUpdate Test case 5");

  RUN_TEST(TestCreateUnsortedDocList1, "CreateUnsortedDocList Test case 1");
  RUN_TEST(TestCreateUnsortedDocList2, "CreateUnsortedDocList Test case 2");
  RUN_TEST(TestCreateUnsortedDocList3, "CreateUnsortedDocList Test case 3");
  RUN_TEST(TestCreateUnsortedDocList4, "CreateUnsortedDocList Test case 4");
  RUN_TEST(TestCreateUnsortedDocList5, "CreateUnsortedDocList Test case 5");
  RUN_TEST(TestCreateUnsortedDocList6, "CreateUnsortedDocList Test case 6");

  RUN_TEST(TestSortDocList, "SortDocList Test case 1");

  if (!cnt) {
    printf("All passed!\n"); return 0;
  } else {
    printf("Some fails!\n"); return 1;
  }

  FreeIndex(ht);
  free(ht);
}


