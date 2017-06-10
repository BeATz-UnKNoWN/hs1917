//Written by Michael Simarta
//With the help of Bernice Chen
//List3.c file that implements all functions declared in List3.h
//implement the insert() functions here
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "List3.h"

#define FALSE 0
#define TRUE 1
 
typedef struct _node {
    int value;
    Node next;
    Node prev;
} node;
 
typedef struct _list {
    Node head;
    Node tail;
} list;
 
//TODO
//Read List3.h to help you look at the hints
//insert nodes into the list so it is in increasing order from head to tail
//and decreasing order from tail to head (You need to uncomment the commented test lines in testList3.c)

/*
the plan
iterate through the list from head
traverse using current->next
if current->value is <= to param value, then we insert the node

need to check current->next is null, so we can determine whether
the passed value is the highest

EDGE CASES
if the list is empty (done)
if there is only a head (done)
if the value is the highest
if the value is the lowest (done)


*/
 
void insert(List l, int value) {
    int inserted = FALSE;
    Node new = newNode(value);
    Node current = l->head;
    assert(l != NULL);
    if (current == NULL) { // list is empty
        l->head = new;
        l->tail = new;
    } else if (current->value >= value) { // 1st value is higher than parameter
        new->next = current;
        current->prev = new;
        l->head = new;
    } else if (current->next == NULL && current->value < value) { // list only contains head has value smaller than parameter
        new->prev = current;
        current->next = new;
        l->tail = new;
    } else { // any other case
        Node temp = current;
        while (current->next->next != NULL && inserted == FALSE) { //use next->next-> to keep track of prev, current, next respectively
            if (current->next->value >= value) { // if the next's value is greater or equal than parameter, we insert node before it
                new->prev = current;
                new->next = current->next;
                current->next->prev = new; // what was current's next will have its prev as "new" rather than "current"
                current->next = new; // current's new next will point to "new"
                inserted = TRUE;
            }
            temp = current;
            current = current->next;
        }

        if (inserted == FALSE) {
            if (current->value >= value) { // check if node should be inserted before 2nd last node
                new->prev = temp;
                new->next = current;
                current->prev = new;
                temp->next = new;
            } else if (current->next->value >= value) { // check if node should be inserted before last node
                new->prev = current;
                new->next = current->next;
                current->next->prev = new; // what was current's next will have its prev as "new" rather than "current"
                current->next = new; // current's new next will point to "new"
            } else { // else the node is largest and will be inserted at the end
                new->prev = current->next;
                current->next->next = new;
                l->tail = new;
            }
        }
    }
}
 
//returns a new list of length 0
List newList() {
    List l = malloc(sizeof(list));
    assert ( l!=NULL );
    l->head = NULL;
    l->tail = NULL;
    return l;
}
 
//frees everything malloced for the list
void destroy(List l) {
    Node curNode = l->head;
    Node prevNode = NULL;
 
    while (curNode != NULL) {
        prevNode = curNode;
        curNode = prevNode->next;
        free(prevNode);
    }
 
    free(l);
}
 
Node newNode (int value) {
    Node n = malloc(sizeof(node));
    n->value = value;
    n->next = NULL;
    n->prev = NULL;
    return n;
}
 
//appends a node of value to the end of the list (only goes forward)
void append(List l, int value) {
    Node newNode = malloc(sizeof(node));
    newNode->value = value;
    newNode->next = NULL;
    Node curNode = l->head;
    if ( curNode==NULL ) {
        l->head = newNode;
    } else {
        while ( curNode->next!=NULL ) {
            curNode = curNode->next;
        }
        curNode->next = newNode;
    }
}
 
//returns the int value of the node at index
//assumes input index is within range of the list's length
int getValue(List l, int index) {
 
    Node curNode = l->head;
    assert (curNode!=NULL);
    int counter = 0;
    while (counter<index) {
        curNode = curNode->next;
        counter++;
    }
    return curNode->value;
}
 
int getValueReverse(List l, int index) {
 
    Node curNode = l->tail;
    assert (curNode!=NULL);
    int counter = 0;
    while (counter<index) {
        curNode = curNode->prev;
        counter++;
    }
    return curNode->value;
}
 
//print List from head to tail
void printList(List l) {
    Node curNode = l->head;
    printf("[HEAD]->");
    while (curNode!=NULL) {
        printf ("[%d]->", curNode->value);
        curNode = curNode->next;
    }
    printf("[NULL]\n");
}
 
//print List from tail to head
void printListReverse(List l) {
    Node curNode = l->tail;
    printf("[TAIL]->");
    while (curNode!=NULL) {
        printf ("[%d]->", curNode->value);
        curNode = curNode->prev;
    }
    printf("[NULL]\n");
}