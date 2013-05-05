#include "page_parse.h"
#include "regex.h"
#include "spider.h"
#include "page_buf.h"
#include "base_func.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int parse_page_by_url(page_parse_task* tsk, char* buf, int buf_len)
{
	page_buf_t page_buf;
	int ret = 0;
	char* rbuf = NULL;
	int rsize = 0;

	if (NULL == tsk
		|| NULL == buf
		|| 0 == buf_len)
	{
		return -1;
	}

	page_buf = create_page_buf(MAX_PAGE_SIZE, PRO_NONE);
	if (NULL == page_buf)
	{
		fprintf(stderr, "�ִ�����,create_page_bufʧ��...\n");
		return -1;
	}

	ret = get_url_page_libcurl(tsk->url, page_buf);
    if (-1 == ret)
    {
        fprintf(stderr, "\nget_url_page_libcurlʧ��:url : %s\n", tsk->url);
        goto err_exit;
    }

	ret = get_rbuf_of_page_buf(page_buf, &rbuf, &rsize);
	if (-1 == ret)
	{
		fprintf(stderr, "get_rbuf_of_page_bufʧ��\n");
		goto err_exit;
	}

    // �Ƿ�Ҫ���ҵ�һ�����
    if (1 == tsk->find_flag)
    {
        char* find_flag_pos = NULL;

        if (NULL == tsk->flag_str)
        {
            fprintf(stderr, "WARNING:find_flag is set,but flag_str is NULL!\n");
            goto err_exit;
        }

        find_flag_pos = find(rbuf, tsk->flag_str);
        if (find_flag_pos != NULL)
        {
            // ������λ���Ƶ�flag����
            rbuf = find_flag_pos;
            rbuf += strlen(tsk->flag_str);
        }
    }

	if (LEFT == tsk->direction)
	{
		char* str_get = NULL;
		regex(rbuf, tsk->pt_find.left, tsk->pt_find.right, &str_get);
		if (str_get != NULL)
		{
			strcpy_s(buf, buf_len, str_get);
			free(str_get);
			str_get = NULL;
		}
	}
	else if (RIGHT == tsk->direction)
	{
		// ���ҵ�right
		char* fd_right = NULL;
		char* fd_left = NULL;
		char* last_left = NULL;

		fd_right = find(rbuf, tsk->pt_find.right);
		if (NULL == fd_right)
		{
			fprintf(stderr, "Right is not found, we quit! tsk->pt_find.right = %s\n"
				, tsk->pt_find.right);
			goto err_exit;
		}

		fd_left = find(rbuf, tsk->pt_find.left);
		if (NULL == fd_left)
		{
			fprintf(stderr, "Left is not found, we quit! tsk->pt_find.left = %s\n"
				, tsk->pt_find.left);
			goto err_exit;
		}

		last_left = fd_left;
		while (fd_left < fd_right)
		{
			last_left = fd_left;
			fd_left = find(fd_left + strlen(tsk->pt_find.left), tsk->pt_find.left);
			if (NULL == fd_left)
			{
				break;
			}
		}

		memset(buf, 0, buf_len);
		memcpy_s(buf, buf_len, last_left + strlen(tsk->pt_find.left), 
			fd_right - (last_left + strlen(tsk->pt_find.left)));
	}
	else
	{
		fprintf(stderr, "Task is wrong. tsk->direction = %d\n", tsk->direction);
		goto err_exit;
	}

	release_page_buf(&page_buf);
	return 0;

err_exit:
	release_page_buf(&page_buf);
	return -1;
}


