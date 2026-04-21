add_executable(${PROJECT_NAME}-train 
  bin/train.cpp
)

add_executable(${PROJECT_NAME}-bdt 
  bin/application.cpp
)

install(TARGETS ${PROJECT_NAME}-train ${PROJECT_NAME}-bdt 
  RUNTIME DESTINATION bin
)

#[[
add_library(STXS_BDT_Train 
  
)

add_library(STXS_BDT_Application

)

add_library(STXS_BDT_Common

)
]]
target_link_libraries(${PROJECT_NAME}-train
  PUBLIC
    ${ROOT_LIBRARIES}
  PRIVATE
    STXS_CommonUtils
    # STXS_BDT_Common
    # STXS_BDT_Train
)

#[[
target_include_directories(${PROJECT_NAME}-train
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_include_directories(${PROJECT_NAME}-application
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
]]

target_link_libraries(${PROJECT_NAME}-bdt
  PUBLIC
    ${ROOT_LIBRARIES}
  #[[
  PRIVATE
    STXS_BDT_Common
    STXS_BDT_Application
  ]]
)


