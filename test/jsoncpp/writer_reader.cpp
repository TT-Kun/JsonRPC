#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>


int main()
{
    // 序列化（将 C++ 数据结构转为 JSON 字符串）
    Json::Value stu;
    stu["name"] = "zhangsan";
    stu["age"] = 19;
    stu["socre"].append(77.5);
    stu["socre"].append(88);
    stu["socre"].append(99.5);

    Json::StreamWriterBuilder swb;
    std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());

    std::stringstream ss;
    int ret = sw->write(stu, &ss);
    if (ret != 0) {
        std::cout << "Serialize failed!\n";
        return -1;
    }
    std::cout << "序列化结果:\n" << ss.str() << std::endl;

    // 反序列化（将 JSON 字符串转回 C++ 数据结构）
    std::string str = ss.str();
    Json::Value root;
    Json::CharReaderBuilder crb;
    std::unique_ptr<Json::CharReader> cr(crb.newCharReader());

    bool ret1 = cr->parse(str.c_str(), str.c_str() + str.size(), &root, nullptr);
    if (!ret1) {
        std::cout << "UnSerialize failed!" << std::endl;
        return -1;
    }

    std::cout << "反序列化结果:\n"
              << "name:" << root["name"].asString() << "\n"
              << "age:" << root["age"].asInt() << "\n"
              << "socre:" 
              << root["socre"][0].asFloat() << " " 
              << root["socre"][1].asInt() << " " 
              << root["socre"][2].asFloat() << "\n";

    return 0;
}