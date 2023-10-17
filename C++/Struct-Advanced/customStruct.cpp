#include "customStruct.hpp"

/**
 * @brief 二叉树初始化和分配二叉树的容量
 * 
 * @param nodeNumbers 树的节点
 * @return TreeNode* 
 */
TreeNode* initializeTree(int nodeNumbers) {
    // 分配内存以存储 numNodes 个 TreeNode 结构体
    // C语言风格 malloc
    TreeNode* treeArray = (TreeNode*)malloc(sizeof(TreeNode) * nodeNumbers);
    // 检查内存分配是否成功
    if (treeArray == NULL) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    // 初始化每个结构体
    for (int i = 0; i < nodeNumbers; i++) {
        treeArray[i].treeNodeId = 0;
        treeArray[i].dataInt = 0;
        treeArray[i].leftNode = nullptr;
        treeArray[i].rightNode = nullptr;
    }
    return treeArray; // 返回指向创建的结构体数组的指针，指针里面每一个元素都是TreeNode实例
}

/**
 * @brief 递归释放二叉树所有节点和数组内存
 * 
 * @param treeArray 
 * @param nodeNumbers 
 */
void freeTree(TreeNode* treeArray, int nodeNumbers) {
    if (treeArray == NULL) {
        return; // 如果 treeArray 为 NULL，不需要进一步释放
    }

    for (int i = 0; i < nodeNumbers; i++) {
        TreeNode* currentNode = &treeArray[i];

        // 递归释放左子树
        freeTree(currentNode->leftNode, 1);

        // 递归释放右子树
        freeTree(currentNode->rightNode, 1);
    }

    free(treeArray); // 释放结构体数组内存
    printf("释放完毕");
}


/**
 * @brief 遍历二叉树
 * 
 * @param node 
 */
void traverseTree(TreeNode* node) {
    if (node == NULL) {
        printf("该节点的下级指针为空.\n");
        return;
    }
    printf("树的节点序号是：%d\n",node->treeNodeId);
    printf("树节点的数据是 ：%d\n", node->dataInt);
    traverseTree(node->leftNode);
    traverseTree(node->rightNode);
}


/**
 * @brief 创建动态数组
 * 
 * @param size 
 * @return DynamicArray* 
 */
DynamicArray* createDynamicArray(int size) {
    DynamicArray* dynamicArray = (struct DynamicArray*)malloc(sizeof(struct DynamicArray));
    if (dynamicArray == NULL) {
        // 处理内存分配失败的情况
        return NULL;
    }
    dynamicArray->array = (int*)malloc(sizeof(int) * size);
    if (dynamicArray->array == NULL) {
        // 处理内存分配失败的情况
        free(dynamicArray);
        return NULL;
    }
    dynamicArray->size = size;
    return dynamicArray;
}

/**
 * @brief 释放动态数组
 * 
 * @param dynamicArray 
 */
void freeDynamicArray(DynamicArray* dynamicArray) {
    if (dynamicArray != NULL) {
        free(dynamicArray->array);
        free(dynamicArray);
    }
}


