#ifndef SCSE_REGEX_H
#define SCSE_REGEX_H

/*
* ˵����������ʽ�ӿ�
* 
* ���ߣ�huangjia/jiangfeng
* �޸ģ�
* [2012.06.01][����]
* [2012.08.05][���replace�ӿ�]
*/

#ifdef  __cplusplus
extern "C" {
#endif

/*
* ��data_buf�в����Ӵ�pattern���������Ӵ�pattern��data_buf�еĿ�ʼλ��
*/
extern char* find(char* data_buf, char* pattern);

/*
*�����������
*/
extern char* rfind(char* data_buf, char* pattern);

/*
* ���ַ���data_buf�л�ȡ�Ӵ�start_pattern���Ӵ�stop_pattern֮����ַ���str_get
* 
*/
extern char* regex(const char* data_buf, const char* start_pattern, const char* stop_pattern, char** str_get);

/*�滻�������֮����ַ�*/
extern void repalce_between(char* data_buf, const char* left_flag, const char* right_flag, char c);

/* str_rpl���ܱ�str_flag�� */
void repalce_of(char* str_src, const char* str_flag, const char* str_rpl);

#ifdef  __cplusplus
}
#endif

#endif
