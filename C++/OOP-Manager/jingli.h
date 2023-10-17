/*
经理类，继承自员工虚基类
*/
#pragma once
#include<iostream>
using namespace std;
#include"VirtualStaff.h"

class jingli : public VirtualStaff
{
private:
    /* data */
public:
    jingli(int id, string name, int did);
    //~Employee();
    //重写
    virtual void ShowStaff(); 
    virtual string ShowDutyName();
    virtual void ShowDutyComment();
};