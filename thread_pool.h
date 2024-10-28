#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "safe_queue.h"

class thread_pool 
{
private:
    std::vector<std::thread> threads;
    std::condition_variable condV;
    std::mutex Mut;
    safe_queue<std::function<void()>> safeQueue;
    std::atomic<bool> stop = false;
public:
    thread_pool(int numOfThreads) 
    {
        for (int i = 0; i < numOfThreads; i++) 
        {
            threads.push_back(std::thread(&thread_pool::work, this));
        }
    }
    ~thread_pool() 
    {
        for (auto& thread : threads)
            thread.join();
    }

    void work() 
    {
        while (!stop) 
        {
            std::unique_lock<std::mutex> lockMutex(Mut);
            if (!safeQueue.empty()) 
            {
                std::cout << "ID: " << std::this_thread::get_id() << std::endl;
                safeQueue.front();
                safeQueue.pop();
            }
            else 
            {
                std::this_thread::yield();
            }
            lockMutex.unlock();
        }
    }
    void submit(std::vector<std::function<void()>> vectorFunctions, int countSubmits) 
    {
        for (int i = 0; i < countSubmits; i++) 
        {
            for (size_t j = 0; j < vectorFunctions.size(); j++) 
            {
                safeQueue.push(vectorFunctions.at(j));
            }
        }
        stop = true;
    }

};