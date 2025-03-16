#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "winapi/thread_processor.h"

class WinApiThreadTest : public ::testing::Test {
protected:
    winapi::SharedData testData;

    void SetUp() override {
        // Default test data
        testData.array = {5, 2, 8, 1, 9, 3};
    }
};

// Test the MinMaxThreadProc function
TEST_F(WinApiThreadTest, MinMaxThread) {
    // Run the min-max function
    winapi::FindMinMax(testData);
    
    // Check results
    EXPECT_EQ(testData.min_value, 1);
    EXPECT_EQ(testData.min_index, 3);
    EXPECT_EQ(testData.max_value, 9);
    EXPECT_EQ(testData.max_index, 4);
}

// Test the AverageThreadProc function
TEST_F(WinApiThreadTest, AverageThread) {
    // Run the average function
    winapi::CalculateAverage(testData);
    
    // Check results (sum = 28, count = 6, avg = 4.6666...)
    EXPECT_NEAR(testData.avg_value, 4.6666, 0.001);
}

// Test the ReplaceMinMaxWithAverage function
TEST_F(WinApiThreadTest, ReplaceMinMaxWithAverage) {
    // Set up the expected values from thread functions
    testData.min_value = 1;
    testData.min_index = 3;
    testData.max_value = 9;
    testData.max_index = 4;
    testData.avg_value = 4.6666;
    
    // Run the replacement function
    winapi::ReplaceMinMaxWithAverage(testData);
    
    // Check if the min and max values were replaced with the average
    EXPECT_NEAR(testData.array[testData.min_index], 4.6666, 0.001);
    EXPECT_NEAR(testData.array[testData.max_index], 4.6666, 0.001);
    
    // Other elements should remain unchanged
    EXPECT_EQ(testData.array[0], 5);
    EXPECT_EQ(testData.array[1], 2);
    EXPECT_EQ(testData.array[2], 8);
    EXPECT_EQ(testData.array[5], 3);
}

// Test empty array handling
TEST_F(WinApiThreadTest, EmptyArray) {
    // Clear the array
    testData.array.clear();
    
    // Run the min-max function
    winapi::FindMinMax(testData);
    
    // Run the average function
    winapi::CalculateAverage(testData);
    
    // For empty arrays, the average should be 0
    EXPECT_EQ(testData.avg_value, 0);
    
    // Replacement should not crash with empty array
    EXPECT_NO_THROW(winapi::ReplaceMinMaxWithAverage(testData));
}

// Test single-element array handling
TEST_F(WinApiThreadTest, SingleElementArray) {
    // Set up a single-element array
    testData.array = {42};
    
    // Run the min-max function
    winapi::FindMinMax(testData);
    
    // Run the average function
    winapi::CalculateAverage(testData);
    
    // For a single-element array, min, max, and avg should all be that element
    EXPECT_EQ(testData.min_value, 42);
    EXPECT_EQ(testData.max_value, 42);
    EXPECT_EQ(testData.avg_value, 42);
    EXPECT_EQ(testData.min_index, 0);
    EXPECT_EQ(testData.max_index, 0);
    
    // Replacement should set the only element to itself (no change)
    winapi::ReplaceMinMaxWithAverage(testData);
    EXPECT_EQ(testData.array[0], 42);
}

// Test array with duplicate min/max values
TEST_F(WinApiThreadTest, DuplicateMinMaxValues) {
    // Set up array with duplicate min and max values
    testData.array = {5, 9, 2, 9, 2, 5};
    
    // Run the min-max function
    winapi::FindMinMax(testData);
    
    // For duplicate values, the function should find the first occurrence
    EXPECT_EQ(testData.min_value, 2);
    EXPECT_EQ(testData.min_index, 2);
    EXPECT_EQ(testData.max_value, 9);
    EXPECT_EQ(testData.max_index, 1);
}

// Test the complete ProcessArray function
TEST_F(WinApiThreadTest, ProcessArrayComplete) {
    // Process the array
    bool result = winapi::ProcessArray(testData);
    
    // Check if processing was successful
    EXPECT_TRUE(result);
    
    // The min and max elements should have been replaced with the average
    // Min was at index 3, max was at index 4
    EXPECT_NEAR(testData.array[3], testData.avg_value, 0.001);
    EXPECT_NEAR(testData.array[4], testData.avg_value, 0.001);
}