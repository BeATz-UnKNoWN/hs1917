//Written by Michael Simarta
//With the help of Bernice Chen
//List2.c file that implements all functions declared in List2.h
//implement the removeUpperCase() functions here
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "List2.h"

#define FALSE 0
#define TRUE 1
 
typedef struct _node {
     char value;
     Node next;
} node;
 
typedef struct _list {
     Node head;
} list;

void showList (List l);
int isUpperCase (char letter);
void deleteNode (List l, int pos);
 
//when the removeUpperCase() function is complete,
//compile with testList2.c, type
//"gcc -Wall -Werror -O -o test testList2.c List2.c"
//to run program, type
//"./test"
 
//TODO
//Read List2.h to help you look at the hints
//remove values which are uppercase
//
//given a list X
//(Empty List)
//then removeUpperCase(list);
//the list is still X
//
//given a list a->X
//then removeUpperCase(list);
//the list is a->X
//
//given a list A->X
//then removeUpperCase(list);
//the list is X
//
//given a list A->a->X
//then removeUpperCase(list);
//the list is a->X
//
//given a list C->a->A->D->X
//then removeUpperCase(list);
//the list is a->X
//
//given a list A->a->B->b->C->c->d->D->E->e->X
//then removeUpperCase(list);
//the list is a->b->c->d->e->X
void removeUpperCase(List l) {
    ////////////////////
    //TODO
    int pos = 0;
    Node current = l->head;
    if (current == NULL) {
        // do nothing
    } else {
        while (current != NULL) {
            if (isUpperCase(current->value) == TRUE) {
                deleteNode(l, pos);
                current = l->head;
                int temp = 0;
                while (temp < pos) {
                    current = current->next;
                    temp++;
                }
            } else {

                current = current->next;
                pos++;
            }
        }
    }
}

void deleteNode (List l, int pos) {
    Node current = l->head;
    int i = 0;
    if (pos == 0) {
        l->head = l->head->next;
        free(current);
    } else {
        while (i < (pos - 1)) {
            current = current->next;
            i++;
        }
        Node next = current->next->next;
        free(current->next);
        current->next = next;
    }
}

// Node deleteNode (List l, int pos) {
//     int i = 0;
//     Node current = l->head;
//     Node temp = l->head;
//     if (pos == 0) {
//         l->head = l->head->next;
//         temp = l->head;
//     } else {
//         while (i != pos) {
//             i++;
//             if (i == pos) {
//                 temp = current;
//             }
//             current = current->next;
//         }

//     if (l->head != NULL) {
//         printf("head: %c, pos: %d\n", current->value, pos);
//     }

//         if (current->next == NULL) {
//             temp->next = NULL;
//         } else {
//             temp->next = current->next;
//         }
//     }
//     free(current);
//     return temp;
// }

int isUpperCase (char letter) {
    int isUpper = FALSE;
    if (letter >= 'A' && letter <= 'Z') {
        isUpper = TRUE;
    }
    return isUpper;
}
 
//returns a new list of length 0
List newList() {
    List l = malloc(sizeof(list));
    assert ( l!=NULL );
    l->head = NULL;
    return l;
}

void showList (List l) {
    Node current = l->head;
    while (current != NULL) {
        printf("[%c] -> ", current->value);
        current = current->next;
    }
    printf("%c\n", 'X');
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
 
//appends a node of value to the end of the list
void append(List l, char value) {
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
    showList(l);
}
 
//returns the int value of the node at index
//assumes input index is within range of the list's length
char getValue(List l, int index) {
 
    Node curNode = l->head;
    assert (curNode!=NULL);
    int counter = 0;
    while (counter<index) {
        curNode = curNode->next;
        counter++;
    }
    return curNode->value;
}