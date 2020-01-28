//
// Created by Administrator on 2019/2/23.
//

#ifndef DPLLTEST_SUDOKU_H
#define DPLLTEST_SUDOKU_H

#include "DPLLTest.h"

char SudoFile[30];

typedef struct Sudo {
    int sudo[9][9];
} Sudo;

Status InitSudo(Sudo **sudo); //初始化数独数组

Status CreatSudo(Sudo **sudo, int n); //生成完整数独

Status generateByDigMethod(CNFSet **CNF,Sudo *sudo ,int number); //挖洞法生产数独问题

Status Judge_sudo(Sudo *sudo,int r,int c);//判断该洞该不该挖

Status ReadSudoFile(Sudo *sudo); //读取数独文件
Status ReadSudoFile2(Sudo *sudo); //读取数独文件

Status CreatSudoFile(Sudo sudo); //数独转化为CNF文件求解

Status TransformSudo(Sudo **sudo); //数独CNF文件解文件转换为数独

Status PrintSudo(Sudo sudo); //打印数独

Status ReadFile2(CNFSet **CNF);//读取文件

#endif //DPLLTEST_SUDOKU_H