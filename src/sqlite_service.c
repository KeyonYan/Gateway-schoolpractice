/* Module: Sqlite服务模块
 * Author: 闫科宇
 * Date:   2020-05-27 17:53:14
 * Desc:   提供Sqlite数据持久化服务，对外开放数据表的增删查改API
 */
#include <stdio.h>
#include "sqlite_service.h"
#include "logger.h"

void* pthread_sqlite() {
    logger("INFO", ">> [Thread]Sqlite Module \033[0;32m[Start]");
    return NULL;
}