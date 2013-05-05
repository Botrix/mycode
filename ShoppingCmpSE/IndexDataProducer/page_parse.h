#ifndef SCSE_PAGE_PARSE_H 
#define SCSE_PAGE_PARSE_H
/*
* 说明：解析页面的接口
* 
* 作者：huangjia/jiangfeng
* 修改：
* [2012.10.03][初稿]
*/

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include "base_def.h"
#include "page_buf.h"
#include "page_cache.h"

typedef struct pattern
{
	char* left;
	char* right;
} pattern;

static void init_pattern(struct pattern* p)
{
	memset(p, 0, sizeof(*p));
}

enum match_direction
{
	LEFT = 0,
	RIGHT
};

typedef struct page_parse_task
{
    int find_flag;          // 0:不用找标记   1:要找标记
    char* flag_str;         // 先找到该标记，然后从该标记后面再找
	char* url;
	pattern pt_find;
	enum match_direction direction;

} page_parse_task;

static void init_parse_task(struct page_parse_task* p)
{
	memset(p, 0, sizeof(*p));
}

extern int parse_page_by_url(struct page_parse_task* tsk, char* buf, int buf_len);

extern int parse_page_by_buf(struct page_parse_task* tsk, page_buf_t page_buf, char* buf, int buf_len);

extern int parse_page_info(struct page_t* page, struct mobile_info* info);

#ifdef  __cplusplus
}
#endif

#endif