#include "../include/ThreadPool1.hpp"

JoinThreads::~JoinThreads(){
    for (auto& t : threads) {
        if (t.joinable()){
            t.join();
        }
    }
}

ThreadPool::~ThreadPool() {
    done = true;
}

void ThreadPool::worker_thread(){
    while(!done){
        auto taskPtr = work_queue.try_pop();
        if(taskPtr){
            (*taskPtr)();
        }else{
            std::this_thread::yield();
        }
    }
}

ThreadPool::ThreadPool() : done(false), joiner(workers) {
    const size_t threadCount = std::thread::hardware_concurrency();
    try{
        for(size_t i=0; i<threadCount; i++){
            workers.push_back(std::thread(&ThreadPool::worker_thread, this));
            // workers.emplace_back([this](){worker_thread();});
        }
    }
    catch(...){
        done = false;
        throw;
    }
}

// submit method only supports lvalue reference
// submit([](){ std::cout << "hello"; }); // ❌ won't compile
template <typename Function_type>
void ThreadPool::submit(Function_type& task){
    work_queue.push(task);
}

