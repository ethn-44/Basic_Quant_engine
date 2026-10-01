cmake_minimum_required(VERSION 3.14)
project(QuantPricingEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

include_directories(include)

# Definir explicitement les fichiers .cpp du projet
set(SOURCES
    main.cpp
    Calib_cost.cpp
    Nelder_Meadsolver.cpp
)

add_executable(quant_engine ${SOURCES})

target_link_libraries(quant_engine PRIVATE OpenMP::OpenMP_CXX)

if(MSVC)
    target_compile_options(quant_engine PRIVATE /W4)
else()
    target_compile_options(quant_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()