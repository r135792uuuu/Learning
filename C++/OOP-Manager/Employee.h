/*
普通员工类，继承自员工虚基类
*/
#pragma once
#include<iostream>
using namespace std;
#include"VirtualStaff.h"

class Employee : public VirtualStaff
{
private:
    /* data */
public:
    Employee(int id, string name, int did);
    //~Employee();
    //重写
    virtual void ShowStaff(); 
    virtual string ShowDutyName();
    virtual void ShowDutyComment();
};

