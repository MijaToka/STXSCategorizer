#include "../interface/Discriminants.h"
#include <Math/Vector4D.h>
#include <Math/Vector4Dfwd.h>
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterfaceBase.hxx>
#include <ROOT/RDFHelpers.hxx>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <RtypesCore.h>
#include <TFile.h>
#include <TTree.h>
#include <cstdlib>
#include <string>

Double_t get_genEventSumw(const std::string &file_path) {
  TFile *file = TFile::Open(file_path.c_str(), "READ");

  TTree *runs = (TTree *)file->Get("Runs");
  TTree *events = (TTree *)file->Get("Events");

  Double_t genEventSumw(0), eventSumw;
  Long64_t genEventCount(0), eventCount;

  runs->SetBranchAddress("genEventSumw", &eventSumw);
  runs->SetBranchAddress("genEventCount", &eventCount);

  for (int iRun = 0; iRun < runs->GetEntries(); iRun++) {
    runs->GetEntry(iRun);
    genEventSumw += eventSumw;
    genEventCount += eventCount;
  }
  return genEventSumw;
};

int STXSStage12MergedCat(int cat) {
  if (cat == 102 || cat == 103 || cat == 104)
    return 101;
  if (cat == 114 || cat == 115 || cat == 116)
    return 113;
  if (cat == 201 || cat == 203 || cat == 205)
    return 202;
  if (cat == 210)
    return 208;
  if (cat == 302 || cat == 401 || cat == 402 || cat == 501 || cat == 502)
    return 301;
  if (cat == 304 || cat == 305 || cat == 403 || cat == 404 || cat == 405 ||
      cat == 503 || cat == 504 || cat == 505)
    return 303;
  if (cat == 602 || cat == 603 || cat == 604 || cat == 605)
    return 601;
  return cat;
}

std::string STXSStage12Label(int cat, const std::string &mode) {
  const int merged_cat = STXSStage12MergedCat(cat);
  std::string label;
  switch (merged_cat) {
  case 0:
    label = "UNKNOWN";
    break;
  case 100:
    label = "GG2H_FWDH";
    break;
  case 101:
    label = "GG2H_PTH_GT200";
    break;
  case 105:
    label = "GG2H_0J_PTH_0_10";
    break;
  case 106:
    label = "GG2H_0J_PTH_GT10";
    break;
  case 107:
    label = "GG2H_1J_PTH_0_60";
    break;
  case 108:
    label = "GG2H_1J_PTH_60_120";
    break;
  case 109:
    label = "GG2H_1J_PTH_120_200";
    break;
  case 110:
    label = "GG2H_GE2J_MJJ_0_350_PTH_0_60";
    break;
  case 111:
    label = "GG2H_GE2J_MJJ_0_350_PTH_60_120";
    break;
  case 112:
    label = "GG2H_GE2J_MJJ_0_350_PTH_120_200";
    break;
  case 113:
    label = "GG2H_GE2J_MJJ_GT350";
    break;
  case 200:
    label = "QQ2HQQ_FWDH";
    break;
  case 202:
    label = "QQ2HQQ_rest";
    break;
  case 204:
    label = "QQ2HQQ_GE2J_MJJ_60_120";
    break;
  case 206:
    label = "QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200";
    break;
  case 207:
    label = "QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25";
    break;
  case 208:
    label = "QQ2HQQ_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25";
    break;
  case 209:
    label = "QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25";
    break;
  case 300:
    label = "QQ2HLNU_FWDH";
    break;
  case 301:
    label = "VH_lep_PTV_0_150";
    break;
  case 303:
    label = "VH_lep_PTV_GT150";
    break;
  case 400:
    label = "QQ2HLL_FWDH";
    break;
  case 500:
    label = "GG2HLL_FWDH";
    break;
  case 600:
    label = "TTH_FWDH";
    break;
  case 601:
    label = "TTH";
    break;
  default:
    label = "UNKNOWN";
    break;
  }

  if (label.rfind("QQ2HQQ", 0) == 0) {
    std::string replacement;
    if (mode == "VBFH125") {
      replacement = "VBF";
    } else if (mode == "WplusH125") {
      replacement = "WplushadH";
    } else if (mode == "WminusH125") {
      replacement = "WminushadH";
    } else if (mode == "ZH125") {
      replacement = "ZhadH";
    }

    if (!replacement.empty()) {
      return replacement + label.substr(6);
    }
  }

  if (label.rfind("VH", 0) == 0) {
    std::string replacement;
    if (mode == "WplusH125") {
      replacement = "WplusH";
    } else if (mode == "WminusH125") {
      replacement = "WminusH";
    } else if (mode == "ZH125") {
      replacement = "ZH";
    }

    if (!replacement.empty()) {
      return replacement + label.substr(2);
    }
  }

  return label;
}

