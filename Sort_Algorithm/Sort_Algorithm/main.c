#include <stdio.h>

#include "heap_sort.h"

void print_array(int* array, int size)
{
	int i = 0;
	for (; i < size; ++i)
	{
		printf("%d ", array[i]);
	}

	printf("\n");
}

int main()
{
	int array[10] = {0, 9, 8, 7, 100, 5, 4, 90, 2, 1};

	heap_sort(array, 9);

	print_array(array+1, 9);

	system("pause");
	return 0;
}