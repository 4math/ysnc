#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

class File {
public:
	File(const std::filesystem::path& path) {
		Read(path);
	}

	const std::string& getData() {
		return data;
	}

private:
	void Read(const std::filesystem::path& path) {
		std::ifstream file(path);

		std::ostringstream sstr;
		sstr << file.rdbuf();
		data = sstr.str();
	}

	std::string data;
};