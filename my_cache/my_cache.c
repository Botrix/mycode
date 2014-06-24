#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_cache.h"
#include "list.h"

#define __gcc_likely(expr__) (__builtin_expect(((expr__) ? 1 : 0), 1))
#define __gcc_unlikely(expr__) (__builtin_expect(((expr__) ? 1 : 0), 0))

#define FREE_MEM(m)	\
	if ((m) != NULL)	\
	{	\
		free((m));	\
		(m) = NULL;	\
	}	\

struct my_cache_node
{
	struct list_head list;
	char data[0];
};

struct my_cache
{
	uint32_t node_count;
	uint32_t node_size;
	
	struct list_head node_list;
	pthread_spinlock_t lock;
};

static struct my_cache_node* alloc_node(uint32_t node_size)
{
	uint32_t size = node_size + sizeof(struct list_head);
	struct my_cache_node* node = (struct my_cache_node*)malloc(size);
	if (NULL == node)
	{
		goto err_exit;
	}
	memset(node, 0, size);
	INIT_LIST_HEAD(&node->list);

	memset(node->data, 0x18, node_size);

	return node;
err_exit:
	FREE_MEM(node);
	return NULL;
}

static void release_node(struct my_cache_node* node)
{
	FREE_MEM(node);
}

static void release_cache_list(struct my_cache* cache)
{
	struct list_head* pos;
	struct list_head* p;

	list_for_each_safe(pos, p, &cache->node_list)
	{
		struct my_cache_node* node = list_entry(pos, struct my_cache_node, list);

		release_node(node);
	}
}

struct my_cache*
my_cache_create(uint32_t init_node_count, uint32_t node_size)
{
	int ret = 0;
	uint32_t i = 0;
	struct my_cache* cache = (struct my_cache*)malloc(sizeof(struct my_cache));
	if (NULL == cache)
	{
		goto err_exit;
	}
	memset(cache, 0, sizeof(struct my_cache));
	INIT_LIST_HEAD(&cache->node_list);

	cache->node_count = init_node_count;
	cache->node_size = node_size;

	for (i = 0; i < init_node_count; i++)
	{
		struct my_cache_node* node = alloc_node(node_size);
		if (NULL == node)
		{
			goto err_exit;
		}

		list_add_tail(&node->list, &cache->node_list);
	}

	ret = pthread_spin_init(&cache->lock, 0);
	if (ret != 0)
	{
		fprintf(stderr, "pthread_spin_init failed : %d\n", ret);
		goto err_exit;
	}

	return cache;
err_exit:
	release_cache_list(cache);
	return NULL;
}

void*
my_cache_get(struct my_cache* cache)
{
	struct my_cache_node* node = NULL;
	
	if (__gcc_unlikely(list_empty(&cache->node_list)))
	{
		node = alloc_node(cache->node_size);
	}
	else
	{
		struct list_head* head;

		pthread_spin_lock(&cache->lock);

		head = cache->node_list.next;
		node = list_entry(head, struct my_cache_node, list);
		list_del(head);

		pthread_spin_unlock(&cache->lock);
	}

	return node->data;
}

void
my_cache_put(struct my_cache* cache, void* arg)
{
	struct my_cache_node* node = NULL;

	node = list_entry(arg, struct my_cache_node, data);
	INIT_LIST_HEAD(&node->list);

	pthread_spin_lock(&cache->lock);

	/* 添加到cache list的头部，增加cache命中率 */
	list_add(&node->list, &cache->node_list);
	
	pthread_spin_unlock(&cache->lock);
}

void
my_cache_release(struct my_cache* cache)
{
	release_cache_list(cache);
	pthread_spin_destroy(&cache->lock);
	FREE_MEM(cache);
}

