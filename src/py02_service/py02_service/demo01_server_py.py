# 1. 导包;
import rclpy
from rclpy.node import Node
from base_interfaces_demo.srv import AddInts

# 3. 自定义节点类;
class AddIntsServer(Node):
    def __init__(self):
        super().__init__("add_ints_server_node_py")
        self.get_logger().info("服务端创建!(python)")
        #创建服务端
        self.server = self.create_service(AddInts,"add_ints",self.add)
        #编写回调函数处理请求并产生响应
    def add(self,request,response):
        response.sum = request.num1 + request.num2 
        self.get_logger().info("%d + %d = %d" % (request.num1,request.num2 ,response.sum))   
        return response
def main():
    # 2. 初始化ROS2客户端;
    rclpy.init()
    # 4. 调用spin函数，并传入节点对象;
    rclpy.spin(AddIntsServer())
    # 5. 资源释放;
    rclpy.shutdown()

if __name__ == '__main__':
    main()