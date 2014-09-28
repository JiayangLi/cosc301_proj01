/*
 * Project 1 Data scanner and listifier
 * Jiayang Li
 * 9-28-2014
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*print all values in a given linked list*/
void list_print(const struct node *list){
	while (list != NULL){
		printf("%d\n", list->value);
		list = list->next;
	}
}

/*free every node in a given linked list*/
void list_clear(struct node *list){
	while (list != NULL){
		struct node *temp = list;
		list = list->next;
		free(temp);
	}
}

/*insert a given value into a given linked list in numeric (ascending) order*/
void list_insert(const int val, struct node **head){
	struct node *addition = malloc(sizeof(struct node));
	addition->value = val;
	addition->next = NULL;

	//when given an empty linked list
	if (*head == NULL){	
		*head = addition;
		return;
	}
	
	//when the new node, addition, should be the new head
	if ((*head)->value >= val){
		addition->next = *head;
		*head = addition;
		return;
	}

	//general case
	struct node *curr = *head;
	struct node *nxt = curr->next;

	while (nxt != NULL){
		//insert addition before the first bigger node
		if (nxt->value >= val){
			curr->next = addition;
			addition->next = nxt;
			return;
		}

		curr = nxt;
		nxt = curr->next;
	}

	//addition is the biggest node, hence the last node
	curr->next = addition;
	addition->next = NULL;
}
