#pragma once

#include "tokenizer.h"

class Analyzer {
public: 
	Analyzer(const std::string& source, const std::string& target) {
		Tokenizer tokenizer(source);
		const auto& tokenizedSource = tokenizer.result();
		for (auto& token : tokenizedSource)
			std::cout << token.getValue() << ":" << token.getId() << " ";

		std::cout << "\n";
	}

	float result() {
		return 0;
	}

private:

};