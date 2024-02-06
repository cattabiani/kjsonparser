#pragma once

#include <string>
#include <variant>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>
#include <tuple>

using namespace std;

namespace K {

class UnrecognizedTokenError : public std::runtime_error {
public:
    explicit UnrecognizedTokenError(const string& token)
        : std::runtime_error("Unrecognized token: \'" + token + '\'') {}
};

class ExpectedDifferentTokenError : public std::runtime_error {
public:
    explicit ExpectedDifferentTokenError(const string& token, const std::string& expected)
        : std::runtime_error("Expected: \"" + expected + "\", found: \"" + token + '\"') {}
};

class KJsonParser {
public:
    struct dict_type;
    struct array_type;
    using value_type = variant<int, bool, double, string, dict_type, array_type>;
    struct dict_type : unordered_map<string, shared_ptr<value_type>> {};
    struct array_type : vector<shared_ptr<value_type>> {};
    using token_type = variant<int, bool, double, string, char>;

    enum class TokenType {
        END,
        OPEN_DICT,
        CLOSE_DICT,
        OPEN_ARRAY,
        CLOSE_ARRAY,
        STRING,
        DOUBLE,
        INT,
        BOOL_FALSE, 
        BOOL_TRUE, 
        COLON,
        COMMA
    };

    static value_type parse(const string& s);
    static tuple<K::KJsonParser::TokenType, K::KJsonParser::value_type, string> parse(stringstream& ss);
    static value_type parse_dict(stringstream& ss);
    static value_type parse_array(stringstream& ss);
    static TokenType get_token(stringstream& ss, string& token);
};
}
