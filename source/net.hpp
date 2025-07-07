#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/base/CountDownLatch.h>
#include <muduo/net/Buffer.h>
#include "abstract.hpp"
#include "detail.hpp"
#include "message.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <mutex>
#include <thread>

namespace MRPC{
    /**
     * @brief MuduoBuffer类 - BaseBuffer的具体实现，基于muduo库的Buffer
     * 
     * 将muduo库的Buffer封装为符合BaseBuffer接口的类，实现网络数据的读取和解析
     * 主要用于处理TCP连接接收到的数据，提供二进制数据到具体数据类型的转换
     */
    class MuduoBuffer:public BaseBuffer{
        public:
            using ptr = std::shared_ptr<MuduoBuffer>;
            MuduoBuffer(muduo::net::Buffer *buffer):_buffer(buffer){}
            //不写析构手动释放，因为*_buffer指向connection里对应的缓冲区，它的资源管理不由MuduoBuffer管理
            
            /**
             * @brief 获取缓冲区中可读取的字节数
             * @return 可读取的字节数
             */
            virtual size_t readable_size() const override{
                return _buffer->readableBytes();
                //返回缓冲区中可读取的字节数
            }
            
            /**
             * @brief 查看缓冲区中前四个字节的内容(不删除)
             * @return 转换为int32_t的值，已处理网络字节序
             */
            virtual int32_t peekInt32() const override{
                return _buffer->peekInt32();
                //尝试取出（不删除）缓冲区中前四个字节的内容，用于判断
                //注意muduo库是一个网络库，会将网络字节序转换为本地主机字节序
            }
            
            /**
             * @brief 删除缓冲区中前四个字节的内容
             */
            virtual void retrieveInt32() override{
                return _buffer->retrieveInt32();
                //取出（删除）缓冲区中前四个字节的内容
            }
            
            /**
             * @brief 读取并删除缓冲区中前四个字节的内容
             * @return 转换为int32_t的值，已处理网络字节序
             */
            virtual int32_t readInt32() const override{
                return _buffer->readInt32();
            }
            
            /**
             * @brief 从缓冲区读取指定长度的字符串并删除
             * @param len 要读取的字节数
             * @return 转换为string的数据
             */
            virtual std::string retrieveAsString(size_t len) override{
                return _buffer->retrieveAsString(len);
                //从缓冲区里面删除len个字节的数据(确定符合条件了，再读取并删除)
            }
        private:
            muduo::net::Buffer *_buffer;  // muduo库的缓冲区对象指针
    };

    /**
     * @brief BufferFactory类 - 创建缓冲区对象的工厂类
     * 
     * 用于创建MuduoBuffer对象，提供统一的创建接口
     * 工厂模式实现，隔离对象创建和使用，提高系统灵活性
     */
    class BufferFactory{
        public:
            /**
             * @brief 创建缓冲区对象
             * @param args 传递给MuduoBuffer构造函数的参数
             * @return 指向创建的缓冲区对象的智能指针
             */
            template<typename ...Args>
            static BaseBuffer::ptr create(Args&&...args){
                return std::make_shared<MuduoBuffer>(std::forward<Args>(args)...);
            }
    };

    /**
     * @brief LVProtocol类 - 实现长度-值协议的消息处理
     * 
     * 消息格式：|==Length==|==Value==|
     * 扩展格式：|==Length==|==MType==|==Idlength==|==Id==|==body==|
     * 
     * 负责网络消息的编码和解码，实现BaseProtocol接口
     * 该协议设计解决了TCP粘包和分包问题，能准确识别消息边界
     */
    class LVProtocol:public BaseProtocol{
        public:
            using ptr = std::shared_ptr<BaseProtocol>;
            
            /**
             * @brief 检查缓冲区中的数据是否足够处理一条完整消息
             * @param buffer 缓冲区对象
             * @return 如果数据量足够返回true，否则返回false
             * 
             * 首先检查消息长度字段，然后判断缓冲区中是否有足够数据
             */
            virtual bool canProcessed(const BaseBuffer::ptr &buffer) const override
            {
                    // 首先检查是否有足够的数据读取长度字段
                if(buffer->readable_size() < fields_len) {
                    return false;
                }

                //peekInt32()中有一个断言，如果不足四字节会断言失败导致程序崩溃
                int32_t length = buffer->peekInt32();
                if(buffer->readable_size() < length+fields_len){
                    return false;
                }
                return true;
            }

