#include "../../src/Jsonpp_Validator.hpp"

int main()
{
    // Valid JSON file test:
    const char *filename = "../test.json";
    // JSON file with errors test:
    // const char *filename = "../test-with-errors.json";

    // 1. The first method is to use the object constructor
    json::Validator val(filename);
    std::cout << "\nFile: " << val.GetFileName()                       // Prints the filename
              << "\n\tSize: " << (val.GetFileSize() / 1024.0) << " KB" // Prints the file size
              << "\n\tResult: " << val.What();                         // Prints the result message

    // 2. The second method is by using CheckFromFile member:
    if (val.CheckFromFile(filename))
    {
        // If the file is a valid JSON, prints Success
        std::cout << "\nSuccess\n";
    }
    else
    {
        // If the file is an invalid JSON, prints the error message
        std::cout << val.What();
    }

    return 0;
}