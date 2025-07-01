#include<iostream>
#include<future>
#include<thread>
#include<memory>

int Add(int num1,int num2){
    return num1 + num2;
}

int main(){
    std::promise<int> prom;
    //实例化一个promise对象
    std::future<int> result = prom.get_future();
    //获取promise对象关联的future对象
    std::thread thr([&prom](){
        prom.set_value(Add(11,22));
        //设置promise对象的值
    });

    std::cout<<result.get()<<std::endl;
    thr.join();

    return 0;
}

