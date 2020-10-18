cmake_minimum_required(VERSION 2.4)
project(Data2Array)

option(Data2Array_TEST "Test array generation" OFF)


subdirs(Source)

set(CMAKE_MODULE_PATH ${Data2Array_SOURCE_DIR}/CMake)

if (Data2Array_TEST )
   subdirs(Test)
endif()
