/*
要求：公司职工分三类，普通员工经理老板，显示信息需要显示职工编号、姓名、职责、岗位
普通员工职责：完成经理下发的任务 ；经理职责：完成老板交给的任务并且下发员工 ；老板：管理公司事务
管理系统功能：
退出管理程序、显示信息、增加信息（批量添加，也可以将信息录入文件中）、按照编号删除、按照编号修改、按照编号姓名查找、按照编号排序、清空
面向对象，所以主菜单也要包装成对象，职工管理类
！！！及得在c_cpp_proper里面加入文件夹
*/

#include<iostream>
using namespace std;
#include"ManagerSys.h"
/*#include"Employee.h"
#include"Boss.h"
#include"jingli.h"*/
#include"VirtualStaff.h"

/*
创建对象数组，但是不同的对象类型不同，二数组只能接收类型相同的数据
所以用双重指针来维护，也就是将所有所有员工的指针放到一个数组里来维护
比如一个公司三个人，数组里面转三个指针，每个只指针指向员工对象
正好可以用多态的父类指针指向子类对象，数组里面保存父类指针
数组开辟在堆区（它的周期比较长），所有返回值就是双重指针
虚基类对象** 指针名字= new 虚基类对象*[个数]；
*/


int main()
{
    //定义各参数
    int user_option_menu; //用户主界面选择参数

/*   //测试代码
    Employee * ep1 = new Employee(1, "唐海英", 3);
    cout << ep1->ShowDutyName() << endl;
    delete ep1;

*/

    //实例化管理类对象
    ManagerSys ms;
    while (true)
    {
        ms.Main_Menu();        //调用展示菜单函数
        cout << "请输入下一步操作" << endl;
        cin >> user_option_menu;
        switch (user_option_menu)   //选择功能
        {
        case 0:
            ms.Exit(); //退出
            break;
        case 1:
            ms.AddStaff(); //增加员工
            break;
        case 2:
            ms.showEmp(); //显示员工
            break;
        case 3:
            ms.DeleteStaff(); //删除
            break;
        case 4:
            ms.ModifyStaff(); //修改
            break;  
        case 5:
            ms.FindStaff(); //
            break;  
        case 6:
            ms.Clean(); //
            break;
        default:
            break;
        }    
    }


    system("pause");
    return 0;
}