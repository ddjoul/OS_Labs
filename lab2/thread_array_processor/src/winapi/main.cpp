#include "thread_processor.h"
#include <iostream>

int main() {
    // Create data
    winapi::SharedData data;
    
    // Step 1: Create an array of integers with size and elements from console
    int size;
    std::cout << "Enter array size: ";
    std::cin >> size;
    
    if (size <= 0) {
        std::cout << "Invalid array size." << std::endl;
        return 1;
    }
    
    data.array.resize(size);
    
    std::cout << "Enter " << size << " integers:" << std::endl;
    for (int i = 0; i < size; ++i) {
        std::cin >> data.array[i];
    }
    
    // Process array with threads
    if (!winapi::ProcessArray(data)) {
        std::cout << "Error processing array." << std::endl;
        return 1;
    }
    
    // Print the final modified array
    std::cout << "Main Thread: Modified array after replacing min and max with average:" << std::endl;
    for (size_t i = 0; i < data.array.size(); ++i) {
        std::cout << data.array[i] << " ";
    }
    std::cout << std::endl;
    
    return 0;
}