/*
 * Andrew Timkov
 * Date: 15/5/17
 * My Linked List extraFns 2nd Attempt
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct _node *link;    // a link points to a node
 
typedef struct _node {
    int value;
    link next;
} node;
 
// a list is represented by a pointer to a struct which contains 
// a pointer to the first node of the list called the "head"
typedef struct _list {
    link head;
} *list; 

// print out a list
void showList (list listToPrint);
 
// inset item at the front of the list
void frontInsert (list l, int item);
 
// count the number of items in the list
int numItems (list l);
 
// insert at end of list
void append (list l, int value);
 
// find the value stored at position i
// i MUST be a valid position in the list
// dont call this on positions outside the list
int lookup (list l, int position);

// create an empty list

list newList (void);

// delete the first node
void deleteHead (list l);

// delete the end node
void deleteLast (list l);

// join two lists

// move all the elements from the second list to the end of
// the first list (in order).  this leaves the second list
// empty and all the elements in the first list. 
void concatenate (list to, list from);

// count the number of nodes containing 42
int count42s (list l);

void zip (list new, list first, list second);

static void testExtra (void);



int main (int argc, char *argv[]) {
    //list newList = malloc(sizeof(list));
    //link ptrToFirst = malloc(sizeof(node));
    //ptrToFirst->value = 1;
    //ptrToFirst->next = NULL;
    //newList->head = ptrToFirst;
    //frontInsert(newList, 2);
    //frontInsert(newList, 3);
    //showList(newList);
    //printf("There are %d items\n", numItems(newList));
    //append(newList, 0);
    //showList(newList);
    //printf("There are %d items\n", numItems(newList));
    //printf("%d is at position 2\n", lookup(newList,2));
    //free(newList);
    //testExtra();
    
    list newList1 = malloc(sizeof(list));
    link ptrToFirst1 = malloc(sizeof(node));
    ptrToFirst1->value = 1;
    ptrToFirst1->next = NULL;
    newList1->head = ptrToFirst1;
    frontInsert(newList1, 2);
    frontInsert(newList1, 3);
    
    list newList2 = malloc(sizeof(list));
    link ptrToFirst2 = malloc(sizeof(node));
    ptrToFirst2->value = 1;
    ptrToFirst2->next = NULL;
    newList2->head = ptrToFirst2;
    frontInsert(newList2, 2);
    frontInsert(newList2, 3);
    
    list finalList = newList();
    showList(newList1);
    showList(newList2);
    zip(finalList, newList1, newList2);
    showList(finalList);
    printf("all tests passed, you are awesome!\n");
    
    

    return EXIT_SUCCESS;
}

list newList (void) {
    // allocate memory for the list
    list new = malloc(sizeof(list));
    // empty so there is no head
    new->head = NULL;
    return new;
}

void showList (list listToPrint) {
    // temp variable to traverse through the linked list
    link current = listToPrint->head;
    while (current->next != NULL) {
        // print value of the current node
        printf("[%d] -> ", current->value);
        // continue traversing through the list
        current = current->next;
    }
    // print the last value without the arrow
    printf("[%d]\n", current->value);
    return;
}

void frontInsert (list l, int item) {
    // allocate memory for a new node, and point to it
    link ptrToNode = malloc(sizeof(node));
    ptrToNode->value = item;
    // point to the current head
    ptrToNode->next = l->head;
    // make the new node the head of the list
    l->head = ptrToNode;

    return;
}

int numItems (list l) {
    int num = 0;
    link current = l->head;
    // while current points to a valid node
    // increment the num count
    while (current != NULL) {
        num++;
        current = current->next;
    }

    return num;
}

void append (list l, int value) {
    link current = l->head;
    // traverse the list until we reach the last node
    // we know the last node points to NULL, so we check for that
    while (current->next != NULL) {
        current = current->next;
    }
    // create a new node and point to it and define it
    link ptrToNode = malloc(sizeof(node));
    ptrToNode->value = value;
    ptrToNode->next = NULL;
    // make what used to be the last node, point to the new node
    current->next = ptrToNode;
    return;
}

int lookup (list l, int position) {
    int value = 0;
    int i = 0;
    // the head is index 0
    link current = l->head;
    // traverse until we reach the last node (points to NULL)
    while (current->next != NULL) {
        if (i == position) {
            // when we reach the specified node, retrieve its value
            value = current->value;
        }
        i++;
        current = current->next;
    }

    return value;
}

void deleteHead (list l) {
    // if there are at least 2 nodes in the list
    if (l->head->next != NULL) {
        // point to the initial head to free later
        link temp = l->head;
        // make the node head points to, the new head
        l->head = l->head->next;
        // free the old head from memory
        free(temp);
    } else {
        // if the head is the only node in the list, set it to NULL
        l->head = NULL;
    }
    return;
}

void deleteLast (list l) {
    link current = l->head;
    // the head points to NULL, it is the last node
    // so we set the head to NULL
    if (current->next == NULL) {
        l->head = NULL;
    }
    while (current->next != NULL) {
        // if the current pointer is pointing to the last node (that points to NULL)
        if (current->next->next == NULL) {
            // we free the last node from memory
            free(current->next);
            // and instead point to null instead of what was the last node
            current->next = NULL;
        } else {
            // otherwise we do nothing and continue traversing
            current = current->next;
        }
    }
    return;
}

// only manipulating pointers, no need to free any nodes
void concatenate (list to, list from) {
    // if the 1st list is empty, make it equal to the 2nd list
    if (to->head == NULL) {
        to->head = from->head;
    } else {
        link current = to->head;
        // while we haven't reached the last node (that points to NULL)
        while (current->next != NULL) {
            // traverse through the list
            current = current->next;
        }
        // get the last node of the 1st list to point to the
        // first node of the 2nd list
        current->next = from->head;
    }
    // make the 2nd head point to nothing
    from->head = NULL;
    return;
}

int count42s (list l) {
    int count = 0;
    link current = l->head;
    if (l->head != NULL) {
        while (current->next != NULL) {
            if (current->value == 42) {
                count++;
            }
            current = current->next;
        }
        if (current->value == 42) {
            count++;
        }
    }
    return count;
}

void zip (list new, list l1, list l2) {
    link temp1 = l1->head;
    link temp2 = l2->head;
    link current = new->head;
    
    if (temp1 == NULL && temp2 == NULL) {
        //do nothing
    } else if (temp1 == NULL) {
        new->head = l2->head;
    } else if (temp2 == NULL) {
        new->head = l1->head;
    } else {
        /*while (temp1->next != NULL && temp2->next != NULL) {
            current = temp1;
            current->next = temp2;
            temp1 = temp1->next;
            temp2 = temp2->next;
            current = current->next->next;
        }
        if (temp1->next != NULL) {
            current = temp1;
        } else if (temp2->next != NULL) {
            current = temp2;    
        }*/
        //for first list
        new->head = temp1;
        temp1 = temp1->next;
        l1->head = temp1;
        current = new->head;
        
        //for second list
        current->next = temp2;
        temp2 = temp2->next;
        l2->head = temp2;
        current = current->next;
        
        while (temp1 != NULL && temp2 != NULL) {
            //for first list
            current->next = temp1;
            temp1 = temp1->next;
            l1->head = temp1;
            current = current->next;
            
            //for second list
            current->next = temp2;
            temp2 = temp2->next;
            l2->head = temp2;
            current = current->next;
        }
    }
}

