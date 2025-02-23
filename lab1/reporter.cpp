#include <iostream>
#include <fstream>
#include <string>
#include "employee.h"

int main(int argc, char* argv[]) {

	char* fileName = argv[1];
	char* textFileName = argv[2];
	int payPerHours = std::atoi(argv[3]);

	std::ifstream bin(fileName, std::ios::binary);
	if (!bin) {
		std::cout << "Error: binary file didn't open.";
	}

	std::ofstream tout(textFileName);
	if (!tout) {
		std::cout << "Error: text file didn't open.";
	}

	tout << "File '" << fileName << "'\n";
	int res = 0;
	employee e;
	while (bin.read(reinterpret_cast<char*>(&e), sizeof(e)))
	{
		res = payPerHours * e.hours;
		tout << e.num << " " << e.name << " " << e.hours << " " << res << "\n";
	}
	bin.close();
	tout.close();
	std::cout << "Succesfully!";
}