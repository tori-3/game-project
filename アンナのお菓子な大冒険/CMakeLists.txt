cmake_minimum_required(VERSION 3.12)
project(Annas_Candy_Adventrue CXX)

if (NOT CMAKE_CONFIGURATION_TYPES AND 
    NOT CMAKE_NO_BUILD_TYPE AND
    NOT CMAKE_BUILD_TYPE AND
    CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    message(STATUS "[!] Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/App)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_executable(Annas_Candy_Adventrue
  Block.cpp
  Enemy.cpp
  IMotionField.cpp
  LastBoss.cpp
  Main.cpp
  Movements.cpp
  Player.cpp
  )

target_precompile_headers(Annas_Candy_Adventrue PRIVATE stdafx.h)

find_package(Siv3D)
target_link_libraries(Annas_Candy_Adventrue PUBLIC Siv3D::Siv3D)

target_compile_features(Annas_Candy_Adventrue PRIVATE cxx_std_20)

