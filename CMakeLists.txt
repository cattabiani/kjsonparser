cmake_minimum_required(VERSION 3.10)
project(kjsonparser)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Globbing source files
file(GLOB SOURCES src/*.cpp)
file(GLOB TESTS src/*.cpp)

# Create the library from the source files
add_library(kjsonparser STATIC ${SOURCES})

# Specify include directories for library users
target_include_directories(kjsonparser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# # Create an executable for testing
add_executable(kjsonparser_test ${TESTS})

# Link the testing executable with the library and Google Test libraries
target_link_libraries(kjsonparser_test PRIVATE kjsonparser ${GTEST_BOTH_LIBRARIES} pthread)

# Register tests
gtest_discover_tests(kjsonparser_test)

# cmake -S . -B build -G Ninja














# cmake_minimum_required(VERSION 3.10)
# project(kjsonparser)
# set(CMAKE_CXX_STANDARD 17)

# set(SOURCE_DIR src)
# file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

# add_executable(kjsonparser ${SOURCES})
# set(CMAKE_GENERATOR "Ninja")
