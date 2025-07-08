#include "../../common/net.hpp"
#include "../../common/message.hpp"
#include "../../common/dispatcher.hpp"
#include "../../server/rpcRouter.hpp"
#include "../../client/rpcCaller.hpp"

int main(){

    auto request_manager = std::make_shared<MRPC::client::RequestManager>();
    auto caller = std::make_shared<MRPC::client::RpcCaller>(request_manager);
    
    auto dispatcher = std::make_shared<MRPC::Dispatcher>();
    
    // 使用lambda修复参数顺序不匹配问题
    auto cb = [request_manager](const MRPC::BaseConnection::ptr &conn, MRPC::BaseMessage::ptr &msg) {
        request_manager->onResponse(conn, msg);
    };

    dispatcher->registerHandler<MRPC::BaseMessage>(MRPC::MType::RSP_RPC, cb);
    //dispatcher->registerHandler<MRPC::TopicResponse>(MRPC::MType::RSP_TOPIC,cb);
    //注册消息类型和回调之间的映射关系

    auto client = MRPC::ClientFactory::create("127.0.0.1",9000);
    auto message_cb=std::bind(&MRPC::Dispatcher::onMessage,dispatcher.get(),
    std::placeholders::_1,std::placeholders::_2);
    client->setMessageCallback(message_cb);
    client->connect();

    auto conn = client->getConnection();
    Json::Value params,result;
    params["num1"] = 11;
    params["num2"] = 22;
    bool ret = caller->call(conn,"Add",params,result);
    if(ret){
        std::cout << "result:" << result["result"].asInt() << std::endl;
    }
    else{
        std::cout << "调用失败" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
    client->shutdown();
    return 0;
}
