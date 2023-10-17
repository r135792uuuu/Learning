// 指向自身（嵌套自己类型的结构体）**主要是用来设计一种具有有向指向的数据结构。
// **相当于就是实现了一个**双向链表**。
// 相当于手动创建列表，**比如可以用在栅格地图上，创建的栅格地图对象**
// 某个栅格地图可以指向下一个和上一个栅格地图，相当于有向链表，可以用来搜索。
#include <iostream>
#include <cstdio>

struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
};

void traverseTree(struct TreeNode* node) {
    if (node == NULL)
        return;
    printf("%d ", node->data);
    traverseTree(node->left);
    traverseTree(node->right);
}

int main() {
    struct TreeNode node1, node2, node3, node4, node5;
    node1.data = 1;
    node2.data = 2;
    node3.data = 3;
    node4.data = 4;
    node5.data = 5;

    // 设置树的结构
    node1.left = &node2;
    node1.right = &node3;
    node2.left = &node4;
    node2.right = NULL;
    node3.left = NULL; // 将 node3 的右子树连接到 node5 而不是 node4
    node3.right = &node5; // 新增此行，将 node3 的右子树连接到 node5
    node4.left = NULL;
    node4.right = NULL;
    node5.left = NULL;
    node5.right = NULL;

    traverseTree(&node1);
    return 0;
}
