/*
抽象员工类：包含员工信息
采用多态的方式
*/
#pragma once //防止重复定义报错：redefination of “class”
#include<iostream>
#include<string>
using namespace std;

//抽象员工基类
class VirtualStaff
{
private:
    /* data */
public:
    //VirtualStaff(/* args */);
    //要虚构函数必须虚函数，不然报错
    //virtual ~VirtualStaff();
    virtual void ShowStaff() = 0; //显示个人信息
    virtual string ShowDutyName() = 0 ; //获取岗位名称
    virtual void ShowDutyComment() = 0; //显示岗位职责
public:
    int m_Id; //编号
    string m_Name; //姓名
    int m_Depid; //部门编号
};

//VirtualStaff::VirtualStaff(/* args */)
//{
//}

//VirtualStaff::~VirtualStaff()
//{
//    cout << "释放员工类" << endl; //必须要有语法不然报错
//}
