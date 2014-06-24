#ifndef RSS_CONVEYOR_H
#define RSS_CONVEYOR_H

#include <stdint.h>

struct conveyor;

struct conveyor* 
conveyor_create(const uint32_t item_count, const uint32_t item_size);

void
conveyor_get(struct conveyor*, int* type, void**);

void
conveyor_commit(struct conveyor*);

void
conveyor_put(struct conveyor*, int type, void* data, const uint32_t size);

void
conveyor_destroy(struct conveyor*);

#endif
