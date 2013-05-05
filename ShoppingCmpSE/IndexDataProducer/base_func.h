#ifndef SCSE_BASE_FUNC_H
#define SCSE_BASE_FUNC_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
* ��buf_gbk��GBK������ת����UTF8�Ĵ洢��buf_utf8��
*/
extern void GBK2UTF8(const char* buf_gbk, const int buf_gbk_len, char* buf_utf8, int buf_utf8_len);

extern void UTF82GBK(const char* buf_utf8, const int buf_utf8_len, char* buf_gbk, int buf_gbk_len);

/* ������ѷ��������е�'&amp;'����һ�£�ֻ����'&' */
extern void fix_amazon_nextpage_url(char* url);

#ifdef __cplusplus
}
#endif

#endif