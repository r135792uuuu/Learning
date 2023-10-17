//外部类声明
#include<string>
#include"ManagerSys.h"

ManagerSys::ManagerSys()
{
    //初始化人数和数组指针，添加了文件之后就换到里面去
    //this->m_NumStaff = 0;
    //this->m_EmpArray = NULL;
    //这个是后面需要往程序里面读文件信息才加入的
    /*读文件的三种情况：
    1.第一次使用，没有创建
    2.文件存在，后期添加
    3.清空文件*/
    ifstream ifs;
    ifs.open("FILENAME", ios::in);

    //初始化显示文件不存在或者错误提示
    if ( !ifs.is_open())
    {
        cout << "文件不存在" << endl;
        m_NumStaff = 0; //初始化人数
        m_FileIsEmpty = true; //文件空标志
        m_EmpArray = NULL; //初始化数组
        ifs.close();
        return;
    }
    //文件存在，但没有记录
    char ch;
    ifs >> ch;
    if (ifs.eof()) //读取文件结尾的标志，如果直接就是结尾说明没有内容
    {
        cout << "文件为空" << endl;
        m_NumStaff = 0; //初始化人数
        m_FileIsEmpty = true; //文件空标志
        m_EmpArray = NULL; //初始化数组
        ifs.close();
        return;        
    }
    //文件存在且保存文件数据
    m_FileIsEmpty = false;
    int num = this->getNum(); //获取磁盘中保存的文件中人数，赋值给了临时num保存人数
    cout << "职工人数为："  << num << endl;
    this->m_NumStaff = num;
    //开辟空间
    this->m_EmpArray = new VirtualStaff*[m_NumStaff];
    this->init_Emp();

    /*测试读取文件中的信息创建到内存
        for (int i = 0; i < m_NumStaff; i++)
    {
        cout << "ID :" << m_EmpArray[i]->m_Id << endl;
    }*/
}

ManagerSys::~ManagerSys() //因为增加了m_EmpArray需要结束后释放
{
    //先清除每一个数组中的元素，在清除整个数组
    if (this->m_EmpArray != NULL)
    {
        for (int i = 0; i < this->m_NumStaff; i++)
        {
            if (this->m_EmpArray[i] != NULL)
            {
                delete this->m_EmpArray[i];
            }
        }
        this->m_NumStaff = 0;
        delete[] this->m_EmpArray;//结束释放内存指针
        this->m_EmpArray = NULL;
    }
}

/**************************************** 文件员工初始化
参数：无
返回值：无
从保存的文件中读取信息创建对象。
文件中已经有信息了，需要读取出来创建对象
每次打开程序启动，需要把信息放到内存里面，这个函数实现此功能
****************************************/
void ManagerSys::init_Emp()
{
    ifstream ifs;
    ifs.open("FILENAME", ios::in);

    int id;
    string name;
    int did;

    int index = 0;
    while (ifs >> id && ifs >> name && ifs >> did)
    {
        VirtualStaff * staff  = NULL;
        //根据文件中读取的不同did创建不同对象
        switch (did)
        {
        case 1:
            staff = new Employee(id, name, did);
            break;
        case 2:
            staff = new jingli(id, name, did);
            break;
        case 3:
            staff = new Boss(id, name, did);
            break;
        default:
            break;
        }
        //指针放入数组进行维护
        this->m_EmpArray[index] = staff;
        index++;
    }
    if (m_NumStaff != index)
    {
        this->m_NumStaff = index;
        cout << "人数不匹配！默认选项重配！" << endl;
    }
    
    ifs.close();
}

/**************************************** 主菜单函数
参数：无
返回值：无
****************************************/
void ManagerSys::Main_Menu()
{
    cout << "欢迎使用职工管理系统!" << endl;
	cout << "请输入接下来的操作:" << endl;
    cout << "****0.退出****" << endl;
	cout << "****1.增加****" << endl;
	cout << "****2.显示****" << endl;		
	cout << "****3.删除****" << endl;
	cout << "****4.修改****" << endl;
	cout << "****5.查找****" << endl;
    cout << "****6.排序****" << endl;
    cout << "****7.清空****" << endl;
    cout << "注意中文此编码下要乱码";
    cout << endl;
}

/**************************************** 退出函数
参数：无
返回值：无
****************************************/
void ManagerSys::Exit()
{
    cout << "欢迎下次使用" << endl;
    system("pause");
    exit(0); //退出正常标志
}

/**************************************** 写为文件
参数：无
返回值：无
注意：三个指针要搞清楚：
m_EmpArray是对象里面指向二维数组的指针,这个数组的每个元素是一个对象
先把Add函数写完再写的，只是因为Add函数里面需要调用这个，所以放在前面
****************************************/
void ManagerSys::Save()
{
    ofstream ofs;
    ofs.open("FILENAME", ios::out);

    for (int i = 0; i < m_NumStaff; i++)
    {
        ofs << m_EmpArray[i]->m_Id << " "
            << m_EmpArray[i]->m_Name << " "
            << m_EmpArray[i]->m_Depid << " " << endl; 
    }
    cout << "成功保存" << endl;
    ofs.close();
}

