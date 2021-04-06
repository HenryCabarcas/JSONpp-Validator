#include "jpp.h"
#include "jpp_validator.h"
#include <math.h>
#include <chrono>
#include <fstream>
std::chrono::steady_clock::time_point hClock()
{
	return std::chrono::high_resolution_clock::now();
}

std::uint32_t TimeDuration(std::chrono::steady_clock::time_point Time)
{
	return (std::chrono::high_resolution_clock::now() - Time) / std::chrono::nanoseconds(1);
}
void log(const std::let& val) {
	std::cout << &val << std::endl;
}
int main()
{
	const int MAX_LENGTH = 52428800;
	const char* filename = "list.txt";
	int lines = 0;
	int it_num = 1000;
	auto time = hClock();
	json::Validator val;
	std::ifstream infile(filename);
	std::string line;
	static bool valid = false;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		double last = 0.0, acum = 0.0, actual = 0.0, max = 0.0, min = 1e9;
		int success = 0, fails = 0;
		for (int i = 0; i < it_num; ++i) {
			time = hClock();
			valid = val.checkFromFile(line.c_str());
			actual = TimeDuration(time) / 1000000.0;
			min = actual < min ? actual : min;
			max = actual > max ? actual : max;
			acum += actual;
			if (valid) success++;
			else {
				std::cout << val.what();
				fails++;
			}
		}
		std::cout << "\nFile: "
			<< line.substr(line.find_last_of('/') + 1)
			<< "\n\tSize: " << (val.getFileSize() / 1024.0) << " KB"
			<< "\n\tMin: " << min
			<< " ms, Avg: " << acum / it_num
			<< " ms, Max: " << max
			<< " ms \n\tCorrects: " << success
			<< ", Errors: " << fails
			<< ", Files per second: " << 1000.f / (acum / it_num)
			<< '\n';
	}

	if (val.lastValid())
		std::cout << "OK" << std::endl;
	else
		std::cout << "ERROR" << std::endl;
	return 0;
	std::JSON handler;
	handler.readJSON("test.json");
	//std::cout << handler << std::endl;


	//Vec<int> Any = {3, 02, 50, 90};
	//Any.forEach([](auto item) {
	//	std::cout << std::to_string(item) << std::endl;
	//});
	//vec2 a({3,4});
	//std::cout << a.x<< std::endl;
	//system("pause");
	return 0;
}
