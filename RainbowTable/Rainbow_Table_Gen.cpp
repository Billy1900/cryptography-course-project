#include "Rainbow_Table_head.hpp"

unsigned char* formatConvert(char *raw){
    /*
     将输入的字符串形式的 MD5 码转换成数字
     */
    unsigned char *converted = (unsigned char*)malloc(sizeof(unsigned char) * 16);
    for (int i=0; i<16; i++) {
        int a = 0,b = 0;
        if (raw[2*i] >= '0' && raw[2*i] <= '9')
            a = raw[2*i] - '0';
        else if (raw[2*i] >= 'a' && raw[2*i] <= 'f')
            a = raw[2*i] - 'a' + 10;
        
        if (raw[2*i + 1] >= '0' && raw[2*i + 1] <= '9')
            b = raw[2*i + 1] - '0';
        else if (raw[2*i + 1] >= 'a' && raw[2*i + 1] <= 'f')
            b = raw[2*i + 1] - 'a' + 10;
        converted[i] = (unsigned char)((a << 4) + b);
    }
    return converted;
}

unsigned char *Red(unsigned char* md5_cr, int round, int lenth,unsigned char* pre_str){
    /*
     约简函数，将 MD5 按照一定规则转换到 Z36 域中
     pre_str 是之前的字符串，释放其空间
     */
    if (pre_str != NULL) {
        free(pre_str);
    }
    unsigned char *Reduced = (unsigned char*)malloc(sizeof(char) * (lenth+1));
    for (int i = 0; i < lenth; i++){
        Reduced[i] = (md5_cr[(round + i*2) % 16] + round) % 36;// 随机选择md5中的若干位，映射到Z36中
        if (Reduced[i] >= 0 && Reduced[i] <= 9)
            Reduced[i] += '0';
        else if (Reduced[i] >= 10 && Reduced[i] <=36)
            Reduced[i] = Reduced[i] - 10 + 'a';
    }
    Reduced[lenth] = '\0';
    return Reduced;
}

bool insert_node_to_write(Tree_Node_ptr &head, Tree_Node_ptr &tail, Tree_Node_ptr &current){
    // 仅仅构成一个单向链表, head 指向链表首节点, tail 指向链表尾节点
    if (current == NULL) {
        return false;
    }
    if (head == NULL && tail == NULL) {
        // 当是空链时插入一个节点
        head = current;
        tail = current;
        return true;
    }
    tail->right = current;
    tail = current;
    return true;
}

void rainbow_table_gen(int length, int chainLength){
    int count = 0;
    int writeTime = 0;
    
    Tree_Node_ptr head_total = NULL;
    if (openRainbowTableFile(rainbow_table_filename, head_total))
        printf("文件读取完成!\n");
    else printf("正在新建文件.....\n");
    
    int index[stringlength] = {0,2,35,14,0,0,0};
    int loop_level; // 指示当前循环的层数
    
    // 当生成的链数少于 chainNum 时不进行文件写操作
    count = 0;
    Tree_Node_ptr head_to_write = NULL, tail_to_write = NULL;
    // 一个多重循环， 循环层数为待生成的字符串长度。用于遍历生成首节点
    do {
        // 每次都从最里层循环开始
        loop_level = stringlength-1;
        // 生成单个字符, 共生成 stringlength 长度的字符串
        unsigned char *head_str = (unsigned char *)malloc(sizeof(char) * (length+1));
        for (int i = 0; i < stringlength; i++) {
            head_str[i] = index[i];
            if (head_str[i] >= 0 && head_str[i] <= 9)
                head_str[i] += '0';
            else if (head_str[i] >= 10 && head_str[i] <=36)
                head_str[i] = head_str[i] - 10 + 'a';
        }
        head_str[length] = '\0';
        // 计算生成彩虹链中的最后一个节点
        unsigned char *node_str = (unsigned char *)malloc(sizeof(char) * (length+1));
        memcpy(node_str, head_str, sizeof(char) * (length+1));
        for (int round = 0; round < chainLength; round++) {
            unsigned char mdStr[16] = {0};
            MD5(node_str, length, mdStr);
            node_str = Red(mdStr, round, length, node_str);
        }
        // 尝试将这一条链（首节点和尾节点）插入总的树中
        bool taller = 0;
        if (Tree_insert_AVL(head_total, head_str, node_str, taller)){
            // 若成功插入, 则把这一条链插入到待写入的链表中
            Tree_Node_ptr node_to_write= (Tree_Node_ptr)malloc(sizeof(Tree_Node));
            node_to_write->head = head_str;
            node_to_write->value = node_str;
            node_to_write->left = NULL;
            node_to_write->right = NULL;
            //printf("%s %s\n",head_str, node_str);
            insert_node_to_write(head_to_write, tail_to_write, node_to_write);
            count ++; // 计数器增加
        }
        // 计数器达到 1000 时, 写文件
        if (count == chainNumToWrite){
            //Tree_preOrder_traverse(head_to_write);
            printf("!!写文件%d\n",++writeTime);
            FILE *fp = fopen(rainbow_table_filename, "a");
            Tree_Node_ptr writing = head_to_write, temp;
            while (writing != NULL) {
                // 读取节点信息, 写入文件后释放空间
                fputs((char *)writing->head, fp);
                fputs(" ", fp);
                fputs((char *)writing->value, fp);
                fputs("\n", fp);
                temp = writing->right;
                free(writing);
                writing = temp;
            }
            // 待写入的链表清空
            head_to_write = tail_to_write = NULL;
            // 计数器清空
            count = 0;
            fclose(fp);
        }
        // 循环控制
        while (loop_level > 0 && (++index[loop_level] > charLimit)) {
            index[loop_level] = 0;
            loop_level --;
        }
    } while (loop_level > 0);
}
