#ifndef _TASKPOOL_H_
#define _TASKPOOL_H_

#include <thread>
#include <future>
#include <queue>
#include <functional>
#include <thread>


namespace vastina {

enum class IMPORTANCE{
        maintk,
        common,
        lowest 
    };

class Taskpool{
private:
    struct common_{
        std::thread worker;
        std::packaged_task<void()> work;
        bool stop;
    } common;
    std::mutex tmutex;
    std::queue<std::function<void()>> tasks2;
    std::queue<std::function<void()>> tasks;
    
public:
    void start();
    template<typename F, typename...Args>
        void submittask(F&& f, Args&&... args, IMPORTANCE level);
    void lastwork();
    void setstoped();
};

template<typename F, typename...Args>
void Taskpool::submittask(F&& f, Args&&... args, IMPORTANCE level) {
    std::function<decltype(f(args...))()> func 
        = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
    if(level == IMPORTANCE::lowest){
        {
            std::unique_lock<std::mutex> lock(tmutex);
            tasks.emplace(std::function<void()>( [task_ptr](){
                (*task_ptr)(); 
            }));
        }       
    }
    else if(level == IMPORTANCE::common){
        {
            std::unique_lock<std::mutex> lk(tmutex);
            tasks2.emplace(std::function<void()>( [task_ptr](){
                (*task_ptr)(); 
            }));
            //common.condition.notify_one();
        }
        if(common.worker.joinable()) common.worker.join();
    }
};


};

#endif