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


/*��ȡһҳ�е����е��ֻ������Ӷ�Ӧ��ҳ��,��ӵ�cache��*/
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

	// ÿ���ֻ���Ϣ������<br clear="all">����,ÿ���ֻ�����Ӧ��Ϣ�������ڵ�һ��������
	page_buf = create_page_buf(MAX_PAGE_SIZE, PRO_NONE);
	if (NULL == page_buf)
	{
		fprintf(stderr, "�ִ�����,create_page_bufʧ��...\n");
		goto err_exit;
	}

	ret = get_url_page_libcurl(url, page_buf);
	if (-1 == ret)
	{
		fprintf(stderr, "��ȡurl����...%s\n", url);
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
			fprintf(stderr, "�ִ�����,create_page_bufʧ��...\n");
			goto err_exit;
		}

		regex(find_pt, pt.left, pt.right, &str_get);
		if (NULL == str_get)
		{
			fprintf(stderr, "regexʧ��: \npt.left = %s, \npt.right = %s\n", pt.left, pt.right);
			release_page_buf(&page_buf_mobile);
			break;
		}
        
		mobile_count++;

		ret = get_url_page_libcurl(str_get, page_buf_mobile);
		if (-1 == ret)
		{
			fprintf(stderr, "get_url_page_libcurlʧ��!\n");
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
            fprintf(stderr, "amazon ��ȡimg urlʧ��!\n");
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
 * �������ܣ������ֻ�����
 * ��    �ߣ�huangjia/jiangfeng
 * ʱ    �䣺2012/10/14
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

    // step 1 ��ȡ����ѷ�ġ����з��ࡱҳ��

	// step 1
	/* ����ҳ�л�ȡ���ֻ������롱�еġ��ֻ�����url*/    
// 
// 	// step 2
// 	// ��"�����ֻ�"ҳ���л�ȡ������Ʒ�ơ���url
// 
// 	init_parse_task(&tsk);
// 	tsk.direction = RIGHT;
// 	tsk.pt_find.left = "href=\"";
// 	tsk.pt_find.right = "\">����Ʒ��</a>";
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
// 		fprintf(stderr, "���ֻ���ͨѶҳ���л�ȡ����Ʒ��rl����...\n");
// 		g_amazon_mobile_product_stop = 1;
// 		goto err_exit;
// 	}
// 
//     // ��ȡ����url����Եģ������Ҫ����
//     memset(buf_url_cur, 0, URL_LEN);
//     sprintf(buf_url_cur, "%s%s", HOME_PAGE_AMAZON, buf_url_step2);

	// step 3
	// ��ȡ����ҳ�����е��ֻ������ӣ���Ϊ�кܶ�ҳ�����ݣ�
	// Ѱ����һҳ������
	
	tsk.direction = RIGHT;
	tsk.pt_find.left = "href=\"";
	tsk.pt_find.right = "<span id=\"pagnNextString\">��һҳ</span>";
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

        // ����һ�»�ȡ��url�����������������,����&amp;��Ҫ��"amp;"ɾ��
        repalce_of(buf_url_next, "&amp;", "&");
        fix_amazon_nextpage_url(buf_url_next);

        memset(buf_url_cur, 0, URL_LEN);
        // ������ǰҳurl
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
		fprintf(stderr, "����amazon-mobile_threadʧ��!\n");
	}
}