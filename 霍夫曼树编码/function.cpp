#include "HuffmanTree_Head.h"

void Menu() {
    printf("\n            �����˵�              \n");
    printf("************************************\n");
    printf("1. ��ʼ����������\n");
    printf("2. ��ӡ��������\n");
    printf("3. ͨ��������������\n");
    printf("4. ����\n");
    printf("5. ��ӡ�����ļ� \n");
    printf("0. �˳�\n");
    printf("************************************\n");
}

void InitializeHuffmanTree(HuffmanTree** TreeNode, int* nodeNum) {
    int n;
    printf("�������ַ���: ");
    scanf("%d", &n);

    Charstat input[MAX];
    for (int i = 0; i < n; i++) {
        printf("�������ַ���Ȩ��: ");
        scanf(" %c %d", &input[i].c, &input[i].w);
    }

    *nodeNum = 2 * n - 1;
    *TreeNode = (HuffmanTree*)malloc(sizeof(HuffmanTree) * (*nodeNum));
    if (!(*TreeNode)) {
        perror("�ڴ����ʧ��");
        exit(EXIT_FAILURE);
    }

    CreatHuffmanTree(input, *TreeNode, n);
    printf("���������ѳɹ���ʼ�������浽�ļ� 'HfmTree.txt'��\n");

    SaveHuffmanTree(*TreeNode, *nodeNum, "HfmTree.txt");
}

void PrintHuffmanTreeMenu(HuffmanTree* TreeNode, int nodeNum) {
    PrintHuffmanTree(TreeNode, nodeNum);
    SaveHuffmanTree(TreeNode, nodeNum, "TreePrint.txt");
    printf("���������ѱ��浽�ļ� 'TreePrint.txt'��\n");
}

void CreatHuffmanTree(Charstat* cStat, HuffmanTree* TreeNode, int codeNum) {
    int w1, w2, p1, p2;
    int nodeNum = 2 * codeNum - 1;

    for (int i = 0; i < nodeNum; i++) {
        if (i < codeNum) {
            TreeNode[i].ch = cStat[i].c;
            TreeNode[i].weight = cStat[i].w;
        }
        else {
            TreeNode[i].ch = '\0';
            TreeNode[i].weight = 0;
        }
        TreeNode[i].lchild = BlankFlag;
        TreeNode[i].rchild = BlankFlag;
        TreeNode[i].parent = BlankFlag;
    }

    for (int i = codeNum; i < nodeNum; i++) {
        w1 = w2 = INT_MAX;
        p1 = p2 = BlankFlag;

        for (int k = 0; k < i; k++) {
            if (TreeNode[k].weight < w1 && TreeNode[k].parent == BlankFlag) {
                w1 = TreeNode[k].weight;
                p1 = k;
            }
        }
        for (int k = 0; k < i; k++) {
            if (TreeNode[k].weight < w2 && TreeNode[k].parent == BlankFlag && k != p1) {
                w2 = TreeNode[k].weight;
                p2 = k;
            }
        }
        TreeNode[i].weight = w1 + w2;
        TreeNode[i].lchild = p1;
        TreeNode[i].rchild = p2;
        TreeNode[p1].parent = i;
        TreeNode[p2].parent = i;
    }
}

void SaveHuffmanTree(HuffmanTree* TreeNode, int nodeNum, const char* outputFile) {
    FILE* out = fopen(outputFile, "w");
    if (!out) {
        perror("�޷����ļ�");
        return;
    }

    for (int i = 0; i < nodeNum; i++) {
        fprintf(out, "�ڵ� %d: char:%c weight:%d lchild:%d rchild:%d parent:%d\n",
            i, TreeNode[i].ch, TreeNode[i].weight,
            TreeNode[i].lchild, TreeNode[i].rchild, TreeNode[i].parent);
    }

    fclose(out);
}

void ReadHuffmanTreeFromFile(const char* inputFile, HuffmanTree** TreeNode, int* nodeNum) {
    FILE* in = fopen(inputFile, "r");
    if (!in) {
        perror("�޷����ļ�");
        *TreeNode = NULL;
        return;
    }

    // ͳ���ļ��нڵ�������ÿ��һ���ڵ㣩
    *nodeNum = 0;
    char tempLine[256];  // ��������ȡÿ��
    while (fgets(tempLine, sizeof(tempLine), in)) {
        (*nodeNum)++;
    }
    rewind(in); // �ص��ļ���ͷ

    // �����ڴ�
    *TreeNode = (HuffmanTree*)malloc(sizeof(HuffmanTree) * (*nodeNum));
    if (!(*TreeNode)) {
        perror("�ڴ����ʧ��");
        fclose(in);
        return;
    }

    // ���н����ļ�
    for (int i = 0; i < *nodeNum; i++) {
        // ʹ�� fscanf ��ָ����ʽ��ȡһ��
        int res = fscanf(in, " �ڵ� %*d: char:%c weight:%d lchild:%d rchild:%d parent:%d\n",
            &(*TreeNode)[i].ch,
            &(*TreeNode)[i].weight,
            &(*TreeNode)[i].lchild,
            &(*TreeNode)[i].rchild,
            &(*TreeNode)[i].parent);

        // ����ȡ�Ƿ�ɹ�
        if (res != 5) {
            printf("�ļ���ʽ�����޷���ȷ��ȡ�� %d ���ڵ�����\n", i);
            free(*TreeNode);
            fclose(in);
            *TreeNode = NULL;
            return;
        }

        // �����ַ�Ϊ�ո���������
        if ((*TreeNode)[i].ch == ' ') {
            (*TreeNode)[i].ch = '_';  // �� '_' ����ո��ַ��������ڵ�����ʾ��
        }

        /*
        ��ӡ������Ϣ
        printf("�ڵ� %d: �ַ�=%c Ȩֵ=%d ����=%d ����=%d ���ڵ�=%d\n",
            i, (*TreeNode)[i].ch, (*TreeNode)[i].weight,
            (*TreeNode)[i].lchild, (*TreeNode)[i].rchild,
            (*TreeNode)[i].parent);
        */
    }

    fclose(in);
}

