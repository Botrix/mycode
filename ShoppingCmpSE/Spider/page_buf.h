#ifndef SCSE_PAGE_BUF_H
#define SCSE_PAGE_BUF_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
 page_buf的第0个字节存储魔术值0x7F
 page_buf的第1-4字节存储page_class
 page_buf的第5-8字节存储page_data的总长度
 page_buf的第9-12字节存储page_data的当前写入位置（初始状态是13）
*/


#define PAGE_HEAD_LEN 13
#define page_buf_t char*

/*创建page_buf并初始化,包括清零*/
extern page_buf_t create_page_buf(int size, enum product_class page_c);

/* 获取page_buf的class字段 */
extern enum product_class get_page_class(page_buf_t page_buf);

/* 获取page_buf的总长度 */
extern int get_page_len(page_buf_t page_buf);

/* 获取page_buf的写入位置 */
extern int get_page_wr_pos(page_buf_t page_buf);

/* 清空其中的数据 */
extern int clear_page_buf(page_buf_t page_buf);

/*往page_buf内写入数据，返回实际写入的大小*/
extern int write_page_buf(page_buf_t page_buf, char* buf, int size);

/*释放page_buf，传入的buf一定是指向0xFD的*/
extern int release_page_buf(page_buf_t* buf);

/*获取page_buf的实际存储数据的buf(其实page_buf+9),size为实际数据buf的大小(page_buf_size-9)*/
extern int get_rbuf_of_page_buf(page_buf_t page_buf, char**buf, int* size);

/*将page_buf的数据以二进制形式dump到C盘*/
extern void dump_page_buf(page_buf_t page_buf);
extern void dump_page_buf_s(char* buf, int size);

#ifdef __cplusplus
}
#endif

#endif