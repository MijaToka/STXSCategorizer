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

  TCanvas *canvas = new TCanvas();
  canvas->Divide(2, 1);

  canvas->cd(1);
  tree->Draw("ZZCand_mass");

  canvas->cd(2);
  tree->Draw("ZCand_mass");

  canvas->SaveAs("output.pdf");
  canvas->Update();

  file->Close();
}
