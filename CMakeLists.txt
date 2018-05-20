cmake_minimum_required(VERSION 3.10)
project(aeron_publisher)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

include_directories("aeron-1.9.4-Linux/include")
link_directories("aeron-1.9.4-Linux/lib/")


link_libraries("aeron_client")

add_library(aeronpubsub  src/aeronpubsub/aeronpublisher.cpp src/aeronpubsub/aeronsubscriber.cpp)

include_directories("src/aeronpubsub")

add_executable(aeron_publisher src/examples/publisher.cpp )
add_executable(aeron_subscriber src/examples/subscriber.cpp )

target_link_libraries(aeron_publisher aeronpubsub)
target_link_libraries(aeron_subscriber aeronpubsub)