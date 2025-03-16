#ifndef THREAD_PROCESSOR_WINAPI_H
#define THREAD_PROCESSOR_WINAPI_H

#include <windows.h>
#include <vector>

namespace winapi {

// Shared data structure for threading
struct SharedData {
    std::vector<int> array;
    int min_value;
    int max_value;
    double avg_value;
    int min_index;
    int max_index;
};

// Thread function for finding minimum and maximum values
DWORD WINAPI MinMaxThreadProc(LPVOID lpParam);

// Thread function for calculating average value
DWORD WINAPI AverageThreadProc(LPVOID lpParam);

// Create and run threads, process results
bool ProcessArray(SharedData& data);

// Public interface functions for testing
void FindMinMax(SharedData& data);
void CalculateAverage(SharedData& data);
void ReplaceMinMaxWithAverage(SharedData& data);

} // namespace winapi

#endif // THREAD_PROCESSOR_WINAPI_H