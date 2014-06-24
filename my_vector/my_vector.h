#ifndef RSS_MY_VECTOR_H
#define RSS_MY_VECTOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct my_vector;

extern struct my_vector*
vector_create(const unsigned int item_size, const uint32_t item_capacity);

int
vector_reserve(struct my_vector* vec, const uint32_t size);

int
vector_push_back(struct my_vector* vec, void* data, const uint32_t size);

void*
vector_at(struct my_vector* vec, const uint32_t idx);

uint32_t
vector_size(struct my_vector* vec);

void
vector_clear(struct my_vector* vec);

extern void
vector_release(struct my_vector* vec);

#ifdef __cplusplus
}
#endif

#endif
