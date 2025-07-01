#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class DictServer {
    public:
        DictServer(int port):
            _server(& _baseloop,muduo::net::InetAddress("0.0.0.0", port), "DictServer",muduo::net::TcpServer::kReusePort)
            //函数参数分别表示：EventLoop的实例，绑定的地址和端口，名字，端口复用选项
        {
            //设置回调函数
            _server.setConnectionCallback(bind(&DictServer::onConnection, this, placeholders::_1));//由于onConnection是类成员函数，所以需要绑定this指针，bind进行函数适配
            //bind通过onConnection函数进行了参数绑定之后，生成了一个适配与当前函数的可调用对象

            //设置消息回调处理函数
            _server.setMessageCallback(bind(&DictServer::onMessage, this, placeholders::_1, placeholders::_2, placeholders::_3));
        }
        void start() {
            //启动获取监听
            _server.start();
            //eventloop开启事件循环监控
            _baseloop.loop();
            //注意这里的顺序是不能够换的，因为如果先开始loop，那么连接就不会建立，因为还没有开始监听
        }
    private:
        void onConnection(const muduo::net::TcpConnectionPtr &conn) {
            if(conn->connected()){
                std::cout<<"连接建立\n";
            }
            else
            {
                std::cout<<"连接断开\n";
            }
        }

        //onMessage处理客户端发送的消息
        void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp){
            static unordered_map<string, string> dict_map={
                {"hello", "你好"},
                {"world", "世界"},
                {"good",   "好"},
                {"server", "服务器"},
                {"client", "客户端"}
            };
            string msg = buf->retrieveAllAsString();
            string result;
            auto it = dict_map.find(msg);//查找字典
            if(it!=dict_map.end()){
                //找到词
                result = it->second; //it->second获取字典中对应的值
            }
            else{
                result = "未知单词";
            }
            conn->send(result); //响应，发送数据
        }
        muduo::net::EventLoop _baseloop;//注意baseloop要放在serv的上面，因为是使用baseloop来构造serv的
        muduo::net::TcpServer _server;
};


int main(){
    DictServer server(9090);
    server.start();

    return 0;
}