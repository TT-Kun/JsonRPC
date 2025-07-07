#pragma once

#include "net.hpp"
#include "message.hpp"

//消息分发，根据消息类型分发到对应的业务处理函数

namespace MRPC{
    class Dispatcher{
        public:
            using ptr = std::shared_ptr<Dispatcher>;

            void registerHandler(MType mtype,const MessageCallback &handler)
            /*注册消息类型和回调之间的关系*/
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _handlers.insert(std::make_pair(mtype,handler));
            }

            void onMessage(const BaseConnection::ptr &conn,const BaseMessage::ptr &msg){
                std::unique_lock<std::mutex> lock(_mutex);
                auto it = _handlers.find(msg->get_MType());
                if(it != _handlers.end()){
                    //找到消息类型对应的业务处理函数，进行调用即可
                    it->second(conn,msg);
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
            std::unordered_map<MType,MessageCallback> _handlers;
    };
}