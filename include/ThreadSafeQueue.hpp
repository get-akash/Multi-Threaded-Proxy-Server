#pragma once
#include<queue>
#include<mutex>
#include<memory>
#include<condition_variable>

template <typename T>
class ThreadSafeQueue{
private:
    std::queue<std::unique_ptr<T>> queue;
    std::mutex mtx;
    std::condition_variable cv;

public:
    ThreadSafeQueue() = default;
    // Delete copy constructor and assignment operator
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    // Allow move constructor and move assignment
    ThreadSafeQueue(ThreadSafeQueue&&) = default;
    ThreadSafeQueue& operator=(ThreadSafeQueue&&) = default;

    void push(const T& value);
    std::unique_ptr<T> pop();
    std::unique_ptr<T> try_pop();
    bool empty();
};
