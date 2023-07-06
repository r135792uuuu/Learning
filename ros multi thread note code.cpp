    // 执行这两个函数 get_img一直执行，收到 flag之后停止 
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
    
    // 通过set_flag设置flag，控制get_img 的执行 
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
    
    // 加入ROS多线程
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
      //启动多线程只需要下面三行 
      ros::AsyncSpinner spinner(2);           //非阻塞式spinner, 可以使用start和stop控制 ，开两个线程 
      spinner.start();                        //启动线程 
      ros::waitForShutdown();                 //等待退出 
      ros::spin();                            //spin循环 
      return 0;
    }
 
