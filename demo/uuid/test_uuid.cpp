#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <sstream>
#include <iomanip>
#include <atomic>

// UUID生成类

/*
使用高质量随机源(std::random_device)作为种子
通过梅森旋转算法(std::mt19937)生成伪随机数序列
均匀分布在0-255范围内的随机数，对应一个字节
生成8个随机字节，并格式化为16进制字符串

使用原子计数器确保线程安全地获取唯一序号
将64位序号分解成8个字节
通过位移操作(cur >> (i*8)) & 0xff提取每个字节
将每个字节格式化为两位16进制数并追加到字符串中
*/

class UUID {
private:
    static std::atomic<size_t> seq;

public:
    static std::string generate() {
        // 随机部分
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        std::stringstream ss;
        
        // 生成前8个随机字节
        for (int i = 0; i < 8; i++) {
            if(i==4||i==6)  ss << "-";
            ss << std::hex << std::setw(2) << std::setfill('0') << dis(gen);
        }
            //std::hex是一个流操纵器，用于将整数转换为十六进制字符串
            //同时为了保证字符串长度为8，使用std::setw(2)和std::setfill('0')来设置宽度为2，不足时用0填充
            //这是第一部分生成前八字节的随机部分，
        
        // 序列号部分
        size_t cur = seq.fetch_add(1);
        ss << "-";
        // 将序列号分解为8个字节
        for(int i=7; i>=0; i--) {
            if(i==5)  ss << "-";
            ss << std::hex << std::setw(2) << std::setfill('0') << ((cur >> (i*8)) & 0xff);
        }
            //std::atomic<size_t>是一个原子类型，用于保证线程安全
            //fetch_add(1)是一个原子操作，用于将seq的值增加1
            //seq是全局的序号，用于保证序号的唯一性
            //这行代码的作用是获取当前的序号，并将其增加1，保证原子性的原因是：
            //多线程环境下，需要保证序号的唯一性、原子性、线程安全、并发性、性能、可靠性、可维护性、可扩展性
        return ss.str();
    }
};

// 初始化静态成员变量
std::atomic<size_t> UUID::seq(0);

int main() {
    // 测试1: 生成10个UUID并打印
    std::cout << "=== 测试1: 生成10个UUID ===" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::string uuid = UUID::generate();
        std::cout << "UUID " << i+1 << ": " << uuid << std::endl;
    }
    
    // 测试2: 检查唯一性 (生成1000个UUID并检查是否有重复)
    std::cout << "\n=== 测试2: 检查UUID唯一性 (1000个) ===" << std::endl;
    std::set<std::string> uuids;
    for (int i = 0; i < 1000; i++) {
        std::string uuid = UUID::generate();
        uuids.insert(uuid);
    }
    
    std::cout << "生成的UUID数量: 1000" << std::endl;
    std::cout << "唯一UUID数量: " << uuids.size() << std::endl;
    std::cout << "重复UUID数量: " << 1000 - uuids.size() << std::endl;
    
    // 测试3: 验证序列号部分是递增的
    std::cout << "\n=== 测试3: 验证序列号部分是递增的 ===" << std::endl;
    std::vector<std::string> seq_uuids;
    for (int i = 0; i < 5; i++) {
        std::string uuid = UUID::generate();
        seq_uuids.push_back(uuid);
        // 提取序列号部分(后16个字符)
        std::string seq_part = uuid.substr(16);
        std::cout << "UUID " << i+1 << " 序列号部分: " << seq_part << std::endl;
    }
    
    return 0;
} 