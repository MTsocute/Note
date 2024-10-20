cmake_minimum_required(VERSION 3.30)
project(QT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.7.2_1/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(QT main.cpp
        Forms/mainwindow.cpp
        header/mainwindow.h
        source/mainwindow.ui)
target_link_libraries(QT
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

