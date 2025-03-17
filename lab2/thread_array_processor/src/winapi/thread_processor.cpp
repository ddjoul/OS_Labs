#include "thread_processor.h"
#include <iostream>

namespace winapi {

DWORD WINAPI MinMaxThreadProc(LPVOID lpParam) {
    SharedData* data = reinterpret_cast<SharedData*>(lpParam);
    
    if (data->array.empty()) {
        std::cout << "Min-Max Thread: Array is empty." << std::endl;
        return 0;
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
        Sleep(7);
        
        if (data->array[i] > data->max_value) {
            data->max_value = data->array[i];
            data->max_index = i;
        }
        Sleep(7);
    }
    
    std::cout << "Min-Max Thread: Minimum value = " << data->min_value 
              << " at index " << data->min_index << std::endl;
    std::cout << "Min-Max Thread: Maximum value = " << data->max_value 
              << " at index " << data->max_index << std::endl;
    
    return 0;
}

DWORD WINAPI AverageThreadProc(LPVOID lpParam) {
    SharedData* data = reinterpret_cast<SharedData*>(lpParam);
    
    if (data->array.empty()) {
        std::cout << "Average Thread: Array is empty." << std::endl;
        data->avg_value = 0;
        return 0;
    }
    
    int sum = 0;
    for (size_t i = 0; i < data->array.size(); ++i) {
        sum += data->array[i];
        Sleep(12);
    }
    
    data->avg_value = static_cast<double>(sum) / data->array.size();
    std::cout << "Average Thread: Average value = " << data->avg_value << std::endl;
    
    return 0;
}

bool ProcessArray(SharedData& data) {
    HANDLE hMinMax = CreateThread(
        NULL,                   
        0,                      
        MinMaxThreadProc,       
        &data,                  
        0,                     
        NULL                   
    );
    
    if (hMinMax == NULL) {
        std::cout << "Error creating min_max thread: " << GetLastError() << std::endl;
        return false;
    }
    
    HANDLE hAverage = CreateThread(
        NULL,                   
        0,                      
        AverageThreadProc,      
        &data,                  
        0,                      
        NULL                    
    );
    
    if (hAverage == NULL) {
        std::cout << "Error creating average thread: " << GetLastError() << std::endl;
        CloseHandle(hMinMax);
        return false;
    }
    
    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);
    
    CloseHandle(hMinMax);
    CloseHandle(hAverage);
    
    ReplaceMinMaxWithAverage(data);
    
    return true;
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