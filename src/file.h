#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

class File {
public:
	explicit File(const std::filesystem::path& path) {
		read(path);
	}

	[[nodiscard]] std::vector<std::string>& getData() {
		return data;
	}

private:
	void read(const std::filesystem::path& path) {
		std::ifstream input(path);
        std::stringstream buffer;
        buffer << input.rdbuf();
        std::string rawString = buffer.str();

        auto result = clearComments(rawString);

		splitToDataVector(result);
	}

	void splitToDataVector(std::string& str) {
	    std::string delimiter = "\n";

	    size_t pos;
	    std::string token;
	    while ((pos = str.find(delimiter)) != std::string::npos) {
	        token = str.substr(0, pos);
	        data.push_back(token);
	        str.erase(0, pos + delimiter.length());
	    }
	    // push last line
	    data.push_back(str);
	}

	static std::string clearComments(std::string& rawString) {
	    std::stringstream result;

	    bool skipOneLine = false;
	    bool skipMultiLine = false;
	    bool pushToBuf = true;
	    for (int i = 0; i < rawString.size() - 1; ++i) {
	        if (rawString[i] == '/' && rawString[i + 1] == '/' && !skipMultiLine) {
	            skipOneLine = true;
	            pushToBuf = false;
	        }

	        if (skipOneLine && rawString[i] == '\n') {
	            skipOneLine = false;
	            pushToBuf = true;
	        }

	        if (rawString[i] == '/' && rawString[i + 1] == '*') {
	            pushToBuf = false;
	            skipMultiLine = true;
	        }

	        if (rawString[i] == '*' && rawString[i + 1] == '/') {
	            pushToBuf = true;
	            skipMultiLine = false;
	            // skip end comment symbols
	            i += 2;
	            // check for file end
	            if (i >= rawString.size()) break;
	        }

	        if (pushToBuf) {
	            result << rawString[i];
	        }
	    }

	    return result.str();
	}


	std::vector<std::string> data;
};