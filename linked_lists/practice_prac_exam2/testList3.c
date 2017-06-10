// Written by Michael Simarta
// With the help of Bernice Chen.
//
//run this testList3.c file with a compilable List3.c
//that should have the functions declared in List3.h implemented 
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "List3.h"
 
typedef struct _node {
    int value;
    Node next;
    Node prev;
} node;
 
typedef struct _list {
    Node head;
    Node tail;
} list;
 
void test_test();
 
void test_insert();
 
int main( int argc, char * argv[] ) {
   printf("......STARTING TESTS......\n");
 
   test_test();
   test_insert();
 
   printf("ALL TESTS PASSED. YOU ARE AWESOME!\n");
 
   return EXIT_SUCCESS;
}
 
void test_test() {
   List l = newList();
   append(l, 0);
   append(l, 1);
   append(l, 2);
   assert ( getValue(l, 0) == 0);
   assert ( getValue(l, 1) == 1);
   assert ( getValue(l, 2) == 2);
 
 
   printf("EXISTING FUNCTIONS WORK FINE.\n");
}
 
 
//function to test the insert() function
void test_insert() {
   printf("TESTING insert()...\n");
   
 
   // Empty List
   //NULL
   printf("Doing Test1...\n");
   List l = newList();
   assert (l->head == NULL);
   assert (l->tail == NULL);
   destroy(l);
   printf("insert Test1 passed.\n");
 
   // 1 Node List
   // NULL <-> 5 <-> NULL
   printf("Doing Test2...\n");
   l = newList();
   insert(l, 5);
   printf("Expecting [HEAD]->[5]->[NULL]\n");
   printList(l);
   assert ( getValue(l, 0) == 5);
   assert (l->head->next == NULL);
 
   printf("Expecting [TAIL]->[5]->[NULL]\n");
   printListReverse(l);
   assert ( getValueReverse(l, 0) == 5);
   assert (l->tail->prev == NULL);
   destroy(l);
   printf("insert Test2 passed.\n");
 
 
   // 3 Node List
   //NULL <-> 2 <-> 5 <-> 7 <-> NULL
   printf("Doing Test3...\n");
   l = newList();
   insert(l, 5);
   insert(l, 2);
   insert(l, 7);
   printf("Expecting [HEAD]->[2]->[5]->[7]->[NULL]\n");
   printList(l);
   assert ( getValue(l, 0) == 2);
   assert ( getValue(l, 1) == 5);
   assert ( getValue(l, 2) == 7);
   assert (l->head->next->next->next == NULL);
 
   printf("Expecting [TAIL]->[7]->[5]->[2]->[NULL]\n");
   printListReverse(l);
   assert ( getValueReverse(l, 0) == 7);
   assert ( getValueReverse(l, 1) == 5);
   assert ( getValueReverse(l, 2) == 2);
   assert (l->tail->prev->prev->prev == NULL);
   destroy(l);
   printf("insert Test3 passed.\n");
 
   // 4 Node List
   //NULL <-> 1 <-> 2 <-> 4 <-> 6 <-> NULL
   printf("Doing Test4...\n");
   l = newList();
   insert(l, 4);
   insert(l, 2);
   insert(l, 1);
   insert(l, 6);
   printf("Expecting [HEAD]->[1]->[2]->[4]->[6]->[NULL]\n");
   printList(l);
   assert ( getValue(l, 0) == 1);
   assert ( getValue(l, 1) == 2);
   assert ( getValue(l, 2) == 4);
   assert ( getValue(l, 3) == 6);
   assert (l->head->next->next->next->next == NULL);
 
   printf("Expecting [TAIL]->[6]->[4]->[2]->[1]->[NULL]\n");
   printListReverse(l);
   assert ( getValueReverse(l, 0) == 6);
   assert ( getValueReverse(l, 1) == 4);
   assert ( getValueReverse(l, 2) == 2);
   assert ( getValueReverse(l, 3) == 1);
   assert (l->tail->prev->prev->prev->prev == NULL);
   destroy(l);
   printf("insert Test4 passed.\n");
 
}