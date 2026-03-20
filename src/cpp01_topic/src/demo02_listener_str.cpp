/* 
需求：订阅发布方发布的消息，并在终端输出。
流程：
    1.包含头文件；
    2.初始化ROS2的客户端；
    3-1.创建订阅方；
    3-2.解析并输出数据。
    4.调用spin函数，并传入节点对象指针；
    5资源释放。  
    
    */
   //包含头文件
   #include "rclcpp/rclcpp.hpp"
   #include "std_msgs/msg/string.hpp"
   
class Listener: public rclcpp::Node{
 public:
        Listener():Node("listener_node_cpp"){
            RCLCPP_INFO(this->get_logger(),"订阅方创建！");
            
            subscription_ = this->create_subscription<std_msgs::msg::String>("chatter",10,std::bind(&Listener::do_cb,this,std::placeholders::_1));    

        }   
 private:
 void do_cb(const std_msgs::msg::String &msg){
    
     RCLCPP_INFO(this->get_logger(),"订阅到的消息：%s",msg.data.c_str());

 }
rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;

};

int main(int argc, char const *argv[])
{
    //1.初始化ROS2的客户端；
    rclcpp::init(argc,argv);

    //4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<Listener>());

     //5.资源释放。
     rclcpp::shutdown();

    return 0;
}