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
		fprintf(stderr, "粗大事啦,create_page_buf失败...\n");
		return -1;
	}

	ret = get_url_page_libcurl(tsk->url, page_buf);
    if (-1 == ret)
    {
        fprintf(stderr, "\nget_url_page_libcurl失败:url : %s\n", tsk->url);
        goto err_exit;
    }

	ret = get_rbuf_of_page_buf(page_buf, &rbuf, &rsize);
	if (-1 == ret)
	{
		fprintf(stderr, "get_rbuf_of_page_buf失败\n");
		goto err_exit;
	}

    // 是否要先找到一个标记
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
            // 将查找位置移到flag后面
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
		// 先找到right
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

    // 是否要先找到一个标记
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
            // 将查找位置移到flag后面
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
		// 先找到right
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

    // 设置价格、好评（从上层获取的）
    //strcpy_s(info->str_price, URL_LEN, page->price_url);
	sprintf_s(info->str_price, URL_LEN, "%d.00", page->price);
    info->good_judge_rate = page->good_judge_rate;
    info->judge_count = page->judge_count;

	// 品牌
	left = "<tr><td class=\"tdTitle\">品牌</td><td>";
	regex(rbuf, left, right, &str_get);

	if (str_get != NULL)
	{
		strcpy_s(info->brand, sizeof(info->brand), str_get);
		free(str_get);
		str_get = NULL;
	}
	else
	{
		fprintf(stderr, "品牌字段为空，丢掉!\n");
		return -1;
	}

	// 型号
	left = "<tr><td class=\"tdTitle\">型号</td><td>";
	regex(rbuf, left, right, &str_get);

	if (str_get != NULL)
	{
		strcpy_s(info->model, sizeof(info->model), str_get);
		free(str_get);
		str_get = NULL;
	}

	// 颜色
	left = "<tr><td class=\"tdTitle\">颜色</td><td>";
	regex(rbuf, left, right, &str_get);
	if (str_get != NULL)
	{
		strcpy_s(info->color, sizeof(info->color), str_get);
		free(str_get);
		str_get = NULL;
	}

	// 操作系统
	left = "<tr><td class=\"tdTitle\">操作系统</td><td>";
	regex(rbuf, left, right, &str_get);
	if (str_get != NULL)
	{
		strcpy_s(info->os, sizeof(info->os), str_get);
		free(str_get);
		str_get = NULL;
	}

	// 机身内存
	left = "<tr><td class=\"tdTitle\">机身内存</td><td>";
	regex(rbuf, left, right, &str_get);
	if (str_get != NULL)
	{
		strcpy_s(info->mem, sizeof(info->mem), str_get);
		free(str_get);
		str_get = NULL;
	}

	// 网络制式
	left = "<tr><td class=\"tdTitle\">网络制式</td><td>";
	regex(rbuf, left, right, &str_get);
	if (str_get != NULL)
	{
		strcpy_s(info->net_type, sizeof(info->net_type), str_get);
		free(str_get);
		str_get = NULL;
	}

	// 价格
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
// 			fprintf(stderr, "获取价格失败...\n");
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

    // 获取title
    left = "<span id=\"btAsinTitle\">";
    right = "</span>";
    regex(rbuf, left, right, &str_get);
    if (str_get != NULL)
    {
        // 存储数据
        char buf_tmp[1024] = {0};
        UTF82GBK(str_get, strlen(str_get), buf_tmp, 1024);

        strcpy_s(info->details, sizeof(info->details), buf_tmp);
        free(str_get);
        str_get = NULL;
    }

    // 品牌
    // 先移到title处
    find_str = "品牌: <a href=\"";
    memset(buf_left, 0, URL_LEN);
    GBK2UTF8(find_str, strlen(find_str), buf_left, URL_LEN);
    find_str = buf_left;
    find_pos = find(rbuf, find_str);
    if (NULL == find_pos)
	{
		fprintf(stderr, "品牌字段为空，丢掉!\n");
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
		fprintf(stderr, "品牌字段为空，丢掉!\n");
		return -1;
	}

    // 型号
    left = "<li><b>型号:</b> ";
    memset(buf_left, 0, URL_LEN);
    GBK2UTF8(left, strlen(left), buf_left, URL_LEN);
    left = buf_left;
    right = "</li>";
    regex(rbuf, left, right, &str_get);
    if (str_get != NULL)
    {
        char buf_tmp[64] = {0};
        UTF82GBK(str_get, strlen(str_get), buf_tmp, 64);

        // 将型号转换成大写
        // strupr(buf_tmp); // 不严谨，比如iphone
        strcpy_s(info->model, sizeof(info->model), buf_tmp);
        free(str_get);
        str_get = NULL;
    }

    // 价格
    left = "<span id=\"actualPriceValue\"><b class=\"priceLarge\">";
    right = "</b></span>";
    regex(rbuf, left, right, &str_get);
    if (str_get != NULL)
    {
        char buf_tmp[512] = {0};
        UTF82GBK(str_get, strlen(str_get), buf_tmp, 512);

		repalce_of(buf_tmp, ",", "");
		repalce_of(buf_tmp, " ", "");
		repalce_of(buf_tmp, "￥", "");

        strcpy_s(info->str_price, sizeof(info->str_price), buf_tmp);
        free(str_get);
        str_get = NULL;
    }

    // img url
    // img 在上层获取
    strcpy_s(info->img_url, URL_LEN, page->img_url);
