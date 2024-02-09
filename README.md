# Simple Json Parser

Parser of simple json files. Assumptions:

- all keys are strings
- base types: int, string, double, bool, umap, vector
- nesting is possible

Example:

```
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
      "double_value": "not_double"
    }
  }
}
```

## Project Structure

- **include**: Hpp files.
- **src**: Cpp files.
- **lib**: Compiled lib.
- **build**: Output directory for build artifacts.

## Building the Project

To build the library and install in `lib`, you can use CMake and Ninja:

```bash
cmake -S . -B build -G Ninja && cmake --build build && cmake --install build
```

To build and run the tests:

```bash
cmake -S . -B build -G Ninja && cmake --build build && ./build/kjsonparser_test
```
