/* Module: Sqlite服务模块
 * Author: 闫科宇
 * Date:   2020-05-27 17:53:14
 * Desc:   提供Sqlite数据持久化服务，对外开放数据表的增删查改API
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite_service.h"
#include "logger.h"
#include "dbhelper.h"

/**
 * 新增货物信息
 * @param goodsInfo 货物信息
 * @return 0 成功 | -1 失败
 */
int insertGoods(GoodsInfo goodsInfo) {
    int ret;
    char* err_msg;
    char sql[1024];
    sprintf(sql, "insert into goods(dev_no, goodsId, goodsCount, goodsTime) values('%d','%d','%d','%s')",
        goodsInfo.dev_no, goodsInfo.goodsId, goodsInfo.goodsCount, goodsInfo.goodsTime);
	ret = execSql(sql, &err_msg);
	if (ret != SQLITE_OK)
	{
		printf("Table goods insert data error!\n");
        printf("Error: %s\n", err_msg);
		return -1;
	}
    return 0;
}

/**
 * 查询所有货物信息 TODO待修改（改成传入结果集、row、col）
 * @return 0 成功 | 1 失败
 */
int findAllGoods() {
    int ret;
    char* err_msg;
    char sql[1024];
    char **sel_ret; 
	int row, col;
	
	strcpy(sql, "select dev_no,goodsId,goodsCount,goodsTime from goods");
	ret = querySql(sql, &sel_ret, &row, &col, &err_msg);
	if (ret != SQLITE_OK) return -1;

	if(row > 0) { /* have data */
        int dev_no = atoi(sel_ret[col]);
        int goodsId = atoi(sel_ret[col+1]);
        int goodsCount = atoi(sel_ret[col+2]);
        char goodsTime[30] = {0};
        strcpy(goodsTime, sel_ret[col+3]);
        
        printf("Read goods data:\n");
        printf("\tdev_no: %d\n", dev_no);
        printf("\tgoodsId: %d\n", goodsId);
        printf("\tgoodsCount: %d\n", goodsCount);
        printf("\tgoodsTime: %s\n", goodsTime);
    } else {
        printf("@@Table goods no data!\n");
    }
    return 0;
}

/**
 * 新增环境信息
 * @param collectEnv 环境信息
 * @return 0 成功 | -1 失败
 */
int insertCollectEnv(CollectEnv collectEnv) {
    int ret;
    char* err_msg;
    char sql[1024];
    sprintf(sql, "insert into collect_env(dev_no32, temperatureVal, humidityVal, illuminationVal, envtime) \
                values('%d','%f','%f','%f','%s')",
        collectEnv.dev_no32, collectEnv.temperatureVal, collectEnv.humidityVal, collectEnv.illuminationVal, 
        collectEnv.envtime);
	ret = execSql(sql, &err_msg);
	if (ret != SQLITE_OK)
	{
		printf("Table collect_env insert data error!\n");
        printf("Error: %s\n", err_msg);
		return -1;
	}
    return 0;
}

/**
 * 查询所有环境信息
 * @return 0 成功 | -1 失败
 */
int findAllCollectEnv() {
    int ret;
    char* err_msg;
    char sql[1024];
    char **sel_ret; 
	int row, col;
	
	strcpy(sql, "select dev_no32, temperatureVal, humidityVal, illuminationVal, envtime from collect_env");
	ret = querySql(sql, &sel_ret, &row, &col, &err_msg);
	if (ret != SQLITE_OK) return -1;
	if(row > 0) { /* have data */
        int dev_no32 = atoi(sel_ret[col]);
        float temperatureVal = atof(sel_ret[col+1]);
        float humidityVal = atof(sel_ret[col+2]);
        float illuminationVal = atof(sel_ret[col+3]);
        char envtime[30] = {0};
        strcpy(envtime, sel_ret[col+4]);
        
        printf("Read collect_env data:\n");
        printf("\tdev_no32: %d\n", dev_no32);
        printf("\ttemperatureVal: %f\n", temperatureVal);
        printf("\thumidityVal: %f\n", humidityVal);
        printf("\tilluminationVal: %f\n", illuminationVal);
        printf("\tenvtime: %s\n", envtime);
    } else {
        printf("@@Table collect_env no data!\n");
    }
    return 0;
}

// SQLITE数据存储线程 入口函数
void* pthread_sqlite() {
    logger("INFO", "[Sqlite Module] >> \033[0;32m[...]");

    init_db();
    logger("INFO", "[Sqlite Module] init_db \033[0;32m[Ok]");

    #ifdef __DEBUG__
    // 货物信息表
    // 1. 数据插入
    GoodsInfo goodsInfo;
    goodsInfo.dev_no = 1;
    goodsInfo.goodsId = 1;
    goodsInfo.goodsCount = 99;
    strcpy(goodsInfo.goodsTime, "2020/5/16");
    //insertGoods(goodsInfo);
    // 2. 数据查询
    findAllGoods();

    // 环境信息表
    // 1. 数据插入
    CollectEnv collectEnv;
    collectEnv.dev_no32 = 1;
    collectEnv.temperatureVal = 23.2;
    collectEnv.humidityVal = 67;
    collectEnv.illuminationVal = 50;
    strcpy(collectEnv.envtime, "2020/5/16");
    //insertCollectEnv(collectEnv);
    // 2. 数据查询
    findAllCollectEnv();
    #endif // !__DEBUG__
    

    logger("INFO", "[Sqlite Module] >> [Close]");
    return NULL;
}