#ifndef SCSE_PAGE_PARSE_H 
#define SCSE_PAGE_PARSE_H
/*
* ˵��������ҳ��Ľӿ�
* 
* ���ߣ�huangjia/jiangfeng
* �޸ģ�
* [2012.10.03][����]
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
    int find_flag;          // 0:�����ұ��   1:Ҫ�ұ��
    char* flag_str;         // ���ҵ��ñ�ǣ�Ȼ��Ӹñ�Ǻ�������
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