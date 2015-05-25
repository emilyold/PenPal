// Filename: Test cases for dictionary.h/.c
// Description: A unit test for dictionary data structure
// 

//
// Test Harness Spec:
// ------------------
//
//  It uses these files but they are not unit tested in this test harness:
//
//  DICTIONARY* InitDictionary();
//  int make_hash(char* c);
//  void CleanDictionary(DICTIONARY* dict)
// 
//  It tests the following functions:
//
//   void DAdd(DICTIONARY* dict, void* data, char* key);
//   void DRemove(DICTIONARY* dict, char* key);
//   void* GetDataWithKey(DICTIONARY* dict, char* key);
//
//  If any of the tests fail it prints status 
//  If all tests pass it prints status.
//
//  Test Cases:
//  -----------
//   
//  The test harness runs a number of test cases to test the code.
//  The approach is to first set up the environment for the test,
//  invoke the function to be tested, then validate the state of
//  the data structures using the SHOULD_BE macro. This is repeated
//  for each test case. 
//
//  The test harness isolates the functions under test and setting
//  up the environment the code expects when integrated in the real
//  system.
//
//  The test harness dummies out the real hash function and through
//  use of a variable called hash manipulates where DNODEs are 
//  inserted into the DICTIONARY. For example, collisions can be controlled. 
//
//  The following test cases  (1-3) are for function:
//
//  void DAdd(DICTIONARY* dict, void* data, char* key);
//
//  Test case: DADD:1
//  This test case calls DAdd() for the condition where dict is empty
//  result is to add a DNODE to the dictionary and look at its values.
//
//  Test case: DADD:2
//  This test case calls DAdd() puts multiple DNODEs on the dict when there is no hash collisions
//  We put multiply elements in dictionary with no collisions.
//
//  Test case: DADD:3
//  This test case calls DAdd() puts multiple DNODEs on the dict when there is hash collisions
//  We put multiply elements in dictionary with collisions.
//
//  The following test cases (1-4) for function:
//
//  void DRemove(DICTIONARY* dict, char* key);
//
//  Test case:DREMOVE:1
//  This test case DAdd() and DRemove()  DNODE from dict for only one element.
//
//  Test case:DREMOVE:2
//  This test case is tries to see how DRemove() works with multiple nodes for the same 
//  hash value, the node to be deleted is at the end of the dynamic list.
//
//  Test case:DREMOVE:3
//  This test case is tries to see how DRemove() works with multiple nodes of the same hash value, 
//  the node to be deleted is at the start of the dynamic list.
//
//  Test case:DREMOVE:4
//  This test case is tries to see how DRemove() works with multiple nodes of the same hash value, 
//  the node to be deleted is at the middle of the dynamic list.
//
//  The following test cases (1) for function:
//
//  void* GetDataWithKey(DICTIONARY* dict, char* key);
//
//  Test case:GetDataWithKey:1
//  This test case tests GetDataWithKey - to get a data with the a certain key.
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

// Test case: DADD:1
// This test case calls DAdd() for the condition where dict is empty
// result is to add a DNODE to the dictionary and look at its values.

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

// Test case: DADD:2
// This test case calls DAdd() puts multiple DNODEs on the dict when there is no hash collisions
// We put multiply elements in dictionary with no collisions.

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


// Test case: DADD:3
// This test case calls DAdd() puts multiple DNODEs on the dict when there is hash collisions
// We put multiply elements in dictionary with collisions.

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

// Test case:DREMOVE:1
// This test case DAdd() and DRemove()  DNODE from dict for only one element.
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


// Test case:DREMOVE:2
// This test case is tries to see how DRemove() works with multiple nodes for the same hash value, the node to be deleted is at the end of the dynamic list.

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

int TestANDUpdate4(){
  START_TEST_CASE;
  DocumentNode *one = NULL;
  DocumentNode *two = NULL;

  DocumentNode *combo = ANDUpdate(one, two);
  SHOULD_BE(combo == NULL);

  END_TEST_CASE;
}


// Test case:DREMOVE:3
// This test case is tries to see how DRemove() works with multiple nodes of the same hash value, the node to be deleted is at the start of the dynamic list

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


// Test case:DREMOVE:4
// This test case is tries to see how DRemove() works with multiple nodes of the same hash value, the node to be deleted is at the middle of the dynamic list
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

// Test case:GetDataWithKey:1
// This test case tests GetDataWithKey - to get a data with the a certain key.
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

int TestORUpdate4(){
  START_TEST_CASE;
  DocumentNode *one = NULL;
  DocumentNode *two = NULL;

  DocumentNode *combo = ORUpdate(one, two);
  SHOULD_BE(combo == NULL);

  END_TEST_CASE;
}

int TestCreateUnsortedDocList1(){
  START_TEST_CASE;


  DocumentNode *list = CreateUnsortedDocList("timesaving", ht);

  SHOULD_BE(list->doc_id == 1076);
  SHOULD_BE(list->freq == 1);
  SHOULD_BE(list->next == NULL);

  FreeDocList(list);

  END_TEST_CASE;
}

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

int TestCreateUnsortedDocList3(){
  START_TEST_CASE;
  
  DocumentNode *list = CreateUnsortedDocList("badword", ht);

  SHOULD_BE(list == NULL);

  FreeDocList(list);

  END_TEST_CASE;
}

int TestCreateUnsortedDocList4(){
  START_TEST_CASE;
  
  DocumentNode *list = CreateUnsortedDocList("captivate AND computer", ht);

  SHOULD_BE(list->doc_id == 560);
  SHOULD_BE(list->freq == 12);
  SHOULD_BE(list->next == NULL);

  FreeDocList(list);

  END_TEST_CASE;
}

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



// This is the main test harness for the set of dictionary functions. It tests all the code
// in dictionary.c:
//
//  It uses these files but they are not unit tested in this test harness:
//
//  DICTIONARY* InitDictionary();
//  int make_hash(char* c);
//  void CleanDictionary(DICTIONARY* dict)
// 
//  It test the following functions:
//
//   void DAdd(DICTIONARY* dict, void* data, char* key);
//   void DRemove(DICTIONARY* dict, char* key);
//   void* GetDataWithKey(DICTIONARY* dict, char* key);
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


