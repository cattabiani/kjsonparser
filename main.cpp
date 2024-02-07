#include "kjsonparser.hpp"
#include "utils.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {

    string s = R"(
   
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

    string s1 =
        " {\"bb\" : 3 \"c\" : [\"a\" , {\"oo\" : 12.3, \"c\":[6, 7.1]} ] }";

    auto ans = K::KJsonParser::parse(s);

    return 0;
}