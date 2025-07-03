#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>    
#include "detail.hpp"
#include "fields.hpp"
#include "abstract.hpp"


namespace MRPC{
    class JsonMessage : public BaseMessage{
        public:
            using ptr = std::shared_ptr<JsonMessage>;
            virtual std::string serialize() const override{
                std::string body;
                bool ret = JSON::serialize(_body, body);
                //调用JSON::serialize将_body序列化为body
                if(!ret){
                    return "";
                }
                return body;
            }
            virtual bool unserialize(const std::string &msg) override{
                return JSON::unserialize(msg,_body);
            }
        protected:
            Json::Value _body;//消息体
    };

    class JsonRequest : public JsonMessage{
        public:
            using ptr = std::shared_ptr<JsonRequest>;
            
        private:
    };

    class JsonResponse : public JsonMessage {
        public:
            using ptr = std::shared_ptr<JsonResponse>;
            virtual bool checkValid() const override {
                //在响应中，大部分的响应都只有响应状态码
                //因此只需要判断响应状态码字段是否存在，类型是否正确即可
                if (_body[KEY_RECODE].isNull() == true) {
                    ELOG("响应中没有响应状态码！");
                    return false;
                }
                if (_body[KEY_RECODE].isIntegral() == false) {
                    ELOG("响应状态码类型错误！");
                    return false;
                }
                return true;
            }
            virtual RCode getRecode() {
                return (RCode)_body[KEY_RECODE].asInt();
            }
            virtual void setReCode(RCode rcode) {
                _body[KEY_RECODE] = (int)rcode;
            }
    };

    class RpcRequest : public JsonRequest{
        public:
            using ptr = std::shared_ptr<RpcRequest>;
            //在一个RPC请求中，包含有请求的方法名称和参数字段，因此需要判断这两个字段是否存在，类型是否正确
            virtual bool checkValid() const override{
                if(_body[KEY_RECODE].isNull()==true || 
                   _body[KEY_RECODE].isString()==false){
                    ELOG("请求中没有请求方法名称，或请求方法类型不是string");
                    return false;
                }
                if(_body[KEY_PARAMS].isNull()==true ||
                   _body[KEY_PARAMS].isObject()==false){
                    ELOG("请求中没有请求参数，或请求参数类型不是object");
                    return false;
                }
                return true;
            }


            //以下内容用于确认请求合法后，获取和设置请求方法名称和请求参数
            std::string getMethod(){
                return _body[KEY_METHOD].asString();
                //返回正文请求方法名称
            }
            void setMethod(const std::string &method_name){
                _body[KEY_METHOD] = method_name;
            }

            Json::Value getParams(){
                return _body[KEY_PARAMS];
            }
            void setParams(const Json::Value &params_name){
                _body[KEY_PARAMS] = params_name;
            }
    };
    
    class TopicRequest : public JsonRequest{
        public:
            using ptr = std::shared_ptr<TopicRequest>;
            virtual bool checkValid() const override{  
                if(_body[KEY_TOPIC_KEY].isNull()==true ||
                   _body[KEY_TOPIC_KEY].isString()==false){
                    ELOG("主题请求中没有主题名称或主题名称类型错误");
                    return false;
                }
                if(_body[KEY_OPTYPE].isNull()==true ||
                   _body[KEY_OPTYPE].isIntegral()==false){
                    ELOG("主题请求中没有操作类型或操作类型类型错误");
                    return false;
                }
                if(_body[KEY_OPTYPE].asInt()==(int)TopicOpType::TOPIC_PUBLISH &&  /*若是发布消息，需要判断主题消息是否存在*/
                  (_body[KEY_TOPIC_MSG].isNull()==true ||
                   _body[KEY_TOPIC_MSG].isString()==false)){
                    ELOG("主题消息发布请求中没有消息内容字段或消息内容类型错误");
                    return false;
                }
                return true;
            }
            std::string getTopickey(){
                return _body[KEY_TOPIC_KEY].asString();
            }
            void setTopickey(const std::string &key){
                _body[KEY_TOPIC_KEY] = key;
            }

            TopicOpType getOptype(){
                return (TopicOpType)_body[KEY_OPTYPE].asInt();
            }
            void setOptype(TopicOpType optype){
                _body[KEY_OPTYPE] = (int)optype;
            }

            std::string getTopicMsg(){
                return _body[KEY_TOPIC_KEY].asString();
            } 
            void setTopicMsg(const std::string &msg){
                _body[KEY_TOPIC_KEY] = msg;
            }
    };


    typedef std::pair<std::string,int>Address;
    class ServiceRequest : public JsonRequest{
        public:
            using ptr = std::shared_ptr<ServiceRequest>;
            virtual bool checkValid() const override{
                if(_body[KEY_METHOD].isNull()==true ||
                   _body[KEY_METHOD].isString()==false){
                    ELOG("服务请求中没有方法名称或方法名称类型错误");
                    return false;
                }
                if(_body[KEY_OPTYPE].isNull()==true ||
                   _body[KEY_OPTYPE].isIntegral()==false){
                    ELOG("服务请求中没有操作类型或操作类型类型错误");
                    return false;
                }

                //判断主机地址信息是否存在，是否正确
                if(_body[KEY_HOST].isNull()==true ||
                   _body[KEY_HOST].isObject()==false/*若false则不是对象字段*/||
                   _body[KEY_HOST_IP].isNull()==true ||
                   _body[KEY_HOST_IP].isString()==false ||
                   _body[KEY_HOST_PORT].isNull()==true ||
                   _body[KEY_HOST_PORT].isIntegral()==false 
                   ){  
                    ELOG("服务请求中主机地址信息错误");
                    return false;
                }
                return true;
            }
            
