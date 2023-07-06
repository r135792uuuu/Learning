    // ִ������������ get_imgһֱִ�У��յ� flag֮��ֹͣ 
	bool service_two::get_img(std_srvs::Empty::Request  &req,std_srvs::Empty::Response &res)
    {
        int cnt = 0;
        while (1)
        {
          if (cnt > 399) {
            break;
          }      
          cnt +=1;
          std::cout<<"while photo_flag:"<<cnt<<std::endl;      
          sleep(1);
          if(photo_flag==false)
              break;
        }
      return true;
    }
    
    // ͨ��set_flag����flag������get_img ��ִ�� 
    bool service_two::set_flag(std_srvs::SetBool::Request  &req,std_srvs::SetBool::Response &res)
    {
      if (req.data){
        photo_flag=true;
      }else{
        photo_flag=false;
      }
      std::cout<<"photo_flag:"<<photo_flag<<std::endl;
      return true;
    }
    
    // ����ROS���߳�
	#include "ros/ros.h"
    #include "std_srvs/Empty.h"
    #include "std_srvs/SetBool.h"
     
    using namespace std;
    class service_two
    {
        public:
        service_two();
        bool photo_flag;
        ros::ServiceServer service_once;
        ros::ServiceServer service_on;
        private:
        bool get_img(std_srvs::Empty::Request  &req,std_srvs::Empty::Response &res);
        bool set_flag(std_srvs::SetBool::Request  &req,std_srvs::SetBool::Response &res);
    };
    service_two::service_two()
    {
        ros::NodeHandle n;
        ros::NodeHandle nh;
        photo_flag = true;  
        service_once = n.advertiseService("photo_once", &service_two::get_img,this);
        service_on = n.advertiseService("photo_on", &service_two::set_flag,this);
    }
    bool service_two::get_img(std_srvs::Empty::Request  &req,std_srvs::Empty::Response &res)
    {
        int cnt = 0;
        while (1)
        {
          if (cnt > 399) {
            break;
          }      
          cnt +=1;
          std::cout<<"while photo_flag:"<<cnt<<std::endl;      
          sleep(1);
          if(photo_flag==false)
              break;
        }
      return true;
    }
     
    bool service_two::set_flag(std_srvs::SetBool::Request  &req,std_srvs::SetBool::Response &res)
    {
      if (req.data){
        photo_flag=true;
      }else{
        photo_flag=false;
      }
      std::cout<<"photo_flag:"<<photo_flag<<std::endl;
      return true;
    }
     
    int main(int argc, char **argv)
    {
      ros::init(argc, argv, "idmvs_server");
      service_two service_two;
      //�������߳�ֻ��Ҫ�������� 
      ros::AsyncSpinner spinner(2);           //������ʽspinner, ����ʹ��start��stop���� ���������߳� 
      spinner.start();                        //�����߳� 
      ros::waitForShutdown();                 //�ȴ��˳� 
      ros::spin();                            //spinѭ�� 
      return 0;
    }
 
