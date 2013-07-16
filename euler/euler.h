#ifndef EULER_H
#define EULER_H

struct edge_t
{
	int left;
	int right;
};

/*
 * edge_array : 所有的边
 * edge_num : 边个数
 * result : 结果数组，把经过的点放入其中
 */
int euler(struct edge_t* edge_array, int edge_num, int* result);

#endif