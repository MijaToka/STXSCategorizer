#include <TApplication.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TTree.h>
#include <cstdlib>
#include <iostream>

void STXS_script() {

  TFile *file = TFile::Open("data/ZZ4lAnalysis.root", "READ");
  if (!file || file->IsZombie()) {
    std::cerr << "Could not open the file" << std::endl;
    exit(EXIT_FAILURE);
  }

  TTree *tree = (TTree *)file->Get("Events");
  if (!tree) {
    std::cerr << "Could not get TTree" << std::endl;
    exit(EXIT_FAILURE);
  }

  float ZZCand_mass, ZCand_mass;
  tree->SetBranchAddress("ZZCand_mass", &ZZCand_mass);
  tree->SetBranchAddress("ZCand_mass", &ZCand_mass);

  TCanvas *canvas = new TCanvas();
  TH1F *histZZmass =
      new TH1F("ZZCand_mass_hist", "ZZCand_mass_hist", 100, 0, 240);
  TH1F *histZmass = new TH1F("ZCand_mass_hist", "ZCand_mass_hist", 100, 0, 125);

  int entries = tree->GetEntries();

  for (int i = 0; i < entries; i++) {
    tree->GetEntry(i);
    histZZmass->Fill(ZZCand_mass);
    histZmass->Fill(ZCand_mass);
  }

  canvas->Divide(2, 1);
  canvas->cd(1);
  histZZmass->Draw();

  canvas->cd(2);
  histZmass->Draw();

  canvas->SaveAs("output.pdf");
  canvas->Update();

  file->Close();
}
