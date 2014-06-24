#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "pkg_reasm.h"

#define __gcc_likely(x)   __builtin_expect(!!(x), 1)
#define __gcc_unlikely(x) __builtin_expect(!!(x), 0)

#define FREE_MEM(m) \
    if ((m) != NULL)    \
    {   \
        free((m));  \
        (m) = NULL; \
    }   \

#define MAX_BUF_SIZE    8192

struct pkg_reasm
{
    char* lv1_buf;
    uint32_t lv1_capacity; /* power of 2 */
    uint32_t mask;
    uint32_t read_pos;
    uint32_t write_pos;

    char* lv2_buf;
	uint32_t lv2_capacity;

    struct pkg_hdr_op hdr_op;
    uint32_t cur_pkg_size;
};

static void init_pkg_reasm(struct pkg_reasm* reasm)
{
    memset(reasm, 0, sizeof(*reasm));
}

struct pkg_reasm* pkg_reasm_create(struct pkg_hdr_op* hdr_op)
{
    struct pkg_reasm* reasm = (struct pkg_reasm*)malloc(sizeof(struct pkg_reasm));
    if (NULL == reasm)
    {
        goto err_exit;
    }
    init_pkg_reasm(reasm);
	
	memcpy(&reasm->hdr_op, hdr_op, sizeof(struct pkg_hdr_op));

	if (hdr_op->pkg_size != 0)
	{
		reasm->cur_pkg_size = hdr_op->pkg_size;
	}

    reasm->lv1_capacity = MAX_BUF_SIZE;

    while (reasm->lv1_capacity < hdr_op->max_pkg_size)
    {
        reasm->lv1_capacity = reasm->lv1_capacity << 1;
    }
    reasm->mask = reasm->lv1_capacity - 1;
    
    reasm->lv1_buf = (char*)malloc(reasm->lv1_capacity);
    if (NULL == reasm->lv1_buf)
    {
        goto err_exit;
    }

    reasm->lv2_buf = (char*)malloc(hdr_op->max_pkg_size);
    if (NULL == reasm->lv2_buf)
    {
        goto err_exit;
    }
    reasm->lv2_capacity = hdr_op->max_pkg_size;

    return reasm;
err_exit:
    FREE_MEM(reasm->lv2_buf);
    FREE_MEM(reasm->lv1_buf);
    return NULL;
}

uint32_t pkg_reasm_size(struct pkg_reasm* reasm)
{
    return (reasm->write_pos - reasm->read_pos);
}

static uint32_t pkg_reasm_room(struct pkg_reasm* reasm)
{
	return reasm->lv1_capacity - (reasm->write_pos - reasm->read_pos);
}

static inline int pkg_reasm_expand(struct pkg_reasm* reasm)
{
    char* new_buf = (char*)malloc(reasm->lv1_capacity << 1);
    if (NULL == new_buf)
    {
        return -1;
    }

    uint32_t write_pos = reasm->write_pos & reasm->mask;
    uint32_t read_pos = reasm->read_pos & reasm->mask;

    uint32_t cur_pos = 0;

    if (write_pos < read_pos)
	{
        memcpy(new_buf + cur_pos, reasm->lv1_buf + read_pos, reasm->lv1_capacity - read_pos);
        cur_pos += (reasm->lv1_capacity - read_pos);

        memcpy(new_buf + cur_pos, reasm->lv1_buf, write_pos);
        cur_pos += write_pos;
    }
    else if (write_pos > read_pos)
    {
        memcpy(new_buf + cur_pos, reasm->lv1_buf + read_pos, write_pos- read_pos);
        cur_pos += (write_pos - read_pos);
    }
	else
	{
		if (pkg_reasm_size(reasm) > 0)
		{
			memcpy(new_buf, reasm->lv1_buf, pkg_reasm_size(reasm));
			cur_pos += pkg_reasm_size(reasm);
		}
	}

    FREE_MEM(reasm->lv1_buf);

    reasm->lv1_buf = new_buf;
    reasm->lv1_capacity <<= 1;
    reasm->mask = reasm->lv1_capacity - 1;
    reasm->read_pos = 0;
    reasm->write_pos = cur_pos;

    return 0;
}