/**************************************** 增加职工
参数：无
返回值：无
注意：三个指针要搞清楚：m_EmpArray是对象里面指向二维数组的指针
newspace是二维数组的指针   
staffTemp是newspace数组里面装载的指针，指向创建的每一个具体对象
****************************************/
void ManagerSys::AddStaff()
{
    using namespace std;
    int addNum = 0;
    cout << "\n" ;
    cout << "请输入要加入的员工个数:  " ;
    cin >> addNum;

    if (addNum > 0 && addNum < 50)
    {
        //计算新空间的大小：从0开始计算的
        int newSize = m_NumStaff + addNum;
        //每次开始程序需要重新在内存开辟空间
        VirtualStaff ** newspace = new VirtualStaff*[newSize];
        //后期再添加时，将原本的数据考进来
        if (m_EmpArray != NULL)
        {
            for (int i = 0; i < m_NumStaff; i++)
            {
                newspace[i] = m_EmpArray[i];
            }
        }
        //输入信息
        for (int i = 0; i < addNum; i++)
        {
            int id;
            string name;
            int dutyselect;
            cout << "输入第" << i + 1 << "个员工编号： " ;
            cin >> id;
            cout << endl;
            cout << "输入第" << i + 1 << "个员工姓名: " << endl;
            cin >> name;
            cout << endl;
            cout << "选择第" << i + 1 << "个员工岗位: " 
            << "(1.员工 2.经理 3.老板)" << endl;
            cin >> dutyselect;
            cout << endl;
            //创建对象
            VirtualStaff * staffTemp;
            switch (dutyselect)
            {
            case 1:
                staffTemp = new Employee(id, name, dutyselect);
                break;
            case 2:
                staffTemp = new jingli(id, name, dutyselect);
                break;
            case 3:
                staffTemp = new Boss(id, name, dutyselect);
                break;
            default:
                {
                    cout << "没有选项" << endl;
                    staffTemp = new Employee(id, name, dutyselect);
                }
                break;
            }
            //把这些个指针对象放到二维数组里面
            newspace[m_NumStaff + i] = staffTemp;
        }
        //释放原有空间
        delete[] this->m_EmpArray;
        //更改新空间指向
        this->m_EmpArray = newspace;
        //更新个数
        this->m_NumStaff = m_NumStaff + addNum;
        //修改文件内容标志
        m_FileIsEmpty = false;
        //提示信息
        cout << "成功添加" << addNum << "个员工" << endl;
        //保存
        Save();  
    }
    else
    {
        cout << "请输入正确人数!" << endl;
    }
    system("cls");
    system("pause");
}

/**************************************** 从文件中获得员工人数
参数：无
返回值：num：读取的文件中的人数
目的是在打开程序运行的同时或得文件中的人数
类中的numStaff是运行时产生的临时数据，重新启动是没有的
打开文件读取，因为我们前面是每一个人的信息放在同一行
所以一行一行的读，读取是一个固定语法见下面while
****************************************/
int ManagerSys::getNum()
{
    ifstream ifs;
    ifs.open("FILENAME", ios::in);
    string str;
    int num = 0; //人数
    //while (ifs >> id && ifs >> name && ifs >> did)
    //{//一行一行的读,读完了就会退出num++;}  这种语法无法进入循环
    for ( int i = 0; !ifs.eof(); i++)
    {
        getline(ifs, str); //碰到换行才退出识别文件
        num = i;
    }
    ifs.close();
    return num;
}

/**************************************** 显示信息功能
参数：无
返回值：无
通过初始化已经把文件中的内容放在堆区内存开辟的数组中了
直接读取数组中的信息，通过多态调用接口
****************************************/
void ManagerSys::showEmp()
{
    //先判断文件是否为空
    if ( m_FileIsEmpty )
    {
        cout << "文件不存在" << endl;
    }
    else
    {
        //利用多态调用接口
        for (int i = 0; i < m_NumStaff; i++)
        {
            this->m_EmpArray[i]->ShowStaff();
        }
    }
    cout << "\n" << endl;
    return;
    system("pause");

}

/**************************************** 员工存在函数
参数：id：传入的查询员工编号
返回值：index：被查询员工在数组中的位置
此函数显示员工是否存在，不存在返回-1，存在返回员工在数组的位置
也是后续系列功能实现的基础，都需要在发现某个人存在的基础之上才能修改、删除等
****************************************/
int ManagerSys::IsExist(int id)
{
    int index = -1;
    //核心是遍历
    for (int i = 0; i < m_NumStaff; i++)
    {
        if (id == this->m_EmpArray[i]->m_Id)
        {
            index = id;
            break;
        }
    }
    return index;
}

