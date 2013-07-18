#include "honeycomb.h"
#include <stdio.h>

static test(int id1, int id2, int len_expect)
{
	int len = calc_distance(id1, id2);

	if (len != len_expect)
	{
		fprintf(stdout, "FAIL  calc_distance(%d, %d) != %d\n", id1, id2, len_expect);
	}
	else
	{
		fprintf(stdout, "SUCCESS\n");
	}
}

int main()
{
	int max_id = 61;
	int len = 0;

	init_honeycomb(max_id);

	test(1, 2, 1);
	test(55, 57, 2);
	test(18, 59, 2);
	test(26, 30, 4);
	test(4, 8, 3);
	test(1, 45, 4);

	system("pause");
	return 0;
}