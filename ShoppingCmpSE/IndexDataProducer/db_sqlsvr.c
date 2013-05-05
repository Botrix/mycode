#include <Windows.h>
#include "db_sqlsvr.h"
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>
#include <stdio.h>
#include "base_def.h"

#define MB2WC(c_buf, c_len, wc_buf_DSN, wc_len)	\
	MultiByteToWideChar(CP_ACP, 0, c_buf, c_len, wc_buf_DSN, wc_len)
#define WC2MB(wc_buf_DSN, wc_len, c_buf, c_len) \
	WideCharToMultiByte(CP_OEMCP, NULL, wc_buf_DSN, wc_len, c_buf, c_len, NULL, FALSE)

static SQLHENV s_henv = SQL_NULL_HENV;
static SQLHDBC s_hdbc = SQL_NULL_HDBC;
static SQLHSTMT s_hstmt = SQL_NULL_HSTMT;

static char strDSN[SQL_MAX_DSN_LENGTH+1] = "SCSE";
static char strUID[MAXNAME] = "sa";
static char strPWD[MAXNAME] = "123456";

static char* sql_create_db = "if not exists (select name from master.dbo.sysdatabases \
							 where name='product') \
							 create database product";
// 
// static char* sql_360buy_create = "if not exists (select * from sys.objects \
// 								 where name='mobile_360buy') \
// 								 create table mobile_360buy( \
// 								 brand char(64), \
// 								 model char(64), \
// 								 color char(64), \
// 								 os char(64), \
// 								 mem char(64), \
// 								 net_type char(64), \
// 								 good_judge_rate int, \
// 								 judge_count int, \
// 								 url char(512), \
// 								 url_img char(512), \
// 								 url_price char(512), \
// 								 )";
// 
// static char* sql_amazon_create = "if not exists (select * from sys.objects \
// 								 where name='mobile_amazon') \
// 								 create table mobile_amazon( \
// 								 brand char(64), \
// 								 model char(64), \
// 								 color char(64), \
// 								 os char(64), \
// 								 mem char(64), \
// 								 net_type char(64), \
// 								 good_judge_rate int, \
// 								 judge_count int, \
// 								 url char(512), \
// 								 url_img char(512), \
// 								 price int, \
// 								 )";
// 
// static char* sql_gome_create = "if not exists (select * from sys.objects \
//                                  where name='mobile_gome') \
//                                  create table mobile_gome( \
//                                  brand char(64), \
//                                  model char(64), \
//                                  color char(64), \
//                                  os char(64), \
//                                  mem char(64), \
//                                  net_type char(64), \
//                                  good_judge_rate int, \
//                                  judge_count int, \
//                                  url char(512), \
//                                  url_img char(512), \
//                                  price int, \
//                                  )";

static char* sql_table_clear = "if exists (select * from sys.objects \
                             where name='mobile_products') \
                             truncate table mobile_products";

static char* sql_table_create = "if not exists (select * from sys.objects \
                                where name='mobile_products') \
                                create table mobile_products( \
                                ID int IDENTITY(1, 1) PRIMARY KEY, \
                                brand_model nvarchar(64), \
                                details nvarchar(64), \
                                good_judge_rate int,    \
                                judge_count int,    \
                                url varchar(512),   \
                                url_img varchar(512),   \
                                price varchar(512), \
                                seller nvarchar(16),    \
                                )";
int connect_db_sqlsvr(void)
{
	wchar_t wc_buf_DSN[MAXNAME] = {0};
	wchar_t wc_buf_UID[MAXNAME] = {0};
	wchar_t wc_buf_PWD[MAXNAME] = {0};
	int ret = 0;

	RETCODE ret_code = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &s_henv);
	if (ret_code != SQL_SUCCESS)
	{
		return -1;
	}

	ret_code = SQLSetEnvAttr(s_henv, SQL_ATTR_ODBC_VERSION, 
						     (SQLPOINTER)SQL_OV_ODBC3,
							 SQL_IS_INTEGER);
	if (ret_code != SQL_SUCCESS)
	{
		return -1;
	}

	ret_code = SQLAllocHandle(SQL_HANDLE_DBC, s_henv, &s_hdbc);
	if (ret_code != SQL_SUCCESS)
	{
		return -1;
	}

	// connnect odbc
#ifdef _UNICODE
	MB2WC(strDSN, strlen(strDSN), wc_buf_DSN, MAXNAME);
	MB2WC(strUID, strlen(strUID), wc_buf_UID, MAXNAME);
	MB2WC(strPWD, strlen(strPWD), wc_buf_PWD, MAXNAME);
	ret_code = SQLConnect(s_hdbc, wc_buf_DSN, wcslen(wc_buf_DSN), 
						  wc_buf_UID, wcslen(wc_buf_UID), wc_buf_PWD, wcslen(wc_buf_PWD));
