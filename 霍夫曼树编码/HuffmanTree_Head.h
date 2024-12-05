#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define MAX 1000
#define BlankFlag -1

typedef struct {
    int weight;
    char ch;
    int parent;
    int lchild;
    int rchild;
} HuffmanTree;

typedef struct {
    char c;
    int w;
} Charstat;

/**
 * @brief 构建哈夫曼树
 * @param cStat 输入的字符及其权重数组
 * @param TreeNode 存储哈夫曼树的节点数组
 * @param codeNum 字符的数量
 *
 * 根据字符的权重构建哈夫曼树，填充节点数组 TreeNode。
 */
void CreatHuffmanTree(Charstat* cStat, HuffmanTree* TreeNode, int codeNum);

/**
 * @brief 保存哈夫曼树到文件
 * @param TreeNode 哈夫曼树的节点数组
 * @param nodeNum 哈夫曼树的节点总数
 * @param outputFile 保存文件的路径
 *
 * 将哈夫曼树的结构保存到指定的文件中，便于后续加载或调试。
 */
void SaveHuffmanTree(HuffmanTree* TreeNode, int nodeNum, const char* outputFile);

/**
 * @brief 生成哈夫曼编码
 * @param codeNum 字符的数量（叶子节点数）
 * @param TreeNode 哈夫曼树的节点数组
 * @param HC 存储生成的哈夫曼编码数组
 *
 * 根据哈夫曼树为每个字符生成唯一的二进制编码，并存储在 HC 中。
 */
void Encoding(int codeNum, HuffmanTree* TreeNode, char** HC);

/**
 * @brief 打印哈夫曼树的可视化结构
 * @param TreeNode 哈夫曼树的节点数组
 * @param index 当前节点的索引
 * @param depth 当前节点所在的深度
 * @param edge 边标记，用于表示左或右子树
 *
 * 以图形化方式打印哈夫曼树的结构，便于直观查看树的布局。
 */
void PrintHuffmanTreeVisual(HuffmanTree* TreeNode, int index, int depth, char edge);

/**
 * @brief 打印哈夫曼树的文本表示
 * @param TreeNode 哈夫曼树的节点数组
 * @param nodeNum 哈夫曼树的节点总数
 *
 * 打印哈夫曼树的节点信息，包括字符、权重、左右子节点和父节点。
 */
void PrintHuffmanTree(HuffmanTree* TreeNode, int nodeNum);

/**
 * @brief 从文件中读取哈夫曼树
 * @param inputFile 输入文件路径
 * @param TreeNode 指向存储哈夫曼树节点数组的指针
 * @param nodeNum 存储节点总数的指针
 *
 * 从指定文件中加载哈夫曼树的节点结构，填充 TreeNode 和节点数量 nodeNum。
 */
void ReadHuffmanTreeFromFile(const char* inputFile, HuffmanTree** TreeNode, int* nodeNum);

/**
 * @brief 对文件内容进行哈夫曼编码
 * @param TreeNode 哈夫曼树的节点数组
 * @param codeNum 字符的数量（叶子节点数）
 *
 * 使用哈夫曼树对文件 ToBeTran.txt 进行编码，并将结果保存到 CodeFile.txt。
 */
void EncodeFile(HuffmanTree* TreeNode, int codeNum);

/**
 * @brief 解码哈夫曼编码文件
 * @param codeFile 输入的编码文件路径
 * @param textFile 输出的解码文本文件路径
 * @param TreeNode 哈夫曼树的节点数组
 * @param root 哈夫曼树的根节点索引
 *
 * 使用哈夫曼树对编码文件进行解码，还原为原始文本内容。
 */
void DecodeHuffmanTree(const char* codeFile, const char* textFile, HuffmanTree* TreeNode, int root);

/**
 * @brief 紧凑打印编码文件
 * @param codeFile 输入的编码文件路径
 * @param codePrintFile 输出的紧凑打印文件路径
 *
 * 将编码文件内容以每行 50 个字符的格式打印到终端，同时保存到指定文件。
 */
void PrintCodeFile(const char* codeFile, const char* codePrintFile);

/**
 * @brief 打印字符的哈夫曼编码
 * @param TreeNode 哈夫曼树的节点数组
 * @param codeNum 字符的数量（叶子节点数）
 *
 * 打印每个字符对应的哈夫曼编码，显示编码映射关系。
 */
void PrintHuffmanCodes(HuffmanTree* TreeNode, int codeNum);

/**
 * @brief 比较两个文件的内容是否相同
 * @param ToBeTran 第一个文件路径
 * @param TestFile 第二个文件路径
 *
 * 比较两个文件的内容是否逐字节相同，并打印结果。
 */
void compare(const char* ToBeTran, const char* TestFile);

/**
 * @brief 显示主菜单
 *
 * 打印哈夫曼树程序的功能菜单，供用户选择操作。
 */
void Menu();

/**
 * @brief 初始化哈夫曼树
 * @param TreeNode 指向存储哈夫曼树节点数组的指针
 * @param nodeNum 存储节点总数的指针
 *
 * 用户输入字符及其权重，构建哈夫曼树并保存到文件。
 */
void InitializeHuffmanTree(HuffmanTree** TreeNode, int* nodeNum);

/**
 * @brief 打印哈夫曼树的菜单功能
 * @param TreeNode 哈夫曼树的节点数组
 * @param nodeNum 哈夫曼树的节点总数
 *
 * 打印哈夫曼树的结构，并将其保存到文件。
 */
void PrintHuffmanTreeMenu(HuffmanTree* TreeNode, int nodeNum);

