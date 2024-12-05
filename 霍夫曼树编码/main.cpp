#include "HuffmanTree_Head.h"

int main() {
    HuffmanTree* TreeNode = NULL;
    int nodeNum = 0;

    while (1) {
        Menu();
        int choice;
        printf("��ѡ����: ");
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
            printf("�����ѽ��õĹ������������ļ�ToBeTran�е����Ľ��б��룬Ȼ�󽫽�������ļ�CodeFile�У�\n");
            if (TreeNode) {
                EncodeFile(TreeNode, nodeNum / 2 + 1);
            }
            else {
                printf("��������δ���أ����Դ��ļ� 'HfmTree.txt' ���ء�\n");
                ReadHuffmanTreeFromFile("HfmTree.txt", &TreeNode, &nodeNum);
                if (TreeNode) {
                    EncodeFile(TreeNode, nodeNum / 2 + 1);
                }
                else {
                    printf("�޷����ع������������ȳ�ʼ����\n");
                }
            }
            break;
        case 4: { 
                // ���� 4: ���� (Decoding)
            printf("�����ѽ��õĹ����������ļ�CodeFile�еĴ���������룬��������ļ�TextFile��\n");
            if (TreeNode) {
                DecodeHuffmanTree("CodeFile.txt", "TextFile.txt", TreeNode, nodeNum - 1); // ���ݸ��ڵ�����
            }
            else {
                ReadHuffmanTreeFromFile("HfmTree.txt", &TreeNode, &nodeNum);
                DecodeHuffmanTree("CodeFile.txt", "TextFile.txt", TreeNode, nodeNum - 1); 
            }
            break;
        }

        case 5: { // ���� 5: ӡ�����ļ� (Print)
            PrintCodeFile("CodeFile.txt", "CodePrin.txt");
            break;
        }
        case 0:
            printf("�˳�����\n");
            if (TreeNode) {
                free(TreeNode);
            }
            return 0;
        default:
            printf("��Чѡ������ԡ�\n");
        }
    }
}