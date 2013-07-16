#include "euler.h"

int main()
{
	struct edge_t edge_array[] = {{1,2}, {2, 3}, {3, 4}, {4,5}, {5,6}, {6,1}, {2,5}};
	//struct edge_t edge_array[1] = {{1, 2}};
	//struct edge_t edge_array[3] = {{1, 2}, {1,3}, {2,3}};
	int array[16] = {0};
	euler(edge_array, sizeof(edge_array)/sizeof(struct edge_t), array);

	system("pause");
	return 0;
}