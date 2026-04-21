#include "STXSCategorizer/Categorize/interface/STXS_common.h"
#include "STXSCategorizer/Categorize/src/STXS_categorization0.cpp"
#include "STXSCategorizer/Categorize/src/parseArgs.cpp"
#include "STXSCategorizer/CommonUtils/src/Preprocess.cpp"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

  ROOT::EnableImplicitMT();

  std::function<std::map<STXS1, ROOT::RDF::RNode>(
      std::map<STXS0, ROOT::RDF::RNode>)>
      second_categorization;

  std::vector<std::string> files;

  int version;

  std::string mode, output, subfolder;

  std::stringstream output_dir;

  parseArguments(argc, argv, files, version, mode, output, subfolder,
                 second_categorization);

  output_dir << output << "/";
  if (subfolder != "")
    output_dir << subfolder << "/";
  output_dir << mode;

  auto df = setup(files[0], mode); // TODO: add multi file support

  auto step0 = first_categorization(df);

  if (version != -1) {
    auto step1 = second_categorization(step0);
    snapshot(step1, output_dir.str(), version);
  }

  snapshot(df, output_dir.str());
  snapshot(step0, output_dir.str());

  exit(EXIT_SUCCESS);
}
