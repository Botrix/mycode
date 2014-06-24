#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "my_vector.h"

#define __gcc_likely(x)   __builtin_expect(!!(x), 1)
#define __gcc_unlikely(x) __builtin_expect(!!(x), 0) 

struct my_vector
{
	void* data_beg;
	void* data_end;
	void* cur_pos;	/* where you can write */
	uint32_t item_size;
	uint32_t item_capacity;
	uint32_t item_count;
	
};

static void init_my_vector(struct my_vector* vec)
{
	memset(vec, 0, sizeof(*vec));
}

struct my_vector* 
vector_create(const uint32_t item_size, const uint32_t item_capacity)
{
	uint32_t mem_size = 0;
	struct my_vector* vec = (struct my_vector*)malloc(sizeof(struct my_vector));
	if (NULL == vec)
	{
		perror("(struct my_vector*)malloc(sizeof(struct my_vector))");
		goto err_exit;
	}

	init_my_vector(vec);

	mem_size = item_size * item_capacity;

	vec->data_beg = malloc(mem_size);
	if (NULL == vec->data_beg)
	{
		goto err_exit;
	}

	vec->data_end = (char*)vec->data_beg + mem_size;
	vec->item_count = 0;
	vec->item_capacity = item_capacity;
	vec->item_size = item_size;
	vec->cur_pos = vec->data_beg;

	return vec;
err_exit:
	if (vec != NULL)
	{
		free(vec);
	}
	
	return NULL;
}

int _vector_expand(struct my_vector* vec, const uint32_t new_capacity)
{
	uint32_t new_size = 0;
	
	if (new_capacity <= vec->item_capacity)
	{
		return 0;
	}

	new_size = new_capacity * vec->item_size;

	if (__gcc_unlikely(new_size < (vec->item_count * vec->item_size)))
	{
		fprintf(stderr, "vec->item_count overflow!!!!!\n");
		return -1;
	}
	else
	{
		void* new_data = realloc(vec->data_beg, new_size);
		if (__gcc_unlikely(NULL == new_data))
		{
			fprintf(stderr, "realloc failed : new size = %u\n", new_size);
			return -1;
		}

		if (__gcc_likely(new_data == vec->data_beg))
		{
			vec->data_end = (char*)vec->data_beg + new_size;
			vec->item_capacity = new_capacity;
		}
		else
		{
			vec->cur_pos = (char*)new_data + ((char*)vec->cur_pos - (char*)vec->data_beg);
			vec->data_beg = new_data;
			vec->data_end = (char*)new_data + new_size;
			vec->item_capacity = new_capacity;
		}
	}

	return 0;
}

int
vector_reserve(struct my_vector* vec, const uint32_t size)
{
	if (size > vec->item_capacity)
	{
		return _vector_expand(vec, size);
	}
	
	return 0;
}

int 
vector_push_back(struct my_vector* vec, void* data, const uint32_t size)
{
	int ret = 0;
#ifdef DEBUG
	if (size > vec->item_size)
	{
		fprintf(stderr, "vector_push_back : invalid argument : size\n");
		return -1;
	}
#endif

	if (__gcc_unlikely((char*)vec->cur_pos >= (char*)vec->data_end))
	{
		uint32_t new_capacity = (vec->item_capacity << 1);
		ret = _vector_expand(vec, new_capacity);
		if (ret != 0)
		{
			return -1;
		}
	}

	memcpy(vec->cur_pos, data, size);
	vec->cur_pos = (char*)vec->cur_pos + vec->item_size;
	vec->item_count++;
	
	return 0;
}

void* 
vector_at(struct my_vector* vec, const uint32_t idx)
{
	char* pos = (char*)vec->data_beg + idx * vec->item_size;

	if (__gcc_unlikely((char*)pos >= (char*)vec->data_end))
	{
		return NULL;
	}

	return pos;
}

uint32_t
vector_size(struct my_vector* vec)
{
	return vec->item_count;
}

void
vector_clear(struct my_vector* vec)
{
	vec->cur_pos = vec->data_beg;
	vec->item_count = 0;
}

void
vector_release(struct my_vector* vec)
{
	if (NULL == vec) return;
	if (vec->data_beg) free(vec->data_beg);
	if (vec) free(vec);
}
