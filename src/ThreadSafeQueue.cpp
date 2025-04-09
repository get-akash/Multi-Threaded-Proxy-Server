#include "../include/ThreadSafeQueue.hpp"

template <typename T>
void ThreadSafeQueue<T>::push(const T& value){
    std::lock_guard<std::mutex> lock(mtx);

    queue.push(std::make_unique<T>(value));
    cv.notify_one();
};

template <typename T>
std::unique_ptr<T> ThreadSafeQueue<T>::pop(){
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [this]{return !queue.empty();});
    std::unique_ptr<T> ref = std::move(queue.front());
    queue.pop();
    return ref;
}

template <typename T>
std::unique_ptr<T> ThreadSafeQueue<T>::try_pop(){
    std::lock_guard<std::mutex> lock(mtx);
    
    if(queue.empty()){
        return nullptr;
    }else{
        std::unique_ptr<T> ref = std::move(queue.front());
        queue.pop();
        return ref;
    }
}

template <typename T>
bool ThreadSafeQueue<T>::empty(){
    std::lock_guard<std::mutex> lock(mtx);
    
    return queue.empty();
}