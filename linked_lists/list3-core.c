/*
 * Andrew Timkov
 * Date: 15/5/17
 * My Linked List Attempt
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list3.h"

int main (int argc, char *argv[]) {
	list newList = malloc(sizeof(list));
	link ptrToFirst = malloc(sizeof(node));
	ptrToFirst->value = 1;
	ptrToFirst->next = NULL;
	newList->head = ptrToFirst;
	frontInsert(newList, 2);
	frontInsert(newList, 3);
	showList(newList);
	printf("There are %d items\n", numItems(newList));
	append(newList, 0);
	showList(newList);
	printf("There are %d items\n", numItems(newList));
	printf("%d is at position 2\n", lookup(newList,2));

	return EXIT_SUCCESS;
}

void showList (list listToPrint) {
	link current = listToPrint->head;
	while (current->next != NULL) {
		printf("[%d] -> ", current->value);
		current = current->next;
	}
	printf("[%d]\n", current->value);
	return;
}

void frontInsert (list l, int item) {
	link ptrToNode = malloc(sizeof(node));
	ptrToNode->value = item;
	ptrToNode->next = l->head;
	l->head = ptrToNode;

	return;
}

int numItems (list l) {
	int num = 1;
	link current = l->head;
	while (current->next != NULL) {
		num++;
		current = current->next;
	}

	return num;
}

void append (list l, int value) {
	link current = l->head;
	while (current->next != NULL) {
		current = current->next;
	}
	link ptrToNode = malloc(sizeof(node));
	ptrToNode->value = value;
	ptrToNode->next = NULL;
	current->next = ptrToNode;
	return;
}

int lookup (list l, int position) {
	int value = 0;
	int i = 0;
	link current = l->head;
	while (current->next != NULL) {
		if (i == position) {
			value = current->value;
		}
		i++;
		current = current->next;
	}

	return value;
}