cmake_minimum_required(VERSION 3.7)
project(Challenge)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads)
add_executable(Challenge main.cpp LockGuard.h)
target_link_libraries (Challenge Threads::Threads)