#include"Employee.h"

Employee::Employee(int id, string name, int did)
{
    m_Id = id;
    m_Name = name;
    m_Depid = did;
}

//Employee::~Employee()
//{
 //  cout << "释放员工对象" << endl;
//}

string Employee::ShowDutyName()
{
    return string("员工岗");
}

void Employee::ShowDutyComment()
{
    cout << "员工的职责是完成经理的任务" << endl;
}

void Employee::ShowStaff()
{
    cout << "编号:" << m_Id << "\t"
        << "姓名" << m_Name << "\t"
        << "岗位编号" << m_Depid << "\t";
    cout << "岗位:  " ; 
    cout << this->ShowDutyName();
    cout << "\t岗位职责:  ";
    ShowDutyComment();
}
