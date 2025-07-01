#include <iostream>
#include <future>
#include <thread>

int Add(int num1,int num2){
    //以异步执行加法运算为例子
    std::cout << "Add函数开始执行" << std::endl;
    return num1 + num2;
}

int main(){
    std::future<int> result = std::async(std::launch::async,Add,11,22);
    //创建一个异步任务，异步任务执行Add函数，参数为11和22
    //返回值是一个future对象，future对象可以获取异步任务的返回值
    //进行了一个异步非阻塞调用
    //如果将async换成deferred，则不会立即执行Add函数，而是等到get函数被调用时才会执行Add函数
    //std::future<int> result = std::async(std::launch::deferred,Add,11,22);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout<<"----------------------------------\n";


    //get函数用于获取异步任务的返回值，如果还没有结果就会阻塞
    std::cout << "result: " << result.get() << std::endl;

    return 0;
}