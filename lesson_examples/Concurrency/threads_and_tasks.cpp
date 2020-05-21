#include <iostream>
#include <thread>
#include <future>
#include <cmath>
#include <memory>
#include <vector>
#include <chrono>

double divideByNumber(double num, double denom)
{
    // print system id of worker thread
    std::cout << "Worker thread id = " << std::this_thread::get_id() << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work

    if (denom == 0)
        throw std::runtime_error("Exception from thread#: Division by zero!");

    return num / denom;
}

void workerFunction(int n)
{
    // print system id of worker thread
    std::cout << "Worker thread id = " << std::this_thread::get_id() << std::endl;

    // perform work
    for (int i = 0; i < n; ++i)
    {
        sqrt(12345.6789);
    }
}

int main()
{
    // print system id of worker thread
    std::cout << "Main thread id = " << std::this_thread::get_id() << std::endl;

    // use async to start a task
    double num = 42.0, denom = 2.0;

    // SYNCHRONOUS - same ID
    std::future<double> ftr = std::async(std::launch::deferred, divideByNumber, num, denom);
    // ASYNCHRONOUS - different ID
    // std::future<double> ftr = std::async(std::launch::async, divideByNumber, num, denom);
    // ANY
    // std::future<double> ftr = std::async(std::launch::deferred|std::launch::async, divideByNumber, num, denom);
    // retrieve result within try-catch-block
    try
    {
        double result = ftr.get();
        std::cout << "Result = " << result << std::endl;
    }
    catch (std::runtime_error e)
    {
        std::cout << e.what() << std::endl;
    }

    // start time measurement
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    // launch various tasks
    std::vector<std::future<void>> futures;
    int nLoops = 1e7, nThreads = 4;
    // int nLoops = 10, nThreads = 4;
    for (int i = 0; i < nThreads; ++i)
    {
        // futures.emplace_back(std::async(std::launch::async | std::launch::deferred, workerFunction, nLoops));
        futures.emplace_back(std::async(std::launch::async, workerFunction, nLoops));
        // futures.emplace_back(std::async(std::launch::deferred, workerFunction, nLoops));
    }

    // wait for tasks to complete
    for (const std::future<void> &ftr : futures)
        ftr.wait();

    // stop time measurement and print execution time
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "Execution finished after " << duration <<" microseconds" << std::endl;

    return 0;
}