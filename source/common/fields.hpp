#pragma once
/*
本文件主要用于声明字段、枚举类型、静态函数等
*/
#include <map>
#include <string>


namespace MRPC{
    #define KEY_METHOD "method"      // RPC请求的方法名称
    #define KEY_PARAMS "params"      // RPC请求的方法参数
    #define KEY_TOPIC_KEY "id"       // 消息ID
    #define KEY_TOPIC_MSG "msg"      // 发布订阅请求的主题消息
    #define KEY_TOPIC_DATA "data"    // 发布订阅请求的数据
    #define KEY_OPTYPE "optype"      // 消息类型
    #define KEY_HOST "host"          // 主机信息
    #define KEY_HOST_IP "ip"         // 主机IP地址
    #define KEY_HOST_PORT "port"     // 主机PORT端口
    #define KEY_RECODE "recode"      // 响应码
    #define KEY_RESULT "result"      // RPC返回结果
    //以上是请求里的字段

    enum class MType{
        REQ_RPC = 0,     // RPC请求
        RSP_RPC,         // RPC响应
        REQ_TOPIC,       // 主题操作请求
        RSP_TOPIC,       // 主题操作响应
        REQ_SERVICE,     // 服务操作请求
        RSP_SERVICE,     // 服务操作响应
    };
    //以上是请求响应的消息类型

    enum class RCode {
        RCODE_OK = 0,              // 成功处理
        RCODE_PARSE_FAILED,        // 解析失败
        RCODE_ERROR_MSGTYPE,       // 消息类型错误
        RCODE_INVALID_MSG,         // 无效消息
        RCODE_DISCONNECTED,        // 连接断开
        RCODE_INVALID_PARAMS,      // 无效的RPC调用参数
        RCODE_NOT_FOUND_SERVICE,   // RPC服务不存在
        RCODE_INVALID_OPTYPE,      // 无效的Topic操作类型
        RCODE_NOT_FOUND_TOPIC,     // 主题不存在
        RCODE_INTERNAL_ERROR,      // 内部错误
    };
    //以上是响应码

    static std::string error_msg(RCode code){
        static std::map<RCode, std::string> error_map = {
            {RCode::RCODE_OK, "成功处理！"},
            {RCode::RCODE_PARSE_FAILED, "消息解析失败！"},
            {RCode::RCODE_ERROR_MSGTYPE, "消息类型错误！"},
            {RCode::RCODE_INVALID_MSG, "无效消息"},
            {RCode::RCODE_DISCONNECTED, "连接已断开！"},
            {RCode::RCODE_INVALID_PARAMS, "无效的Rpc参数！"},
            {RCode::RCODE_NOT_FOUND_SERVICE, "没有找到对应的服务！"},
            {RCode::RCODE_INVALID_OPTYPE, "无效的操作类型"},
            {RCode::RCODE_NOT_FOUND_TOPIC, "内部错误！"}
        };
        auto it = error_map.find(code);
        if(it == error_map.end()){
            return "未知错误！";
        }
        return it->second;
    }
    //以上是错误原因的映射


    enum class ReqType{
        REQ_SYNC = 0,       // 同步请求
        REQ_ASYNC,          // 异步请求
        REQ_CALLBACK,       // 回调请求
    };
    //以上是RPC请求类型


    enum class TopicOpType{
        TOPIC_CREATE = 0,   // 创建主题
        TOPIC_REMOVE,       // 删除主题
        TOPIC_SUBSCRIBE,    // 订阅主题
        TOPIC_CANCEL,       // 取消订阅
        TOPIC_PUBLISH,      // 发布主题
    };
    //主题操作类型

    enum class ServiceOpType{
        SERVICE_REGISTRY = 0,   // 注册服务
        SERVICE_DISCOVERY,      // 发现服务
        SERVICE_ONLINE,         // 上线
        SERVICE_OFFLINE,        // 下线
        SERVICE_UNKNOW          // 未知
    };
    //主题服务类型
}