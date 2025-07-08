#include "../common/message.hpp"
#include "../common/dispatcher.hpp"

namespace MRPC{
    //多加一层命名空间，和客户端做区别
    namespace server{
        enum class VType {
            BOOL = 0,
            INTEGRAL,
            NUMERIC,
            STRING,
            ARRAY,
            OBJECT,
        };

        class ServDiscribe{
            public:
                using ptr = std::shared_ptr<ServDiscribe>;
                using ServiceCallback = std::function<void(const Json::Value& ,Json::Value&)>;
                using ParamDesc = std::pair<std::string,VType>;
                ServDiscribe(const std::string &method_name,const std::vector<ParamDesc> &&params,
                    VType return_type,const ServiceCallback &&callback):
                    _method_name(method_name),_params(std::move(params)),
                    return_type(return_type),_callback(std::move(callback))
                {}


                bool paramCheck(const Json::Value &params)//参数校验函数
                {
                    for(auto &param : _params){
                        if(params.isMember(param.first)==false){
                            ELOG("参数校验失败，缺少参数：%s", param.first.c_str());
                            return false;
                        }
                        if(check(params[param.first],param.second)==false){
                            ELOG("参数校验失败，参数类型不匹配：%s", param.first.c_str());
                            return false;
                        }
                    }
                    return true;
                }

                bool returnTypeCheck(const Json::Value &val){
                    return check(val,return_type);
                }

                const std::string &methodName() {
                    return _method_name;
                }

                bool call(const Json::Value &params,Json::Value &result){
                    _callback(params,result);
                    if(returnTypeCheck(result)==false){
                        ELOG("返回值类型校验失败");
                        return false;
                    }
                    return true;
                }
                
            private:
                bool check(const Json::Value &val,VType type){
                    //检查参数类型辅助函数
                    switch(type){
                        case VType::BOOL: return val.isBool();
                        case VType::INTEGRAL: return val.isIntegral();
                        case VType::NUMERIC: return val.isNumeric();
                        case VType::STRING: return val.isString();
                        case VType::ARRAY: return val.isArray();
                        case VType::OBJECT: return val.isObject();
                        default: return false;
                    }
                }
            private:
                std::string _method_name;
                ServiceCallback _callback; //回调
                std::vector<ParamDesc> _params; //参数描述
                VType return_type; //返回值类型
        };

        class ServDescFactory {
            public:
                void setMethodName(const std::string &name) {
                    _method_name = name;
                }
                void setReturnType(VType vtype) {
                    _return_type = vtype;
                }
                void setParamsDesc(const std::string &pname, VType vtype) {
                    _params_desc.push_back(ServDiscribe::ParamDesc(pname, vtype));
                }
                void setCallback(const ServDiscribe::ServiceCallback &cb) {
                    _callback = cb;
                }
                ServDiscribe::ptr build() {
                    return std::make_shared<ServDiscribe>(std::move(_method_name), 
                        std::move(_params_desc), _return_type, std::move(_callback));
                }
            private:
                std::string _method_name;
                ServDiscribe::ServiceCallback _callback;  // 实际的业务回调函数
                std::vector<ServDiscribe::ParamDesc> _params_desc; // 参数字段格式描述
                VType _return_type; //结果作为返回值类型的描述
        };
        //写一个工厂用来创建服务描述，严格来说是建造者模式，构造出来的对象的各项数据完整且不可修改，更安全稳定

        class ServiceManager{
            //封装一下用来校验的函数
            public:
                using ptr = std::shared_ptr<ServiceManager>;
                void insertService(const ServDiscribe::ptr &service){
                    //
                    std::unique_lock<std::mutex> lock(_mutex);
                    _services.insert(std::make_pair(service->methodName(),service));
                }
                ServDiscribe::ptr selectService(const std::string &method_name){
                    std::lock_guard<std::mutex> lock(_mutex);
                    auto it = _services.find(method_name);
                    if(it == _services.end()){
                        return ServDiscribe::ptr();
                    }
                    return it->second;
                }

                void removeService(const std::string &method_name){
                    std::unique_lock<std::mutex> lock(_mutex);
                    _services.erase(method_name);
                }

            private:
                std::mutex _mutex;
                std::unordered_map<std::string,ServDiscribe::ptr> _services;
        };

        class RpcRouter{
            public:
            //注册到Dispatcher模块针对rpc请求进行回调处理
                using ptr = std::shared_ptr<RpcRouter>;
                RpcRouter(){
                    _service_manager = std::make_shared<ServiceManager>();
                }
                void onRpcRequest(const BaseConnection::ptr &conn, RpcRequest::ptr &request){
                    //判断服务端是否注册了该服务->如果注册了，进行参数校验->通过后调用服务端回调函数->组织相应，发送回客户端
                    auto service = _service_manager->selectService(request->getMethod());
                    if(service.get()==nullptr){
                        ELOG("未找到对应的服务，方法名：%s", request->getMethod().c_str());
                        return response(conn,request,Json::Value(),RCode::RCODE_NOT_FOUND_SERVICE);
                    }
                    if(service->paramCheck(request->getParams())==false){
                        ELOG("参数校验失败，方法名：%s", request->getMethod().c_str());
                        return response(conn,request,Json::Value(),RCode::RCODE_INVALID_PARAMS);
                    }

                    Json::Value result;
                    service->call(request->getParams(),result);
                    bool ret = service->returnTypeCheck(result);
                    if(ret==false){
                        ELOG("返回值类型校验失败，方法名：%s", request->getMethod().c_str());
                        return response(conn,request,Json::Value(),RCode::RCODE_INTERNAL_ERROR);
                    }
                    
                    // 调用成功后发送响应
                    response(conn,request,result,RCode::RCODE_OK);
                }
                void registerService(const ServDiscribe::ptr &service)
                {
                    return _service_manager->insertService(service);
                }
            private:
                void response(const BaseConnection::ptr &conn,
                const RpcRequest::ptr &request,
                const Json::Value &resp,RCode rcode){
                    //构造响应对象进行返回
                    auto msg = MessageFactory::create<RpcResponse>();
                    msg->set_Id(request->get_Id());
                    msg->set_MType(MType::RSP_RPC);
                    msg->setRcode(rcode);
                    msg->setResult(resp);
                    conn->sendMessage(msg);
                }
            private:
                ServiceManager::ptr _service_manager;
        };
    }
}

