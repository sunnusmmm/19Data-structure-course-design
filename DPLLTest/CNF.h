//
// Created by Administrator on 2019/2/21.
//

#ifndef DPLLTEST_CNF_H
#define DPLLTEST_CNF_H

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR -1
#define NOEXIT -1
#define ANTIEXIT 0

typedef int Status;

char FILENAME[30];

typedef struct Element {
    int elem;                //变元
    struct Element *next;    //下一变元
} Element, *pElement;

typedef struct Clause {
    int length;              //该子句变元的数量
    struct Element *First;   //第一个变元
    struct Clause *prev;     //上一个子句
    struct Clause *next;     //下一个子句
} Clause;

typedef struct CNFSet {
    double t;                //该CNF求解DPLL运行时间
    int Enumber;             //变元个数
    int Cnumber;             //子句个数
    int *ElemSet;            //变元数组，存放变元真假，0为假，1为真，2为未知
    struct Clause *root;     //字句集中第一个子句
} CNFSet;

Status CreateCNF(CNFSet **CNF, int E, int C);//CNF创建(E变元个数、C子句个数)
Status DestroyCNF(CNFSet **CNF);//CNF销毁
Status AddClause(CNFSet **CNF, int n, int *elem);//增加子句(该子句有n个变元）
Status AddElement(Clause **C, int elem);//增加变元
Status DeleteClause(CNFSet *CNF, Clause *C);//删除子句
Status DeleteElement(Clause *C, int elem);//删除变元
Status isUnitClause(Clause C);//判断是否为单子句
Status evaluateClause(Clause *C, int elem);//评估子句的真假状态
Status ReadFile(CNFSet **CNF);//读取文件
Status PrintCNF(CNFSet CNF);//打印CNF
Clause *HideClause(CNFSet *CNF, Clause *C);//隐藏子句
Element *HideElement(Clause *C, int elem);//隐藏变元
Status HaveEmptyClause(CNFSet CNF); //是否存在空字句
Status HaveClause(CNFSet CNF);//是否存在字句
Status WriteFile(CNFSet CNF, int result);//将求解结果按要求保存到相应文件当中

#endif //DPLLTEST_CNF_H