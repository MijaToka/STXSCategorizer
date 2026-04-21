add_library(STXS_CommonUtils
  src/Preprocess.cpp
)

add_library(Analysis
  src/cConstants.cc 
  src/Discriminants.cc
)

target_include_directories(Analysis
  PUBLIC ${ZZANALYSIS_INCLUDE_DIR}
)
