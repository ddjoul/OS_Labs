#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "stdthread/thread_processor.h"

class StdThreadTest : public ::testing::Test {
protected:
    stdthread::SharedData testData;

    void SetUp() override {
        testData.array = { 5, 2, 8, 1, 9, 3 };
    }
};

TEST_F(StdThreadTest, MinMaxThread) {
    stdthread::FindMinMax(testData);

    EXPECT_EQ(testData.min_value, 1);
    EXPECT_EQ(testData.min_index, 3);
    EXPECT_EQ(testData.max_value, 9);
    EXPECT_EQ(testData.max_index, 4);
}

TEST_F(StdThreadTest, AverageThread) {
    stdthread::CalculateAverage(testData);

    EXPECT_NEAR(testData.avg_value, 4.6666, 0.001);
}

TEST_F(StdThreadTest, ReplaceMinMaxWithAverage) {
    testData.min_value = 1;
    testData.min_index = 3;
    testData.max_value = 9;
    testData.max_index = 4;
    testData.avg_value = 4.6666;

    stdthread::ReplaceMinMaxWithAverage(testData);

    EXPECT_NEAR(testData.array[testData.min_index], 4.6666, 0.001);
    EXPECT_NEAR(testData.array[testData.max_index], 4.6666, 0.001);

    EXPECT_EQ(testData.array[0], 5);
    EXPECT_EQ(testData.array[1], 2);
    EXPECT_EQ(testData.array[2], 8);
    EXPECT_EQ(testData.array[5], 3);
}

TEST_F(StdThreadTest, EmptyArray) {
    testData.array.clear();

    stdthread::FindMinMax(testData);

    stdthread::CalculateAverage(testData);

    EXPECT_EQ(testData.avg_value, 0);

    EXPECT_NO_THROW(stdthread::ReplaceMinMaxWithAverage(testData));
}
TEST_F(StdThreadTest, SingleElementArray) {
    testData.array = { 42 };

    stdthread::FindMinMax(testData);

    stdthread::CalculateAverage(testData);

    EXPECT_EQ(testData.min_value, 42);
    EXPECT_EQ(testData.max_value, 42);
    EXPECT_EQ(testData.avg_value, 42);
    EXPECT_EQ(testData.min_index, 0);
    EXPECT_EQ(testData.max_index, 0);

    stdthread::ReplaceMinMaxWithAverage(testData);
    EXPECT_EQ(testData.array[0], 42);
}

TEST_F(StdThreadTest, DuplicateMinMaxValues) {
    testData.array = { 5, 9, 2, 9, 2, 5 };

    stdthread::FindMinMax(testData);

    EXPECT_EQ(testData.min_value, 2);
    EXPECT_EQ(testData.min_index, 2);
    EXPECT_EQ(testData.max_value, 9);
    EXPECT_EQ(testData.max_index, 1);
}

TEST_F(StdThreadTest, ProcessArrayComplete) {
    bool result = stdthread::ProcessArray(testData);

    EXPECT_TRUE(result);

    EXPECT_NEAR(testData.array[3], testData.avg_value, 0.001);
    EXPECT_NEAR(testData.array[4], testData.avg_value, 0.001);
}

TEST_F(StdThreadTest, ThreadSafeOutput) {
    std::string message = "Test message";
    EXPECT_NO_THROW(stdthread::safe_cout(message));
}