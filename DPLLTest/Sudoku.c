//
// Created by Administrator on 2019/2/23.
//
#include <stdio.h>
#include <mem.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <stdbool.h>
#include "Sudoku.h"

Status InitSudo(Sudo **sudo) {
    (*sudo) = malloc(sizeof(Sudo));
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            (*sudo)->sudo[x][y] = 0;
        }
    }
    return OK;
}

Status CreatSudo(Sudo **sudo, int n) {
    if (InitSudo(sudo) != OK) return ERROR;
    int nums[9][9];
    for (int l = 0; l < 9; ++l) {
        for (int i = 0; i < 9; ++i) {
            nums[l][i] = 0;
        }
    }
    if (ReadSudoFile2(*sudo) == OK) { //读取一个完备数独
        //将每个数字变换为其他字符
        int h = 0, k[9];
        for (int j = 0; j < 9; ++j) {
            k[j] = 0;
        }
        for (int i = 1; i < 10; ++i) {
            srand((unsigned int) time(NULL));
            while (h == 0) {
                h = rand() % 10;
                if (h != 0 && k[h - 1] == 1)
                    h = 0;
            }
            for (int x = 0; x < 9; ++x) {
                for (int y = 0; y < 9; ++y) {
                    if ((*sudo)->sudo[x][y] == i && nums[x][y] != 1) {
                        (*sudo)->sudo[x][y] = h;
                        nums[x][y] = 1;
                    }
                }
            }
            k[h - 1] = 1;
            h = 0;
        }
    }
    return OK;
}

Status generateByDigMethod(CNFSet **CNF, Sudo *sudo, int number) {
    Sudo *sudo1 = (Sudo *) malloc(sizeof(Sudo));
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            sudo1->sudo[row][col] = sudo->sudo[row][col];
        }
    }
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                int i = 1;
                for (; i <= 9; ++i) {
                    if (i != sudo->sudo[row][col]) {
                        sudo1->sudo[row][col] = i;
                        //先判断是否满足数独基本要求
                        if (!Judge_sudo(sudo1, row, col))
                            continue;
                        CreatSudoFile(*sudo1); //数独生成CNF文件
                        ReadFile2(&(*CNF));    //读取生成的CNF文件
                        if (DPLL(&(*CNF), 1) == OK) { //DPLL求解其他情况是否有解
                            break;
                        }
                    }
                }
                if (i == 10) {
                    sudo1->sudo[row][col] = 0;
                } else
                    sudo1->sudo[row][col] = sudo->sudo[row][col];
            }
    }
    printf("数独游戏：\n");
    t:PrintSudo(*sudo1);
    return OK;
}


Status Judge_sudo(Sudo *sudo, int row, int col) {
    for (int j = 0; j < 9; ++j) {
        if (sudo->sudo[row][col] == sudo->sudo[row][j] && j != col)
            return false;
    }
    for (int i = 0; i < 9; ++i) {
        if (sudo->sudo[row][col] == sudo->sudo[i][col] && i != row)
            return false;
    }
    for (int i = (row / 3) * 3; i < (row / 3) * 3 + 3; ++i) {
        for (int j = (col / 3) * 3; j < (col / 3) * 3 + 3; ++j) {
            if (sudo->sudo[row][col] == sudo->sudo[i][j] && (i != row || j != col))
                return false;
        }
    }
    return true;
}

Status ReadSudoFile(Sudo *sudo) {
    char filename[30];
    int m;
    char cnf[5] = ".txt";
    printf("你要读取的数独文件: ");
    scanf("%s", filename);
//    memcpy(SudoFile, filename, 30);
    strncat(filename, cnf, 4);
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) {
        printf("文件打开失败123\n ");
        return FALSE;
    }
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            fscanf(fp, "%d", &m);
            (sudo)->sudo[x][y] = m;
        }
    }
    fclose(fp);
    return OK;
}

