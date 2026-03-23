#include <ROOT/RDataFrame.hxx>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <cstdlib>

void STXS_Filter() {
  ROOT::RDataFrame df = ROOT::RDataFrame("Events", "data/ZZ4lAnalysis.root");

  TCanvas *canvas = new TCanvas();
  canvas->Divide(2, 1);

  auto histZZmass = df.Histo1D("ZZCand_mass");
  auto histZmass = df.Histo1D("ZCand_mass");

  canvas->cd(1);
  histZZmass->Draw();

  canvas->cd(2);
  histZmass->Draw();

  canvas->SaveAs("df_output.pdf");
}
