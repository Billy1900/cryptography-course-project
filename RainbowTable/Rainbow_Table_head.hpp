#ifndef Rainbow_Table_Gen_hpp
#define Rainbow_Table_Gen_hpp

#include "openssl/md5.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"

#define random(x) (rand()%x)
#define chainNumToWrite 1000
#define nodeInChain 1000
#define stringlength 7
#define charLimit 35
#define LH +1
#define EH 0
#define RH -1
#define rainbow_table_filename "/Users/cl/Desktop/RT_1000.txt"

typedef unsigned char* ElemType;
typedef struct Tree_Node{
    ElemType head,value;
    char bf; // 节点的平衡因子
    struct Tree_Node *left, *right; // 左、右孩子指针
} Tree_Node, *Tree_Node_ptr;

void rainbow_table_gen(int length, int chainLength);
unsigned char *Red(unsigned char* md5_cr, int round, int lenth, unsigned char* pre_str);
bool insert_node_to_write(Tree_Node_ptr &head, Tree_Node_ptr &tail, Tree_Node_ptr &current);
bool md5Cracker(unsigned char* md5code, Tree_Node_ptr &T);
bool openRainbowTableFile(char *filename, Tree_Node_ptr &head);
unsigned char encode(unsigned char);
unsigned char* formatConvert(char *raw);

Tree_Node_ptr Tree_search(Tree_Node_ptr head, ElemType value);

bool Tree_inOrder_traverse(Tree_Node_ptr);
bool Tree_preOrder_traverse(Tree_Node_ptr);
bool Tree_lastOrder_traverse(Tree_Node_ptr);

void R_Rotate(Tree_Node_ptr &p);
void L_Rotate(Tree_Node_ptr &p);
void LeftBalance(Tree_Node_ptr &T);
void RightBalance(Tree_Node_ptr &T);

bool Tree_insert_AVL(Tree_Node_ptr &T, ElemType head, ElemType e, bool &taller);
#endif /* Rainbow_Table_Gen_hpp */
