#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <intrin.h>
#include <conio.h>  
#include "employee.h"

std::vector<employee> employees;
HANDLE hMutex;
volatile LONG activeClients = 0;
bool serverRunning = true;


void CreateFile() {
    std::string filename;
    std::cout << "Enter filename: ";
    std::cin >> filename;

    std::ofstream out(filename.c_str(), std::ios::binary);
    if (!out) {
        std::cerr << "Cannot create file!" << std::endl;
        return;
    }

    int count;
    std::cout << "Enter number of employees: ";
    std::cin >> count;

    for (int i = 0; i < count; ++i) {
        employee emp;
        std::cout << "Employee " << i + 1 << ":\n";
        std::cout << "ID: "; std::cin >> emp.num;
        std::cout << "Name: "; std::cin >> emp.name;
        std::cout << "Hours: "; std::cin >> emp.hours;
        out.write(reinterpret_cast<char*>(&emp), sizeof(emp));
        employees.push_back(emp);
    }
    out.close();
}

void PrintFile() {
    std::cout << "\nCurrent file content:\n";
    for (const auto& emp : employees) {
        std::cout << "ID: " << emp.num
            << "\tName: " << emp.name
            << "\tHours: " << emp.hours << std::endl;
    }
}

DWORD WINAPI ProcessClient(LPVOID lpParam) {
    HANDLE hPipe = (HANDLE)lpParam;
    Request req;
    Response res;
    DWORD bytesRead, bytesWritten;

    InterlockedIncrement(&activeClients);
    std::cout << "Client processing started. Active clients: " << activeClients << std::endl;

    while (true) {
        if (!ReadFile(hPipe, &req, sizeof(req), &bytesRead, NULL)) {
            DWORD err = GetLastError();
            if (err == ERROR_BROKEN_PIPE) {
                std::cout << "Client disconnected normally" << std::endl;
            }
            else {
                std::cerr << "ReadFile error: " << err << std::endl;
            }
            break;
        }

        if (req.type == CLIENT_EXIT) {
            std::cout << "Client requested exit" << std::endl;
            break;
        }

        res.success = false;
        for (auto& emp : employees) {
            if (emp.num == req.employee_num) {
                switch (req.type) {
                case READ_REQUEST:
                    res.success = true;
                    res.emp_data = emp;
                    break;

                case WRITE_REQUEST:
                    WaitForSingleObject(hMutex, INFINITE);
                    res.success = true;
                    res.emp_data = emp;
                    break;

                case RELEASE_WRITE:
                    emp = req.emp_data;
                    res.success = true;
                    ReleaseMutex(hMutex);
                    break;
                }
                break;
            }
        }

        if (!WriteFile(hPipe, &res, sizeof(res), &bytesWritten, NULL)) {
            std::cerr << "WriteFile error: " << GetLastError() << std::endl;
            break;
        }
    }

    InterlockedDecrement(&activeClients);
    std::cout << "Client processing finished. Active clients: " << activeClients << std::endl;

    if (hPipe != INVALID_HANDLE_VALUE) {
        FlushFileBuffers(hPipe);
        DisconnectNamedPipe(hPipe);
        CloseHandle(hPipe);
    }
    return 0;
}

DWORD WINAPI ServerControl(LPVOID lpParam) {
    std::string command;
    while (serverRunning) {
        std::cout << "Enter 'stop' to shutdown server: ";
        std::cin >> command;
        if (command == "stop") {
            serverRunning = false;
            break;
        }
    }
    return 0;
}

int main() {
    CreateFile();
    PrintFile();

    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        std::cerr << "Mutex creation failed" << std::endl;
        return 1;
    }

    int clientCount;
    std::cout << "Enter number of client processes: ";
    std::cin >> clientCount;

    HANDLE hControlThread = CreateThread(NULL, 0, ServerControl, NULL, 0, NULL);
    if (hControlThread == NULL) {
        std::cerr << "Control thread creation failed" << std::endl;
        CloseHandle(hMutex);
        return 1;
    }

    for (int i = 0; i < clientCount && serverRunning; ++i) {
        HANDLE hPipe = CreateNamedPipeA(
            PIPE_NAME,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            sizeof(Response),
            sizeof(Request),
            0,
            NULL);

        if (hPipe == INVALID_HANDLE_VALUE) {
            std::cerr << "CreateNamedPipe failed: " << GetLastError() << std::endl;
            continue;
        }

        std::cout << "Waiting for client (" << i + 1 << "/" << clientCount << ")..." << std::endl;

        BOOL connected = ConnectNamedPipe(hPipe, NULL) ?
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (connected) {
            std::cout << "Client connected (" << i + 1 << "/" << clientCount << ")" << std::endl;
            HANDLE hClientThread = CreateThread(NULL, 0, ProcessClient, hPipe, 0, NULL);
            if (hClientThread == NULL) {
                std::cerr << "Client thread creation failed" << std::endl;
                CloseHandle(hPipe);
            }
            else {
                CloseHandle(hClientThread);
            }
        }
        else {
            std::cerr << "ConnectNamedPipe failed: " << GetLastError() << std::endl;
            CloseHandle(hPipe);
        }
    }

    while (activeClients > 0 && serverRunning) {
        Sleep(100);
    }

    serverRunning = false;
    WaitForSingleObject(hControlThread, INFINITE);

    PrintFile();
    std::cout << "All clients disconnected. Press any key to exit..." << std::endl;
    _getch();

    CloseHandle(hControlThread);
    CloseHandle(hMutex);
    return 0;
}