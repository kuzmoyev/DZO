# How to run Qt in CLion: https://stackoverflow.com/a/38971661/4254425
# Qt CMake example: https://github.com/pbek/QOwnNotes/blob/develop/src/CMakeLists.txt
cmake_minimum_required(VERSION 3.5)
project(gradient_painter)

enable_language(CUDA)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

find_package(CUDA REQUIRED)

find_package(Qt5Widgets REQUIRED)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#include_directories(forms)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(include)
add_subdirectory(thirdparty/amgcl)

add_library(utility SHARED
        lib/utility/utility.cpp
        lib/utility/gaussian_blur.cpp
        lib/utility/matrix_crs.cpp
        lib/utility/matrix_fixed4.cpp)
target_link_libraries(utility Qt5::Core)

add_library(amgcl_solver SHARED
        lib/amgcl_solver/amgcl_solver.cpp)
target_link_libraries(amgcl_solver amgcl utility)

add_library(gauss_seidel_solver SHARED
        lib/gauss_seidel_solver/solver.cpp)
target_link_libraries(gauss_seidel_solver utility)

cuda_add_library(cuda_gauss_seidel_solver SHARED
        lib/cuda_gauss_seidel_solver/solver.cpp
        lib/cuda_gauss_seidel_solver/cuda.cu)
target_include_directories(cuda_gauss_seidel_solver PRIVATE CUDA_INCLUDE_DIRS)
target_link_libraries(cuda_gauss_seidel_solver utility gauss_seidel_solver)
set_target_properties(cuda_gauss_seidel_solver PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

add_executable(gradient_gui
        forms/mainwindow.cpp
        forms/sidebar.cpp

        src/main.cpp

        src/shapes/line.cpp
        src/shapes/rectangle.cpp
        src/shapes/scribble.cpp
        src/shapes/shape.cpp

        src/canvas/enums.h
        src/canvas/canvas_model.cpp
        src/canvas/canvas_view.cpp
        src/canvas/multi_canvas_view.cpp

        src/gui/clickable_label.cpp
        )

target_link_libraries(gradient_gui
        Qt5::Widgets
        utility
        amgcl_solver
        gauss_seidel_solver
        cuda_gauss_seidel_solver)
target_include_directories(gradient_gui PRIVATE src)

add_executable(test_app src/test_app.cpp)
target_link_libraries(test_app Qt5::Widgets utility amgcl_solver gauss_seidel_solver)