void destroy(list l) {
    link curNode = l->head;
    link prevNode = NULL;
 
    while (curNode != NULL) {
        prevNode = curNode;
        curNode = prevNode->next;
        free(prevNode);
    }
 
    free(l);
}

static void testExtra (void) {
    printf ("testing newList()\n");
    list testList = newList();
    assert (testList->head == NULL);
 
 
    printf ("testing deleteHead()\n");
    // create a 3 element list
    testList->head = malloc (sizeof (node));
    testList->head->value = 3;
    testList->head->next  = malloc (sizeof (node));
    testList->head->next->value = 1;
    testList->head->next->next  = malloc (sizeof (node));
    testList->head->next->next->value = 4;
    testList->head->next->next->next  = NULL;
 
    deleteHead (testList);
 
    assert (testList != NULL);
    assert (testList->head != NULL);
    assert (testList->head->value == 1);
    assert (testList->head->next != NULL);
    assert (testList->head->next->value == 4);
    assert (testList->head->next->next == NULL);
 
    deleteHead (testList);
 
    assert (testList != NULL);
    assert (testList->head != NULL);
    assert (testList->head->value == 4);
    assert (testList->head->next == NULL);
 
    deleteHead (testList);
 
    assert (testList != NULL);
    assert (testList->head == NULL);
 
 
    printf ("testing deleteLast()\n");
    // create a 3 element list
    testList->head = malloc (sizeof (node));
    testList->head->value = 3;
    testList->head->next  = malloc (sizeof (node));
    testList->head->next->value = 1;
    testList->head->next->next  = malloc (sizeof (node));
    testList->head->next->next->value = 4;
    testList->head->next->next->next  = NULL;
 
    deleteLast (testList);
 
    assert (testList != NULL);
    assert (testList->head != NULL);
    assert (testList->head->value == 3);
    assert (testList->head->next != NULL);
    assert (testList->head->next->value == 1);
    assert (testList->head->next->next == NULL);
 
    deleteLast (testList);
 
    assert (testList != NULL);
    assert (testList->head != NULL);
    assert (testList->head->value == 3);
    assert (testList->head->next == NULL);
 
    deleteLast (testList);
 
    assert (testList != NULL);
    assert (testList->head == NULL);
 
 
    printf ("testing concatenate()\n");
 
    // (3,2) -> (5,0)
    // create a 3 element list
    testList->head = malloc (sizeof (node));
    testList->head->value = 3;
    testList->head->next  = malloc (sizeof (node));
    testList->head->next->value = 1;
    testList->head->next->next  = malloc (sizeof (node));
    testList->head->next->next->value = 4;
    testList->head->next->next->next  = NULL;
 
    // create a two element list
    list testListB = newList();
 
    testListB->head = malloc (sizeof (node));
    testListB->head->value = 1;
    testListB->head->next  = malloc (sizeof (node));
    testListB->head->next->value = 5;
    testListB->head->next->next  = NULL;
 
    concatenate (testList, testListB);
    assert (testListB != NULL);
    assert (testListB->head == NULL);
 
    assert (testList != NULL);
    assert (testList->head != NULL);
    assert (testList->head->value == 3);
    assert (testList->head->next != NULL);
    assert (testList->head->next->value == 1);
    assert (testList->head->next->next != NULL);
    assert (testList->head->next->next->value == 4);
    assert (testList->head->next->next->next != NULL);
    assert (testList->head->next->next->next->value == 1);
    assert (testList->head->next->next->next->next != NULL);
    assert (testList->head->next->next->next->next->value == 5);
    assert (testList->head->next->next->next->next->next == NULL);
 
 
    // (5,0) -> (5,0)
    concatenate (testList, testListB);
    assert (testListB != NULL);
    assert (testListB->head == NULL);
 
    assert (testList != NULL);
    assert (testList->head != NULL);
    assert (testList->head->value == 3);
    assert (testList->head->next != NULL);
    assert (testList->head->next->value == 1);
    assert (testList->head->next->next != NULL);
    assert (testList->head->next->next->value == 4);
    assert (testList->head->next->next->next != NULL);
    assert (testList->head->next->next->next->value == 1);
    assert (testList->head->next->next->next->next != NULL);
    assert (testList->head->next->next->next->next->value == 5);
    assert (testList->head->next->next->next->next->next == NULL);
 
    // (0,5) -> (5,0)
    concatenate (testListB, testList);
    assert (testList != NULL);
    assert (testList->head == NULL);
 
    assert (testListB != NULL);
    assert (testListB->head != NULL);
    assert (testListB->head->value == 3);
    assert (testListB->head->next != NULL);
    assert (testListB->head->next->value == 1);
    assert (testListB->head->next->next != NULL);
    assert (testListB->head->next->next->value == 4);
    assert (testListB->head->next->next->next != NULL);
    assert (testListB->head->next->next->next->value == 1);
    assert (testListB->head->next->next->next->next != NULL);
    assert (testListB->head->next->next->next->next->value == 5);

    assert (testListB->head->next->next->next->next->next == NULL);
 
    printf ("testing count42s()\n");
    assert (count42s (testList) == 0);
    assert (count42s (testListB) == 0);
 
    testListB->head->value = 42;
    assert (count42s (testListB) == 1);
 
    testListB->head->next->value = 42;
    assert (count42s (testListB) == 2);
 
    testListB->head->next->next->value = 42;
    assert (count42s (testListB) == 3);
 
    testListB->head->next->next->next->value = 42;
    assert (count42s (testListB) == 4);
 
    testListB->head->next->next->next->next->value = 42;
    assert (count42s (testListB) == 5);

    return;
}
