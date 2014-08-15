#include "TSystem.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TDirectory.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TAxis.h"

#include "iostream"

#include "genLangaus.hh"
#include "langauFit.hh"

int main(int argc, char* argv[])
{
  if(argc != 2)
    {
      std::cout << "\tusage: langausPlay outFile" << std::endl;
      return 1;
    }

  // constants
  const int nEntries = 10000;

  // variables
  double lanMPV;
  double lanWidth;
  double gausSig;
  TH1* hist;
  TF1* fit;
  char name[200];
  char title[400];
  int nPoint;

  // big axis labels
  gStyle->SetLabelSize(0.05, "x");
  gStyle->SetLabelSize(0.05, "y");

  gStyle->SetTitleSize(0.05, "x");
  gStyle->SetTitleSize(0.05, "y");

  gStyle->SetTitleOffset(0.95, "x");
  gStyle->SetTitleOffset(0.95, "y");

  gStyle->SetMarkerSize(2);
  gStyle->SetLineWidth(1);

  TFile* outFile = new TFile(argv[1], "RECREATE");
  TDirectory* histDir = outFile->mkdir("histograms");
  histDir->cd();

  TCanvas* servCan = new TCanvas(); // used for fitting and drawing stuff

  lanMPV = 40;
  lanWidth = 4;

  sprintf(name, "fitLanMPV_genLanMPV_%.00f_genLanWidth_%.00f", lanMPV, lanWidth);
  sprintf(title, "Fitted Landau MPV vs generator gaus sigma, generator: lanMPV %.00f, lanWidth %.00f", lanMPV, lanWidth);
  TGraphErrors* fitMPVvsGenSig = new TGraphErrors();
  fitMPVvsGenSig->SetName(name);
  fitMPVvsGenSig->SetTitle(title);

  sprintf(name, "fitGausSig_genLanMPV_%.00f_genLanWidth_%.00f", lanMPV, lanWidth);
  sprintf(title, "Fitted Gaus sigma vs generator gaus sigma, generator: lanMPV %.00f, lanWidth %.00f", lanMPV, lanWidth);
  TGraphErrors* fitGsigvsGenSig = new TGraphErrors();
  fitGsigvsGenSig->SetName(name);
  fitGsigvsGenSig->SetTitle(title);

  sprintf(name, "fitLanW_genLanMPV_%.00f_genLanWidth_%.00f", lanMPV, lanWidth);
  sprintf(title, "Fitted Landau width vs generator gaus sigma, generator: lanMPV %.00f, lanWidth %.00f", lanMPV, lanWidth);
  TGraphErrors* fitWidthvsGenSig = new TGraphErrors();
  fitWidthvsGenSig->SetName(name);
  fitWidthvsGenSig->SetTitle(title);

  sprintf(name, "fitMax_genLanMPV_%.00f_genLanWidth_%.00f", lanMPV, lanWidth);
  sprintf(title, "Max of the fit function vs generator gaus sigma, generator: lanMPV %.00f, lanWidth %.00f", lanMPV, lanWidth);
  TGraphErrors* fitMaxvsGenSig = new TGraphErrors();
  fitMaxvsGenSig->SetName(name);
  fitMaxvsGenSig->SetTitle(title);

  for(int i = 0; i < 10; ++i)
    {
      gausSig = i * 2;
      hist = genLangaus(nEntries, lanMPV, lanWidth, gausSig);
      fit = lanGausFit(hist, 10, 100); // warning: fixed parameters!!!!
      hist->Write();

      nPoint = fitMPVvsGenSig->GetN();
      fitMPVvsGenSig->SetPoint(nPoint, gausSig, fit->GetParameter(1));
      fitMPVvsGenSig->SetPointError(nPoint, 0, fit->GetParError(1));

      fitGsigvsGenSig->SetPoint(nPoint, gausSig, fit->GetParameter(3));
      fitGsigvsGenSig->SetPointError(nPoint, 0, fit->GetParError(3));

      fitWidthvsGenSig->SetPoint(nPoint, gausSig, fit->GetParameter(0));
      fitWidthvsGenSig->SetPointError(nPoint, 0, fit->GetParError(0));

      fitMaxvsGenSig->SetPoint(nPoint, gausSig, fit->GetMaximumX());
    }

  fitMPVvsGenSig->Draw("AP");
  fitMPVvsGenSig->GetXaxis()->SetTitle("Generator Gauss sigma [A. U.]");
  fitMPVvsGenSig->GetYaxis()->SetTitle("Fitted Landau MPV [A. U.]");

  fitGsigvsGenSig->Draw("AP");
  fitGsigvsGenSig->GetXaxis()->SetTitle("Generator Gauss sigma [A. U.]");
  fitGsigvsGenSig->GetYaxis()->SetTitle("Fitted Gauss sigma [A. U.]");

  fitWidthvsGenSig->Draw("AP");
  fitWidthvsGenSig->GetXaxis()->SetTitle("Generator Gauss sigma [A. U.]");
  fitWidthvsGenSig->GetYaxis()->SetTitle("Fitted Landau width [A. U.]");

  fitMaxvsGenSig->Draw("AP");
  fitMaxvsGenSig->GetXaxis()->SetTitle("Generator Gauss sigma [A. U.]");
  fitMaxvsGenSig->GetYaxis()->SetTitle("Max of the fit function [A. U.]");

  delete servCan;

  outFile->cd();
  fitMPVvsGenSig->Write();
  fitGsigvsGenSig->Write();
  fitWidthvsGenSig->Write();
  fitMaxvsGenSig->Write();

  outFile->Close();

  return 0;
}
