#ifndef RSS_MY_HASH_H
#define RSS_MY_HASH_H
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* 本hash table 不是线程安全的，不可重入 */
struct my_hash;

/* 用来快速遍历hash table的迭代器 */
struct my_hash_ite
{
	struct list_head* slot_pos;
	struct list_head* node_pos;
};

struct my_hash_attr
{
	/* my_hash内部要malloc&copy一份key */
	void* (* hash_key_clone_func)(void*);

	/* 计算hash code的函数 */
	uint32_t (* hash_func)(void*);

	/* 比较hash key的函数，用于处理hash冲突 */
	uint32_t (* hash_key_cmp_func)(void*, void*);

	/* 初始时的hash slot个数，必须是2的幂 */
	uint32_t init_slot_count;
};

struct my_hash*
hash_create(struct my_hash_attr* ha);

int
hash_add(struct my_hash* hash, void* key, void* val);

void*
hash_get(struct my_hash* hash, void* key);

void
hash_del(struct my_hash* hash, void* key);

uint32_t
hash_size(struct my_hash* hash);

void*
hash_begin(struct my_hash* hash, struct my_hash_ite* ite);

void*
hash_next(struct my_hash_ite* ite);

void*
hash_last(struct my_hash* hash);

void
hash_release(struct my_hash* hash);

#ifdef __cplusplus
}
#endif

#endif
