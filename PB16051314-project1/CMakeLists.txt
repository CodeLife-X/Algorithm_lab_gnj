cmake_minimum_required(VERSION 3.13)
project(PB16051314_project1)

set(CMAKE_CXX_STANDARD 14)

add_executable(PB16051314_project1 main.cpp )
add_executable(quick_sort ex1/source/quick_sort.cpp )
add_executable(heap_sort ex1/source/heap_sort.cpp )
add_executable(merge_sort ex1/source/merge_sort.cpp )
add_executable(count_sort ex1/source/count_sort.cpp )
add_executable(test ex1/source/test.cpp )

