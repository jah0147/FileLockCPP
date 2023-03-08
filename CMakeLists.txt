cmake_minimum_required(VERSION 3.22)
project(FileLockCPP)

set(CMAKE_CXX_STANDARD 17)
#for Exe to work
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(FileLockCPP main.cpp file_status.cpp file_status.h csv.cpp csv.h ini.cpp ini.h UI.cpp UI.h)
