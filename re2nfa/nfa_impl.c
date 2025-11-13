#include "main.h"

/*---------------- 工具函数 ----------------*/

int is_operator(char c) {
    return (c == '*' || c == '|' || c == '.');
}

int priority(char c) {
    if (c == '*') return 3;
    if (c == '.') return 2;
    if (c == '|') return 1;
    return 0;
}

void add_edge(NFA* nfa, int from, int to, char symbol) {
    State* s = &nfa->graph[from];
    s->edges[s->edgeCount].to = to;
    s->edges[s->edgeCount].symbol = symbol;
    s->edgeCount++;
}

int new_state(NFA* nfa) {
    nfa->stateNum++;
    nfa->graph[nfa->stateNum].edgeCount = 0;
    return nfa->stateNum;
}

/*---------------- 1. 插入连接符 ----------------*/
/*  例如:  a(b|c)*d  ->  a.(b|c)*.d                 */

void insert_contact(NFA* nfa) {
    int len = (int)strlen(nfa->re);
    int k = 0;

    for (int i = 0; i < len - 1; i++) {
        char c1 = nfa->re[i];
        char c2 = nfa->re[i + 1];
        nfa->re_[k++] = c1;

        if (c1 != '(' && c2 != ')' &&
            c1 != '|' && c2 != '|' &&
            c2 != '*') {
            nfa->re_[k++] = '.';
        }
    }
    nfa->re_[k++] = nfa->re[len - 1];
    nfa->re_[k] = '\0';
}

/*---------------- 2. 中缀 -> 后缀 ----------------*/
/*  使用栈实现 Shunting-yard 算法                   */

void re2pe(NFA* nfa) {
    char stack[MAX_STACK];
    int top = -1;
    int k = 0;
    int len = (int)strlen(nfa->re_);

    for (int i = 0; i < len; i++) {
        char c = nfa->re_[i];

        if (c == '(') {
            stack[++top] = c;
        }
        else if (c == ')') {
            while (top >= 0 && stack[top] != '(') {
                nfa->pe[k++] = stack[top--];
            }
            if (top >= 0 && stack[top] == '(') top--;
        }
        else if (is_operator(c)) {
            while (top >= 0 && is_operator(stack[top]) &&
                priority(stack[top]) >= priority(c)) {
                nfa->pe[k++] = stack[top--];
            }
            stack[++top] = c;
        }
        else {
            nfa->pe[k++] = c;
            /* 记录字符集 */
            int exist = 0;
            for (int j = 0; j < nfa->charCount; j++)
                if (nfa->charSet[j] == c) { exist = 1; break; }
            if (!exist) nfa->charSet[nfa->charCount++] = c;
        }
    }
    while (top >= 0) {
        nfa->pe[k++] = stack[top--];
    }
    nfa->pe[k] = '\0';
}

/*---------------- 3. 后缀 -> NFA (Thompson) ----------------*/

typedef struct { int s, e; } SE;

void pe2nfa(NFA* nfa) {
    SE stack[MAX_STACK];
    int top = -1;
    int len = (int)strlen(nfa->pe);

    for (int i = 0; i < len; i++) {
        char c = nfa->pe[i];

        if (!is_operator(c)) {
            /* 基本 NFA：一个字符一条边 */
            SE t;
            t.s = new_state(nfa);
            t.e = new_state(nfa);
            add_edge(nfa, t.s, t.e, c);
            stack[++top] = t;
        }
        else if (c == '*') {
            SE a = stack[top--];
            SE r;
            r.s = new_state(nfa);
            r.e = new_state(nfa);

            add_edge(nfa, r.s, a.s, '#');
            add_edge(nfa, r.s, r.e, '#');
            add_edge(nfa, a.e, r.e, '#');
            add_edge(nfa, a.e, a.s, '#');

            stack[++top] = r;
        }
        else if (c == '.') {
            SE b = stack[top--];
            SE a = stack[top--];

            add_edge(nfa, a.e, b.s, '#');
            SE r; r.s = a.s; r.e = b.e;
            stack[++top] = r;
        }
        else if (c == '|') {
            SE b = stack[top--];
            SE a = stack[top--];
            SE r;
            r.s = new_state(nfa);
            r.e = new_state(nfa);

            add_edge(nfa, r.s, a.s, '#');
            add_edge(nfa, r.s, b.s, '#');
            add_edge(nfa, a.e, r.e, '#');
            add_edge(nfa, b.e, r.e, '#');

            stack[++top] = r;
        }
    }

    nfa->startState = stack[top].s;
    nfa->endState = stack[top].e;
}

/*---------------- 4. ε-闭包 (单点) ----------------*/

void closure(NFA* nfa, int state, int* visited) {
    if (visited[state]) return;
    visited[state] = 1;

    for (int i = 0; i < nfa->graph[state].edgeCount; i++) {
        if (nfa->graph[state].edges[i].symbol == '#') {
            int to = nfa->graph[state].edges[i].to;
            if (!visited[to])
                closure(nfa, to, visited);
        }
    }
}

/*---------------- 5. NFA 验证字符串 ----------------*/

int validate(NFA* nfa, const char* input) {
    int cur[MAX_STATE] = { 0 };
    int nxt[MAX_STATE] = { 0 };

    /* 初始 ε-闭包 */
    closure(nfa, nfa->startState, cur);

    int len = (int)strlen(input);
    for (int i = 0; i < len; i++) {
        char c = input[i];
        memset(nxt, 0, sizeof(nxt));

        for (int s = 1; s <= nfa->stateNum; s++) {
            if (!cur[s]) continue;

            for (int k = 0; k < nfa->graph[s].edgeCount; k++) {
                Edge e = nfa->graph[s].edges[k];
                if (e.symbol == c) {
                    closure(nfa, e.to, nxt);
                }
            }
        }

        memcpy(cur, nxt, sizeof(cur));
    }

    return cur[nfa->endState] != 0;
}

/*---------------- 6. 输出 NFA ----------------*/

void print_nfa(NFA* nfa) {
    printf("stateNum = %d\n", nfa->stateNum);
    printf("start = %d\nend = %d\n", nfa->startState, nfa->endState);

    for (int i = 1; i <= nfa->stateNum; i++) {
        for (int j = 0; j < nfa->graph[i].edgeCount; j++) {
            printf("%d --%c--> %d\n",
                i,
                nfa->graph[i].edges[j].symbol,
                nfa->graph[i].edges[j].to);
        }
    }
}

/*---------------- 7. 保存 NFA 为 dot ----------------*/

void save_dot(NFA* nfa, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Cannot open %s\n", filename);
        return;
    }

    fprintf(fp, "digraph NFA {\n");
    fprintf(fp, "  rankdir=LR;\n");
    fprintf(fp, "  node [shape=circle];\n\n");

    fprintf(fp, "  start [shape=plaintext];\n");
    fprintf(fp, "  start -> %d;\n\n", nfa->startState);

    fprintf(fp, "  %d [shape=doublecircle];\n\n", nfa->endState);

    for (int i = 1; i <= nfa->stateNum; i++) {
        for (int j = 0; j < nfa->graph[i].edgeCount; j++) {
            char c = nfa->graph[i].edges[j].symbol;
            if (c == '#')
                fprintf(fp, "  %d -> %d [label=\"eps\"];\n",
                    i, nfa->graph[i].edges[j].to);
            else
                fprintf(fp, "  %d -> %d [label=\"%c\"];\n",
                    i, nfa->graph[i].edges[j].to, c);
        }
    }

    fprintf(fp, "}\n");
    fclose(fp);

    printf("NFA dot saved to %s\n", filename);
}