/**************************************** 删除员工
参数：
返回值：
对于保存在数组里面的内容，要做删除
也就是把删除成员之后的成员依次全部前移
****************************************/
void ManagerSys::DeleteStaff()
{
    int id = 0;
    int index = 0;
    if (this->m_FileIsEmpty)
    {
        cout << "记录为空" << endl;
    }
    else
    {
        cout << "请输入要删除的员工的编号" << endl;
        cin >> id;
        index = this->IsExist(id);

        if (index != -1) //说明职工存在且位置在index处
        {
            //数据前移，数组的是从0开始计数，NumStaff是从1开始需要减1
            for (int i = index; i < this->m_NumStaff - 1; i++)
            {
                this->m_EmpArray[i] = this->m_EmpArray[i + 1];
            }
            this->m_NumStaff--; //人数少了
            //同步更新到文件中
            this->Save();
            cout << "删除成功！" << endl;
        }
        else
        {
            cout << "没有找到这个职工" << endl;
        }
    }
    system("pause");
    system("cls");
}

/**************************************** 修改员工
参数：
返回值：
修改的逻辑和删除一样，查找到结果之后进行修改
第二种方法就是直接修改数组，重新创建对象，然后替换掉数组原来的对象成员
****************************************/
void ManagerSys::ModifyStaff()
{
    int id = 0, index = 0, choose = 0, option = 1;
    string name;
    int did = 0;

    if ( !this->m_FileIsEmpty )
    {
        this->showEmp();
        cout << "输入想要修改的员工编号" << endl;
        cin >> id;
        index = this->IsExist(id);
        if (index = -1)
        {
            cout << "没有找到这个人" << endl;
        }
        else
        {
            for (int i = index; option = 1; i++)
            {
                if ( i == index+1)
                {
                    this->showEmp();
                }
                cout << "您想要修改哪一个：1.编号 2.姓名 3.岗位编号: ";
                cin >> choose;
                switch (choose)
                {
                    case 1:
                        {
                            cout << "编号修改为： ";
                            this->m_EmpArray[i]->m_Id = index;
                        }
                        break;
                    case 2:
                        {
                            cout << "职位编号修改为： " ;
                            cin >> did;
                            this->m_EmpArray[i]->m_Depid = did;
                        }
                        break;
                    case 3:
                        {
                            cout << "姓名修改为： " ;
                            cin >> name;
                            this->m_EmpArray[i]->m_Name = name;
                        }
                        break;
                    default:
                        break;
                }
                cout << "修改成功,是否继续？ 1.继续 2.退出" << endl;
                cin >> option;
                if (option == 1)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            
            this->Save();
        }
    }
    else
    {
        cout << "文件内容为空" << endl;
    }
    system("cls");
    return;
    system("pause");
}

/**************************************** 查找员工
参数：
返回值：

****************************************/
void ManagerSys::FindStaff()
{
    int id = 0;
    int choose = 1;
    string name;
    if (this->m_FileIsEmpty)
    {
        cout << "记录为空" << endl;
    }
    else
    {
        cout << "请输入要查找的方式 1.编号查询  2.姓名查询 " << endl;
        cin >> choose;
        if ( choose == 1)
        {
            cout << "请输入要查找的员工的编号：";
            cin >> id;
            //避免再次遍历浪费内存
            int ret = this->IsExist(id);
                if (ret != -1)
                {
                    this->m_EmpArray[ret]->ShowStaff();
                }
                else
                { cout << "查无此人" << endl;}
        }
        else
        {
            cout << "请输入要查找的员工的姓名：";
            cin >> name;
            for (int i = 0; i < this->m_NumStaff; i++)
            {
                if (this->m_EmpArray[i]->m_Name == name)
                {
                    this->m_EmpArray[i]->ShowStaff();
                    break;
                }
                else
                { cout << "查无此人" << endl;}
            }
        }
    }
    return;
    system("pause");
}

/**************************************** 清空文件
参数：
返回值：
实现原理就是用ios：：trunc的方式打开文件，如果文件存在就删除并重新创建
****************************************/
void ManagerSys::Clean()
{
    int choose;
    cout << "确认清空? 1.确认 2.返回" << endl;
    cin >> choose;
    if (choose == 1)
    {
        ofstream ofs;
        ofs.open("FILENAME", ios::trunc);
        ofs.close();
        //先清理数组中每个对象再清理维护的数组
        if (this->m_EmpArray != NULL)
        {
            for (int i = 0; i < this->m_NumStaff; i++)
            {
                delete this->m_EmpArray[i];
            }
            this->m_NumStaff = 0;
            delete[] this->m_EmpArray;
            this->m_EmpArray = NULL;
            this->m_FileIsEmpty = true;
        }
        cout << "清除成功" << endl;
    }
    else
    {
        return;
    }
    system("cls");
}
