#include "Rainbow_Table_head.hpp"

int main(void){
    int op;
    printf("请选择模式: 1) 彩虹表生成 2) md5解密\n");
    scanf("%d",&op);
    switch (op) {
        case 1:
            // 能破译长度为 stringlength 的字符串（小写字母和数字）
            // 每条彩虹链中计算 nodeInChain 次节点
            rainbow_table_gen(stringlength, nodeInChain);
            break;
        case 2:
        {
            
            Tree_Node_ptr T = NULL;
            clock_t start = clock();
            openRainbowTableFile("/Users/cl/Desktop/RT_1000.txt", T);
            clock_t end = clock();
            printf("Time cost : %lu",(end-start)/CLOCKS_PER_SEC);
            char md5Str[32] = {0};
            while (true) {
                printf("\n请输入要解密的md5码:\n");
                scanf("%32s",md5Str);
                // 将字符串转换成16进制
                unsigned char *md5 = formatConvert(md5Str);
                md5Cracker(md5, T);
            }
            break;
        }
        default:
            break;
    }
    
}

