#pragma once
#include <iostream>
#include <queue>
#include <mutex>
#include <functional>

template<class T>
class safe_queue 
{
private:
    std::queue<T> queueOfWorks;
    std::mutex QueueMut;
    std::condition_variable condV;
public:
    void push(T work) 
    {
        std::lock_guard<std::mutex> lockMutex(QueueMut);
        queueOfWorks.push(work);
        condV.notify_one();
    };

    T pop() 
    {
        std::unique_lock<std::mutex> lockMutex(QueueMut);
        condV.wait(lockMutex, [this]() {return !queueOfWorks.empty(); });
        T work = std::move(queueOfWorks.front());
        queueOfWorks.pop();
        return work;
    }
    bool empty() 
    {
        std::lock_guard<std::mutex> lock(QueueMut);
        return queueOfWorks.empty();
    }
    void front() 
    {
        std::lock_guard<std::mutex> lock(QueueMut);
        queueOfWorks.front()();
    }

};