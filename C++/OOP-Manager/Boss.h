/*
老板类，继承自员工虚基类
*/
#pragma once
#include<iostream>
using namespace std;
#include"VirtualStaff.h"

class Boss : public VirtualStaff
{
private:
    /* data */
public:
    Boss(int id, string name, int did);
    //~Employee();
    //重写
    virtual void ShowStaff(); 
    virtual string ShowDutyName();
    virtual void ShowDutyComment();
};