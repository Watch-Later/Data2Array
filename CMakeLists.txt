cmake_minimum_required(VERSION 2.4)
project(Data2Array)
subdirs(Source)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)


set(DO_TEST False)
if (DO_TEST)
   subdirs(Test)
endif()
