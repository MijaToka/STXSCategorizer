#include "STXS_common.cpp"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <ROOT/RDF/RVariationsDescription.hxx>
#include <ROOT/RVec.hxx>
#include <RtypesCore.h>
#include <map>

std::map<STXS0, ROOT::RDF::RNode> first_categorization(ROOT::RDF::RNode df) {

  df =
      df.Define("VBF2j_mask",
                [](ROOT::VecOps::RVec<Int_t> nExtraLep, Float_t DVBF2j,
                   Char_t nJets, Int_t nBtag, Short_t idx) {
                  return (nExtraLep[idx] == 0) && (DVBF2j > 0.5) &&
                         (((nJets == 2 || nJets == 3) && (nBtag <= 1)) ||
                          (nJets == 4 && nBtag == 0));
                },
                {"ZZCand_nExtraLep", "DVBF2j_ME", "nCleanedJetsPt30",
                 "nBtagged_filtered", "bestCandIdx"})
          .Define("VH_had_mask",
                  [](ROOT::VecOps::RVec<Int_t> nExtraLep, Float_t DWHh,
                     Float_t DZHh, Char_t nJets, Int_t nBtag, Short_t idx) {
                    return (idx != -1) ? (nExtraLep[idx] == 0) &&
                                             (DWHh > 0.5 || DZHh > 0.5) &&
                                             ((nJets == 2 || nJets == 3) ||
                                              (nJets == 4 && nBtag == 0))
                                       : false;
                  },
                  {"ZZCand_nExtraLep", "DWHh_ME", "DZHh_ME", "nCleanedJetsPt30",
                   "nBtagged_filtered", "bestCandIdx"})
          .Define("is_OSSF_pair",
                  [](Int_t id4, Int_t id5) {
                    return (id4 != 0) && (id4 + id5 == 0);
                  },
                  {"LepPdgId_4", "LepPdgId_5"})
          .Define("VH_lep_mask",
                  [](ROOT::VecOps::RVec<Int_t> nExtraLep, Int_t nBtag,
                     Bool_t is_OSSF, Char_t nJets, Short_t idx) {
                    return (idx != -1) ? (nExtraLep[idx] <= 3 && nBtag == 0 &&
                                          (nExtraLep[idx] == 1 || is_OSSF)) ||
                                             (nExtraLep[idx] >= 1 && nJets == 0)
                                       : false;
                  },
                  {"ZZCand_nExtraLep", "nBtagged_filtered", "is_OSSF_pair",
                   "nCleanedJetsPt30", "bestCandIdx"})
          .Define("ttH_had_mask",
                  [](ROOT::VecOps::RVec<Int_t> nExtraLep, Char_t nJets,
                     Int_t nBtag, Short_t idx) {
                    return (idx != -1)
                               ? nJets >= 4 && nBtag >= 1 && nExtraLep[idx] == 0
                               : false;
                  },
                  {"ZZCand_nExtraLep", "nCleanedJetsPt30", "nBtagged_filtered",
                   "bestCandIdx"})
          .Define("ttH_lep_mask",
                  [](ROOT::VecOps::RVec<Int_t> nExtraLep, Short_t idx) {
                    return (idx != -1) ? nExtraLep[idx] >= 1 : false;
                  },
                  {"ZZCand_nExtraLep", "bestCandIdx"})
          .Define("VBF1j_mask",
                  [](ROOT::VecOps::RVec<Int_t> nExtraLep, Float_t DVBF1j,
                     Char_t nJets, Short_t idx) {
                    return (idx != -1) ? nJets == 1 && nExtraLep[idx] == 0 &&
                                             DVBF1j > 0.7
                                       : false;
                  },
                  {"ZZCand_nExtraLep", "DVBF1j_ME", "nCleanedJetsPt30",
                   "bestCandIdx"});

  std::map<STXS0, ROOT::RDF::RNode> arr;

  arr.emplace(STXS0::VBF_2jet, df.Filter("VBF2j_mask"));
  arr.emplace(STXS0::VH_hadronic, df.Filter("!VBF2j_mask && VH_had_mask"));
  arr.emplace(STXS0::VH_leptonic,
              df.Filter("!VBF2j_mask && !VH_had_mask && VH_lep_mask"));
  arr.emplace(
      STXS0::ttH_hadronic,
      df.Filter("!VBF2j_mask && !VH_had_mask && !VH_lep_mask && ttH_had_mask"));
  arr.emplace(
      STXS0::ttH_leptonic,
      df.Filter("!VBF2j_mask && !VH_had_mask && !VH_lep_mask && !ttH_had_mask "
                "&& ttH_lep_mask"));
  arr.emplace(
      STXS0::VBF_1jet,
      df.Filter("!VBF2j_mask && !VH_had_mask && !VH_lep_mask && !ttH_had_mask "
                "&& !ttH_lep_mask && VBF1j_mask"));
  arr.emplace(
      STXS0::Untagged,
      df.Filter("!VBF2j_mask && !VH_had_mask && !VH_lep_mask && !ttH_had_mask "
                "&& !ttH_lep_mask && !VBF1j_mask"));
  return arr;
}
