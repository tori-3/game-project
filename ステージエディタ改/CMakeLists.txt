cmake_minimum_required(VERSION 3.12)
project(Stage_Editor_Kai CXX)

if (NOT CMAKE_CONFIGURATION_TYPES AND 
    NOT CMAKE_NO_BUILD_TYPE AND
    NOT CMAKE_BUILD_TYPE AND
    CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    message(STATUS "[!] Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/App)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_executable(Stage_Editor_Kai
  Main.cpp
  )

target_precompile_headers(Stage_Editor_Kai PRIVATE stdafx.h)

find_package(Siv3D)
target_link_libraries(Stage_Editor_Kai PUBLIC Siv3D::Siv3D)

target_compile_features(Stage_Editor_Kai PRIVATE cxx_std_20)
