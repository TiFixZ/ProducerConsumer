cmake_minimum_required(VERSION 3.16)
project(ProducerConsumer LANGUAGES CXX)

find_package(Threads REQUIRED)
add_executable(ProducerConsumer main.cpp)
target_compile_features(ProducerConsumer PRIVATE cxx_std_17)
target_link_libraries(ProducerConsumer PRIVATE Threads::Threads)
