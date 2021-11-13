#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <set>

class Tokenizer {
public:
	struct TokenDefinition {
	public:
		TokenDefinition() : id(0), expects{} {}
		TokenDefinition(int id_, std::set<std::string> expects_) : id(id_), expects(std::move(expects_)) {}

		int id;
		std::set<std::string> expects;

		struct Compare {
			bool operator() (const TokenDefinition& lhs, const TokenDefinition& rhs) const {
				return lhs.id < rhs.id;
			}
		};
	};

	class Token {
	public:
		Token(int position_, const TokenDefinition& tokenDefinition_) : position(position_), id(tokenDefinition_.id) {
			value = reverseTokenMap[tokenDefinition_];
		}

		// ?? move
		Token(long long position_, const std::string& value_) : position(position_), value(value_) {
			id = tokenMap[value].id;
		}

		int getPosition() const {
			return position;
		}

		int getId() const {
			return id;
		}

		const std::string& getValue() const {
			return value;
		}

	private:
		int position;
		int id;
		std::string value;
	};

	Tokenizer(const std::string& inputString_) : inputString(inputString_) {}

	std::vector<std::string> splitData(const std::string& data) {
		return { data };
	}

	const std::vector<Token>& result() {
		std::stringstream sstream(inputString);

		long long currentPos = 0;
		std::string data;
		TokenDefinition tokenDefinition;

		while (sstream >> data) {
			// Processing the keywords
			if (tokenMap.count(data) > 0) {
				tokenDefinition = { tokenMap[data] };

				Token token(currentPos, data);
				tokenVector.push_back(token);
				currentPos = sstream.tellg();

				continue;
			}

			auto dataVector = splitData(data);

			for (auto& data_ : dataVector) {
				if (tokenDefinition.expects.empty()) {
					if (tokenMap.count(data_) > 0) {
						tokenDefinition = { tokenMap[data_] };

						Token token(currentPos, data_);
						tokenVector.push_back(token);
						currentPos = sstream.tellg();

						continue;
					}
				}

				for (auto& expectedToken : tokenDefinition.expects) {
					if (expectedToken == "keyword") {
						if (tokenMap.count(data_) > 0) {
							tokenDefinition = { tokenMap[data_] };

							Token token(currentPos, data_);
							tokenVector.push_back(token);
							currentPos = sstream.tellg();
						}
						else {
							std::cerr << "expected keyword at " << currentPos << "\n";
						}

						continue;
					}

					if (expectedToken == "identifier") {
						if (tokenMap.count(data_) > 0) {
							std::cerr << "got keyword, but expected identifier at " << currentPos << "\n";
							continue;
						}

						Token token(currentPos, "identifier");
						tokenVector.push_back(token);
						currentPos = sstream.tellg();

						continue;
					}
				}
			}


			currentPos = sstream.tellg();
		}

		return tokenVector;
	}

	typedef std::map<std::string, TokenDefinition> tMap;
	typedef std::map<TokenDefinition, std::string, TokenDefinition::Compare> rtMap;

	static tMap tokenMap;
	static rtMap reverseTokenMap;

	static rtMap initReverseTokenMap() {
		rtMap map;

		for (auto & it : tokenMap)
			map[it.second] = it.first;

		return map;
	}

private:
	const std::string& inputString;
	std::vector<Token> tokenVector;
};

Tokenizer::tMap Tokenizer::tokenMap = {
	{"abstract", {1, {"keyword"}}},
	{"boolean", {2, {"keyword", "identifier"}}},
	{"byte", {3, {"keyword", "identifier"}}},
	{"break", {4, {"identifier", "semicolon"}}},
	{"class", {5, {"identifier"}}},
	{"case", {6, {}}},
	{"catch", {7, {}}},
	{"char", {8, {"identifier"}}},
	{"continue", {9, {"semicolon"}}},
	{"default", {10, {"colon"}}},
	{"do", {11, {"curlyOpen", "identifier"}}},
	{"double", {12, {"identifier"}}},
	{"else", {13, {"curlyOpen", "identifier"}}},
	{"extends", {14, {"identifier"}}},
	{"final", {15, {}}},
	{"finally", {16, {}}},
	{"float", {17, {"identifier"}}},
	{"for", {18, {"parenthesesOpen"}}},
	{"if", {19, {"parenthesesOpen"}}},
	{"implements", {20, {}}},
	{"import", {21, {}}},
	{"instanceof", {22, {}}},
	{"int", {23, {"identifier"}}},
	{"interface", {24, {}}},
	{"long", {25, {"identifier"}}},
	{"native", {26, {}}},
	{"new", {27, {"identifier"}}},
	{"package", {28, {"identifier"}}},
	{"private", {29, {"keyword"}}},
	{"protected", {30, {"keyword"}}},
	{"public", {31, {"keyword"}}},
	{"return", {32, {"identifier", "semicolon"}}},
	{"short", {33, {"identifier"}}},
	{"static", {34, {"keyword"}}},
	{"super", {35, {}}},
	{"switch", {36, {}}},
	{"synchronized", {37, {}}},
	{"this", {38, {}}},
	{"throw", {39, {}}},
	{"throws", {40, {}}},
	{"transient", {41, {}}},
	{"try", {42, {}}},
	{"void", {43, {"identifier"}}},
	{"volatile", {44, {"keyword"}}},
	{"while", {45, {"parenthesesOpen"}}},
	{"assert", {46, {}}},
	{"const", {47, {"keyword", "identifier"}}},
	{"enum", {48, {"identifier"}}},
	{"goto", {49, {"identifier"}}},
	{"strictfp", {50, {}}},
	{"identifier", {51, {}}}
};

Tokenizer::rtMap Tokenizer::reverseTokenMap = Tokenizer::initReverseTokenMap();