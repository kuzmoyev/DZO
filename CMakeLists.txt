# How to run Qt in CLion: https://stackoverflow.com/a/38971661/4254425
# Qt CMake example: https://github.com/pbek/QOwnNotes/blob/develop/src/CMakeLists.txt
cmake_minimum_required(VERSION 3.5)
project(gradient_painter)

set(CMAKE_CXX_STANDARD 14)

find_package(Qt5Widgets REQUIRED)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

add_executable(gradient_gui src/main.cpp)
target_link_libraries(gradient_gui Qt5::Widgets)