            /**
             * @brief 处理缓冲区中的消息数据，反序列化为消息对象
             * @param buffer 缓冲区对象
             * @param msg 输出参数，存储反序列化后的消息对象
             * @return 处理成功返回true，否则返回false
             * 
             * 按照协议格式依次解析各个字段，构造对应类型的消息对象
             */
            virtual bool onMessage(const BaseBuffer::ptr &buffer,BaseMessage::ptr &msg) override{
                //针对接收数据进行处理，默认已判断数据足够用于处理
                int32_t length = buffer->readInt32();   //读取消息长度
                MType mtype = (MType)buffer->readInt32();    //读取消息类型
                int32_t id_len = buffer->readInt32();   //读取消息id长度
                std::string id_str = buffer->retrieveAsString(id_len); //读取消息id
                std::string body = buffer->retrieveAsString(length-fields_len-mtype_len-id_len);
                //读取消息体
                msg = MessageFactory::create(mtype);
                if (msg.get() == nullptr) {
                    ELOG("消息类型错误，构造消息对象失败！");
                    return false;
                }
                if(!msg->unserialize(body)){
                    ELOG("消息反序列化失败！");
                    return false;
                }
                msg->set_Id(id_str);
                msg->set_MType(mtype);
                return true;
            }

            /**
             * @brief 将消息对象序列化为二进制数据
             * @param msg 要序列化的消息对象
             * @return 序列化后的二进制数据字符串
             * 
             * 序列化格式：|==Length==|==MType==|==Idlength==|==Id==|==body==|
             * 注意处理网络字节序问题，确保跨平台兼容性
             */
            virtual std::string serialize(const BaseMessage::ptr &msg) const override
            {
                std::string body = msg->serialize();
                std::string id = msg->get_Id();
                auto mtype = htonl((int32_t)msg->get_MType());
                int32_t idlen = htonl(id.size());
                int32_t h_total_len = mtype_len + id_len + id.size() + body.size();
                int32_t n_total_len = htonl(h_total_len);
                //DLOG("h_total_len:%d", h_total_len);
                std::string result;
                result.reserve(h_total_len);
                result.append((char*)&n_total_len, fields_len);
                result.append((char*)&mtype, mtype_len);
                result.append((char*)&idlen, id_len);
                result.append(id);
                result.append(body);
                return result;
            }

        private:
            const size_t fields_len = 4;    //存放消息长度的字段长度
            const size_t mtype_len = 4;     //存放消息类型的字段长度
            const size_t id_len = 4;        //存放消息id的字段长度
    };

    /**
     * @brief ProtocolFactory类 - 创建协议对象的工厂类
     * 
     * 工厂模式实现，用于创建LVProtocol对象
     * 将对象创建与使用分离，便于后续扩展其他协议实现
     */
    class ProtocolFactory {
        public:
            template<typename ...Args>
            static BaseProtocol::ptr create(Args&& ...args) {
                return std::make_shared<LVProtocol>(std::forward<Args>(args)...);
            }
    };

    /**
     * @brief MuduoConnection类 - BaseConnection的具体实现
     * 
     * 封装muduo库的TcpConnection，实现BaseConnection接口
     * 负责消息发送、连接管理等功能
     */
    class MuduoConnection:public BaseConnection{
        public:
            using ptr = std::shared_ptr<MuduoConnection>;
            //用智能指针来管理通信连接对象
            MuduoConnection(muduo::net::TcpConnectionPtr connection,
            BaseProtocol::ptr protocol)
            :_connection(connection),_protocol(protocol){}
            
            /**
             * @brief 发送消息
             * @param msg 要发送的消息对象
             * 
             * 使用协议对象序列化消息，然后通过TCP连接发送
             */
            virtual void sendMessage(const BaseMessage::ptr &msg) override{
                //基于protocol的序列化方法，将消息序列化后发送
                std::string body = _protocol->serialize(msg);
                _connection->send(body);
            }
            
            /**
             * @brief 关闭连接
             * 
             * 主动关闭TCP连接
             */
            virtual void shutdown() override{  
                _connection->shutdown();
            }
            
            /**
             * @brief 检查连接是否正常
             * @return 连接正常返回true，否则返回false
             */
            virtual bool connected() override{
                return _connection->connected();
            }
        private:
            BaseProtocol::ptr _protocol;              // 协议对象，用于消息序列化和反序列化
            muduo::net::TcpConnectionPtr _connection; // muduo库的连接对象
    };
    
    /**
     * @brief ConnectionFactory类 - 创建连接对象的工厂类
     * 
     * 工厂模式实现，用于创建MuduoConnection对象
     * 将连接对象的创建与使用分离，提高系统灵活性
     */
    class ConnectionFactory 
    {
        public:
            template<typename ...Args>
            static BaseConnection::ptr create(Args&& ...args) {
                return std::make_shared<MuduoConnection>(std::forward<Args>(args)...);
            }
    };

