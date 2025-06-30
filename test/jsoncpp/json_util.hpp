#ifndef JSON_UTIL_H
#define JSON_UTIL_H

#include <jsoncpp/json/json.h>
#include <string>

class json_util
{
public:
    // 序列化：Json对象 -> 字符串
    static bool serialize(const Json::Value &root, std::string &str);

    // 反序列化：字符串 -> Json对象
    static bool unserialize(const std::string &str, Json::Value &root);
};  

#endif // JSON_UTIL_H