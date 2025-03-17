#ifndef THREAD_PROCESSOR_STDTHREAD_H
#define THREAD_PROCESSOR_STDTHREAD_H

#include <vector>
#include <mutex>
#include <string>

namespace stdthread {

extern std::mutex consoleMutex;

template<typename T>
void safe_cout(const T& message);

struct SharedData {
    std::vector<int> array;
    int min_value;
    int max_value;
    double avg_value;
    int min_index;
    int max_index;
};

void MinMaxThreadProc(SharedData* data);

void AverageThreadProc(SharedData* data);

bool ProcessArray(SharedData& data);

void FindMinMax(SharedData& data);
void CalculateAverage(SharedData& data);
void ReplaceMinMaxWithAverage(SharedData& data);

}
#endif