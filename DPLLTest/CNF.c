#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <mem.h>
#include "CNF.h"
#include "DPLLTest.h"

Status CreateCNF(CNFSet **CNF, int E, int C) {
    int i;
    (*CNF) = malloc(sizeof(CNFSet));
    (*CNF)->Cnumber = C;
    (*CNF)->Enumber = E;
    (*CNF)->root = NULL;
    (*CNF)->ElemSet = (int *) malloc(sizeof(int) * (E + 1)); //0位不储存变元赋值信息
    for (i = 1; i < E + 1; i++) {
        (*CNF)->ElemSet[i] = -1;
    }
    return OK;
}

Status DestroyCNF(CNFSet **CNF) {
    Clause *p;
    for (p = (*CNF)->root; p; p = (*CNF)->root) {
        (*CNF)->root = p->next;
        if (DeleteClause((*CNF), p) != OK) {
            return ERROR;
        }
    }
    free((*CNF)->ElemSet);
    free(CNF);
    (*CNF) = NULL;
    return OK;
}

Status AddClause(CNFSet **CNF, int n, int *elem) {
    if (!CNF) return ERROR;
    int i;
    Clause *clause = malloc(sizeof(Clause));
    clause->length = 0;
    clause->next = (*CNF)->root;
    if ((*CNF)->root) (*CNF)->root->prev = clause;//root存在时
    (*CNF)->root = clause;
    (*CNF)->Cnumber++;
    clause->prev = NULL;
    clause->First = NULL;
    for (i = 0; i < n; i++) AddElement(&clause, elem[i]);
    return OK;
}

Status AddElement(Clause **C, int elem) {
    if (!(*C))return ERROR;
    Element *p = malloc(sizeof(Element));
    if (!p)return ERROR;//分配空间失败
    p->elem = elem;
    p->next = (*C)->First;
    (*C)->First = p;
    (*C)->length++;
    return OK;
}

Status DeleteClause(CNFSet *CNF, Clause *C) {
    Element *lp;
    if (C->prev == NULL) {
        CNF->root = C->next;
        if (C->next != NULL) C->next->prev = C->prev;
    } else{
        C->prev->next = C->next;
        if (C->next != NULL) C->next->prev = C->prev;
    }
    for (lp = C->First; lp; lp = C->First) {
        C->First = lp->next;
        free(lp);
    }
    free(C);
    CNF->Cnumber--;
    return OK;
}

Status DeleteElement(Clause *C, int elem) {
    if (!(C) || !C->First) return ERROR;
    Element *p = (C)->First, *q;
    if (p->elem == elem) {
        (C)->First = p->next;
        free(p);
        C->length--;
        return OK;
    } else {
        for (q = p->next; q; p = q, q = q->next) {
            if (q->elem == elem) {
                p->next = q->next;
                free(q);
                C->length--;
                return OK;
            }
        }
    }
    return ERROR;
}

Status isUnitClause(Clause C) {
    if (C.length == 1) return true;
    else return false;
}

Status evaluateClause(Clause *C, int elem) { //可能不包含该变元，或包含该变元负文字
    int Flag = NOEXIT;
    Element *p;
    for (p = C->First; p; p = p->next) {
        if (p->elem == elem) {
            Flag = TRUE;
        } else if (p->elem == -elem) {
            Flag = ANTIEXIT;
        }
    }
    return Flag;
}

Status ReadFile(CNFSet **CNF) {
    char filename[30];
    int i, j, Elem[60];
    char cnf[5] = ".cnf";
    int Cnumber, Enumber;
    printf("输入需要读取的CNF文件: ");
    scanf("%s", filename);
    memcpy(FILENAME, filename, 30);
    strncat(filename, cnf, 4);
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) {
        printf("文件打开失败123\n ");
        return FALSE;
    }
    while (fgetc(fp) == 'c') {
        while (fgetc(fp) != '\n') {
        }
    }

    for (i = 0; i < 5; i++) {
        getc(fp);
    }
    fscanf(fp, "%d", &Enumber);
    fscanf(fp, "%d", &Cnumber);
    if (CreateCNF(&(*CNF), Enumber, 0) != OK) return ERROR;

    for (j = 0; j < Cnumber; j++) {
        for (i = 0;; i++) {
            fscanf(fp, "%d", &Elem[i]);
            if (Elem[i] == 0) break;
        }
        if (AddClause(&(*CNF), i, Elem) != OK) {
            fclose(fp);
            return ERROR;
        }
    }
    fclose(fp);
    return OK;
}

Status PrintCNF(CNFSet CNF) {
    if (CNF.Cnumber == 0 || CNF.ElemSet == 0) return false;
    Clause *p = CNF.root;
    Element *e;
    while (p != NULL) {
        e = p->First;
        while (e != NULL) {
            printf("%d ", e->elem);
            e = e->next;
        }
        printf("\n");
        p = p->next;
    }
    return true;
}

Clause *HideClause(CNFSet *CNF, Clause *C) {
    Clause *p = C->prev;
    if (C->next) {
        C->next->prev = p;
    }
    if (p) {
        p->next = C->next;
    } else {
        CNF->root = C->next;
    }
    CNF->Cnumber--;
    return C;
}

Element *HideElement(Clause *C, int elem) { //在该部分隐藏时，需要将该子句和被隐藏变元联系起来
    Element *p, *q = NULL;
    p = C->First;
    if (p->elem == -elem) {
        C->First = p->next;
        C->length--;
        return p;
    } else {
        for (q = p->next; q; q = q->next) {
            if (q->elem == -elem) {
                p->next = q->next;
                C->length--;
                return q;
            }
            p = q;
        }
    }
    return NULL;
}

Status HaveEmptyClause(CNFSet CNF) {
    Clause *p;
    for (p = CNF.root; p; p = p->next) {
        if (p->length == 0) return true;
    }
    return false;
}

Status HaveClause(CNFSet CNF) {
    if (CNF.Cnumber != 0) return true;
    else return false;
}

Status WriteFile(CNFSet CNF, int result) {
    char *filename;
    int i, j, tag = 0;
    char cnf[5] = ".res";
    int Enumber;
    filename = FILENAME;
    strncat(filename, cnf, 4);
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL) {
        printf("文件打开失败123\n");
        return FALSE;
    }
    Enumber = CNF.Enumber;
    if (result == 1) {
        tag = 1;
        for (i = 1; i < Enumber; i++) {
            if (CNF.ElemSet[i] < 0) tag = -1;
        }
    }
    fputs("s ", fp);
    fprintf(fp, "%d", result);
    fputs("\n", fp);
    if (result == 1) {
        fputs("v ", fp);
        for (j = 1; j <= Enumber; ++j) {
            if (CNF.ElemSet[j] == 0) fprintf(fp, "%d", -j);
            else fprintf(fp, "%d", j);
            fputs(" ", fp);
        }
    }
    fputs("\n", fp);
    fputs("t ", fp);
    fprintf(fp, "%.3lf", CNF.t);
    fclose(fp);
    return OK;
}