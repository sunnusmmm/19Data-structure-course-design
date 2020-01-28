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
        printf("    	  1. ��ȡCNF�ļ�\n");
        printf("    	  2. ���CNF�ļ�����������ļ�������\n");
        printf("    	  3. ��ӡCNF�ļ�\n");
        printf("    	  4. ��֤���(���ڸ�CNF�ļ����й�����ʹ�øò�����)\n");
        printf("    	  5. ������Ϸ\n");
        printf("    	  6. ���ٵ�ǰCNF\n");
        printf("    	  7. �����Ӿ�\n");
        printf("    	  8. ɾ���Ӿ�\n");
        printf("    	  9. ���ӱ�Ԫ\n");
        printf("    	  10. ɾ����Ԫ\n");
        printf("    	  0. exit\n");
        printf("-------------------------------------------------\n");
        printf("Please choose your number: [0~10]:");
        scanf("%d", &op);
        switch (op) {
            case 1:
                if (ReadFile(&CNF) == OK)
                    printf("��ȡ�ļ��ɹ�\n");
                system("pause");
                break;
            case 2:
                if (!CNF) {
                    printf("���ȶ�ȡ�򴴽�һ��CNF�ļ�\n");
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
                    printf("��������--���ɹ�:\n");
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
                        printf("�ⲻΨһ");
                    }
                    printf("\n");
                    printf("DPLL: %f\n", duration);
//                    printf("DPLL2: %f\n", duration2);
//                    printf("�Ż��ʣ�%f %%\n", (duration2-duration)/duration2*100);
                    printf("\n");
                } else {
                    printf("����������\n");
                    printf("DPLL: %f\n", duration);
//                    printf("DPLL2: %f\n", duration2);
                }
                int a;
                printf("�Ƿ񱣴���������ļ��У�1/0):");
                scanf("%d", &a);
                if (a == 1) {
                    if (WriteFile(*CNF, res) == OK)
                        printf("�������ļ��ɹ�\n");
                    else
                        printf("�������ļ�ʧ��\n");
                }
                system("pause");
                break;
            case 3:
                if (!CNF) {
                    printf("���ȶ�ȡ�򴴽�һ��CNF�ļ�\n");
                    system("pause");
                    break;
                }
                if (PrintCNF(*CNF) == OK) {
                    printf("��ӡ�ɹ���\n");
                }
                system("pause");
                break;
            case 4: {
                if (res != 0) {
                    if (Verify(CNF, res) == 1)
                        printf("�������ȷ��\n");
                    else
                        printf("�𰸴���\n");
                } else printf("���ļ��޽⣬�޷���֤��\n");
                system("pause");
                break;
            }
            case 18: {
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
                    printf("�𰸴�ӡ�ɹ�");
                }
                system("pause");
                break;
            case 5:
                if (CreatSudo(&sudo, 11) == OK) {
                    printf("������������...���Ժ�\n");
                    if (generateByDigMethod(&CNF, sudo, 55) == OK) {
                        printf("��������Ĵ𰸣�(����10��������0��ӡ��)\n");
                        int ans[9][9];
                        for (int x = 0; x < 9; ++x) {
                            for (int y = 0; y < 9; ++y) {
                                scanf("%d", &ans[x][y]);
                                if (ans[x][y] == 10) goto s;
                                else if (ans[x][y] == 0) {
                                    printf("�𰸣�\n");
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
                        printf("�𰸴���\n");
                        s:
                        printf("�Ƿ��ӡ�𰸣�(1/0):");
                        int m;
                        scanf("%d", &m);
                        if (m == 1) PrintSudo(*sudo);
                        system("pause");
                        break;
                        m:
                        printf("����ȷ��\n");
                        system("pause");
                        break;
                        t:;
                    }
//                    PrintSudo(*sudo);
//                    printf("��ӡ�ɹ�");
                }
                system("pause");
                break;
            case 6:
                if (CNF) {
                    printf("��Ҫ���ٸ�CNF��(1/0)");
                    int l;
                    scanf("%d", &l);
                    if (l == 1) {
                        if (DestroyCNF(&CNF) == OK)
                            printf("���ٳɹ���\n");
                    } else {
                        printf("����ȡ����\n");
                    }
                } else {
                    printf("���ȶ�ȡ�򴴽�һ��CNF�ļ�\n");
                }
                system("pause");
                break;
            case 7:
                printf("������Ҫ���ӵ��Ӿ��еı�Ԫ������");
                int Q;
                scanf("%d", &Q);
                printf("������Ҫ��ӵı�Ԫֵ��-%d~%d)��",CNF->Enumber,CNF->Enumber);
                int* M;
                M = malloc(sizeof(int)*Q);
                for (int i = 0; i < Q; i++) {
                    scanf("%d", &M[i]);
                }
                if (AddClause(&CNF, Q, M) == OK) {
                    printf("��ӳɹ���\n");
                } else{
                    printf("���ʧ�ܣ�\n");
                }
                system("pause");
                break;
            case 8:
                printf("������Ҫɾ�����Ӿ��ţ�����(1-%d)��",CNF->Cnumber);
                int W;
                scanf("%d",&W);
                Clause *p=CNF->root;
                for (int i = 1; i < W; ++i) {
                    p = p->next;
                }
                if(DeleteClause(CNF,p) == OK)printf("ɾ���ɹ���\n");
                else printf("ɾ��ʧ�ܣ�\n");
                system("pause");
                break;
            case 9:
                printf("������Ҫ���ӱ�Ԫ���Ӿ��ţ�����(1-%d)��",CNF->Cnumber);
                int C;
                scanf("%d",&C);
                printf("������Ҫ���ӱ�Ԫ��ֵ��-%d~%d)��",CNF->Enumber,CNF->Enumber);
                int E;
                scanf("%d",&E);
                Clause *q=CNF->root;
                for (int i = 1; i < C; ++i) {
                    q = q->next;
                }
                if(AddElement(&q,E) == OK) printf("��ӱ�Ԫ�ɹ���\n");
                else printf("��ӱ�Ԫʧ�ܣ�\n");
                system("pause");
                break;
            case 10:
                printf("������Ҫɾ����Ԫ���Ӿ��ţ�����(1-%d)��",CNF->Cnumber);
                int R;
                scanf("%d",&R);
                printf("������Ҫɾ����Ԫ��ֵ��-%d~%d)��",CNF->Enumber,CNF->Enumber);
                int T;
                scanf("%d",&T);
                Clause *G=CNF->root;
                for (int i = 1; i < R; ++i) {
                    G = G->next;
                }
                if(DeleteElement(G,T) == OK)printf("ɾ����Ԫ�ɹ���\n");
                else printf("ɾ����Ԫʧ�ܣ�\n");
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