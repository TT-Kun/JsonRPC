/*用于测试common下的net.hpp、message.hpp、dispatcher.hpp*/
#include "../common/net.hpp"
#include "../common/message.hpp"
#include "../common/dispatcher.hpp"

void onRpcResponse(const MRPC::BaseConnection::ptr &conn,const MRPC::BaseMessage::ptr &msg){
    std::cout << "收到了Rpc响应：" << std::endl;
    std::cout << msg->serialize() << std::endl;
}

void onTopicResponse(const MRPC::BaseConnection::ptr &conn,const MRPC::BaseMessage::ptr &msg){
    std::cout << "收到了Topic响应：" << std::endl;
    std::cout << msg->serialize() << std::endl;
}

int main(){
    auto dispatcher = std::make_shared<MRPC::Dispatcher>();

    dispatcher->registerHandler<MRPC::RpcResponse>(MRPC::MType::RSP_RPC,onRpcResponse);
    dispatcher->registerHandler<MRPC::TopicResponse>(MRPC::MType::RSP_TOPIC,onTopicResponse);
    //注册消息类型和回调之间的映射关系

    auto client = MRPC::ClientFactory::create("127.0.0.1",9000);
    auto message_cb=std::bind(&MRPC::Dispatcher::onMessage,dispatcher.get(),

    std::placeholders::_1,std::placeholders::_2);
    client->setMessageCallback(message_cb);
    client->connect();


    auto rpc_req = MRPC::MessageFactory::create<MRPC::RpcRequest>();
    rpc_req->set_Id("11");
    rpc_req->set_MType(MRPC::MType::REQ_RPC);
    rpc_req->setMethod("add");
    rpc_req->setParams(2233);
    Json::Value params;
    params["a"] = 1;
    params["b"] = 2;
    rpc_req->setParams(params);
    client->sendMessage(rpc_req);

    auto topic_req = MRPC::MessageFactory::create<MRPC::TopicRequest>();
    topic_req->set_Id("22");
    topic_req->set_MType(MRPC::MType::REQ_TOPIC);
    topic_req->setOptype(MRPC::TopicOpType::TOPIC_CREATE);
    topic_req->setTopickey("test");
    topic_req->setTopicMsg("content");
    client->sendMessage(topic_req);

    std::this_thread::sleep_for(std::chrono::seconds(10));
    client->shutdown();
    return 0;
}
