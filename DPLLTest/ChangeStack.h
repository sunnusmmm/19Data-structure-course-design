//
// Created by Administrator on 2019/2/27.
//

#ifndef DPLLTEST_CHANGESTACK_H
#define DPLLTEST_CHANGESTACK_H

#include "CNF.h"

typedef struct ChangeStack {
    int tag;      //操作标识
    int floor;    //回溯层数
    int elem;     //变元
    Clause *C;    //操作字句
    struct ChangeStack *next;
} ChangeStack;

Status SaveChanges(ChangeStack *stack, int tag, int floor, Clause *C, Element *elem);//保存简化时修改 进栈
Status RestoreChanges(CNFSet **CNF,int floor, ChangeStack *stack);  //恢复简化时修改 出栈

#endif //DPLLTEST_CHANGESTACK_H