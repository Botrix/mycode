#include <stdlib.h>
#include <stdio.h>
#include "data_product_amazon.h"
#include "base_def.h"
#include "spider.h"
#include "regex.h"
#include "page_parse.h"
#include "page_buf.h"
#include "page_cache.h"
#include "pthread.h"
#include "base_func.h"

static pthread_t mobile_thread;
static pthread_t computer_thread;

int g_amazon_mobile_product_stop;


/*获取一页中的所有的手机的链接对应的页面,添加到cache中*/
static void get_all_mobile_in_a_page(char* url)
{
	page_buf_t page_buf = NULL;
	page_buf_t page_buf_mobile = NULL;
	int ret = 0;
	char* rbuf = NULL;
	int rsize = 0;
	char* find_pt = NULL;

	char* start_pt = "<br clear=\"all\">";
	struct pattern pt;
	int mobile_count = 0;
	char* str_get = NULL;

	// 每条手机信息都是在<br clear="all">后面,每个手机的相应信息的链接在第一个链接中
	page_buf = create_page_buf(MAX_PAGE_SIZE, PRO_NONE);
	if (NULL == page_buf)
	{
		fprintf(stderr, "粗大事啦,create_page_buf失败...\n");
		goto err_exit;
	}

	ret = get_url_page_libcurl(url, page_buf);
	if (-1 == ret)
	{
		fprintf(stderr, "获取url出错...%s\n", url);
		goto err_exit;
	}

	get_rbuf_of_page_buf(page_buf, &rbuf, &rsize);

	find_pt = find(rbuf, start_pt);
	
	while (find_pt != NULL)
	{
		page_t page;

		pt.left = "<a href=\"";
		pt.right = "\" target=\"_blank\">";

		init_page(&page);
		page_buf_mobile = create_page_buf(MAX_PAGE_SIZE, PRO_MOBILE_AMAZON);
		if (NULL == page_buf_mobile)
		{
			fprintf(stderr, "粗大事啦,create_page_buf失败...\n");
			goto err_exit;
		}

		regex(find_pt, pt.left, pt.right, &str_get);
		if (NULL == str_get)
		{
			fprintf(stderr, "regex失败: \npt.left = %s, \npt.right = %s\n", pt.left, pt.right);
			release_page_buf(&page_buf_mobile);
			break;
		}
        
		mobile_count++;

		ret = get_url_page_libcurl(str_get, page_buf_mobile);
		if (-1 == ret)
		{
			fprintf(stderr, "get_url_page_libcurl失败!\n");
			release_page_buf(&page_buf_mobile);
			break;
		}

		page.page_buf = page_buf_mobile;

        if (strlen(str_get) > URL_LEN)
        {
            break;
        }

		strcpy_s(page.url, URL_LEN, str_get);

        free(str_get);
        str_get = NULL;

        // img url
        pt.left = "src=\"";
        pt.right = "\" class=\"productImage\"";

        regex(find_pt, pt.left, pt.right, &str_get);
        if (NULL == str_get)
        {
            fprintf(stderr, "amazon 获取img url失败!\n");
            break;
        }
        else
        {
            if (strlen(str_get) > URL_LEN)
            {
                free(str_get);
                str_get = NULL;
                break;
            }

            strcpy_s(page.img_url, URL_LEN, str_get);
            free(str_get);
            str_get = NULL;
        }

		add_page(&page);

		release_page_buf(&page_buf_mobile);
		
		find_pt = find(find_pt + strlen(start_pt), start_pt);
	}

	release_page_buf(&page_buf);
	return;
err_exit:

	release_page_buf(&page_buf);
	return;
}

/*********************************************************
 * 函数功能：生产手机数据
 * 作    者：huangjia/jiangfeng
 * 时    间：2012/10/14
 *********************************************************/
