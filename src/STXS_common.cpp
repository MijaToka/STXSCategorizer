#include "../interface/STXS_common.h"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <RtypesCore.h>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <vector>

std::set<STXS1> cut_ranges(STXS1 prevSTXS1, const Category column,
                           std::vector<Float_t> ranges) {
  std::set<STXS1> output;
  for (int i = 1; i < ranges.size(); i++) {
    STXS1 newCategory = prevSTXS1;
    switch (column) {
    case Pt:
      newCategory.Pt =
          Range{{"ZZCand_pt", "bestCandIdx"}, ranges[i - 1], ranges[i]};
      break;
    case Mjj:
      newCategory.Mjj = Range{{"m_jj"}, ranges[i - 1], ranges[i]};
      break;
    case Hjj_pt:
      newCategory.Hjj_pt = Range{{"ZZjj_pt"}, ranges[i - 1], ranges[i]};
      break;
    case Hj_H_pt:
      newCategory.Hj_H_pt = Range{
          {"ZZj_pt", "ZZCand_pt", "bestCandIdx"}, ranges[i - 1], ranges[i]};
      break;
    case deltaPhi_jj:
      newCategory.deltaPhi_jj =
          Range{{"deltaPhi_jj"}, ranges[i - 1], ranges[i]};
      break;
    case nJets:
      break;
    }
    output.insert(newCategory);
  }
  return output;
};

void snapshot(ROOT::RDF::RNode df, std::string output_dir,
              std::string file_name) {
  std::vector<std::string> column_to_save = {
      // ZZ candidate features
      "ZZCand_pt",
      "ZZCand_eta",
      "ZZCand_phi",
      "ZZCand_mass",
      "ZZCand_costheta1",
      "ZZCand_costheta2",
      "ZZCand_costhetastar",
      "ZZCand_Phi1",
      "ZZCand_nExtraLep",
      "ZZjj_pt",
      // MET
      "PFMET_pt",
      // Jet features
      "JetLeading_pt",
      "JetLeading_eta",
      "JetLeading_mass",
      "JetLeading_phi",
      "JetSubleading_pt",
      "JetSubleading_eta",
      "JetSubleading_mass",
      "JetSubleading_phi",
      "nCleanedJetsPt30",
      "nBtagged_filtered",
      "JetLeading_btag",
      "JetSubleading_btag",
      // Dijet features
      "deltaEta_jj",
      "deltaPhi_jj",
      "m_jj",
      // Lepton features (primary 4 leptons)
      "LepPt_0",
      "LepPt_1",
      "LepPt_2",
      "LepPt_3",
      "LepEta_0",
      "LepEta_1",
      "LepEta_2",
      "LepEta_3",
      "LepPhi_0",
      "LepPhi_1",
      "LepPhi_2",
      "LepPhi_3",
      "LepPdgId_0",
      "LepPdgId_1",
      "LepPdgId_2",
      "LepPdgId_3",
      // Extra lepton features
      "LepPt_4",
      "LepPt_5",
      "LepEta_4",
      "LepEta_5",
      "LepPhi_4",
      "LepPhi_5",
      "LepPdgId_4",
      "LepPdgId_5",
      // Discriminants
      "DVBF2j_ME",
      "DVBF1j_ME",
      "DWHh_ME",
      "DZHh_ME",
      "DVBF2j_ME_noC",
      "DVBF1j_ME_noC",
      "DWHh_ME_noC",
      "DZHh_ME_noC",
      "ZZCand_KD",
      // Event weights and label
      /*
        "EventWeight_lumi18", "EventWeight_lumi9", "EventWeight_lumi138",
        "production_mode", "HTXS_stage_0", "overallEventWeight", "genWeight",
        "puWeight", "trainWeight", "HTXS_stage1_2_cat_pTjet30GeV",
        "HTXS_stage1_2_cat_pTjet30GeV_label", "genEventSumw"
      */
  };

  std::filesystem::create_directories(output_dir);
  std::stringstream ss;
  ss << output_dir << "/" << file_name;
  df.Snapshot("Events", ss.str(), column_to_save);
}

void snapshot(ROOT::RDF::RNode df, std::string output_dir) {
  snapshot(df, output_dir, "snapshot.root");
};

void snapshot(std::map<STXS0, ROOT::RDF::RNode> df_map,
              std::string output_dir) {
  std::map<STXS0, std::string> STXS0_names = {
      {STXS0::VBF_2jet, "VBF_2jet.root"},
      {STXS0::VH_hadronic, "VH_hadronic.root"},
      {STXS0::VH_leptonic, "VH_leptonic.root"},
      {STXS0::ttH_hadronic, "ttH_hadronic.root"},
      {STXS0::ttH_leptonic, "ttH_leptonic.root"},
      {STXS0::VBF_1jet, "VBF_1jet.root"},
      {STXS0::Untagged, "Untagged.root"}};

  std::stringstream ss;
  ss << output_dir << "/first_categorization";
  for (const STXS0 category : STXS0_categories) {
    snapshot(df_map.at(category), ss.str(), STXS0_names.at(category));
  }
};
