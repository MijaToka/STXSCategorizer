add_executable(${PROJECT_NAME}
  bin/main.cpp
)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
)

add_library(Scripts
  src/STXS_common.cpp
  src/STXS_categorization0.cpp
  src/STXS_categorization1p2.cpp
  src/STXS_categorization1p3.cpp
  src/parseArgs.cpp
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ${ROOT_LIBRARIES}
  PRIVATE
    Scripts
    Analysis
    STXS_CommonUtils
)

target_include_directories(Scripts
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