            std::string getMethod(){
                return _body[KEY_METHOD].asString();
            }
            void setMethod(const std::string &method_name){
                _body[KEY_METHOD] = method_name;
            }

            ServiceOpType getOptype(){
                return (ServiceOpType)_body[KEY_OPTYPE].asInt();
            }
            void setOptype(ServiceOpType optype){
                _body[KEY_OPTYPE] = (int)optype;
            }

            Address getHost(){
                Address addr;
                addr.first = _body[KEY_HOST][KEY_HOST_IP].asString();  // 先访问host对象，再访问ip字段
                addr.second = _body[KEY_HOST][KEY_HOST_PORT].asInt();  // 先访问host对象，再访问port字段
                return addr;
            }
            void setHost(const Address &host){
                Json::Value val;
                val[KEY_HOST_IP] = host.first;
                val[KEY_HOST_PORT] = host.second;
                _body[KEY_HOST] = val;
            }

    };

    class RpcResponse : public JsonResponse{
    public:
        using ptr = std::shared_ptr<RpcResponse>;
        virtual bool checkValid() const override{
            if(_body[KEY_RECODE].isNull()==true ||
               _body[KEY_RECODE].isIntegral()==false){
                ELOG("响应中没有响应状态码或响应状态码类型错误");
                return false;
            }
            if(_body[KEY_RESULT].isNull()==true ||
               _body[KEY_RESULT].isObject()==false){
                ELOG("响应中没有响应结果或响应结果类型错误");
                return false;
            }
            return true;
        }

        RCode getRcode(){
            return (RCode)_body[KEY_RECODE].asInt();
        }
        void setRcode(RCode rcode){
            _body[KEY_RECODE] = (int)rcode;
        }

        Json::Value getResult() {
            return _body[KEY_RESULT];
        }
        void setResult(const Json::Value &result) {
                _body[KEY_RESULT] = result;
        }
    };
    
    class ServiceResponse : public JsonResponse {
        public:
            using ptr = std::shared_ptr<ServiceResponse>;
            virtual bool checkValid() const override {
                if (_body[KEY_RECODE].isNull() == true ||
                    _body[KEY_RECODE].isIntegral() == false) {
                    ELOG("响应中没有响应状态码,或状态码类型错误！");
                    return false;
                }
                if (_body[KEY_OPTYPE].isNull() == true ||
                    _body[KEY_OPTYPE].isIntegral() == false) {
                    ELOG("响应中没有操作类型,或操作类型的类型错误！");
                    return false;
                }

                
                if (_body[KEY_OPTYPE].asInt() == (int)(ServiceOpType::SERVICE_DISCOVERY) &&
                   (_body[KEY_METHOD].isNull() == true ||
                    _body[KEY_METHOD].isString() == false ||
                    _body[KEY_HOST].isNull() == true ||
                    _body[KEY_HOST].isArray() == false)) {
                    ELOG("服务发现响应中响应信息字段错误！");
                    return false;
                }
                return true;
            }
            ServiceOpType optype() {
                return (ServiceOpType)_body[KEY_OPTYPE].asInt();
            }
            void setOptype(ServiceOpType optype) {
                _body[KEY_OPTYPE] = (int)optype;
            }
            std::string method() {
                return _body[KEY_METHOD].asString();
            }
            void setMethod(const std::string &method) {
                _body[KEY_METHOD] = method;
            }
            void setHost(std::vector<Address> addrs) {
                for (auto &addr : addrs) {
                    Json::Value val;
                    val[KEY_HOST_IP] = addr.first;
                    val[KEY_HOST_PORT] = addr.second;
                    _body[KEY_HOST].append(val);
                }
            }
            std::vector<Address> getHosts() {
                std::vector<Address> addrs;
                int sz = _body[KEY_HOST].size();
                for (int i = 0; i < sz; i++) {
                    Address addr;
                    addr.first = _body[KEY_HOST][i][KEY_HOST_IP].asString();
                    addr.second = _body[KEY_HOST][i][KEY_HOST_PORT].asInt();
                    addrs.push_back(addr);
                }
                return addrs;
            }
    };

    class TopicResponse : public JsonResponse {
        public:
            using ptr = std::shared_ptr<TopicResponse>;
    };

    //实现一个消息对象的生产工厂
    //用一个简单工厂模式，根据消息类型创建不同的消息对象
    class MessageFactory {
        public:
            static BaseMessage::ptr create(MType mtype) {
                switch(mtype) {
                    case MType::REQ_RPC : return std::make_shared<RpcRequest>();
                    case MType::RSP_RPC : return std::make_shared<RpcResponse>();
                    case MType::REQ_TOPIC : return std::make_shared<TopicRequest>();
                    case MType::RSP_TOPIC : return std::make_shared<TopicResponse>();
                    case MType::REQ_SERVICE : return std::make_shared<ServiceRequest>();
                    case MType::RSP_SERVICE : return std::make_shared<ServiceResponse>();
                }
                return BaseMessage::ptr();
            }
            
            template<typename T, typename ...Args>
            static std::shared_ptr<T> create(Args&& ...args) {
                return std::make_shared<T>(std::forward(args)...);
            }
    };
}








