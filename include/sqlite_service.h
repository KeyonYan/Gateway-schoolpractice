#ifndef _SQLITE_SERVICE_H_
#define _SQLITE_SERVICE_H_

// 货物信息表 VO
typedef struct goodsInfo {
    int dev_no;
    int goodsId;
    int goodsCount;
    char goodsTime[30];
}GoodsInfo;
// 环境信息表 VO
typedef struct collect_env {
    int dev_no32;
    float temperatureVal;
    float humidityVal;
    float illuminationVal;
    char envtime[30];
}CollectEnv;

void* pthread_sqlite();

// 数据持久层接口
int insertGoods(GoodsInfo goodsInfo);
int findAllGoods();
int insertCollectEnv(CollectEnv collectEnv);
int findAllCollectEnv();


#endif // !_SQLITE_SERVICE_H_
