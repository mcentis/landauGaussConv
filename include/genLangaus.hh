#ifndef GENLANGAUS_HH
#define GENLANGAUS_HH

#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "iostream"

Double_t landauFun(Double_t* x, Double_t* par)
{
  Double_t mpshift  = -0.22278298;       // Landau maximum location
  Double_t mpc;
  // MP shift correction
  mpc = par[1] - mpshift * par[0]; 

  return par[2] * TMath::Landau(*x,mpc,par[0]) / par[0];
}

TH1F* genLangaus(int nEntries, double lanMPV, double lanWidth, double gausSigma)
{
  TF1* lan = new TF1("lan", landauFun, -20, 1500, 3); // check limits !!!
  lan->SetParameters(lanWidth, lanMPV, 100);

  TF1* gaus = new TF1("gaus", "gaus", -1000, 1000); // check limits !!!
  gaus->SetParameters(100, 0, gausSigma);

  char name[200];
  sprintf(name, "lanGausHist_%d_%.00f_%.00f_%.00f", nEntries, lanMPV, lanWidth, gausSigma);

  //TH1F* lanGausHist = new TH1F(name, "Convolution of Landau and Gaussian distributions;Measured energy deposit [A.U.];Entries", 1520, -20, 1500);
  TH1F* lanGausHist = new TH1F(name, "Convolution of Landau and Gaussian distributions;Measured energy deposit [A.U.];Entries", 300, -50.5, 511.5); // binning from analysis program

  float lanRnd;
  float gausRnd;

  for(int i = 0; i < nEntries; i++)
    {
      lanRnd = lan->GetRandom(); // check seed!!!

      gausRnd = gaus->GetRandom(); // check seed!!!

      lanGausHist->Fill(lanRnd + gausRnd);
    }

  return lanGausHist;
}

#endif // #ifndef GENLANGAUS_HH
