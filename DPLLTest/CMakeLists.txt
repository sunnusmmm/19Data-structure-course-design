cmake_minimum_required(VERSION 3.12)
project(DPLLTest C)

set(CMAKE_C_STANDARD 99)

add_executable(DPLLTest main.c DPLLTest.h CNF.c CNF.h DPLLTest.c Sudoku.h Sudoku.c ChangeStack.h ChangeStack.c)