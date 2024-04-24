cmake_minimum_required(VERSION 3.8)

project(Task_1_Server)

set(CMAKE_CXX_STANDARD 17)

 if(WIN32)
     add_executable(server main.cpp Info.cpp win/WINServer.cpp)
 elseif(UNIX)
    add_executable(server main.cpp Info.cpp lin/LINServer.cpp)
 endif()