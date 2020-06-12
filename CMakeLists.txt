cmake_minimum_required(VERSION 3.16)
project(Virtual_Memory)

set(CMAKE_CXX_STANDARD 14)

add_executable(Virtual_Memory Status.h Constants.h Page.cpp Page.h main.cpp Process.cpp Process.h ProccessControlBlock.h OS.cpp OS.h Replacement_algorithms.h Printer.cpp Printer.h)