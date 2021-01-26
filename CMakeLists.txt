cmake_minimum_required(VERSION 3.12...3.18)
project(pocolog_pybind)

# Find packages **************************************************************************************************
find_package(pybind11 REQUIRED)

# Include **************************************************************************************************
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${pybind11_INCLUDE_DIR})

# add_library(traversability perception-traversability/src/traversability.cpp)
# target_link_libraries(traversability PUBLIC ${OpenCV_LIBS})

pybind11_add_module(pocolog_pybind src/pocolog_pybind.cpp)
# target_link_libraries(traversability_pybind PUBLIC traversability ${OpenCV_LIBS})

# TRAVERSABILITY_PYBIND_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(pocolog_pybind PRIVATE VERSION_INFO=${POCOLOG_PYBIND_VERSION_INFO})
