#include "json_util.hpp"
#include <iostream>

int main()
{
    // 构建 JSON 数据
    Json::Value data;
    data["name"] = "zhangsan";
    data["age"] = 19;
    data["scores"] = Json::arrayValue; // 创建数组
    data["scores"].append(85);
    data["scores"].append(90);
    data["scores"].append(95);

    // 序列化
    std::string jsonStr;
    if (json_util::serialize(data, jsonStr))
    {
        std::cout << "序列化结果:\n"
                  << jsonStr << "\n";
    }
    else
    {
        std::cerr << "序列化失败\n";
        return 1;
    }

    // 反序列化
    Json::Value parsedData;
    if (json_util::unserialize(jsonStr, parsedData))
    {
        std::cout << "\n反序列化结果:\n";
        std::cout << "name: " << parsedData["name"].asString() << "\n";
        std::cout << "age: " << parsedData["age"].asInt() << "\n";
        std::cout << "scores: ";

        const Json::Value &scoresArray = parsedData["scores"];
    for (size_t i = 0; i < scoresArray.size(); ++i) {
        std::cout << scoresArray[Json::Value::ArrayIndex(i)].asInt() << " ";
    }
        std::cout << "\n";
    }
    else
    {
        std::cerr << "反序列化失败\n";
        return 1;
    }

    return 0;
}