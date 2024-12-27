#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20
#define INF 99999

// 景点结构体
typedef struct {
    char name[50];   // 景点名称
    char code[10];   // 景点代号
    char desc[200];  // 景点简介
} ScenicSpot;

// 图结构
typedef struct {
    ScenicSpot spots[MAX];  // 存储景点信息
    int matrix[MAX][MAX];   // 邻接矩阵，存储路径长度
    int numSpots;           // 景点数量
} Graph;

// 初始化图
void initGraph(Graph* g) {
    g->numSpots = 0;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            g->matrix[i][j] = (i == j) ? 0 : INF;
        }
    }
}

// 添加景点
void addSpot(Graph* g, const char* name, const char* code, const char* desc) {
    if (g->numSpots >= MAX) {
        printf("无法添加更多景点\n");
        return;
    }
    strcpy(g->spots[g->numSpots].name, name);
    strcpy(g->spots[g->numSpots].code, code);
    strcpy(g->spots[g->numSpots].desc, desc);
    g->numSpots++;
}

// 添加路径
void addPath(Graph* g, int from, int to, int length) {
    if (from < 0 || to < 0 || from >= g->numSpots || to >= g->numSpots) {
        printf("路径添加失败：索引超出范围\n");
        return;
    }
    g->matrix[from][to] = length;
    g->matrix[to][from] = length; // 假设路径是双向的
}
//清空缓存区
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // 清空输入缓冲区
    }
}

// 查询景点信息
void querySpotInfo(Graph* g) {
    int index;
    char buffer[50]; // 临时缓冲区，用于存储输入

    while (1) {
        printf("请输入景点编号：");
        if (fgets(buffer, sizeof(buffer), stdin)) { // 使用 fgets 读取输入
            if (sscanf(buffer, "%d", &index) == 1) { // 尝试将输入解析为整数
                if (index >= 0 && index < g->numSpots) {
                    // 如果索引有效，打印景点信息并退出循环
                    printf("\n景点名称：%s\n", g->spots[index].name);
                    printf("景点代号：%s\n", g->spots[index].code);
                    printf("简介：%s\n\n", g->spots[index].desc);
                    break;
                }
                else {
                    printf("无效的景点索引，请输入 0 到 %d 之间的数字！\n", g->numSpots - 1);
                }
            }
            else {
                printf("输入无效，请输入数字！\n");
            }
        }
        else {
            printf("读取输入失败，请重试！\n");
        }
    }
}

// 弗洛伊德算法求最短路径
void floyd(int numSpots, int dist[MAX][MAX], int path[MAX][MAX]) {
    for (int k = 0; k < numSpots; k++) {
        for (int i = 0; i < numSpots; i++) {
            for (int j = 0; j < numSpots; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = k; // 记录中间点
                }
            }
        }
    }
}

// 打印路径
void printPath(int path[MAX][MAX], int u, int v) {
    if (path[u][v] == -1) {
        printf("%d ", v);
        return;
    }
    printPath(path, u, path[u][v]);
    printPath(path, path[u][v], v);
}

// 查询两点最短路径
void queryShortestPath(Graph* g, int from, int to) {
    int dist[MAX][MAX], path[MAX][MAX];
    for (int i = 0; i < g->numSpots; i++) {
        for (int j = 0; j < g->numSpots; j++) {
            dist[i][j] = g->matrix[i][j];
            path[i][j] = (g->matrix[i][j] < INF && i != j) ? -1 : INF;
        }
    }

    floyd(g->numSpots, dist, path);

    if (dist[from][to] == INF) {
        printf("从%s到%s没有路径\n", g->spots[from].name, g->spots[to].name);
    }
    else {
        printf("\n从%s到%s的最短路径长度为：%d\n", g->spots[from].name, g->spots[to].name, dist[from][to]);
        printf("路径为：%d ", from);
        printPath(path, from, to);
        printf("\n\n");
    }
}

