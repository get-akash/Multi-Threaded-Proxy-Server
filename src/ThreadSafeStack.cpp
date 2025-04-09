#include "../include/ThreadSafeStack.hpp"

template <typename T>
void ThreadSafeStack<T>::push(const T& value){
    std::lock_guard<std::mutex> lock(mtx);

    stack.push(std::make_unique<T>(value));
    cv.notify_one();
};

template <typename T>
std::unique_ptr<T> ThreadSafeStack<T>::pop(){
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [this]{return !stack.empty();});
    std::unique_ptr<T> ref = std::move(stack.top());
    stack.pop();
    return ref;
}

template <typename T>
std::unique_ptr<T> ThreadSafeStack<T>::try_pop(){
    if(stack.empty()){
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(mtx);
    
    std::unique_ptr<T> ref = std::move(stack.top());
    stack.pop();
    return ref;
}

template <typename T>
bool ThreadSafeStack<T>::empty(){
    std::lock_guard<std::mutex> lock(mtx);
    
    return stack.empty();
}