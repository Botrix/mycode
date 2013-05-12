#include "heap_sort.h"

/* heap sort */
/* Heap is a complete binary tree, so we can store the heap with a array. 
   To make it easier to calculate, we define the root as heap[1] NOT heap[0].
   Then the left child of heap[i] is heap[i*2], and the right child of heap[i]
   is array[2*i + 1]. */

/* To design a heap sort, we begin with the algorithm heapify_max or heapify_min . */

/* We assume that we have two max_heap heap[2i] and heap[2i + 1], now we adjust 
   the heap heap[i] to make it a max_heap. */

void heapify_max(int* heap, int size, int i)
{
	int l_child = i * 2;
	int r_child = i * 2 + 1;
	int tmp = 0; // for swap two value

	int larger = i;

	if (l_child <= size && heap[l_child] >= heap[i])
	{
		larger = l_child;
	}

	if (r_child <= size && heap[r_child] >= heap[larger])
	{
		larger = r_child;
	}

	if (larger != i)
	{
		tmp = heap[i];
		heap[i] = heap[larger];
		heap[larger] = tmp;

		heapify_max(heap, size, larger);
	}
}

void build_max_heap(int* heap, int size)
{
	int start_node = size / 2;
	
	while (start_node >= 1)
	{
		heapify_max(heap, size, start_node);
		start_node--;
	}
}

/* 从小到大排序，需要建立大顶堆 */
void heap_sort(int* heap, int size)
{
	int tmp = 0;
	/* 首先，建立大顶堆 */
	build_max_heap(heap, size);

	while (size > 1)
	{
		/* 从把heap[1]和heap[size]替换 然后调整堆 */
		tmp = heap[1];
		heap[1] = heap[size];
		heap[size] = tmp;

		heapify_max(heap, size - 1, 1);

		size--;
	}
}