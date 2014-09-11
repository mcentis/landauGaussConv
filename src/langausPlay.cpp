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
  TDirectory* histDir = outFile->mkdir("Histos_different_sigma");
  histDir->cd();

  TCanvas* servCan = new TCanvas(); // used for fitting and drawing stuff

  // ---------------------------------- fix landau mpv and width, vary the gauss sigma -------------------

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
      if(gausSig == 0) gausSig = 0.01; // gaus with 0 sigma creates problems

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

  outFile->cd();
  fitMPVvsGenSig->Write();
  fitGsigvsGenSig->Write();
  fitWidthvsGenSig->Write();
  fitMaxvsGenSig->Write();

  // ------------------------------ fix all parameters, fit many distributions and look for correlations in the fitted values ----------------

  histDir = outFile->mkdir("Histos_same_parameters");
  histDir->cd();

  lanMPV = 30;
  lanWidth = 2;
  gausSig = 5;

  sprintf(name, "corrGsigma_lanWidth_genLanMPV_%.00f_genLanWidth_%.00f_genGsigma_%.00f", lanMPV, lanWidth, gausSig);
  sprintf(title, "Fitted Landau width vs Gaus sigma, generator: lanMPV %.00f, lanWidth %.00f, Gsig %.00f", lanMPV, lanWidth, gausSig);
  TGraphErrors* corrGsigLanW = new TGraphErrors();
  corrGsigLanW->SetName(name);
  corrGsigLanW->SetTitle(title);

  sprintf(name, "corrGsigma_lanMPV_genLanMPV_%.00f_genLanWidth_%.00f_genGsigma_%.00f", lanMPV, lanWidth, gausSig);
  sprintf(title, "Fitted Landau MPV vs Gaus sigma, generator: lanMPV %.00f, lanWidth %.00f, Gsig %.00f", lanMPV, lanWidth, gausSig);
  TGraphErrors* corrGsigLanMPV = new TGraphErrors();
  corrGsigLanMPV->SetName(name);
  corrGsigLanMPV->SetTitle(title);

  sprintf(name, "corrLanMPV_lanWidth_genLanMPV_%.00f_genLanWidth_%.00f_genGsigma_%.00f", lanMPV, lanWidth, gausSig);
  sprintf(title, "Fitted Landau MPV vs Landau width, generator: lanMPV %.00f, lanWidth %.00f, Gsig %.00f", lanMPV, lanWidth, gausSig);
  TGraphErrors* corrLanMPVLanW = new TGraphErrors();
  corrLanMPVLanW->SetName(name);
  corrLanMPVLanW->SetTitle(title);

  sprintf(name, "distrFitGsig_genLanMPV_%.00f_genLanWidth_%.00f_genGsigma_%.00f", lanMPV, lanWidth, gausSig);
  sprintf(title, "Fitted Gaus sigma, generator: lanMPV %.00f, lanWidth %.00f, Gsig %.00f", lanMPV, lanWidth, gausSig);
  TH1D* distrFitGsig = new TH1D(name, title, 150, 0, 15);

  sprintf(name, "distrFitLanW_genLanMPV_%.00f_genLanWidth_%.00f_genGsigma_%.00f", lanMPV, lanWidth, gausSig);
  sprintf(title, "Fitted Landau width, generator: lanMPV %.00f, lanWidth %.00f, Gsig %.00f", lanMPV, lanWidth, gausSig);
  TH1D* distrFitLanW = new TH1D(name, title, 150, 0, 15);

  sprintf(name, "distrFitMPV_genLanMPV_%.00f_genLanWidth_%.00f_genGsigma_%.00f", lanMPV, lanWidth, gausSig);
  sprintf(title, "Fitted Landau MPV, generator: lanMPV %.00f, lanWidth %.00f, Gsig %.00f", lanMPV, lanWidth, gausSig);
  TH1D* distrFitLanMPV = new TH1D(name, title, 500, 0, 50);

  double fitSig; // fitted parameters
  double fitMPV;
  double fitW;
  double EfitSig; // fitted parameters errors
  double EfitMPV;
  double EfitW;

  for(int i = 0; i < 1000; ++i)
    {
      hist = genLangaus(nEntries, lanMPV, lanWidth, gausSig);
      fit = lanGausFit(hist, 10, 100); // warning: fixed parameters!!!!
      hist->Write();
      delete hist;

      fitW = fit->GetParameter(0);
      EfitW = fit->GetParError(0);
      fitMPV = fit->GetParameter(1);
      EfitMPV = fit->GetParError(1);
      fitSig = fit->GetParameter(3);
      EfitSig = fit->GetParError(3);

      distrFitGsig->Fill(fitSig);
      distrFitLanW->Fill(fitW);
      distrFitLanMPV->Fill(fitMPV);

      corrGsigLanW->SetPoint(i, fitSig, fitW);
      corrGsigLanW->SetPointError(i, EfitSig, EfitW);

      corrGsigLanMPV->SetPoint(i, fitSig, fitMPV);
      corrGsigLanMPV->SetPointError(i, EfitSig, EfitMPV);

      corrLanMPVLanW->SetPoint(i, fitW, fitMPV);
      corrLanMPVLanW->SetPointError(i, EfitW, EfitMPV);
    }

  corrGsigLanW->Draw("AP");
  corrGsigLanW->GetXaxis()->SetTitle("Fitted Gauss sigma [A. U.]");
  corrGsigLanW->GetYaxis()->SetTitle("Fitted Landau width [A. U.]");

  corrGsigLanMPV->Draw("AP");
  corrGsigLanMPV->GetXaxis()->SetTitle("Fitted Gauss sigma [A. U.]");
  corrGsigLanMPV->GetYaxis()->SetTitle("Fitted Landau MPV [A. U.]");

  corrLanMPVLanW->Draw("AP");
  corrLanMPVLanW->GetXaxis()->SetTitle("Fitted Landau width [A. U.]");
  corrLanMPVLanW->GetYaxis()->SetTitle("Fitted Landau MPV [A. U.]");

  outFile->cd();
  distrFitGsig->Write();
  distrFitLanW->Write();
  distrFitLanMPV->Write();
  corrGsigLanW->Write();
  corrGsigLanMPV->Write();
  corrLanMPVLanW->Write();

  delete servCan;

  outFile->Close();

  return 0;
}
