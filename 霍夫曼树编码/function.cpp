#include "HuffmanTree_Head.h"

void Menu() {
    printf("\n            操作菜单              \n");
    printf("************************************\n");
    printf("1. 初始化哈夫曼树\n");
    printf("2. 打印哈夫曼树\n");
    printf("3. 通过哈夫曼树编码\n");
    printf("4. 译码\n");
    printf("5. 打印代码文件 \n");
    printf("0. 退出\n");
    printf("************************************\n");
}

void InitializeHuffmanTree(HuffmanTree** TreeNode, int* nodeNum) {
    int n;
    printf("请输入字符数: ");
    scanf("%d", &n);

    Charstat input[MAX];
    for (int i = 0; i < n; i++) {
        printf("请输入字符和权重: ");
        scanf(" %c %d", &input[i].c, &input[i].w);
    }

    *nodeNum = 2 * n - 1;
    *TreeNode = (HuffmanTree*)malloc(sizeof(HuffmanTree) * (*nodeNum));
    if (!(*TreeNode)) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }

    CreatHuffmanTree(input, *TreeNode, n);
    printf("哈夫曼树已成功初始化并保存到文件 'HfmTree.txt'。\n");

    SaveHuffmanTree(*TreeNode, *nodeNum, "HfmTree.txt");
}

void PrintHuffmanTreeMenu(HuffmanTree* TreeNode, int nodeNum) {
    PrintHuffmanTree(TreeNode, nodeNum);
    SaveHuffmanTree(TreeNode, nodeNum, "TreePrint.txt");
    printf("哈夫曼树已保存到文件 'TreePrint.txt'。\n");
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
        perror("无法打开文件");
        return;
    }

    for (int i = 0; i < nodeNum; i++) {
        fprintf(out, "节点 %d: char:%c weight:%d lchild:%d rchild:%d parent:%d\n",
            i, TreeNode[i].ch, TreeNode[i].weight,
            TreeNode[i].lchild, TreeNode[i].rchild, TreeNode[i].parent);
    }

    fclose(out);
}

