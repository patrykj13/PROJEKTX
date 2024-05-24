cmake_minimum_required(VERSION 3.15...3.27)
project(PROJEKTX)

add_subdirectory(pybind11)

pybind11_add_module(Signal src/main.cpp)

add_subdirectory(matplotplusplus)
add_subdirectory(AudioFile)

target_link_libraries (Signal PUBLIC AudioFile matplot)