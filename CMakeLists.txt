cmake_minimum_required(VERSION 3.8)
project(Multithreaded_Work_Queue)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp WorkQueue.tpp WorkQueue.h)
add_executable(Multithreaded_Work_Queue ${SOURCE_FILES})


target_link_libraries(Multithreaded_Work_Queue -lpthread)
