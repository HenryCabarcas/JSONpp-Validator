#include "../../src/JSONpp_Validator.hpp"

int main()
{
    // This is your stored json string, it can be also a const char*
    std::string json_var = "{ \"tests\" : [ { \"data\" : {\"foo\" : false}, \"valid\" : true } ] }";

    // You can easily allocate it with calling str2json:
    char *json_string = json::str2json(json_var);

    // Then use the constructor as validator
    json::Validator val(json_string);
    
    std::cout << "\nFile: " << val.GetFileName()                       // Prints the filename
              << "\n\tSize: " << (val.GetFileSize() / 1024.0) << " KB" // Prints the file size
              << "\n\tResult: " << val.What();                         // Prints the result message

    // Or the CheckFromBuffer function
    val.CheckFromBuffer(json_string);

    std::cout << "\nFile: " << val.GetFileName()                       // Prints the filename
              << "\n\tSize: " << (val.GetFileSize() / 1024.0) << " KB" // Prints the file size
              << "\n\tResult: " << val.What();                         // Prints the result message

    return 0;
}