Status CreatSudoFile(Sudo sudo) {
    char filename[15] = "SudoFile";
    int clause, hole = 0;
    char cnf[5] = ".cnf";
    strncat(filename, cnf, 4);
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL) {
        printf("文件打开失败123\n");
        return FALSE;
    }
    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; ++y) {
            if (sudo.sudo[x][y] != 0)
                hole++;
        }
    }
    clause = 8829 + hole;
    fputs("p cnf 729 ", fp);
    fprintf(fp, "%d\n", clause);
    for (int x = 0; x < 9; ++x) { //创建已经存在的变元单子句
        for (int y = 0; y < 9; ++y)
            if (sudo.sudo[x][y] != 0) {
                fprintf(fp, "%d 0\n", x * 81 + y * 9 + sudo.sudo[x][y]);
            }
    }
    for (int x = 1; x < 10; ++x) { //每一格必须出现一个数
        for (int y = 1; y < 10; ++y) {
            for (int z = 1; z < 10; ++z) {
                fprintf(fp, "%d ", (x - 1) * 81 + (y - 1) * 9 + z);
            }
            fprintf(fp, "0\n");
        }
    }
    for (int y = 1; y <= 9; ++y) { //每一行必须出现一个数
        for (int z = 1; z <= 9; ++z)
            for (int x = 1; x <= 8; ++x) {
                for (int i = x + 1; i < 10; ++i) {
                    fprintf(fp, "%d ", 0 - ((y - 1) * 81 + (x - 1) * 9 + z));
                    fprintf(fp, "%d ", 0 - ((y - 1) * 81 + (i - 1) * 9 + z));
                    fprintf(fp, "0\n");
                }
            }
    }
    for (int x = 1; x <= 9; ++x) { //每一列必须出现一个数
        for (int z = 1; z <= 9; ++z)
            for (int y = 1; y <= 8; ++y) {
                for (int i = y + 1; i <= 9; ++i) {
                    fprintf(fp, "%d ", 0 - ((y - 1) * 81 + (x - 1) * 9 + z));
                    fprintf(fp, "%d ", 0 - ((i - 1) * 81 + (x - 1) * 9 + z));
                    fprintf(fp, "0\n");
                }
            }
    }
    //3*3 sub-grids
    for (int z = 1; z < 10; ++z) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int x = 1; x < 4; ++x)
                    for (int y = 1; y < 3; ++y) {
                        for (int k = y + 1; k < 4; ++k) {
                            fprintf(fp, "%d ", 0 - ((3 * i + x - 1) * 81 + (3 * j + y - 1) * 9 + z));
                            fprintf(fp, "%d ", 0 - ((3 * i + x - 1) * 81 + (3 * j + k - 1) * 9 + z));
                            fprintf(fp, "0\n");
                        }
                    }
    }
    for (int z = 1; z <= 9; z++) {
        for (int i = 0; i <= 2; i++)
            for (int j = 0; j <= 2; j++)
                for (int x = 1; x <= 2; x++)
                    for (int y = 1; y <= 3; y++)
                        for (int k = x + 1; k <= 3; k++) {
                            for (int l = 1; l <= 3; l++) {
                                fprintf(fp, "%d ", 0 - ((3 * i + x - 1) * 81 + (3 * j + y - 1) * 9 + z));
                                fprintf(fp, "%d ", 0 - ((3 * i + k - 1) * 81 + (3 * j + l - 1) * 9 + z));
                                fprintf(fp, "0\n");
                            }
                        }
    }
    fclose(fp);
    return OK;
}

Status TransformSudo(Sudo **sudo) {
    (*sudo) = malloc(sizeof(Sudo));
    if (!(*sudo)) return ERROR;
    char filename[15] = "SudoFile";
    char cnf[5] = ".res";
    int h, k = 1;
//    memcpy(filename, SudoFile, 30);
//    printf("输入需要读取的数独CNF文件: ");
//    scanf("%s", filename);
    strncat(filename, cnf, 4);
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) {
        printf("文件打开失败123\n ");
        return FALSE;
    }
    while (fgetc(fp) == 's') {
        while (fgetc(fp) != '\n') {
        }
    }
    for (int i = 0; i < 1; i++) {
        getc(fp);
    }
    while (fscanf(fp, "%d", &h) != EOF) {
        if (k > 729) break;
        if (h == 36) {}
        if (h > 0) {
            int x = h / 81;
            h = h % 81;
            int y = h / 9;
            h = h % 9;
            int z = h;
            if (z == 0) {
                y = y - 1;
                z = 9;
            }
            (*sudo)->sudo[x][y] = z;
        }
        k++;
    }
    fclose(fp);
    return OK;
}

Status PrintSudo(Sudo sudo) {
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            printf("%d ", sudo.sudo[x][y]);
            if (y == 2 || y == 5) {
                printf("|");
            }
        }
        if (x == 2 || x == 5) {
            printf("\n");
            for (int y = 0; y < 10; ++y) {
                printf("—");
            }
        }
        printf("\n");
    }
}

Status ReadFile2(CNFSet **CNF) {
    char filename[15] = "SudoFile";
    int i, j, Elem[50];
    char cnf[5] = ".cnf";
    int Cnumber, Enumber;
//    memcpy(FILENAME, filename, 30);
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

Status ReadSudoFile2(Sudo *sudo) {
    char filename[30] = "output";
    int m;
    char cnf[5] = ".cnf";
//    memcpy(SudoFile, filename, 30);
    strncat(filename, cnf, 4);
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) {
        printf("文件打开失败123\n ");
        return FALSE;
    }
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            fscanf(fp, "%d", &m);
            (sudo)->sudo[x][y] = m;
        }
    }
    fclose(fp);
    return OK;
}