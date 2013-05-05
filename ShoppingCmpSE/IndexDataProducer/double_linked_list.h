#ifndef SCSE_DOUBLE_LINKED_LIST_H
#define SCSE_DOUBLE_LINKED_LIST_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct list_node
{
	int data;

	struct list_node* next;
	struct list_node* prev;
}list_node;

void init_list_head(struct list_node* head)
{
	if (head != NULL)
	{
		head->next = head;
		head->prev = head;

		head->data = -1;
	}
}

void init_list_node(struct list_node* node)
{
	if (node != NULL)
	{
		node->next = NULL;
		node->prev = NULL;

		node->data = -1;
	}
}

void _list_add(struct list_node* new_node, struct list_node* prev, struct list_node* next)
{
	next->prev = new_node;
	prev->next = new_node;
	new_node->prev = prev;
	new_node->next = next;
}

/*
*         new_node        
*            /
*  --->node--->node_after--->
*/
void list_add_before(struct list_node* new_node, struct list_node* node_after)
{
	_list_add(new_node, node_after->prev, node_after);
}

/*
*            new_node        
*                  \
*  --->node_before--->node--- 、5
‘p、>
*/
void list_add_after(struct list_node* new_node, struct list_node* node_before)
{
	_list_add(new_node, node_before, node_before->next);
}

/*
* 只是从链表中摘下来，并没有释放内存
*/
void list_del(struct list_node* node)
{
	node->next->prev = node->prev;
	node->prev->next = node->next;

	node->next = NULL;
	node->prev = NULL;
}

int list_empty(struct list_node* head)
{
	return head->next == head;
}

#ifdef  __cplusplus
}
#endif

#endif