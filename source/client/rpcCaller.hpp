#pragma once

#include "requestManager.hpp"

namespace MRPC{
    namespace client{
        class RpcCaller{
            public:
                using ptr = std::shared_ptr<RpcCaller>;
                using JsonAsyncResp = std::future<Json::Value>;
                using JsonCallback = std::function<void(const Json::Value &)>;

                RpcCaller(const RequestManager::ptr &req_manager)
                    :_req_manager(req_manager){}

                bool call(const BaseConnection::ptr &conn,const std::string &method,
                        const Json::Value &params,Json::Value &rsp){
                    //同步
                    //1.组织请求
                    auto req_msg = MessageFactory::create<RpcRequest>();
                    req_msg->set_Id(UUID::uuid());
                    req_msg->set_MType(MType::REQ_RPC);
                    req_msg->setMethod(method);
                    req_msg->setParams(params);
                    BaseMessage::ptr rsp_msg;
                    //2.发送请求
                    bool ret = _req_manager->send(conn,req_msg,rsp_msg);
                    if(!ret){
                        ELOG("发送请求失败");
                        return false;
                    }
                    //3.等待响应
                    auto rpc_rsp_msg = std::dynamic_pointer_cast<RpcResponse>(rsp_msg);
                    if(!rpc_rsp_msg){
                        ELOG("向下类型转换失败");
                        return false;
                    }
                    if(rpc_rsp_msg->getRcode() != RCode::RCODE_OK){
                        ELOG("请求失败");
                        return false;
                    }
                    rsp = rpc_rsp_msg->getResult();
                    return true;
                }

                bool call(const BaseConnection::ptr &conn,const std::string &method,
                        const Json::Value &params,std::future<Json::Value> &rsp){
                    //异步
                    //向服务器发送异步回调请求，设置回调函数，回调函数中回传入一个promise函数，在回调函数中区对promise设置数据
                    auto req_msg = MessageFactory::create<RpcRequest>();
                    req_msg->set_Id(UUID::uuid());
                    req_msg->set_MType(MType::REQ_RPC);
                    req_msg->setMethod(method);
                    req_msg->setParams(params);
                    BaseMessage::ptr rsp_msg;

                    std::promise<Json::Value> json_promise;
                    rsp = json_promise.get_future();
                    // 使用lambda表达式代替std::bind，避免std::promise无法拷贝的问题
                    RequestManager::RequestCallback cb = [this, &json_promise](const BaseMessage::ptr &msg) {
                        this->Callback(json_promise, msg);
                    };
                    //2.发送请求
                    bool ret = _req_manager->send(conn,req_msg,cb);
                    if(!ret){
                        ELOG("发送请求失败");
                        return false;
                    }
                    return true;
                }

                bool call(const BaseConnection::ptr &conn,const std::string &method,
                        const Json::Value &params, const JsonCallback &cb){
                    //回调
                    auto req_msg = MessageFactory::create<RpcRequest>();
                    req_msg->set_Id(UUID::uuid());
                    req_msg->set_MType(MType::REQ_RPC);
                    req_msg->setMethod(method);
                    req_msg->setParams(params);

                    // 使用lambda表达式代替std::bind
                    RequestManager::RequestCallback req_cb = [this, cb](const BaseMessage::ptr &msg) {
                        this->Callback1(cb, msg);
                    };
                    
                    bool ret = _req_manager->send(conn, req_msg, req_cb);
                    if (ret == false) {
                        ELOG("回调Rpc请求失败！");
                        return false;
                    }
                    return true;
                }
            private:
                void Callback1(const JsonCallback &cb, const BaseMessage::ptr &msg)  {
                    auto rpc_rsp_msg = std::dynamic_pointer_cast<RpcResponse>(msg);
                    if (!rpc_rsp_msg) {
                        ELOG("rpc响应，向下类型转换失败！");
                        return ;
                    }
                    if (rpc_rsp_msg->getRcode() != RCode::RCODE_OK) {
                        ELOG("rpc回调请求出错：%s", error_msg(rpc_rsp_msg->getRcode()).c_str());
                        return ;
                    }
                    cb(rpc_rsp_msg->getResult());
                }
                void Callback(std::promise<Json::Value> &result,const BaseMessage::ptr &msg){
                    auto rpc_rsp_msg = std::dynamic_pointer_cast<RpcResponse>(msg);
                    if(!rpc_rsp_msg){
                        ELOG("向下类型转换失败");
                        result.set_value(Json::Value());
                        return;
                    }
                    if(rpc_rsp_msg->getRcode() != RCode::RCODE_OK){
                        ELOG("rpc异步请求出错");
                        return;
                    }
                    result.set_value(rpc_rsp_msg->getResult());
                }
            private:
                RequestManager::ptr _req_manager;

        };
    }
        
}