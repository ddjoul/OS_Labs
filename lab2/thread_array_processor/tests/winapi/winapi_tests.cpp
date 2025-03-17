#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "winapi/thread_processor.h"

class WinApiThreadTest : public ::testing::Test {
protected:
    winapi::SharedData testData;

    void SetUp() override {
        testData.array = {5, 2, 8, 1, 9, 3};
    }
};

TEST_F(WinApiThreadTest, MinMaxThread) {
    winapi::FindMinMax(testData);
    
    EXPECT_EQ(testData.min_value, 1);
    EXPECT_EQ(testData.min_index, 3);
    EXPECT_EQ(testData.max_value, 9);
    EXPECT_EQ(testData.max_index, 4);
}

TEST_F(WinApiThreadTest, AverageThread) {
    winapi::CalculateAverage(testData);
    
    EXPECT_NEAR(testData.avg_value, 4.6666, 0.001);
}

TEST_F(WinApiThreadTest, ReplaceMinMaxWithAverage) {
    testData.min_value = 1;
    testData.min_index = 3;
    testData.max_value = 9;
    testData.max_index = 4;
    testData.avg_value = 4.6666;
    
    winapi::ReplaceMinMaxWithAverage(testData);
    
    EXPECT_NEAR(testData.array[testData.min_index], 4.6666, 0.001);
    EXPECT_NEAR(testData.array[testData.max_index], 4.6666, 0.001);
    
    EXPECT_EQ(testData.array[0], 5);
    EXPECT_EQ(testData.array[1], 2);
    EXPECT_EQ(testData.array[2], 8);
    EXPECT_EQ(testData.array[5], 3);
}


TEST_F(WinApiThreadTest, EmptyArray) {
    testData.array.clear();
    
    winapi::FindMinMax(testData);
    
    winapi::CalculateAverage(testData);
    
    EXPECT_EQ(testData.avg_value, 0);
    
    EXPECT_NO_THROW(winapi::ReplaceMinMaxWithAverage(testData));
}

TEST_F(WinApiThreadTest, SingleElementArray) {
    testData.array = {42};
    
    winapi::FindMinMax(testData);
    
    winapi::CalculateAverage(testData);
    
    EXPECT_EQ(testData.min_value, 42);
    EXPECT_EQ(testData.max_value, 42);
    EXPECT_EQ(testData.avg_value, 42);
    EXPECT_EQ(testData.min_index, 0);
    EXPECT_EQ(testData.max_index, 0);
    
    winapi::ReplaceMinMaxWithAverage(testData);
    EXPECT_EQ(testData.array[0], 42);
}

TEST_F(WinApiThreadTest, DuplicateMinMaxValues) {
    testData.array = {5, 9, 2, 9, 2, 5};
    
    winapi::FindMinMax(testData);
    
    EXPECT_EQ(testData.min_value, 2);
    EXPECT_EQ(testData.min_index, 2);
    EXPECT_EQ(testData.max_value, 9);
    EXPECT_EQ(testData.max_index, 1);
}

TEST_F(WinApiThreadTest, ProcessArrayComplete) {
    bool result = winapi::ProcessArray(testData);
    
    EXPECT_TRUE(result);
    
    EXPECT_NEAR(testData.array[3], testData.avg_value, 0.001);
    EXPECT_NEAR(testData.array[4], testData.avg_value, 0.001);
}