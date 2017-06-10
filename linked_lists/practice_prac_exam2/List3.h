//Written by Michael Simarta
//With the help of Bernice Chen
//header file for a Linked List structure
//contains functions that can be implemented on a Linked List
 
typedef struct _node* Node;
typedef struct _list* List;
 
 
//returns a new list of length 0
List newList();
//frees everything malloced for the list
void destroy(List l);
//appends a node of value to the end of the list
void append(List l, int value);
//returns the int value of the node at index
//assumes input index is within range of the list's length
int getValue(List l, int index);
 
int getValueReverse(List l, int index);
 
//Task 1: Insert so that when printed from the head to the tail, it is in increasing order
//(The above was probably practiced before)
 
//Task 2: Insert so that when printed from the tail to the head, it is in decreasing order
void insert(List l, int value);
 
//makes a new node
Node newNode (int value);
 
//print List from head to tail
void printList(List l);
 
//print List from tail to head
void printListReverse(List l);