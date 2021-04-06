#include "../../src/Jsonpp_Validator.hpp"
#include <chrono>
#include <sstream>

int main()
{

	const char *filename = "../list.txt"; // List of files to be in the benchmark
	int it_num = 10000;					  // Verifications per file
	json::Validator val;				  // JSON Validation object declaration
	std::ifstream infile(filename);		  // Opens the list file
	std::string line;					  // Stores one line of the list file

	/** Each line of the list file has the path of every json file in this folders,
	* 	what means we have to read the list file line per line and verify the file 
	*	of each line as next:
	*/
	while (std::getline(infile, line))
	{
		std::string jsonFile("../" + line); // Due to json files are in a parent folder, we add ../
		// This variables stores the time values required in each test:
		long double t_accumulated = 0.0, actual = 0.0, max = 0.0, min = 1.0e8;
		int success = 0, fails = 0; // Number of success readings and failed ones
		// Read and verify the file it_num times:
		for (int i = 0; i < it_num; ++i)
		{
			// 1. Read the time before start:
			auto time = std::chrono::high_resolution_clock::now();
			// 2. Verifies the file:
			bool valid = val.CheckFromFile(jsonFile.c_str());
			// 3. Calculates the passed time in ms after the file is verified:
			actual = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - time).count() / 1000000.0;
			// 4. Redefine the time values in case some changed:
			min = actual < min ? actual : min;
			max = actual > max ? actual : max;
			t_accumulated += actual;
			// 5. Check if file is a correct JSON file by checking the verifiaction process output:
			if (valid)
				success++;
			else
			{
				//std::cout << val.What(); // Prints the error in case the verification failed
				fails++;
			}
			// 6. Tries again
		}
		// 7. Once the file was read it_num times, prints the collected information
		std::cout << "\nFile: " << val.GetFileName()
				  << "\n\tSize: " << (val.GetFileSize() / 1024.0) << " KB"
				  << "\n\tMin: " << min
				  << " ms, Avg: " << t_accumulated / it_num
				  << " ms, Max: " << max
				  << " ms \n\tCorrects: " << success
				  << ", Errors: " << fails
				  << ", Files per second: " << 1000.f / (t_accumulated / it_num)
				  << '\n';
	}

	return 0;
}