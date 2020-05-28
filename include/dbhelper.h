#ifndef _DBHELPER_H
#define _DBHELPER_H

#include <sqlite3.h>

int init_db();
void close_db();
int execSql(char* sql, char** err_msg);
int querySql(char* sql, char*** sel_ret, int* row, int* col, char** err_msg);

#endif // !_DBHELPER_H