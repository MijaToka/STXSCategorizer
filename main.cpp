#include "interface/STXS_common.h"
#include "src/Preprocess.cpp"
#include "src/STXS_categorization0.cpp"
#include "src/STXS_categorization1p2.cpp"
#include "src/STXS_categorization1p3.cpp"
#include "src/STXS_script.cpp"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

std::string help_message() {
  std::stringstream ss;
  ss << "Usage:\n\tSTXS13-plots -f ROOT_FILE [-f ROOT_FILE]... -v VERSION -m "
        "MODE"
     << std::endl
     << std::endl
     << "Options:" << std::endl
     << "\t-f FILE\t\tintput file; may be given multiple times" << std::endl
     << "\t-v VERSION\tversion number: 2 or 3" << std::endl
     << "\t-m,--mode MODE\tproduction mode (ggH, qqH, ttH, VH) determines\n"
        "\t\t\tthe output directory"
     << std::endl
     << "\t-h, --help\tShow this text.";

  return ss.str();
};

void parseArguments(
    int argc, char *argv[], std::vector<std::string> &files, int &version,
    std::string &mode,
    std::function<std::map<STXS1, ROOT::RDF::RNode>(
        std::map<STXS0, ROOT::RDF::RNode>)> &second_categorization) {

  bool hasFiles(false), hasVersion(false), hasMode(false);

  if (argc < 2) {
    std::cerr << help_message();
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
      std::cout << help_message();
      exit(EXIT_SUCCESS);
    }

    if (std::string(argv[i]) == "-f") {
      std::cout << "Adding file: " << argv[i + 1] << std::endl;
      files.push_back(argv[++i]);
      hasFiles = true;
    }

    else if (std::string(argv[i]) == "-v")

      try {

        version = std::stoi(argv[++i]);
        if (!(version == 2 || version == 3))
          throw std::invalid_argument("must be 2 or 3");
        else if (version == 2)
          second_categorization = Version1_2::second_categorization;
        else if (version == 3)
          second_categorization = Version1_3::second_categorization;

        std::cout << "Running version: " << version << std::endl;
        hasVersion = true;

      } catch (const std::exception &e) {

        std::cerr << "Invalid -v value: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
      }
    else if (std::string(argv[i]) == "-m" || std::string(argv[i]) == "--mode") {

      mode = argv[++i];
      std::cout << "Running production mode " << mode << std::endl;
      hasMode = true;

    } else {
      std::cerr << "Error parsing arguments: Unknown flag " << argv[i]
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  // Check the obligatory variables are set
  if (!hasFiles) {
    std::cerr << "Must pass in a file." << std::endl;
    exit(EXIT_FAILURE);
  }
  if (!hasMode) {
    std::cerr << "Must pass in a mode." << std::endl;
    exit(EXIT_FAILURE);
  }
  if (!hasVersion) {
    std::cerr << "Must pass in a version." << std::endl;
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {

  ROOT::EnableImplicitMT();

  std::function<std::map<STXS1, ROOT::RDF::RNode>(
      std::map<STXS0, ROOT::RDF::RNode>)>
      second_categorization;
  std::vector<std::string> files;
  int version;
  std::string mode;

  std::stringstream output_dir;

  parseArguments(argc, argv, files, version, mode, second_categorization);

  output_dir << "output/" << mode;

  auto df = setup(files[0], mode); // TODO: add multi file support

  auto step0 = first_categorization(df);

  auto step1 = second_categorization(step0);

  snapshot(df, output_dir.str());
  snapshot(step0, output_dir.str());
  snapshot(step1, output_dir.str(), version);

  exit(EXIT_SUCCESS);
}
