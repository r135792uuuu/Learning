#ifndef CUSTOMSTRUCT_HPP
#define CUSTOMSTRUCT_HPP
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

#define PI 3.14159

/**
 * @brief 二叉树结构体与相关接口
 * 
 */
typedef struct TreeNode {
	int treeNodeId;
	int dataInt;
    TreeNode* leftNode;
    TreeNode* rightNode;
} TreeNode;
// 二叉树初始化
TreeNode* initializeTree(int nodeNumbers);
// 递归释放二叉树所有节点和数组内存
void freeTree(TreeNode* treeArray, int nodeNumbers);
// 二叉树赋值
void assignTree(int id, TreeNode* nodeLeft, TreeNode* nodeRight, double dataDouble);
// 遍历二叉树
void traverseTree(TreeNode* node);


/**
 * @brief 动态数组
 * 
 */
typedef struct DynamicArray {
    int* array;
    int size;
} DynamicArray;
// 创建动态数组 
DynamicArray* createDynamicArray(int size);
// 释放动态数组
void freeDynamicArray(DynamicArray* dynamicArray);


#endif  // __CUSTOMSTRUCT_HPP__
