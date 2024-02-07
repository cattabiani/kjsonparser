#include "kjsonparser.hpp"
#include "utils.hpp"

#include <sstream>
#include <iostream>


K::KJsonParser::value_type K::KJsonParser::parse(const string& s) {
    stringstream ss(s);
    return get<1>(KJsonParser::parse(ss));
}

void append_next_n_chars(stringstream& ss, string& token, int n) {
    char c;
    for (int i = 0; i < n && ss.get(c); ++i) {
        token += c;
    }
}




K::KJsonParser::TokenType K::KJsonParser::get_token(stringstream& ss, string& token) {
    ss >> std::ws;
    if (ss.eof())
        return TokenType::END;
    
    token.clear();
    
    char c;
    ss.get(c);

    token += c;
    switch (c) {
        case '{':
            return TokenType::OPEN_DICT;
        case '}':
            return TokenType::CLOSE_DICT;
        case '[':
            return TokenType::OPEN_ARRAY;
        case ']':
            return TokenType::CLOSE_ARRAY;
        case ':':
            return TokenType::COLON;
        case ',':
            return TokenType::COMMA;
        case '\"': {
            getline(ss, token, '\"');
            return TokenType::STRING;
        }
        case 'f': {
            append_next_n_chars(ss, token, 4);
            if (token != "false" || (!ss.eof() && string(":,\n ").find(ss.peek()) == string::npos)) {
                throw UnrecognizedTokenError(token);
            }
            return TokenType::BOOL_FALSE;
        }
        case 't': {
            append_next_n_chars(ss, token, 3);
            if (token != "true" || (!ss.eof() && string(":,\n ").find(ss.peek()) == string::npos)) {
                throw UnrecognizedTokenError(token);
            }
            return TokenType::BOOL_TRUE;
        }

        default: {}
    }

    int ndots = 0;
    c = ss.peek();
    while (isdigit(c) || c == '.') {
        token += c;
        ndots += (c == '.');
        ss.ignore(1);
        c = ss.peek();
    }

    if (ndots > 1)
        throw UnrecognizedTokenError(token);
    
    if (ndots == 1)
        return TokenType::DOUBLE;
    
    return TokenType::INT;
}

tuple<K::KJsonParser::TokenType, K::KJsonParser::value_type, string> K::KJsonParser::parse(stringstream& ss) {
    string token;
    auto tt = KJsonParser::get_token(ss, token);
    switch(tt) {
        case TokenType::END:
            return {tt, false, token};
        case TokenType::OPEN_DICT:
            return {tt, KJsonParser::parse_dict(ss), token};
        case TokenType::OPEN_ARRAY:
            return {tt, KJsonParser::parse_array(ss), token};
        case TokenType::INT:
            return {tt, stoi(token), token};
        case TokenType::DOUBLE:
            return {tt, stod(token), token};
        case TokenType::BOOL_FALSE:
            return {tt, false, token};
        case TokenType::BOOL_TRUE:
            return {tt, true, token};
        default:
            return {tt, token, token};
    }
}

K::KJsonParser::value_type K::KJsonParser::parse_dict(stringstream& ss) {
    dict_type ans;
    auto [tt, value, token] = KJsonParser::parse(ss);
    while (tt != TokenType::CLOSE_DICT) {
        if (tt == TokenType::COMMA) {
            tie(tt, value, token) = KJsonParser::parse(ss);
            continue;
        }


        string key;
        if (auto str_ptr = std::get_if<std::string>(&value)) {
            key = *str_ptr;
        } else {
            throw ExpectedDifferentTokenError(token, "string");
        }

        tie(tt, value, token) = KJsonParser::parse(ss);
        if (tt != TokenType::COLON)
            throw ExpectedDifferentTokenError(token, ":");
        ans.emplace(key, move(make_shared<value_type>(get<1>(KJsonParser::parse(ss)))));

        tie(tt, value, token) = KJsonParser::parse(ss);
    }

    return ans;
}


K::KJsonParser::value_type K::KJsonParser::parse_array(stringstream& ss) {
    array_type ans;
    auto [tt, value, token] = KJsonParser::parse(ss);
    while (tt != TokenType::CLOSE_ARRAY) {
        if (tt == TokenType::COMMA) {
            tie(tt, value, token) = KJsonParser::parse(ss);
            continue;
        }

        ans.emplace_back(move(make_shared<value_type>(value)));
        tie(tt, value, token) = KJsonParser::parse(ss);
    }

    return ans;
}

// Custom comparison function for value_type
bool K::KJsonParser::dict_type::operator==(const dict_type& rhs) const {

    // Compare sizes first
    if (size() != rhs.size()) {
        return false;
    }

    // Iterate through each key-value pair in d1
    for (const auto& [key, value0] : *this) {
        // Check if the key exists in d2
        const auto value1_ptr = rhs.find(key);
        if (value1_ptr == rhs.end()) {
            return false; // Key does not exist in d2
        }
        const auto value1 = value1_ptr->second;

        if (!value0 && !value1) {
            return true;
        }
        
        // If only one pointer is null, they are not equal
        if (!value0 || !value1) {
            return false;
        }

        // Compare the values recursively
        if (!((*value0) == (*value1))) {
            return false; // Values are not equal
        }
    }

    return true;
}

// Custom comparison function for array_type
bool K::KJsonParser::array_type::operator==(const array_type& rhs) const {
    // Compare sizes first
    if (size() != rhs.size()) {
        return false;
    }

    // Compare elements pairwise
    for (size_t i = 0; i < size(); ++i) {
        const auto value0 = (this->operator[](i));
        const auto value1 = rhs[i];

        if (!value0 && !value1) {
            return true;
        }
        
        // If only one pointer is null, they are not equal
        if (!value0 || !value1) {
            return false;
        }

        if (!((*value0) == (*value1))) {
            return false; // Elements are not equal
        }
    }
    return true;
}

