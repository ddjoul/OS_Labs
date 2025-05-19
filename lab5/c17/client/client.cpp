#include <windows.h>
#include <iostream>
#include "employee.h"

HANDLE ConnectToServer() {
    while (true) {
        HANDLE hPipe = CreateFileA(
            PIPE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

        if (hPipe != INVALID_HANDLE_VALUE) {
            DWORD mode = PIPE_READMODE_MESSAGE;
            if (SetNamedPipeHandleState(hPipe, &mode, NULL, NULL)) {
                return hPipe;
            }
            CloseHandle(hPipe);
        }

        DWORD err = GetLastError();
        if (err == ERROR_PIPE_BUSY) {
            if (!WaitNamedPipeA(PIPE_NAME, 5000)) {
                std::cerr << "Could not open pipe: 5 second timeout" << std::endl;
                return INVALID_HANDLE_VALUE;
            }
        }
        else {
            std::cerr << "Failed to connect: " << err << std::endl;
            return INVALID_HANDLE_VALUE;
        }
    }
}

bool SendRequest(HANDLE hPipe, Request& req, Response& res) {
    DWORD bytesWritten;
    if (!WriteFile(hPipe, &req, sizeof(req), &bytesWritten, NULL)) {
        std::cerr << "Write error: " << GetLastError() << std::endl;
        return false;
    }

    DWORD bytesRead;
    if (!ReadFile(hPipe, &res, sizeof(res), &bytesRead, NULL)) {
        std::cerr << "Read error: " << GetLastError() << std::endl;
        return false;
    }

    return true;
}

void ModifyRecord(HANDLE hPipe) {
    Request req;
    Response res;

    std::cout << "Enter employee ID: ";
    std::cin >> req.employee_num;
    req.type = WRITE_REQUEST;

    if (!SendRequest(hPipe, req, res)) return;

    if (!res.success) {
        std::cout << "Employee not found!" << std::endl;
        return;
    }

    std::cout << "Current data:\n"
        << "ID: " << res.emp_data.num << "\n"
        << "Name: " << res.emp_data.name << "\n"
        << "Hours: " << res.emp_data.hours << std::endl;

    std::cout << "Enter new name: ";
    std::cin >> res.emp_data.name;
    std::cout << "Enter new hours: ";
    std::cin >> res.emp_data.hours;

    req.type = RELEASE_WRITE;
    req.emp_data = res.emp_data;
    if (!SendRequest(hPipe, req, res)) return;

    std::cout << (res.success ? "Update successful!" : "Update failed!") << std::endl;
}

void ReadRecord(HANDLE hPipe) {
    Request req;
    Response res;

    std::cout << "Enter employee ID: ";
    std::cin >> req.employee_num;
    req.type = READ_REQUEST;

    if (!SendRequest(hPipe, req, res)) return;

    if (res.success) {
        std::cout << "Employee data:\n"
            << "ID: " << res.emp_data.num << "\n"
            << "Name: " << res.emp_data.name << "\n"
            << "Hours: " << res.emp_data.hours << std::endl;
    }
    else {
        std::cout << "Employee not found!" << std::endl;
    }
}

int main() {
    HANDLE hPipe = ConnectToServer();
    if (hPipe == INVALID_HANDLE_VALUE) {
        return 1;
    }

    int choice;
    do {
        std::cout << "\nMenu:\n"
            << "1. Modify record\n"
            << "2. Read record\n"
            << "3. Exit\n"
            << "Choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: ModifyRecord(hPipe); break;
        case 2: ReadRecord(hPipe); break;
        case 3: break;
        default: std::cout << "Invalid choice!" << std::endl;
        }
    } while (choice != 3);

    // Уведомляем сервер о завершении
    Request req;
    req.type = CLIENT_EXIT;
    Response res;
    SendRequest(hPipe, req, res);

    CloseHandle(hPipe);
    return 0;
}