int parse_page_by_buf(struct page_parse_task* tsk, page_buf_t page_buf, char* buf, int buf_len)
{
	int ret = 0;
	char* rbuf = NULL;
	int rsize = 0;

	if (NULL == tsk
		|| NULL == page_buf
		|| NULL == buf
		|| 0 == buf_len)
	{
		return -1;
	}
	
	ret = get_rbuf_of_page_buf(page_buf, &rbuf, &rsize);
	if (-1 == ret)
	{
		fprintf(stderr, "Failed to get url %s\n", tsk->url);
		goto err_exit;
	}

    // �Ƿ�Ҫ���ҵ�һ�����
    if (1 == tsk->find_flag)
    {
        char* find_flag_pos = NULL;

        if (NULL == tsk->flag_str)
        {
            fprintf(stderr, "WARNING:find_flag is set,but flag_str is NULL!\n");
            goto err_exit;
        }

        find_flag_pos = find(rbuf, tsk->flag_str);
        if (find_flag_pos != NULL)
        {
            // ������λ���Ƶ�flag����
            rbuf = find_flag_pos;
            rbuf += strlen(tsk->flag_str);
        }
    }

	if (LEFT == tsk->direction)
	{
		char* str_get = NULL;
		regex(rbuf, tsk->pt_find.left, tsk->pt_find.right, &str_get);
		if (str_get != NULL)
		{
			strcpy_s(buf, buf_len, str_get);

			free(str_get);
			str_get = NULL;
		}
	}
	else if (RIGHT == tsk->direction)
	{
		// ���ҵ�right
		char* fd_right = NULL;
		char* fd_left = NULL;
		char* last_left = NULL;

		fd_right = find(rbuf, tsk->pt_find.right);
		if (NULL == fd_right)
		{
			fprintf(stderr, "Right is not found, we quit! tsk->pt_find.right = %s\n"
				, tsk->pt_find.right);
			goto err_exit;
		}

		fd_left = find(rbuf, tsk->pt_find.left);
		if (NULL == fd_left)
		{
			fprintf(stderr, "Left is not found, we quit! tsk->pt_find.left = %s\n"
				, tsk->pt_find.left);
			goto err_exit;
		}

		last_left = fd_left;
		while (fd_left < fd_right)
		{
			last_left = fd_left;
			fd_left = find(fd_left + strlen(tsk->pt_find.left), tsk->pt_find.left);
			if (NULL == fd_left)
			{
				break;
			}
		}

		memset(buf, 0, buf_len);
		memcpy_s(buf, buf_len, last_left + strlen(tsk->pt_find.left), 
			fd_right - (last_left + strlen(tsk->pt_find.left)));
	}
	else
	{
		fprintf(stderr, "Task is wrong. tsk->direction = %d\n", tsk->direction);
		goto err_exit;
	}

	return 0;

err_exit:
	return -1;
}

static int parse_mobile_info_360buy(struct page_t* page, struct mobile_info* info)
{
	char* rbuf;
	int rsize;
	char* left = NULL;
	char* right = "</td></tr>";
	char* str_get = NULL;
	int ret = 0;

	if (NULL == page
		|| NULL == page->page_buf)
	{
		return -1;
	}

	ret = get_rbuf_of_page_buf(page->page_buf, &rbuf, &rsize);
	if (-1 == ret)
	{
		return -1;
	}

	info->store_c = PRO_MOBILE_360BUY;

    // ���ü۸񡢺��������ϲ��ȡ�ģ�
    //strcpy_s(info->str_price, URL_LEN, page->price_url);
	sprintf_s(info->str_price, URL_LEN, "%d.00", page->price);
    info->good_judge_rate = page->good_judge_rate;
    info->judge_count = page->judge_count;

	// Ʒ��
	left = "<tr><td class=\"tdTitle\">Ʒ��</td><td>";
	regex(rbuf, left, right, &str_get);

	if (str_get != NULL)
	{
		strcpy_s(info->brand, sizeof(info->brand), str_get);
		free(str_get);
		str_get = NULL;
	}
	else
	{
		fprintf(stderr, "Ʒ���ֶ�Ϊ�գ�����!\n");
		return -1;
	}

	// �ͺ�
	left = "<tr><td class=\"tdTitle\">�ͺ�</td><td>";
	regex(rbuf, left, right, &str_get);

	if (str_get != NULL)
	{
		strcpy_s(info->model, sizeof(info->model), str_get);
		free(str_get);
		str_get = NULL;
	}

	// ��ɫ
	left = "<tr><td class=\"tdTitle\">��ɫ</td><td>";
	regex(rbuf, left, right, &str_get);
	if (str_get != NULL)
	{
		strcpy_s(info->color, sizeof(info->color), str_get);
		free(str_get);
		str_get = NULL;
	}

	// ����ϵͳ
	left = "<tr><td class=\"tdTitle\">����ϵͳ</td><td>";
	regex(rbuf, left, right, &str_get);
	if (str_get != NULL)
	{
		strcpy_s(info->os, sizeof(info->os), str_get);
		free(str_get);
		str_get = NULL;
	}

	// �����ڴ�
	left = "<tr><td class=\"tdTitle\">�����ڴ�</td><td>";
	regex(rbuf, left, right, &str_get);
	if (str_get != NULL)
	{
		strcpy_s(info->mem, sizeof(info->mem), str_get);
		free(str_get);
		str_get = NULL;
	}

	// ������ʽ
	left = "<tr><td class=\"tdTitle\">������ʽ</td><td>";
	regex(rbuf, left, right, &str_get);
	if (str_get != NULL)
	{
		strcpy_s(info->net_type, sizeof(info->net_type), str_get);
		free(str_get);
		str_get = NULL;
	}

	// �۸�
	// "<strong class=\"p-price\"><img data-img=\"3\" src =\""
	// "<strong class=\"p-price\"><img data-img=\"3\" src=\""
// 	left = "<strong class=\"p-price\"><img data-img=\"3\" src =\"";
// 	right = "\"/></strong>";
// 	regex(rbuf, left, right, &str_get);
// 	if (NULL == str_get)
// 	{
// 		left = "<strong class=\"p-price\"><img data-img=\"3\" src=\"";
// 		regex(rbuf, left, right, &str_get);
// 		if (NULL == str_get)
// 		{
// 			fprintf(stderr, "��ȡ�۸�ʧ��...\n");
// 			return -1;
// 		}
// 	}
// 
// 	if (str_get != NULL)
// 	{
// 		strcpy_s(info->url_price, URL_LEN, str_get);
// 		free(str_get);
// 		str_get = NULL;
// 	}

	// url
	strcpy_s(info->url, URL_LEN, page->url);

	// img url
	left = "jqimg=\"";
	right = "\"/>";
	regex(rbuf, left, right, &str_get);
	if (str_get != NULL)
	{
		strcpy_s(info->img_url, URL_LEN, str_get);
		free(str_get);
		str_get = NULL;
	}

	return 0;
}

