#include "page_buf.h"
#include "base_def.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*创建page_buf并初始化,包括清零*/
page_buf_t create_page_buf(int size, enum product_class page_c)
{
	char* page_buf = NULL;

	if (size <= 0)
	{
		return NULL;
	}

	page_buf = (char*)malloc(size + PAGE_HEAD_LEN);
	if (NULL == page_buf)
	{
		return NULL;
	}

	memset(page_buf, 0, size + PAGE_HEAD_LEN);

	*page_buf = 0x7F;
	*(int*)(page_buf+1) = page_c;
	*(int*)(page_buf+5) = size;
	*(int*)(page_buf+9) = PAGE_HEAD_LEN;

	return page_buf;
}

/*释放page_buf，传入的buf一定是指向0x7F的*/
int release_page_buf(page_buf_t* buf)
{
	if (NULL == *buf
		|| **buf != (char)0x7F)
	{
		fprintf(stderr, "传入page_buf错误!\n");
		return -1;
	}

	free(*buf);
	*buf = NULL;

	return 0;
}

/* 获取page_buf的class字段 */
enum product_class get_page_class(page_buf_t page_buf)
{
	if (NULL == page_buf)
	{
		return PRO_NONE;
	}

	return (enum product_class)*(int*)(page_buf+1);
}

/* 获取page_buf的总长度 */
int get_page_len(page_buf_t page_buf)
{
	int page_buf_size = 0;
	if (NULL == page_buf)
	{
		return -1;
	}

	page_buf_size = *(int*)(page_buf+5);
	return page_buf_size;
}

/* 获取page_buf的写入位置 */
int get_page_wr_pos(page_buf_t page_buf)
{
	int wr_pos = 0;
	if (NULL == page_buf)
	{
		return -1;
	}
	
	wr_pos = *(int*)(page_buf+9);
	return wr_pos;
}

/* 清空其中的数据 */
int clear_page_buf(page_buf_t page_buf)
{
    *(int*)(page_buf+9) = PAGE_HEAD_LEN;

    return 0;
}

/*往page_buf内写入数据，返回实际写入的大小*/
int write_page_buf(page_buf_t page_buf, char* buf, int size)
{
	int page_buf_size = 0;
	int wr_pos = 0;
	int left_size = 0;
	int wr_size = 0;

	if (NULL == page_buf
		|| *page_buf != 0x7F
		|| NULL == buf
		|| size <= 0)
	{
		return 0;
	}

	// 取出page_buf的大小
	page_buf_size = get_page_len(page_buf);
	// 取出写入位置
	wr_pos = get_page_wr_pos(page_buf);

	left_size = page_buf_size - (wr_pos - PAGE_HEAD_LEN);
	wr_size = min(size, left_size);

	memcpy(page_buf + wr_pos, buf, wr_size);
	*(int*)(page_buf+9) += wr_size;

	return wr_size;
}

/*获取page_buf的实际存储数据的buf(其实page_buf+9),size为实际数据buf的大小(page_buf_size-9)*/
int get_rbuf_of_page_buf(page_buf_t page_buf, char**buf, int* size)
{
	int page_buf_size = 0;
	int wr_pos = 0;

	if (*page_buf != 0x7F
		|| NULL == page_buf)
	{
		*buf = NULL;
		*size = 0;
		return -1;
	}

	// 取出page_buf的大小
	page_buf_size = get_page_len(page_buf);
	// 取出写入位置
	wr_pos = get_page_wr_pos(page_buf);

	// 设置返回值
	*buf = page_buf + PAGE_HEAD_LEN;
	*size = page_buf_size;

	return 0;
}

/*将page_buf的数据以二进制形式dump到C盘*/
void dump_page_buf(page_buf_t page_buf)
{
    FILE* file = NULL;
    char* rbuf = NULL;
    int rsize = 0;
    file = fopen("C:\\PAGE_BUF_DUMP.dat", "wb");
    if (NULL == file)
    {
        fprintf(stderr, "创建文件失败:C:\PAGE_BUF_DUMP.dat\n");
        return;
    }

    get_rbuf_of_page_buf(page_buf, &rbuf, &rsize);

    fwrite(rbuf, 1, rsize, file);

    fprintf(stderr, "dump completed C:\\PAGE_BUF_DUMP.dat\n");

    fclose(file);
}

void dump_page_buf_s(char* buf, int size)
{
    FILE* file = NULL;
    char* rbuf = NULL;
    int rsize = 0;
    file = fopen("C:\\PAGE_BUF_DUMP.dat", "wb");
    if (NULL == file)
    {
        fprintf(stderr, "创建文件失败:C:\PAGE_BUF_DUMP.dat\n");
        return;
    }

    fwrite(buf, 1, size, file);

    fprintf(stderr, "dump completed C:\\PAGE_BUF_DUMP.dat\n");

    fclose(file);
}