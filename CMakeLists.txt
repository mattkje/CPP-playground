cmake_minimum_required(VERSION 3.31)
project(untitled)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt/lib/cmake")
find_package(Qt6 REQUIRED COMPONENTS Widgets)
add_executable(untitled Tetris.cpp)
target_link_libraries(untitled PRIVATE Qt6::Widgets)
