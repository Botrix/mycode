#include "honeycomb.h"

#include <stdlib.h>
#include <string.h> /* for memset */

#define MAX_RING 256

/*第N环的蜂窝数为6N，N从1开始*/

struct ring_info
{
	int min_id;
	int max_id;
};

static void init_ring_info(struct ring_info* ri)
{
	memset(ri, 0, sizeof(*ri));
}

static struct ring_info g_all_ring[MAX_RING];

static void init_all_ring(void)
{
	int i = 0;
	int last_max = 0;

	g_all_ring[0].min_id = 1;
	g_all_ring[0].max_id = 1;

	last_max = 1;

	for (i = 1; i < MAX_RING; ++i)
	{
		int count = i * 6;
		g_all_ring[i].min_id = last_max + 1;
		g_all_ring[i].max_id = g_all_ring[i].min_id + count - 1;

		last_max = g_all_ring[i].max_id;
	}
}

/* 共六个方向 */
enum DIRECTION
{
	LEFT_UP = 0,
	UP,
	RIGHT_UP,
	RIGHT_DOWN,
	DOWN,
	LEFT_DOWN
};

static void move(enum DIRECTION direct, int* x, int* y)
{
	switch (direct)
	{
	case LEFT_UP:
		*x -= 1;
		*y += 1;
		break;
	case UP:
		*y += 1;
		break;
	case RIGHT_UP:
		*x += 1;
		break;
	case RIGHT_DOWN:
		*x += 1;
		*y -= 1;
		break;
	case DOWN:
		*y -= 1;
		break;
	case LEFT_DOWN:
		*x -= 1;
		break;
	default:
		break;
	}
}

struct comb_info
{
	int x;
	int y;
};

static struct comb_info* g_all_comb = NULL;
static int g_max_id = 0;

void init_honeycomb(int max_id)
{
	int i = 0;

	if (max_id <= 0)
	{
		g_max_id = 0;
		return;
	}

	g_max_id = max_id;

	init_all_ring();

	if (g_all_comb != NULL)
	{
		free(g_all_comb);
		g_all_comb = NULL;
	}

	g_all_comb = (struct comb_info*)malloc((max_id + 1) * sizeof(struct comb_info));
	if (NULL == g_all_comb)
	{
		g_max_id = 0;
		return;
	}

	g_all_comb[1].x = 0;
	g_all_comb[1].y = 0;

	for (i = 1; i < MAX_RING; i++)
	{
		int tmp = 0;
		int x = 0;
		int y = 0;
		int ring = i;
/*		int count = ring * 6;*/
		int cur_id = g_all_ring[ring].min_id;

		/* 每环的第一个元素都在上一环最后一个元素的下面 */
		x = g_all_comb[g_all_ring[ring-1].max_id].x;
		y = g_all_comb[g_all_ring[ring-1].max_id].y;

		y -= 1;

		g_all_comb[cur_id].x = x;
		g_all_comb[cur_id].y = y ;

		cur_id++;

		/* 走一圈，不同的环数，每个方向走的步数不同 */

		// 先LEFT_DOWN
		for (tmp = 0; tmp < ring - 1; tmp++)
		{
			if (cur_id > max_id)
			{
				return;
			}

			move(LEFT_DOWN, &x, &y);

			g_all_comb[cur_id].x = x;
			g_all_comb[cur_id].y = y ;

			cur_id++;
		}

		for (tmp = LEFT_UP; tmp <= DOWN; tmp++)
		{
			int j = 0;

			// 每个方向走ring步
			for (j = 0; j < ring; ++j)
			{
				if (cur_id > max_id)
				{
					return;
				}

				move(tmp, &x, &y);

				g_all_comb[cur_id].x = x;
				g_all_comb[cur_id].y = y ;

				cur_id++;
			}
		}
	}
}

int calc_distance(int id1, int id2)
{
	int d_x = 0;
	int d_y = 0;

	if (NULL == g_all_comb)
	{
		return -1;
	}

	if (id1 <= 0
		|| id2 <= 0
		|| id1 > g_max_id
		|| id2 > g_max_id)
	{
		return -1;
	}

	d_x = g_all_comb[id1].x - g_all_comb[id2].x;
	d_y = g_all_comb[id1].y - g_all_comb[id2].y;

	if (d_x*d_y < 0)
	{
		return max(abs(d_x), abs(d_y));
	}
	else
	{
		return abs(d_x) + abs(d_y);
	}
}

void clear(void)
{
	if (g_all_comb != NULL)
	{
		free(g_all_comb);
		g_all_comb = NULL;
	}

	g_max_id = 0;
}
