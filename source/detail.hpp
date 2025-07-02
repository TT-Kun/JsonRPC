/*
实现项目中用到的一些琐碎代码
1.json的序列化和反序列化
2.日志宏的实现
3.uuid的生成
*/

#pragma once

#include "detail.hpp"
#include <cstdio>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>
#include <iomanip>
#include <atomic>

namespace MRPC{
    #define LDBG 0
    #define LINF 1
    #define LERR 2

    #define LDEFAULT LDBG

    #define LOG(level, format, ...) {\
        if (level >= LDEFAULT){\
            time_t t = time(NULL);\
            struct tm *lt = localtime(&t);\
            char time_tmp[32] = {0};\
            strftime(time_tmp, 31, "%m-%d %T", lt);\
            fprintf(stdout, "[%s][%s:%d] " format "\n", time_tmp, __FILE__, __LINE__, ##__VA_ARGS__);\
        }\
    }
    #define DLOG(format, ...) LOG(LDBG, format, ##__VA_ARGS__)
    #define ILOG(format, ...) LOG(LINF, format, ##__VA_ARGS__)
    #define ELOG(format, ...) LOG(LERR, format, ##__VA_ARGS__)


        class JSON {
        public:
            static bool serialize(const Json::Value &val,  std::string &body) {
                std::stringstream ss;
                //先实例化一个工厂类对象
                Json::StreamWriterBuilder swb;
                //通过工厂类对象来生产派生类对象
                std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
                int ret = sw->write(val, &ss);
                if (ret != 0) {
                    ELOG("json serialize failed!");
                    return false;
                }
                body = ss.str();
                return true;
            }

            static bool unserialize(const std::string &body, Json::Value &val) {
                Json::CharReaderBuilder crb;
                std::string errs;
                std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
                bool ret = cr->parse(body.c_str(), body.c_str() + body.size(), &val, &errs);
                if (ret == false) {
                    ELOG("json unserialize failed : %s", errs.c_str());
                    return false;
                }
                return true;
            }
        };

        class UUID {
        public:
            static std::string uuid() {

            //1.使用std::random_device构造一个机器随机数对象，确保随机数的生成质量，保证极大可能不重复
            std::random_device rd;
            //2.以机器随机数当作种子来生成伪随机数
            std::mt19937 gen(rd());
            //3.限定数据范围
            std::uniform_int_distribution<> dis(0, 255);
            //4.生成8个随机数，组织成十六进制数字字符的字符串
            std::stringstream ss;
            for (int i = 0; i < 8; i++) {
                ss << std::hex << std::setw(2) << std::setfill('0') << dis(gen);
            }
            ss << "-";
            //5.定义一个8字节的序号，逐字节组织成十六进制数字字符的字符串
            static std::atomic<size_t> seq;
            size_t cur = seq.fetch_add(1);//当前的序号

            for(int i=7; i>=0; i--) {
                if(i==5)  ss << "-";
                ss << std::hex << std::setw(2) << std::setfill('0') << std::hex << (cur >> (i*8)) & 0xff;
            }
            return ss.str();
            }
        };
}

