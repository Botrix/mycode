#ifndef SCSE_BASE_FUNC_H
#define SCSE_BASE_FUNC_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
* 将buf_gbk中GBK的数据转换成UTF8的存储在buf_utf8中
*/
extern void GBK2UTF8(const char* buf_gbk, const int buf_gbk_len, char* buf_utf8, int buf_utf8_len);

extern void UTF82GBK(const char* buf_utf8, const int buf_utf8_len, char* buf_gbk, int buf_gbk_len);

/* 把亚马逊相对链接中的'&amp;'处理一下，只留下'&' */
extern void fix_amazon_nextpage_url(char* url);

#ifdef __cplusplus
}
#endif

#endif