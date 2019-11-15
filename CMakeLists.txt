cmake_minimum_required(VERSION 3.5.1)
project(MemoryAllocation C)

set(CMAKE_C_STANDARD 99)

add_executable(MemoryAllocation memorytests.c mymem.h mymem.c testrunner.h testrunner.c)