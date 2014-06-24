#ifndef RSS_MY_CACHE_H
#define RSS_MY_CACHE_H

#include <stdint.h>

struct my_cache;

struct my_cache*
my_cache_create(uint32_t init_node_count, uint32_t node_size);

void*
my_cache_get(struct my_cache*);

void
my_cache_put(struct my_cache*, void*);

void
my_cache_release(struct my_cache*);

#endif
