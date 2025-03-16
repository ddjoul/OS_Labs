#include "thread_processor.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace stdthread {

// Global mutex for console output
std::mutex consoleMutex;

// Thread-safe console output implementation
template<typename T>
void safe_cout(const T& message) {
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << message << std::endl;
}

// Explicit template instantiation for strings
template void safe_cout<std::string>(const std::string& message);

void MinMaxThreadProc(SharedData* data) {
    if (data->array.empty()) {
        safe_cout<std::string>("Min-Max Thread: Array is empty.");
        return;
    }
    
    data->min_value = data->array[0];
    data->max_value = data->array[0];
    data->min_index = 0;
    data->max_index = 0;
    
    for (size_t i = 1; i < data->array.size(); ++i) {
        if (data->array[i] < data->min_value) {
            data->min_value = data->array[i];
            data->min_index = i;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7)); // Sleep 7 milliseconds
        
        if (data->array[i] > data->max_value) {
            data->max_value = data->array[i];
            data->max_index = i;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7)); // Sleep 7 milliseconds
    }
    
    safe_cout<std::string>("Min-Max Thread: Minimum value = " + std::to_string(data->min_value) + 
              " at index " + std::to_string(data->min_index));
    safe_cout<std::string>("Min-Max Thread: Maximum value = " + std::to_string(data->max_value) + 
              " at index " + std::to_string(data->max_index));
}

void AverageThreadProc(SharedData* data) {
    if (data->array.empty()) {
        safe_cout<std::string>("Average Thread: Array is empty.");
        data->avg_value = 0;
        return;
    }
    
    int sum = 0;
    for (size_t i = 0; i < data->array.size(); ++i) {
        sum += data->array[i];
        std::this_thread::sleep_for(std::chrono::milliseconds(12)); // Sleep 12 milliseconds
    }
    
    data->avg_value = static_cast<double>(sum) / data->array.size();
    safe_cout<std::string>("Average Thread: Average value = " + std::to_string(data->avg_value));
}

bool ProcessArray(SharedData& data) {
    try {
        // Create min_max and average threads
        std::thread minMaxThread(MinMaxThreadProc, &data);
        std::thread averageThread(AverageThreadProc, &data);
        
        // Wait for min_max and average threads to complete
        minMaxThread.join();
        averageThread.join();
        
        // Replace min and max elements with average
        ReplaceMinMaxWithAverage(data);
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error in thread processing: " << e.what() << std::endl;
        return false;
    }
}

// These functions are exposed for testing
void FindMinMax(SharedData& data) {
    MinMaxThreadProc(&data);
}

void CalculateAverage(SharedData& data) {
    AverageThreadProc(&data);
}

void ReplaceMinMaxWithAverage(SharedData& data) {
    if (!data.array.empty()) {
        data.array[data.min_index] = data.avg_value;
        data.array[data.max_index] = data.avg_value;
    }
}

} // namespace stdthread