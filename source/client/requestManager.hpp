#pragma once
#include "../common/net.hpp"
#include "../common/message.hpp"
#include <future>
#include <functional>

namespace MRPC{

    namespace client{
        class RequestManager{
            public:
                using RequestCallback = std::function<void(BaseMessage::ptr)>;
                using AsyncResponse = std::future<BaseMessage::ptr>;
                using ptr = std::shared_ptr<RequestManager>;
                struct ReqDesc{
                    using ptr = std::shared_ptr<ReqDesc>;
                    BaseMessage::ptr request;
                    ReqType req_type;
                    std::promise<BaseMessage::ptr> response;//存放响应
                    RequestCallback _callback;
                };

                void onResponse(const BaseMessage::ptr &msg,const BaseConnection::ptr &conn){
                    std::string req_id = msg->get_Id();
                    ReqDesc::ptr req_desc = getDesc(req_id);
                    if(req_desc.get()==nullptr){
                        ELOG("收到响应，但是未找到对应请求描述，请检查代码",req_id.c_str());
                        return;
                        //因为不可能出现这个情况，只能是代码问题。
                    }
                    if(req_desc->req_type == ReqType::REQ_ASYNC){
                        //异步响应
                        req_desc->response.set_value(msg);
                    }
                    else if(req_desc->req_type == ReqType::REQ_CALLBACK){
                        //回调
                        if(req_desc->_callback){
                            req_desc->_callback(msg);
                        }
                    }
                    else{
                        ELOG("收到未知类型响应，检查代码");
                        return;
                    }
                    delDesc(req_id);//一定要记得删除！不然会造成内存泄漏
                }

                bool send(const BaseConnection::ptr &conn,const BaseMessage::ptr &req,AsyncResponse &async_rsp){
                    //异步
                    ReqDesc::ptr req_desc = newDesc(req,ReqType::REQ_ASYNC);
                    if(req_desc.get()==nullptr){
                        ELOG("创建请求描述失败，请检查代码和内存");
                        return false;
                    }
                    // conn->sendMessage(req_desc->request);
                    conn->sendMessage(req);
                    async_rsp = req_desc->response.get_future();//获取异步响应
                    return true;
                }

                bool send(const BaseConnection::ptr &conn,const BaseMessage::ptr &req,BaseMessage::ptr &rsp){
                    AsyncResponse async_rsp;
                    bool ret = send(conn,req,async_rsp);
                    if(!ret){
                        return false;
                    }
                    rsp = async_rsp.get();
                    return true;
                }


                bool send(const BaseConnection::ptr &conn,const BaseMessage::ptr &req,RequestCallback &callback){
                    //同步
                    ReqDesc::ptr req_desc = newDesc(req,ReqType::REQ_CALLBACK,callback);
                    if(req_desc.get()==nullptr){
                        ELOG("创建请求描述失败，请检查代码和内存");
                        return false;
                    }
                    conn->sendMessage(req_desc->request);
                    return true;
                }

            private:
                ReqDesc::ptr newDesc(const BaseMessage::ptr &req,ReqType req_type,const RequestCallback &_callback = RequestCallback()){
                    std::unique_lock<std::mutex> lock(_mutex);
                    ReqDesc::ptr req_desc = std::make_shared<ReqDesc>();
                    if(req_type == ReqType::REQ_CALLBACK){
                        req_desc->_callback = _callback;
                    }
                    _request_desc.insert(std::make_pair(req->get_Id(),req_desc));
                    return req_desc;
                }
                ReqDesc::ptr getDesc(const std::string &req_id)//根据请求id来查找关联的请求描述
                {
                    std::unique_lock<std::mutex> lock(_mutex);
                    auto it = _request_desc.find(req_id);
                    if(it != _request_desc.end()){
                        return it->second;
                    }
                    return ReqDesc::ptr();
                }   

                void delDesc(const std::string &req_id){
                    std::unique_lock<std::mutex> lock(_mutex);
                    _request_desc.erase(req_id);
                }
            private:
                std::mutex _mutex;
                std::unordered_map<std::string,ReqDesc::ptr> _request_desc;
            
        };
    } // namespace client

}; // namespace MRPC