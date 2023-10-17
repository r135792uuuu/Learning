// 把同一种类型的函数用typedef模板包装，放入结构体中，可以统一管理（我目前没有碰到过）
// 在这个例子中，我们定义了一个包含函数指针数组的结构体MathOperations。
// 数组中的每个元素都是一个函数指针，指向不同的数学操作函数。
// 我们将加法、减法和乘法函数分别分配给数组中的元素，并通过遍历数组来调用不同的数学操作。
// 通过一个数组指针结构体转同类型的运算代码

#include <iostream>
#include <cstdio>

using namespace std;

typedef int (*MathOperation)(int, int);

struct MathOperations
{
    MathOperation operations[3];
};

int add(int a, int b)
{
    return a + b;
}

int subtract(int a, int b)
{
    return a - b;
}

int multiply(int a, int b)
{
    return a * b;
}

int main()
{
    struct MathOperations math;
    math.operations[0] = add;
    math.operations[1] = subtract;
    math.operations[2] = multiply;
    int a = 5;
    int b = 3;
    for (int i = 0; i < 3; i++) {
        int result = math.operations[i](a, b);
        cout << "Operation " << i + 1 << " result: " << result << endl;
    }
    return 0;
}
