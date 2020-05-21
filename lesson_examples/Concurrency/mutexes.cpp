#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include<algorithm>

class Vehicle
{
public:
    Vehicle(int id) : _id(id) {}

private:
    int _id;
};

class WaitingVehicles
{
public:
    WaitingVehicles() : _tmpVehicles(0) {}

    // getters / setters
    void printSize()
    {
        //// 2 ADDING A MUTEX TO AVOID DATA RACE
        _mutex.lock();
        // std::cout << "#vehicles = " << _tmpVehicles << std::endl;
        std::cout << "#vehicles = " << _vehicles.size() << std::endl;
        _mutex.unlock();
    }

    // typical behaviour methods
    void pushBack(Vehicle &&v)
    {
        //// 2 ADDING A MUTEX TO AVOID DATA RACE
        _mutex.lock();
        // data race would cause an exception
        _vehicles.push_back(std::move(v));
        int oldNum = _tmpVehicles;
        //// 1 THIS SLEEP SHOWS THE DATA RACE
        std::this_thread::sleep_for(std::chrono::microseconds(1000)); // wait deliberately to expose the data race
        _tmpVehicles = oldNum + 1;
        _mutex.unlock();
    }

private:
    std::vector<Vehicle> _vehicles; // list of all vehicles waiting to enter this intersection
    int _tmpVehicles;
    std::mutex _mutex;
};

int main()
{
    std::shared_ptr<WaitingVehicles> queue(new WaitingVehicles); 
    std::vector<std::future<void>> futures;
    for (int i = 0; i < 1000; ++i)
    {
        Vehicle v(i);
        //// 1 DATA RACE FOR ASYNC CALL
        //// 2 FIXED BY USING MUTEXES
        // futures.emplace_back(std::async(std::launch::deferred, &WaitingVehicles::pushBack, queue, std::move(v)));
        futures.emplace_back(std::async(std::launch::async, &WaitingVehicles::pushBack, queue, std::move(v)));
    }

    std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) {
        ftr.wait();
    });

    queue->printSize();

    return 0;
}
