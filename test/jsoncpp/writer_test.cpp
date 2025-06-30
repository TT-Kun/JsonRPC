#include <jsoncpp/json/json.h>
#include <fstream>
#include <iostream>
using namespace std;
using namespace Json;
int main()
{
    Json::Value data;
    string name = "张三";
    int age = 25;
    int scores[3] = {85, 90, 95};

    data["姓名"] = name;
    data["年龄"] = age;
    data["成绩"][0] = scores[0];
    data["成绩"][1] = scores[1];
    data["成绩"][2] = scores[2];
    data["成绩"].append(80);

    // 创建 FastWriter 对象（紧凑格式生成器）
    // 特点：无缩进、无换行，所有内容压缩在一行，数据体积最小
    Json::FastWriter fastWriter;
    // 将 JSON 数据序列化为紧凑格式字符串
    std::string compactJson = fastWriter.write(data);
    // 输出结果示例：{"姓名":"张三","年龄":25,"成绩":[85,90,95]}
    std::cout << "Compact JSON:\n"<< compactJson << "\n";


    // 创建 StyledWriter 对象（格式化输出生成器）
    // 特点：自动添加缩进和换行，层级结构清晰，便于人工阅读
    Json::StyledWriter styledWriter;
    // 将 JSON 数据序列化为格式化字符串
    std::string formattedJson = styledWriter.write(data);
    // 输出结果示例：
    // {
    //    "姓名" : "张三",
    //    "年龄" : 25,
    //    "成绩" : [
    //       85,
    //       90,
    //       95
    //    ]
    // }
    std::cout << "Formatted JSON:\n"<< formattedJson << "\n";

    // 打开文件流（用于写入 JSON 数据）
    std::ofstream file("output.json");
    // 创建流式写入器（适合大文件，避免内存溢出）
    Json::StyledStreamWriter streamWriter;
    // 将 JSON 数据格式化为流式输出并写入文件
    streamWriter.write(file, data);
    // 关闭文件流
    file.close();

    return 0;
}