void PrintHuffmanTreeVisual(HuffmanTree* TreeNode, int index, int depth, char edge) {
    if (index == BlankFlag) {
        return;
    }

    PrintHuffmanTreeVisual(TreeNode, TreeNode[index].rchild, depth + 1, '/');
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }

    if (edge != ' ') {
        printf("%c---", edge);
    }
    if (TreeNode[index].lchild != BlankFlag && TreeNode[index].rchild != BlankFlag) {
        printf("(%d)\n", TreeNode[index].weight);
    }
    else {
        printf("(%c)\n", TreeNode[index].ch);
    }

    PrintHuffmanTreeVisual(TreeNode, TreeNode[index].lchild, depth + 1, '\\');
}

void PrintHuffmanTree(HuffmanTree* TreeNode, int nodeNum) {
    int root = nodeNum - 1;
    printf("��������:\n");
    PrintHuffmanTreeVisual(TreeNode, root, 0, ' ');
}

void EncodeFile(HuffmanTree* TreeNode, int codeNum) {
    char** HC = (char**)malloc(sizeof(char*) * codeNum);
    if (!HC) {
        perror("HC ����ʧ��");
        return;
    }

    // ���ɹ���������
    Encoding(codeNum, TreeNode, HC);

    // �������ļ�
    FILE* input = fopen("ToBeTran.txt", "r");
    if (!input) {
        perror("�޷����ļ� ToBeTran");
        for (int i = 0; i < codeNum; i++) {
            if (HC[i]) free(HC[i]);
        }
        free(HC);
        return;
    }

    // ������ļ�
    FILE* output = fopen("CodeFile.txt", "w");
    if (!output) {
        perror("�޷����ļ� CodeFile");
        fclose(input);
        for (int i = 0; i < codeNum; i++) {
            if (HC[i]) free(HC[i]);
        }
        free(HC);
        return;
    }

    // �����ļ�����
    char c;
    while ((c = fgetc(input)) != EOF) {
        int found = 0; // ����Ƿ��ҵ��ַ�
        for (int i = 0; i < codeNum; i++) {
            if (TreeNode[i].ch == c) {
                fprintf(output, "%s ", HC[i]); // д�����
                found = 1;
                break;
            }
        }

        if (!found) {
            // ����δ�ҵ��ַ�
            printf("����: �ַ� '%c' �޷��ڹ������������ҵ���������\n", c);
        }
    }

    fclose(input);
    fclose(output);

    printf("�ļ��ѱ��벢���浽 CodeFile��\n");

    // �ͷ��ڴ�
    for (int i = 0; i < codeNum; i++) {
        if (HC[i]) {
            free(HC[i]);
        }
    }
    free(HC);
}

void PrintHuffmanCodes(HuffmanTree* TreeNode, int codeNum) {
    if (!TreeNode || codeNum <= 0) {
        printf("��������δ��ʼ�����ַ�����Ч��\n");
        return;
    }

    char* tempCode = (char*)malloc(codeNum * sizeof(char)); // ��ʱ�洢����·��
    if (!tempCode) {
        perror("�ڴ����ʧ��");
        return;
    }
    tempCode[codeNum - 1] = '\0'; // ������

    printf("��ĸ�����Ӧ�Ĺ���������:\n");
    for (int i = 0; i < codeNum; i++) {
        if (TreeNode[i].ch == '\0') continue; // ���Է�Ҷ�ڵ�

        int start = codeNum - 1; // ������ʼλ��
        for (int c = i, p = TreeNode[i].parent; p != BlankFlag; c = p, p = TreeNode[p].parent) {
            tempCode[--start] = (TreeNode[p].lchild == c) ? '0' : '1'; // ������Ϊ0��������Ϊ1
        }

        printf("�ַ� '%c': %s\n", TreeNode[i].ch, &tempCode[start]);
    }

    free(tempCode);
}