// 查看邻接矩阵
void displayAdjMatrix(Graph* g) {
    printf("\n邻接矩阵：\n");
    for (int i = 0; i < g->numSpots; i++) {
        for (int j = 0; j < g->numSpots; j++) {
            if (g->matrix[i][j] == INF) {
                printf("%5s", "INF");
            }
            else {
                printf("%5d", g->matrix[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// 将邻接矩阵保存到文件
void saveAdjMatrixToFile(Graph* g, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("无法打开文件 %s\n", filename);
        return;
    }

    fprintf(file, "邻接矩阵:\n");
    for (int i = 0; i < g->numSpots; i++) {
        for (int j = 0; j < g->numSpots; j++) {
            if (g->matrix[i][j] == INF) {
                fprintf(file, "INF ");
            }
            else {
                fprintf(file, "%d ", g->matrix[i][j]);
            }
        }
        fprintf(file, "\n");
    }

    fprintf(file, "\n景点信息:\n");
    for (int i = 0; i < g->numSpots; i++) {
        fprintf(file, "编号: %d, 名称: %s, 代号: %s, 简介: %s\n",
            i, g->spots[i].name, g->spots[i].code, g->spots[i].desc);
    }

    fclose(file);
    printf("邻接矩阵已保存到文件 %s\n", filename);
}


// 显示菜单
void showMenu() {
    printf("========== 校园导游系统 ==========\n");
    printf("1. 查看所有景点\n");
    printf("2. 查询景点信息\n");
    printf("3. 查询两点之间的最短路径\n");
    printf("4. 显示邻接矩阵\n");
    printf("5. 添加新景点\n");
    printf("6. 保存文件\n");
    printf("0. 退出\n");
    printf("=================================\n");
    printf("请输入您的选择：");
}

// 主函数
int main() {
    Graph g;
    initGraph(&g);
    int k;

    char name[50];   // 景点名称
    char code[10];   // 景点代号
    char desc[200];  // 景点简介

    addSpot(&g, "正门", "A0", "校园正门"); 
    addSpot(&g, "图书馆", "A1", "丰富的书籍与学习场所");
    addSpot(&g, "教学楼", "A2", "授课场所");
    addSpot(&g, "体育馆", "A3", "运动健身的场所");
    addSpot(&g, "食堂", "A4", "美味的餐厅");
    addSpot(&g, "操场", "A5", "操场运动");
    addSpot(&g, "实验室", "A6", "科学研究的场所");
    addSpot(&g, "行政楼", "A7", "学校办公场所");
    addSpot(&g, "宿舍", "A8", "学生生活的地方");
    addSpot(&g, "小湖", "A9", "风景优美的湖泊");

    // 添加路径
    addPath(&g, 0, 1, 300);
    addPath(&g, 0, 2, 200);
    addPath(&g, 5, 0, 350);
    addPath(&g, 1, 2, 200);
    addPath(&g, 0, 4, 600);
    addPath(&g, 8, 6, 300);
    addPath(&g, 2, 3, 400);
    addPath(&g, 3, 4, 100);
    addPath(&g, 2, 4, 100);
    addPath(&g, 4, 5, 500);
    addPath(&g, 5, 6, 300);
    addPath(&g, 1, 6, 100);
    addPath(&g, 4, 7, 50);
    addPath(&g, 6, 7, 250);
    addPath(&g, 5, 7, 600);
    addPath(&g, 7, 8, 350);
    addPath(&g, 1, 8, 1100);
    addPath(&g, 4, 6, 700);
    addPath(&g, 8, 9, 200);
    addPath(&g, 3, 6, 250);
    addPath(&g, 9, 0, 400);
    addPath(&g, 2, 6, 100);
    addPath(&g, 1, 9, 600);
    addPath(&g, 0, 6, 700);


    int choice;
    while (1) {
        showMenu();
        scanf("%d", &choice);

        switch (choice) {
        case 1: // 查看所有景点
            printf("\n所有景点信息：\n");
            for (int i = 0; i < g.numSpots; i++) {
                printf("编号: %d, 名称: %s, 代号: %s\n", i, g.spots[i].name, g.spots[i].code);
            }
            printf("\n");
            break;
        case 2: { // 查询景点信息
            clearInputBuffer();
            querySpotInfo(&g);
            break;
        }
        case 3: { // 查询两点之间的最短路径
            int from, to;
            while(1)
            {
                printf("请输入起点编号：");
                scanf("%d", &from);
                printf("请输入终点编号：");
                scanf("%d", &to);
                if (from >= g.numSpots || to >= g.numSpots) {
                    printf("输入不合法!请重新输入\n");
                }
                else
                {
                    queryShortestPath(&g, from, to);
                    break;
                }
                
            }
            break;
        }
        case 4: // 查看邻接矩阵
            displayAdjMatrix(&g);
            break;

        case 5:
            k = g.numSpots;
            printf("已有%d个景点,请输入景点信息(名称 代号 简介):\n",k);
            scanf("%s %s %s", name, code, desc);
            addSpot(&g, name, code, desc);

            // 获取新添加的景点索引
            k = g.numSpots-1;
            printf("新景点已添加，编号为: %d\n", k);

            printf("请输入要添加的路径数量:\n");
            int num;
            scanf("%d", &num);

            for (int i = 0; i < num; i++) {
                printf("请输入终点编号与路径长度 (终点 距离):\n");
                int end, length;
                scanf("%d %d", &end, &length);

                // 验证路径输入是否合法
                if (end >= 0 && end < g.numSpots) {
                    addPath(&g, k, end, length);
                    printf("路径已添加：从 %d 到 %d 距离 %d\n", k, end, length);
                }
                else {
                    printf("无效的终点编号，请重新输入！\n");
                    i--; // 重新输入当前路径
                }
            }///
            break;
        case 6:
            saveAdjMatrixToFile(&g, "E:\jupyter file\adj_matrix.txt");
            break;
        case 0: // 退出
            printf("感谢使用校园导游系统，再见！\n");
            exit(0);
        default:
            printf("无效选择，请重新输入！\n");
        }
    }

    return 0;
}
