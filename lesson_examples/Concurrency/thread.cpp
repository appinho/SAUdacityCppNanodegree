// g++ -pthread thread.cpp -std=c++17
#include <iostream>
#include <thread>

void threadFunction2()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // simulate work
    std::cout << "Finished work 1 in thread\n"; 

    std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
    std::cout << "Finished work 2 in thread\n"; 
}

void threadFunction()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // simulate work
    std::cout << "Finished work in thread\n"; 
}

void threadFunctionEven()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // simulate work
    std::cout << "Even thread\n";
}

void threadFunctionOdd()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // simulate work
    std::cout << "Odd thread\n";
}

int main()
{
	//// Thread ID
    std::cout << "Hello concurrent world from main! Thread id = " 
    	// << std::this_thread::get_id() << std::endl;
    	<< pthread_self() << std::endl;

    //// Number of threads
    unsigned int nCores = std::thread::hardware_concurrency();
    std::cout << "This machine supports concurrency with " 
    	<< nCores << " cores available" << std::endl;

    //// Second thread
    std::thread t(threadFunction);
    // do something in main()
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // simulate work
    std::cout << "Finished work in main\n";
    // wait for thread to finish
    t.join();

    //// Randomness of events
    // create thread
    std::thread t2(threadFunction2);
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // simulate work
    std::cout << "Finished work 1 in main\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
    std::cout << "Finished work 2 in main\n";
    // wait for thread to finish
    t2.join();

    //// Using join() as a barrier
    // Move line up to first execute threadFunction2 and then let main continue its work

    //// Detach
    // t.detach();

    //// QUIZ
    for(int i = 0; i < 6; i++){
        if(i%2 == 0){
            std::thread t3(threadFunctionEven);
            t3.detach();
        }
        else{
            std::thread t3(threadFunctionOdd);
            t3.detach();
        }
    }

    // Depending on this sleep either none (0), some(1), or all threads (10) are finished
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // simulate work

    std::cout << "End of main is reached" << std::endl;

    return 0;
}
