//
// Created by Administrator on 2019/2/19.
//
#ifndef DPLLTEST_DPLLTEST_H
#define DPLLTEST_DPLLTEST_H

#include "CNF.h"
#include "ChangeStack.h"

ChangeStack *st;
int A[1000];

Status DPLL(CNFSet **CNF, int floor);             //递归
Clause * SingleClause(CNFSet *CNF);               //找到第一个单子句
Status ChooseElem(CNFSet *CNF);                   //选择最短子句变元假设赋值
Status SimpleSingleClause(CNFSet *CNF, int floor, int elem, ChangeStack *stack);//简化单子句
Status AddDecision(CNFSet** CNF, int flool, int elem); //将决策变量化为单子句添加进CNF并保存修改
Status Verify(CNFSet *CNF, int res);               //验证求解结果

#endif //DPLLTEST_DPLLTEST_H