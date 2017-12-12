cmake_minimum_required(VERSION 3.5)

link_libraries(pthread)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(PROJECT_NAME mytest)
project(${PROJECT_NAME})

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

set(INCLUDES_DIR
        src
        ext_src/stb
        ext_src/tinyxml2
        )
include_directories(${INCLUDES_DIR})

set(SOURCES
        src/file.cpp
        src/font.cpp
        src/image.cpp
        src/renderer.cpp
        src/application.cpp
        ext_src/tinyxml2/tinyxml2.cpp
        src/main.cpp
        src/client.cpp
        src/client.h
        src/webclient.cpp
        src/webclient.h
        src/timetableparser.cpp
        src/timetableparser.h
        src/departure.h
        src/timetabledownloader.cpp
        src/timetabledownloader.h
        src/texture.cpp
        src/texture.h
        src/backlight.cpp
        src/backlight.h
        src/stopwatch.cpp
        src/stopwatch.h src/backlighthandler.cpp src/backlighthandler.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
