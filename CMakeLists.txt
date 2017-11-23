cmake_minimum_required(VERSION 3.5)

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
        src/image_loader.cpp
        src/renderer.cpp
        src/application.cpp
        src/stopwatch.cpp
        ext_src/tinyxml2/tinyxml2.cpp
        src/main.cpp
        src/sleep.cpp
        src/client.cpp src/client.h src/webclient.cpp src/webclient.h src/trainannouncementparser.cpp src/trainannouncementparser.h src/trainannouncement.h src/actionobject.cpp src/actionobject.h src/blockingqueue.cpp src/blockingqueue.h src/trainannouncementdownloader.cpp src/trainannouncementdownloader.h src/texture.cpp src/texture.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
