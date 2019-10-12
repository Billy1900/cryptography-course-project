#include "Rainbow_Table_head.hpp"
#include "string.h"
void R_Rotate(Tree_Node_ptr &p){
    /*
     对以*p为根的二叉排序树进行右旋处理，处理之后p指向新的树根节点，
     即旋转处理之前的左子树的根节点
     */
    Tree_Node_ptr lc = p->left;
    p->left = lc->right;
    lc->right = p;
    p = lc;
} // R_Rotate

void L_Rotate(Tree_Node_ptr &p){
    /*
     对以*p为根的二叉排序树进行左旋处理，处理之后p指向新的树根节点，
     即旋转处理之前的右子树的根节点
     */
    Tree_Node_ptr lc = p->right;
    p->right = lc->left;
    lc->left = p;
    p = lc;
}

Tree_Node_ptr Tree_search(Tree_Node_ptr head, ElemType value){
    // AVL树的搜索的非递归实现
    Tree_Node_ptr current = head;
    while (current != NULL && strcmp((char*)value, (char*)current->value) != 0) {
        if (strcmp((char*)value, (char*)current->value) < 0)
            current = current->left;
        else
            current = current->right;
    }
    return current;
}

bool Tree_inOrder_traverse(Tree_Node_ptr root){
    // 二叉树的中序遍历
    // 先访问左子树
    if(root->left != NULL)
        Tree_inOrder_traverse(root->left);
    // 再访问根节点
    printf("%s -> ",root->value);
    // 最后访问右子树
    if (root->right != NULL)
        Tree_inOrder_traverse(root->right);
    return true;
}

bool Tree_preOrder_traverse(Tree_Node_ptr root){
    // 二叉树的先序遍历
    // 先访问根节点
    printf("%s\n",root->value);
    // 再访问左子树
    if(root->left != NULL)
        Tree_preOrder_traverse(root->left);
    // 再访问右子树
    if (root->right != NULL)
        Tree_preOrder_traverse(root->right);
    return true;
}

bool Tree_lastOrder_traverse(Tree_Node_ptr root){
    // 二叉树的先序遍历
    // 先访问左子树
    if(root->left != NULL)
        Tree_lastOrder_traverse(root->left);
    // 再访问右子树
    if (root->right != NULL)
        Tree_lastOrder_traverse(root->right);
    // 再访问根节点
    printf("%s -> ",root->value);
    return true;
}

bool Tree_insert_AVL(Tree_Node_ptr &T, ElemType head, ElemType e, bool &taller){
    /*
     若在平衡的二叉排序树T中不存在和 e 有相同关键字的节点
     则插入一个数据元素为 e 的新节点，并返回 1 ，否则返回 0
     若因插入而使二叉排序树失去平衡
     则作平衡旋转处理，布尔变量 taller 反应 T 长高与否
     */
    if (!T){
        T = (Tree_Node_ptr)malloc(sizeof(Tree_Node));
        T->value = e;
        T->head = head;
        T->right = T->left = NULL;
        T->bf = EH; taller = true;
    }
    else {
        if (strcmp((char*)e,(char*)T->value) == 0) {
            taller = false;
            return 0;
        }
        if (strcmp((char*)e,(char*)T->value) < 0) {
            if (!Tree_insert_AVL(T->left, head, e, taller))
                return 0; // 未插入
            if (taller)
                switch (T->bf) {
                    case LH:
                        LeftBalance(T);
                        taller = false;
                        break;
                    case EH:
                        T->bf = LH;
                        taller = true;
                        break;
                    case RH:
                        T->bf = EH;
                        taller = false;
                        break;
                }
        }
        else {
            if (!Tree_insert_AVL(T->right, head, e, taller))
                return 0;
            if (taller) {
                switch (T->bf) {
                    case LH:
                        T->bf = RH;
                        taller = false;
                        break;
                    case EH:
                        T->bf = RH;
                        taller = true;
                        break;
                    case RH:
                        RightBalance(T);
                        taller = false;
                        break;
                }
            }
        }
    }
    return 1;
}
void LeftBalance(Tree_Node_ptr &T){
    /*
     对以指针 T 所指结点为根的二叉树作作平衡旋转处理
     结束时指针 T 指向新的根节点
     */
    Tree_Node_ptr lc = T->left;
    switch (lc->bf) {
        case LH:
            T->bf = lc->bf = EH;
            R_Rotate(T);
            break;
        case RH:
            Tree_Node_ptr rd = lc->right;
            switch (rd->bf) {
                case LH:
                    T->bf = RH;
                    lc->bf = EH;
                    break;
                case EH:
                    T->bf = lc->bf = EH;
                    break;
                case RH:
                    T->bf = EH;
                    lc->bf = LH;
                    break;
            }
            rd->bf = EH;
            L_Rotate(T->left);
            R_Rotate(T);
    }
}
void RightBalance(Tree_Node_ptr &T){
    /*
    对以指针 T 所指结点为根的二叉树作作平衡旋转处理
    结束时指针 T 指向新的根节点
    */
    Tree_Node_ptr rd = T->right;
    switch (rd->bf) {
        case RH:
            T->bf = rd->bf = EH;
            L_Rotate(T);
            break;
        case LH:
            Tree_Node_ptr lc = rd->left;
            switch (lc->bf) {
                case LH:
                    T->bf = EH;
                    rd->bf = RH;
                    break;
                case EH:
                    T->bf = rd->bf = EH;
                    break;
                case RH:
                    T->bf = LH;
                    rd->bf = EH;
                    break;
            }
            lc->bf = EH;
            R_Rotate(T->right);
            L_Rotate(T);
    }
}
