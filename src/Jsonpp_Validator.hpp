/**
 * @file Jsonpp_Validator.hpp
 * @author NerdTronik (cabarcasortiz@gmail.com)
 * @brief This file contains a JSON validator written in C++
 * @version 0.1
 * @date 2021-04-05
 * @copyright Copyright (c) 2021
 */

#pragma once

#ifndef _JSON_VALIDATOR_
#define _JSON_VALIDATOR_

//#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#define JSON(...) _strdup(#__VA_ARGS__);

#ifndef _FILE_READER_
#define _FILE_READER_
#include <fstream>
/**
 * @brief Reads a file and allocates it in memory in one read
 * 
 * @param fileName path of the file to be opened.
 * @param size pointer to a size variable in case you want the file size.
 * @return char* pointer to a memory of chars allocation
 */
inline char *readFile(const char *fileName, long int *size = (long int *)0)
{
	std::ifstream ifs(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	std::ifstream::pos_type fileSize = ifs.tellg();
	char *fi = new char[fileSize];
	ifs.seekg(0, ifs.beg);
	ifs.read(fi, fileSize);
	ifs.close();
	*size = fileSize;
	return fi;
}
#endif

/** @brief Namespace identifier of json operations */
namespace json
{
	std::string toLow(std::string value);
	std::string toUp(std::string value);
	char *str2json(const char *value);
	char *str2json(std::string value);
	/**  @brief Infile json possible operations  */
	enum class jsonOperations
	{
		none,
		objOpen,
		objClose,
		arrayOpen,
		arrayClose
	};
	/** @brief JSON validation class */
	class Validator
	{
	public:
		Validator() = default;
		/**
		 * @brief Construct a new JSON Validator object from a json file path
		 * @param filepath path to a json file
		 * @param verifyFileExtension flag to verify if file path has the .json extension
		 */
		Validator(const char *filepath, bool verifyFileExtension = true);
		/**
		 * @brief Construct a new Validator object from a buffer and validates it
		 * @param buffer JSON content pointer
		 */
		Validator(char *buffer) { CheckFromBuffer(buffer); }
		/**
		 * @brief Verify file from disk
		 * @param path Path to the json file
		 * @param verifyFileExtension Flag to verify or not the file .json extension
		 * @return true If the file is valid
		 * @return false If the file has an error
		 */
		bool CheckFromFile(const char *path, bool verifyFileExtension = true);
		/**
		 * @brief Verify JSON file from char buffer 
		 * @param buffer: Pointer to memory allocated file
		 * @return true If file is a valid JSON
		 * @return false If file is an invalid JSON
		 */
		bool CheckFromBuffer(char *buffer = (char *)0);
		/**
		 * @brief Gets result from last validation
		 * @return true If last JSON verification was successful
		 * @return false If las JSON verification was wrong
		 */
		bool LastValid() const { return fileIsValid; }
		/**
		 * @brief Gets error description in case there is an error
		 * @return const char* Error description
		 */
		const char *What() const { return errorDescription.c_str(); }
		/**
		 * @brief Get the File bytes size
		 * @return long int: File Size
		 */
		long int GetFileSize() const { return size; }
		/**
		 * @brief Get the File Name
		 * @return const char* 
		 */
		const char *GetFileName() const { return filename.c_str(); }
		/**
		 * @brief Get the Error Line number
		 * @return int 
		 */
		int GetErrorLine() const { return errorLine; }
		/**
		 * @brief Get the Error Character Position
		 * @return int 
		 */
		int GetErrorPos() const { return errorPos; }
		/**
		 * @brief Get the Expected values in the error position
		 * @return std::vector<const char *> of expected values
		 */
		std::vector<const char *> GetExpected() const { return expectedValues; }
		const short int error = -1; // Default error value
	private:
		/**
		 * @brief Verify if filepath points to a .json file
		 * @param filepath Path to .json file
		 * @return true If file has .json extension
		 * @return false If file hasn't .json extension
		 */
		bool VerifyIsJson(const char *filepath);
		/** @brief Parses String value and skips it */
		void ParseString();
		/** @brief Parses Boolean value and skips it */
		int ParseBoolean();
		/** @brief Parses Number value and skips it */
		void ParseNumber();
		/** @brief Parses Null value and skips it */
		bool ParseNull();
		/** @brief Parses Comments and skips them */
		bool parseComments();
		std::string errorDescription = "This is a valid JSON",					   // Description of error in case of one
			filename = "none";													   // Last file validated name
		std::vector<const char *> expectedValues = {"none"};					   // Values to be expected in the error instance
		long int idx = -1,														   // Buffer index and
			size = 0;															   // Buffer size
		int actualChar = 0, keyLevels = 0, levels = 0, limit = 0, actualParam = 0; // Temporal use variable
		int errorLine = 1;														   // Line number where the error is located
		int errorPos = 0;														   // Character position from error line start
		char actualH = ' ';														   // Determines the last syntax char
		char *Buffer = NULL;													   // Pointer to the JSON buffer
		jsonOperations lastOperation = jsonOperations::none;					   // Last Operation infile
		std::vector<int> arrayLevel;											   // Determines ho many objects are declared in an array to avoid the use of : in an array root
		bool fileIsValid = true;												   // Determines if file is a valid JSON
	};

	inline std::string toLow(std::string value)
	{
		for (auto &i : value)
			i = tolower(i);
		return value;
	}

	inline std::string toUp(std::string value)
	{
		for (auto &i : value)
			i = toupper(i);
		return value;
	}
	inline char *str2json(const char *value)
	{
		return _strdup(value);
	}
	inline char *str2json(std::string value)
	{
		return _strdup(value.c_str());
	}
	inline Validator::Validator(const char *filepath, bool verifyFileExtension)
	{
		CheckFromFile(filepath, verifyFileExtension);
	}

	inline bool Validator::VerifyIsJson(const char *filepath)
	{
		std::string f = toLow(filepath);
		return f.find(".json") >= 0 ? true : false;
	}

	inline bool Validator::CheckFromFile(const char *path, bool verifyFileExtension)
	{
		filename = path;
		filename = filename.substr(filename.find_last_of('/') + 1);
		if (verifyFileExtension)
		{
			if (VerifyIsJson(path))
			{
				Buffer = readFile(path, &size);
				return CheckFromBuffer();
			}
			errorDescription = "File has no '.json' extension, verify the path or deactivate the extension verification flag\n";
			expectedValues = {".json extension"};
			return fileIsValid = false;
		}
		Buffer = readFile(path, &size);
		return CheckFromBuffer();
	}

	inline bool Validator::parseComments()
	{
		idx++;
		if (Buffer[idx] == '*')
		{
			idx++;
			while (Buffer[idx] != '*' || Buffer[idx + 1] != '/')
			{
				idx++;
				if (idx >= size)
					return fileIsValid = false;
				if (Buffer[idx] == '\n')
					actualChar = idx;
			}
			idx++;
			return true;
		}
		else if (Buffer[idx] == '/')
		{
			idx++;
			while (Buffer[idx] != '\n')
			{
				idx++;
				if (idx >= size)
					return fileIsValid = false;
			}
			actualChar = idx;
			idx++;
			return true;
		}
		fileIsValid = false;
		return false;
	}

	inline void Validator::ParseString()
	{
		idx++;
		if (Buffer[idx] == '\"')
			return;
		while (Buffer[idx] != '\"')
		{
			if (Buffer[idx] == '\\' && Buffer[idx + 1] == '\"')
				idx++;
			idx++;
		}
	}

	inline int Validator::ParseBoolean()
	{
		switch (Buffer[idx])
		{
		case 't':
			idx++;
			if (idx > size - 1)
				return error;
			if (Buffer[idx] != 'r')
				return error;
			idx++;
			if (idx > size - 1)
				return error;
			if (Buffer[idx] != 'u')
				return error;
			idx++;
			if (idx > size - 1)
				return error;
			if (Buffer[idx] != 'e')
				return error;
			return 1;
		case 'f':
			idx++;
			if (idx > size - 1)
				return error;
			if (Buffer[idx] != 'a')
				return error;
			idx++;
			if (idx > size - 1)
				return error;
			if (Buffer[idx] != 'l')
				return error;
			idx++;
			if (idx > size - 1)
				return error;
			if (Buffer[idx] != 's')
				return error;
			idx++;
			if (idx > size - 1)
				return error;
			if (Buffer[idx] != 'e')
				return error;
			return 0;
		default:
			return error;
		}
		return error;
	}

	inline void Validator::ParseNumber()
	{
		bool keep = true;
		int num_dots = 0, num_minus = 0, num_nums = 0;
		std::string out = "";
		do
		{
			if (idx >= size)
				break;
			switch (Buffer[idx])
			{
			case 'e':
			case 'E':
				if (Buffer[idx - 1] == '-' || Buffer[idx - 1] == '+')
				{
					fileIsValid = false;
					return;
				}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				num_nums++;
				break;
			case '.':
				if (num_dots >= 1)
				{
					fileIsValid = false;
					return;
				}
				num_dots++;
				break;
			case '+':
				if (Buffer[idx - 1] == 'e' || Buffer[idx - 1] == 'E')
					break;
				fileIsValid = false;
				return;
			case '-':
				if (Buffer[idx - 1] == '0' ||
					Buffer[idx - 1] == '1' ||
					Buffer[idx - 1] == '2' ||
					Buffer[idx - 1] == '3' ||
					Buffer[idx - 1] == '4' ||
					Buffer[idx - 1] == '5' ||
					Buffer[idx - 1] == '6' ||
					Buffer[idx - 1] == '7' ||
					Buffer[idx - 1] == '8' ||
					Buffer[idx - 1] == '9')
				{
					fileIsValid = false;
					return;
				}
				if (num_minus == 1)
				{
					if (Buffer[idx - 1] != 'e' && Buffer[idx - 1] != 'E')
					{
						fileIsValid = false;
						return;
					}
				}
				else if (num_minus > 1)
				{
					fileIsValid = false;
					return;
				}
				num_minus++;
				break;
			default:
				if ((num_minus > 0 && num_nums <= 0) || Buffer[idx - 1] == '+' || Buffer[idx - 1] == 'e' || Buffer[idx - 1] == 'E')
				{
					fileIsValid = false;
					return;
				}
				keep = false;
				break;
			}
			idx++;
		} while (keep);
		idx -= 2;
	}

	inline bool Validator::ParseNull()
	{
		if (Buffer[idx] == 'n')
		{
			idx++;
			if (idx > size - 1)
				return false;
			if (Buffer[idx] != 'u')
				return false;
			idx++;
			if (idx > size - 1)
				return false;
			if (Buffer[idx] != 'l')
				return false;
			idx++;
			if (idx > size - 1)
				return false;
			if (Buffer[idx] != 'l')
				return false;
			return true;
		}
		return false;
	}

	inline bool Validator::CheckFromBuffer(char *buffer)
	{
		fileIsValid = true;
		actualH = ' ';
		errorLine = 1, actualChar = 0, keyLevels = 0, levels = 0, limit = 0, actualParam = 0, idx = -1;
		if (buffer != 0)
			Buffer = buffer;
		if (size == 0)
			size = strlen(Buffer) + 2;
		auto Log = [&]() {
			std::string val = "";
			switch (Buffer[idx])
			{
			case '\n':
				val = "Line end: \\n";
				break;
			case '\r':
				val = "\\r";
				break;
			case '\t':
				val = "Tabular: \\t";
				break;
			case EOF:
			case 0x03:
				val = "End of file";
				break;
			case '\0':
				val = "Null character";
				break;
			default:
				val = Buffer[idx];
				break;
			}
			errorPos = idx - actualChar;
			errorDescription = "Error at line " + std::to_string(errorLine) + ", in character -> '" + val + "' in position: " + std::to_string(errorPos) + '\n';
		};

		do
		{
			idx++;
			switch (Buffer[idx])
			{
			case '\"':
			{
				switch (actualH)
				{
				case ',':
				case '{':
				case '[':
				case ':':
					break;
				case '}':
				case ']':
				case '\"':
				{
					Log();
					errorDescription += "Expected -> ',' , ':' , '}' , ']'\n";
					expectedValues = {",", ":", "}", "]"};
					return fileIsValid = false;
				}
				default:
					break;
				}
				actualH = '\"';
				ParseString();
				break;
			}
			case '{':
			{
				if (arrayLevel.size() > 0)
					if (arrayLevel.back() >= 0)
						arrayLevel.back()++;
				lastOperation = jsonOperations::objOpen;
				switch (actualH)
				{
				case ',':
				case '{':
				case '[':
				case ':':
					break;
				case '}':
				case ']':
				case '\"':
				{
					Log();
					errorDescription += "Expected -> 'Definition' , ',' , '}' , ']'\n";
					expectedValues = {"Definition", ",", "}", "]"};
					return fileIsValid = false;
				}
				default:
					break;
				}
				actualH = '{';
				keyLevels++;
				break;
			}
			case '}':
			{
				if (arrayLevel.size() > 0)
					if (arrayLevel.back() >= 0)
						arrayLevel.back()--;
				if (lastOperation == jsonOperations::arrayOpen)
				{
					Log();
					errorDescription += "Expected -> ']'\n";
					expectedValues = {"]"};
					return fileIsValid = false;
				}
				lastOperation = jsonOperations::objClose;
				switch (actualH)
				{
				case '}':
				case ']':
				case '{':
				case '\"':
					break;
				case ':':
				case '[':
				case ',':
					Log();
					errorDescription += "Expected -> 'Definition' , 'Value' , '{' , '['\n";
					expectedValues = {"Definition", "Value", "{", "["};
					return fileIsValid = false;
				default:
					break;
				}
				actualH = '}';
				keyLevels--;
				break;
			}
			case '[':
			{
				arrayLevel.push_back(0);
				lastOperation = jsonOperations::arrayOpen;
				switch (actualH)
				{
				case ',':
				case '{':
				case '[':
				case ':':
					break;
				case '}':
				case ']':
				case '\"':
					Log();
					errorDescription += "Expected -> ',' , '}' , ']'\n";
					expectedValues = {",", "}", "]"};
					return fileIsValid = false;
				default:
					break;
				}
				actualH = '[';
				levels++;
				break;
			}
			case ']':
			{
				if (arrayLevel.size() > 0)
					arrayLevel.pop_back();
				if (lastOperation == jsonOperations::objOpen)
				{
					Log();
					errorDescription += "Expected-> '}' \n";
					expectedValues = {"]"};
					return fileIsValid = false;
				}
				lastOperation = jsonOperations::arrayClose;
				switch (actualH)
				{
				case '}':
				case ']':
				case '[':
				case '\"':
					break;
				case ':':
				case '{':
				case ',':
					Log();
					errorDescription += "Expected -> '[' , '{' , 'Value' , 'Definition'\n";
					expectedValues = {"[", "{", "Value", "Definition"};
					return fileIsValid = false;
				default:
					break;
				}
				actualH = ']';
				levels--;
				break;
			}
			case ',':
			{
				switch (actualH)
				{
				case ']':
				case '\"':
				case '}':
					break;
				case '[':
				case ':':
				case '{':
				case ',':
					Log();
					errorDescription += "Expected -> ']' , '[' , '{' , 'Value' , 'Definition'\n";
					expectedValues = {"[", "]", "{", "Value", "Definition"};
					return fileIsValid = false;
				default:
					break;
				}
				actualH = ',';
				break;
			}
			case ':':
			{
				if (arrayLevel.size() > 0)
					if (arrayLevel.back() == 0)
					{
						Log();
						errorDescription += "Illegal declaration, you cannot make a declaration into an array.\nExpected ',' , ']'\n";
						expectedValues = {",", "]"};
						return fileIsValid = false;
					}
				switch (actualH)
				{
				case ']':
				case '}':
				case '[':
				case ':':
				case '{':
				case ',':
					Log();
					errorDescription += "Expected -> 'Definition' , '[' , 'Value'\n";
					expectedValues = {"Definition", "[", "Value"};
					return fileIsValid = false;
				case '\"':
				default:
					break;
				}
				actualH = ':';
				break;
			}
			case 'f':
			case 't':
			{
				switch (actualH)
				{
				case '[':
				case ',':
				case ':':
					break;
				case '\"':
				case ']':
				case '}':
				case '{':
					Log();
					errorDescription += "Expected -> ',' , ':' , '}' , ']' , 'Definition'\n";
					expectedValues = {",", ":", "}", "]", "Definition"};
					return fileIsValid = false;
				default:
					break;
				}
				actualH = '\"';
				switch (ParseBoolean())
				{
				case 0:
					break;
				case 1:
					break;
				default:
					Log();
					errorDescription += "Expected -> 'true' , 'false'\n";
					expectedValues = {"true", "false"};
					return fileIsValid = false;
					break;
				}
				break;
			}
			case 'n':
			{
				switch (actualH)
				{
				case '[':
				case ',':
				case ':':
					break;
				case '\"':
				case ']':
				case '}':
				case '{':
					Log();
					errorDescription += "Expected -> ',' , ':' , '}' , ']' , 'Definition'\n";
					expectedValues = {",", ":", "}", "]", "Definition"};
					return fileIsValid = false;
				default:
					break;
				}
				actualH = '\"';
				if (!ParseNull())
				{
					Log();
					errorDescription += "Expected -> 'null' , ',' , ':' , '}' , ']'\n";
					expectedValues = {"null", ",", ":", "}", "]"};
					return fileIsValid = false;
				}
				break;
			}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
			{
				switch (actualH)
				{
				case '[':
				case ',':
				case ':':
					break;
				case '\"':
				case ']':
				case '}':
				case '{':
					Log();
					errorDescription += "Expected -> ',' , ':' , '}' , ']' , 'Definition'\n";
					expectedValues = {",", ":", "}", "]", "Definition"};
					return fileIsValid = false;
				default:
					break;
				}
				actualH = '\"';
				ParseNumber();
				if (!fileIsValid)
				{
					Log();
					errorDescription += "Expected -> ',' , '}' , ']' , 'Number'\n";
					expectedValues = {",", "}", "]", "Number"};
					return fileIsValid = false;
				}
				break;
			}
			case '\n':
				errorLine += 1;
				actualChar = idx;
				break;
			case '\r':
			case ' ':
			case '\t':
			case '\v':
			case '\b':
			case '\f':
			case '\a':
				break;
			case EOF:
			case '\0':
				idx = size;
				break;
			case '/':
				if (!parseComments())
				{
					Log();
					if (idx >= size)
					{
						errorDescription += "Comment has no end statement. Expected -> '*/' , '\\n'\n";
						expectedValues = {"*/", "\\n"};
					}
					else
					{
						errorDescription += "Invalid comment. Expected -> '/' , '*'\n";
						expectedValues = {"/", "*"};
					}
					return fileIsValid = false;
				}
				break;
			default:
				Log();
				errorDescription += "Invalid character\n";
				return fileIsValid = false;
			}

		} while (idx < size - 1);

		delete[] Buffer;
		if (buffer != 0)
			delete[] buffer;
		arrayLevel.clear();
		fileIsValid = levels == 0 && keyLevels == 0 ? true : false;
		if (keyLevels > 0)
		{
			Log();
			errorDescription += "Missing " + std::to_string(keyLevels) + " '}'\n";
			expectedValues = {"}"};
		}
		else if (keyLevels < 0)
		{
			Log();
			errorDescription += "Missing " + std::to_string(keyLevels) + " '{'\n";
			expectedValues = {"{"};
		}

		if (levels > 0)
		{
			Log();
			errorDescription += "Missing " + std::to_string(levels) + " ']'\n";
			expectedValues = {"]"};
		}
		else if (levels < 0)
		{
			Log();
			errorDescription += "Missing " + std::to_string(levels) + " '['\n";
			expectedValues = {"["};
		}
		return fileIsValid;
	}
}
#endif // !_JSON_VALIDATOR_
