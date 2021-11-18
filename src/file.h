#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

class File {
public:
	explicit File(const std::filesystem::path& path) {
		read(path);
	}

	[[nodiscard]] const std::vector<std::string>& getData() const {
		return data;
	}

private:
	void read(const std::filesystem::path& path) {
		std::ifstream file(path);

        std::string line;
		while (std::getline(file, line)) {
		    data.push_back(line);
		}
	}

	std::vector<std::string> data;
};