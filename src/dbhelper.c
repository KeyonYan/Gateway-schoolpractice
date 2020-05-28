/* Module: 数据库帮助库
 * Author: 闫科宇
 * Date:   2020-05-28 22:27:19
 * Desc:   提供数据库的初始化、数据库连接的关闭、数据库的基础增删改查接口
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbhelper.h"

#define DBNAME "gateway.db"

static sqlite3 *sql_db = NULL;

/**
 * 打开数据库
 * @return open result
 */
int open_db()
{
	if (NULL == sql_db)
	{
		return sqlite3_open(DBNAME, &sql_db);
	}
	return SQLITE_OK;
}

/**
 * 关闭数据库 (public)
 */
void close_db()
{
	if (NULL != sql_db)
		sqlite3_close(sql_db);
}

/**
 * 检查表是否存在
 * @param table_name 	表名
 * @return 				0 存在 | 1 不存在
 */
int check_table_isexist(char *table_name)
{
	int ret;
	char sql[1024];
	char **sel_ret;
	int row, col;
	char *err = NULL;
	
	ret = open_db();
	if (ret != SQLITE_OK)
	{
		printf("open db error\n");
		return -1;
	}
	
	sprintf(sql, "SELECT COUNT(*) FROM sqlite_master where type='table' and name='%s'", table_name);
	if (SQLITE_OK != sqlite3_get_table(sql_db, sql, &sel_ret, &row, &col, &err))
	{
		printf("check table %s error\n", table_name);
		return -2;
	}
	
	if (row > 0)
	{
		int count = atoi(sel_ret[col]);
		sqlite3_free_table(sel_ret);
		return count;
	}
	
	sqlite3_free_table(sel_ret);
	return 0;
}

/**
 * 创建表
 * @return 0 表已存在 | <0 失败 | 1 创建成功
 */
int create_table()
{
	int ret;
    char *err_msg;
	char sql[1024] = {0};
	
	ret = open_db();
	if(ret != SQLITE_OK)
	{
		printf("open db error\n");
		return -1;
	}
	

	// 创建 货物信息表
    if (check_table_isexist("goods"))
	{
		printf("table goods has exist!\n");
		return 0;
	}
	strcpy(sql, "create table goods(dev_no int not null, \
                                    goodsId int not null, \
                                    goodsCount int not null, \
                                    goodsTime varchar(30) not null);");
	ret = sqlite3_exec(sql_db, sql, NULL,NULL,&err_msg);
	if(ret != SQLITE_OK)
	{
		printf("CREATE table goods error!\n");
        printf("Error: %s\n", err_msg);
		return -2;
	}

    // 创建 环境信息表
    if (check_table_isexist("collect_env"))
	{
		printf("table collect_env has exist!\n");
		return 0;
	}
    strcpy(sql, "create table collect_env(dev_no32 int not null, \
                                    temperatureVal float not null, \
                                    humidityVal float not null, \
                                    illuminationVal float not null, \
                                    envtime varchar(30) not null);");
	ret = sqlite3_exec(sql_db, sql, NULL,NULL,&err_msg);
	if(ret != SQLITE_OK)
	{
		printf("CREATE table collect_env error!\n");
        printf("Error: %s\n", err_msg);
		return -2;
	}
	
	return 1;
}

/**
 * 执行增删改sql (public)
 * @param sql 		sql字符串
 * @param err_msg 	错误信息
 * @return 			0 成功 | !0 失败
 */
int execSql(char* sql, char** err_msg) {
    return sqlite3_exec(sql_db, sql, NULL, NULL, err_msg);
}

/**
 * 执行查询sql (public)
 * @param sql 		sql字符串
 * @param sel_ret 	查询结果集
 * @param row 		结果集行数
 * @param col 		结果集列数
 * @param err_msg 	错误信息
 */
int querySql(char* sql, char*** sel_ret, int* row, int* col, char** err_msg) {
    return sqlite3_get_table(sql_db, sql, sel_ret, row, col, err_msg);
}

/**
 * 数据库初始化函数(public)
 * @return 0 成功 | -1 失败
 */
int init_db()
{
	if (SQLITE_OK == open_db())
		return create_table();
	
	return -1;
}
