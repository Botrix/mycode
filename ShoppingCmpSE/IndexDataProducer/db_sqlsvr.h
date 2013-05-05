#ifndef SCSE_DB_SQLSVR_H 
#define SCSE_DB_SQLSVR_H

#ifdef __cplusplus
extern "C"
{
#endif
	
extern int connect_db_sqlsvr(void);

extern void disconnect_db_sqlsvr(void);

extern int exe_sql_without_res(char* sql);

extern int exe_sql_with_res(char* sql);

#ifdef __cplusplus
}
#endif

#endif