#include "STXS/Categorize/interface/STXS_common.h"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <ROOT/RDF/RVariationsDescription.hxx>
#include <ROOT/RVec.hxx>
#include <RtypesCore.h>
#include <map>
#include <sstream>

std::map<STXS0, ROOT::RDF::RNode> first_categorization(ROOT::RDF::RNode df) {

  df =
      df.Define(
            "VBF2j_mask",
            [](ROOT::VecOps::RVec<Int_t> nExtraLep, Float_t DVBF2j,
               Char_t nJets, Int_t nBtag, Short_t idx) {
              return (idx != -1)
                         ? (nExtraLep[idx] == 0) && (DVBF2j > 0.5) &&
                               (((nJets == 2 || nJets == 3) && (nBtag <= 1)) ||
                                (nJets == 4 && nBtag == 0))
                         : false;
            },
            {"ZZCand_nExtraLep", "DVBF2j_ME", "nCleanedJetsPt30",
             "nBtagged_filtered", "bestCandIdx"})
          .Define("VH_had_mask",
                  [](ROOT::VecOps::RVec<Int_t> nExtraLep, Float_t DWHh,
                     Float_t DZHh, Char_t nJets, Int_t nBtag, Short_t idx) {
                    return (idx != -1) ? (nExtraLep[idx] == 0) &&
                                             (DWHh > 0.5 || DZHh > 0.5) &&
                                             (((nJets == 2 || nJets == 3) &&
                                               nBtag <= 1) ||
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
                  [](ROOT::VecOps::RVec<Int_t> nExtraLep, Char_t nJets,
                     Int_t nBtag, Short_t idx) {
                    return (idx != -1)
                               ? (nBtag >= 2 || (nBtag >= 1 && nJets >= 2)) &&
                                     nExtraLep[idx] >= 1
                               : false;
                  },
                  {"ZZCand_nExtraLep", "nCleanedJetsPt30", "nBtagged_filtered",
                   "bestCandIdx"})
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

  STXS0 filterOrder[] = {STXS0::VBF_2jet,     STXS0::VH_hadronic,
                         STXS0::VH_leptonic,  STXS0::ttH_hadronic,
                         STXS0::ttH_leptonic, STXS0::VBF_1jet,
                         STXS0 ::Untagged};

  std::map<STXS0, std::string> maskName = {
      {STXS0::VBF_2jet, "VBF2j_mask"},
      {STXS0::VH_hadronic, "VH_had_mask"},
      {STXS0::VH_leptonic, "VH_lep_mask"},
      {STXS0::ttH_hadronic, "ttH_had_mask"},
      {STXS0::ttH_leptonic, "ttH_lep_mask"},
      {STXS0::VBF_1jet, "VBF1j_mask"},
      {STXS0::Untagged, "true"}};

  std::stringstream exclusionMask;
  for (STXS0 category : filterOrder) {
    std::stringstream currMask;
    currMask << exclusionMask.str() << maskName.at(category);

    arr.emplace(category, df.Filter(currMask.str()));

    exclusionMask << "!" << maskName.at(category) << " & ";
  }

  return arr;
}