#else
	ret_code = SQLConnect(s_hdbc, strDSN, strlen(strDSN), strUID, strlen(strUID), strPWD, 0);
#endif
	if (ret_code != SQL_SUCCESS
		&& ret_code != SQL_SUCCESS_WITH_INFO)
	{
		return -1;
	}

    /* 如果表中有数据则清空 */
    ret = exe_sql_without_res(sql_table_clear);

	/* 如果表没有常见则创建 */
	ret = exe_sql_without_res(sql_create_db);
	if (-1 == ret)
	{
		fprintf(stderr, "创建数据库失败!\n");
		return -1;
	}

	ret = exe_sql_without_res(sql_table_create);
	
	return 0;
}

void disconnect_db_sqlsvr(void)
{
	SQLDisconnect(s_hdbc);
	SQLFreeConnect(s_hdbc);
	SQLFreeConnect(s_henv);
}

int exe_sql_without_res(char* sql)
{
	wchar_t wc_sql[MAX_SQL_LEN] = {0};
	RETCODE ret_code;
	SQLRETURN ret;
	char list[1024] = {0};
	int len = 1024;
	ret_code = SQLAllocHandle(SQL_HANDLE_STMT, s_hdbc, &s_hstmt);
	if (ret_code != SQL_SUCCESS
		&& ret_code != SQL_SUCCESS_WITH_INFO)
	{
		return -1;
	}

#ifdef _UNICODE
	MB2WC(sql, strlen(sql), wc_sql, MAX_SQL_LEN);
	SQLPrepare(s_hstmt, wc_sql, wcslen(wc_sql));
	ret = SQLExecute(s_hstmt);
	//ret = SQLExecDirect (s_hstmt, wc_sql, wcslen(wc_sql));  
#else
	ret = SQLExecDirect (s_hstmt, sql, strlen(sql)); 
#endif
	if (ret == -1)
	{
		wchar_t c_state[8] = {0};
		long native_err;
		short msg_len = 0;
		wchar_t err_msg[SQL_MAX_MESSAGE_LENGTH] = {0};
		SQLGetDiagRec(SQL_HANDLE_STMT, s_hstmt, 1, c_state, 
			&native_err, err_msg, SQL_MAX_MESSAGE_LENGTH, &msg_len);

		native_err = 0;
	}

	SQLCloseCursor (s_hstmt);  
	SQLFreeHandle (SQL_HANDLE_STMT, s_hstmt);  

	return ret;
}

int exe_sql_with_res(char* sql)
{
	wchar_t wc_sql[MAX_SQL_LEN] = {0};
	RETCODE ret_code;
	SQLRETURN ret;
	char list[1024] = {0};
	int len = 1024;
	ret_code = SQLAllocHandle(SQL_HANDLE_STMT, s_hdbc, &s_hstmt);
	if (ret_code != SQL_SUCCESS
		&& ret_code != SQL_SUCCESS_WITH_INFO)
	{
		return -1;
	}

#ifdef _UNICODE
	MB2WC(sql, strlen(sql), wc_sql, MAX_SQL_LEN);
	SQLPrepare(s_hstmt, wc_sql, wcslen(wc_sql));
	ret = SQLExecute(s_hstmt);
	//ret = SQLExecDirect (s_hstmt, wc_sql, wcslen(wc_sql));  
#else
	ret = SQLExecDirect (s_hstmt, sql, strlen(sql)); 
#endif
	if (ret == -1)
	{
		wchar_t c_state[8] = {0};
		long native_err;
		short msg_len = 0;
		wchar_t err_msg[SQL_MAX_MESSAGE_LENGTH] = {0};
		SQLGetDiagRec(SQL_HANDLE_STMT, s_hstmt, 1, c_state, 
			&native_err, err_msg, SQL_MAX_MESSAGE_LENGTH, &msg_len);

		native_err = 0;
	}

	SQLBindCol(s_hstmt, 1, SQL_C_CHAR, list, 1024, 0);
	do{
		ret_code = SQLFetch(s_hstmt);
		if (ret_code == SQL_NO_DATA)
		{
			break;
		}

		printf("%s\n", list);
	}while(1);

	SQLCloseCursor (s_hstmt);  
	SQLFreeHandle (SQL_HANDLE_STMT, s_hstmt);  

	return ret;
}