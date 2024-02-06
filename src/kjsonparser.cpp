#include "kjsonparser.hpp"

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





















// KJsonParser::value_type KJsonParser::parse(stringstream& ss) {
//     ss >> std::ws;
//     char a = ss.peek();

//     switch (a) {
//         case '{': {
//             return KJsonParser::parse_dict(ss);
//         }
//         case '[': {
//             return KJsonParser::parse_array(ss);
//         }
//         default: {
//             return KJsonParser::parse_base(ss);
//         }
//     }
// }





// KJsonParser::value_type KJsonParser::parse_dict(stringstream& ss) {
//     // char c;
//     // ss.get(c);
//     // KJsonParser::array_type ans;
//     // while (c != '}') {
//     //     auto token = parse(ss);
//     //     // auto p = make_shared<KJsonParser::value_type>(token);
//     //     ans.emplace_back(move(make_shared<value_type>(token)));
//     //     ss >> std::ws;
//     //     ss.get(c);
//     // }

//     // return ans;
//     return {};
// }

// KJsonParser::value_type KJsonParser::parse_array(stringstream& ss) {
//     char c;
//     ss.get(c);
//     KJsonParser::array_type ans;
//     while (c != ']') {
//         auto token = parse(ss);
//         // auto p = make_shared<KJsonParser::value_type>(token);
//         ans.emplace_back(move(make_shared<value_type>(token)));
//         ss >> std::ws;
//         ss.get(c);
//     }

//     return ans;
// }


// KJsonParser::value_type KJsonParser::parse_base(stringstream& ss) {
//     char v = ss.peek();
//     string token;
//     if (v == '"') {
//         ss.ignore(1);
//         getline(ss, token, '"');
//         return token;
//     }

//     if (v == 'f') {
//         ss >> token;
//         if (token != "false") {
//             throw UnrecognizedTokenError(token);
//         }

//         return false;
//     }

//     if (v == 't') {
//         ss >> token;
//         if (token != "true") {
//             throw UnrecognizedTokenError(token);
//         }

//         return true;
//     }

//     return KJsonParser::parse_number(ss);
// }

// KJsonParser::value_type process(const string& ans, const int ndots) {
//     if (ans.empty()) {
//         throw UnrecognizedTokenError(ans);
//     }

//     if (ndots == 1) {
//         return stod(ans);
//     }

//     if (ndots == 0) {
//         return stoi(ans);
//     }

//     throw UnrecognizedTokenError(ans);
// }


