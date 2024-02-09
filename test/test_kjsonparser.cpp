#include "../include/kjsonparser.hpp"
#include "utils.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

const string s0 =
    " {\"bb\" : 3 \"c\" : [\"a\" , {\"oo\" : 12.3, \"c\":[6, 7.1]} ] }";

const string s1 = R"(
   
   {
  "ints": [1, 2, 3, 4, 5],
  "strings": ["apple", "banana", "orange", "grape", "kiwi"],
  "nested_dict": {
    "key1": {
      "int_value": 10,
      "bool_value": true,
      "double_value": 3.14,
      "string_value": "hello"
    },
    "key2": {
      "int_value": 20,
      "bool_value": false,
      "double_value": 2.718,
      "string_value": "world"
    }
  }
}
)";

const string s2 = R"(
   
   {
  "ints": [1, 2, 3, 4, 5],
  "strings": ["apple", "banana", "orange", "grape", "kiwi"],
  "nested_dict": {
    "key1": {
      "int_value": 10,
      "bool_value": true,
      "double_value": 3.14,
      "string_value": "hlelo"
    },
    "key2": {
      "int_value": 20,
      "bool_value": false,
      "double_value": 2.718,
      "string_value": "world"
    }
  }
}
)";

TEST(JsonParserTest, BasicParsing) {
    std::ostringstream oss;

    auto ans = K::KJsonParser::parse(s0);
    oss << ans;
    string expected =
        "{\"c\": [\"a\", {\"c\": [6, 7.1], \"oo\": 12.3}], \"bb\": 3}";
    ASSERT_EQ(oss.str(), expected);

    oss.str("");
    ans = K::KJsonParser::parse(s1);
    oss << ans;
    expected = "{\"nested_dict\": {\"key2\": {\"string_value\": \"world\", "
               "\"double_value\": 2.718, \"bool_value\": 0, \"int_value\": "
               "20}, \"key1\": {\"string_value\": \"hello\", \"double_value\": "
               "3.14, \"bool_value\": 1, \"int_value\": 10}}, \"strings\": "
               "[\"apple\", \"banana\", \"orange\", \"grape\", \"kiwi\"], "
               "\"ints\": [1, 2, 3, 4, 5]}";
    ASSERT_EQ(oss.str(), expected);
}

TEST(JsonParserTest, CompareParsings) {
    using dict_type = K::KJsonParser::dict_type;

    auto ans0 = K::KJsonParser::parse("{}");
    auto ans1 = K::KJsonParser::parse("{}");
    ASSERT_EQ(ans0, ans1);
    ans0 = K::KJsonParser::parse("{\"aa\":\"aba\"}");
    ans1 = K::KJsonParser::parse("{}");
    ASSERT_NE(ans0, ans1);
    ans0 = K::KJsonParser::parse("{\"aa\":\"aba\"}");
    ans1 = K::KJsonParser::parse("{\"aa\":\"aab\"}");
    ASSERT_NE(ans0, ans1);
    ans0 = K::KJsonParser::parse("{\"aa\":\"aba\"}");
    ans1 = K::KJsonParser::parse("{\"aa\":\"aba\"}");
    ASSERT_EQ(ans0, ans1);
    ans0 = K::KJsonParser::parse("{\"aa\":\"aba\"}");
    ans1 = K::KJsonParser::parse("{\"ab\":\"aba\"}");
    ASSERT_NE(ans0, ans1);
    ans0 = K::KJsonParser::parse("{\"aa\":[]}");
    ans1 = K::KJsonParser::parse("{\"aa\":[1]}");
    ASSERT_NE(ans0, ans1);
    ans0 = K::KJsonParser::parse("{\"aa\":[]}");
    ans1 = K::KJsonParser::parse("{\"aa\":[]}");
    ASSERT_EQ(ans0, ans1);
    ans0 = K::KJsonParser::parse("{\"aa\":[1, 2, 3]}");
    ans1 = K::KJsonParser::parse("{\"aa\":[ 1, 2, 3 ]}");
    ASSERT_EQ(ans0, ans1);
    ans0 = K::KJsonParser::parse("{\"aa\":[1]}");
    ans1 = K::KJsonParser::parse("{\"aa\":[ 1  ]}");
    ASSERT_EQ(ans0, ans1);
    ans0 = K::KJsonParser::parse("{\"aa\":[1, 3, 2 ]}");
    ans1 = K::KJsonParser::parse("{\"aa\":[ 1, 2, 3 ]}");
    ASSERT_NE(ans0, ans1);
    ans0 = K::KJsonParser::parse("{\"aa\":{\"ab\": \"aba\"}}");
    ans1 = K::KJsonParser::parse("{\"aa\":{\"ab\": \"aba\"}}");
    ASSERT_EQ(ans0, ans1);
    ans0 = K::KJsonParser::parse("{\"aa\":{\"ab\": \"aba\"}}");
    ans1 = K::KJsonParser::parse("{\"aa\":{\"ab\": \"aab\"}}");
    ASSERT_NE(ans0, ans1);
    ans0 = K::KJsonParser::parse(s0);
    ans1 = K::KJsonParser::parse(s1);
    ASSERT_NE(ans0, ans1);
    ans0 = K::KJsonParser::parse(s1);
    ans1 = K::KJsonParser::parse(s1);
    ASSERT_EQ(ans0, ans1);
    ans0 = K::KJsonParser::parse(s1);
    ans1 = K::KJsonParser::parse(s2);
    ASSERT_NE(ans0, ans1);
    ans1 = K::KJsonParser::parse("{\"aa\":\"a\\nb\"}");
    auto v = get<string>(*get<dict_type>(ans1).at("aa"));
    ASSERT_EQ(v, "a\nb");
}
