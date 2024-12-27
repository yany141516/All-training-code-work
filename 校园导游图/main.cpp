#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20
#define INF 99999

// ����ṹ��
typedef struct {
    char name[50];   // ��������
    char code[10];   // �������
    char desc[200];  // ������
} ScenicSpot;

// ͼ�ṹ
typedef struct {
    ScenicSpot spots[MAX];  // �洢������Ϣ
    int matrix[MAX][MAX];   // �ڽӾ��󣬴洢·������
    int numSpots;           // ��������
} Graph;

// ��ʼ��ͼ
void initGraph(Graph* g) {
    g->numSpots = 0;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            g->matrix[i][j] = (i == j) ? 0 : INF;
        }
    }
}

// ��Ӿ���
void addSpot(Graph* g, const char* name, const char* code, const char* desc) {
    if (g->numSpots >= MAX) {
        printf("�޷���Ӹ��ྰ��\n");
        return;
    }
    strcpy(g->spots[g->numSpots].name, name);
    strcpy(g->spots[g->numSpots].code, code);
    strcpy(g->spots[g->numSpots].desc, desc);
    g->numSpots++;
}

// ���·��
void addPath(Graph* g, int from, int to, int length) {
    if (from < 0 || to < 0 || from >= g->numSpots || to >= g->numSpots) {
        printf("·�����ʧ�ܣ�����������Χ\n");
        return;
    }
    g->matrix[from][to] = length;
    g->matrix[to][from] = length; // ����·����˫���
}
//��ջ�����
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // ������뻺����
    }
}

// ��ѯ������Ϣ
void querySpotInfo(Graph* g) {
    int index;
    char buffer[50]; // ��ʱ�����������ڴ洢����

    while (1) {
        printf("�����뾰���ţ�");
        if (fgets(buffer, sizeof(buffer), stdin)) { // ʹ�� fgets ��ȡ����
            if (sscanf(buffer, "%d", &index) == 1) { // ���Խ��������Ϊ����
                if (index >= 0 && index < g->numSpots) {
                    // ���������Ч����ӡ������Ϣ���˳�ѭ��
                    printf("\n�������ƣ�%s\n", g->spots[index].name);
                    printf("������ţ�%s\n", g->spots[index].code);
                    printf("��飺%s\n\n", g->spots[index].desc);
                    break;
                }
                else {
                    printf("��Ч�ľ��������������� 0 �� %d ֮������֣�\n", g->numSpots - 1);
                }
            }
            else {
                printf("������Ч�����������֣�\n");
            }
        }
        else {
            printf("��ȡ����ʧ�ܣ������ԣ�\n");
        }
    }
}

// ���������㷨�����·��
void floyd(int numSpots, int dist[MAX][MAX], int path[MAX][MAX]) {
    for (int k = 0; k < numSpots; k++) {
        for (int i = 0; i < numSpots; i++) {
            for (int j = 0; j < numSpots; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = k; // ��¼�м��
                }
            }
        }
    }
}

// ��ӡ·��
void printPath(int path[MAX][MAX], int u, int v) {
    if (path[u][v] == -1) {
        printf("%d ", v);
        return;
    }
    printPath(path, u, path[u][v]);
    printPath(path, path[u][v], v);
}

// ��ѯ�������·��
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
        printf("��%s��%sû��·��\n", g->spots[from].name, g->spots[to].name);
    }
    else {
        printf("\n��%s��%s�����·������Ϊ��%d\n", g->spots[from].name, g->spots[to].name, dist[from][to]);
        printf("·��Ϊ��%d ", from);
        printPath(path, from, to);
        printf("\n\n");
    }
}

// �鿴�ڽӾ���
void displayAdjMatrix(Graph* g) {
    printf("\n�ڽӾ���\n");
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

// ���ڽӾ��󱣴浽�ļ�
void saveAdjMatrixToFile(Graph* g, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("�޷����ļ� %s\n", filename);
        return;
    }

    fprintf(file, "�ڽӾ���:\n");
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

    fprintf(file, "\n������Ϣ:\n");
    for (int i = 0; i < g->numSpots; i++) {
        fprintf(file, "���: %d, ����: %s, ����: %s, ���: %s\n",
            i, g->spots[i].name, g->spots[i].code, g->spots[i].desc);
    }

    fclose(file);
    printf("�ڽӾ����ѱ��浽�ļ� %s\n", filename);
}


