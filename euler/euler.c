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

static void cpy_list(struct point_t* dst, struct point_t* src)
{
	while (src != NULL)
	{
		list_add_tail(dst, src->val);
		src = src->next;
	}
}
struct my_edge_t
{
	struct edge_t edge;
	int walk;
};

/*邻接表*/
struct ALGraph
{
	int point_num;
	int edge_num;
	struct point_t* point_array;
	struct my_edge_t* edge_array;
};

static void init_ALGraph(struct ALGraph* alg)
{
	memset(alg, 0, sizeof(struct ALGraph));
}

static int if_walk_through(struct ALGraph* alg)
{
	int i = 0;

	for (i = 0; i < alg->edge_num; ++i)
	{
		if (alg->edge_array[i].walk == 0)
		{
			return 0;
		}
	}

	return 1;
}

static void cpy_ALGraph(struct ALGraph* dst, struct ALGraph* src)
{
	int i = 0;

	dst->point_array = (struct point_t*)malloc(src->point_num * sizeof(struct point_t));
	dst->edge_array = (struct my_edge_t*)malloc(src->edge_num * sizeof(struct my_edge_t));

	dst->point_num = src->point_num;

	for (i = 0 ; i < src->point_num; i++)
	{
		dst->point_array[i].val = src->point_array[i].val;
		dst->point_array[i].next = NULL;

		cpy_list(dst->point_array + i, src->point_array[i].next);
	}

	dst->edge_num = src->edge_num;

	for (i = 0; i < src->edge_num; i++)
	{
		dst->edge_array[i] = src->edge_array[i];
	}
}

/*找到则返回下标，否则返回-1*/
static int find_point(struct ALGraph* alg, int val)
{
	int i = 0;
	for (i = 0; i < alg->point_num; ++i)
	{
		if (val == alg->point_array[i].val)
		{
			return i;
		}
	}

	return -1;
}

static int find_edge(struct ALGraph* alg, int left, int right)
{
	int i = 0;
	for (i = 0; i < alg->edge_num; ++i)
	{
		if ((left == alg->edge_array[i].edge.left)
			&& (right == alg->edge_array[i].edge.right))
		{
			return i;
		}

		if ((right == alg->edge_array[i].edge.left)
			&& (left == alg->edge_array[i].edge.right))
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
	for (i = 0; i < alg->edge_num; i++)
	{
		fprintf(stdout, "%d---%d\n", alg->edge_array[i].edge.left, alg->edge_array[i].edge.right);
	}

	for (i = 0; i < alg->point_num; i++)
	{
		print_list_point(alg->point_array + i);
		fprintf(stdout, "\n");
	}
}

static void create_ALGraph(struct ALGraph* alg, struct edge_t* edge_array, int edge_num)
{
	int i = 0;

	alg->point_array = (struct point_t*)malloc(edge_num * 2 * sizeof(struct point_t));
	if (NULL == alg->point_array)
	{
		fprintf(stdout, "malloc failed!\n");
	}

	alg->edge_array = (struct my_edge_t*)malloc(edge_num * sizeof(struct my_edge_t));
	if (NULL == alg->edge_array)
	{
		fprintf(stdout, "malloc failed!\n");
	}

	alg->point_num = 0;
	alg->edge_num = edge_num;
	
	/* 先搞定数组 */
	for (i = 0; i < edge_num; i++)
	{
		int left = edge_array[i].left;
		int right = edge_array[i].right;

		alg->edge_array[i].edge.left = edge_array[i].left;
		alg->edge_array[i].edge.right = edge_array[i].right;
		alg->edge_array[i].walk = 0;

		if (-1 == find_point(alg, left))
		{
			alg->point_array[alg->point_num].val = left;
			alg->point_array[alg->point_num].next = NULL;
			alg->point_num++;
		}

		if (-1 == find_point(alg, right))
		{
			alg->point_array[alg->point_num].val = right;
			alg->point_array[alg->point_num].next = NULL;
			alg->point_num++;
		}
	}

	/*从小到大排序*/
	qsort(alg->point_array, alg->point_num, sizeof(struct point_t), cmp_point);

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

		list_add_tail(alg->point_array + idx, right);

		/*right*/
		idx = find_point(alg, right);

		if (-1 == idx)
		{
			fprintf(stderr, "find_point failed!\n");
		}

		list_add_tail(alg->point_array + idx, left);
	}
}

static void print_stack(struct stack_t* stk)
{
	int i = 0;

	fprintf(stdout, "\npath:\n");
	for (i = 0; i < stk->pos; i++)
	{
		fprintf(stdout, "%d -> ", stk->array[i]);
	}
}

void walk(int point, struct stack_t* stk, struct ALGraph* _alg)
{
	int idx = -1;
	struct point_t* next_point = NULL;

	struct ALGraph* alg = (struct ALGraph*)malloc(sizeof(struct ALGraph));
	init_ALGraph(alg);
	cpy_ALGraph(alg, _alg);
	
	idx = find_point(alg, point);

	if (-1 == idx)
	{
		fprintf(stderr, "find_point THIS CAN NOT HAPPEN!\n");
		exit(-1);
	}

	next_point = alg->point_array[idx].next;

	while (next_point != NULL)
	{
		idx = find_edge(alg, point, next_point->val);

		if (-1 == idx)
		{
			fprintf(stderr, "find_edge THIS CAN NOT HAPPEN!\n");
			exit(-1);
		}

		if (!alg->edge_array[idx].walk)
		{
			alg->edge_array[idx].walk = 1;

			stack_push(stk, point);

			/*是否全部走完*/
			if (if_walk_through(alg))
			{
				print_stack(stk);

				stack_pop(stk);
				return;
			}
			else
			{
				walk(next_point->val, stk, alg);
			}
		}

		next_point = next_point->next;
	}

	stack_pop(stk);
	return;
}

/*
 * edge_array : 所有的边
 * edge_num : 边个数
 * result : 结果数组，把经过的点放入其中
 */
int euler(struct edge_t* edge_array, int edge_num, int* result)
{
	struct ALGraph algraph;
	int i = 0;
	struct stack_t stk;
	stack_init(&stk);

	init_ALGraph(&algraph);
	create_ALGraph(&algraph, edge_array, edge_num);
	print_ALGraph(&algraph);

	for (i = 0; i < algraph.point_num; i++)
	{
		walk(algraph.point_array[i].val, &stk, &algraph);
	}
// 
// 	init_ALGraph(&algraph2);
// 	algraph2.point_array = (struct point_t*)malloc(algraph.point_num * sizeof(struct point_t));
// 	algraph2.edge_array = (struct my_edge_t*)malloc(edge_num * sizeof(struct my_edge_t));
// 
// 	cpy_ALGraph(&algraph2, &algraph);
// 	print_ALGraph(&algraph2);

	return 0;
}