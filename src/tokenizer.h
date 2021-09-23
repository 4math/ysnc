#pragma once

#include <map>
#include <string>

class Tokenizer {
public:
	class Token {
	public:
		Token(int position_, int id_) : position( position_ ), id( id_ ) {
			value = reverseTokenMap[id];
		}

		Token(int position_, const std::string& value_) : position( position_ ), value( value_ ) {
			id = tokenMap[value];
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

	const std::vector<Token>& result() {
		int pos = 0;
		Token t(pos++, "package");
		Token t1(pos++, "const");
		Token t2(pos++, 40);

		tokenVector = { t, t1, t2 };
		return tokenVector;
	}

	typedef std::map<std::string, int> tMap;
	typedef std::map<int, std::string> rtMap;

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
	{"abstract", 1},
	{"boolean", 2},
	{"byte", 3},
	{"break", 4},
	{"class", 5},
	{"case", 6},
	{"catch", 7},
	{"char", 8},
	{"continue", 9},
	{"default", 10},
	{"do", 11},
	{"double", 12},
	{"else", 13},
	{"extends", 14},
	{"final", 15},
	{"finally", 16},
	{"float", 17},
	{"for", 18},
	{"if", 19},
	{"implements", 20},
	{"import", 21},
	{"instanceof", 22},
	{"int", 23},
	{"interface", 24},
	{"long", 25},
	{"native", 26},
	{"new", 27},
	{"package", 28},
	{"private", 29},
	{"protected", 30},
	{"public", 31},
	{"return", 32},
	{"short", 33},
	{"static", 34},
	{"super", 35},
	{"switch", 36},
	{"synchronized", 37},
	{"this", 38},
	{"throw", 39},
	{"throws", 40},
	{"transient", 41},
	{"try", 42},
	{"void", 43},
	{"volatile", 44},
	{"while", 45},
	{"assert", 46},
	{"const", 47},
	{"enum", 48},
	{"goto", 49},
	{"strictfp", 50}
};

Tokenizer::rtMap Tokenizer::reverseTokenMap = Tokenizer::initReverseTokenMap();