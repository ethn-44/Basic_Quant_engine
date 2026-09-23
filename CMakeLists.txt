cmake_minimum_required(VERSION 3.14)
project(QuantPricingEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenMP REQUIRED)
target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(quant_engine main.cpp ${SOURCES})


if(MSVC)
    target_compile_options(quant_engine PRIVATE /W4)
else()
    target_compile_options(quant_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()