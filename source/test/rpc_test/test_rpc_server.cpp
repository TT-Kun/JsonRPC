#include "../../common/net.hpp"
#include "../../common/message.hpp"
#include "../../common/dispatcher.hpp"
#include "../../server/rpcRouter.hpp"


void Add(const Json::Value& req,Json::Value& rsp){
    int num1 = req["num1"].asInt();
    int num2 = req["num2"].asInt();
    rsp["result"] = num1 + num2;
}

int main(){

    auto dispatcher = std::make_shared<MRPC::Dispatcher>();

    auto router = std::make_shared<MRPC::server::RpcRouter>();

    std::unique_ptr<MRPC::server::ServDescFactory> desc_factory(new MRPC::server::ServDescFactory());
    desc_factory->setMethodName("Add");
    desc_factory->setParamsDesc("num1",MRPC::server::VType::INTEGRAL);
    desc_factory->setParamsDesc("num2",MRPC::server::VType::INTEGRAL);
    desc_factory->setReturnType(MRPC::server::VType::OBJECT);

    desc_factory->setCallback(Add);
    router->registerService(desc_factory->build());

    auto cb = std::bind(&MRPC::server::RpcRouter::onRpcRequest,router.get(),
    std::placeholders::_1,std::placeholders::_2);
    dispatcher->registerHandler<MRPC::RpcRequest>(MRPC::MType::REQ_RPC,cb);
    //dispatcher->registerHandler<MRPC::TopicRequest>(MRPC::MType::REQ_TOPIC,cb);

    auto message_cb=std::bind(&MRPC::Dispatcher::onMessage,dispatcher.get(),
    std::placeholders::_1,std::placeholders::_2);
    auto server = MRPC::ServerFactory::create(9000);
    server->setMessageCallback(message_cb);
    server->start();

    return 0;
}
