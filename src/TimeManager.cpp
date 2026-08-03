#include "TimeManager.h"

TimeManager::TimeManager() : currentTime(0), intervalMs(20000), isRunning(false) {}

TimeManager::~TimeManager() {
    stop();
}

void TimeManager::start() {
    if (isRunning) return;
    
    isRunning = true;
    
    timerThread = std::thread([this]() {
        while (isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
            if (isRunning) {
                currentTime += intervalMs;
                for (auto& callback : callbacks) {
                    callback(currentTime);
                }
            }
        }
    });
}

void TimeManager::stop() {
    isRunning = false;
    if (timerThread.joinable()) {
        timerThread.join();
    }
}

void TimeManager::addCallback(std::function<void(long long)> callback) {
    callbacks.push_back(callback);
}

long long TimeManager::getCurrentTime() const {
    return currentTime;
}

void TimeManager::setInterval(long long ms) {
    intervalMs = ms;
    // If running, we would need to restart the thread to apply the new interval
    // For simplicity, we'll just update the value
}

bool TimeManager::isRunningState() const {
    return isRunning;
}