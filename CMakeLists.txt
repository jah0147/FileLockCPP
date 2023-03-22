cmake_minimum_required(VERSION 3.22)
project(FileLockCPP)

set(CMAKE_CXX_STANDARD 20)
#for Exe to work
set(CMAKE_EXE_LINKER_FLAGS "-static")

#set .ico
set(SOURCE_FILES main.cpp resources.rc file_status.cpp file_status.h csv.cpp csv.h ini.cpp ini.h UI.cpp UI.h colorCodes.h)

add_executable(FileLockCPP ${SOURCE_FILES})
