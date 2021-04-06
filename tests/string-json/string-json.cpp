#include "../../src/Jsonpp_Validator.hpp"

int main()
{
    // A very convenient way to write json in c++ code don't you think?
    char *json_string = JSON(
        {
            "tests" : [
                {
                    "description" : "match",
                    "data" : {"foo" : false},
                    "valid" : true
                },
                /* Comments only allowed with / * * / 
            format due all this string is just a line */
                [0, 1, 2, 3, 2, -3e-4],
                {
                    "description", "mismatch",
                    "data" : {"bar" : false},
                    "valid" : false
                }
            ]
        });
    // 1. You can use the constructor:
    json::Validator val(json_string);

    std::cout << "\nFile: " << val.GetFileName()                       // Prints the filename
              << "\n\tSize: " << (val.GetFileSize() / 1024.0) << " KB" // Prints the file size
              << "\n\tResult: " << val.What();                         // Prints the result message

    // 2. Or you can use the CheckFromBuffer function:
    val.CheckFromBuffer(json_string);

    std::cout << "\nFile: " << val.GetFileName()                       // Prints the filename
              << "\n\tSize: " << (val.GetFileSize() / 1024.0) << " KB" // Prints the file size
              << "\n\tResult: " << val.What();                         // Prints the result message

    return 0;
}