#pragma once
#include<stack>
#include<mutex>
#include<memory>
#include<condition_variable>

template <typename T>
class ThreadSafeStack{
private:
    std::stack<std::unique_ptr<T>> stack;
    std::mutex mtx;
    std::condition_variable cv;

public:
    ThreadSafeStack() = default;

    ThreadSafeStack(const ThreadSafeStack&) = delete;
    ThreadSafeStack& operator=(const ThreadSafeStack&) = delete;

    ThreadSafeStack(ThreadSafeStack&&) = default;
    ThreadSafeStack& operator=(ThreadSafeStack&&) = default; //&& is a Rvalue reference

    void push(const T& value);
    std::unique_ptr<T> pop();
    std::unique_ptr<T> try_pop();
    bool empty();
};