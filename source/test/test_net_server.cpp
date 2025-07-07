#include "../net.hpp"
#include "../message.hpp"
#include "../dispatcher.hpp"

void onRpcRequest(const MRPC::BaseConnection::ptr &conn,const MRPC::BaseMessage::ptr &msg){
    std::cout << "收到了RPC请求" << std::endl;
    std::cout << msg->serialize() << std::endl;
    auto rpc_req = MRPC::MessageFactory::create<MRPC::RpcResponse>();
    rpc_req->set_Id("11");
    rpc_req->set_MType(MRPC::MType::RSP_RPC);
    rpc_req->setRcode(MRPC::RCode::RCODE_OK);
    rpc_req->setResult(2233);
    conn->sendMessage(rpc_req);
}

void onTopicRequest(const MRPC::BaseConnection::ptr &conn,const MRPC::BaseMessage::ptr &msg){
    std::cout << "收到了Topic请求" << std::endl;
    std::cout << msg->serialize() << std::endl;
    auto topic_req = MRPC::MessageFactory::create<MRPC::TopicResponse>();
    topic_req->set_Id("11");
    topic_req->set_MType(MRPC::MType::RSP_TOPIC);
    topic_req->setReCode(MRPC::RCode::RCODE_OK);
    conn->sendMessage(topic_req);
}

int main(){

    auto dispatcher = std::make_shared<MRPC::Dispatcher>();

    dispatcher->registerHandler<MRPC::RpcRequest>(MRPC::MType::REQ_RPC,onRpcRequest);
    dispatcher->registerHandler<MRPC::TopicRequest>(MRPC::MType::REQ_TOPIC,onTopicRequest);
    //注册映射关系
    auto message_cb=std::bind(&MRPC::Dispatcher::onMessage,dispatcher.get(),
    std::placeholders::_1,std::placeholders::_2);
    auto server = MRPC::ServerFactory::create(9000);
    server->setMessageCallback(message_cb);
    server->start();

    return 0;
}
