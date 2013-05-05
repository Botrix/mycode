#ifndef SCSE_PAGE_CACHE_H
#define SCSE_PAGE_CACHE_H

#ifdef  __cplusplus
extern "C" 
{
#endif

#include "page_buf.h"
#include "base_def.h"

typedef struct page_t
{
	page_buf_t page_buf;
	char url[URL_LEN];
    //char price_url[URL_LEN];
    char img_url[URL_LEN];
    int price;
	int good_judge_rate;
	int judge_count;
} page_t;

extern void init_page(struct page_t* page);

extern void clone_page(struct page_t* page_dst, struct page_t* page_src);

/*
* ��ʼ��page cache
*/
extern int init_page_cache(void);

/*
* �ͷ�page cache
*/
extern void release_page_cache(void);

/*
* ���һ���µ�ҳ����
*/
extern void add_page(struct page_t* page);

/*
* ��ȡһ��ҳ�������ڷ���,page�е�buf�����Ѿ�׼����
*/
extern void get_page(struct page_t* page);

#ifdef  __cplusplus
}
#endif

#endif