#include <iostream>
#include <fstream>
#include <string>
#include "employee.h"

int main(int argc, char* argv[]) {
	
	char* fileName = argv[1];
	int numberOfRecords = std::atoi(argv[2]);

	std::ofstream fout(fileName, std::ios::binary);

	if (!fout) {
		std::cout << "Error: binary file didn't open.";
	}

	for (int i = 0; i < numberOfRecords; i++) {
		employee e;

		std::cout << "\nNumber: ";
		std::cin >> e.num;
		std::cout << "\nName: ";
		std::cin >> e.name;
		std::cout << "\nHours: ";
		std::cin >> e.hours;

		fout.write(reinterpret_cast<char*>(&e), sizeof(e));
	}

	fout.close();
	std::cout << "Succesfully!";
}