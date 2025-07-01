#include <iostream>
#include <future>
#include <thread>

int Add(int num1,int num2){
    std::cout << "Add函数开始执行" << std::endl;
    return num1 + num2;
}

int main(){
    
    //1.封装对象
    auto task = std::make_shared<std::packaged_task<int(int,int)>>(Add);
    //创建一个packaged_task对象，packaged_task对象可以包装一个函数，并返回一个future对象
    //packaged_task对象可以异步执行函数
    //创建一个智能指针对象，避免拷贝构造

    //2.获取任务包关联的future对象
    std::future<int> result = task->get_future();

    //3.执行任务
    std::thread thr([task](){
        (*task)(11,22);
    });

    // 添加这行等待线程完成
    thr.join();

    //4.获取结果
    std::cout << "result: " << result.get() << std::endl;

    return 0;
}
