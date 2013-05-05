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
* 初始化page cache
*/
extern int init_page_cache(void);

/*
* 释放page cache
*/
extern void release_page_cache(void);

/*
* 添加一个新的页数据
*/
extern void add_page(struct page_t* page);

/*
* 获取一个页数据用于分析,page中的buf必须已经准备好
*/
extern void get_page(struct page_t* page);

#ifdef  __cplusplus
}
#endif

#endif