#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STATE 500
#define MAX_EDGE  2000
#define MAX_STACK 500
#define MAX_RE    500

/* ---------- NFA 相关结构 ---------- */

typedef struct {
    int to;
    char symbol;          // 普通字符，或者 '#' 表示 epsilon
} Edge;

typedef struct {
    Edge edges[MAX_EDGE];
    int  edgeCount;
} State;

typedef struct {
    char  re[MAX_RE];     // 原始正则
    char  re_[MAX_RE];    // 插入连接符后的正则
    char  pe[MAX_RE];     // 后缀表达式

    State graph[MAX_STATE];
    int   stateNum;

    int startState;
    int endState;

    char charSet[128];    // 字符集
    int  charCount;
} NFA;

/* ---------- NFA 函数原型（在你原来的 main.cpp 里已实现） ---------- */

int  is_operator(char c);
int  priority(char c);
void add_edge(NFA* nfa, int from, int to, char symbol);
int  new_state(NFA* nfa);

void insert_contact(NFA* nfa);
void re2pe(NFA* nfa);
void pe2nfa(NFA* nfa);

void closure(NFA* nfa, int state, int* visited);
int  validate(NFA* nfa, const char* input);

void print_nfa(NFA* nfa);
void save_dot(NFA* nfa, const char* filename);

#endif  // MAIN_H
