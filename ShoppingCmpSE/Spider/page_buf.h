#ifndef SCSE_PAGE_BUF_H
#define SCSE_PAGE_BUF_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
 page_buf�ĵ�0���ֽڴ洢ħ��ֵ0x7F
 page_buf�ĵ�1-4�ֽڴ洢page_class
 page_buf�ĵ�5-8�ֽڴ洢page_data���ܳ���
 page_buf�ĵ�9-12�ֽڴ洢page_data�ĵ�ǰд��λ�ã���ʼ״̬��13��
*/


#define PAGE_HEAD_LEN 13
#define page_buf_t char*

/*����page_buf����ʼ��,��������*/
extern page_buf_t create_page_buf(int size, enum product_class page_c);

/* ��ȡpage_buf��class�ֶ� */
extern enum product_class get_page_class(page_buf_t page_buf);

/* ��ȡpage_buf���ܳ��� */
extern int get_page_len(page_buf_t page_buf);

/* ��ȡpage_buf��д��λ�� */
extern int get_page_wr_pos(page_buf_t page_buf);

/* ������е����� */
extern int clear_page_buf(page_buf_t page_buf);

/*��page_buf��д�����ݣ�����ʵ��д��Ĵ�С*/
extern int write_page_buf(page_buf_t page_buf, char* buf, int size);

/*�ͷ�page_buf�������bufһ����ָ��0xFD��*/
extern int release_page_buf(page_buf_t* buf);

/*��ȡpage_buf��ʵ�ʴ洢���ݵ�buf(��ʵpage_buf+9),sizeΪʵ������buf�Ĵ�С(page_buf_size-9)*/
extern int get_rbuf_of_page_buf(page_buf_t page_buf, char**buf, int* size);

/*��page_buf�������Զ�������ʽdump��C��*/
extern void dump_page_buf(page_buf_t page_buf);
extern void dump_page_buf_s(char* buf, int size);

#ifdef __cplusplus
}
#endif

#endif