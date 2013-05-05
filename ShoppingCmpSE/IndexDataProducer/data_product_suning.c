#include <stdlib.h>
#include <stdio.h>
#include "data_product_suning.h"
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

int g_suning_mobile_product_stop;

/*获取一页中的所有的手机的链接对应的页面,添加到cache中*/
static void get_all_mobile_in_a_page(char* url)
{
	page_buf_t page_buf = NULL;
	page_buf_t page_buf_mobile = NULL;
	int ret = 0;
	char* rbuf = NULL;
	int rsize = 0;
    char* find_pt = NULL;
    char* tmp_buf = NULL;

	struct pattern pt;
	int mobile_count = 0;
	char* str_get = NULL;

	// 每条手机信息都是在<li sku= </li>中,每个手机的相信信息的链接在第一个链接中
	page_buf = create_page_buf(MAX_PAGE_SIZE, PRO_NONE);
	if (NULL == page_buf)
	{
		fprintf(stderr, "粗大事啦,create_page_buf失败...\n");
		goto err_exit;
	}

	ret = get_url_page_libcurl("http://item.51buy.com/item-457773.html?YTAG=3.311212000"/*url*/, page_buf);
	if (-1 == ret)
	{
		fprintf(stderr, "获取url出错...%s\n", url);
		goto err_exit;
	}

    //tmp_buf = (char*)malloc(512*1024);
    //memset(tmp_buf, 0, 512*1024);
    //get_url_page("www.gome.com.cn/ec/homeus/jump/category/cat10000070-1-1-1-10-1-1-0-1.html", tmp_buf, 512*1024);

 	get_rbuf_of_page_buf(page_buf, &rbuf, &rsize);
    find_pt = rbuf;
	
	while (1)
	{
		page_t page;
        char* find_tmp = NULL;
        char* find_slash = NULL;
        char buf_utf8_left[URL_LEN] = {0};
        char buf_utf8_right[URL_LEN] = {0};
        char* str_price = NULL;

        char cur_url[URL_LEN] = {0};
        strcpy_s(cur_url, URL_LEN, url);

		pt.left = "<h2><a href=\"";
		pt.right = "\" target=\"_blank\">";

		init_page(&page);
		page_buf_mobile = create_page_buf(MAX_PAGE_SIZE, PRO_MOBILE_GOME);
		if (NULL == page_buf_mobile)
		{
			fprintf(stderr, "粗大事啦,create_page_buf失败...\n");
			goto err_exit;
		}

		find_pt = regex(find_pt, pt.left, pt.right, &str_get);
		if (NULL == str_get)
		{
			release_page_buf(&page_buf_mobile);
			break;
		}

        // 在这里提取价格，因为详细信息里面的价格是隐藏的 damn it!
        pt.left = "<span class=\"yuan\">";
        GBK2UTF8(pt.left, strlen(pt.left), buf_utf8_left, URL_LEN);
        pt.left = buf_utf8_left;
        pt.right = "</p>";

        find_tmp = regex(find_pt, pt.left, pt.right, &str_price);
        if (find_tmp != NULL)
        {
            find_tmp = find(str_price, "</span>");
            if (find_tmp != NULL)
            {
                find_tmp += strlen("</span>");
                page.price = atoi(find_tmp);
            }

            free(str_price);
            str_price = NULL;
        }

		mobile_count++;

        // 处理一下gome的url
        // 去掉.html后面的数据
        find_tmp = find(str_get, ".html");
        if (find_tmp != NULL)
        {
            find_tmp += strlen(".html");
            *find_tmp = 0;
        }

        // 由于手机的链接是相对链接，所以这里需要拼接
        // 下一页的链接是一个相对链接，所以需要处理一下
        find_slash = rfind(cur_url, "/");
        if (NULL == find_slash)
        {
            fprintf(stderr, "链接格式错误!\n");
            break;
        }

        find_slash += 1;

        memset(find_slash, 0, strlen(find_slash));
        strncpy(find_slash, str_get, strlen(str_get) + 1);

		ret = get_url_page_libcurl(cur_url, page_buf_mobile);
		if (-1 == ret)
		{
			fprintf(stderr, "get_url_page_libcurl失败!\n");
			release_page_buf(&page_buf_mobile);
			break;
		}

        strcpy_s(page.url, URL_LEN, cur_url);
		page.page_buf = page_buf_mobile;

		add_page(&page);

		release_page_buf(&page_buf_mobile);
		free(str_get);
		str_get = NULL;
		
		find_pt += (strlen(pt.left) + strlen(pt.right) + strlen(page.url));
	}

	release_page_buf(&page_buf);
	return;
err_exit:

	release_page_buf(&page_buf);
	return;
}

/*********************************************************
 * 函数功能：从主页中获取“手机”的url
 * 作    者：huangjia/jiangfeng
 * 时    间：2012/09/09
 *********************************************************/
