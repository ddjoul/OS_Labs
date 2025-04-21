#include <gtest/gtest.h>
#include "employee.h"
#include <fstream>
#include <cstring>

//ѕроверка, что размер структуры соответствует ожидани€м
TEST(EmployeeTest, SizeCheck) {
    employee emp;
    EXPECT_EQ(sizeof(emp.num), 4);
    EXPECT_EQ(sizeof(emp.name), 10);
    EXPECT_EQ(sizeof(emp.hours), 8);
    EXPECT_EQ(sizeof(employee), 24);
}

// ѕроверка, что размер структуры соответствует ожидани€м
TEST(EmployeeTest, BinaryWriteRead) {
    const char* testFileName = "test_employee.bin";

    employee original;
    original.num = 123;
    strncpy(original.name, "TestName", sizeof(original.name));
    original.hours = 40.5;
    {
        std::ofstream fout(testFileName, std::ios::binary);
        ASSERT_TRUE(fout) << "Failed to open file for writing";
        fout.write(reinterpret_cast<char*>(&original), sizeof(original));
    }

    employee read;
    {
        std::ifstream fin(testFileName, std::ios::binary);
        ASSERT_TRUE(fin) << "Failed to open file for reading";
        fin.read(reinterpret_cast<char*>(&read), sizeof(read));
    }

    EXPECT_EQ(original.num, read.num);
    EXPECT_STREQ(original.name, read.name);
    EXPECT_DOUBLE_EQ(original.hours, read.hours);

    remove(testFileName);
}

// “ест граничных условий дл€ имени
TEST(EmployeeTest, NameBoundary) {
    employee emp;

    const char* longName = "123456789";
    strncpy(emp.name, longName, sizeof(emp.name));
    EXPECT_STREQ(emp.name, longName);

    const char* tooLongName = "1234567890abc";
    strncpy(emp.name, tooLongName, sizeof(emp.name));
    EXPECT_STRNE(emp.name, tooLongName);
    EXPECT_EQ(strlen(emp.name), 9);
}