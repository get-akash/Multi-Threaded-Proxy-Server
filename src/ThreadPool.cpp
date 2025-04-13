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
std::future<typename std::invoke_result<Function_type()>::type> 
ThreadPool::submit(Function_type& f){
    typedef typename std::invoke_result<Function_type()>::type result_type;
    std::packaged_task<result_type()> task(std::move(f));
    std::future<result_type> res(task.get_future());
    work_queue.push(std::function<void()>(std::move(task)));
    return res;
}

template <typename T>
T ThreadPool::wait_for_future(std::future<T>& fut) {
    std::future_status status = future.wait_for(std::chrono::milliseconds(0));
    while (status != std::future_status::ready) {
        auto task = work_queue.try_pop();
        if (task) {
            (*task)(); 
        } else {
            std::this_thread::yield();
        }
    }
    return fut.get();
}

