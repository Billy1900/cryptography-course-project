#include "Rainbow_Table_head.hpp"

bool md5Cracker(unsigned char* md5code, Tree_Node_ptr &T){
    // 再依次假设其在彩虹表中的位置
    printf("正在查找....\n");
    unsigned char* pass = md5code;
    for (int i = 0; i < nodeInChain; i++) {
        pass = Red(md5code, i, stringlength, NULL);
        for (int round = i+1; round < nodeInChain; round++) {
            unsigned char mdStr[16] = {0};
            MD5(pass, stringlength, mdStr);
            pass = Red(mdStr, round, stringlength, NULL);
        }
        // 在二叉树中搜索该尾节点
        Tree_Node_ptr matched = Tree_search(T, pass);
        if (matched != NULL) {
            // 如果匹配到链尾相同， 则要从链头计算一遍
            unsigned char* chainHead = matched->head;
            for (int round = 0; round < i; round++) {
                unsigned char mdStr[16] = {0};
                MD5(chainHead, stringlength, mdStr);
                chainHead = Red(mdStr, round, stringlength, NULL);
            }
            unsigned char mdStr[16] = {0};
            MD5(chainHead, stringlength, mdStr);
            // 如果比较发现并不一致, 则认为是错误的链, 应该继续比较
            bool flag = 0; // 指示两者的比较情况
            for (int i = 0; i < 16; i++)
                if (mdStr[i] != md5code[i]) {
                    flag = 1;
                    break;
                }
            printf("head: %s\t tail: %s result:%s\n",matched->head, pass, chainHead);
            if (!flag){
                printf("找到啦! 结果是:\n");
                printf("%s\n",chainHead);
                return true;
            }
        }
    }
    // 匹配失败, 彩虹链中找不到这个md5
    printf("嘤嘤嘤，解密失败!\n");
    return true;
}

