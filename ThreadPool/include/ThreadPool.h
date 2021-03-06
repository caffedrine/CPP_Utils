#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <list>

#include <queue/SharedQueue.h>

class ThreadPool
{
public:
    explicit ThreadPool(size_t ThreadsNo);
    template<class F, class... Args> auto enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();
    
    ssize_t ResultsAvailable();
    template<class F, class... Args> auto pop_result(F &&f, Args &&... args) -> std::result_of<F(Args...)>
    {
        if( results.empty() )
            return nullptr;
        
        
    }
    
    
private:
    // need to keep track of threads so we can join them
    std::vector<std::thread> workers;
    // the task queue
    std::queue<std::function<void()> > tasks;
    // tasks finished
    std::list<void *> results;
    
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
    
    auto task = std::make_shared<std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        
        // don't allow enqueueing after stopping the pool
        if( stop )
            throw std::runtime_error("enqueue on stopped ThreadPool");
        
        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}



#endif //_THREADPOOL_H_
