/*
* ˵�������ļ���������spiderģ������ṩ�Ľӿ�
* 
* ���ߣ�huangjia/jiangfeng
* �޸ģ�
* [2012.05.28][����]
* [2012.10.04][���get_url_page_libcurl]
*/
#ifndef SCSE_SPIDER_H
#define SCSE_SPIDER_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "page_buf.h"

/*
 * ͨ��url��ȡ��ҳ����
 * @url : uniform resource locator
 * @page_data : ��url�ڵ�����
 */
extern int get_url_page(const char* url, char* page_data, int page_size);

/*������ɿ�*/
extern int get_url_page_libcurl(const char* url, page_buf_t page_buf);

#ifdef  __cplusplus
}
#endif

#endif