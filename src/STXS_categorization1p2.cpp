#include "../interface/STXS_common.h"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <map>
#include <set>
#include <vector>

namespace Version1_2 {

std::set<STXS1> define_untagged_subcategories() {
  std::set<STXS1> untagged_categories;

  // ggH0jet categories
  STXS1 ggH0j =
      STXS1{.Category = STXS0::Untagged, .nJets = numberJets{.nJet = 0}};
  for (STXS1 pt_cut : cut_ranges(ggH0j, Pt, {0.f, 10.f, 200.})) {
    untagged_categories.insert(pt_cut);
  }

  // ggH1jet categories
  STXS1 ggH1j =
      STXS1{.Category = STXS0::Untagged, .nJets = numberJets{.nJet = 1}};
  for (STXS1 pt_cut : cut_ranges(ggH1j, Pt, {0.f, 60.f, 120.f, 200.f})) {
    untagged_categories.insert(pt_cut);
  }

  // ggH2j_mjj0to350
  STXS1 ggH2j =
      STXS1{.Category = STXS0::Untagged, .nJets = numberJets{.nJet = 2}};
  STXS1 ggH2jlowMjj = create_range(ggH2j, Mjj, 0, 350);
  for (STXS1 pt_cut : cut_ranges(ggH2jlowMjj, Pt, {0.f, 60.f, 120.f, 200.f})) {
    untagged_categories.insert(pt_cut);
  }

  // ggH2j_mjj350above
  STXS1 ggH2jhighMjj = create_range(ggH2j, Mjj, 350, infty);
  untagged_categories.insert(ggH2jhighMjj);

  // ggH_Pt220above
  STXS1 ggHhighPt = create_range(
      STXS1{
          .Category = STXS0::Untagged,
      },
      Pt, 200, infty);
  untagged_categories.insert(ggHhighPt);

  return untagged_categories;
}

std::set<STXS1> define_VBF_categories() {
  std::set<STXS1> VBF_categories;

  // No VBF0j
  // VBF_1jet is simply itself
  STXS1 VBF1j =
      STXS1{.Category = STXS0::VBF_1jet, .nJets = numberJets{.nJet = 1}};
  VBF_categories.insert(VBF1j);

  // VBF_2jet subcategories
  // Low m_jj
  STXS1 VBF2j = STXS1{.Category = STXS0::VBF_2jet,
                      .nJets = numberJets{.nJet = 2, .above = true}};
  STXS1 VBF2j_lowMjj = create_range(VBF2j, Mjj, 0, 350);
  VBF_categories.insert(VBF2j_lowMjj);

  // High m_jj
  STXS1 VBF2j_highMjj = create_range(VBF2j, Mjj, 350, infty);

  STXS1 VBF2j_highPt = create_range(VBF2j_highMjj, Pt, 200, infty);
  VBF_categories.insert(VBF2j_highPt);

  STXS1 VBF2j_lowPt = create_range(VBF2j_highMjj, Pt, 0, 200);
  for (STXS1 mjj_cut : cut_ranges(VBF2j_lowPt, Mjj, {350.f, 700.f, infty})) {
    auto Hjj_pt_split = cut_ranges(mjj_cut, Hjj_pt, {0.f, 25.f, infty});
    VBF_categories.insert(Hjj_pt_split.begin(), Hjj_pt_split.end());
  }
  return VBF_categories;
}

std::set<STXS1> defined_VH_had_subcategories() {
  std::set<STXS1> VH_had_categories;
  // VH_had is defined by having 2 or more jets
  // VH_had_2jet subcategories
  STXS1 VH_had = STXS1{.Category = STXS0::VH_hadronic};
  for (STXS1 mjj_cut : cut_ranges(VH_had, Mjj, {0.f, 60.f, 120.f, infty})) {
    VH_had_categories.insert(mjj_cut);
  }
  return VH_had_categories;
};

std::set<STXS1> define_VH_lep_subcategories() {
  std::set<STXS1> VH_lep_categories;

  STXS1 VH_lep = STXS1{.Category = STXS0::VH_leptonic};
  for (STXS1 pt_cut : cut_ranges(VH_lep, Pt, {0.f, 150.f, infty})) {
    VH_lep_categories.insert(pt_cut);
  }

  return VH_lep_categories;
};

std::set<STXS1> define_ttH_subcategories() {
  std::set<STXS1> ttH_categories;

  STXS1 ttH_had = STXS1{.Category = STXS0::ttH_hadronic};
  ttH_categories.insert(ttH_had);

  STXS1 ttH_lep = STXS1{.Category = STXS0::ttH_leptonic};
  ttH_categories.insert(ttH_lep);

  return ttH_categories;
}

std::set<STXS1> define_categories() {
  std::set<STXS1> all_categories;

  std::set<STXS1> STXS0_subactegories[] = {
      define_untagged_subcategories(), define_VBF_categories(),
      defined_VH_had_subcategories(), define_VH_lep_subcategories(),
      define_ttH_subcategories()};

  for (std::set<STXS1> categories : STXS0_subactegories) {
    all_categories.insert(categories.begin(), categories.end());
  }

  return all_categories;
}

std::map<STXS1, ROOT::RDF::RNode>
second_categorization(std::map<STXS0, ROOT::RDF::RNode> arr) {

  std::map<STXS1, ROOT::RDF::RNode> output;

  for (STXS1 category : define_categories()) {
    output.emplace(category, applySTXS1(arr.at(category.Category), category));
  }

  return output;
}

} // namespace Version1_2
