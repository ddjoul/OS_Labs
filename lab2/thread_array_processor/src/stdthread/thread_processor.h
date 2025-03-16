#ifndef THREAD_PROCESSOR_STDTHREAD_H
#define THREAD_PROCESSOR_STDTHREAD_H

#include <vector>
#include <mutex>
#include <string>

namespace stdthread {

// Mutex for thread-safe console output
extern std::mutex consoleMutex;

// Thread-safe console output
template<typename T>
void safe_cout(const T& message);

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
void MinMaxThreadProc(SharedData* data);

// Thread function for calculating average value
void AverageThreadProc(SharedData* data);

// Process the array using threads
bool ProcessArray(SharedData& data);

// Public interface functions for testing
void FindMinMax(SharedData& data);
void CalculateAverage(SharedData& data);
void ReplaceMinMaxWithAverage(SharedData& data);

} // namespace stdthread

#endif // THREAD_PROCESSOR_STDTHREAD_H