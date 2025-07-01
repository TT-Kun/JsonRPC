#include "json_util.hpp"
#include <sstream>
#include <memory>
#include <iostream>

bool json_util::serialize(const Json::Value &root, std::string &str)
{
    Json::StreamWriterBuilder swb;
    std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());

    std::stringstream ss;
    int ret = sw->write(root, &ss);
    if (ret != 0) {
        std::cout << "Serialize failed!" << std::endl;
        return false;
    }
    str = ss.str();
    return true;
}

bool json_util::unserialize(const std::string &str, Json::Value &root)
{
    Json::CharReaderBuilder crb;
    std::unique_ptr<Json::CharReader> cr(crb.newCharReader());

    bool ret = cr->parse(
        str.c_str(),
        str.c_str() + str.size(),
        &root,
        nullptr
    );
    if (!ret) {
        std::cout << "Unserialize failed!" << std::endl;
        return false;
    }
    return true;
}