static inline void _pkg_reasm_add(struct pkg_reasm* reasm, const char* buf, uint32_t size)
{
    uint32_t write_pos = reasm->write_pos & reasm->mask;
    uint32_t read_pos = reasm->read_pos & reasm->mask;

    if (write_pos < read_pos)
    {
        memcpy(reasm->lv1_buf + write_pos, buf, size);
    }
    else if (write_pos > read_pos)
    {
		if (__gcc_likely(size <= reasm->lv1_capacity - write_pos))
		{
			 memcpy(reasm->lv1_buf + write_pos, buf, size);
		}
		else
		{
	        memcpy(reasm->lv1_buf + write_pos, buf, reasm->lv1_capacity - write_pos);
    	    memcpy(reasm->lv1_buf, buf, size - (reasm->lv1_capacity - write_pos));
		}
    }
	else
	{
		if (pkg_reasm_size(reasm) > 0)
		{
			fprintf(stderr, "No space to store!!!!!!!!!!\n");
		}
		else
		{
			memcpy(reasm->lv1_buf,  buf, size);
		}
	}

    reasm->write_pos += size;
}

static inline int cpy_to_lv2_buf(struct pkg_reasm* reasm, uint32_t size)
{
    if (size > reasm->lv2_capacity)
    {
        fprintf(stderr, "Insufficient lv2_buf!\n");
        return -1;
    }
	
	uint32_t write_pos = reasm->write_pos & reasm->mask;
    uint32_t read_pos = reasm->read_pos & reasm->mask;

    if (read_pos < write_pos)
    {
        memcpy(reasm->lv2_buf, reasm->lv1_buf + read_pos, size);
    }
    else if (read_pos > write_pos)
    {
		if (__gcc_likely(size <= (reasm->lv1_capacity - read_pos)))
		{
			 memcpy(reasm->lv2_buf, reasm->lv1_buf + read_pos, size);
		}
		else
		{
	        uint32_t pos = 0;
	        memcpy(reasm->lv2_buf + pos, reasm->lv1_buf + read_pos, reasm->lv1_capacity - read_pos);
	        pos += (reasm->lv1_capacity - read_pos);
	        memcpy(reasm->lv2_buf + pos, reasm->lv1_buf, size - (reasm->lv1_capacity - read_pos));
		}
    }
	else
	{
		memcpy(reasm->lv2_buf, reasm->lv1_buf, size);
	}

    return 0;
}

int pkg_reasm_add(struct pkg_reasm* reasm, const char* buf, uint32_t size)
{
    int ret = 0;
    if (__gcc_unlikely(size > pkg_reasm_room(reasm)))
    {
        ret = pkg_reasm_expand(reasm);
        if (ret != 0)
        {
            return -1;
        }
    }

    _pkg_reasm_add(reasm, buf, size);

    return 0;    
}

char* pkg_reasm_get(struct pkg_reasm* reasm)
{
	int ret = 0;
	char* pkg_start = NULL;
	uint32_t write_pos = reasm->write_pos & reasm->mask;
    uint32_t read_pos = reasm->read_pos & reasm->mask;

    if (__gcc_likely((reasm->cur_pkg_size > 0)
        && (pkg_reasm_size(reasm) >= reasm->cur_pkg_size)))
    {
        goto complete_pkg;
    }
    else if ((0 == reasm->cur_pkg_size)
		&& (pkg_reasm_size(reasm) >= reasm->hdr_op.header_size))
	{
        ret = cpy_to_lv2_buf(reasm, reasm->hdr_op.header_size);
        if (ret != 0)
        {
            return NULL;
        }
        
        reasm->cur_pkg_size = reasm->hdr_op.hdr_parser(reasm->lv2_buf);
        if (pkg_reasm_size(reasm) >= reasm->cur_pkg_size)
        {
            goto complete_pkg;
        }
    }

incomplete_pkg:
    return NULL;
    
complete_pkg:
	
	if (write_pos < read_pos)
	{
    	ret = cpy_to_lv2_buf(reasm, reasm->cur_pkg_size);
	    if (ret != 0)
	    {
	        return NULL;
	    }
		
		pkg_start = reasm->lv2_buf;
	}
	else if (write_pos > read_pos)
	{
		pkg_start = reasm->lv1_buf + reasm->read_pos;
	}
	else
	{
		pkg_start = reasm->lv1_buf;
	}
	
	//reasm->read_pos += reasm->cur_pkg_size;
	//reasm->cur_pkg_size = 0;
	
	return pkg_start;
}

uint32_t
pkg_reasm_pkg_size(struct pkg_reasm* reasm)
{
	return reasm->cur_pkg_size;
}

void
pkg_reasm_cutoff(struct pkg_reasm* reasm)
{
	reasm->read_pos += reasm->cur_pkg_size;
}

void pkg_reasm_release(struct pkg_reasm* reasm)
{
	FREE_MEM(reasm->lv1_buf);
	FREE_MEM(reasm->lv2_buf);
	init_pkg_reasm(reasm);
}
