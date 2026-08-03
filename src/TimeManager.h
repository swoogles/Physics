#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <chrono>
#include <functional>
#include <vector>
#include <thread>
#include <atomic>
#include <iostream>

class TimeManager {
private:
    std::atomic<long long> currentTime;
    long long intervalMs;
    std::atomic<bool> isRunning;
    std::vector<std::function<void(long long)> > callbacks;
    std::thread timerThread;

public:
    TimeManager();
    ~TimeManager();
    
    void start();
    void stop();
    void addCallback(std::function<void(long long)> callback);
    long long getCurrentTime() const;
    void setInterval(long long ms);
    bool isRunningState() const;
};

#endif // TIME_MANAGER_H