#include "TSystem.h"
#include "TFile.h"
#include "TH1F.h"

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
  outFile->cd();
  TH1* hist;

  lanMPV = 40;
  lanWidth = 4;

  for(int i = 0; i < 10; ++i)
    {
      gausSig = i * 2;
      hist = genLangaus(nEntries, lanMPV, lanWidth, gausSig);
      hist->Write();
    }

  outFile->Close();

  return 0;
}
