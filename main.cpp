#include <iostream>
#include <thread>
#include "thread_pool.h"


static void Func1()
{
    std::cout << "Function 1: completed!" << std::endl << std::endl;
}

static void Func2() 
{
    std::cout << "Function 2: completed!" << std::endl << std::endl;
}

int main(int argc, char** argv) 
{
     
    const int cores = std::thread::hardware_concurrency();
    std::vector<std::function<void()>> vectorOfFunctions{ Func1, Func2 };
    int countSubmits = 4;
    thread_pool ThreadPool{ cores };


    std::thread thread1(&thread_pool::submit, &ThreadPool, vectorOfFunctions, countSubmits);
    thread1.join();

    return 0;
}