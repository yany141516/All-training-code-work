#ifndef DFA_H
#define DFA_H

#include "main.h"   // 或者替换成你存放 NFA 的头文件，如 "nfa.h"
#include <stdio.h>
#include <string.h>

#define MAX_DFA_STATE 500

typedef struct {
    int states[MAX_STATE];   // 某个 DFA 状态对应的 NFA 状态集合
    int count;
} StateSet;

typedef struct {
    int from;
    int to;
    char symbol;
} DFAEdge;

typedef struct {
    StateSet dfaStates[MAX_DFA_STATE];
    int dfaStateCount;

    DFAEdge edges[MAX_EDGE];
    int edgeCount;

    int start;
    int accept[MAX_DFA_STATE];
} DFA;

/* -------- 工具：集合判断相等 -------- */
int set_equal(StateSet* a, StateSet* b) {
    if (a->count != b->count) return 0;
    for (int i = 0; i < MAX_STATE; i++)
        if (a->states[i] != b->states[i])
            return 0;
    return 1;
}

/* -------- 工具：查找集合是否存在 -------- */
int find_state(DFA* dfa, StateSet* s) {
    for (int i = 0; i < dfa->dfaStateCount; i++) {
        if (set_equal(&dfa->dfaStates[i], s))
            return i;
    }
    return -1;
}

/* ------------- ε-闭包 --------------- */
void epsilon_closure(NFA* nfa, StateSet* src, StateSet* out) {
    memcpy(out, src, sizeof(StateSet));
    int changed = 1;

    while (changed) {
        changed = 0;

        for (int i = 1; i <= nfa->stateNum; i++) {
            if (out->states[i]) {
                for (int k = 0; k < nfa->graph[i].edgeCount; k++) {
                    if (nfa->graph[i].edges[k].symbol == '#') {
                        int to = nfa->graph[i].edges[k].to;
                        if (!out->states[to]) {
                            out->states[to] = 1;
                            out->count++;
                            changed = 1;
                        }
                    }
                }
            }
        }
    }
}

/* ------------- move 集合 -------------- */
void move_set(NFA* nfa, StateSet* src, char c, StateSet* out) {
    memset(out, 0, sizeof(StateSet));

    for (int i = 1; i <= nfa->stateNum; i++) {
        if (!src->states[i]) continue;

        for (int k = 0; k < nfa->graph[i].edgeCount; k++) {
            if (nfa->graph[i].edges[k].symbol == c) {
                int to = nfa->graph[i].edges[k].to;
                if (!out->states[to]) {
                    out->states[to] = 1;
                    out->count++;
                }
            }
        }
    }
}

/* ------------- NFA → DFA 子集构造法 ------------- */
void nfa_to_dfa(NFA* nfa, DFA* dfa) {
    memset(dfa, 0, sizeof(DFA));

    StateSet start = { 0 };
    start.states[nfa->startState] = 1;
    start.count = 1;

    StateSet start_closure = { 0 };
    epsilon_closure(nfa, &start, &start_closure);

    dfa->dfaStates[0] = start_closure;
    dfa->dfaStateCount = 1;
    dfa->start = 0;

    if (start_closure.states[nfa->endState])
        dfa->accept[0] = 1;

    int queue[500], front = 0, rear = 0;
    queue[rear++] = 0;

    while (front < rear) {
        int id = queue[front++];
        StateSet cur = dfa->dfaStates[id];

        /* 遍历字符集 */
        for (int ci = 0; ci < nfa->charCount; ci++) {
            char ch = nfa->charSet[ci];
            if (ch == '#') continue;

            StateSet moved = { 0 };
            move_set(nfa, &cur, ch, &moved);

            if (moved.count == 0) continue;

            StateSet closure = { 0 };
            epsilon_closure(nfa, &moved, &closure);

            int exist = find_state(dfa, &closure);
            int newID = exist;

            if (exist == -1) {
                newID = dfa->dfaStateCount;
                dfa->dfaStates[newID] = closure;
                dfa->dfaStateCount++;

                if (closure.states[nfa->endState])
                    dfa->accept[newID] = 1;

                queue[rear++] = newID;
            }

            dfa->edges[dfa->edgeCount].from = id;
            dfa->edges[dfa->edgeCount].to = newID;
            dfa->edges[dfa->edgeCount].symbol = ch;
            dfa->edgeCount++;
        }
    }
}

/* ------------- 输出 DFA 到 dot 文件 ------------- */
void save_dfa_dot(DFA* dfa, const char* filename) {
    FILE* fp = fopen(filename, "w");

    fprintf(fp, "digraph DFA {\n");
    fprintf(fp, "  rankdir=LR;\n");
    fprintf(fp, "  start [shape=plaintext];\n");
    fprintf(fp, "  start -> %d;\n", dfa->start);

    for (int i = 0; i < dfa->dfaStateCount; i++) {
        if (dfa->accept[i])
            fprintf(fp, "  %d [shape=doublecircle];\n", i);
        else
            fprintf(fp, "  %d [shape=circle];\n", i);
    }

    for (int i = 0; i < dfa->edgeCount; i++) {
        fprintf(fp, "  %d -> %d [label=\"%c\"];\n",
            dfa->edges[i].from,
            dfa->edges[i].to,
            dfa->edges[i].symbol);
    }

    fprintf(fp, "}\n");
    fclose(fp);

    printf("DFA dot saved to %s\n", filename);
}

#endif