    /**
     * @brief MuduoServer类 - BaseServer的具体实现
     * 
     * 封装muduo库的TcpServer，实现BaseServer接口
     * 负责服务器的启动、连接管理、消息处理等功能
     */
    class MuduoServer:public BaseServer{
        public:
            using ptr = std::shared_ptr<BaseServer>;
            
            /**
             * @brief 构造函数
             * @param port 服务器监听端口
             * 
             * 初始化TcpServer和协议对象，设置服务器参数
             * 使用kReusePort允许多进程绑定同一端口，提高并发性能
             */
            MuduoServer(int port):
            _server(& _baseloop,muduo::net::InetAddress("0.0.0.0", port), "DictServer",
            muduo::net::TcpServer::kReusePort),_protocol(ProtocolFactory::create())
            {}

            /**
             * @brief 启动服务器
             * 
             * 设置连接回调和消息回调，启动服务器并开始事件循环
             */
            virtual void start() override{
                _server.setConnectionCallback(std::bind(&MuduoServer::onConnection, this, std::placeholders::_1));
                //设置消息回调处理函数
                _server.setMessageCallback(std::bind(&MuduoServer::onMessage, this, std::placeholders::_1, 
                std::placeholders::_2, std::placeholders::_3));
                _server.start();
                _baseloop.loop();
            }
            //启动服务器
         
            /**
             * @brief 连接回调函数
             * @param conn muduo库的TCP连接对象
             * 
             * 处理连接建立和断开事件
             * 连接建立时创建Connection对象并存入连接表
             * 连接断开时从连接表中移除并通知应用层
             */
            void onConnection(const muduo::net::TcpConnectionPtr &conn){
                if(conn->connected()){
                    std::cout<<"新连接建立"<<std::endl;
                    auto connection = ConnectionFactory::create(conn,_protocol);
                    {
                        std::unique_lock<std::mutex> lock(_mutex);
                        _connections.insert(std::make_pair(conn,connection));
                    }
                    if(_connection_cb){
                        _connection_cb(connection);
                    }
                }
                else{
                    std::cout<<"连接断开";
                    BaseConnection::ptr muduo_conn;
                    {
                        std::unique_lock<std::mutex> lock(_mutex);
                        auto it = _connections.find(conn);
                        if (it == _connections.end()) {
                            return;
                        }
                        muduo_conn = it->second;
                        _connections.erase(conn);
                    }
                    if (_close_cb) _close_cb(muduo_conn);
                }

            }

            /**
             * @brief 消息回调函数
             * @param conn muduo库的TCP连接对象
             * @param buf muduo库的缓冲区对象
             * @param timestamp 消息到达时间戳
             * 
             * 处理新到达的网络数据
             * 将缓冲区数据封装为BaseBuffer对象
             * 检查消息完整性，解析消息，并调用应用层回调
             * 支持循环处理缓冲区中的多条消息
             */
            void onMessage(const muduo::net::TcpConnectionPtr &conn,muduo::net::Buffer *buf,muduo::Timestamp){
                auto base_buffer = BufferFactory::create(buf);
                while(1){
                    if(_protocol->canProcessed(base_buffer)==false){
                        //数据不足，进行返回
                        //有可能缓冲区中数据过大，不足以去处理这一条完整的消息，直接关闭连接不处理
                        if(base_buffer->readable_size() > maxDataSize){
                            conn->shutdown();
                            ELOG("数据过大，关闭连接");
                            return;
                        }
                        break;  // 数据不完整但未超限，等待更多数据
                    }
                    //数据足够，处理消息
                    BaseMessage::ptr msg;
                    bool ret = _protocol->onMessage(base_buffer,msg);
                    if(ret==false){
                        conn->shutdown();
                        ELOG("消息解析失败，关闭连接");
                        return;
                    }

                    BaseConnection::ptr base_conn;
                    {
                        std::unique_lock<std::mutex> lock(_mutex);
                        auto it = _connections.find(conn);
                        if(it == _connections.end()){
                            conn->shutdown();
                            return;
                        }
                        base_conn = it->second;
                    }
                    if(_message_cb){
                        _message_cb(base_conn,msg);  // 调用业务层回调处理消息
                    }
                    
                }
            }
 
        private:
            const size_t maxDataSize = (1<<16);  // 最大消息大小限制(64KB)，防止恶意超大消息攻击
            muduo::net::EventLoop _baseloop;     // 事件循环，注意baseloop要放在serv的上面，因为是使用baseloop来构造serv的
            muduo::net::TcpServer _server;       // muduo库的TCP服务器对象
            BaseProtocol::ptr _protocol;         // 协议对象，用于消息解析和序列化
            std::mutex _mutex;                   // 互斥锁，保护连接表的线程安全
            std::unordered_map<muduo::net::TcpConnectionPtr,BaseConnection::ptr> _connections;  // 连接映射表，关联底层连接和业务连接
    };

