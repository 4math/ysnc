#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <algorithm>

static int freeTokenId = 100;

class Tokenizer {
public:
    class Token {
    public:
        Token(int position_, int tokenId_) : position(position_), id(tokenId_) {
            if (reverseTokenMap.count(tokenId_) > 0)
                value = reverseTokenMap[tokenId_];
        }

        Token(int position_, const std::string &value_) : position(position_), value(value_) {
            if (tokenMap.count(value_) > 0)
                id = tokenMap[value];
        }

        int getPosition() const {
            return position;
        }

        int getId() const {
            return id;
        }

        const std::string &getValue() const {
            return value;
        }

    private:
        int position = -1;
        int id = -1;
        std::string value = "invalid";
    };

    explicit Tokenizer(const std::vector<std::string> &inputStrings_) : inputStrings(inputStrings_) {}

    static std::vector<std::string> splitData(const std::string &data) {
        static const std::set<char> splitters = {
                '{', '}', '(', ')', '\"', '+', '-', '/', '*', '%', '=', '!',
                '<', '>', '?', ':', '&', '|', '^', '~', '[', ']', ',', '.', ';'
        };

        std::vector<std::string> result;
        std::string token;
        for (const auto &c : data) {
            if (splitters.count(c) > 0) {
                if (!token.empty())
                    result.push_back(token);
                result.push_back({c});
                token = "";
            } else {
                token += c;
            }
        }

        if (!token.empty())
            result.push_back(token);

        return result;
    }

    const std::vector<Token> &result() {
        if (!tokenVector.empty())
            return tokenVector;

        long long currentPos = 0;
        unsigned int lineNumber = 0;

        for (const auto &str : inputStrings) {
            ++lineNumber;
            std::string data;
            std::stringstream sstream(str);

            while (sstream >> data) {
                if (tokenMap.count(data) > 0) { // Processing the keywords
                    Token token(currentPos, data);
                    tokenVector.push_back(token);
                    tokenToLine.push_back(lineNumber);
                    currentPos = sstream.tellg();

                    continue;
                }

                auto dataVector = splitData(data);

                for (auto &data_ : dataVector) {
                    if (tokenMap.count(data_) > 0) {
                        Token token(currentPos, data_);
                        tokenVector.push_back(token);
                        tokenToLine.push_back(lineNumber);
                        currentPos = sstream.tellg();
                    } else { // The token is an indentifier
                        if (identifierMap.count(data_) > 0) { // Was found previously
                            Token token(currentPos, identifierMap[data_]);
                            tokenVector.push_back(token);
                            tokenToLine.push_back(lineNumber);
                        } else {
                            Token token(currentPos, freeTokenId);
                            identifierMap[data_] = freeTokenId++;
                            tokenVector.push_back(token);
                            tokenToLine.push_back(lineNumber);
                        }

                        currentPos = sstream.tellg();
                    }
                }

                currentPos = sstream.tellg();
            }
        }

        return tokenVector;
    }

    typedef std::map<std::string, int> tMap;
    typedef std::map<int, std::string> rtMap;

    static tMap identifierMap;
    static tMap tokenMap;
    static rtMap reverseTokenMap;

    static rtMap initReverseTokenMap() {
        rtMap map;

        for (auto &it : tokenMap)
            map[it.second] = it.first;

        return map;
    }

    const std::vector<unsigned int> &getTokenToLine() const {
        return tokenToLine;
    }

private:
    const std::vector<std::string> &inputStrings;
    std::vector<Token> tokenVector;
    std::vector<unsigned int> tokenToLine;
};

Tokenizer::tMap Tokenizer::identifierMap = {};

Tokenizer::tMap Tokenizer::tokenMap = {
        {"abstract",     1},
        {"boolean",      2},
        {"byte",         3},
        {"break",        4},
        {"class",        5},
        {"case",         6},
        {"catch",        7},
        {"char",         8},
        {"continue",     9},
        {"default",      10},
        {"do",           11},
        {"double",       12},
        {"else",         13},
        {"extends",      14},
        {"final",        15},
        {"finally",      16},
        {"float",        17},
        {"for",          18},
        {"if",           19},
        {"implements",   20},
        {"import",       21},
        {"instanceof",   22},
        {"int",          23},
        {"interface",    24},
        {"long",         25},
        {"native",       26},
        {"new",          27},
        {"package",      28},
        {"private",      29},
        {"protected",    30},
        {"public",       31},
        {"return",       32},
        {"short",        33},
        {"static",       34},
        {"super",        35},
        {"switch",       36},
        {"synchronized", 37},
        {"this",         38},
        {"throw",        39},
        {"throws",       40},
        {"transient",    41},
        {"try",          42},
        {"void",         43},
        {"volatile",     44},
        {"while",        45},
        {"assert",       46},
        {"const",        47},
        {"enum",         48},
        {"goto",         49},
        {"strictfp",     50},
        {"{",            51},
        {"}",            52},
        {"(",            53},
        {")",            54},
        {"\"",           55},
        {"+",            56},
        {"-",            57},
        {"/",            58},
        {"*",            59},
        {"%",            60},
        {"=",            61},
        {"!",            62},
        {"<",            63},
        {">",            64},
        {"?",            65},
        {":",            66},
        {"&",            67},
        {"|",            68},
        {"^",            69},
        {"~",            70},
        {"[",            71},
        {"]",            72},
        {",",            73},
        {".",            74},
        {";",            75},
        {"identifier",   90},
        {"literal",      91}
};

Tokenizer::rtMap Tokenizer::reverseTokenMap = Tokenizer::initReverseTokenMap();