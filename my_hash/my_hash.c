#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "my_hash.h"
#include "list.h"

#define PAGE_SIZE 4096
#define __gcc_likely(expr__) (__builtin_expect(((expr__) ? 1 : 0), 1))
#define __gcc_unlikely(expr__) (__builtin_expect(((expr__) ? 1 : 0), 0))

#define FREE_MEM(m)	\
	if ((m) != NULL)	\
	{	\
		free((m));	\
		(m) = NULL;	\
	}	\

struct my_hash_slot
{
	/* 用于链接hash code相同的node */
	struct list_head node_list;

	/* 用于链接有数据的slot */
	struct list_head slot_list;
};

struct my_hash_node
{
	void* key;
	void* val;
	
	/* list用于解决冲突，即链式 */
	struct list_head list;
};


struct my_hash
{
	struct my_hash_attr ha;
	uint32_t mask;

	void* buf;
	uint32_t node_count;

	/* 存储有数据的节点链表， 用于快速遍历hash table */
	struct list_head slot_list;
	
	struct list_head cache_list;

	pthread_mutex_t lock;
}/*__attribute__((aligned(PAGE_SIZE)))*/;

static void
init_hash_slot(struct my_hash_slot* slot)
{
	INIT_LIST_HEAD(&slot->node_list);
	INIT_LIST_HEAD(&slot->slot_list);
}

static void 
init_hash_node(struct my_hash_node* node)
{
	memset(node, 0, sizeof(struct my_hash_node));
	INIT_LIST_HEAD(&node->list);
}

static void 
init_my_hash(struct my_hash* hash)
{
	memset(hash, 0, sizeof(*hash));
	INIT_LIST_HEAD(&hash->cache_list);
	INIT_LIST_HEAD(&hash->slot_list);
}

struct my_hash*
hash_create(struct my_hash_attr* ha)
{
	uint32_t i = 0;
	struct my_hash* hash = (struct my_hash*)malloc(sizeof(struct my_hash));
	if (NULL == hash)
	{
		goto err_exit;
	}

	init_my_hash(hash);

	memcpy(&hash->ha, ha, sizeof(*ha));

	hash->mask = ha->init_slot_count - 1;

	hash->buf = malloc(ha->init_slot_count * sizeof(struct my_hash_slot));
	if (NULL == hash->buf)
	{
		goto err_exit;
	}

	for (i = 0; i < ha->init_slot_count; i++)
	{
		struct my_hash_slot* slot = (struct my_hash_slot*)hash->buf + i;
		init_hash_slot(slot);
	}

	pthread_mutex_init(&hash->lock, 0);
	return hash;
err_exit:
	FREE_MEM(hash->buf);
	FREE_MEM(hash);
	return NULL;
}

uint32_t
hash_size(struct my_hash* hash)
{
	return hash->node_count;
}

static struct my_hash_node* get_free_node(struct my_hash* hash)
{
	struct my_hash_node* new_node = NULL;
	
	if (__gcc_unlikely(list_empty(&hash->cache_list)))
	{
		new_node = (struct my_hash_node*)malloc(sizeof(struct my_hash_node));
	}
	else
	{
		new_node = list_entry(hash->cache_list.next, struct my_hash_node, list);
		list_del(hash->cache_list.next);
	}

	return new_node;
}

int
hash_add(struct my_hash* hash, void* key, void* val)
{
	uint32_t idx = hash->ha.hash_func(key);

	pthread_mutex_lock(&hash->lock);

	struct my_hash_slot* slot = (struct my_hash_slot*)hash->buf + idx;
	
	struct my_hash_node* new_node = get_free_node(hash);

	if (__gcc_unlikely(NULL == new_node))
	{
		goto err_exit;
	}

	init_hash_node(new_node);
	new_node->key = hash->ha.hash_key_clone_func(key);
	new_node->val = val;

#ifdef DEBUG
	struct list_head* pos = NULL;
	list_for_each(pos, &slot->node_list)
	{
		struct my_hash_node* node = list_entry(pos, struct my_hash_node, list);

		if (hash->ha.hash_key_cmp_func(node->key, key))
		{
			fprintf(stderr, "Duplicated key!\n");
			goto err_exit;
		}
	}
#endif

	/* 如果这个位置没有数据，现在有了数据，则将其加入到data_list中 */
	if (__gcc_likely(list_empty(&slot->node_list)))
	{
		list_add_tail(&slot->slot_list, &hash->slot_list);
	}

	list_add_tail(&new_node->list, &slot->node_list);

	hash->node_count++;

	pthread_mutex_unlock(&hash->lock);
	return 0;
err_exit:
	pthread_mutex_unlock(&hash->lock);
	return -1;
}

void*
hash_get(struct my_hash* hash, void* key)
{
	uint32_t idx = hash->ha.hash_func(key);

	struct my_hash_slot* slot = (struct my_hash_slot*)hash->buf + idx;

	struct list_head* pos = NULL;
	list_for_each(pos, &slot->node_list)
	{
		struct my_hash_node* node = list_entry(pos, struct my_hash_node, list);

		if (hash->ha.hash_key_cmp_func(node->key, key))
		{
			return node->val;
		}
	}

	return NULL;
}

