#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <future>
#include <functional>
#include "./ThreadSafeQueue.hpp"

class JoinThreads {
private:
    std::vector<std::thread>& threads;

public:
    explicit JoinThreads(std::vector<std::thread>& threads_);
    // why explicit? because implicit conversions are not required ex: 
    // std::vector<std::thread> myThreads;
    // JoinThreads jt = myThreads; (not required and unwanted)
    ~JoinThreads();
};

class ThreadPool {
private:
    std::atomic<bool> done;
    ThreadSafeQueue<std::function<void()>> work_queue;
    std::vector<std::thread> workers;
    JoinThreads joiner;

    void worker_thread();
public:
    ThreadPool();
    ~ThreadPool();

    template <typename Function_type>
    auto submit(Function_type&& f) -> std::future<std::invoke_result_t<Function_type>> {
        using result_type = std::invoke_result_t<Function_type>;
        using task_type = std::packaged_task<result_type()>;

        auto task_ptr = std::make_shared<task_type>(std::forward<Function_type>(f));
        auto res = task_ptr->get_future();

        work_queue.push([task_ptr]() {
            (*task_ptr)();
        });

        return res;
    }
    
    template <typename T>
    T wait_for_future(std::future<T>& fut){
        std::future_status status = fut.wait_for(std::chrono::milliseconds(0));
        while (status != std::future_status::ready) {
            auto task = work_queue.try_pop();
            if (task) {
                (*task)(); 
            } else {
                std::this_thread::yield();
            }
        }
    return fut.get();
    }; // instead of using future.get() directly, use this method
};