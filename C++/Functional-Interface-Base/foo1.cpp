#include "foo1.hpp"
#include <iostream>

// 类的构造函数实现
Example::Example() {
    number = 42;
}

// 类的成员函数实现
void Example::printMessage() {
    std::cout << "This is an example message. Number: " << number << std::endl;
}
