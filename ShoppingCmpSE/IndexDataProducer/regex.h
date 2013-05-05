#ifndef SCSE_REGEX_H
#define SCSE_REGEX_H

/*
* 说明：正则表达式接口
* 
* 作者：huangjia/jiangfeng
* 修改：
* [2012.06.01][初稿]
* [2012.08.05][添加replace接口]
*/

#ifdef  __cplusplus
extern "C" {
#endif

/*
* 在data_buf中查找子串pattern，并返回子串pattern在data_buf中的开始位置
*/
extern char* find(char* data_buf, char* pattern);

/*
*从右往左查找
*/
extern char* rfind(char* data_buf, char* pattern);

/*
* 从字符串data_buf中获取子串start_pattern和子串stop_pattern之间的字符串str_get
* 
*/
extern char* regex(const char* data_buf, const char* start_pattern, const char* stop_pattern, char** str_get);

/*替换两个标记之间的字符*/
extern void repalce_between(char* data_buf, const char* left_flag, const char* right_flag, char c);

/* str_rpl不能比str_flag长 */
void repalce_of(char* str_src, const char* str_flag, const char* str_rpl);

#ifdef  __cplusplus
}
#endif

#endif
