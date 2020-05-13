//g++ gamma_gate_mix.cxx -std=c++0x `root-config --cflags --libs` -lTreePlayer -lMathMore -lSpectrum -lMinuit -lPyROOT -o GammaGate


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
	for(int i = 0; i < 5; i++) {
		fitresult->SetParameter(i,fun->GetParameter(i));
	}
	double bmin = fun->GetParameter(1) - gatesize;
	double bmax = fun->GetParameter(1) + gatesize;
	double peakarea = fitresult->Integral(bmin,bmax);
	return peakarea;
}

void gamma_check(char const * infile, char const *ofile, int gate, int peak) {

	//---------------------------------
	//CHANGE ME!
	int gatesize = 1;
	//int gatesize = 4;
	//double gatesize = 2.5;
	//---------------------------------

	TList * list = new TList;

	TFile * inputfile = new TFile(infile, "READ");
	if (!inputfile->IsOpen()) {
		printf("Opening file failed, aborting\n");
		return;
	}

	TH2D *gg;
	gg = (TH2D*)inputfile->Get("gen");
	list->Add(gg);
	TH1D *gg_pro = new TH1D("gg_pro","Full Matrix Projection",2048,0,2048);
	list->Add(gg_pro);
	TH1D *gg_gated = new TH1D("gg_gated","Gamma Gated Spectrum",2048,0,2048);
	list->Add(gg_gated);
	TH1D *gg_back = new TH1D("gg_back","Unscaled Background Spectrum",2048,0,2048);
	list->Add(gg_back);
	TH1D *gg_sub = new TH1D("gg_sub", "Gamma Gated Background Subtracted Spectrum",2048,0,2048);
	list->Add(gg_sub);

	gg->ProjectionY("gg_pro",0,-1);

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(500000);

	TF1 *f1 = new TF1("f1","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4]))) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",gate-6,gate+8);
	f1->SetParameters(1e6,gate,0.5,1,0.1,100,200);
	f1->SetParLimits(1,gate-2,gate+2);
	f1->SetParLimits(2,0.5,5);
	f1->SetParLimits(3,0.01,100);
	f1->SetParLimits(4,0.0,1);
	gg_pro->Fit("f1","R");

	TAxis *axis = gg_pro->GetXaxis();
	int bmin = axis->FindBin(f1->GetParameter(1)-gatesize*f1->GetParameter(2));
	int bmax = axis->FindBin(f1->GetParameter(1)+gatesize*f1->GetParameter(2));

	gg->ProjectionY("gg_gated",axis->FindBin(f1->GetParameter(1)-gatesize*f1->GetParameter(2)),axis->FindBin(f1->GetParameter(1)+gatesize*f1->GetParameter(2)));
	gg->ProjectionY("gg_sub",axis->FindBin(f1->GetParameter(1)-gatesize*f1->GetParameter(2)),axis->FindBin(f1->GetParameter(1)+gatesize*f1->GetParameter(2)));
	//gg->ProjectionY("gg_back",axis->FindBin(f1->GetParameter(1)+gatesize*f1->GetParameter(2)+4),-1);
	gg->ProjectionY("gg_back",axis->FindBin(f1->GetParameter(1)-gatesize*f1->GetParameter(2)+3),axis->FindBin(f1->GetParameter(1)+gatesize*f1->GetParameter(2)+3));

	//gg_gated->Fit("f1","QR");
	double apeak = peak_area(f1,gatesize*f1->GetParameter(2));
	double agate = gg_pro->Integral(bmin,bmax);
	double bg = 1 - apeak/agate;
	double gated_counts = gg_gated->Integral(0,-1);
	double back_counts = gg_back->Integral(0,-1);
	f1->SetParameter(1,gate);
	f1->SetParameter(2,1);
	cout << axis->FindBin(f1->GetParameter(1)-gatesize*f1->GetParameter(2)) << "\t" <<  axis->FindBin(f1->GetParameter(1)+gatesize*f1->GetParameter(2)) << endl;
	cout << axis->FindBin(f1->GetParameter(1)-gatesize*f1->GetParameter(2)+3) << "\t" <<  axis->FindBin(f1->GetParameter(1)+gatesize*f1->GetParameter(2)+3) << endl;

	// gg_back->Scale(bg*(gated_counts/back_counts));
	//gg_back->Fit("f1","QR");
	//double back_counts = peak_area(f1,gatesize*f1->GetParameter(2));
	gg_sub->Add(gg_back,-1);
//  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(500000);

	TF1 *f2 = new TF1("f2","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4]))) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",peak-8,peak+10);
	f2->SetParameters(1e6,peak,0.5,1,0.1,100,200);
	f2->SetParLimits(1,peak-2,peak+2);
	f2->SetParLimits(2,0.5,5);
	f2->SetParLimits(3,0.01,100);
	f2->SetParLimits(4,0.0,1);
	// gg_sub->Fit("f2","QR");
	//gg_gated->Fit("f2","QR");

//  double peakarea = f2->Integral(f2->GetParameter(1) - gatesize*f2->GetParameter(2), f2->GetParameter(1) + gatesize*f2->GetParameter(2));
//  double peakerror =f2->IntegralError(f2->GetParameter(1) - gatesize*f2->GetParameter(2), f2->GetParameter(1) + gatesize*f2->GetParameter(2));
//  double peakArea = peak_area(f2,gatesize*f2->GetParameter(2));
//  double peakError = (peakerror/peakarea) * peakArea;


/*  string resultfile = ofile;
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
   string tempangle = tempstring.substr(last-1,last+1);
   std::size_t first2= tempangle.find_first_of("0123456789");
   std::size_t last2= tempangle.find_last_of("0123456789");
    //string angle
    //if(first2 == last2) {
    //	angle = tempangle.substr(first2,last2);
    //} else {
        string angle = tempangle.substr(first2,last2+1);
    //} //ALL OF THIS STUFF BREAKS, DON'T BOTHER!


   ffile << angle << "\t" << peakArea << "\t" << peakError << "\t"
                                 << //(gated_counts/back_counts) <<
                                        endl;
   ffile.close();

   string outfile = infile;
   string str = "-fits.root";
   outfile.resize(outfile.size()-5);
   outfile.append(str);*/
	string outfile = "gTest.root";
	TFile * myfile = new TFile(outfile.c_str(), "RECREATE");
	myfile->cd();
	list->Write();
	myfile->Close();
}

int main(int argc, char ** argv) {

	char const * hfile;
	char const * ofile;
	int gate;
	int pfit;
//  printf("Starting sortcode\n");

	//This fixes the TCanvas output:
	TCanvas *c1 = new TCanvas();
	c1->Draw();
	c1->cd();


	// Input-chain-file, output-histogram-file
	if (argc != 5) {
		cout << "Insufficient arguments: histogram, outfile, gamma gate and peak to fit" << endl;
		return 0;
	} else {
		hfile = argv[1];
		ofile = argv[2];
		gate = atoi(argv[3]);
		pfit = atoi(argv[4]);
	}

	//printf("Input file:%s\nGating on: %i\nFitting: %i\n", hfile, gate, pfit);

	gamma_check(hfile, ofile, gate, pfit);

	return 0;
}
