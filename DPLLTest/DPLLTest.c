//
// Created by Administrator on 2019/2/21.
//
#include <stddef.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include "DPLLTest.h"


Status DPLL(CNFSet **CNF, int floor) {
    int elem, i, x = 0;
    Clause *p;
    while ((p = SingleClause(*CNF)) != NULL) {//���Ӿ䴫��
        if (p->First->elem < 0) (*CNF)->ElemSet[-p->First->elem] = 0;
        else (*CNF)->ElemSet[p->First->elem] = 1;
        A[x++] = p->First->elem;
        SimpleSingleClause((*CNF), floor, p->First->elem, st);//�����Ӿ�
//        PrintCNF((**CNF));
        if (HaveEmptyClause((**CNF)) == TRUE) {//����п��Ӿ�
            if (RestoreChanges(&(*CNF), floor, st) == OK) {
                for (i = 0; i < x; i++) {
                    if(A[i] < 0) (*CNF)->ElemSet[-A[i]] = -1;
                    else (*CNF)->ElemSet[A[i]] = -1;
                }
            }
            return FALSE;
        } else if (HaveClause((**CNF)) == FALSE) {//���û���Ӿ�
            RestoreChanges(&(*CNF), floor, st);
            return TRUE;
        }
    }//while

//    elem = ChooseElem((*CNF));//�ۺϲ���ѡ����Ԫ
    elem = (*CNF)->root->First->elem;

    AddDecision(&(*CNF), floor, elem);
    int s = DPLL(&(*CNF), floor + 1);//�ݹ���һ��
    if (s == TRUE) {//�������
        RestoreChanges(&(*CNF), floor, st);
        return TRUE;
    }
    AddDecision(&(*CNF), floor, -elem);
    s = DPLL(&(*CNF), floor + 1);//�ݹ�
    RestoreChanges(&(*CNF), floor, st);
    return s;
}

Clause *SingleClause(CNFSet *CNF) {
    Clause *p = CNF->root;
    while (p != NULL) {
        if (isUnitClause(*p) == true)
            return p;
        p = p->next;
    }
    return NULL;
}

Status SimpleSingleClause(CNFSet *CNF, int floor, int elem, ChangeStack *stack) {
    Element *e;
    int flag;
    Clause *p = CNF->root;
    while (p) {
        flag = evaluateClause(p, elem);
        if (flag == TRUE) {
            HideClause(CNF, p);
            SaveChanges(stack, 0, floor, p, NULL);
        } else if (flag == ANTIEXIT) {
            e = HideElement(p, elem);
            SaveChanges(stack, 1, floor, p, e);
        }
        p = p->next;
    }
    return OK;
}

Status ChooseElem(CNFSet *CNF) {
    if (!CNF)
        return ERROR;
    int flag;
    int positive = 100;
    Clause *p;
    Element *e, *res = (CNF)->root->First;
    for (p = (CNF)->root; p; p = p->next) {
        flag = 1;
        for (e = p->First; e; e = e->next) {
            if (e->elem < 0) flag = 0;
        }
        if (flag == 1 && p->length < positive) {
            res = p->First;
            positive = p->length;
        }
    }
    return res->elem;
}

Status AddDecision(CNFSet **CNF, int floor, int elem) {
    if (elem < 0) (*CNF)->ElemSet[-elem] = 0;
    else (*CNF)->ElemSet[elem] = 1;
    if (elem > 0) (*CNF)->ElemSet[elem] = 1;
    else if (elem < 0) (*CNF)->ElemSet[-elem] = 0;
    AddClause(&(*CNF), 1, &elem);//��ӵ��Ӿ伯��
    SaveChanges(st, 2, floor + 1, (*CNF)->root, NULL); //����ӵ��Ӿ���޸Ĳ���������һ����޸�ջ��
    return OK;
}

Status Verify(CNFSet *CNF, int res) {
    printf("(����ļ�)��������Ҫ��֤��CNF�ļ�-");
    int f[CNF->Enumber + 1];
    for (int k = 1; k < CNF->Enumber + 1; ++k) {
        f[k] = CNF->ElemSet[k];
    }
    if (ReadFile(&CNF) == OK) {
        Clause *p = CNF->root;
        Element *e;
        while (p != NULL) {
            int l[p->length];
            e = p->First;
            int i = 0;
            while (e != NULL) {
                if (e->elem < 0) {
                    if (f[-e->elem] == 0) l[i] = 1;
                    else if (f[-e->elem] == 1) l[i] = 0;
                    else l[i] = 1;
                } else if (e->elem > 0) {
                    if (f[e->elem] == 0) l[i] = 0;
                    else l[i] = 1;
                }
                i++;
                e = e->next;
            }
            int mm = 0;
            for (int j = 0; j < p->length; ++j) {
                if (l[j] == 1) {
                    mm = 1;
                    break;
                }
            }
            if (mm == 0) {
                if (res != OK && res != -1)return TRUE;
                else return FALSE;
            }
            p = p->next;
        }
    }
    if (res == OK || res == -1) return TRUE;
    else return FALSE;
}