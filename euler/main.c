#include "euler.h"

int main()
{
	struct edge_t edge_array[5] = {{3, 4}, {4, 1}, {1,2}, {2, 3}, {2, 4}};
	int array[16] = {0};
	euler(edge_array, 5, array);

	system("pause");
	return 0;
}