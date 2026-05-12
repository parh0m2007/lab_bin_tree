cmake_minimum_required(VERSION 3.16)
project(BSTQtLab15 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(BSTQtLab15
    main.cpp
    binarytree.h binarytree.cpp
    treewidget.h treewidget.cpp
    mainwindow.h mainwindow.cpp
)

target_link_libraries(BSTQtLab15 PRIVATE Qt5::Widgets)