#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include "conveyor.h"

struct item_struct
{
	int type;
	char data[0];
};

struct conveyor
{
	void* buf;
	uint32_t item_count;
	uint32_t item_size;

	uint32_t write_pos;
	uint32_t read_pos;

	sem_t ava_write_sem;
	sem_t ava_read_sem;
};

struct conveyor* 
conveyor_create(const uint32_t item_count, const uint32_t item_size)
{
	struct conveyor* cvr = (struct conveyor*)malloc(sizeof(struct conveyor));
	if (NULL == cvr)
	{
		return NULL;
	}

	cvr->item_count = item_count;
	cvr->item_size = item_size + 4;
	cvr->read_pos = 0;
	cvr->write_pos = 0;

	cvr->buf = malloc(cvr->item_count * cvr->item_size);
	if (NULL == cvr->buf)
	{
		goto err_exit;
	}

	sem_init(&cvr->ava_read_sem, 0, 0);
	sem_init(&cvr->ava_write_sem, 0, cvr->item_count);

	return cvr;
err_exit:
	if (cvr->buf != NULL)
	{
		free(cvr->buf);
	}
	
	if (cvr != NULL)
	{
		free(cvr);
	}

	return NULL;
}

void
conveyor_get(struct conveyor* cvr, int* type, void** data)
{
	struct item_struct* p = NULL;
	sem_wait(&cvr->ava_read_sem);

	p = (struct item_struct*)((char*)cvr->buf + cvr->item_size * (cvr->read_pos & (cvr->item_count - 1)));
	cvr->read_pos++;

	*type = p->type;
	*data = p->data;
}


void
conveyor_commit(struct conveyor* cvr)
{
	sem_post(&cvr->ava_write_sem);
}

void
conveyor_put(struct conveyor* cvr, int type, void* data, const uint32_t size)
{
	struct item_struct* p = NULL;
	sem_wait(&cvr->ava_write_sem);

	p = (struct item_struct*)((char*)cvr->buf + cvr->item_size * (cvr->write_pos & (cvr->item_count - 1)));
	p->type = type;
	memcpy(p->data, data, size);

	cvr->write_pos++;
	sem_post(&cvr->ava_read_sem);
}

void
conveyor_destroy(struct conveyor* cvr)
{
	free(cvr->buf);
	sem_destroy(&cvr->ava_read_sem);
	sem_destroy(&cvr->ava_write_sem);
	free(cvr);
}
