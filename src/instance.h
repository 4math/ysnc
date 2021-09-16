#pragma once

#include <iostream>
#include <filesystem>
#include <vector>

#include "file.h"

class Instance {
public:
	int Run() {
		std::cin >> workspace;
		outputDir = workspace / "result";

		std::cout << "workspace: " << workspace << "\n";
		std::cout << "outputDir: " << outputDir << "\n";
		
		std::vector<File> sourceFiles;
		for (auto const& dirEntry : std::filesystem::directory_iterator{ workspace }) {
			if (dirEntry.is_regular_file()) {
				std::cout << dirEntry.path() << "\n";
				sourceFiles.push_back(File(dirEntry.path()));
			}
		}

		int result = ProcessData(sourceFiles);

		if (result != 0) {
			std::cerr << "Failed to process data\n";
			return result;
		}

		if (!std::filesystem::create_directory(outputDir)) {
			std::cerr << "Unable to create output directory\n";
			return 1;
		}

		return 0;
	}

private:
	std::filesystem::path workspace;
	std::filesystem::path outputDir;

	int ProcessData(const std::vector<File> data) {
		return 1;
	}

};