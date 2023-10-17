// 在C语言中，结构体中的数组成员必须具有固定大小，不能实现灵活的数组成员。
// 以下是一个重新调整的例子，展示了如何使用指针实现动态大小的数组。

// 不过**如果是C++11以上的版本，可以直接使用STL容器，比如vector，list等动态容器。**

#include <stdio.h>
#include <stdlib.h>

struct DynamicArray {
    int* array;
    int size;
};

struct DynamicArray* createDynamicArray(int size) {
    struct DynamicArray* dynamicArray = (struct DynamicArray*)malloc(sizeof(struct DynamicArray));
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

void freeDynamicArray(struct DynamicArray* dynamicArray) {
    if (dynamicArray != NULL) {
        free(dynamicArray->array);
        free(dynamicArray);
    }
}

int main() {
    int size = 5;
    struct DynamicArray* myArray = createDynamicArray(size);
    if (myArray == NULL) {
        printf("Dynamic array creation failed.\n");
        return 1;
    }

    for (int i = 0; i < myArray->size; i++) {
        myArray->array[i] = i + 1;
    }

    printf("Array elements: ");
    for (int i = 0; i < myArray->size; i++) {
        printf("%d ", myArray->array[i]);
    }
    printf("\n");

    freeDynamicArray(myArray);
    return 0;
}
