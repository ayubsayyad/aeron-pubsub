cmake_minimum_required(VERSION 3.10)
project(aeron_publisher)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

include_directories("aeron-1.9.4-Linux/include")
link_directories("aeron-1.9.4-Linux/lib/")

link_libraries("aeron_client")
add_executable(aeron_publisher publisher.cpp aeronpublisher.cpp aeronsubscriber.cpp aeronsubscriber.hpp)
add_executable(aeron_subscriber subscriber.cpp aeronsubscriber.cpp)