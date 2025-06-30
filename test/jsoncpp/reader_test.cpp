#include <jsoncpp/json/json.h>
#include <fstream>
#include <iostream>
using namespace std;
using namespace Json;

int main()
{
    // 从文件读取 JSON 数据
    ifstream file("output.json");
    if (!file.is_open())
    {
        cerr << "Error: 无法打开文件 output.json" << endl;
        return -1;
    }

    // 解析 JSON 内容
    Value root;
    CharReaderBuilder builder;
    string errors;

    bool parseSuccess = parseFromStream(builder, file, &root, &errors);
    file.close();

    if (!parseSuccess)
    {
        cerr << "JSON 解析失败:\n"
             << errors << endl;
        return -1;
    }

    // 读取基本类型字段
    string name = root["姓名"].asString();
    int age = root["年龄"].asInt();

    // 读取数组
    const Value &scoresArray = root["成绩"];
    vector<double> scores;
    scores.reserve(scoresArray.size());

    for (size_t i = 0; i < scoresArray.size(); ++i)
    {
        scores.push_back(scoresArray[Json::Value::ArrayIndex(i)].asDouble());
    }

    // 输出读取结果
    cout << "读取结果:" << endl;
    cout << "姓名: " << name << endl;
    cout << "年龄: " << age << endl;
    cout << "成绩: ";
    for (double score : scores)
    {
        cout << score << " ";
    }
    cout << endl;

    // 可选：验证数据完整性
    if (root.isMember("姓名") && root["姓名"].isString() &&
        root.isMember("年龄") && root["年龄"].isInt())
    {
        cout << "JSON 结构完整" << endl;
    }
    else
    {
        cout << "JSON 结构不完整" << endl;
    }

    return 0;
}