//g++ gamma_gate.cxx -std=c++0x `root-config --cflags --libs` -lTreePlayer -lMathMore -lSpectrum -lMinuit -lPyROOT -o GammaGate


#include <iostream>
#include <iomanip>
#include <fstream>
#include "TCutG.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TList.h"
#include "TRandom.h"
#include "/opt/root_v6.14.06/include/Math/MinimizerOptions.h"
using namespace std;
double peak_area(TF1 *fun, int gatesize){
  TF1 * fitresult = new TF1("fitresult","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4])))");
  for(int i = 0; i < 5; i++){
    fitresult->SetParameter(i,fun->GetParameter(i));
  }
  double bmin = fun->GetParameter(1) - gatesize;
  double bmax = fun->GetParameter(1) + gatesize;
  double peakarea = fitresult->Integral(bmin,bmax);
  return peakarea;
}

void gamma_check(char const * infile, int gate, int peak) {
  //gROOT->SetBatch(kTRUE);
  TCanvas *c1 = new TCanvas();
  c1->Draw();
c1->cd();
  int gatesize = 2;
  TList * list = new TList;

  TFile * inputfile = new TFile(infile, "READ");
  if (!inputfile->IsOpen()) {
    printf("Opening file failed, aborting\n");
    return;
  }

  TH2D *gg;
  gg = (TH2D*)inputfile->Get("gen"); 
  list->Add(gg);
  TH1D *gg_pro = new TH1D("gg_pro","Full Matrix Projection",4000,0,2000);
  list->Add(gg_pro);
  TH1D *gg_gated = new TH1D("gg_gated","Gamma Gated Spectrum",4000,0,2000);
  list->Add(gg_gated);
  TH1D *gg_back = new TH1D("gg_back","Unscaled Background Spectrum",4000,0,2000);
  list->Add(gg_back);
  TH1D *gg_sub = new TH1D("gg_sub", "Gamma Gated Background Subtracted Spectrum",4000,0,2000);
  list->Add(gg_sub);

  gg->ProjectionY("gg_pro",0,-1);
 
 // ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(50000);

  TF1 *f1 = new TF1("f1","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4]))) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",gate-8,gate+6);
  f1->SetParameters(1e6,gate,0.5,1,0.1,100,200);
  f1->SetParLimits(1,gate-2,gate+2);
  f1->SetParLimits(2,0.5,5);
  f1->SetParLimits(3,0.01,100);
  f1->SetParLimits(4,0.0,1);
  gg_pro->Fit("f1","QR");

  TAxis *axis = gg_pro->GetXaxis();
  int bmin = axis->FindBin(f1->GetParameter(1)-gatesize);
  int bmax = axis->FindBin(f1->GetParameter(1)+gatesize);

  gg->ProjectionY("gg_gated",axis->FindBin(f1->GetParameter(1)-gatesize),axis->FindBin(f1->GetParameter(1)+gatesize));
  gg->ProjectionY("gg_sub",axis->FindBin(f1->GetParameter(1)-gatesize),axis->FindBin(f1->GetParameter(1)+gatesize));
  gg->ProjectionY("gg_back",axis->FindBin(f1->GetParameter(1)+gatesize+4),-1);

  double apeak = peak_area(f1,gatesize*f1->GetParameter(2));
  double agate = gg_pro->Integral(bmin,bmax);
  double bg = 1 - apeak/agate;
  double gated_counts = gg_gated->Integral(0,-1);
  double back_counts = gg_back->Integral(0,-1); 
  gg_back->Scale(0.8*bg*(gated_counts/back_counts));
  gg_sub->Add(gg_back,-1);
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(500000);
  TF1 *f2 = new TF1("f2","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4]))) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",peak-8,peak+10);
  f2->SetParameters(1e6,peak,0.5,1,0.1,100,200);
  f2->SetParLimits(1,peak-2,peak+2);
  f2->SetParLimits(2,0.5,5);
  f2->SetParLimits(3,0.01,100);
  f2->SetParLimits(4,0.0,1);
  gg_sub->Fit("f2","qR");

  double peakarea = f2->Integral(f2->GetParameter(1) - gatesize, f2->GetParameter(1) + gatesize);
  double peakerror =f2->IntegralError(f2->GetParameter(1) - gatesize, f2->GetParameter(1) + gatesize);
  double peakArea = peak_area(f2,gatesize);
  double peakError = (peakerror/peakarea) * peakArea;

/*  string resultfile = "/tig/smilodon_data2/sgillespie/Charlotte/fitresults.txt";
  ifstream test(resultfile.c_str());
  ofstream ffile;
  if(test.good()) {
    ffile.open(resultfile.c_str(),ios::app);
  }
  else {
    ffile.open(resultfile.c_str());
  }
  string tempstring = infile;
  std::size_t first= tempstring.find_first_of("0123456789");
  std::size_t last= tempstring.find_last_of("0123456789");
  string angle = tempstring.substr(first,last+1);
  ffile << angle << "\t" << peakArea << "\t" << peakError << endl;
  ffile.close();*/

  string outfile = infile;
  string str = "-fits.root";
  outfile.resize(outfile.size()-5);
  outfile.append(str);
  cout << outfile << endl;
  TFile * myfile = new TFile(outfile.c_str(), "RECREATE");
  myfile->cd();
  list->Write();
  myfile->Close();
}

int main(int argc, char ** argv) {

  char const * hfile;
  int gate;
  int pfit;
//  printf("Starting sortcode\n");

  // Input-chain-file, output-histogram-file
  if (argc != 4) {
	  cout << "Insufficient arguments, histogram, gamma gate and peak to fit" << endl;
	  return 0;
  } else {
	  hfile = argv[1];
	  gate = atoi(argv[2]);
	  pfit = atoi(argv[3]);
  } 

//  printf("Input file:%s\nGating on: %i\nFitting: %i\n", hfile, gate, pfit);

  gamma_check(hfile, gate, pfit);

  return 0;
}
