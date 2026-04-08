#include "STXS_categorization1p2.cpp"
#include "STXS_categorization1p3.cpp"
#include "STXS_common.h"
#include <algorithm>
#include <exception>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

std::string help_message() {
  std::stringstream ss;
  ss << "Usage:\n\tSTXS13-plots -f ROOT_FILE [-f ROOT_FILE]... -v VERSION -m "
        "MODE [-d SUB_DIR]"
     << std::endl
     << std::endl
     << "Options:" << std::endl
     << "\t-f FILE\t\tintput file; may be given multiple times" << std::endl
     << "\t-v VERSION\tversion number: 2 or 3" << std::endl
     << "\t-m,--mode MODE\tproduction mode (ggH, qqH, ttH, VH) determines\n"
        "\t\t\tthe output directory"
     << std::endl
     << "\t-d SUB_DIR\tdefines the subdirectory under output the files "
        "will\n\t\t\tbe generated"
     << std::endl
     << "\t--verbose\tenables the printing out of the input specification"
     << "\t-h, --help\tShow this text.";

  return ss.str();
};

enum class Flag { HELP, FILE, VERSION, MODE, VERBOSE, DIRECTORY, UNKNOWN };

static const std::map<std::string, Flag> flagMap = {
    {"-h", Flag::HELP},           {"--help", Flag::HELP},
    {"-f", Flag::FILE},           {"-v", Flag::VERSION},
    {"-m", Flag::MODE},           {"--mode", Flag::MODE},
    {"--verbose", Flag::VERBOSE}, {"-d", Flag::DIRECTORY}};

Flag getFlag(const std::string &arg) {
  auto keyvalPair = flagMap.find(arg);
  return (keyvalPair != flagMap.end() ? keyvalPair->second : Flag::UNKNOWN);
}

void parseArguments(
    int argc, char *argv[], std::vector<std::string> &files, int &version,
    std::string &mode, std::string &subfolder,
    std::function<std::map<STXS1, ROOT::RDF::RNode>(
        std::map<STXS0, ROOT::RDF::RNode>)> &second_categorization) {

  bool hasFiles(false), hasVersion(false), hasMode(false);

  std::vector<std::string> args(argv, argv + argc);
  bool verbose =
      (std::find(args.begin(), args.end(), "--verbose") != args.end());

  if (argc < 2) {
    std::cerr << help_message();
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < argc; i++) {
    Flag flag = getFlag(std::string(argv[i]));
    switch (flag) {
    case Flag::VERBOSE:
      break;
    case Flag::HELP:
      std::cout << help_message();
      exit(EXIT_SUCCESS);
      break;

    case Flag::FILE:
      if (verbose)
        std::cout << "Adding file: " << argv[i + 1] << std::endl;
      files.push_back(argv[++i]);
      hasFiles = true;
      break;
    case Flag::DIRECTORY:
      if (verbose)
        std::cout << "Output will be saved under output/" << argv[i + 1]
                  << std::endl;
      subfolder = std::string(argv[++i]);
      break;
    case Flag::VERSION:
      try {
        version = std::stoi(argv[++i]);
        if (!(version == 2 || version == 3))
          throw std::invalid_argument("must be 2 or 3");
        else if (version == 2)
          second_categorization = Version1_2::second_categorization;
        else if (version == 3)
          second_categorization = Version1_3::second_categorization;

        if (verbose)
          std::cout << "Running version: " << version << std::endl;
        hasVersion = true;

      } catch (const std::exception &e) {
        std::cerr << "Invalid -v value: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
      }
      break;
    case Flag::MODE:
      mode = argv[++i];
      if (verbose)
        std::cout << "Running production mode " << mode << std::endl;
      hasMode = true;
      break;
    case Flag::UNKNOWN:
      std::cerr << "Error parsing arguments: Unknown flag " << argv[i]
                << std::endl;
      exit(EXIT_FAILURE);
      break;
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
    if (verbose)
      std::cout << "Running only the preprocess and STXS0" << std::endl;
    version = -1;
  }
}
