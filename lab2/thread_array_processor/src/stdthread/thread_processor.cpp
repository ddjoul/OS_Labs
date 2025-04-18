#include "thread_processor.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace stdthread {

std::mutex consoleMutex;

template<typename T>
void safe_cout(const T& message) {
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << message << std::endl;
}

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
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        
        if (data->array[i] > data->max_value) {
            data->max_value = data->array[i];
            data->max_index = i;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7)); 
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
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }
    
    data->avg_value = static_cast<double>(sum) / data->array.size();
    safe_cout<std::string>("Average Thread: Average value = " + std::to_string(data->avg_value));
}

bool ProcessArray(SharedData& data) {
    try {
        std::thread minMaxThread(MinMaxThreadProc, &data);
        std::thread averageThread(AverageThreadProc, &data);
        
        minMaxThread.join();
        averageThread.join();
        
        ReplaceMinMaxWithAverage(data);
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error in thread processing: " << e.what() << std::endl;
        return false;
    }
}

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

}