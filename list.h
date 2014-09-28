#ifndef __LIST_H__
#define __LIST_H__

/* your list data structure declarations */
struct node {
	int value;
	struct node* next;
};

/* your function declarations associated with the list */
void list_print(const struct node *);
void list_insert(const int, struct node **);
void list_clear(struct node *);

#endif // __LIST_H__
