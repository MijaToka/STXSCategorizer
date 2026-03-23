#include <Math/PtEtaPhiM4D.h>
#include <Math/Vector4D.h>
#include <Math/Vector4Dfwd.h>
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterfaceBase.hxx>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <RtypesCore.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

ROOT::RDF::RNode setup() {
  ROOT::RDF::RNode df = ROOT::RDataFrame("Events", "data/ZZ4lAnalysis.root");
  df =
      df.Filter([](Short_t id) { return id > -1; }, {"bestCandIdx"})
          .Filter(
              [](const ROOT::RVec<float> &mass, Short_t idx) {
                return mass[idx] > 105.f && mass[idx] < 160.f;
              },
              {"ZZCand_mass", "bestCandIdx"})
          .Define("ZZVector",
                  [](ROOT::RVec<float> &pt, ROOT::RVec<float> &eta,
                     ROOT::RVec<float> &phi, ROOT::RVec<float> mass,
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
          .Define(
              "LeadingVector",
              [](ROOT::RVec<float> &pt, ROOT::RVec<float> &eta,
                 ROOT::RVec<float> &phi, ROOT::RVec<float> &mass, Short_t idx) {
                return (idx != -1)
                           ? ROOT::Math::PtEtaPhiMVector(pt[idx], eta[idx],
                                                         phi[idx], mass[idx])
                           : ROOT::Math::PtEtaPhiMVector(-999.f, -999.f, -999.f,
                                                         -999.f);
              },
              {"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "JetLeadingIdx"})
          .Define(
              "SubLeadingVector",
              [](ROOT::RVec<float> &pt, ROOT::RVec<float> &eta,
                 ROOT::RVec<float> &phi, ROOT::RVec<float> &mass, Short_t idx) {
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
          .Define("Jet_btagPNetB_filtered",
                  [](ROOT::RVec<Float_t> pt, ROOT::RVec<Bool_t> ZZMask,
                     ROOT::RVec<Float_t> btagPNetB) {
                    ROOT::VecOps::RVec<float> filtered;
                    for (int i = 0; i < pt.size(); i++) {
                      if (pt[i] > 30 && !ZZMask[i]) {
                        filtered.push_back(btagPNetB[i]);
                      }
                    }
                    return filtered;
                  },
                  {"Jet_pt", "Jet_ZZMask", "Jet_btagPNetB"})
          .Define("nBtagged_filtered",
                  [](ROOT::VecOps::RVec<float> bTag) {
                    return ROOT::VecOps::Sum(bTag > 0.2421);
                  },
                  {"Jet_btagPNetB_filtered"});

  df = df.Define("PFMET_pt", "MET_pt");

  return df;
}

void snapshot(ROOT::RDF::RNode df) {
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
      /*
        "LepPt_0", "LepPt_1", "LepPt_2", "LepPt_3", "LepEta_0", "LepEta_1",
        "LepEta_2", "LepEta_3", "LepPhi_0", "LepPhi_1", "LepPhi_2", "LepPhi_3",
        "LepPdgId_0", "LepPdgId_1", "LepPdgId_2", "LepPdgId_3",
      */
      // Extra lepton features
      /*
        "LepPt_4",
        "LepPt_5",
        "LepEta_4",
        "LepEta_5",
        "LepPhi_4",
        "LepPhi_5",
        "LepPdgId_4",
        "LepPdgId_5",
      */
      // Discriminants
      /*
        "DVBF2j_ME", "DVBF1j_ME", "DWHh_ME", "DZHh_ME", "DVBF2j_ME_noC",
        "DVBF1j_ME_noC", "DWHh_ME_noC", "DZHh_ME_noC", "ZZCand_KD",
        */
      // Event weights and label
      /*
        "EventWeight_lumi18", "EventWeight_lumi9", "EventWeight_lumi138",
        "production_mode", "HTXS_stage_0", "overallEventWeight", "genWeight",
        "puWeight", "trainWeight", "HTXS_stage1_2_cat_pTjet30GeV",
        "HTXS_stage1_2_cat_pTjet30GeV_label", "genEventSumw"
      */
  };
  df.Snapshot("Events", "data/snapshot.root", column_to_save);
}

/*std::map<std::string, ROOT::RDataFrame> STXS_step0(ROOT::RDataFrame df) {
  std::map<std::string, ROOT::RDataFrame> output;
  output["VBF_2jet_tagged"] = df.Filter(
      [](Int_t ZZCand_nExtraLep, float DVBF2j_ME, Char_t nCleanedJetsPt30,
         Char_t nBtagged_filtered) {
        return (ZZCand_nExtraLep == 0) && (DVBF2j_ME > 0.5) &&
               ((((nCleanedJetsPt30 == 2) || (nCleanedJetsPt30 == 3)) &&
                 (nBtagged_filtered <= 1)) ||
                ((nCleanedJetsPt30 == 4) && (nBtagged_filtered == 0)));
      },
      {"ZZCand_nExtraLep", "DVBF2j_ME", "nCleanedJetsPt30",
       "nBtagged_filtered"});

  return output;
}*/

void STXS_Filter() {
  auto df = setup();
  snapshot(df);
  TCanvas *canvas = new TCanvas();
  canvas->Divide(2, 1);

  auto histZZmass =
      df.Histo1D({"ZZmass", "ZZCand_mass", 100u, 40., 160.}, "ZZCand_mass");
  auto histZmass =
      df.Histo1D({"Zmass", "ZCand_mass", 100u, 0., 125.}, "ZCand_mass");

  canvas->cd(1);
  histZZmass->Draw();

  canvas->cd(2);
  histZmass->Draw();

  canvas->SaveAs("df_output.pdf");
}