static int parse_mobile_info_amazon(struct page_t* page, struct mobile_info* info)
{
    char* rbuf;
    int rsize;
    char* left = NULL;
    char* right = "</td></tr>";
    char* str_get = NULL;
    char* find_pos = NULL;
    char* find_str = NULL;
    int ret = 0;
    char buf_left[URL_LEN] = {0};
    char buf_right[URL_LEN] = {0};

    if (NULL == page
        || NULL == page->page_buf)
    {
        return -1;
    }

    ret = get_rbuf_of_page_buf(page->page_buf, &rbuf, &rsize);
    if (-1 == ret)
    {
        return -1;
    }

    info->store_c = PRO_MOBILE_AMAZON;

    strcpy_s(info->url, URL_LEN, page->url);

    // ��ȡtitle
    left = "<span id=\"btAsinTitle\">";
    right = "</span>";
    regex(rbuf, left, right, &str_get);
    if (str_get != NULL)
    {
        // �洢����
        char buf_tmp[1024] = {0};
        UTF82GBK(str_get, strlen(str_get), buf_tmp, 1024);

        strcpy_s(info->details, sizeof(info->details), buf_tmp);
        free(str_get);
        str_get = NULL;
    }

    // Ʒ��
    // ���Ƶ�title��
    find_str = "Ʒ��: <a href=\"";
    memset(buf_left, 0, URL_LEN);
    GBK2UTF8(find_str, strlen(find_str), buf_left, URL_LEN);
    find_str = buf_left;
    find_pos = find(rbuf, find_str);
    if (NULL == find_pos)
	{
		fprintf(stderr, "Ʒ���ֶ�Ϊ�գ�����!\n");
        return -1;
    }

    left = "\">";
    right = "</a>";
    regex(find_pos, left, right, &str_get);
    if (str_get != NULL)
    {
        char buf_tmp[64] = {0};
        UTF82GBK(str_get, strlen(str_get), buf_tmp, 64);

        strcpy_s(info->brand, sizeof(info->brand), buf_tmp);
        free(str_get);
        str_get = NULL;
    }
	else
	{
		fprintf(stderr, "Ʒ���ֶ�Ϊ�գ�����!\n");
		return -1;
	}

    // �ͺ�
    left = "<li><b>�ͺ�:</b> ";
    memset(buf_left, 0, URL_LEN);
    GBK2UTF8(left, strlen(left), buf_left, URL_LEN);
    left = buf_left;
    right = "</li>";
    regex(rbuf, left, right, &str_get);
    if (str_get != NULL)
    {
        char buf_tmp[64] = {0};
        UTF82GBK(str_get, strlen(str_get), buf_tmp, 64);

        // ���ͺ�ת���ɴ�д
        // strupr(buf_tmp); // ���Ͻ�������iphone
        strcpy_s(info->model, sizeof(info->model), buf_tmp);
        free(str_get);
        str_get = NULL;
    }

    // �۸�
    left = "<span id=\"actualPriceValue\"><b class=\"priceLarge\">";
    right = "</b></span>";
    regex(rbuf, left, right, &str_get);
    if (str_get != NULL)
    {
        char buf_tmp[512] = {0};
        UTF82GBK(str_get, strlen(str_get), buf_tmp, 512);

		repalce_of(buf_tmp, ",", "");
		repalce_of(buf_tmp, " ", "");
		repalce_of(buf_tmp, "��", "");

        strcpy_s(info->str_price, sizeof(info->str_price), buf_tmp);
        free(str_get);
        str_get = NULL;
    }

    // img url
    // img ���ϲ��ȡ
    strcpy_s(info->img_url, URL_LEN, page->img_url);
//     left = "function(){var i=new Image;i.src = \"";
//     right = "\";}";
//     regex(rbuf, left, right, &str_get);
//     if (NULL == str_get)
//     {
//         fprintf(stderr, "��ȡimg urlʧ��...\n");
//     }
// 
//     if (str_get != NULL)
//     {
//         strcpy_s(info->img_url, URL_LEN, str_get);
//         free(str_get);
//         str_get = NULL;
//     }

    // �������� 
    find_str = "<span class='sl-title-tab-title'>���岻��</span>";
    memset(buf_left, 0, URL_LEN);
    GBK2UTF8(find_str, strlen(find_str), buf_left, URL_LEN);
    find_str = buf_left;
    find_pos = find(rbuf, find_str);
    if (NULL == find_pos)
    {
        //fprintf(stderr, "��ȡ����������ʧ��...\n");
        // ��ȡʧ���п����ǻ�û�п�����
        //return -1;
    }

    left = "<span class='sl-title-tab-count'>(";
    right = ")</span>";
    regex(find_pos, left, right, &str_get);
    if (str_get != NULL)
    {
        info->judge_count = atoi(str_get);
        free(str_get);
        str_get = NULL;
    }

    // ������
    left = "<span style='float:right'>";
    right = "%</span>";
    regex(find_pos, left, right, &str_get);
    if (str_get != NULL)
    {
        info->good_judge_rate = atoi(str_get);
        free(str_get);
        str_get = NULL;
    }

    // ���������û�У���ͨ��4�ǡ�5����ת��
    if (0 == info->judge_count)
    {
        int good_judge_count = 0;

        find_str = "������</b>";
        memset(buf_left, 0, URL_LEN);
        GBK2UTF8(find_str, strlen(find_str), buf_left, URL_LEN);
        find_str = buf_left;
        find_pos = find(rbuf, find_str);

        if (NULL == find_pos)
        {
            return 0;
        }

        // ��find_pos��ǰ�ƶ����Ƶ�����ǰ��ȥ
        find_pos -= 16;

        left = "<b>";
        right = "������</b>";
        memset(buf_left, 0, URL_LEN);
        GBK2UTF8(right, strlen(right), buf_left, URL_LEN);
        right = buf_left;

        regex(find_pos, left, right, &str_get);

        // ȥ�����������е�','
        repalce_of(str_get, ",", "");

        info->judge_count = atoi(str_get);
        free(str_get);
        str_get = NULL;

        if (info->judge_count <= 0)
        {
            return 0;
        }

        // ��ȡ5�Ǹ���
        find_str = "5 ��</a>";
        memset(buf_left, 0, URL_LEN);
        GBK2UTF8(find_str, strlen(find_str), buf_left, URL_LEN);
        find_str = buf_left;
        find_pos = find(rbuf, find_str);

        if (NULL == find_pos)
        {
            return 0;
        }

        left = "&nbsp;(";
        right = ")</td>";

        regex(find_pos, left, right, &str_get);

        good_judge_count += atoi(str_get);
        free(str_get);
        str_get = NULL;

        // ��ȡ4�Ǹ���
        find_str = "4 ��</a>";
        memset(buf_left, 0, URL_LEN);
        GBK2UTF8(find_str, strlen(find_str), buf_left, URL_LEN);
        find_str = buf_left;
        find_pos = find(rbuf, find_str);

        if (NULL == find_pos)
        {
            goto calc_rate;
        }

        left = "&nbsp;(";
        right = ")</td>";

        regex(find_pos, left, right, &str_get);

        good_judge_count += atoi(str_get);
        free(str_get);
        str_get = NULL;

calc_rate:
        // ���������
        info->good_judge_rate = (int)((float)(good_judge_count) / (float)(info->judge_count) * 100);
    }

	return 0;
}

