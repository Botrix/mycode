#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "regex.h"

char* find(char* data_buf, char* pattern)
{
	char* buf_cur = data_buf;
	char* pt_cur = pattern;

	char* last_find_str = NULL;
	int last_find = 0;

	if (NULL == data_buf
		|| NULL == pattern)
	{
		return NULL;
	}

	while (*buf_cur != 0 && *pt_cur != 0)
	{
		if ((*buf_cur == *pt_cur)/* || ('*' == *pt_cur)*/)
		{
			if (0 == last_find)
			{
				last_find_str = buf_cur;
			}

			buf_cur++;
			pt_cur++;

			last_find = 1;
		}
		else
		{
			if (last_find)
			{
				buf_cur = last_find_str + 1;
				pt_cur = pattern;
			}
			else
			{
				buf_cur++;
			}

			last_find = 0;
		}
	}

	if (0 == *pt_cur)
	{
		return last_find_str;
	}

	return NULL;
}

/*
*从右往左查找
*/
char* rfind(char* data_buf, char* pattern)
{
	char* find_pt = find(data_buf, pattern);

	char* last_find = find_pt;
	while (find_pt != NULL && *find_pt != 0)
	{
		last_find = find_pt;
		find_pt = find(find_pt + 1, pattern);
	}

	return last_find;
}

/*
* data_buf : ****ABcdefGH***
* start_pattern : AB
* stop_pattern : GH
* str_get : cdef
*
*/
char* regex(const char* data_buf, 
		   const char* start_pattern, 
		   const char* stop_pattern, 
		   char** str_get)
{

	int len = 0;
	char* start = NULL;
	char* pos_begin = NULL;
	char* pos_end = NULL;

	if (NULL == data_buf
		|| NULL == start_pattern
		|| NULL == stop_pattern)
	{
		str_get = NULL;
		return NULL;
	}

    if (*str_get != NULL)
    {
        fprintf(stderr, "regex 使用方法错误, *strget != NULL\n");
        return NULL;
    }

	len = strlen(data_buf);
	start = (char*)data_buf;


	pos_begin = (char*)find(start, start_pattern);
	if (NULL == pos_begin)
	{
		return NULL;
	}

	pos_end = (char*)find(pos_begin + strlen(start_pattern), stop_pattern);
	if (pos_end != NULL)
	{
		char* start_cp = pos_begin + strlen(start_pattern);
		int len_cp = pos_end - start_cp;

		*str_get = (char*)malloc(len_cp + 1);
		if (NULL ==str_get)
		{
			return NULL;
		}
		memset(*str_get, 0, len_cp + 1);

		memcpy(*str_get, start_cp, len_cp);
	}

    return pos_begin;
}

/*替换两个标记之间的字符*/
/*left : AB
  right : CD
  c : 空格
*/
/*xxxABxxxxCDxxxxABxxxxxCD*/
/*xxx        xxxx         */
void repalce_between(char* data_buf, const char* left_flag, const char* right_flag, char c)
{
	int len = 0;
	char* start = NULL;
	char* pos_begin = NULL;
	char* pos_end = NULL;

	len = strlen(data_buf);
	start = data_buf;

	while (1)
	{
		if ((start < data_buf) || (start >= (data_buf + len)))
		{
			break;
		}

		pos_begin = (char*)find(start, left_flag);
		if (NULL == pos_begin)
		{
			break;
		}

		pos_end = (char*)find(pos_begin + strlen(left_flag), right_flag);
		if (pos_end != NULL)
		{
			pos_end = pos_end + strlen(right_flag) - 1;
			
			memset(pos_begin, c, pos_end - pos_begin + 1);

			start = pos_end + 1;
		}
		else
		{
			break;
		}
	}
}

void repalce_of(char* str_src, const char* str_flag, const char* str_rpl)
{ 
    char* pi = str_src; 
    char* po = NULL;
    char *p = NULL; 
    int nLen = 0; 
    int src_len = strlen(str_src);
    int nSrcRplLen = strlen(str_flag); 
    int nDstRplLen = strlen(str_rpl); 

    char* str_dst = (char*)malloc(src_len + 1);
    if (NULL == str_dst)
    {
        return;
    }
    memset(str_dst, 0, src_len+1);

    po = str_dst; 

    p = NULL; 
    nLen = 0; 

    do 
    {
        // 找到下一个替换点
        p = strstr(pi, str_flag); 

        if(p != NULL) 
        { 
            // 拷贝上一个替换点和下一个替换点中间的字符串
            nLen = p - pi; 
            memcpy(po, pi, nLen);

            // 拷贝需要替换的字符串
            memcpy(po + nLen, str_rpl, nDstRplLen); 
        } 
        else 
        { 
            strcpy(po, pi); 

            // 如果没有需要拷贝的字符串,说明循环应该结束
            break;
        } 

        pi = p + nSrcRplLen; 
        po = po + nLen + nDstRplLen; 

    } while (p != NULL); 

    memset(str_src, 0, src_len+1);
    memcpy(str_src, str_dst, src_len);

	free(str_dst);
	str_dst = NULL;
}