void
hash_del(struct my_hash* hash, void* key)
{
	uint32_t idx = hash->ha.hash_func(key);

	pthread_mutex_lock(&hash->lock);

	struct my_hash_slot* slot = (struct my_hash_slot*)hash->buf + idx;
	struct list_head* head = &slot->node_list;

	struct list_head* pos = NULL;
	struct list_head* p = NULL;
	list_for_each_safe(pos, p, head)
	{
		struct my_hash_node* node = list_entry(pos, struct my_hash_node, list);

		if (hash->ha.hash_key_cmp_func(node->key, key))
		{
			list_del(pos);
			hash->node_count--;
			break;
		}
	}

	/* 如果这个位置数据都删除掉了，则把这个节点从data_list中删除 */
	if (__gcc_likely(list_empty(head)))
	{
		list_del(&slot->slot_list);
	}

	pthread_mutex_unlock(&hash->lock);
}

void*
hash_begin(struct my_hash* hash, struct my_hash_ite* ite)
{
	struct list_head* 	cur_node_list_head = NULL;
	struct my_hash_slot* cur_slot = NULL;
	struct my_hash_node* cur_node = NULL;
	struct list_head* 	slot_list_head = NULL;

	slot_list_head = &hash->slot_list;
	
	if (__gcc_unlikely(list_empty(slot_list_head)))
	{
		ite->slot_pos = NULL;
		return NULL;
	}

	ite->slot_pos = slot_list_head->next;

	cur_slot = list_entry(ite->slot_pos, struct my_hash_slot, slot_list);

#ifdef DEBUG
	if (list_empty(cur_slot->node_list))
	{
		fprintf(stderr, "ERROR!!! %s + %d\n", __FILE__, __LINE__);
	}
#endif

	cur_node_list_head = &cur_slot->node_list;
	cur_node_list_head = cur_node_list_head->next;

	ite->node_pos = cur_node_list_head;

	cur_node = list_entry(cur_node_list_head, struct my_hash_node, list);

	return cur_node->val;
}

void*
hash_next(struct my_hash_ite* ite)
{
	struct my_hash_node* node = NULL;
	struct list_head* cur_slot_pos = ite->slot_pos;
	struct list_head* cur_node_pos = ite->node_pos;

	struct my_hash_slot* cur_slot = NULL;

	cur_slot = list_entry(cur_slot_pos, struct my_hash_slot, slot_list);

	/* 这个slot已经遍历完， 可以切换到下一个slot */
	if (cur_node_pos->next == &cur_slot->node_list)
	{
		cur_slot_pos = cur_slot_pos->next;

		cur_slot = list_entry(cur_slot_pos, struct my_hash_slot, slot_list);

		ite->slot_pos = &cur_slot->slot_list;
		ite->node_pos = cur_slot->node_list.next;
	}
	else
	{
		ite->node_pos = ite->node_pos->next;
	}

	node = list_entry(ite->node_pos, struct my_hash_node, list);

	return node->val;
}

void*
hash_last(struct my_hash* hash)
{
	struct list_head* 	cur_node_list_head = NULL;
	struct my_hash_slot* cur_slot = NULL;
	struct my_hash_node* cur_node = NULL;
	struct list_head* 	slot_list_head = NULL;

	slot_list_head = &hash->slot_list;
	
	if (__gcc_unlikely(list_empty(slot_list_head)))
	{
		return NULL;
	}

	slot_list_head = slot_list_head->prev;

	cur_slot = list_entry(slot_list_head, struct my_hash_slot, slot_list);

#ifdef DEBUG
	if (list_empty(cur_slot->node_list))
	{
		fprintf(stderr, "ERROR!!! %s + %d\n", __FILE__, __LINE__);
	}
#endif

	cur_node_list_head = &cur_slot->node_list;
	cur_node_list_head = cur_node_list_head->prev;

	cur_node = list_entry(cur_node_list_head, struct my_hash_node, list);

	return cur_node->val;
}

static void release_slot(struct list_head* slot_list_head)
{
	struct list_head* pos;
	struct list_head* p;

	struct list_head* cur_node_list_head = NULL;
	struct my_hash_slot* cur_slot = NULL;

	cur_slot = list_entry(slot_list_head, struct my_hash_slot, slot_list);
	cur_node_list_head = &cur_slot->node_list;
	

	list_for_each_safe(pos, p, cur_node_list_head)
	{
		struct my_hash_node* node = list_entry(pos, struct my_hash_node, list);

		list_del(pos);
		FREE_MEM(node->val);
		FREE_MEM(node);
	}
}

void
hash_release(struct my_hash* hash)
{
	struct list_head* slot_list_head = NULL;
	struct list_head* pos;
	
	if (NULL == hash) return;

	slot_list_head = &hash->slot_list;

	list_for_each(pos, slot_list_head)
	{
		release_slot(pos);

		list_del(pos);
	}

	FREE_MEM(hash->buf);

	pthread_mutex_destroy(&hash->lock);
	FREE_MEM(hash);
}
