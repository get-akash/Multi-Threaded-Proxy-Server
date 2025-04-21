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

    void push(const T& value){
        std::lock_guard<std::mutex> lock(mtx);

        queue.push(std::make_unique<T>(value));
        cv.notify_one();
    };
    std::unique_ptr<T> pop(){
         std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [this]{return !queue.empty();});
        std::unique_ptr<T> ref = std::move(queue.front());
        queue.pop();
        return ref;
    };
    std::unique_ptr<T> try_pop(){
        if(queue.empty()){
            return nullptr;
        }
        std::lock_guard<std::mutex> lock(mtx);
        
        std::unique_ptr<T> ref = std::move(queue.front());
        queue.pop();
        return ref;
    };
    bool empty(){
        std::lock_guard<std::mutex> lock(mtx);
    
        return queue.empty();
    };
};
