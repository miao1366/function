// future example
#include <iostream>             // std::cout
#include <future>               // std::async, std::future
#include <chrono>               // std::chrono::milliseconds

#include <unistd.h>             // syscall()
#include <sys/syscall.h>        // syscall()
// a non-optimized way of checking for prime numbers:
bool is_prime(int x)
{    
    std::cout << "tid-----::" << syscall(SYS_gettid) << std::endl;

    for (int i = 2; i < x; ++i) {
        //std::cout << " tid:: " << syscall(SYS_gettid) << std::endl;
        if (x % i == 0)
            return false;        
    }
    std::cout << "tid-----::" << syscall(SYS_gettid) << std::endl;
    return true;
}

int main()
{
    // call function asynchronously:
    std::future < bool > fut = std::async(is_prime, 444444443);
    std::cout << " tid:: " << syscall(SYS_gettid) << std::endl;

   // do something while waiting for function to set future:
    std::cout << "checking, please wait\n";
    std::chrono::milliseconds span(100);
    while (fut.wait_for(span) == std::future_status::timeout)
        std::cout << '.' << std::endl;
    std::cout << " tid:: " << syscall(SYS_gettid) << std::endl;

    bool x = fut.get();         // retrieve return value
    std::cout << " tid:: " << syscall(SYS_gettid) << std::endl;

    std::cout << "\n444444443 " << (x ? "is" : "is not") << " prime.\n";

    return 0;
}