// ��ʾ�˵�
void showMenu() {
    printf("========== У԰����ϵͳ ==========\n");
    printf("1. �鿴���о���\n");
    printf("2. ��ѯ������Ϣ\n");
    printf("3. ��ѯ����֮������·��\n");
    printf("4. ��ʾ�ڽӾ���\n");
    printf("5. ����¾���\n");
    printf("6. �����ļ�\n");
    printf("0. �˳�\n");
    printf("=================================\n");
    printf("����������ѡ��");
}

// ������
int main() {
    Graph g;
    initGraph(&g);
    int k;

    char name[50];   // ��������
    char code[10];   // �������
    char desc[200];  // ������

    addSpot(&g, "����", "A0", "У԰����"); 
    addSpot(&g, "ͼ���", "A1", "�ḻ���鼮��ѧϰ����");
    addSpot(&g, "��ѧ¥", "A2", "�ڿγ���");
    addSpot(&g, "������", "A3", "�˶�����ĳ���");
    addSpot(&g, "ʳ��", "A4", "��ζ�Ĳ���");
    addSpot(&g, "�ٳ�", "A5", "�ٳ��˶�");
    addSpot(&g, "ʵ����", "A6", "��ѧ�о��ĳ���");
    addSpot(&g, "����¥", "A7", "ѧУ�칫����");
    addSpot(&g, "����", "A8", "ѧ������ĵط�");
    addSpot(&g, "С��", "A9", "�羰�����ĺ���");

    // ���·��
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
        case 1: // �鿴���о���
            printf("\n���о�����Ϣ��\n");
            for (int i = 0; i < g.numSpots; i++) {
                printf("���: %d, ����: %s, ����: %s\n", i, g.spots[i].name, g.spots[i].code);
            }
            printf("\n");
            break;
        case 2: { // ��ѯ������Ϣ
            clearInputBuffer();
            querySpotInfo(&g);
            break;
        }
        case 3: { // ��ѯ����֮������·��
            int from, to;
            while(1)
            {
                printf("����������ţ�");
                scanf("%d", &from);
                printf("�������յ��ţ�");
                scanf("%d", &to);
                if (from >= g.numSpots || to >= g.numSpots) {
                    printf("���벻�Ϸ�!����������\n");
                }
                else
                {
                    queryShortestPath(&g, from, to);
                    break;
                }
                
            }
            break;
        }
        case 4: // �鿴�ڽӾ���
            displayAdjMatrix(&g);
            break;

        case 5:
            k = g.numSpots;
            printf("����%d������,�����뾰����Ϣ(���� ���� ���):\n",k);
            scanf("%s %s %s", name, code, desc);
            addSpot(&g, name, code, desc);

            // ��ȡ����ӵľ�������
            k = g.numSpots-1;
            printf("�¾�������ӣ����Ϊ: %d\n", k);

            printf("������Ҫ��ӵ�·������:\n");
            int num;
            scanf("%d", &num);

            for (int i = 0; i < num; i++) {
                printf("�������յ�����·������ (�յ� ����):\n");
                int end, length;
                scanf("%d %d", &end, &length);

                // ��֤·�������Ƿ�Ϸ�
                if (end >= 0 && end < g.numSpots) {
                    addPath(&g, k, end, length);
                    printf("·������ӣ��� %d �� %d ���� %d\n", k, end, length);
                }
                else {
                    printf("��Ч���յ��ţ����������룡\n");
                    i--; // �������뵱ǰ·��
                }
            }///
            break;
        case 6:
            saveAdjMatrixToFile(&g, "E:\jupyter file\adj_matrix.txt");
            break;
        case 0: // �˳�
            printf("��лʹ��У԰����ϵͳ���ټ���\n");
            exit(0);
        default:
            printf("��Чѡ�����������룡\n");
        }
    }

    return 0;
}
