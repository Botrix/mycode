#ifndef RSS_MY_HASH_H
#define RSS_MY_HASH_H
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* ��hash table �����̰߳�ȫ�ģ��������� */
struct my_hash;

/* �������ٱ���hash table�ĵ����� */
struct my_hash_ite
{
	struct list_head* slot_pos;
	struct list_head* node_pos;
};

struct my_hash_attr
{
	/* my_hash�ڲ�Ҫmalloc&copyһ��key */
	void* (* hash_key_clone_func)(void*);

	/* ����hash code�ĺ��� */
	uint32_t (* hash_func)(void*);

	/* �Ƚ�hash key�ĺ��������ڴ���hash��ͻ */
	uint32_t (* hash_key_cmp_func)(void*, void*);

	/* ��ʼʱ��hash slot������������2���� */
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
