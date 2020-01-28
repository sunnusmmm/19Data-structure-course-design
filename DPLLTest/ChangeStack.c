//
// Created by Administrator on 2019/2/27.
//
#include <stdlib.h>
#include <stdio.h>
#include "ChangeStack.h"

Status SaveChanges(ChangeStack *stack, int tag, int floor, Clause *C, Element *elem) {
    if (stack == NULL) {
        return FALSE;
    }
    ChangeStack *newChange = malloc(sizeof(ChangeStack));
    newChange->next = stack->next;
    stack->next = newChange;
    newChange->tag = tag;
    newChange->floor = floor;
    if (tag == 0) { //保存子句
        newChange->C = C;
    } else if (tag == 1) { //保存子句地址和变元
        newChange->C = C;
        newChange->elem = elem->elem;
    } else if (tag == 2) {//确定决策变元后添加的在首句的字句要删除
        newChange->C = C;
    }
//    else if (tag == 3) {//由化简而产生的单子句的赋值需要还原
//        newChange->elem = elem->elem;
//    }
    return OK;
}

Status RestoreChanges(CNFSet **CNF,int floor, ChangeStack *stack) {
    int i;
    ChangeStack *p = stack->next;
    while (p && p->floor == floor) {
        if (p->tag == 0) {
            p->C->next = (*CNF)->root;
            if ((*CNF)->root) (*CNF)->root->prev = p->C;
            (*CNF)->root = p->C;
            (*CNF)->Cnumber++;
        } else if (p->tag == 1) {
            AddElement(&(p->C), p->elem);
        } else if (p->tag == 2) {
            DeleteClause(*CNF, p->C);
        }
//        else if (p->tag == 3) {
//            (*CNF)->ElemSet[ABS(p->elem)] = -1;
//        }
        stack->next = p->next;
        free(p);
        p = stack->next;
    }
    return OK;
}