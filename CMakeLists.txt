cmake_minimum_required(VERSION 3.10)
project(pocolog_pybind)

# Find packages **************************************************************************************************
include(FindPkgConfig)
pkg_check_modules(POCOLOG_CPP "pocolog_cpp" REQUIRED)
include_directories(
    src ${POCOLOG_CPP_INCLUDE_DIRS}
)
link_directories(
    ${POCOLOG_CPP_LIBRARY_DIRS}
)
find_package(pybind11 REQUIRED)

# Include **************************************************************************************************
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${pybind11_INCLUDE_DIR})

pybind11_add_module(pocolog_pybind src/pocolog_pybind.cpp)
target_link_libraries(pocolog_pybind PUBLIC pocolog_cpp)

set_property(TARGET pocolog_pybind PROPERTY CXX_STANDARD 14)

# Install
install(
    TARGETS pocolog_pybind
    DESTINATION bin
)


# TRAVERSABILITY_PYBIND_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(pocolog_pybind PRIVATE VERSION_INFO=${POCOLOG_PYBIND_VERSION_INFO})
target_compile_features(pocolog_pybind PRIVATE cxx_std_14)