    /**
     * @brief ServerFactory类 - 创建服务器对象的工厂类
     * 
     * 工厂模式实现，用于创建MuduoServer对象
     * 将服务器对象的创建与使用分离，提高系统灵活性
     */
    class ServerFactory {
        public:
            template<typename ...Args>
            static BaseServer::ptr create(Args&& ...args) {
                return std::make_shared<MuduoServer>(std::forward<Args>(args)...);
            }
    };


    class MuduoClient:public BaseClient{
        public:
            using ptr = std::shared_ptr<BaseClient>;
            MuduoClient(const std::string & server_ip, int server_port):
                _protocol(ProtocolFactory::create()),
                _client(_baseloop, muduo::net::InetAddress(server_ip, server_port), "MuduoClient"),
                //参数分别为：EventLoop的实例，绑定的地址和端口，名字
                _baseloop(_loopthread.startLoop()),
                _latch(1)//计数器初始化为1，等待连接建立，计数大于0时，会阻塞,--等待计数为0时会唤醒
            {}

            virtual void connect() override{
                //连接服务器
                _client.setConnectionCallback(std::bind(&MuduoClient::onConnection, this, std::placeholders::_1));

                //设置消息回调处理函数
                _client.setMessageCallback(std::bind(&MuduoClient::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

                _client.connect();
                _latch.wait();//等待连接建立，计数为0时会唤醒阻塞
            }

            virtual bool sendMessage(const BaseMessage::ptr &msg) override{
                //发送消息
                if(connected()==false){
                    ELOG("连接断开，无法发送消息");
                    return false;
                }
                _conn->sendMessage(msg);
                return true;
            }

            virtual void shutdown() override{
                //关闭连接
                _client.disconnect();
            }

            virtual bool connected() override{
                return (_conn&&_conn->connected());
                //注意强调_conn不为空，因为连接建立之前，_conn为空
            }
            //连接是否正常
            virtual BaseConnection::ptr getConnection() override{
                return _conn;
            }
            //获取连接

        private:
            BaseProtocol::ptr _protocol;
            BaseConnection::ptr _conn;
            muduo::CountDownLatch _latch;
            muduo::net::EventLoopThread _loopthread;
            muduo::net::EventLoop *_baseloop;
            muduo::net::TcpClient _client;
            const size_t maxDataSize = (1<<16);  // 最大消息大小限制(64KB)，防止恶意超大消息攻击
            std::mutex _mutex;                   // 互斥锁，保护连接表的线程安全
            std::unordered_map<muduo::net::TcpConnectionPtr,BaseConnection::ptr> _connections;  // 连接映射表，关联底层连接和业务连接


            void onConnection(const muduo::net::TcpConnectionPtr &conn) {
                if(conn->connected()){
                    std::cout<<"连接建立\n";
                    _latch.countDown();//计数器减1，计数为0时会唤醒阻塞
                    _conn = ConnectionFactory::create(conn,_protocol);
                }
                else
                {
                    std::cout<<"连接断开\n";
                    _conn.reset();//断开连接的时候，连接对象置空
                }
            }

            //onMessage处理客户端发送的消息
            void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp){
                auto base_buffer = BufferFactory::create(buf);
                while(1){
                    if(_protocol->canProcessed(base_buffer)==false){
                        //数据不足，进行返回
                        //有可能缓冲区中数据过大，不足以去处理这一条完整的消息，直接关闭连接不处理
                        if(base_buffer->readable_size() > maxDataSize){
                            conn->shutdown();
                            ELOG("数据过大，关闭连接");
                            return;
                        }
                        break;  // 数据不完整但未超限，等待更多数据
                    }
                    //数据足够，处理消息
                    BaseMessage::ptr msg;
                    bool ret = _protocol->onMessage(base_buffer,msg);
                    if(ret==false){
                        conn->shutdown();
                        ELOG("消息解析失败，关闭连接");
                        return;
                    }
                    if(_message_cb){
                        _message_cb(_conn,msg);  // 调用业务层回调处理消息
                    }
                    //客户端只会有一个连接，服务端会为每一个连接创建一个连接对象，所以这里直接使用_conn
                }
            }
    };

    class ClientFactory{
        public:
            template<typename ...Args>
            static BaseClient::ptr create(Args&& ...args) {
                return std::make_shared<MuduoClient>(std::forward<Args>(args)...);
            }
    };
    

}
