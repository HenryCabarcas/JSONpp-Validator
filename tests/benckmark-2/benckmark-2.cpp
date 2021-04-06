#include "../../src/Jsonpp_Validator.hpp"
#include <chrono>
#include <sstream>

std::vector<std::string> _readFile(const char *filename)
{
    std::string STRING;
    std::vector<std::string> output;
    std::ifstream nfile;
    nfile.open(filename);
    while (nfile.eof() == false) // To get you all the lines.
    {
        getline(nfile, STRING); // Saves the line in STRING.
        output.push_back(STRING);
    }
    nfile.close();
    return output;
}

int main()
{

    const char *filename = "../list-2.txt"; // List of files to be in the benchmark
    int it_num = 10000;                     // Verifications per file
    json::Validator val;                    // JSON Validation object declaration
    std::ifstream infile;                   // Opens the list file
    std::string line;                       // Stores one line of the list file
    std::vector<std::string> file = _readFile(filename);
    for (int j = 0; j < file.size(); ++j)
        file[j] = "../" + file[j];
    long double t_accumulated = 0.0, actual = 0.0, max = 0.0, min = 1.0e8;
    int success = 0, fails = 0;
    auto time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < it_num; ++i)
    {
        /** Each line of the list file has the path of every json file in this folders,
	* 	what means we have to read the list file line per line and verify the file 
	*	of each line as next:
	*/
        bool valid = false;
        int j = 0;
        time = std::chrono::high_resolution_clock::now();
        for (j = 0; j < file.size(); ++j)
            valid = val.CheckFromFile(file[j].c_str());
        actual = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - time).count() / 1000000.0;
        // Redefine the time values in case some changed:
        min = actual < min && actual > 0 ? actual : min;
        max = actual > max ? actual : max;
        t_accumulated += actual;
        // Check if file is a correct JSON file by checking the verifiaction process output:
        if (valid)
            success++;
        else
            fails++;
    }
    std::cout << "\n\tMin: " << min
              << " ms, Avg: " << t_accumulated / it_num
              << " ms, Max: " << max
              << " ms \n\tCorrects: " << success
              << ", Errors: " << fails
              << ", Tests per second: " << 1000.f / (t_accumulated / it_num)
              << '\n';

    system("pause");

    return 0;
}