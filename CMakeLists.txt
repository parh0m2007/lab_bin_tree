cmake_minimum_required(VERSION 3.16)
project(BSTQtLab15 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Поддержка Qt5 и Qt6
find_package(Qt6 COMPONENTS Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.15 COMPONENTS Widgets REQUIRED)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

add_executable(BSTQtLab15
    main.cpp
    binarytree.h binarytree.cpp
    treewidget.h treewidget.cpp
    mainwindow.h mainwindow.cpp
)

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(BSTQtLab15 PRIVATE Qt6::Widgets)
else()
    target_link_libraries(BSTQtLab15 PRIVATE Qt5::Widgets)
endif()