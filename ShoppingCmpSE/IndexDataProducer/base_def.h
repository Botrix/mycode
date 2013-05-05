#ifndef SCSE_BASE_DEF_H
#define SCSE_BASE_DEF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <string.h>

// ��������ֹͣ���
extern int g_jd_mobile_product_stop;
extern int g_amazon_mobile_product_stop;

#define MAX_PAGE_SIZE 524288
#define DEFAULT_BUF_LEN 1024
#define URL_LEN 512
#define MAX_SQL_LEN 2048

//////////////////�����̳�////////////////////
#define HOME_PAGE_JD_ORGI "http://www.jd.com/"
#define HOME_PAGE_JD "http://list.jd.com/652-653-655.html"


///////////////////����ѷ/////////////////////
//#define HOME_PAGE_AMAZON "http://www.amazon.cn"
#define HOME_PAGE_AMAZON_ORIG "http://www.amazon.cn"
#define HOME_PAGE_AMAZON  "http://www.amazon.cn/s/ref=sr_abn_pp?ie=UTF8&bbn=665002051&rh=n%3A665002051"

///////////////////�����׹�/////////////////////
#define HOME_PAGE_SUNING "http://www.suning.com/"

///////////////////��������/////////////////////
#define HOME_PAGE_GOME "http://www.gome.com.cn/"

#define SELLER_JD "�����̳�"
#define SELLER_AMAZON "����ѷ"

enum product_class
{
	PRO_NONE = 0,

	PRO_360BUY_START,
	PRO_MOBILE_360BUY,
	PRO_COMPUTER_360BUY,
	PRO_360BUY_END,

	PRO_AMAZON_START,
	PRO_MOBILE_AMAZON,
	PRO_COMPUTER_AMAZON,
	PRO_AMAZON_END,

    PRO_GOME_START,
    PRO_MOBILE_GOME,
    PRO_COMPUTER_GOME,
    PRO_GOME_END,
};

typedef struct mobile_info
{
	enum product_class store_c;
    char details[1024]; // for amazon
	char brand[64];	// apple
	char model[64];	// iphone 4s
	char color[64];	// ��ɫ
	char os[64];	// ios5
	char mem[64];	// 16G
	char net_type[64];	// WCDMA/GSM

	char url[URL_LEN];	// ���ֻ���Ϣ��url
	char img_url[URL_LEN];	// ���ֻ���ͼƬ����
	int good_judge_rate;
	int judge_count;
	char str_price[URL_LEN];	// 360buy�ļ۸���url���ӣ�����ѷ�����ַ���
} mobile_info;

// typedef struct mobile_info
// {
//     enum product_class store_c;
//     char brand_model[64];	// apple iphone4
//     char details[1024];	// 
//     int good_judge_rate;
//     int judge_count;
//     char url[URL_LEN];	// ���ֻ���Ϣ��url
//     char img_url[URL_LEN];	// ���ֻ���ͼƬ����
// } mobile_info;

static void init_mobile_info(struct mobile_info* info)
{
	memset(info, 0, sizeof(mobile_info));
}

#ifdef __cplusplus
}
#endif

#endif