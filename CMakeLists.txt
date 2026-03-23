cmake_minimum_required(VERSION 4.1.2)
project(STXS13-plots VERSION 0.0.1)

add_executable(${PROJECT_NAME}
  main.cpp
)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
)

add_library(Scripts
  src/STXS_script.cpp
  src/STXS_Filter.cpp
)

find_package(
  ROOT 6.38 CONFIG REQUIRED
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ${ROOT_LIBRARIES}
  PRIVATE
    Scripts
)

