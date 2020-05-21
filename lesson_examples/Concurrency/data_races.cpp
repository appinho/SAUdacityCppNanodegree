#include <iostream>
#include <thread>
#include <future>
#include <memory>

class Vehicle
{
public:
    //default constructor
    Vehicle() : _id(0), _name(new std::string("Default Name"))
    {
        std::cout << "Vehicle #" << _id << " Default constructor called" << std::endl;
    }

    //initializing constructor
    Vehicle(int id, std::string name) : _id(id), _name(new std::string(name))
    {
        std::cout << "Vehicle #" << _id << " Initializing constructor called" << std::endl;
    }

    //// 3 DEEP COPY CONSTRUCTOR
    // copy constructor 
    Vehicle(Vehicle const &src)
    {
        // QUIZ: Student code STARTS here
        _id = src._id;
        if (src._name != nullptr)
        {
            _name = new std::string;
            *_name = *src._name;
            // _name = std::unique_ptr<std::string>();
            // _name = std::make_unique<std::string>(*src._name);
        }
        // QUIZ: Student code ENDS here
        std::cout << "Vehicle #" << _id << " copy constructor called" << std::endl;
    };

    //// 5 MOVE CONSTRUCTOR
    // move constructor 
    Vehicle(Vehicle && src)
    // _name(std::move(src._name))
    {
        _id = src.getID();
        _name = new std::string(src.getName());

        src.setID(0);
        src.setName("Default Name");

        std::cout << "Vehicle #" << _id << " move constructor called" << std::endl;
    };

    // setter and getter
    void setID(int id) { _id = id; }
    int getID() { return _id; }
    void setName(std::string name) { *_name = name; }
    std::string getName() { return *_name; }

private:
    int _id;
    std::string *_name;
    //// 6 NOTION OF UNIQUENESS
    // std::unique_ptr<std::string> _name;
};

int main()
{
    // create instances of class Vehicle
    Vehicle v0; // default constructor
    Vehicle v1(1, ""); // initializing constructor

    // read and write name in different threads (which one of the above creates a data race?)
    std::future<void> ftr = std::async([](Vehicle v) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work
        v.setID(2);
    }, v0);

    v0.setID(3);
    ftr.wait();

    //// 1 NO PROBLEM SINCE ID IS ATOMIC TYPE INT
    std::cout << "Vehicle #" << v0.getID() << std::endl;

    Vehicle v2(1, "Vehicle 1"); // initializing constructor

    // launch a thread that modifies the Vehicle name
    std::future<void> ftr2 = std::async([](Vehicle v) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work
        v.setName("Vehicle 2");
    },v0);

    v0.setName("Vehicle 3");

    ftr2.wait();
    //// 2 PASSING COMPLEX TYPES/POINTERS CAN CAUSE DATA RACES
    //// 3 ALWAYS PRINTS VEHICLE 3 NOW
    std::cout << v0.getName() << std::endl;

    //// 6 USE UNIQUE POINTER TOGETHER WITH MOVE SEMANTICS TO CATCH DATA RACE
    std::future<void> ftr5 = std::async([](Vehicle v) {
        v.setName("Vehicle 2");
    },std::move(v0));

    ftr5.wait();
    std::cout << v0.getName() << std::endl;

    return 0;
}