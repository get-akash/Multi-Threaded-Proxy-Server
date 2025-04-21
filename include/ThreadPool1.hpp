#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include "../include/ThreadSafeQueue.hpp"

class JoinThreads1 {
private:
    std::vector<std::thread>& threads;

public:
    explicit JoinThreads1(std::vector<std::thread>& threads_) : threads(threads_) {};
    // why explicit? because implicit conversions are not required ex: 
    // std::vector<std::thread> myThreads;
    // JoinThreads jt = myThreads; (not required and unwanted)
    ~JoinThreads1();
};

class ThreadPool1 {
private:
    std::atomic<bool> done;
    ThreadSafeQueue<std::function<void()>> work_queue;
    std::vector<std::thread> workers;
    JoinThreads1 joiner;

    void worker_thread();
public:
    ThreadPool1();
    ~ThreadPool1();

    template <typename Function_type>
    void submit(Function_type& task);
};