static int parse_mobile_info_gome(struct page_t* page, struct mobile_info* info)
{
    char* rbuf;
    int rsize;
    char* left = NULL;
    char* right = "</td></tr>";
    char* str_get = NULL;
    int ret = 0;

    if (NULL == page
        || NULL == page->page_buf)
    {
        return -1;
    }

    ret = get_rbuf_of_page_buf(page->page_buf, &rbuf, &rsize);
    if (-1 == ret)
    {
        return -1;
    }

    info->store_c = PRO_MOBILE_360BUY;

    // Ʒ��
    left = "<tr><td class=\"tdTitle\">Ʒ��</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->brand, sizeof(info->brand), str_get);
        free(str_get);
        str_get = NULL;
    }

    // �ͺ�
    left = "<tr><td class=\"tdTitle\">�ͺ�</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->model, sizeof(info->model), str_get);
        free(str_get);
        str_get = NULL;
    }

    // ��ɫ
    left = "<tr><td class=\"tdTitle\">��ɫ</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->color, sizeof(info->color), str_get);
        free(str_get);
        str_get = NULL;
    }

    // ����ϵͳ
    left = "<tr><td class=\"tdTitle\">����ϵͳ</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->os, sizeof(info->os), str_get);
        free(str_get);
        str_get = NULL;
    }

    // �����ڴ�
    left = "<tr><td class=\"tdTitle\">�����ڴ�</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->mem, sizeof(info->mem), str_get);
        free(str_get);
        str_get = NULL;
    }

    // ������ʽ
    left = "<tr><td class=\"tdTitle\">������ʽ</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->net_type, sizeof(info->net_type), str_get);
        free(str_get);
        str_get = NULL;
    }

    // �۸�
    // "<strong class=\"p-price\"><img data-img=\"3\" src =\""
    // "<strong class=\"p-price\"><img data-img=\"3\" src=\""
    left = "<strong class=\"p-price\"><img data-img=\"3\" src =\"";
    right = "\"/></strong>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->str_price, URL_LEN, str_get);
        free(str_get);
        str_get = NULL;
    }

    // url
    strcpy_s(info->url, URL_LEN, page->url);

    // img url
    left = "jqimg=\"";
    right = "\"/>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->img_url, URL_LEN, str_get);
        free(str_get);
        str_get = NULL;
    }

    return 0;
}

int parse_page_info(struct page_t* page, struct mobile_info* info)
{
	int page_c = PRO_NONE;

	if (NULL == page
		|| NULL == page->page_buf
		|| NULL == info)
	{
		return -1;
	}

	page_c = *(int*)(page->page_buf + 1);
	if (page_c > PRO_360BUY_START && page_c < PRO_360BUY_END)
	{
		return parse_mobile_info_360buy(page, info);
	}
	else if (page_c > PRO_AMAZON_START && page_c < PRO_AMAZON_END)
	{
		return parse_mobile_info_amazon(page, info);
	}
    else if (page_c > PRO_GOME_START && page_c < PRO_GOME_END)
    {
        return parse_mobile_info_gome(page, info);
    }
	else if (PRO_NONE == page_c)
	{
		return -1;
	}
	else
	{
		fprintf(stderr, "Page class wrong.....%d\n", page_c);
		return -1;
	}

	return 0;
}