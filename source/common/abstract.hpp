#pragma once
#include "fields.hpp"
#include "detail.hpp"
#include <string>
#include <memory>
#include <functional>

namespace MRPC{
    class BaseMessage//消息的基类，基类中只包含id和类型，具体的序列化、反序列化、校验等方法在子类中实现
    {
        public:
            using ptr = std::shared_ptr<BaseMessage>;
            //用智能指针来管理消息对象
            //virtual ~BaseMessage();
            virtual void set_Id(const std::string &id){
                rid = id;
            }
            virtual std::string get_Id() const{
                return rid;
            }
            virtual void set_MType(MType type){
                mtype = type;
            }
            virtual MType get_MType() const{
                return mtype;
            }

            virtual std::string serialize() const = 0;
            //将消息序列化为字符串
            virtual bool unserialize(const std::string &msg) = 0;
            //根据得到的一个字符串message，进行一个反序列化，将消息中的字段赋值给消息对象

            virtual bool checkValid() const = 0;
            //反序列化之后，对消息进行校验，检查消息字段是否都是完整且合法的

        private:
            MType mtype;
            std::string rid;
    };

    class BaseBuffer//这个类是缓冲区的基类，所有的缓冲区类都继承自这个类，主要包含缓冲区的读取、写入、删除等方法
    {
        public:
            using ptr = std::shared_ptr<BaseBuffer>;//用智能指针来管理缓冲区对象
            virtual size_t readable_size() const = 0;
            //用来获取缓冲区中可读取的字节数
            virtual int32_t peekInt32() const = 0;
            //用来尝试获取一个四字节的内容用于判断
            virtual void retrieveInt32() = 0;
            //从缓冲区里面删除四个字节的数据(确定符合条件了，再读取并删除)
            virtual int32_t readInt32() const = 0;
            //相当于peekInt32()和retrieveInt32()的组合
            virtual std::string retrieveAsString(size_t len) = 0;
            //从缓冲区里面删除len个字节的数据(确定符合条件了，再读取并删除)
        private:
    };

    class BaseProtocol 
    {
        public:
            using ptr = std::shared_ptr<BaseProtocol>;  //用智能指针来管理协议对象
            virtual bool canProcessed(const BaseBuffer::ptr &buffer) const = 0;
            //用来根据缓冲区前四个字节的数据来判断是否能够进行处理，返回一个bool类型
            virtual bool onMessage (const BaseBuffer::ptr &buffer,BaseMessage::ptr &msg) = 0;
            //根据缓冲区的buffer内容，将buffer中的数据反序列化成一个消息对象，并赋值给msg
            virtual std::string serialize(const BaseMessage::ptr &msg) const = 0;
            //对消息msg进行序列化，序列化完毕后返回一个字符串
    };


    //通信连接的抽象，需要有发送消息，
    class BaseConnection{
        public:
            using ptr = std::shared_ptr<BaseConnection>;
            //用智能指针来管理通信连接对象
            //virtual ~BaseConnection();
            virtual void sendMessage(const BaseMessage::ptr &msg) = 0;
            //发送消息
            virtual void shutdown() = 0;
            //关闭连接
            virtual bool connected() = 0;
            //连接是否正常
    };

    using ConnectionCallback = std::function<void(const BaseConnection::ptr &)>;
    //连接建立回调函数，用于在连接建立时进行回调
    using CloseCallback = std::function<void(const BaseConnection::ptr &)>;
    //连接关闭回调函数，用于在连接关闭时进行回调
    using MessageCallback = std::function<void(const BaseConnection::ptr &,const BaseMessage::ptr &)>;
    //消息回调函数，用于在收到消息时进行回调，包含连接和消息两个参数

    class BaseServer//服务端抽象，设置好回调函数，启动服务器
    {
        public:
            using ptr = std::shared_ptr<BaseServer>;
            virtual void setConnectionCallback(const ConnectionCallback &cb){
                //设置连接建立回调函数
                _connection_cb = cb;
            }
            virtual void setCloseCallback(const CloseCallback &cb){
                //设置连接关闭回调函数
                _close_cb = cb;
            }
            virtual void setMessageCallback(const MessageCallback &cb){
                //设置消息回调函数
                _message_cb = cb;
            }
            virtual void start() = 0;
            //启动服务器
        protected:
            ConnectionCallback _connection_cb;
            CloseCallback _close_cb;
            MessageCallback _message_cb;
    };

    class BaseClient//客户端抽象，设置好回调函数，连接服务器，发送消息，关闭连接，判断客户端连接是否正常，获取连接等
    {
        public:
            using ptr = std::shared_ptr<BaseClient>;
            virtual void setConnectionCallback(const ConnectionCallback &cb){
                //设置连接建立回调函数
                _connection_cb = cb;
            }
            virtual void setCloseCallback(const CloseCallback &cb){
                //设置连接关闭回调函数
                _close_cb = cb;
            }
            virtual void setMessageCallback(const MessageCallback &cb){
                //设置消息回调函数
                _message_cb = cb;
            }
            virtual void connect() = 0;
            //连接服务器
            virtual bool sendMessage(const BaseMessage::ptr &msg) = 0;
            //发送消息
            virtual void shutdown() = 0;
            //关闭连接
            virtual bool connected() = 0;
            //连接是否正常
            virtual BaseConnection::ptr getConnection() = 0;
            //获取连接

        protected:
            ConnectionCallback _connection_cb;
            CloseCallback _close_cb;
            MessageCallback _message_cb;
    };
     
}