ROOT::RDF::RNode setup(std::string &file, const std::string &mode) {

  ROOT::RDF::RNode df = ROOT::RDataFrame("Events", file);

  // Define kinematical variables
  // TODO: Hj_H_pt
  df =
      df.Filter([](Short_t id) { return id > -1; }, {"bestCandIdx"})
          .Filter(
              [](const ROOT::RVec<Float_t> &mass, Short_t idx) {
                return mass[idx] > 105.f && mass[idx] < 160.f;
              },
              {"ZZCand_mass", "bestCandIdx"})
          .Define("ZZVector",
                  [](ROOT::RVec<Float_t> &pt, ROOT::RVec<Float_t> &eta,
                     ROOT::RVec<Float_t> &phi, ROOT::RVec<Float_t> mass,
                     Short_t idx) {
                    return ROOT::Math::PtEtaPhiMVector(pt[idx], eta[idx],
                                                       phi[idx], mass[idx]);
                  },
                  {"ZZCand_pt", "ZZCand_eta", "ZZCand_phi", "ZZCand_mass",
                   "bestCandIdx"})
          .Define("JetLeading_pt",
                  [](ROOT::RVec<Float_t> &pt, Short_t idx) {
                    return idx != -1 ? pt[idx] : -999.f;
                  },
                  {"Jet_pt", "JetLeadingIdx"})
          .Define("JetLeading_eta",
                  [](ROOT::RVec<Float_t> &eta, Short_t idx) {
                    return idx != -1 ? eta[idx] : -999.f;
                  },
                  {"Jet_eta", "JetLeadingIdx"})
          .Define("JetLeading_phi",
                  [](ROOT::RVec<Float_t> &phi, Short_t idx) {
                    return idx != -1 ? phi[idx] : -999.f;
                  },
                  {"Jet_phi", "JetLeadingIdx"})
          .Define("JetLeading_mass",
                  [](ROOT::RVec<Float_t> &mass, Short_t idx) {
                    return idx != -1 ? mass[idx] : -999.f;
                  },
                  {"Jet_mass", "JetLeadingIdx"})
          .Define("JetLeading_btag",
                  [](ROOT::RVec<Float_t> &btag, Short_t idx) {
                    return idx != -1 ? btag[idx] : -999.f;
                  },
                  {"Jet_btagPNetB", "JetLeadingIdx"})
          .Define("JetSubleading_pt",
                  [](ROOT::RVec<Float_t> &pt, Short_t idx) {
                    return idx != -1 ? pt[idx] : -999.f;
                  },
                  {"Jet_pt", "JetSubleadingIdx"})
          .Define("JetSubleading_eta",
                  [](ROOT::RVec<Float_t> &eta, Short_t idx) {
                    return idx != -1 ? eta[idx] : -999.f;
                  },
                  {"Jet_eta", "JetSubleadingIdx"})
          .Define("JetSubleading_phi",
                  [](ROOT::RVec<Float_t> &phi, Short_t idx) {
                    return idx != -1 ? phi[idx] : -999.f;
                  },
                  {"Jet_phi", "JetSubleadingIdx"})
          .Define("JetSubleading_mass",
                  [](ROOT::RVec<Float_t> &mass, Short_t idx) {
                    return idx != -1 ? mass[idx] : -999.f;
                  },
                  {"Jet_mass", "JetSubleadingIdx"})
          .Define("JetSubleading_btag",
                  [](ROOT::RVec<Float_t> &btag, Short_t idx) {
                    return idx != -1 ? btag[idx] : -999.f;
                  },
                  {"Jet_btagPNetB", "JetSubleadingIdx"})
          .Define(
              "deltaEta_jj",
              [](ROOT::RVec<Float_t> &eta, Short_t leadIdx, Short_t subIdx) {
                return (leadIdx != 1 && subIdx != 1)
                           ? std::abs(eta[leadIdx] - eta[subIdx])
                           : -999.f;
              },
              {"Jet_eta", "JetLeadingIdx", "JetSubleadingIdx"})
          .Define(
              "deltaPhi_jj",
              [](ROOT::RVec<Float_t> &phi, Short_t leadIdx, Short_t subIdx) {
                return (leadIdx != -1 && subIdx != -1)
                           ? ROOT::VecOps::DeltaPhi(phi[leadIdx], phi[subIdx])
                           : -999.f;
              },
              {"Jet_phi", "JetLeadingIdx", "JetSubleadingIdx"})
          .Define("LeadingVector",
                  [](ROOT::RVec<Float_t> &pt, ROOT::RVec<Float_t> &eta,
                     ROOT::RVec<Float_t> &phi, ROOT::RVec<Float_t> &mass,
                     Short_t idx) {
                    return (idx != -1)
                               ? ROOT::Math::PtEtaPhiMVector(
                                     pt[idx], eta[idx], phi[idx], mass[idx])
                               : ROOT::Math::PtEtaPhiMVector(-999.f, -999.f,
                                                             -999.f, -999.f);
                  },
                  {"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "JetLeadingIdx"})
          .Define(
              "SubLeadingVector",
              [](ROOT::RVec<Float_t> &pt, ROOT::RVec<Float_t> &eta,
                 ROOT::RVec<Float_t> &phi, ROOT::RVec<Float_t> &mass,
                 Short_t idx) {
                return (idx != -1)
                           ? ROOT::Math::PtEtaPhiMVector(pt[idx], eta[idx],
                                                         phi[idx], mass[idx])
                           : ROOT::Math::PtEtaPhiMVector(-999.f, -999.f, -999.f,
                                                         -999.f);
              },
              {"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "JetSubleadingIdx"})
          .Define("m_jj",
                  [](ROOT::Math::PtEtaPhiMVector lead,
                     ROOT::Math::PtEtaPhiMVector sublead, Short_t leadIdx,
                     Short_t subIdx) {
                    return (leadIdx != -1 && subIdx != -1)
                               ? (lead + sublead).M()
                               : -999.f;
                  },
                  {"LeadingVector", "SubLeadingVector", "JetLeadingIdx",
                   "JetSubleadingIdx"})
          .Define("ZZjj_pt",
                  [](ROOT::Math::PtEtaPhiMVector ZZ,
                     ROOT::Math::PtEtaPhiMVector lead,
                     ROOT::Math::PtEtaPhiMVector sublead, Short_t leadIdx,
                     Short_t subIdx) {
                    return (leadIdx != -1 && subIdx != -1)
                               ? (ZZ + lead + sublead).Pt()
                               : -999.f;
                  },
                  {"ZZVector", "LeadingVector", "SubLeadingVector",
                   "JetLeadingIdx", "JetSubleadingIdx"})
          .Define("ZZj_pt",
                  [](ROOT::Math::PtEtaPhiMVector ZZ,
                     ROOT::Math::PtEtaPhiMVector lead, Short_t leadIdx) {
                    return (leadIdx != -1) ? (ZZ + lead).Pt() : -999.f;
                  },
                  {"ZZVector", "LeadingVector", "JetLeadingIdx"})
          .Define("Jet_btagPNetB_filtered",
                  [](ROOT::RVec<Float_t> pt, ROOT::RVec<Bool_t> ZZMask,
                     ROOT::RVec<Float_t> btagPNetB) {
                    ROOT::VecOps::RVec<Float_t> filtered;
                    for (int i = 0; i < pt.size(); i++) {
                      if (pt[i] > 30 && !ZZMask[i]) {
                        filtered.push_back(btagPNetB[i]);
                      }
                    }
                    return filtered;
                  },
                  {"Jet_pt", "Jet_ZZMask", "Jet_btagPNetB"})
          .Define("nBtagged_filtered",
                  [](ROOT::VecOps::RVec<Float_t> bTag) {
                    return ROOT::VecOps::Sum(bTag > 0.2421);
                  },
                  {"Jet_btagPNetB_filtered"});

  // Define MET alias for cosnsistency
  df = df.Define("PFMET_pt", "MET_pt");

  // Define discriminants
  df =
      df.Define(
            "DVBF2j_ME",
            [](ROOT::VecOps::RVec<Float_t> P_JJVBF,
               ROOT::VecOps::RVec<Float_t> P_JJQCD,
               ROOT::VecOps::RVec<Float_t> ZZMass, Char_t nJets, Short_t idx) {
              return (nJets >= 2)
                         ? DVBF2j_ME(P_JJVBF[idx], P_JJQCD[idx], ZZMass[idx])
                         : -999.f;
            },
            {"ZZCand_P_JJVBF_SIG_ghv1_1_JHUGen_JECNominal",
             "ZZCand_P_JJQCD_SIG_ghg2_1_JHUGen_JECNominal", "ZZCand_mass",
             "nCleanedJetsPt30", "bestCandIdx"})
          .Define("DVBF1j_ME",
                  [](ROOT::VecOps::RVec<Float_t> P_JVBF,
                     ROOT::VecOps::RVec<Float_t> P_JVBF_aux,
                     ROOT::VecOps::RVec<Float_t> P_JQCD,
                     ROOT::VecOps::RVec<Float_t> ZZMass, Char_t nJets,
                     Short_t idx) {
                    return (nJets == 1)
                               ? DVBF1j_ME(P_JVBF[idx], P_JVBF_aux[idx],
                                           P_JQCD[idx], ZZMass[idx])
                               : -999.f;
                  },
                  {"ZZCand_P_JVBF_SIG_ghv1_1_JHUGen_JECNominal",
                   "ZZCand_P_JVBF_SIG_ghv1_1_JHUGen_JECNominal_aux",
                   "ZZCand_P_JQCD_SIG_ghg2_1_JHUGen_JECNominal", "ZZCand_mass",
                   "nCleanedJetsPt30", "bestCandIdx"})
          .Define("DWHh_ME",
                  [](ROOT::VecOps::RVec<Float_t> P_HadWH,
                     ROOT::VecOps::RVec<Float_t> P_HadWH_mvajj,
                     ROOT::VecOps::RVec<Float_t> P_HadWH_mvajj_true,
                     ROOT::VecOps::RVec<Float_t> P_JJQCD,
                     ROOT::VecOps::RVec<Float_t> ZZMass, Char_t nJets,
                     Short_t idx) {
                    return (nJets >= 2)
                               ? DWHh_ME(P_HadWH[idx], P_JJQCD[idx],
                                         P_HadWH_mvajj[idx],
                                         P_HadWH_mvajj_true[idx], ZZMass[idx])
                               : -999.f;
                  },
                  {"ZZCand_P_HadWH_SIG_ghw1_1_JHUGen_JECNominal",
                   "ZZCand_P_HadWH_SIG_ghw1_1_JHUGen_JECNominal_mavjj",
                   "ZZCand_P_HadWH_SIG_ghw1_1_JHUGen_JECNominal_mvajj_true",
                   "ZZCand_P_JJQCD_SIG_ghg2_1_JHUGen_JECNominal", "ZZCand_mass",
                   "nCleanedJetsPt30", "bestCandIdx"})
          .Define("DZHh_ME",
                  [](ROOT::VecOps::RVec<Float_t> P_HadZH,
                     ROOT::VecOps::RVec<Float_t> P_HadZH_mvajj,
                     ROOT::VecOps::RVec<Float_t> P_HadZH_mvajj_true,
                     ROOT::VecOps::RVec<Float_t> P_JJQCD,
                     ROOT::VecOps::RVec<Float_t> ZZMass, Char_t nJets,
                     Short_t idx) {
                    return (nJets >= 2)
                               ? DWHh_ME(P_HadZH[idx], P_JJQCD[idx],
                                         P_HadZH_mvajj[idx],
                                         P_HadZH_mvajj_true[idx], ZZMass[idx])
                               : -999.f;
                  },
                  {"ZZCand_P_HadZH_SIG_ghz1_1_JHUGen_JECNominal",
                   "ZZCand_P_HadZH_SIG_ghz1_1_JHUGen_JECNominal_mavjj",
                   "ZZCand_P_HadZH_SIG_ghz1_1_JHUGen_JECNominal_mvajj_true",
                   "ZZCand_P_JJQCD_SIG_ghg2_1_JHUGen_JECNominal", "ZZCand_mass",
                   "nCleanedJetsPt30", "bestCandIdx"})
          .Define("DVBF2j_ME_noC",
                  [](ROOT::VecOps::RVec<Float_t> P_JJVBF,
                     ROOT::VecOps::RVec<Float_t> P_JJQCD, Char_t nJets,
                     Short_t idx) {
                    return (nJets >= 2)
                               ? 1.f / (1.f + P_JJQCD[idx] / P_JJVBF[idx])
                               : -999.f;
                  },
                  {"ZZCand_P_JJVBF_SIG_ghv1_1_JHUGen_JECNominal",
                   "ZZCand_P_JJQCD_SIG_ghg2_1_JHUGen_JECNominal",
                   "nCleanedJetsPt30", "bestCandIdx"})
          .Define("DVBF1j_ME_noC",
                  [](ROOT::VecOps::RVec<Float_t> P_JVBF,
                     ROOT::VecOps::RVec<Float_t> P_JVBF_aux,
                     ROOT::VecOps::RVec<Float_t> P_JQCD, Char_t nJets,
                     Short_t idx) {
                    return (nJets == 1)
                               ? 1.f / (1.f + P_JQCD[idx] / (P_JVBF[idx] *
                                                             P_JVBF_aux[idx]))
                               : -999.f;
                  },
                  {"ZZCand_P_JVBF_SIG_ghv1_1_JHUGen_JECNominal",
                   "ZZCand_P_JVBF_SIG_ghv1_1_JHUGen_JECNominal_aux",
                   "ZZCand_P_JQCD_SIG_ghg2_1_JHUGen_JECNominal",
                   "nCleanedJetsPt30", "bestCandIdx"})
          .Define("DWHh_ME_noC",
                  [](ROOT::VecOps::RVec<Float_t> P_HadWH,
                     ROOT::VecOps::RVec<Float_t> P_HadWH_mvajj,
                     ROOT::VecOps::RVec<Float_t> P_HadWH_mvajj_true,
                     ROOT::VecOps::RVec<Float_t> P_JJQCD, Char_t nJets,
                     Short_t idx) {
                    return (nJets >= 2)
                               ? 1.f /
                                     (1.f +
                                      (P_HadWH_mvajj_true[idx] * P_JJQCD[idx]) /
                                          (P_HadWH_mvajj[idx] * P_HadWH[idx]))
                               : -999.f;
                  },
                  {"ZZCand_P_HadWH_SIG_ghw1_1_JHUGen_JECNominal",
                   "ZZCand_P_HadWH_SIG_ghw1_1_JHUGen_JECNominal_mavjj",
                   "ZZCand_P_HadWH_SIG_ghw1_1_JHUGen_JECNominal_mvajj_true",
                   "ZZCand_P_JJQCD_SIG_ghg2_1_JHUGen_JECNominal",
                   "nCleanedJetsPt30", "bestCandIdx"})
          .Define("DZHh_ME_noC",
                  [](ROOT::VecOps::RVec<Float_t> P_HadZH,
                     ROOT::VecOps::RVec<Float_t> P_HadZH_mvajj,
                     ROOT::VecOps::RVec<Float_t> P_HadZH_mvajj_true,
                     ROOT::VecOps::RVec<Float_t> P_JJQCD, Char_t nJets,
                     Short_t idx) {
                    return (nJets >= 2)
                               ? 1.f /
                                     (1.f +
                                      (P_HadZH_mvajj_true[idx] * P_JJQCD[idx]) /
                                          (P_HadZH_mvajj[idx] * P_HadZH[idx]))
                               : -999.f;
                  },
                  {"ZZCand_P_HadZH_SIG_ghz1_1_JHUGen_JECNominal",
                   "ZZCand_P_HadZH_SIG_ghz1_1_JHUGen_JECNominal_mavjj",
                   "ZZCand_P_HadZH_SIG_ghz1_1_JHUGen_JECNominal_mvajj_true",
                   "ZZCand_P_JJQCD_SIG_ghg2_1_JHUGen_JECNominal",
                   "nCleanedJetsPt30", "bestCandIdx"});

  // Define lepton features
  df =
      df.Define(
            "Z1_lep_hi_idx",
            [](ROOT::VecOps::RVec<Float_t> LepPt,
               ROOT::VecOps::RVec<Short_t> l1, ROOT::VecOps::RVec<Short_t> l2,
               Short_t idx) {
              return (LepPt[l1[idx]] >= LepPt[l1[idx]]) ? l1[idx] : l2[idx];
            },
            {"Lepton_pt", "ZZCand_Z1l1Idx", "ZZCand_Z1l2Idx", "bestCandIdx"})
          .Define(
              "Z1_lep_lo_idx",
              [](ROOT::VecOps::RVec<Float_t> LepPt,
                 ROOT::VecOps::RVec<Short_t> l1, ROOT::VecOps::RVec<Short_t> l2,
                 Short_t idx) {
                return (LepPt[l1[idx]] >= LepPt[l1[idx]]) ? l2[idx] : l1[idx];
              },
              {"Lepton_pt", "ZZCand_Z1l1Idx", "ZZCand_Z1l2Idx", "bestCandIdx"})
          .Define(
              "Z2_lep_hi_idx",
              [](ROOT::VecOps::RVec<Float_t> LepPt,
                 ROOT::VecOps::RVec<Short_t> l1, ROOT::VecOps::RVec<Short_t> l2,
                 Short_t idx) {
                return (LepPt[l1[idx]] >= LepPt[l1[idx]]) ? l1[idx] : l2[idx];
              },
              {"Lepton_pt", "ZZCand_Z2l1Idx", "ZZCand_Z2l2Idx", "bestCandIdx"})
          .Define(
              "Z2_lep_lo_idx",
              [](ROOT::VecOps::RVec<Float_t> LepPt,
                 ROOT::VecOps::RVec<Short_t> l1, ROOT::VecOps::RVec<Short_t> l2,
                 Short_t idx) {
                return (LepPt[l1[idx]] >= LepPt[l1[idx]]) ? l2[idx] : l1[idx];
              },
              {"Lepton_pt", "ZZCand_Z2l1Idx", "ZZCand_Z2l2Idx", "bestCandIdx"})
          .Define("LepPt",
                  [](ROOT::VecOps::RVec<Float_t> Pt, Short_t Z1hi, Short_t Z1lo,
                     Short_t Z2hi, Short_t Z2lo) {
                    ROOT::VecOps::RVec<Float_t> LepPt{Pt[Z1hi], Pt[Z1lo],
                                                      Pt[Z2hi], Pt[Z2lo]};
                    return LepPt;
                  },
                  {"Lepton_pt", "Z1_lep_hi_idx", "Z1_lep_lo_idx",
                   "Z2_lep_hi_idx", "Z2_lep_lo_idx"})
          .Define("LepEta",
                  [](ROOT::VecOps::RVec<Float_t> Eta, Short_t Z1hi,
                     Short_t Z1lo, Short_t Z2hi, Short_t Z2lo) {
                    ROOT::VecOps::RVec<Float_t> LepEta{Eta[Z1hi], Eta[Z1lo],
                                                       Eta[Z2hi], Eta[Z2lo]};
                    return LepEta;
                  },
                  {"Lepton_eta", "Z1_lep_hi_idx", "Z1_lep_lo_idx",
                   "Z2_lep_hi_idx", "Z2_lep_lo_idx"})
          .Define("LepPhi",
                  [](ROOT::VecOps::RVec<Float_t> Phi, Short_t Z1hi,
                     Short_t Z1lo, Short_t Z2hi, Short_t Z2lo) {
                    ROOT::VecOps::RVec<Float_t> LepPhi{Phi[Z1hi], Phi[Z1lo],
                                                       Phi[Z2hi], Phi[Z2lo]};
                    return LepPhi;
                  },
                  {"Lepton_phi", "Z1_lep_hi_idx", "Z1_lep_lo_idx",
                   "Z2_lep_hi_idx", "Z2_lep_lo_idx"})
          .Define("LepPdgId",
                  [](ROOT::VecOps::RVec<Int_t> PdgId, Short_t Z1hi,
                     Short_t Z1lo, Short_t Z2hi, Short_t Z2lo) {
                    ROOT::VecOps::RVec<Int_t> LepPdgId{
                        PdgId[Z1hi], PdgId[Z1lo], PdgId[Z2hi], PdgId[Z2lo]};
                    return LepPdgId;
                  },
                  {"Lepton_pdgId", "Z1_lep_hi_idx", "Z1_lep_lo_idx",
                   "Z2_lep_hi_idx", "Z2_lep_lo_idx"})
          .Define("LepPt_0",
                  [](ROOT::VecOps::RVec<Float_t> LepPt) { return LepPt[0]; },
                  {"LepPt"})
          .Define("LepPt_1",
                  [](ROOT::VecOps::RVec<Float_t> LepPt) { return LepPt[1]; },
                  {"LepPt"})
          .Define("LepPt_2",
                  [](ROOT::VecOps::RVec<Float_t> LepPt) { return LepPt[2]; },
                  {"LepPt"})
          .Define("LepPt_3",
                  [](ROOT::VecOps::RVec<Float_t> LepPt) { return LepPt[3]; },
                  {"LepPt"})
          .Define("LepPt_4",
                  [](ROOT::VecOps::RVec<Float_t> LepPt,
                     ROOT::VecOps::RVec<Short_t> extraLepIdx, Short_t idx) {
                    return (extraLepIdx[idx] != -1) ? LepPt[extraLepIdx[idx]]
                                                    : -999.f;
                  },
                  {"Lepton_pt", "ZZCand_extraLep1Idx", "bestCandIdx"})
          .Define("LepPt_5",
                  [](ROOT::VecOps::RVec<Float_t> LepPt,
                     ROOT::VecOps::RVec<Short_t> extraLepIdx, Short_t idx) {
                    return (extraLepIdx[idx] != -1) ? LepPt[extraLepIdx[idx]]
                                                    : -999.f;
                  },
                  {"Lepton_pt", "ZZCand_extraLep2Idx", "bestCandIdx"})
          .Define("LepEta_0",
                  [](ROOT::VecOps::RVec<Float_t> LepEta) { return LepEta[0]; },
                  {"LepEta"})
          .Define("LepEta_1",
                  [](ROOT::VecOps::RVec<Float_t> LepEta) { return LepEta[1]; },
                  {"LepEta"})
          .Define("LepEta_2",
                  [](ROOT::VecOps::RVec<Float_t> LepEta) { return LepEta[2]; },
                  {"LepEta"})
          .Define("LepEta_3",
                  [](ROOT::VecOps::RVec<Float_t> LepEta) { return LepEta[3]; },
                  {"LepEta"})
          .Define("LepEta_4",
                  [](ROOT::VecOps::RVec<Float_t> LepEta,
                     ROOT::VecOps::RVec<Short_t> extraLepIdx, Short_t idx) {
                    return (extraLepIdx[idx] != -1) ? LepEta[extraLepIdx[idx]]
                                                    : -999.f;
                  },
                  {"Lepton_eta", "ZZCand_extraLep1Idx", "bestCandIdx"})
          .Define("LepEta_5",
                  [](ROOT::VecOps::RVec<Float_t> LepEta,
                     ROOT::VecOps::RVec<Short_t> extraLepIdx, Short_t idx) {
                    return (extraLepIdx[idx] != -1) ? LepEta[extraLepIdx[idx]]
                                                    : -999.f;
                  },
                  {"Lepton_eta", "ZZCand_extraLep2Idx", "bestCandIdx"})
          .Define("LepPhi_0",
                  [](ROOT::VecOps::RVec<Float_t> LepPhi) { return LepPhi[0]; },
                  {"LepPhi"})
          .Define("LepPhi_1",
                  [](ROOT::VecOps::RVec<Float_t> LepPhi) { return LepPhi[1]; },
                  {"LepPhi"})
          .Define("LepPhi_2",
                  [](ROOT::VecOps::RVec<Float_t> LepPhi) { return LepPhi[2]; },
                  {"LepPhi"})
          .Define("LepPhi_3",
                  [](ROOT::VecOps::RVec<Float_t> LepPhi) { return LepPhi[3]; },
                  {"LepPhi"})
          .Define("LepPhi_4",
                  [](ROOT::VecOps::RVec<Float_t> LepPhi,
                     ROOT::VecOps::RVec<Short_t> extraLepIdx, Short_t idx) {
                    return (extraLepIdx[idx] != -1) ? LepPhi[extraLepIdx[idx]]
                                                    : -999.f;
                  },
                  {"Lepton_phi", "ZZCand_extraLep1Idx", "bestCandIdx"})
          .Define("LepPhi_5",
                  [](ROOT::VecOps::RVec<Float_t> LepPhi,
                     ROOT::VecOps::RVec<Short_t> extraLepIdx, Short_t idx) {
                    return (extraLepIdx[idx] != -1) ? LepPhi[extraLepIdx[idx]]
                                                    : -999.f;
                  },
                  {"Lepton_phi", "ZZCand_extraLep2Idx", "bestCandIdx"})
          .Define(
              "LepPdgId_0",
              [](ROOT::VecOps::RVec<Int_t> LepPdgId) { return LepPdgId[0]; },
              {"LepPdgId"})
          .Define(
              "LepPdgId_1",
              [](ROOT::VecOps::RVec<Int_t> LepPdgId) { return LepPdgId[1]; },
              {"LepPdgId"})
          .Define(
              "LepPdgId_2",
              [](ROOT::VecOps::RVec<Int_t> LepPdgId) { return LepPdgId[2]; },
              {"LepPdgId"})
          .Define(
              "LepPdgId_3",
              [](ROOT::VecOps::RVec<Int_t> LepPdgId) { return LepPdgId[3]; },
              {"LepPdgId"})
          .Define("LepPdgId_4",
                  [](ROOT::VecOps::RVec<Int_t> LepPdgId,
                     ROOT::VecOps::RVec<Short_t> extraLepIdx, Short_t idx) {
                    return (extraLepIdx[idx] != -1) ? LepPdgId[extraLepIdx[idx]]
                                                    : -999;
                  },
                  {"Lepton_pdgId", "ZZCand_extraLep1Idx", "bestCandIdx"})
          .Define("LepPdgId_5",
                  [](ROOT::VecOps::RVec<Int_t> LepPdgId,
                     ROOT::VecOps::RVec<Short_t> extraLepIdx, Short_t idx) {
                    return (extraLepIdx[idx] != -1) ? LepPdgId[extraLepIdx[idx]]
                                                    : -999;
                  },
                  {"Lepton_pdgId", "ZZCand_extraLep2Idx", "bestCandIdx"});

  // Event weights and production mode
  Double_t genEventSumw = get_genEventSumw(file);
  df = df.Define("genEventSumw", [genEventSumw]() { return genEventSumw; }, {})
           .Define("EventWeight_lumi18",
                   [genEventSumw](Float_t overalW) {
                     return 18.063 * overalW * 1000 / genEventSumw;
                   },
                   {"overallEventWeight"})
           .Define("EventWeight_lumi138",
                   [genEventSumw](Float_t overalW) {
                     return 138. * overalW * 1000 / genEventSumw;
                   },
                   {"overallEventWeight"})
           .Define("EventWeight_lumi9",
                   [genEventSumw](Float_t overalW) {
                     return 9.693 * overalW * 1000 / genEventSumw;
                   },
                   {"overallEventWeight"})
           .Define("EventWeight_lumi250",
                   [genEventSumw](Float_t overalW) {
                     return 250 * overalW * 1000 / genEventSumw;
                   },
                   {"overallEventWeight"})
           .Define("EventWeight_lumi300",
                   [genEventSumw](Float_t overalW) {
                     return 300 * overalW * 1000 / genEventSumw;
                   },
                   {"overallEventWeight"})
           .Define("EventWeight_lumi350",
                   [genEventSumw](Float_t overalW) {
                     return 300 * overalW * 1000 / genEventSumw;
                   },
                   {"overallEventWeight"})
           .Define("production_mode", [mode]() { return mode; }, {})
           .Define("trainWeight",
                   [](Float_t genW, Float_t puW) { return genW * puW; },
                   {"genWeight", "puWeight"})
           .Define("HTXS_stage1_2_cat_pTjet30GeV_label",
                   [mode](int cat) { return STXSStage12Label(cat, mode); },
                   {"HTXS_stage1_2_cat_pTjet30GeV"});
  return df;
}
