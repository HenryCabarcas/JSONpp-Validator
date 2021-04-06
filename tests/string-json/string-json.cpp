#include "../../src/Jsonpp_Validator.hpp"

int main()
{
    char *json_string = JSON(
        {
            "tests" : [
                {
                    "description" : "match",
                    "data" : {"foo" : false},
                    "valid" : true
                },
                {
                    "description" : "recursive match",
                    "data" : {"foo" : {"foo" : false}},
                    "valid" : true
                },
                /* Comments only allowed with / * * / format due all this string is just a line*/
                [0, 1, 2, 3, 2, -3e-4],
                {
                    "description", "mismatch",
                    "data" : {"bar" : false},
                    "valid" : false
                },
                {
                    "description" : "recursive mismatch",
                    "data" : {"foo" : {"bar" : false}},
                    "valid" : false
                }
            ]
        }
    );

    json::Validator val;

    val.CheckFromBuffer(json_string);

    std::cout << "\nFile: " << val.GetFileName()                       // Prints the filename
              << "\n\tSize: " << (val.GetFileSize() / 1024.0) << " KB" // Prints the file size
              << "\n\tResult: " << val.What();                         // Prints the result message

    return 0;
}