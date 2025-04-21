#include "../include/ThreadPool.hpp"

// If the thread is joinable (i.e., still running and hasn’t been joined/detached), and its destructor is called, the program crashes.
// join() blocks the calling thread until the target thread completes, ensuring safe cleanup.
JoinThreads::~JoinThreads(){
    for (auto& t : threads) {
        if (t.joinable()){
            t.join();
        }
    }
}

JoinThreads::JoinThreads(std::vector<std::thread>& threads_) : threads(threads_) {}

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
