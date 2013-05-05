#include <stdlib.h>
#include <stdio.h>
#include "data_product_360buy.h"
#include "base_def.h"
#include "spider.h"
#include "regex.h"
#include "page_parse.h"
#include "page_buf.h"
#include "page_cache.h"
#include "pthread.h"

static pthread_t mobile_thread;
static pthread_t computer_thread;

int g_jd_mobile_product_stop;

static int get_jd_price(int skuid)
{
	int ret = 0;
	char* url = "p.3.cn/prices/get?skuid=J_";
	char url_append[URL_LEN] = {0};
	page_buf_t page_buf = NULL;
	char* rbuf = NULL;
	int rsize = 0;
	struct pattern pt;
	char* str_get = NULL;
	int price = 0;
	
	sprintf_s(url_append, URL_LEN, "%s%d", url, skuid);

	page_buf = create_page_buf(MAX_PAGE_SIZE, PRO_NONE);
	if (NULL == page_buf)
	{
		fprintf(stderr, "粗大事啦,create_page_buf失败...\n");
		goto err_exit;
	}

	ret = get_url_page_libcurl(url_append, page_buf);
	if (-1 == ret)
	{
		fprintf(stderr, "get_url_page_libcurl出错...%s\n", url);
		goto err_exit;
	}

	get_rbuf_of_page_buf(page_buf, &rbuf, &rsize);

	pt.left = "[{\"p\":\"";
	pt.right = ".00\"";

	regex(rbuf, pt.left, pt.right, &str_get);
	if (NULL == str_get)
	{
		fprintf(stderr, "regex失败: \npt.left = %s, \npt.right = %s\n", pt.left, pt.right);
		goto err_exit;
	}

	price = atoi(str_get);

	free(str_get);
	str_get = NULL;

	release_page_buf(&page_buf);
	return price;

err_exit:
	release_page_buf(&page_buf);
	return -1;
}

/*获取一页中的所有的手机的链接对应的页面,添加到cache中*/
static void get_all_mobile_in_a_page(char* url)
{
	page_buf_t page_buf = NULL;
	page_buf_t page_buf_mobile = NULL;
	int ret = 0;
	char* rbuf = NULL;
	int rsize = 0;
	char* find_pt = NULL;

	char* start_pt = "<li onclick";
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
		int skuid = 0;

		// 获取该手机的skuid，用于查询价格
		pt.left = "sku='";
		pt.right = "'";
		regex(find_pt, pt.left, pt.right, &str_get);
		if (NULL == str_get)
		{
			fprintf(stderr, "regex失败: \npt.left = %s, \npt.right = %s\n", pt.left, pt.right);
			break;
		}
		skuid = atoi(str_get);	
		free(str_get);
		str_get = NULL;

		// 获取该手机的链接
		pt.left = "href='";
		pt.right = "'><img";

		init_page(&page);
		page_buf_mobile = create_page_buf(MAX_PAGE_SIZE, PRO_MOBILE_360BUY);
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
			free(str_get);
			str_get = NULL;
			release_page_buf(&page_buf_mobile);
			break;
		}

		page.page_buf = page_buf_mobile;
		strcpy_s(page.url, URL_LEN, str_get);
        free(str_get);
        str_get = NULL;

        // 获取价格
		page.price = get_jd_price(skuid);
		//fprintf(stderr, "价格: %d\n", page.price);

		// 抽取好评
		pt.left = "已有";
		pt.right = "人评价</a></span>";
		regex(find_pt, pt.left, pt.right, &str_get);
		if (str_get != NULL)
		{
			page.judge_count = atoi(str_get);
			free(str_get);
			str_get = NULL;
		}

		pt.left = "<span class='reputation'>(";
		pt.right = "%好评)</span>";
		regex(find_pt, pt.left, pt.right, &str_get);
		if (str_get != NULL)
		{
			page.good_judge_rate = atoi(str_get);
			free(str_get);
			str_get = NULL;
		}
		
		add_page(&page);

		release_page_buf(&page_buf_mobile);
		free(str_get);
		str_get = NULL;
		
		find_pt = find(find_pt + strlen(start_pt), start_pt);
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
	int page_count = 0;

	char buf_url_cur[URL_LEN] = {0};
	char buf_url_next[URL_LEN] = {0};
	struct page_parse_task tsk;

	tsk.direction = RIGHT;
	tsk.pt_find.left = "<a href='";
	tsk.pt_find.right = "' class='next' >下一页<b></b>";

	strcpy_s(buf_url_cur, URL_LEN, HOME_PAGE_JD);
	while (!g_jd_mobile_product_stop)
	{
		char* find_slash = NULL;
		tsk.url = buf_url_cur;

		page_count++;

		get_all_mobile_in_a_page(buf_url_cur);

		memset(buf_url_next, 0, URL_LEN);
		ret = parse_page_by_url(&tsk, buf_url_next, URL_LEN);
		if (-1 == ret 
			|| strlen(buf_url_next) == 0)
		{
			g_jd_mobile_product_stop = 1;
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

	fprintf(stderr, "\n京东有 %d 页数据!\n", page_count);

	g_jd_mobile_product_stop = 1;
	return NULL;
}

void data_product_360buy(void)
{
	int ret = 0;
	ret = pthread_create(&mobile_thread, NULL, product_mobile_data, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "创建mobile_thread失败!\n");
	}
}