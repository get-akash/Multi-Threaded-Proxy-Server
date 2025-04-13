#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include "../include/ThreadSafeQueue.hpp"

class JoinThreads {
private:
    std::vector<std::thread>& threads;

public:
    explicit JoinThreads(std::vector<std::thread>& threads_) : threads(threads_) {};
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
    void submit(Function_type& task);
};