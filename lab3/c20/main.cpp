#include "func.h"
#include <iostream>

int main() {
    int n;
    std::cout << "Enter array size: ";
    std::cin >> n;
    
    ThreadManager manager(n);
    
    int threadCount;
    std::cout << "Enter threads amount: ";
    std::cin >> threadCount;
    
    manager.initialize(threadCount);
    manager.createThreads(threadCount);
    
    while (true) {
        manager.waitForAllThreadsToStop();
        
        manager.printArray("Array before deleting: ");
        
        int thrId;
        std::cout << "Enter id of thread that needs to stop: ";
        std::cin >> thrId;
        
        while (thrId < 0 || thrId >= threadCount || !manager.isThreadWorkingAt(thrId)) {
            std::cout << "This thread does not exist, enter another: ";
            std::cin >> thrId;
        }
        
        manager.deleteThread(thrId);
        
        manager.printArray("Array after deleting: ");
        
        if (manager.getThreadCount() <= 0) {
            std::cout << "End of cycle" << std::endl;
            break;
        }
        
        manager.resumeAllThreads();
    }
    
    return 0;
}