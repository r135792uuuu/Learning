#include"Boss.h"

Boss::Boss(int id, string name, int did)
{
    m_Id = id;
    m_Name = name;
    m_Depid = did;
}

string Boss::ShowDutyName()
{
    return string("老板岗");
}

void Boss::ShowDutyComment()
{
    cout << "老板的职责是管理公司" << endl;
}

void Boss::ShowStaff()
{
    cout << "编号:" << m_Id << "\t"
        << "姓名" << m_Name << "\t"
        << "岗位编号" << m_Depid << "\t";
    cout << "岗位:  " ; 
    cout << this->ShowDutyName();
    cout << "\t岗位职责:  ";
    ShowDutyComment();
}