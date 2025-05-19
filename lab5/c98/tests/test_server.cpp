#include <gtest/gtest.h>
#include "employee.h"
#include <vector>
#include <algorithm>

bool HandleRequest(Request& req, Response& res, std::vector<employee>& data) {
    res.success = false;
    for (auto& emp : data) {
        if (emp.num == req.employee_num) {
            switch (req.type) {
                case READ_REQUEST:
                    res.success = true;
                    res.emp_data = emp;
                    break;
                case WRITE_REQUEST:
                    res.success = true;
                    res.emp_data = emp;
                    break;
                case RELEASE_WRITE:
                    emp = req.emp_data;
                    res.success = true;
                    break;
                default:
                    return false;
            }
            return true;
        }
    }
    return true;
}

TEST(ServerLogicTest, ReadExistingEmployee) {
    std::vector<employee> db = {
        {1, "Max", 20.0},
        {2, "Lily", 25.5}
    };

    Request req = { READ_REQUEST, 2 };
    Response res;

    bool handled = HandleRequest(req, res, db);

    EXPECT_TRUE(handled);
    EXPECT_TRUE(res.success);
    EXPECT_EQ(res.emp_data.num, 2);
    EXPECT_STREQ(res.emp_data.name, "Lily");
    EXPECT_DOUBLE_EQ(res.emp_data.hours, 25.5);
}

TEST(ServerLogicTest, ModifyEmployee) {
    std::vector<employee> db = {
        {5, "Nick", 10.0}
    };

    Request writeReq = { WRITE_REQUEST, 5 };
    Response writeRes;
    HandleRequest(writeReq, writeRes, db);

    EXPECT_TRUE(writeRes.success);
    EXPECT_EQ(writeRes.emp_data.num, 5);

    Request updateReq = {
        RELEASE_WRITE,
        5,
        {5, "Nick_New", 55.5}
    };
    Response updateRes;
    HandleRequest(updateReq, updateRes, db);

    EXPECT_TRUE(updateRes.success);
    EXPECT_EQ(db[0].hours, 55.5);
    EXPECT_STREQ(db[0].name, "Nick_New");
}
