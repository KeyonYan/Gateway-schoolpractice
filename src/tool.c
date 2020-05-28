/* Module: 工具模块
 * Author: 闫科宇
 * Date:   2020-05-28 21:15:53
 * Desc:   一个工具类，包含字符串处理等操作
 */

/**
 * 分割字符串
 * @param srcStr      源字符串
 * @param delim       分割符
 * @param num         子串个数
 * @return 所有子串
 */
char** split(char *srcStr, char delim, int *num) {
    *num = 1;
    int srcStrlen = strlen(srcStr);
    char **res = (char **)malloc(sizeof(char*) * (srcStrlen+1));
    res[0] = (char *)malloc(sizeof(char) * srcStrlen);
    memset(res[0], 0, strlen(res[0]));
    int k = 0;
    for (int i = 0, j = 0; i < strlen(srcStr); i++) {
        if (srcStr[i] != delim) {
            res[k][j++] = srcStr[i];
        } else {
            k++; j = 0;
            res[k] = (char *)malloc(sizeof(char) * srcStrlen);
            memset(res[k], 0, strlen(res[k]));
        }
    }
    *num = k+1;
    // 多分配一行，置0
    res[*num] = (char *)malloc(sizeof(char)*srcStrlen);
    memset(res[*num], 0, strlen(res[*num]));
    res[*num] = 0;
    return res;
}

int main() {
    char str[] = "^^ 22a2 333 d";
    int num = 0;
    char **res = split(str, ' ', &num);
    printf("%d\n", num);
    for (int i = 0; i < num; i++) {
        printf("%s\n", res[i]);
    }

    // linux execvp()
    char cmd[] = "ls -l /etc";
    char **argv = split(cmd, ' ', &num);
    execvp(argv[0], argv);

    return 0;
}