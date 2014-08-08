#ifndef GENLANGAUS_HH
#define GENLANGAUS_HH

#include "TH1.h"
#include "TF1.h"
#include "iostream"

TH1F* genLangaus(int nEntries, double lanMPV, double lanWidth, double gausSigma)
{
  TF1* lan = new TF1("lan", "landau", -20, 1500); // check limits !!!
  lan->SetParameters(100, lanMPV, lanWidth); // parameters: constant (as for gaus), mpv, width

  TF1* gaus = new TF1("gaus", "gaus", -500, 500); // check limits !!!
  gaus->SetParameters(100, 0, 3);

  char name[200];
  sprintf(name, "lanGausHist_%d_%.00f_%.00f_%.00f", nEntries, lanMPV, lanWidth, gausSigma);

  TH1F* lanGausHist = new TH1F(name, "Convolution of Landau and Gaussian distributions;Measured energy deposit [A.U.];Entries", 1520, -20, 1500);

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
