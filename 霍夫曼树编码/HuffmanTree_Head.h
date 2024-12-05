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
 * @brief ������������
 * @param cStat ������ַ�����Ȩ������
 * @param TreeNode �洢���������Ľڵ�����
 * @param codeNum �ַ�������
 *
 * �����ַ���Ȩ�ع����������������ڵ����� TreeNode��
 */
void CreatHuffmanTree(Charstat* cStat, HuffmanTree* TreeNode, int codeNum);

/**
 * @brief ��������������ļ�
 * @param TreeNode ���������Ľڵ�����
 * @param nodeNum ���������Ľڵ�����
 * @param outputFile �����ļ���·��
 *
 * �����������Ľṹ���浽ָ�����ļ��У����ں������ػ���ԡ�
 */
void SaveHuffmanTree(HuffmanTree* TreeNode, int nodeNum, const char* outputFile);

/**
 * @brief ���ɹ���������
 * @param codeNum �ַ���������Ҷ�ӽڵ�����
 * @param TreeNode ���������Ľڵ�����
 * @param HC �洢���ɵĹ�������������
 *
 * ���ݹ�������Ϊÿ���ַ�����Ψһ�Ķ����Ʊ��룬���洢�� HC �С�
 */
void Encoding(int codeNum, HuffmanTree* TreeNode, char** HC);

/**
 * @brief ��ӡ���������Ŀ��ӻ��ṹ
 * @param TreeNode ���������Ľڵ�����
 * @param index ��ǰ�ڵ������
 * @param depth ��ǰ�ڵ����ڵ����
 * @param edge �߱�ǣ����ڱ�ʾ���������
 *
 * ��ͼ�λ���ʽ��ӡ���������Ľṹ������ֱ�۲鿴���Ĳ��֡�
 */
void PrintHuffmanTreeVisual(HuffmanTree* TreeNode, int index, int depth, char edge);

/**
 * @brief ��ӡ�����������ı���ʾ
 * @param TreeNode ���������Ľڵ�����
 * @param nodeNum ���������Ľڵ�����
 *
 * ��ӡ���������Ľڵ���Ϣ�������ַ���Ȩ�ء������ӽڵ�͸��ڵ㡣
 */
void PrintHuffmanTree(HuffmanTree* TreeNode, int nodeNum);

/**
 * @brief ���ļ��ж�ȡ��������
 * @param inputFile �����ļ�·��
 * @param TreeNode ָ��洢���������ڵ������ָ��
 * @param nodeNum �洢�ڵ�������ָ��
 *
 * ��ָ���ļ��м��ع��������Ľڵ�ṹ����� TreeNode �ͽڵ����� nodeNum��
 */
void ReadHuffmanTreeFromFile(const char* inputFile, HuffmanTree** TreeNode, int* nodeNum);

/**
 * @brief ���ļ����ݽ��й���������
 * @param TreeNode ���������Ľڵ�����
 * @param codeNum �ַ���������Ҷ�ӽڵ�����
 *
 * ʹ�ù����������ļ� ToBeTran.txt ���б��룬����������浽 CodeFile.txt��
 */
void EncodeFile(HuffmanTree* TreeNode, int codeNum);

/**
 * @brief ��������������ļ�
 * @param codeFile ����ı����ļ�·��
 * @param textFile ����Ľ����ı��ļ�·��
 * @param TreeNode ���������Ľڵ�����
 * @param root ���������ĸ��ڵ�����
 *
 * ʹ�ù��������Ա����ļ����н��룬��ԭΪԭʼ�ı����ݡ�
 */
void DecodeHuffmanTree(const char* codeFile, const char* textFile, HuffmanTree* TreeNode, int root);

/**
 * @brief ���մ�ӡ�����ļ�
 * @param codeFile ����ı����ļ�·��
 * @param codePrintFile ����Ľ��մ�ӡ�ļ�·��
 *
 * �������ļ�������ÿ�� 50 ���ַ��ĸ�ʽ��ӡ���նˣ�ͬʱ���浽ָ���ļ���
 */
void PrintCodeFile(const char* codeFile, const char* codePrintFile);

/**
 * @brief ��ӡ�ַ��Ĺ���������
 * @param TreeNode ���������Ľڵ�����
 * @param codeNum �ַ���������Ҷ�ӽڵ�����
 *
 * ��ӡÿ���ַ���Ӧ�Ĺ��������룬��ʾ����ӳ���ϵ��
 */
void PrintHuffmanCodes(HuffmanTree* TreeNode, int codeNum);

/**
 * @brief �Ƚ������ļ��������Ƿ���ͬ
 * @param ToBeTran ��һ���ļ�·��
 * @param TestFile �ڶ����ļ�·��
 *
 * �Ƚ������ļ��������Ƿ����ֽ���ͬ������ӡ�����
 */
void compare(const char* ToBeTran, const char* TestFile);

/**
 * @brief ��ʾ���˵�
 *
 * ��ӡ������������Ĺ��ܲ˵������û�ѡ�������
 */
void Menu();

/**
 * @brief ��ʼ����������
 * @param TreeNode ָ��洢���������ڵ������ָ��
 * @param nodeNum �洢�ڵ�������ָ��
 *
 * �û������ַ�����Ȩ�أ������������������浽�ļ���
 */
void InitializeHuffmanTree(HuffmanTree** TreeNode, int* nodeNum);

/**
 * @brief ��ӡ���������Ĳ˵�����
 * @param TreeNode ���������Ľڵ�����
 * @param nodeNum ���������Ľڵ�����
 *
 * ��ӡ���������Ľṹ�������䱣�浽�ļ���
 */
void PrintHuffmanTreeMenu(HuffmanTree* TreeNode, int nodeNum);