static void* product_mobile_data(void* p)
{
	int ret = 0;

	char buf_url_mobile_phone[URL_LEN] = {0};
	char buf_url_step2[URL_LEN] = {0};
	char buf_url_cur[URL_LEN] = {0};
	char buf_url_next[URL_LEN] = {0};
	struct page_parse_task tsk;

    char buf_utf8_flag[URL_LEN] = {0};
    char buf_left[URL_LEN] = {0};
    char buf_right[URL_LEN] = {0};

	// step 1
    // 首先找到“手机 数码”，然后转到该页面
    init_parse_task(&tsk);
    tsk.pt_find.left = "<a href=\"";
    memset(buf_left, 0, URL_LEN);
    GBK2UTF8(tsk.pt_find.left, strlen(tsk.pt_find.left), buf_left, URL_LEN);
    tsk.pt_find.left = buf_left;

    tsk.pt_find.right = "\" name=\"dac_index_promo01\" target=\"_blank\">手机 数码";
    memset(buf_right, 0, URL_LEN);
    GBK2UTF8(tsk.pt_find.right, strlen(tsk.pt_find.right), buf_right, URL_LEN);
    tsk.pt_find.right = buf_right;
    tsk.direction = RIGHT;
    tsk.url = HOME_PAGE_SUNING;

    ret = parse_page_by_url(&tsk, buf_url_mobile_phone, URL_LEN);
    if (-1 == ret)
    {
        fprintf(stderr, "在suning主页中获取'手机 数码'的url出错...\n");
        g_suning_mobile_product_stop = 1;
        return NULL;
    }

    // step2 在'手机 数码'页面中找‘手机’的url
	init_parse_task(&tsk);
    tsk.find_flag = 1;
    tsk.flag_str = "手机通讯</a><b></b></h3>";
    memset(buf_utf8_flag, 0, URL_LEN);
    GBK2UTF8(tsk.flag_str, strlen(tsk.flag_str), buf_utf8_flag, URL_LEN);
    tsk.flag_str = buf_utf8_flag;

    tsk.pt_find.left = "href=\"";
    memset(buf_left, 0, URL_LEN);
    GBK2UTF8(tsk.pt_find.left, strlen(tsk.pt_find.left), buf_left, URL_LEN);
    tsk.pt_find.left = buf_left;

    tsk.pt_find.right = "\" target=\"_blank\" >手机</a></li>";
    memset(buf_right, 0, URL_LEN);
    GBK2UTF8(tsk.pt_find.right, strlen(tsk.pt_find.right), buf_right, URL_LEN);
    tsk.pt_find.right = buf_right;
	tsk.url = buf_url_mobile_phone;
    tsk.direction = RIGHT;

	ret = parse_page_by_url(&tsk, buf_url_step2, URL_LEN);
	if (-1 == ret)
	{
		fprintf(stderr, "在'手机 数码'中获取‘手机’的url出错...\n");
		g_suning_mobile_product_stop = 1;
		return NULL;
	}
	
	if (strlen(buf_url_step2) == 0)
	{
		fprintf(stderr, "未能找到‘手机’对应的url...\n");
		g_suning_mobile_product_stop = 1;
		return NULL;
	}

	// step 3
	// 获取所有页中所有的手机的链接（因为有很多页的数据）
	// 寻找下一页的链接
	init_parse_task(&tsk);
    tsk.direction = LEFT;
    // 之所以不弄‘下一页’是因为'下一页'之前和之后有换行
	tsk.pt_find.left = "<a class=\"pr page-next\" href=\"/";    // 很特殊，带了下一个链接的'/'
    tsk.pt_find.right = "\">";

	strcpy_s(buf_url_cur, URL_LEN, buf_url_step2);
	while (!g_suning_mobile_product_stop)
	{
		char* find_slash = NULL;
		tsk.url = buf_url_cur;

		get_all_mobile_in_a_page(buf_url_cur);

		memset(buf_url_next, 0, URL_LEN);
		ret = parse_page_by_url(&tsk, buf_url_next, URL_LEN);
		if (-1 == ret || strlen(buf_url_next) == 0)
		{
			g_suning_mobile_product_stop = 1;
			fprintf(stderr, "Mobile Data Extracting Finished!\n");
			break;
		}

		// 下一页的链接是一个相对链接，所以需要处理一下
		find_slash = rfind(buf_url_cur, "/");
		if (NULL == find_slash)
		{
			fprintf(stderr, "链接格式错误!\n");
			break;
		}

		find_slash += 1;

		memset(find_slash, 0, strlen(find_slash));
		strncpy(find_slash, buf_url_next, strlen(buf_url_next) + 1);
	}

	g_suning_mobile_product_stop = 1;
	return NULL;
}

void data_product_suning(void)
{
	int ret = 0;
	ret = pthread_create(&mobile_thread, NULL, product_mobile_data, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "创建mobile_thread失败!\n");
	}
}