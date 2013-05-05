#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include "base_func.h"

/*
* 将buf_gbk中GBK的数据转换成UTF8的存储在buf_utf8中
*/
void GBK2UTF8(const char* buf_gbk, const int buf_gbk_len, char* buf_utf8, int buf_utf8_len)
{
    wchar_t* wbuf = NULL;
    int wbuf_len = (buf_gbk_len + 1) * 2;
    if (NULL == buf_gbk
        || NULL == buf_utf8
        || 0 == buf_gbk_len
        || 0 == buf_utf8_len)
    {
        return;
    }
    
    wbuf = (wchar_t*)malloc(wbuf_len);
    if (NULL == wbuf)
    {
        return;
    }
    memset(wbuf, 0, wbuf_len);

    // GBK --> UTF16(UNICODE)
    MultiByteToWideChar(CP_ACP, 0, buf_gbk, strlen(buf_gbk), wbuf, wbuf_len/2);

    // UTF16 --> UTF8
    WideCharToMultiByte(CP_UTF8, 0, wbuf, wcslen(wbuf), buf_utf8, buf_utf8_len, NULL, NULL);

    free(wbuf);
    wbuf = NULL;
    return;
}

void UTF82GBK(const char* buf_utf8, const int buf_utf8_len, char* buf_gbk, int buf_gbk_len)
{
    wchar_t* wbuf = NULL;
    int wbuf_len = (buf_utf8_len + 1) * 2;
    if (NULL == buf_gbk
        || NULL == buf_utf8
        || 0 == buf_gbk_len
        || 0 == buf_utf8_len)
    {
        return;
    }

    wbuf = (wchar_t*)malloc(wbuf_len);
    if (NULL == wbuf)
    {
        return;
    }
    memset(wbuf, 0, wbuf_len);

    // UTF8 --> UTF16(UNICODE)
    MultiByteToWideChar(CP_UTF8, 0, buf_utf8, strlen(buf_utf8), wbuf, wbuf_len/2);

    // UTF16 --> UTF8
    WideCharToMultiByte(CP_ACP, 0, wbuf, wcslen(wbuf), buf_gbk, buf_gbk_len, NULL, NULL);

    free(wbuf);
    wbuf = NULL;
    return;
}

void fix_amazon_nextpage_url(char* url)
{
    while (*url != 0)
    {
        if (*url == '\"')
        {
            *url = 0;
            return;
        }

        url++;
    }
}