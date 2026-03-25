#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <map>

enum class STXS0 {
  VBF_2jet,
  VH_hadronic,
  VH_leptonic,
  ttH_hadronic,
  ttH_leptonic,
  VBF_1jet,
  Untagged
};

static const STXS0 STXS0_categories[] = {
    STXS0::VBF_2jet,     STXS0::VH_hadronic,  STXS0::VH_leptonic,
    STXS0::ttH_hadronic, STXS0::ttH_leptonic, STXS0::VBF_1jet,
    STXS0::Untagged};

enum Category { Pt, Mjj, Hjj_pt, Hj_H_pt, deltaPhi_jj, nJets };

struct Range {
  std::vector<std::string> columns;
  float lower;
  float upper;
};

struct numberJets {
  int nJet;
  bool above;
};

struct STXS1 {
  STXS0 Category;
  std::optional<Range> Pt, Mjj, Hjj_pt, Hj_H_pt, deltaPhi_jj;
  std::optional<numberJets> nJets;
};

void snapshot(ROOT::RDF::RNode df, std::string output_dir,
              std::string file_name);

void snapshot(ROOT::RDF::RNode df, std::string output_dir);

void snapshot(std::map<STXS0, ROOT::RDF::RNode> df_map, std::string output_dir);
