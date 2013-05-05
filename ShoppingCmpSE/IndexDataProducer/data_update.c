#include "data_update.h"
#include "pthread.h"
#include "data_product_360buy.h"
#include "data_product_amazon.h"
#include "data_product_gome.h"
#include "data_product_suning.h"
#include "page_cache.h"
#include <stdlib.h>
#include <stdio.h>
#include "page_parse.h"
#include "db_sqlsvr.h"
#include "spider.h"

pthread_t thread_producer;

pthread_t thread_360buy;
pthread_t thread_amazon;
pthread_t thread_gome;
pthread_t thread_suning;

extern int g_jd_mobile_product_stop;
extern int g_suning_mobile_product_stop;

static void* producer_360buy(void* p)
{
	data_product_360buy();

	return NULL;
}

static void* producer_amazon(void* p)
{
	data_update_amazon();

	return NULL;
}

static void* producer_gome(void* p)
{
    data_product_gome();

    return NULL;
}

static void* producer_suning(void* p)
{
    data_product_suning();

    return NULL;
}


static void* producer(void* p)
{
	int ret = 0;
    ret = pthread_create(&thread_360buy, NULL, producer_360buy, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "创建线程失败...producer_360buy\n");
		return NULL;
	}

	ret = pthread_create(&thread_amazon, NULL, producer_amazon, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "创建线程失败...producer_amazon\n");
		return NULL;
	}

//     ret = pthread_create(&thread_gome, NULL, producer_gome, NULL);
//     if (ret != 0)
//     {
//         fprintf(stderr, "创建线程失败...producer_gome\n");
//         return NULL;
//     }
// 
//     ret = pthread_create(&thread_suning, NULL, producer_suning, NULL);
//     if (ret != 0)
//     {
//         fprintf(stderr, "创建线程失败...producer_suning\n");
//         return NULL;
//     }

	return NULL;
}

static int add_to_db(struct mobile_info* info)
{
	char sql[MAX_SQL_LEN] = {0};
	int ret = 0;
	if (PRO_MOBILE_360BUY == info->store_c)
	{
        char buf_detailes[1024] = {0};
        char buf_brand_model[64] = {0};
        sprintf_s(buf_detailes, 1024, "%s %s %s %s %s %s",
                                    info->brand,
                                    info->model,
                                    info->color,
                                    info->os,
                                    info->mem,
                                    info->net_type);
        sprintf_s(buf_brand_model, 64, "%s %s", info->brand, info->model);

        sprintf_s(sql, MAX_SQL_LEN, "insert into mobile_products values \
                                    ('%s', '%s', '%d', '%d',\
                                    '%s', '%s', '%s', '%s')",
                                    buf_brand_model, buf_detailes,
                                    info->good_judge_rate, info->judge_count,
                                    info->url, info->img_url, info->str_price,
                                    SELLER_JD);
	}
    else if (PRO_MOBILE_AMAZON == info->store_c)
    {
        char buf_brand_model[64] = {0};
        sprintf_s(buf_brand_model, 64, "%s %s", info->brand, info->model);

        sprintf_s(sql, MAX_SQL_LEN, "insert into mobile_products values \
                                    ('%s', '%s', '%d', '%d',\
                                    '%s', '%s', '%s', '%s')",
                                    buf_brand_model, info->details,
                                    info->good_judge_rate, info->judge_count,
                                    info->url, info->img_url, info->str_price,
                                    SELLER_AMAZON);
    }

	ret = exe_sql_without_res(sql);

	return ret;
}

static void* consumer(void* p)
{
	struct page_t page;
	struct mobile_info info;
	int ret = 0;

	while (!(g_jd_mobile_product_stop 
        && g_amazon_mobile_product_stop))
	{
		init_page(&page);
		init_mobile_info(&info);
		page.page_buf = create_page_buf(MAX_PAGE_SIZE, PRO_NONE);
		if (NULL == page.page_buf)
		{
			fprintf(stderr, "粗大事啦,create_page_buf失败!\n");
			continue;
		}

		get_page(&page);

		ret = parse_page_info(&page, &info);
		if (-1 == ret)
		{
			/* 处理数据失败,写日志，并重新获取 */
// 			if (strlen(page.url) != 0)
// 			{
// 				fprintf(stderr, "ERROR : URL : %s\n", page.url);
// 			}
			release_page_buf(&page.page_buf);
			continue;
		}

		add_to_db(&info);
		release_page_buf(&page.page_buf);

		fprintf(stderr, "%d\t ++++++++++++%s\t%s\t 好评率:%d%%+++++++++++++++++++++\n", info.store_c, info.brand, info.model, info.good_judge_rate);
	}

	fprintf(stderr, "Consumer finished!\n");
	return NULL;
}


void update_data(void)
{
	int ret = 0;
	g_jd_mobile_product_stop = 0;
    g_suning_mobile_product_stop = 0;

	ret = pthread_create(&thread_producer, NULL, producer, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "创建线程失败...thread_producer\n");
		return;
	}
	consumer(NULL);

// 	pthread_join(thread_360buy, NULL);
// 	pthread_join(thread_amazon, NULL);
// 	pthread_join(thread_producer, NULL);
}