cmake_minimum_required(VERSION 3.14)
project(libadic VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for optimization and debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -march=native")

# Find required libraries
find_library(GMP_LIBRARY gmp REQUIRED)
find_library(MPFR_LIBRARY mpfr REQUIRED)

if(NOT GMP_LIBRARY)
    message(FATAL_ERROR "GMP library not found. Please install libgmp-dev")
endif()

if(NOT MPFR_LIBRARY)
    message(FATAL_ERROR "MPFR library not found. Please install libmpfr-dev")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(LIBADIC_SOURCES
    src/base/gmp_wrapper.cpp
    src/base/modular_arith.cpp
    src/fields/zp.cpp
    src/fields/qp.cpp
    src/fields/cyclotomic.cpp
    src/functions/padic_log.cpp
    src/functions/padic_gamma.cpp
    src/functions/l_functions.cpp
    src/functions/characters.cpp
    src/functions/bernoulli.cpp
)

# Create the library
add_library(adic STATIC ${LIBADIC_SOURCES})
target_link_libraries(adic ${GMP_LIBRARY} ${MPFR_LIBRARY})

# Enable testing
enable_testing()

# Test executables
add_executable(test_gmp_wrapper tests/test_gmp_wrapper.cpp)
target_link_libraries(test_gmp_wrapper adic ${GMP_LIBRARY} ${MPFR_LIBRARY})
add_test(NAME test_gmp_wrapper COMMAND test_gmp_wrapper)

add_executable(test_zp tests/test_zp.cpp)
target_link_libraries(test_zp adic ${GMP_LIBRARY} ${MPFR_LIBRARY})
add_test(NAME test_zp COMMAND test_zp)

add_executable(test_qp tests/test_qp.cpp)
target_link_libraries(test_qp adic ${GMP_LIBRARY} ${MPFR_LIBRARY})
add_test(NAME test_qp COMMAND test_qp)

add_executable(test_functions tests/test_functions.cpp)
target_link_libraries(test_functions adic ${GMP_LIBRARY} ${MPFR_LIBRARY})
add_test(NAME test_functions COMMAND test_functions)

# Milestone test
add_executable(milestone1_test tests/milestone1_test.cpp)
target_link_libraries(milestone1_test adic ${GMP_LIBRARY} ${MPFR_LIBRARY})
add_test(NAME milestone1_test COMMAND milestone1_test 7 60)

# Mathematical validation suite
add_executable(validate_mathematics validate_mathematics.cpp)
target_link_libraries(validate_mathematics adic ${GMP_LIBRARY} ${MPFR_LIBRARY})
add_test(NAME validate_mathematics COMMAND validate_mathematics)

# Interactive demo
add_executable(interactive_demo interactive_demo.cpp)
target_link_libraries(interactive_demo adic ${GMP_LIBRARY} ${MPFR_LIBRARY})

# Install rules
install(TARGETS adic DESTINATION lib)
install(DIRECTORY include/libadic DESTINATION include)