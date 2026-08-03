#include "TimeManager.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    TimeManager timer;
    
    timer.addCallback([](long long time) {
        std::cout << "Time: " << time << "ms" << std::endl;
    });
    
    std::cout << "Starting timer..." << std::endl;
    timer.start();
    
    // Let it run for 100ms to see one callback
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    timer.stop();
    std::cout << "Timer stopped. Final time: " << timer.getCurrentTime() << "ms" << std::endl;
    
    return 0;
}