#include "rclcpp/rclcpp.hpp"
//这是 ROS2的c++客户端库核心，几乎所有ROS2的功能全在里面（创建节点，日志输出，时间处理等）都包含在里面。
#include "std_msgs/msg/string.hpp"
// 我们要接收字符串 所以引用标准信息库（std_msgs)里String定义。
class Listener : public rclcpp::Node{
//这里运用了c++继承 rclcpp::Node 是父类（标准节点模板）   Listenenr是你自定义类的名称 
//这句话的意思是我创建的Listener的类 继承了ROS2所有节点的能力。
public :
    Listener() : Node("listener_node_cpp"){
        RCLCPP_INFO(this->get_logger(),"订阅方创建！");//(这条消息相当于printf "订阅方创建！使在终端打印出来")
        subscription_ =this->create_subscription<std_msgs::msg::String>("chatter",10,std::bind(&Listener::do_cb,this,std::placeholders::_1));
        //(this->create_subscription 意思是下指令 我要开始订阅消息啦）
        //(<std_msgs::msg::String>表示我要的是字符串的格式)
        //(chatter 是话题名称 必须与发布方保持一致)
        //（10 队列长度 如果消息发送太快或断网时 节点处理不过来时 最多在内存里缓存10条 ）
        //（std::bind(...) 绑定回调函数     一旦频道里有新的消息 请自动执行我自定义do_cb函数 )
        //(std::placeholders::_1是一个占位符 代表接收那条消息内容本身)


    //(构造函数 括号里的”listener_node_cpp"是这个节点在ROS2系统里的名字)
};
private :

    void do_cb(const std_msgs::msg::String &msg){
        RCLCPP_INFO(this->get_logger(),"订阅到的消息：%s",msg.data.c_str());
        // %s是一个占位符 告诉打印机：后面会有一个字符串传过来 你把它添到这个位置
        // msg.data 在ROS2的String内实际的文字内容在储存在 data成员变量里面
        // .c_str() 本质是格式转换将c++转化为c语言 因为底层日志工具仍使用的是c语言
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
}; //SharedPtr （智能指针）：它会自动管理内存 只要subscrption_变量还在 系统就知道订阅合同还在生效
    

int main(int argc, char const *argv[])
{
    rclcpp::init(argc,argv);

    rclcpp::spin(std::make_shared<Listener>()); 
    //rclcpp::spin(...) 由于Listener是通过回调函数工作的 程序执行到这里就会停住 开始不断检查是否有新的消息到这 如果有spin函数 会立刻调用对应的do_cb 
    //std::make_shared<Listener>() 在堆内存中创建了一个Listener动态实例 并用智能指针去管理它
    //保证了对象在程序运行期间一直有效，且不会发生内存泄露。
    
    rclcpp::shutdown();
    return ;
}
