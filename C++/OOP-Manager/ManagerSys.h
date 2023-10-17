/*
职工管理类：实现功能
与用户的交互
对职工功能选择的分类
与文件的交互读写
*/
#pragma once
#include<iostream>
#include<string>
using namespace std;
#include"Employee.h"
#include"Boss.h"
#include"jingli.h"
#include<fstream> //文件流
#define FILENAME "FILENAME.txt" //保存文件宏定义，后面方便调用

//管理界面类
class ManagerSys
{
private:
    /* data */
public:
    ManagerSys(/* args */);
    ~ManagerSys();
    void Main_Menu(); //主菜单
    void Exit(); //退出
    void AddStaff(); //增加
    void showEmp(); //显示员工函数
    void DeleteStaff(); //删除员工
    void ModifyStaff(); //修改
    void FindStaff(); //查找员工
    void Clean(); //清空文件
public:
    void Save(); //读写文件
    int getNum(); //获取文件中统计的人数
    void init_Emp(); //初始化员工
    int IsExist(int id); //员工查找基础函数
public:
    int m_NumStaff;//人数
    VirtualStaff ** m_EmpArray; //在内存中保存对象的指针，指向数组
    bool m_FileIsEmpty; //检查保存信息的文件是否为空
    
};
