#ifndef EULER_H
#define EULER_H

struct edge_t
{
	int left;
	int right;
};

/*
 * edge_array : ���еı�
 * edge_num : �߸���
 * result : ������飬�Ѿ����ĵ��������
 */
int euler(struct edge_t* edge_array, int edge_num, int* result);

#endif