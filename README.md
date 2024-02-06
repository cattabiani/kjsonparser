# Simple Json Parser

Parser of simple json files. Assumptions:

- all keys are strings
- base types: int, string, double, bool, umap, vector

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

- **src**: Place your C++ source and header files here.
- **build**: Output directory for build artifacts.

## Building the Project

To build the project, you can use CMake and Ninja:

```bash
mkdir build
cd build
cmake ..
make
```
