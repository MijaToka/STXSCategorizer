cmake_minimum_required(VERSION 3.26.5)
project(STXS-Categorize VERSION 1.0.0)

option(BUILD_CATEGORIZE "Build Categorize SubModule" OFF)
option(BUILD_CATEGORIZEBDT "Build CategorizeBDT SubModule" OFF)

find_package(ROOT 6.36 CONFIG REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(BUILD_CATEGORIZE OR BUILD_CATEGORIZEBDT)
  add_subdirectory(CommonUtils)
endif()

if(BUILD_CATEGORIZE)
  add_subdirectory(Categorize)
endif()

if(BUILD_CATEGORIZEBDT)
  add_subdirectory(CategorizeBDT)
endif()
