cmake_minimum_required(VERSION 3.12)
project(lora_test)

set(CMAKE_CXX_STANDARD 11)

add_executable(lora_test main.cpp main1.cpp cmake-build-debug/Data2File.cpp cmake-build-debug/Data2File.h ChirpGenerator.h)