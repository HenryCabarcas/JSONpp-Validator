# JSONpp Validator

[![MSBuild](https://github.com/nerdtronik/JSONpp-Validator/actions/workflows/msbuild.yml/badge.svg?branch=main)](https://github.com/nerdtronik/JSONpp-Validator/actions/workflows/msbuild.yml)
[![clang](https://github.com/nerdtronik/JSONpp-Validator/actions/workflows/clang.yml/badge.svg?branch=main)](https://github.com/nerdtronik/JSONpp-Validator/actions/workflows/clang.yml)
[![g++](https://github.com/nerdtronik/JSONpp-Validator/actions/workflows/g++.yml/badge.svg?branch=main)](https://github.com/nerdtronik/JSONpp-Validator/actions/workflows/g++.yml)

This is a simple JSON validator for C++11 and up with some very useful features, but first don't forget to include your headers 😊:

```cpp
#include "JSONpp_Validator.hpp"
```

## Features

### 1. File parsing

Using a file path easily you can validate a file in milliseconds 😎:

```cpp
// JSON file path:
const char* filename = "test.json";

// 1. The first method is to use the object constructor
json::Validator val(filename);

if(val.LastValid())
{
    // This is just for information purposes:
    std::cout << "\nFile: " << val.GetFileName()              
              << "\n\tSize: " << (val.GetFileSize() / 1024.0)
              << " KB" 
              << "\n\tResult: " << val.What();   
}

// 2. The second method is by using CheckFromFile member:
if (val.CheckFromFile(filename)) 
{ // If the file is a valid JSON, prints Success
    std::cout << "\nSuccess\n";
}
else 
{ // If the file is an invalid JSON, prints the error message
    std::cout << val.What();
}
```

And we obtain the next output when the file is a valid json 😋:

```shell
File: test.json
        Size: 10.6611 KB
        Result: This is a valid JSON
Success
```

If not 😥, then we have an output like:

```shell
Error at line 464, in character -> ',' in position: 23
Expected -> ']' , '[' , '{' , 'Value' , 'Definition'
```

You can review at the tests folder, exactly at [single-file](/tests/single-file) folder.

### 2. String parsing

If you have a string instead of a file, you can validate it with the next method:

```cpp
// A very convenient way to write json in c++ code don't you think?
char* json_string = JSON(
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
    }
);

// 1. You can use the constructor:
json::Validator val(json_string);

// 2. Or you can use the CheckFromBuffer function:
bool valid = val.CheckFromBuffer(json_string);
```

A functional test is stored at [tests/string-json](/tests/string-json).
In case your string is stored into a variable you can do the next 🙈:

```cpp
// This is your stored json string, it can be also a const char*
std::string json_var = "{ \"tests\" : [ { \"data\" : {\"foo\" : false}, \"valid\" : true } ] }";

// You can easily allocate it with calling str2json:
char* json_string = json::str2json(json_var);

// Then use the constructor as validator
json::Validator val(json_string);

// Or the CheckFromBuffer function
val.CheckFromBuffer(json_string);
```

Another functional test of this is in the file [var-json](/tests/var-json).

### 3. Custom error handle

In case the next error output doesn't suits your needs:

```shell
Error at line 44, in character -> ',' in position: 3
Expected -> ']' , '[' , '{' , 'Value' , 'Definition'
```

You can obtain the error parameters with the following methods:

```cpp
json::Validator val(some_json);

// To get the filename just:
const char * filename = val.GetFileName();
// To get the file size only do:
long int mySize = val.GetFileSize(); // Divide by 1024 to get KB
// To obtain the error line just:
int errorLine = val.GetErrorLine();
// If you want to get the cursor pos in the error line just:
int cursorPos = val.GetErrorPos();
// And if you want to get the expected input:
std::vector<const char*> expected = val.GetExpected();
```

## Benchmark

Ok, so pretty, but how fast?. Well, a very inaccurate way to tell you is <b>very fast</b>. That could be true if you have an SSD or a very good processor. In my case using:

| <b>Component</b> | Description                          |
| ---------------- | ------------------------------------ |
| Processor        | Intel Core I5 5200U @2.2GHz (4 CPUs) |
| Ram              | 12GB DDR3 @1600MHz                   |
| Disk             | 250GB SSD Samsung EVO 850 @560Mb/s   |

With those specs and parsing 10000 times the json files stored in the [tests](/tests) directory and using the [draft7](/tests/draft7) folder from the [JSON Schema Test Suite](https://github.com/json-schema-org/JSON-Schema-Test-Suite); some values I get (some of them) are:

- with GCC 8.1.0:
  
  ```shell
  File: allOf.json
          Size: 7.60059 KB
          Min: 0 ms, Avg: 0.099594 ms, Max: 2.022 ms
          Corrects: 10000, Errors: 0, Files per second: 10040.8
  
  File: anyOf.json
          Size: 5.35059 KB
          Min: 0 ms, Avg: 0.0808818 ms, Max: 2.494 ms
          Corrects: 10000, Errors: 0, Files per second: 12363.7
  
  File: dependencies.json
          Size: 6.71484 KB
          Min: 0 ms, Avg: 0.0945804 ms, Max: 2.473 ms
          Corrects: 10000, Errors: 0, Files per second: 10573
  
  File: exclusiveMinimum.json
          Size: 0.756836 KB
          Min: 0 ms, Avg: 0.0455853 ms, Max: 2.021 ms
          Corrects: 10000, Errors: 0, Files per second: 21936.9
  
  File: format.json
          Size: 15.9902 KB
          Min: 0 ms, Avg: 0.152931 ms, Max: 2.34 ms
          Corrects: 10000, Errors: 0, Files per second: 6538.91
  
  File: idn-hostname.json
          Size: 13.4961 KB
          Min: 0 ms, Avg: 0.110601 ms, Max: 2.038 ms
          Corrects: 10000, Errors: 0, Files per second: 9041.5
  
  File: ipv4.json
          Size: 1.09961 KB
          Min: 0 ms, Avg: 0.0468375 ms, Max: 2.025 ms
          Corrects: 10000, Errors: 0, Files per second: 21350.4
  
  File: test-with-errors.json
          Size: 10.6621 KB
          Min: 0 ms, Avg: 0.0902663 ms, Max: 2.214 ms
          Corrects: 0, Errors: 10000, Files per second: 11078.3
  
  File: catalog.json
          Size: 1.686 MB
          Min: 10.987 ms, Avg: 15.5964 ms, Max: 170.767 ms
          Corrects: 10000, Errors: 0, Files per second: 64.11
  ```

And I know, its not the fastest JSON validator out there 😐, but it makes its job.

Obviously if there is a problem, you can open a issue whenever you want, I'll be working on this the most I can.
