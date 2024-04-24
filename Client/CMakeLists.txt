project(Task_1_Server)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    add_executable(client
    main.cpp AScreenshot.cpp
    win/Bitmap.cpp win/WINScreenshot.cpp win/WINConnectionManager.cpp)
elseif(UNIX)
    add_executable(client main.cpp AScreenshot.cpp
    lin/LINConnectionManager.cpp lin/LINScreenshot.cpp)

    find_package(X11 REQUIRED)
    target_link_libraries(client PRIVATE ${X11_LIBRARIES})
    target_include_directories(client PRIVATE ${X11_INCLUDE_DIR})
endif()
