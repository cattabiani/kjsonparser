#include <iostream>
#include <vector>
#include <unordered_map>

#include "utils.hpp"

using namespace std;

int main() {
    cout << "Hello World!" << endl;

    vector<pair<int, char>> a{{1, 'a'}, {2, 'b'},{3, 'c'} };
    unordered_map<int, vector<int>> b{{1, {1, 2}}, {2, {3, 4, 5}}};
    cout << a << endl;
    cout << b << endl;
    return 0;
}