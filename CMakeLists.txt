cmake_minimum_required(VERSION 3.16)
project(oregon-trail-xeneon VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(oregon-trail-xeneon
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/GameState.cpp
    src/GameState.h
    src/TrailWidget.cpp
    src/TrailWidget.h
)

target_link_libraries(oregon-trail-xeneon PRIVATE Qt6::Widgets)

# Recommended window size for Xeneon Edge
set_target_properties(oregon-trail-xeneon PROPERTIES
    WIN32_EXECUTABLE TRUE
)
