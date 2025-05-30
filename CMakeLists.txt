cmake_minimum_required(VERSION 3.31)
project(StackTheThings)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt/lib/cmake")
set(MACOSX_BUNDLE_ICON_FILE appicon.icns)
set_source_files_properties(appicon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(StackTheThings MACOSX_BUNDLE StackTheThings.cpp appicon.icns)
set_target_properties(StackTheThings PROPERTIES MACOSX_BUNDLE TRUE)
target_link_libraries(StackTheThings PRIVATE Qt6::Widgets)