static void* product_mobile_data(void* p)
{
	int ret = 0;

	char buf_url_step1[URL_LEN] = {0};
	char buf_url_step2[URL_LEN] = {0};
	char buf_url_cur[URL_LEN] = {0};
	char buf_url_next[URL_LEN] = {0};
	struct page_parse_task tsk;
    page_buf_t page_buf;
    char* rbuf = NULL;
    int rsize = 0;
    char* tmp = NULL;
    char* str_find = NULL;
    char* str_get = NULL;
    char buf_left[URL_LEN] = {0};
    char buf_right[URL_LEN] = {0};

    // step 1 获取亚马逊的“所有分类”页面

	// step 1
	/* 在主页中获取“手机、数码”中的“手机”的url*/    
// 
// 	// step 2
// 	// 在"所有手机"页面中获取“所有品牌”的url
// 
// 	init_parse_task(&tsk);
// 	tsk.direction = RIGHT;
// 	tsk.pt_find.left = "href=\"";
// 	tsk.pt_find.right = "\">更多品牌</a>";
//     memset(buf_left, 0, URL_LEN);
//     memset(buf_right, 0, URL_LEN);
//     GBK2UTF8(tsk.pt_find.left, strlen(tsk.pt_find.left), buf_left, URL_LEN);
//     GBK2UTF8(tsk.pt_find.right, strlen(tsk.pt_find.right), buf_right, URL_LEN);
//     tsk.pt_find.left = buf_left;
//     tsk.pt_find.right = buf_right;
// 
// 	tsk.url = buf_url_cur;
// 	ret = parse_page_by_url(&tsk, buf_url_step2, URL_LEN);
// 	if (-1 == ret)
// 	{
// 		fprintf(stderr, "在手机、通讯页面中获取更多品牌rl出错...\n");
// 		g_amazon_mobile_product_stop = 1;
// 		goto err_exit;
// 	}
// 
//     // 获取到的url是相对的，因此需要重组
//     memset(buf_url_cur, 0, URL_LEN);
//     sprintf(buf_url_cur, "%s%s", HOME_PAGE_AMAZON, buf_url_step2);

	// step 3
	// 获取所有页中所有的手机的链接（因为有很多页的数据）
	// 寻找下一页的链接
	
	tsk.direction = RIGHT;
	tsk.pt_find.left = "href=\"";
	tsk.pt_find.right = "<span id=\"pagnNextString\">下一页</span>";
    memset(buf_left, 0, URL_LEN);
    memset(buf_right, 0, URL_LEN);
    GBK2UTF8(tsk.pt_find.left, strlen(tsk.pt_find.left), buf_left, URL_LEN);
    GBK2UTF8(tsk.pt_find.right, strlen(tsk.pt_find.right), buf_right, URL_LEN);
    tsk.pt_find.left = buf_left;
    tsk.pt_find.right = buf_right;

	strcpy_s(buf_url_cur, URL_LEN, HOME_PAGE_AMAZON);
	while (!g_amazon_mobile_product_stop)
	{
		char* find_slash = NULL;
		tsk.url = buf_url_cur;

		get_all_mobile_in_a_page(buf_url_cur);

		memset(buf_url_next, 0, URL_LEN);
		ret = parse_page_by_url(&tsk, buf_url_next, URL_LEN);
		if ((-1 == ret) 
			|| (strlen(buf_url_next) == 0))
		{
			g_amazon_mobile_product_stop = 1;
			fprintf(stderr, "Amazon Mobile Data Extracting Finished!\n");
			break;
		}

        // 处理一下获取的url，后面带有其他数据,另外&amp;需要把"amp;"删除
        repalce_of(buf_url_next, "&amp;", "&");
        fix_amazon_nextpage_url(buf_url_next);

        memset(buf_url_cur, 0, URL_LEN);
        // 修正当前页url
        sprintf(buf_url_cur, "%s%s", HOME_PAGE_AMAZON_ORIG, buf_url_next);
	}

    g_amazon_mobile_product_stop = 1;
    release_page_buf(&page_buf);
	return NULL;
}

void data_update_amazon(void)
{
	int ret = 0;
	ret = pthread_create(&mobile_thread, NULL, product_mobile_data, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "创建amazon-mobile_thread失败!\n");
	}
}