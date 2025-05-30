cmake_minimum_required(VERSION 3.31)
project(TetrisClone)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt/lib/cmake")
set(MACOSX_BUNDLE_ICON_FILE appicon.icns)
set_source_files_properties(appicon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(TetrisClone MACOSX_BUNDLE Tetris.cpp appicon.icns
)
set_target_properties(TetrisClone PROPERTIES MACOSX_BUNDLE TRUE)
target_link_libraries(TetrisClone PRIVATE Qt6::Widgets)