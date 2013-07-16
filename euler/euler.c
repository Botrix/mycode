#include <stdlib.h>
#include <stdio.h>
#include "euler.h"
#include "stack.h"

struct point_t
{
	int val;
	struct point_t* next;
};

static void init_point(struct point_t* p)
{
	memset(p, 0, sizeof(*p));
}

static int cmp_point(const void* left, const void* right)
{
	return (((struct point_t*)left)->val > ((struct point_t*)right)->val);
}

static void list_add_tail(struct point_t* head, int val)
{
	struct point_t* new_node = (struct point_t*)malloc(sizeof(struct point_t));
	if (NULL == new_node)
	{
		fprintf(stderr, "malloc failed!\n");
		return;
	}

	init_point(new_node);
	new_node->next = NULL;
	new_node->val = val;

	while (head != NULL)
	{
		if (NULL == head->next)
		{
			head->next = new_node;
			break;
		}

		head = head->next;
	}
}

/*邻接表*/
struct ALGraph
{
	int size;
	struct point_t* array;
};

static void init_ALGraph(struct ALGraph* alg)
{
	memset(alg, 0, sizeof(struct ALGraph));
}

/*找到则返回下标，否则返回-1*/
static int find_point(struct ALGraph* alg, int val)
{
	int i = 0;
	for (i = 0; i < alg->size; ++i)
	{
		if (val == alg->array[i].val)
		{
			return i;
		}
	}

	return -1;
}

static void print_list_point(struct point_t* head)
{
	while (head != NULL)
	{
		fprintf(stdout, "%d ", head->val);
		head = head->next;
	}
}

static void print_ALGraph(struct ALGraph* alg)
{
	int i = 0;

	fprintf(stdout, "ALGraph : \n");
	for (i = 0; i < alg->size; i++)
	{
		print_list_point(alg->array + i);
		fprintf(stdout, "\n");
	}
}

static void create_ALGraph(struct ALGraph* alg, struct edge_t* edge_array, int edge_num)
{
	int i = 0;

	/* 先搞定数组 */
	for (i = 0; i < edge_num; i++)
	{
		int left = edge_array[i].left;
		int right = edge_array[i].right;

		if (-1 == find_point(alg, left))
		{
			alg->array[alg->size].val = left;
			alg->array[alg->size].next = NULL;
			alg->size++;
		}

		if (-1 == find_point(alg, right))
		{
			alg->array[alg->size].val = right;
			alg->array[alg->size].next = NULL;
			alg->size++;
		}
	}

	/*从小到大排序*/
	qsort(alg->array, alg->size, sizeof(struct point_t), cmp_point);

	/*再搞定链表*/
	for (i = 0; i < edge_num; i++)
	{
		int left = edge_array[i].left;
		int right = edge_array[i].right;

		/*left*/
		int idx = find_point(alg, left);

		if (-1 == idx)
		{
			fprintf(stderr, "find_point failed!\n");
		}

		list_add_tail(alg->array + idx, right);

		/*right*/
		idx = find_point(alg, right);

		if (-1 == idx)
		{
			fprintf(stderr, "find_point failed!\n");
		}

		list_add_tail(alg->array + idx, left);
	}
}

/*
 * edge_array : 所有的边
 * edge_num : 边个数
 * result : 结果数组，把经过的点放入其中
 */
int euler(struct edge_t* edge_array, int edge_num, int* result)
{
	struct ALGraph algraph;
	init_ALGraph(&algraph);
	algraph.array = (struct point_t*)malloc(edge_num*2*sizeof(struct point_t));
	if (NULL == algraph.array)
	{
		fprintf(stdout, "malloc failed!\n");
		return -1;
	}

	create_ALGraph(&algraph, edge_array, edge_num);
	print_ALGraph(&algraph);

	return 0;
}