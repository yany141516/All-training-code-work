#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"      
#include "nfa.h" 
#include "main.h"     // 你的 NFA 结构所在文件

int main() {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    memset(nfa, 0, sizeof(NFA));

    printf("输入正则表达式: ");
    scanf("%s", nfa->re);

    insert_contact(nfa);
    re2pe(nfa);
    pe2nfa(nfa);

    print_nfa(nfa);
    save_dot(nfa, "nfa.dot");



    // 1. 在堆上分配 DFA
    DFA* dfa = (DFA*)malloc(sizeof(DFA));

    // 2. 初始化内存 (很重要)
    memset(dfa, 0, sizeof(DFA));

    // 3. 传递指针 (dfa 本身就是指针，不用取地址 &)
    nfa_to_dfa(nfa, dfa);

    // 4. 传递指针
    save_dfa_dot(dfa, "dfa.dot");

    printf("\n已生成 nfa.dot 以及 dfa.dot，可用于 Graphviz 可视化。\n");

    /* ============= 保留原来的字符串验证功能 ============= */
    char s[200];
    while (1) {
        printf("输入测试字符串 (quit退出): ");
        scanf("%s", s);

        if (strcmp(s, "quit") == 0) break;

        if (validate(nfa, s)) printf("接受!\n");
        else printf("拒绝!\n");
    }

    free(nfa);
    // 5. 释放 DFA
    free(dfa);

    return 0;
}