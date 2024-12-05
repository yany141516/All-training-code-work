#include "HuffmanTree_Head.h"

int main() {
    HuffmanTree* TreeNode = NULL;
    int nodeNum = 0;

    while (1) {
        Menu();
        int choice;
        printf("请选择功能: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            InitializeHuffmanTree(&TreeNode, &nodeNum);
            break;
        case 2:
            if (TreeNode) {
                PrintHuffmanTreeMenu(TreeNode, nodeNum);
                PrintHuffmanTree(TreeNode, nodeNum);
                PrintHuffmanCodes(TreeNode, nodeNum);
            }
            else {
                ReadHuffmanTreeFromFile("HfmTree.txt", &TreeNode, &nodeNum);
                PrintHuffmanTreeMenu(TreeNode, nodeNum);
                PrintHuffmanCodes(TreeNode, nodeNum);
            }
            break;
        case 3:
            printf("利用已建好的哈夫曼树，对文件ToBeTran中的正文进行编码，然后将结果存入文件CodeFile中；\n");
            if (TreeNode) {
                EncodeFile(TreeNode, nodeNum / 2 + 1);
            }
            else {
                printf("哈夫曼树未加载，尝试从文件 'HfmTree.txt' 加载。\n");
                ReadHuffmanTreeFromFile("HfmTree.txt", &TreeNode, &nodeNum);
                if (TreeNode) {
                    EncodeFile(TreeNode, nodeNum / 2 + 1);
                }
                else {
                    printf("无法加载哈夫曼树，请先初始化。\n");
                }
            }
            break;
        case 4: { 
                // 功能 4: 译码 (Decoding)
            printf("利用已建好的哈夫曼树将文件CodeFile中的代码进行译码，结果存入文件TextFile中\n");
            if (TreeNode) {
                DecodeHuffmanTree("CodeFile.txt", "TextFile.txt", TreeNode, nodeNum - 1); // 传递根节点索引
            }
            else {
                ReadHuffmanTreeFromFile("HfmTree.txt", &TreeNode, &nodeNum);
                DecodeHuffmanTree("CodeFile.txt", "TextFile.txt", TreeNode, nodeNum - 1); 
            }
            break;
        }

        case 5: { // 功能 5: 印代码文件 (Print)
            PrintCodeFile("CodeFile.txt", "CodePrin.txt");
            break;
        }
        case 0:
            printf("退出程序。\n");
            if (TreeNode) {
                free(TreeNode);
            }
            return 0;
        default:
            printf("无效选项，请重试。\n");
        }
    }
}