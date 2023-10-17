#include"jingli.h"

jingli::jingli(int id, string name, int did)
{
    this->m_Id = id;
    m_Name = name;
    m_Depid = did;
}

string jingli::ShowDutyName()
{
    return string("经理岗");
}

void jingli::ShowDutyComment()
{
    cout << "经理的职责是完成老板的任务并且发布给员工" << endl;
}

void jingli::ShowStaff()
{
    cout << "编号:" << m_Id << "\t"
        << "姓名" << m_Name << "\t"
        << "岗位编号" << m_Depid << "\t";
    cout << "岗位:  " ; 
    cout << this->ShowDutyName();
    cout << "\t岗位职责:  ";
    ShowDutyComment();
}