#include "../message.hpp"  // 包含消息相关的头文件

int main(){
    // 创建一个RPC请求对象
    MRPC::RpcRequest::ptr req = MRPC::MessageFactory::create<MRPC::RpcRequest>();
    
    // 构造参数对象，模拟加法运算的两个参数
    Json::Value param;
    param["num1"]=11;  // 设置第一个参数为11
    param["num2"]=20;  // 设置第二个参数为20
    
    // 设置请求参数和方法名
    req->setParams(param);  // 设置RPC参数
    req->setMethod("add");  // 设置RPC方法名为"add"
    
    // 将请求对象序列化为JSON字符串
    std::string msg = req->serialize();
    std::cout << "msg: " << msg << std::endl;  // 输出序列化后的消息字符串

    // 创建一个RPC响应对象
    MRPC::RpcResponse::ptr rsp = MRPC::MessageFactory::create<MRPC::RpcResponse>();
    rsp->unserialize(msg);  // 尝试将请求消息反序列化为响应对象（测试兼容性）
    std::cout << "rsp: " << rsp->serialize() << std::endl;  // 输出响应对象序列化结果

    // 创建另一个RPC请求对象
    MRPC::RpcRequest::ptr req2 = MRPC::MessageFactory::create<MRPC::RpcRequest>();
    req2->unserialize(msg);  // 将之前序列化的请求消息反序列化到新的请求对象中
    std::cout << "req2: " << req2->serialize() << std::endl;  // 输出新请求对象序列化结果

    // 测试访问反序列化后的请求对象的各个字段
    std::cout<<req2->getMethod()<<std::endl;  // 输出方法名
    std::cout<<req2->getParams()["num1"].asInt()<<std::endl;  // 输出参数1的值
    std::cout<<req2->getParams()["num2"].asInt()<<std::endl;  // 输出参数2的值
    
    // ========== 测试 TopicRequest ==========
    std::cout << "\n===== 测试 TopicRequest =====" << std::endl;
    
    // 创建一个主题请求对象
    MRPC::TopicRequest::ptr topic_req = MRPC::MessageFactory::create<MRPC::TopicRequest>();
    
    // 设置主题相关信息
    topic_req->setTopickey("sensor_data");  // 设置主题名称
    topic_req->setOptype(MRPC::TopicOpType::TOPIC_PUBLISH);  // 设置操作类型为发布
    topic_req->setTopicMsg("Temperature: 25.5C");  // 设置主题消息内容
    
    // 序列化主题请求对象
    std::string topic_msg = topic_req->serialize();
    std::cout << "topic_msg: " << topic_msg << std::endl;
    
    // 创建新的主题请求对象并反序列化
    MRPC::TopicRequest::ptr topic_req2 = MRPC::MessageFactory::create<MRPC::TopicRequest>();
    topic_req2->unserialize(topic_msg);
    
    // 验证反序列化后的字段
    std::cout << "Topic key: " << topic_req2->getTopickey() << std::endl;
    std::cout << "Topic operation: " << static_cast<int>(topic_req2->getOptype()) << std::endl;
    std::cout << "Topic message: " << topic_req2->getTopicMsg() << std::endl;
    
    // ========== 测试 ServiceRequest ==========
    std::cout << "\n===== 测试 ServiceRequest =====" << std::endl;
    
    // 创建一个服务请求对象
    MRPC::ServiceRequest::ptr service_req = MRPC::MessageFactory::create<MRPC::ServiceRequest>();
    
    // 设置服务相关信息
    service_req->setMethod("calculator_service");  // 设置服务方法名
    service_req->setOptype(MRPC::ServiceOpType::SERVICE_REGISTRY);  // 设置操作类型为注册服务
    
    // 设置主机地址信息
    MRPC::Address host_addr;
    host_addr.first = "192.168.1.100";  // 设置IP地址
    host_addr.second = 8080;  // 设置端口
    service_req->setHost(host_addr);
    
    // 序列化服务请求对象
    std::string service_msg = service_req->serialize();
    std::cout << "service_msg: " << service_msg << std::endl;
    
    // 创建新的服务请求对象并反序列化
    MRPC::ServiceRequest::ptr service_req2 = MRPC::MessageFactory::create<MRPC::ServiceRequest>();
    service_req2->unserialize(service_msg);
    
    // 验证反序列化后的字段
    std::cout << "Service method: " << service_req2->getMethod() << std::endl;
    std::cout << "Service operation: " << static_cast<int>(service_req2->getOptype()) << std::endl;
    
    // 获取并验证主机信息
    MRPC::Address host_addr2 = service_req2->getHost();
    std::cout << "Host IP: " << host_addr2.first << std::endl;
    std::cout << "Host Port: " << host_addr2.second << std::endl;
    
    return 0;
}