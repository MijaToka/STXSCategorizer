#include "../interface/STXS_common.h"
#include <Math/Math.h>
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <map>
#include <set>

namespace Version1_3 {

static const float pi = ROOT::Math::Pi();

std::set<STXS1> define_untagged_subcategories() {
  std::set<STXS1> untagged_categories;

  // ggH0j
  STXS1 ggH0j =
      STXS1{.Category = STXS0::Untagged, .nJets = numberJets{.nJet = 0}};
  for (STXS1 pt_cut : cut_ranges(ggH0j, Pt, {0, 5, 10, 15, 20, 25, 30, 200})) {
    untagged_categories.insert(pt_cut);
  }

  // ggH1j

  STXS1 ggH1j =
      STXS1{.Category = STXS0::Untagged, .nJets = numberJets{.nJet = 1}};
  for (STXS1 pt_cut : cut_ranges(ggH1j, Pt, {0, 30, 60, 120, 200})) {
    untagged_categories.insert(pt_cut);
  }

  // ggH2j
  STXS1 ggH2j = STXS1{.Category = STXS0::Untagged,
                      .nJets = numberJets{.nJet = 2, .above = true}};
  ////ggH2j_lowMjj
  STXS1 ggH2j_lowMjj = create_range(ggH2j, Mjj, 0, 350);
  for (STXS1 pt_cut : cut_ranges(ggH2j_lowMjj, Pt, {0, 30, 60, 120, 200})) {
    for (STXS1 Hjj_pt_cut : cut_ranges(pt_cut, Hjj_pt, {0, 25, infty})) {
      untagged_categories.insert(Hjj_pt_cut);
    }
  }
  ////ggH2j_highMjj
  STXS1 ggH2j_highMjj = create_range(ggH2j, Mjj, 350, infty);
  for (STXS1 mjj_cut :
       cut_ranges(ggH2j_highMjj, Mjj, {350, 700, 1000, 1500, infty})) {
    for (STXS1 Hjj_pt_cut : cut_ranges(mjj_cut, Hjj_pt, {0, 25, infty})) {
      for (STXS1 phi_cut :
           cut_ranges(Hjj_pt_cut, deltaPhi_jj, {-pi, -pi / 2, 0, pi / 2, pi})) {
        untagged_categories.insert(phi_cut);
      }
    }
  }

  // ggHhighPt
  std::set<STXS1> ggHhighPt = cut_ranges(STXS1{.Category = STXS0::Untagged}, Pt,
                                         {200, 300, 450, 650, 1000, infty});
  untagged_categories.insert(ggHhighPt.begin(), ggHhighPt.end());

  return untagged_categories;
}

std::set<STXS1> define_EW_1jet(const STXS0 category) {
  std::set<STXS1> EW1j_categories;

  STXS1 EW1j = STXS1{.Category = category, .nJets = numberJets{.nJet = 1}};
  for (STXS1 pt_cut : cut_ranges(EW1j, Pt, {0, 200, 450, 650, infty})) {
    EW1j_categories.insert(pt_cut);
  }

  return EW1j_categories;
};

std::set<STXS1> define_EW_2jet(const STXS0 category) {
  std::set<STXS1> EW2j_categories;

  STXS1 EW2j = STXS1{.Category = category,
                     .nJets = numberJets{.nJet = 2, .above = true}};
  STXS1 EW2j_lowMjj = create_range(EW2j, Mjj, 0, 350);
  for (STXS1 pt_cut : cut_ranges(EW2j_lowMjj, Pt, {0, 200, infty})) {
    for (STXS1 mjj_cut : cut_ranges(pt_cut, Mjj, {0, 60, 120, 350})) {
      for (STXS1 Hjj_pt_cut : cut_ranges(mjj_cut, Hjj_pt, {0, 25, infty})) {
        EW2j_categories.insert(Hjj_pt_cut);
      }
    }
  }

  STXS1 EW2j_highMjj = create_range(EW2j, Mjj, 350, infty);
  for (STXS1 pt_cut : cut_ranges(EW2j_highMjj, Pt, {0., 200., 450.})) {
    for (STXS1 mjj_cut :
         cut_ranges(pt_cut, Mjj, {350, 700, 1000, 1500, infty})) {
      for (STXS1 Hjj_pt_cut : cut_ranges(mjj_cut, Hjj_pt, {0, 25, infty})) {
        for (STXS1 phi_cut : cut_ranges(Hjj_pt_cut, deltaPhi_jj,
                                        {-pi, -pi / 2, 0, pi / 2, pi})) {
          EW2j_categories.insert(phi_cut);
        }
      }
    }
  }

  STXS1 EW2j_highPt = create_range(EW2j_highMjj, Pt, 450, infty);
  for (STXS1 mjj_cut :
       cut_ranges(EW2j_highPt, Mjj, {350, 700, 1000, 1500, infty})) {
    EW2j_categories.insert(mjj_cut);
  }

  return EW2j_categories;
};

std::set<STXS1> define_VBF_subcategories() {
  std::set<STXS1> VBF_categories;

  // No VBF0j
  // VBF_1jet
  for (STXS1 category : define_EW_1jet(STXS0::VBF_1jet))
    VBF_categories.insert(category);

  // VBF_2jet
  for (STXS1 category : define_EW_2jet(STXS0::VBF_2jet))
    VBF_categories.insert(category);

  return VBF_categories;
};

std::set<STXS1> define_VH_had_subcategories() {
  std::set<STXS1> VH_had_categories;

  // VH_0jet
  STXS1 VH0j =
      STXS1{.Category = STXS0::VH_hadronic, .nJets = numberJets{.nJet = 0}};
  VH_had_categories.insert(VH0j);

  // VH_1jet
  for (STXS1 category : define_EW_1jet(STXS0::VH_hadronic))
    VH_had_categories.insert(category);

  // VH_2jet
  for (STXS1 category : define_EW_2jet(STXS0::VH_hadronic))
    VH_had_categories.insert(category);

  return VH_had_categories;
};

std::set<STXS1> define_VH_lep_subcategories() {
  std::set<STXS1> VH_lep_categories;

  for (int i = 0; i <= 2; i++) {
    STXS1 VHXj = {.Category = STXS0::VH_leptonic,
                  .nJets = numberJets{.nJet = i, .above = i == 2}};
    for (STXS1 pt_cut :
         cut_ranges(VHXj, Pt, {0, 75, 150, 250, 400, 600, infty}))
      VH_lep_categories.insert(pt_cut);
  }

  return VH_lep_categories;
}

std::set<STXS1> define_ttH_subcategories(const STXS0 category) {
  std::set<STXS1> ttH_categories;

  STXS1 ttH = STXS1{.Category = category};
  for (STXS1 pt_cut :
       cut_ranges(ttH, Pt, {0, 60, 120, 200, 300, 450, 650, infty}))
    ttH_categories.insert(pt_cut);

  return ttH_categories;
}

std::set<STXS1> define_categories() {
  std::set<STXS1> all_categories;

  std::set<STXS1> STXS0_subactegories[] = {
      define_untagged_subcategories(),
      define_VBF_subcategories(),
      define_VH_had_subcategories(),
      define_VH_lep_subcategories(),
      define_ttH_subcategories(STXS0::ttH_hadronic),
      define_ttH_subcategories(STXS0::ttH_leptonic)};

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

} // namespace Version1_3
