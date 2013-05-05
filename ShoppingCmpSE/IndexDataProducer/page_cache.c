#include <stdio.h>
#include "page_cache.h"
#include "double_linked_list.h"
#include "pthread.h"
#include "semaphore.h"
#include "base_def.h"

#define DEFAULT_PAGE_SIZE MAX_PAGE_SIZE	// 每个页数据默认512K
#define DEFAULT_PAGE_COUNT 128			// 默认缓存128个页

/*
* 空闲的页的下标组成的链表
*/
static list_node* list_free = NULL;
static pthread_spinlock_t list_free_spin;
/*
* busy的页的下标组成的链表
*/
static list_node* list_busy = NULL;
static pthread_spinlock_t list_busy_spin;

/*缓存表*/
static page_t page_array[DEFAULT_PAGE_COUNT];

static sem_t sema_empty;
static sem_t sema_full;


void init_page(struct page_t* page)
{
	memset(page, 0, sizeof(struct page_t));
}

void clone_page(struct page_t* page_dst, struct page_t* page_src)
{
	memcpy(page_dst->page_buf, page_src->page_buf, DEFAULT_PAGE_SIZE + PAGE_HEAD_LEN);
	memcpy(page_dst->url, page_src->url, URL_LEN);
	memcpy(page_dst->img_url, page_src->img_url, URL_LEN);
	page_dst->judge_count = page_src->judge_count;
	page_dst->good_judge_rate = page_src->good_judge_rate;
	page_dst->price = page_src->price;
	//strcpy_s(page_dst->price_url, URL_LEN, page_src->price_url);
}

static int init_free_busy_lists(void)
{
	int i = 0;

	list_free = (list_node*)malloc(sizeof(list_node));
	if (NULL == list_free)
	{
		goto err_out;
	}

	list_busy = (list_node*)malloc(sizeof(list_node));
	if (NULL == list_busy)
	{
		goto err_out;
	}

	init_list_node(list_free);
	init_list_node(list_busy);
	init_list_head(list_free);
	init_list_head(list_busy);

	/*初始情况下，所有页面为空闲的*/
	for (i = 0; i < DEFAULT_PAGE_COUNT; ++i)
	{
		list_node* new_node = (list_node*)malloc(sizeof(list_node));
		if (NULL == new_node)
		{
			goto err_out;
		}

		init_list_node(new_node);
		init_list_head(new_node);

		new_node->data = i;

		list_add_before(new_node, list_free);
	}

	return 0;
err_out:
	if (list_free != NULL)
	{
		free(list_free);
		list_free = NULL;
	}

	if (list_busy != NULL)
	{
		free(list_busy);
		list_busy = NULL;
	}

	/*释放链表*/

	return -1;
}

int init_page_cache(void)
{
	int i = 0;
	int ret = 0;

	for (i = 0; i < DEFAULT_PAGE_COUNT; ++i)
	{
		init_page(&page_array[i]);
		page_array[i].page_buf = create_page_buf(DEFAULT_PAGE_SIZE, PRO_NONE);
		if (NULL == page_array[i].page_buf)
		{
			goto err_out;
		}
	}

	init_free_busy_lists();

	ret = sem_init(&sema_empty, 0, DEFAULT_PAGE_COUNT);
	if (-1 == ret)
	{
		fprintf(stderr, "sem_init sema_empty failed!\n");
		goto err_out;
	}

	ret = sem_init(&sema_full, 0, 0);
	if (-1 == ret)
	{
		fprintf(stderr, "sem_init sema_full failed!\n");
		goto err_out;
	}

	pthread_spin_init(&list_free_spin, 0);
	pthread_spin_init(&list_busy_spin, 0);
	return 0;

err_out:
	release_page_cache();
	return -1;
}

void release_page_cache(void)
{
	int i = 0;
	
	sem_destroy(&sema_empty);

	pthread_spin_destroy(&list_free_spin);
	pthread_spin_destroy(&list_busy_spin);

	for (i = 0; i < DEFAULT_PAGE_COUNT; ++i)
	{
		release_page_buf(&page_array[i].page_buf);
	}

	/*释放链表 or memleak*/
	while (list_free != NULL)
	{
		list_node* p = list_free;
		list_free = p->next;

		free(p);
		p = NULL;
	}

	while (list_busy != NULL)
	{
		list_node* p = list_busy;
		list_busy = p->next;

		free(p);
		p = NULL;
	}
}


void add_page(struct page_t* page)
{
	list_node* node = NULL;
	int index = 0;

	if (NULL == page
		|| NULL == page->page_buf)
	{
		return;
	}

	sem_wait(&sema_empty);

	pthread_spin_lock(&list_free_spin);

	node = list_free->next;
	index = node->data;
	list_del(node);

	pthread_spin_unlock(&list_free_spin);

	/* copy the page to the cache  */
	clone_page(&page_array[index], page);

	/* add the node to busy list */
	pthread_spin_lock(&list_busy_spin);
	list_add_after(node, list_busy);
	pthread_spin_unlock(&list_busy_spin);

	sem_post(&sema_full);
}

void get_page(struct page_t* page)
{
	list_node* node = NULL;
	int index = 0;

	if (NULL == page->page_buf)
	{
		return;
	}

	sem_wait(&sema_full);

	pthread_spin_lock(&list_busy_spin);

	node = list_busy->next;
	index = node->data;

	/* remove from busy list */
	list_del(node);

	pthread_spin_unlock(&list_busy_spin);

	/* copy the data from cache to page */
	clone_page(page, &page_array[index]);
	
	/* add the node to free list */
	pthread_spin_lock(&list_free_spin);
	list_add_after(node, list_free);
	pthread_spin_unlock(&list_free_spin);

	sem_post(&sema_empty);
}