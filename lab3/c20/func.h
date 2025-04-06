#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <functional>

class ThreadManager {
private:
    std::vector<int> mas;
    std::vector<std::thread> threads;
    std::vector<bool> isThreadWorks;
    std::vector<bool> threadStopped;
    std::vector<bool> threadResumed;
    std::vector<bool> threadDeleted;
    
    std::mutex cs, csout;
    std::condition_variable cv;
    
    int n = 0;
    std::atomic<int> thrkol = 0;

public:
    explicit ThreadManager(int size) : n(size), mas(size, -1) {}
    
    void initialize(int threadCount) {
        thrkol = threadCount;
        isThreadWorks.resize(threadCount, true);
        threadStopped.resize(threadCount, false);
        threadResumed.resize(threadCount, false);
        threadDeleted.resize(threadCount, false);
        threads.reserve(threadCount);
    }
    
    void createThreads(int count) {
        for (int i = 0; i < count; i++) {
            threads.emplace_back([this, i]() { markerFunction(i); });
        }
    }
    
    void markerFunction(int id) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, n - 1);
        int elmarked = 0;
        
        while (true) {
            int index = distrib(gen);
            
            {
                std::lock_guard<std::mutex> lock(cs);
                if (mas[index] == -1) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    mas[index] = id;
                    elmarked++;
                }
                else {
                    {
                        std::lock_guard<std::mutex> outLock(csout);
                        std::cout << "Thread id" << id << " marked " << elmarked
                            << " elements. Stop on " << index << std::endl;
                        threadStopped[id] = true;
                    }
                    
                    std::unique_lock<std::mutex> lock(cs);
                    
                    // Wait until resumed or deleted
                    cv.wait(lock, [this, id]() { 
                        return threadResumed[id] || threadDeleted[id]; 
                    });
                    
                    if (threadDeleted[id]) {
                        isThreadWorks[id] = false;
                        for (int i = 0; i < n; ++i) {
                            if (mas[i] == id) {
                                mas[i] = -1;
                            }
                        }
                        thrkol--;
                        return;
                    }
                    
                    threadResumed[id] = false;
                }
            }
        }
    }
    
    bool areAllThreadsStopped() const {
        for (int i = 0; i < isThreadWorks.size(); i++) {
            if (isThreadWorks[i] && !threadStopped[i]) {
                return false;
            }
        }
        return true;
    }
    
    void waitForAllThreadsToStop() {
        while (!areAllThreadsStopped()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    void resumeThread(int id) {
        if (isThreadWorks[id]) {
            threadStopped[id] = false;
            threadResumed[id] = true;
            cv.notify_all();
        }
    }
    
    void deleteThread(int id) {
        if (isThreadWorks[id]) {
            threadDeleted[id] = true;
            cv.notify_all();
            if (id < threads.size() && threads[id].joinable()) {
                threads[id].join();
            }
        }
    }
    
    void resumeAllThreads() {
        for (int i = 0; i < isThreadWorks.size(); i++) {
            if (isThreadWorks[i]) {
                threadStopped[i] = false;
                threadResumed[i] = true;
            }
        }
        cv.notify_all();
    }
    
    void printArray(const std::string& message) const {
        std::cout << message;
        for (int i = 0; i < n; i++) {
            std::cout << mas[i] << " ";
        }
        std::cout << std::endl;
    }
    
    bool isThreadWorkingAt(int id) const {
        if (id >= 0 && id < isThreadWorks.size())
            return isThreadWorks[id];
        return false;
    }
    
    int getThreadCount() const {
        return thrkol;
    }
};