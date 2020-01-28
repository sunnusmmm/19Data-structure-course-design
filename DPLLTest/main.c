#include <mem.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include "DPLLTest.h"
#include "CNF.h"
#include "Sudoku.h"

int main() {
    CNFSet *CNF = NULL;
    Sudo *sudo;
    double start, end, start2, end2;
    int res, res2;
    st = malloc(sizeof(ChangeStack));
    st->floor = -1;
    st->next = NULL;
    int op = 1;
    while (op) {
        system("cls");
        printf("\n\n");
        printf("      Menu for Linear Table On Sequence Structure \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. 读取CNF文件\n");
        printf("    	  2. 求解CNF文件与生成输出文件并保存\n");
        printf("    	  3. 打印CNF文件\n");
        printf("    	  4. 验证结果(请在该CNF文件进行过求解后使用该操作！)\n");
        printf("    	  5. 数独游戏\n");
        printf("    	  6. 销毁当前CNF\n");
        printf("    	  7. 增加子句\n");
        printf("    	  8. 删除子句\n");
        printf("    	  9. 增加变元\n");
        printf("    	  10. 删除变元\n");
        printf("    	  0. exit\n");
        printf("-------------------------------------------------\n");
        printf("Please choose your number: [0~10]:");
        scanf("%d", &op);
        switch (op) {
            case 1:
                if (ReadFile(&CNF) == OK)
                    printf("读取文件成功\n");
                system("pause");
                break;
            case 2:
                if (!CNF) {
                    printf("请先读取或创建一个CNF文件\n");
                    system("pause");
                    break;
                }
                start = clock();
                res = DPLL(&CNF, 1);
                end = clock();
//                start2 = clock();
//                res2 = DPLL2(&CNF, 1);
//                end2 = clock();
                int tag = 0;
                double duration = (end - start);
//                double duration2 = (end2 - start2) / CLK_TCK;
                CNF->t = duration;
                if (res == OK) {
                    printf("算例满足--求解成功:\n");
                    for (int j = 1; j <= CNF->Enumber; ++j) {
                        if (CNF->ElemSet[j] == -1) {
                            tag = 1;
                            res = -1;
                            break;
                        }
                    }
                    if (tag == 0) {
                        for (int i = 1; i <= CNF->Enumber; ++i) {
                            if (CNF->ElemSet[i] == 0) printf("%d ", -i);
                            else if (CNF->ElemSet[i] == 1) printf("%d ", i);
                        }
                    } else {
                        printf("解不唯一");
                    }
                    printf("\n");
                    printf("DPLL: %f\n", duration);
//                    printf("DPLL2: %f\n", duration2);
//                    printf("优化率：%f %%\n", (duration2-duration)/duration2*100);
                    printf("\n");
                } else {
                    printf("算例不满足\n");
                    printf("DPLL: %f\n", duration);
//                    printf("DPLL2: %f\n", duration2);
                }
                int a;
                printf("是否保存求解结果到文件中（1/0):");
                scanf("%d", &a);
                if (a == 1) {
                    if (WriteFile(*CNF, res) == OK)
                        printf("保存结果文件成功\n");
                    else
                        printf("保存结果文件失败\n");
                }
                system("pause");
                break;
            case 3:
                if (!CNF) {
                    printf("请先读取或创建一个CNF文件\n");
                    system("pause");
                    break;
                }
                if (PrintCNF(*CNF) == OK) {
                    printf("打印成功！\n");
                }
                system("pause");
                break;
            case 4: {
                if (res != 0) {
                    if (Verify(CNF, res) == 1)
                        printf("检验答案正确！\n");
                    else
                        printf("答案错误！\n");
                } else printf("该文件无解，无法验证！\n");
                system("pause");
                break;
            }
            case 18: {
                printf("(求解文件)请输入你要验证的CNF文件-");
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
                                if (f[-e->elem] == 0) printf("%d(1) ", e->elem);
                                else if (f[-e->elem] == 1) printf("%d(0) ", e->elem);
                                else printf("%d(-1) ", e->elem);
                            } else {
                                if (f[e->elem] == 0) printf("%d(0) ", e->elem);
                                else if (f[e->elem] == 1) printf("%d(1) ", e->elem);
                                else printf("%d(-1) ", e->elem);
                            }
                            e = e->next;
                        }
                        printf("\n");
                        p = p->next;
                    }
                }
                system("pause");
                break;
            }
            case 17:
                if (InitSudo(&sudo) == OK) {
                    if (ReadSudoFile(sudo) == OK) {
                        PrintSudo(*sudo);
                        CreatSudoFile(*sudo);
                    }
                }
                system("pause");
                break;
            case 19:
                if (TransformSudo(&sudo) == OK) {
                    PrintSudo(*sudo);
                    printf("答案打印成功");
                }
                system("pause");
                break;
            case 5:
                if (CreatSudo(&sudo, 11) == OK) {
                    printf("正在生成数独...请稍后！\n");
                    if (generateByDigMethod(&CNF, sudo, 55) == OK) {
                        printf("请输入你的答案：(输入10放弃作答、0打印答案)\n");
                        int ans[9][9];
                        for (int x = 0; x < 9; ++x) {
                            for (int y = 0; y < 9; ++y) {
                                scanf("%d", &ans[x][y]);
                                if (ans[x][y] == 10) goto s;
                                else if (ans[x][y] == 0) {
                                    printf("答案：\n");
                                    PrintSudo(*sudo);
                                    goto t;
                                }
                            }
                        }
                        for (int x = 0; x < 9; ++x) {
                            for (int y = 0; y < 9; ++y) {
                                if (ans[x][y] != sudo->sudo[x][y]) {
                                    goto f;
                                }
                            }
                        }
                        goto m;
                        f:
                        printf("答案错误！\n");
                        s:
                        printf("是否打印答案：(1/0):");
                        int m;
                        scanf("%d", &m);
                        if (m == 1) PrintSudo(*sudo);
                        system("pause");
                        break;
                        m:
                        printf("答案正确！\n");
                        system("pause");
                        break;
                        t:;
                    }
//                    PrintSudo(*sudo);
//                    printf("打印成功");
                }
                system("pause");
                break;
            case 6:
                if (CNF) {
                    printf("你要销毁该CNF：(1/0)");
                    int l;
                    scanf("%d", &l);
                    if (l == 1) {
                        if (DestroyCNF(&CNF) == OK)
                            printf("销毁成功！\n");
                    } else {
                        printf("操作取消！\n");
                    }
                } else {
                    printf("请先读取或创建一个CNF文件\n");
                }
                system("pause");
                break;
            case 7:
                printf("输入你要增加的子句中的变元个数：");
                int Q;
                scanf("%d", &Q);
                printf("输入你要添加的变元值（-%d~%d)：",CNF->Enumber,CNF->Enumber);
                int* M;
                M = malloc(sizeof(int)*Q);
                for (int i = 0; i < Q; i++) {
                    scanf("%d", &M[i]);
                }
                if (AddClause(&CNF, Q, M) == OK) {
                    printf("添加成功！\n");
                } else{
                    printf("添加失败！\n");
                }
                system("pause");
                break;
            case 8:
                printf("输入你要删除的子句编号（倒序）(1-%d)：",CNF->Cnumber);
                int W;
                scanf("%d",&W);
                Clause *p=CNF->root;
                for (int i = 1; i < W; ++i) {
                    p = p->next;
                }
                if(DeleteClause(CNF,p) == OK)printf("删除成功！\n");
                else printf("删除失败！\n");
                system("pause");
                break;
            case 9:
                printf("输入你要增加变元的子句编号（倒序）(1-%d)：",CNF->Cnumber);
                int C;
                scanf("%d",&C);
                printf("输入你要增加变元的值（-%d~%d)：",CNF->Enumber,CNF->Enumber);
                int E;
                scanf("%d",&E);
                Clause *q=CNF->root;
                for (int i = 1; i < C; ++i) {
                    q = q->next;
                }
                if(AddElement(&q,E) == OK) printf("添加变元成功！\n");
                else printf("添加变元失败！\n");
                system("pause");
                break;
            case 10:
                printf("输入你要删除变元的子句编号（倒序）(1-%d)：",CNF->Cnumber);
                int R;
                scanf("%d",&R);
                printf("输入你要删除变元的值（-%d~%d)：",CNF->Enumber,CNF->Enumber);
                int T;
                scanf("%d",&T);
                Clause *G=CNF->root;
                for (int i = 1; i < R; ++i) {
                    G = G->next;
                }
                if(DeleteElement(G,T) == OK)printf("删除变元成功！\n");
                else printf("删除变元失败！\n");
                system("pause");
                break;
            case 0:
                break;
            default:
                printf("Please select the right action number you need!\n");
                system("pause");
                break;
        }
    }
}