void ReadHuffmanTreeFromFile(const char* inputFile, HuffmanTree** TreeNode, int* nodeNum) {
    FILE* in = fopen(inputFile, "r");
    if (!in) {
        perror("无法打开文件");
        *TreeNode = NULL;
        return;
    }

    // 统计文件中节点数量（每行一个节点）
    *nodeNum = 0;
    char tempLine[256];  // 缓冲区读取每行
    while (fgets(tempLine, sizeof(tempLine), in)) {
        (*nodeNum)++;
    }
    rewind(in); // 回到文件开头

    // 分配内存
    *TreeNode = (HuffmanTree*)malloc(sizeof(HuffmanTree) * (*nodeNum));
    if (!(*TreeNode)) {
        perror("内存分配失败");
        fclose(in);
        return;
    }

    // 逐行解析文件
    for (int i = 0; i < *nodeNum; i++) {
        // 使用 fscanf 按指定格式读取一行
        int res = fscanf(in, " 节点 %*d: char:%c weight:%d lchild:%d rchild:%d parent:%d\n",
            &(*TreeNode)[i].ch,
            &(*TreeNode)[i].weight,
            &(*TreeNode)[i].lchild,
            &(*TreeNode)[i].rchild,
            &(*TreeNode)[i].parent);

        // 检查读取是否成功
        if (res != 5) {
            printf("文件格式错误：无法正确读取第 %d 个节点数据\n", i);
            free(*TreeNode);
            fclose(in);
            *TreeNode = NULL;
            return;
        }

        // 处理字符为空格的特殊情况
        if ((*TreeNode)[i].ch == ' ') {
            (*TreeNode)[i].ch = '_';  // 用 '_' 代替空格字符（仅用于调试显示）
        }

        /*
        打印调试信息
        printf("节点 %d: 字符=%c 权值=%d 左子=%d 右子=%d 父节点=%d\n",
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
    printf("哈夫曼树:\n");
    PrintHuffmanTreeVisual(TreeNode, root, 0, ' ');
}

void EncodeFile(HuffmanTree* TreeNode, int codeNum) {
    char** HC = (char**)malloc(sizeof(char*) * codeNum);
    if (!HC) {
        perror("HC 分配失败");
        return;
    }

    // 生成哈夫曼编码
    Encoding(codeNum, TreeNode, HC);

    // 打开输入文件
    FILE* input = fopen("ToBeTran.txt", "r");
    if (!input) {
        perror("无法打开文件 ToBeTran");
        for (int i = 0; i < codeNum; i++) {
            if (HC[i]) free(HC[i]);
        }
        free(HC);
        return;
    }

    // 打开输出文件
    FILE* output = fopen("CodeFile.txt", "w");
    if (!output) {
        perror("无法打开文件 CodeFile");
        fclose(input);
        for (int i = 0; i < codeNum; i++) {
            if (HC[i]) free(HC[i]);
        }
        free(HC);
        return;
    }

    // 编码文件内容
    char c;
    while ((c = fgetc(input)) != EOF) {
        int found = 0; // 标记是否找到字符
        for (int i = 0; i < codeNum; i++) {
            if (TreeNode[i].ch == c) {
                fprintf(output, "%s ", HC[i]); // 写入编码
                found = 1;
                break;
            }
        }

        if (!found) {
            // 提醒未找到字符
            printf("警告: 字符 '%c' 无法在哈夫曼编码中找到！已跳过\n", c);
        }
    }

    fclose(input);
    fclose(output);

    printf("文件已编码并保存到 CodeFile。\n");

    // 释放内存
    for (int i = 0; i < codeNum; i++) {
        if (HC[i]) {
            free(HC[i]);
        }
    }
    free(HC);
}

void PrintHuffmanCodes(HuffmanTree* TreeNode, int codeNum) {
    if (!TreeNode || codeNum <= 0) {
        printf("哈夫曼树未初始化或字符数无效。\n");
        return;
    }

    char* tempCode = (char*)malloc(codeNum * sizeof(char)); // 临时存储编码路径
    if (!tempCode) {
        perror("内存分配失败");
        return;
    }
    tempCode[codeNum - 1] = '\0'; // 结束符

    printf("字母及其对应的哈夫曼编码:\n");
    for (int i = 0; i < codeNum; i++) {
        if (TreeNode[i].ch == '\0') continue; // 忽略非叶节点

        int start = codeNum - 1; // 编码起始位置
        for (int c = i, p = TreeNode[i].parent; p != BlankFlag; c = p, p = TreeNode[p].parent) {
            tempCode[--start] = (TreeNode[p].lchild == c) ? '0' : '1'; // 左子树为0，右子树为1
        }

        printf("字符 '%c': %s\n", TreeNode[i].ch, &tempCode[start]);
    }

    free(tempCode);
}

void Encoding(int codeNum, HuffmanTree* TreeNode, char** HC) {
    int nodeNum = 2 * codeNum - 1; // Huffman Tree 节点总数
    char* cd = (char*)malloc(sizeof(char) * codeNum); // 临时编码存储
    if (!cd) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }
    cd[codeNum - 1] = '\0'; // 初始化字符串结束符

    for (int i = 0; i < codeNum; i++) {
        int start = codeNum - 1;
        for (int c = i, p = TreeNode[i].parent; p != BlankFlag; c = p, p = TreeNode[p].parent) {
            if (p < 0 || p >= nodeNum) { // 边界检查
                fprintf(stderr, "非法访问：p=%d 超出范围\n", p);
                free(cd);
                return;
            }
            cd[--start] = (TreeNode[p].lchild == c) ? '0' : '1';
        }
        HC[i] = _strdup(&cd[start]); // 保存编码
        if (!HC[i]) {
            perror("内存分配失败");
            free(cd);
            exit(EXIT_FAILURE);
        }
    }

    free(cd); // 释放临时编码存储
}

void DecodeHuffmanTree(const char* codeFile, const char* textFile, HuffmanTree* TreeNode, int root) {
    FILE* in = fopen(codeFile, "r");
    FILE* out = fopen(textFile, "w");
    if (!in || !out) {
        perror("文件打开失败");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }

    int current = root;  // 从根节点开始译码
    char bit;
    while ((bit = fgetc(in)) != EOF) {
        if (bit == ' ') continue;  // 忽略空格
        if (bit == '0') {
            current = TreeNode[current].lchild;
        }
        else if (bit == '1') {
            current = TreeNode[current].rchild;
        }

        // 如果到达叶子节点，输出对应字符并回到根节点
        if (TreeNode[current].lchild == -1 && TreeNode[current].rchild == -1) {
            fputc(TreeNode[current].ch, out);
            current = root;
        }
    }

    printf("译码完成，结果已写入文件：%s\n", textFile);
    fclose(in);
    fclose(out);
}

void PrintCodeFile(const char* codeFile, const char* codePrintFile) {
    FILE* in = fopen(codeFile, "r");
    FILE* out = fopen(codePrintFile, "w");
    if (!in || !out) {
        perror("文件打开失败");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }

    int count = 0;
    char bit;
    printf("代码文件内容（每行50个）：\n");
    while ((bit = fgetc(in)) != EOF) {
        if (bit == ' ') continue;  // 忽略空格

        // 打印到终端
        printf("%c", bit);
        fputc(bit, out);  // 同时写入目标文件
        count++;

        if (count % 50 == 0) {
            printf("\n");
            fputc('\n', out);  // 每50个字符换行
        }
    }

    // 如果最后一行字符不足50个，换行
    if (count % 50 != 0) {
        printf("\n");
        fputc('\n', out);
    }

    printf("代码文件已写入文件：%s\n", codePrintFile);
    fclose(in);
    fclose(out);
}

void compare(const char* ToBeTran, const char* TextFile) {
    FILE* s1 = fopen(ToBeTran, "r"); // 打开第一个文件
    FILE* s2 = fopen(TextFile, "r"); // 打开第二个文件

    // 检查文件是否成功打开
    if (!s1 || !s2) {
        perror("文件打开失败");
        if (s1) fclose(s1); // 如果第一个文件已打开，关闭它
        if (s2) fclose(s2); // 如果第二个文件已打开，关闭它
        return;
    }

    bool areSame = true; // 假设文件内容相同
    int ch1, ch2; // 用于存储从文件中读取的字符

    // 逐字节比较两个文件的内容
    while (true) {
        ch1 = fgetc(s1); // 从第一个文件读取一个字符
        ch2 = fgetc(s2); // 从第二个文件读取一个字符

        // 比较字符是否相同
        if (ch1 != ch2) {
            areSame = false;
            break;
        }

        // 如果两个文件同时到达末尾，退出循环
        if (ch1 == EOF && ch2 == EOF) {
            break;
        }

        // 如果一个文件到达末尾而另一个文件没有，表示不相同
        if ((ch1 == EOF && ch2 != EOF) || (ch1 != EOF && ch2 == EOF)) {
            areSame = false;
            break;
        }
    }

    // 打印比较结果
    if (areSame) {
        printf("TextFile文件与ToBeTran文件的内容相同。\n");
    }
    else {
        printf("TextFile文件与ToBeTran文件的内容不同。\n");
    }

    // 关闭文件
    fclose(s1);
    fclose(s2);
}