cmake_minimum_required(VERSION 3.16)
project(BSTQtLab15 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

qt_standard_project_setup()

qt_add_executable(BSTQtLab15
    main.cpp
    bstree.h bstree.cpp
    treewidget.h treewidget.cpp
    mainwindow.h mainwindow.cpp
    binarytree.cpp binarytree.h
)

target_link_libraries(BSTQtLab15 PRIVATE Qt6::Widgets)