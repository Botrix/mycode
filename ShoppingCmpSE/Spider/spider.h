/*
* 说明：本文件中声明了spider模块对外提供的接口
* 
* 作者：huangjia/jiangfeng
* 修改：
* [2012.05.28][初稿]
* [2012.10.04][添加get_url_page_libcurl]
*/
#ifndef SCSE_SPIDER_H
#define SCSE_SPIDER_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "page_buf.h"

/*
 * 通过url获取网页数据
 * @url : uniform resource locator
 * @page_data : 该url内的数据
 */
extern int get_url_page(const char* url, char* page_data, int page_size);

/*这个更可靠*/
extern int get_url_page_libcurl(const char* url, page_buf_t page_buf);

#ifdef  __cplusplus
}
#endif

#endif