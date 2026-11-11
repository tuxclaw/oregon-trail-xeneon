cmake_minimum_required(VERSION 3.16)
project(oregon-trail-xeneon VERSION 1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

add_executable(oregon-trail-xeneon
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/GameState.cpp
    src/GameState.h
    src/TrailWidget.cpp
    src/TrailWidget.h
    src/HuntingMinigame.cpp
    src/HuntingMinigame.h
    src/StartDialog.cpp
    src/StartDialog.h
    src/HighScoreManager.cpp
    src/HighScoreManager.h
    src/SoundManager.cpp
    src/SoundManager.h
)

target_link_libraries(oregon-trail-xeneon PRIVATE Qt6::Widgets Qt6::Multimedia)

set_target_properties(oregon-trail-xeneon PROPERTIES
    WIN32_EXECUTABLE TRUE
)
