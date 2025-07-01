#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/base/CountDownLatch.h>
#include <muduo/net/Buffer.h>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class DictClient{
    public:
        DictClient(const string & server_ip, int server_port):
            _client(_baseloop, muduo::net::InetAddress(server_ip, server_port), "DictClient"),
            //参数分别为：EventLoop的实例，绑定的地址和端口，名字
            _baseloop(_loop_thread.startLoop()),
            _latch(1)//计数器初始化为1，等待连接建立，计数大于0时，会阻塞,--等待计数为0时会唤醒
        {
            //设置回调函数
            _client.setConnectionCallback(bind(&DictClient::onConnection, this, placeholders::_1));

            //设置消息回调处理函数
            _client.setMessageCallback(bind(&DictClient::onMessage, this, placeholders::_1, placeholders::_2, placeholders::_3));

            //客户端首先要连接服务器
            _client.connect();
            _latch.wait();//等待连接建立，计数为0时会唤醒阻塞

                //_baseloop.loop();   
                //开始时间循环监控，但是这个循环监控是一个死循环，如果陷入死循环，则无法执行其他代码send发送不了数据了，所以对于客户端来说不能这么用
                //所以需要使用其他方式来发送数据，比如使用muduo::net::EventLoopThread
                //EventLoopThread是一个线程，用来处理事件循环，可以用来发送数据
                //EventLoopThread::startLoop()   启动线程
                //EventLoopThread::queueInLoop() 将数据添加到事件循环中
                //EventLoopThread::getLoop()     获取事件循环
                //EventLoopThread::stop()        停止线程
                //EventLoopThread::join()        等待线程结束
        };

        bool send(const string &msg){
            if(_conn->connected()){
                _conn->send(msg);
                return true;
            }
            else{
                cout<<"连接断开，无法发送消息\n";
                return false;
            }
        }
        
    private:
    void onConnection(const muduo::net::TcpConnectionPtr &conn) {
            if(conn->connected()){
                cout<<"连接建立\n";
                _latch.countDown();//计数器减1，计数为0时会唤醒阻塞
                _conn = conn;   //建立连接的时候
            }
            else
            {
                cout<<"连接断开\n";
                _conn.reset();//断开连接的时候，连接对象置空
            }
        }

        //onMessage处理客户端发送的消息
        void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp){
            string result = buf->retrieveAllAsString();//将缓冲区中的数据转换为字符串
            cout<<"服务器响应: " << result << endl;
        }

        muduo::net::TcpConnectionPtr _conn; //连接对象，用来发送和接收消息
        muduo::CountDownLatch _latch;       //计数器，用来等待连接建立
        muduo::net::EventLoopThread _loop_thread;   //事件循环线程，用来处理事件
        muduo::net::EventLoop *_baseloop;   //事件循环，用来处理事件
        muduo::net::TcpClient _client;     //客户端，用来连接服务器
};


int main(){

    DictClient client("127.0.0.1", 9090);
    while(1){
        string msg;
        cin>>msg;
        if(client.send(msg)){
            cout<<"发送成功\n";
        }
        else{
            cout<<"发送失败\n";
        }
    }
    return 0;
}