//     left = "function(){var i=new Image;i.src = \"";
//     right = "\";}";
//     regex(rbuf, left, right, &str_get);
//     if (NULL == str_get)
//     {
//         fprintf(stderr, "获取img url失败...\n");
//     }
// 
//     if (str_get != NULL)
//     {
//         strcpy_s(info->img_url, URL_LEN, str_get);
//         free(str_get);
//         str_get = NULL;
//     }

    // 评论总数 
    find_str = "<span class='sl-title-tab-title'>总体不错</span>";
    memset(buf_left, 0, URL_LEN);
    GBK2UTF8(find_str, strlen(find_str), buf_left, URL_LEN);
    find_str = buf_left;
    find_pos = find(rbuf, find_str);
    if (NULL == find_pos)
    {
        //fprintf(stderr, "获取‘总评数’失败...\n");
        // 获取失败有可能是还没有开卖呢
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

    // 好评率
    left = "<span style='float:right'>";
    right = "%</span>";
    regex(find_pos, left, right, &str_get);
    if (str_get != NULL)
    {
        info->good_judge_rate = atoi(str_get);
        free(str_get);
        str_get = NULL;
    }

    // 如果好评率没有，则通过4星、5星来转换
    if (0 == info->judge_count)
    {
        int good_judge_count = 0;

        find_str = "条评论</b>";
        memset(buf_left, 0, URL_LEN);
        GBK2UTF8(find_str, strlen(find_str), buf_left, URL_LEN);
        find_str = buf_left;
        find_pos = find(rbuf, find_str);

        if (NULL == find_pos)
        {
            return 0;
        }

        // 将find_pos向前移动，移到数字前面去
        find_pos -= 16;

        left = "<b>";
        right = "条评论</b>";
        memset(buf_left, 0, URL_LEN);
        GBK2UTF8(right, strlen(right), buf_left, URL_LEN);
        right = buf_left;

        regex(find_pos, left, right, &str_get);

        // 去掉评论总数中的','
        repalce_of(str_get, ",", "");

        info->judge_count = atoi(str_get);
        free(str_get);
        str_get = NULL;

        if (info->judge_count <= 0)
        {
            return 0;
        }

        // 获取5星个数
        find_str = "5 星</a>";
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

        // 获取4星个数
        find_str = "4 星</a>";
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
        // 计算好评率
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

    // 品牌
    left = "<tr><td class=\"tdTitle\">品牌</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->brand, sizeof(info->brand), str_get);
        free(str_get);
        str_get = NULL;
    }

    // 型号
    left = "<tr><td class=\"tdTitle\">型号</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->model, sizeof(info->model), str_get);
        free(str_get);
        str_get = NULL;
    }

    // 颜色
    left = "<tr><td class=\"tdTitle\">颜色</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->color, sizeof(info->color), str_get);
        free(str_get);
        str_get = NULL;
    }

    // 操作系统
    left = "<tr><td class=\"tdTitle\">操作系统</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->os, sizeof(info->os), str_get);
        free(str_get);
        str_get = NULL;
    }

    // 机身内存
    left = "<tr><td class=\"tdTitle\">机身内存</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->mem, sizeof(info->mem), str_get);
        free(str_get);
        str_get = NULL;
    }

    // 网络制式
    left = "<tr><td class=\"tdTitle\">网络制式</td><td>";
    regex(rbuf, left, right, &str_get);

    if (str_get != NULL)
    {
        strcpy_s(info->net_type, sizeof(info->net_type), str_get);
        free(str_get);
        str_get = NULL;
    }

    // 价格
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