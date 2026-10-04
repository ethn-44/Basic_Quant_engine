cmake_minimum_required(VERSION 3.14)
project(QuantPricingEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(OpenMP REQUIRED)

include_directories(include .)

set(SOURCES
    main.cpp
    Calib_cost.cpp
    Nelder_Meadsolver.cpp
    Risk_calc.cpp
)

add_executable(quant_engine ${SOURCES})
target_link_libraries(quant_engine PRIVATE OpenMP::OpenMP_CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

set(ENGINE_SOURCES
    Calib_cost.cpp
    Nelder_Meadsolver.cpp
    Risk_calc.cpp
)

add_executable(unit_tests
    ${ENGINE_SOURCES}
    ${TEST_SOURCES}
)

target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(unit_tests PRIVATE gtest_main OpenMP::OpenMP_CXX)

if(MSVC)
    target_compile_options(unit_tests PRIVATE /W4)
else()
    target_compile_options(unit_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(GoogleTest)
gtest_discover_tests(unit_tests)