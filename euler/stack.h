#ifndef STACK_H
#define STACK_H
#include <string.h> /* memset */

#define TYPE int
#define STACK_SIZE 256

/* 1k */
struct stack_t
{
	TYPE array[STACK_SIZE];
	int pos; /* 下一次存放数据的位置　*/
};

void stack_init(struct stack_t* stk)
{
	memset(stk, 0, sizeof(struct stack_t));
}

int stack_empty(struct stack_t* stk)
{
	return (stk->pos == 0);
}

void stack_push(struct stack_t* stk, TYPE data)
{
	stk->array[stk->pos++] = data;
}

TYPE stack_pop(struct stack_t* stk)
{
	stk->pos--;
	return stk->array[stk->pos];
}

void stack_clear(struct stack_t* stk)
{
	stk->pos = 0;
}

#endif