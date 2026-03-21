/* 
     需求：创建客户端，组织数据并提交，然后响应结果（需要关注业务流程）；

         流程：
         前提：main函数中需要判断提交的参数是否正确
     1.包含头文件；
    2.初始化ROS2客户端;
    3.自定义节点类；
     3-1创建客户端；
    3-2 连接服务器（对于服务通信而言，如果客户端连接不到服务器，那么不能发送请求)；
    3-3. 发送请求。
    4.创建对象指针；
     需要调用连接服务的函数，根据连接结果做下一步处理。
     连接服务后，调用请求发送函数；
     在处理响应结果
    5.释放资源；
    */
    // 1. 包含头文件；
    #include "rclcpp/rclcpp.hpp"
    #include "base_interfaces_demo/srv/add_ints.hpp"

    using base_interfaces_demo::srv::AddInts;
    using namespace std::chrono_literals;
    
    //3.自定义节点类；
    class AddIntsClient: public rclcpp:: Node{
             public :
         AddIntsClient():Node("add_ints_client_node_cpp"){
            RCLCPP_INFO(this->get_logger(),"客户端创建！");

//       3-1创建客户端；
          /*
             模板 ：服务接口；
             参数 ：服务话题名称；
             返回值 ：服务对象指针；
          
          */
         client_= this->create_client<AddInts>("add_ints");
         }
         

//     3-2 连接服务器（对于服务通信而言，如果客户端连接不到服务器，那么不能发送请求)                
                   
             /* 
             连接服务器，如果成功连接 ture 否则返回 flase
             
             */        
             bool connect_service(){

            
    while (!client_->wait_for_service(1s))
    {
        if (!rclcpp::ok())
        {
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "强行终止客户端");
            return false;
        }

        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务连接中...");
    }

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务已连接");
    return true;


 }

        //3-3. 发送请求
        // 编写发送请求函数 ---参数是两个整形数据，返回值是提交请求后的服务端的返回结果
        rclcpp::Client<AddInts>::FutureAndRequestId send_request(int num1 ,int num2){
            //组织请求内容
            /* 
                rclcpp::Client<base
            
            
            */
            //发送
            auto request = std::make_shared<AddInts::Request>();
            request->num1 = num1;
            request->num2 = num2;
            return client_->async_send_request(request);
        }

private:

     rclcpp::Client<AddInts>::SharedPtr client_;    

};
    int main(int argc, char const *argv[])
    {     


     if (argc != 3)
          {    

          RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),"请递交两个整形数字！");
               return 1;
               
                             
               } 


         // 2.初始化ROS2客户端;
     rclcpp::init(argc,argv);
         
          //创建客户端对象；
          auto client = std::make_shared<AddIntsClient>();  
          // 调用客户端对象连接服务器功能；
          bool flag = client->connect_service();
          // 根据连接结果进一步处理
          if (! flag)
          {  
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"服务器连接失败，强制退出！");

            return 0;

          }
          // 执行后续操作...
          // 调用请求提交函数，接受并处理吸纳更应结果
            auto future = client->send_request(atoi(argv[1]),atoi(argv[2]));
          // 处理响应
             if (rclcpp::spin_until_future_complete(client,future) == rclcpp::FutureReturnCode::SUCCESS)//成功
             {
                   RCLCPP_INFO(client->get_logger(),"响应成功!sum = %d",future.get()->sum);
             }
             else//失败
             {
                    RCLCPP_INFO(client->get_logger(),"响应失败！");
             }
          //5.释放资源
                rclcpp::shutdown();
                return 0;
         }