void Encoding(int codeNum, HuffmanTree* TreeNode, char** HC) {
    int nodeNum = 2 * codeNum - 1; // Huffman Tree �ڵ�����
    char* cd = (char*)malloc(sizeof(char) * codeNum); // ��ʱ����洢
    if (!cd) {
        perror("�ڴ����ʧ��");
        exit(EXIT_FAILURE);
    }
    cd[codeNum - 1] = '\0'; // ��ʼ���ַ���������

    for (int i = 0; i < codeNum; i++) {
        int start = codeNum - 1;
        for (int c = i, p = TreeNode[i].parent; p != BlankFlag; c = p, p = TreeNode[p].parent) {
            if (p < 0 || p >= nodeNum) { // �߽���
                fprintf(stderr, "�Ƿ����ʣ�p=%d ������Χ\n", p);
                free(cd);
                return;
            }
            cd[--start] = (TreeNode[p].lchild == c) ? '0' : '1';
        }
        HC[i] = _strdup(&cd[start]); // �������
        if (!HC[i]) {
            perror("�ڴ����ʧ��");
            free(cd);
            exit(EXIT_FAILURE);
        }
    }

    free(cd); // �ͷ���ʱ����洢
}

void DecodeHuffmanTree(const char* codeFile, const char* textFile, HuffmanTree* TreeNode, int root) {
    FILE* in = fopen(codeFile, "r");
    FILE* out = fopen(textFile, "w");
    if (!in || !out) {
        perror("�ļ���ʧ��");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }

    int current = root;  // �Ӹ��ڵ㿪ʼ����
    char bit;
    while ((bit = fgetc(in)) != EOF) {
        if (bit == ' ') continue;  // ���Կո�
        if (bit == '0') {
            current = TreeNode[current].lchild;
        }
        else if (bit == '1') {
            current = TreeNode[current].rchild;
        }

        // �������Ҷ�ӽڵ㣬�����Ӧ�ַ����ص����ڵ�
        if (TreeNode[current].lchild == -1 && TreeNode[current].rchild == -1) {
            fputc(TreeNode[current].ch, out);
            current = root;
        }
    }

    printf("������ɣ������д���ļ���%s\n", textFile);
    fclose(in);
    fclose(out);
}

void PrintCodeFile(const char* codeFile, const char* codePrintFile) {
    FILE* in = fopen(codeFile, "r");
    FILE* out = fopen(codePrintFile, "w");
    if (!in || !out) {
        perror("�ļ���ʧ��");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }

    int count = 0;
    char bit;
    printf("�����ļ����ݣ�ÿ��50������\n");
    while ((bit = fgetc(in)) != EOF) {
        if (bit == ' ') continue;  // ���Կո�

        // ��ӡ���ն�
        printf("%c", bit);
        fputc(bit, out);  // ͬʱд��Ŀ���ļ�
        count++;

        if (count % 50 == 0) {
            printf("\n");
            fputc('\n', out);  // ÿ50���ַ�����
        }
    }

    // ������һ���ַ�����50��������
    if (count % 50 != 0) {
        printf("\n");
        fputc('\n', out);
    }

    printf("�����ļ���д���ļ���%s\n", codePrintFile);
    fclose(in);
    fclose(out);
}

void compare(const char* ToBeTran, const char* TextFile) {
    FILE* s1 = fopen(ToBeTran, "r"); // �򿪵�һ���ļ�
    FILE* s2 = fopen(TextFile, "r"); // �򿪵ڶ����ļ�

    // ����ļ��Ƿ�ɹ���
    if (!s1 || !s2) {
        perror("�ļ���ʧ��");
        if (s1) fclose(s1); // �����һ���ļ��Ѵ򿪣��ر���
        if (s2) fclose(s2); // ����ڶ����ļ��Ѵ򿪣��ر���
        return;
    }

    bool areSame = true; // �����ļ�������ͬ
    int ch1, ch2; // ���ڴ洢���ļ��ж�ȡ���ַ�

    // ���ֽڱȽ������ļ�������
    while (true) {
        ch1 = fgetc(s1); // �ӵ�һ���ļ���ȡһ���ַ�
        ch2 = fgetc(s2); // �ӵڶ����ļ���ȡһ���ַ�

        // �Ƚ��ַ��Ƿ���ͬ
        if (ch1 != ch2) {
            areSame = false;
            break;
        }

        // ��������ļ�ͬʱ����ĩβ���˳�ѭ��
        if (ch1 == EOF && ch2 == EOF) {
            break;
        }

        // ���һ���ļ�����ĩβ����һ���ļ�û�У���ʾ����ͬ
        if ((ch1 == EOF && ch2 != EOF) || (ch1 != EOF && ch2 == EOF)) {
            areSame = false;
            break;
        }
    }

    // ��ӡ�ȽϽ��
    if (areSame) {
        printf("TextFile�ļ���ToBeTran�ļ���������ͬ��\n");
    }
    else {
        printf("TextFile�ļ���ToBeTran�ļ������ݲ�ͬ��\n");
    }

    // �ر��ļ�
    fclose(s1);
    fclose(s2);
}