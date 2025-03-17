#ifndef THREAD_PROCESSOR_WINAPI_H
#define THREAD_PROCESSOR_WINAPI_H

#include <windows.h>
#include <vector>

namespace winapi {

struct SharedData {
    std::vector<int> array;
    int min_value;
    int max_value;
    double avg_value;
    int min_index;
    int max_index;
};

DWORD WINAPI MinMaxThreadProc(LPVOID lpParam);

DWORD WINAPI AverageThreadProc(LPVOID lpParam);

bool ProcessArray(SharedData& data);

void FindMinMax(SharedData& data);
void CalculateAverage(SharedData& data);
void ReplaceMinMaxWithAverage(SharedData& data);

}

#endif