#include "interface/STXS_common.h"
#include "src/Preprocess.cpp"
#include "src/STXS_categorization0.cpp"
#include "src/STXS_categorization1p2.cpp"
#include "src/STXS_script.cpp"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  // STXS_script();

  if (argc < 2) {
    std::cerr << "Usage:  STXS13-plots <data_dirs>" << std::endl;
  }

  std::vector<std::string> files;
  for (int i = 1; i < argc; i++) {
    files.push_back(argv[i]);
  }

  auto df = setup(files);

  snapshot(df, "output");

  auto step0 = first_categorization(df);

  snapshot(step0, "output");

  auto step1 = second_categorization(step0);

  snapshot(step1, "output");

  return 0;
}
