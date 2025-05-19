#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#define PIPE_NAME "\\\\.\\pipe\\employee_pipe"
#define BUF_SIZE 1024
#define MAX_CLIENTS 10

#pragma pack(push, 1)
struct employee {
    int num;
    char name[10];
    double hours;
};
#pragma pack(pop)

enum RequestType {
    READ_REQUEST,
    WRITE_REQUEST,
    RELEASE_READ,
    RELEASE_WRITE,
    CLIENT_EXIT
};

struct Request {
    RequestType type;
    int employee_num;
    employee emp_data;
};

struct Response {
    bool success;
    employee emp_data;
};

#endif