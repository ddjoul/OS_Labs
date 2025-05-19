#include <gtest/gtest.h>
#include "employee.h"

TEST(ClientLogicTest, RequestDataIntegrity) {
    Request req;
    req.type = WRITE_REQUEST;
    req.employee_num = 42;
    req.emp_data = {42, "Anna", 37.5};

    EXPECT_EQ(req.type, WRITE_REQUEST);
    EXPECT_EQ(req.employee_num, 42);
    EXPECT_EQ(req.emp_data.num, 42);
    EXPECT_STREQ(req.emp_data.name, "Anna");
    EXPECT_DOUBLE_EQ(req.emp_data.hours, 37.5);
}

TEST(ClientLogicTest, ResponseSuccess) {
    Response res;
    res.success = true;
    res.emp_data = {7, "Tom", 40.0};

    EXPECT_TRUE(res.success);
    EXPECT_EQ(res.emp_data.num, 7);
    EXPECT_STREQ(res.emp_data.name, "Tom");
    EXPECT_DOUBLE_EQ(res.emp_data.hours, 40.0);
}
