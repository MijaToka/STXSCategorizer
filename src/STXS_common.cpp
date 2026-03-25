#include "../interface/STXS_common.h"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <ROOT/RVec.hxx>
#include <RtypesCore.h>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <vector>

STXS1 create_range(STXS1 prevSTXS1, const Category column, Float_t lower,
                   Float_t upper) {
  STXS1 newCategory = prevSTXS1;
  switch (column) {
  case Pt:
    newCategory.Pt = Range{{"ZZCand_pt", "bestCandIdx"}, lower, upper};
    break;
  case Mjj:
    newCategory.Mjj = Range{{"m_jj"}, lower, upper};
    break;
  case Hjj_pt:
    newCategory.Hjj_pt = Range{{"ZZjj_pt"}, lower, upper};
    break;
  case Hj_H_pt:
    newCategory.Hj_H_pt =
        Range{{"ZZj_pt", "ZZCand_pt", "bestCandIdx"}, lower, upper};
    break;
  case deltaPhi_jj:
    newCategory.deltaPhi_jj = Range{{"deltaPhi_jj"}, lower, upper};
    break;
  case nJets:
    break;
  }
  return newCategory;
}

std::set<STXS1> cut_ranges(STXS1 prevSTXS1, const Category column,
                           std::vector<Float_t> ranges) {
  std::set<STXS1> output;
  for (int i = 1; i < ranges.size(); i++) {
    auto newCategory =
        create_range(prevSTXS1, column, ranges[i - 1], ranges[i]);
    output.insert(newCategory);
  }
  return output;
};

std::string parseRange(const Range &r) {
  std::stringstream ss;
  ss << r.lower;
  if (r.upper != infty)
    ss << "To" << r.upper;
  else
    ss << "above";

  return ss.str();
};

std::string generate_STXS1_categoty_name(const STXS1 &s) {

  std::stringstream ss;
  ss << categoryNames.at(s.Category);
  if (s.nJets)
    ss << "_" << s.nJets->nJet << "jet";
  if (s.Pt)
    ss << "_Pt" << parseRange(*s.Pt);
  if (s.Mjj)
    ss << "_Mjj" << parseRange(*s.Mjj);
  if (s.Hjj_pt)
    ss << "_Hjj_pt" << parseRange(*s.Hjj_pt);
  if (s.Hj_H_pt)
    ss << "_Hj_H_pt" << parseRange(*s.Hj_H_pt);
  if (s.deltaPhi_jj)
    ss << "_deltaPhi_jj" << parseRange(*s.deltaPhi_jj);

  return ss.str();
};

void printCategory(STXS1 &s) {
  std::cout << "STXS1 {" << std::endl;
  std::cout << "\t" << generate_STXS1_categoty_name(s) << std::endl;
  std::cout << "\tCategory:\t" << categoryNames.at(s.Category) << std::endl;

  if (s.nJets)
    std::cout << "\tnJets:\t" << (s.nJets->above ? ">=" : "") << s.nJets->nJet
              << std::endl;
  if (s.Pt)
    std::cout << "\tPt:\t[" << s.Pt->lower << ", " << s.Pt->upper << "]"
              << std::endl;
  if (s.Mjj)
    std::cout << "\tMjj:\t[" << s.Mjj->lower << ", " << s.Mjj->upper << "]"
              << std::endl;
  if (s.Hjj_pt)
    std::cout << "\tHjj_pt:\t[" << s.Hjj_pt->lower << ", " << s.Hjj_pt->upper
              << "]" << std::endl;
  if (s.Hj_H_pt)
    std::cout << "\tHj_H_pt:\t[" << s.Hj_H_pt->lower << ", " << s.Hj_H_pt->upper
              << "]" << std::endl;
  if (s.deltaPhi_jj)
    std::cout << "\tdeltaPhi_jj:\t[" << s.deltaPhi_jj->lower << ", "
              << s.deltaPhi_jj->upper << "]" << std::endl;
  std::cout << "}" << std::endl;
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

void snapshot(std::map<STXS1, ROOT::RDF::RNode> df_map,
              std::string output_dir) {
  std::stringstream ss;
  ss << output_dir << "/second_categorization";
  for (auto const &[category, df] : df_map) {
    std::stringstream name_ss;
    name_ss << generate_STXS1_categoty_name(category) << ".root";
    snapshot(df, ss.str(), name_ss.str());
  }
};

ROOT::RDF::RNode applySTXS1(ROOT::RDF::RNode df, const STXS1 &category) {
  if (category.nJets)
    df =
        df.Filter([n = category.nJets->nJet, geq = category.nJets->above](
                      Char_t nJets) { return (geq) ? nJets >= n : nJets == n; },
                  {"nCleanedJetsPt30"});

  if (category.Pt)
    df = df.Filter(
        [lower = category.Pt->lower, upper = category.Pt->upper](
            ROOT::Internal::VecOps::RVec<Float_t> Pt, Short_t idx) {
          return Pt[idx] >= lower && Pt[idx] < upper;
        },
        category.Pt->columns);
  if (category.Mjj)
    df = df.Filter([lower = category.Mjj->lower, upper = category.Mjj->upper](
                       Double_t Mjj) { return Mjj >= lower && Mjj < upper; },
                   category.Mjj->columns);
  if (category.Hjj_pt)
    df = df.Filter(
        [lower = category.Hjj_pt->lower, upper = category.Hjj_pt->upper](
            Double_t Hjj_pt) { return Hjj_pt >= lower && Hjj_pt < upper; },
        category.Hjj_pt->columns);

  if (category.Hj_H_pt)
    df = df.Filter(
        [lower = category.Hj_H_pt->lower, upper = category.Hj_H_pt->upper](
            Double_t Hj_H_pt, ROOT::VecOps::RVec<Float_t> Pt, Short_t idx) {
          return Hj_H_pt / Pt[idx] >= lower && Hj_H_pt / Pt[idx] < upper;
        },
        category.Hj_H_pt->columns);

  if (category.deltaPhi_jj)
    df = df.Filter(
        [lower = category.deltaPhi_jj->lower,
         upper = category.deltaPhi_jj->upper](Float_t deltaPhi_jj) {
          return deltaPhi_jj >= lower && deltaPhi_jj < upper;
        },
        category.deltaPhi_jj->columns);

  return df;
};
