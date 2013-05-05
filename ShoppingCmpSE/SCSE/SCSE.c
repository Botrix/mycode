#include <stdio.h>
#include <stdlib.h>
#include "page_cache.h"
#include "data_update.h"
#include "db_sqlsvr.h"

int main()
{
	init_page_cache();
	connect_db_sqlsvr();

	update_data();

	disconnect_db_sqlsvr();
	release_page_cache();

	system("pause");
	return 0;
}