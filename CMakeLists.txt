cmake_minimum_required(VERSION 2.4)
project(Data2Array)
subdirs(Source)


set(DO_TESTS True)
if (DO_TEST)
   subdirs(Test)
endif()