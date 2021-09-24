#pragma once

#include <map>
#include <string>
#include <sstream>
#include <set>

class Tokenizer {
public:
	struct TokenDefinition {
	public:
		TokenDefinition() : id(0), expects{} {}
		TokenDefinition(int id_, const std::set<std::string>& expects_) : id(id_), expects(expects_) {}

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
		Token(int position_, TokenDefinition tokenDefinition_) : position(position_), id(tokenDefinition_.id) {
			value = reverseTokenMap[tokenDefinition_];
		}

		Token(int position_, const std::string& value_) : position(position_), value(value_) {
			id = tokenMap[value].id;
		}

		int getId() const {
			return id;
		}

		const std::string& getValue() const {
			return value;
		}

		int getPosition() const {
			return position;
		}

	private:
		int position;
		int id;
		std::string value;
	};

	Tokenizer(const std::string& inputString_) : inputString(inputString_) {}

	const std::vector<Token>& result() {
		std::stringstream sstream(inputString);

		std::string data;
		while (sstream >> data) {
			if (tokenMap.count(data) > 0) {
				TokenDefinition td(tokenMap[data]);

				Token t(sstream.tellg(), data);
				tokenVector.push_back(t);
			}
		}

		return tokenVector;
	}

	typedef std::map<std::string, TokenDefinition> tMap;
	typedef std::map<TokenDefinition, std::string, TokenDefinition::Compare> rtMap;

	static tMap tokenMap;
	static rtMap reverseTokenMap;

	static rtMap initReverseTokenMap() {
		rtMap map;

		for (auto it = tokenMap.begin(); it != tokenMap.end(); it++)
			map[it->second] = it->first;

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
	{"do", {11, {}}},
	{"double", {12, {"identifier"}}},
	{"else", {13, {}}},
	{"extends", {14, {"identifier"}}},
	{"final", {15, {}}},
	{"finally", {16, {}}},
	{"float", {17, {"identifier"}}},
	{"for", {18, {}}},
	{"if", {19, {}}},
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
	{"volatile", {44, {}}},
	{"while", {45, {}}},
	{"assert", {46, {}}},
	{"const", {47, {}}},
	{"enum", {48, {"identifier"}}},
	{"goto", {49, {"identifier"}}},
	{"strictfp", {50, {}}}
};

Tokenizer::rtMap Tokenizer::reverseTokenMap = Tokenizer::initReverseTokenMap();