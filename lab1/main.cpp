#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
#include "employee.h"

int main(int argc, char* argv[]) {
	std::string fileName;
	int numberOfRecords;

	std::cout << "File name: ";
	std::cin >> fileName;
	std::cout << "\nNumber of records: ";
	std::cin >> numberOfRecords;

	std::string path = argv[0];
	path.replace(path.find("Main.exe"), path.rfind("Main.exe"), "");

	STARTUPINFOA si;
	PROCESS_INFORMATION piCom;
	ZeroMemory(&si, sizeof(STARTUPINFOA));
	si.cb = sizeof(STARTUPINFOA);

    if (!CreateProcessA((path + "Creator.exe").c_str(),
        (LPSTR)((path + "Creator.exe " + fileName + " " + std::to_string(numberOfRecords)).c_str()),
        NULL, NULL, FALSE, NULL, NULL, NULL, &si, &piCom))
    {
        DWORD error = GetLastError();
        std::cout << "Bad. Error code: " << error << std::endl;
        return 1;
    }

    WaitForSingleObject(piCom.hProcess, INFINITE);
    std::cout << "Good" << std::endl;

    CloseHandle(piCom.hThread);
    CloseHandle(piCom.hProcess);
    std::ifstream bin(fileName, std::ios::binary);
    employee e;
    while (bin.read(reinterpret_cast<char*>(&e), sizeof(e))) {
        std::cout << " Employee ID: " << e.num << " Employee Name: " << e.name << " Hours Worked: " << e.hours << std::endl;
    }
    std::string reportFile;
    int payPerHours = 0;
    std::cout << "Enter name of report file:" << std::endl;
    std::cin >> reportFile;
    std::cout << "Enter cost:" << std::endl;
    std::cin >> payPerHours;
    ZeroMemory(&si, sizeof(STARTUPINFOA));
    if (!CreateProcessA((path + "Reporter.exe").c_str(),
        (LPSTR)((path + "Reporter.exe " + fileName + " " + reportFile + " " + std::to_string(payPerHours)).c_str()),
        NULL, NULL, FALSE, NULL, NULL, NULL, &si, &piCom))
    {
        DWORD error = GetLastError();
        std::cout << "Bad. Error code: " << error << std::endl;
        return 1;
    }
    WaitForSingleObject(piCom.hProcess, INFINITE);

    CloseHandle(piCom.hThread);
    CloseHandle(piCom.hProcess);
    std::ifstream tin(reportFile);
    std::string stream;
    while (getline(tin, stream))
    {
        std::cout << stream << std::endl;
    }
    tin.close();
    std::cout << "\Press any key to end the program... ";
    _getch();
    return 0;
}







