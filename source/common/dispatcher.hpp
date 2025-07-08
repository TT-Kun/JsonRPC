#pragma once

#include "net.hpp"
#include "message.hpp"

//消息分发，根据消息类型分发到对应的业务处理函数

namespace MRPC{

    class CallBack{
        public:
            using ptr = std::shared_ptr<CallBack>;
            virtual void onMessage(const BaseConnection::ptr &conn,const BaseMessage::ptr &msg) = 0;
        private:
    };

    template<typename T>
    class CallBackImpl : public CallBack{
        /*实现回调内容的模板化，用于放到unordered_map中*/
        public:
            using ptr = std::shared_ptr<CallBackImpl<T>>;
            using MsgCallback = std::function<void(const BaseConnection::ptr &conn,std::shared_ptr<T> &msg)>;
            CallBackImpl(const MsgCallback &handler):_handler(handler){}
            void onMessage(const BaseConnection::ptr &conn,const BaseMessage::ptr &msg){
                auto msg_type = std::dynamic_pointer_cast<T>(msg);
                //先将类型统一转换为T类型，然后调用回调函数
                _handler(conn,msg_type);
            }
        private:
            MsgCallback _handler;
    };

    class Dispatcher{
        public:
            using ptr = std::shared_ptr<Dispatcher>;
            template<typename T>
            void registerHandler(MType mtype,const typename CallBackImpl<T>::MsgCallback &handler)
            /*注册消息类型和回调之间的关系*/
            {
                std::unique_lock<std::mutex> lock(_mutex);
                auto callback = std::make_shared<CallBackImpl<T>>(handler);
                _handlers.insert(std::make_pair(mtype, callback));
            }

            void onMessage(const BaseConnection::ptr &conn,const BaseMessage::ptr &msg){
                std::unique_lock<std::mutex> lock(_mutex);
                auto it = _handlers.find(msg->get_MType());
                if(it != _handlers.end()){
                    //找到消息类型对应的业务处理函数，进行调用即可
                    it->second->onMessage(conn,msg);
                }   
                //如果找不到，打印错误信息，因为客户端和服务端消息类型都已经写好了，所以代码正确的时候不可能找不到
                //所以到这里说明代码有问题，需要检查
                else{
                    ELOG("收到未知类型消息，检查代码");
                    conn->shutdown();
                }
            }

        private:
            std::mutex _mutex;
            std::unordered_map<MType,CallBack::ptr> _handlers;
    };
}