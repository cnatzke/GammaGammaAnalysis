//////////////////////////////////////////////////////////////////////////////////
// Generates gamma-gamma distributions from histograms created by GenerateHistogram
//
// Author:        Connor Natzke (cnatzke@triumf.ca)
// Creation Date: Tuesday May 12, 2020	T14:41:17-07:00
// Last Update:   Tuesday May 12, 2020	T14:41:17-07:00
// Usage:
//
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TGRSIUtilities.h"
#include "TParserLibrary.h" // needed for GetRunNumber
#include "TMath.h"
#include "TEnv.h"
#include "TList.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TRWPeak.h"

#include "GenerateDistribution.h"
#include "HistogramManager.h"
#include "FitManager.h"

int main(int argc, char **argv) {

	if (argc == 1) { // no inputs given
		PrintUsage(argv);
		return 0;
	}
	else if (argc > 2) {
		std::cerr << "Too many arguments given: "
		          << argc << " given, 1 required.\n" << std::endl;
		PrintUsage(argv);
		return 0;
	}


	HistogramManager *hist_man = new HistogramManager();
	FitManager fit_man;
	int file_type = 0;
	TList *time_random_hist_list;
	TList *event_mixed_hist_list;
	std::vector<TH1D*> time_random_projection_vec;
	std::vector<TH1D*> time_random_clone_vec;
	std::vector<TH1D*> event_mixed_projection_vec;
	std::vector<TH1D*> event_mixed_clone_vec;

	InitializeGRSIEnv();
	file_type = AutoFileDetect(argv[1]);
	// use for multiple input files
	//for (auto i = 1; i < argc; i++) AutoFileDetect(argv[i]);

	if (file_type == 1) {
		time_random_hist_list = hist_man->LoadHistograms(argv[1], "TimeRandomSubtacted");
		event_mixed_hist_list = hist_man->LoadHistograms(argv[1], "EventMixed");
	} else {
		return 1;
	}

	float fit_low = 1162;
	float fit_high = 1182;
	time_random_projection_vec = fit_man.GenerateProjections(time_random_hist_list, 1300, 1500, fit_low, fit_high); // 1332 keV Gate, 1172 fit
	event_mixed_projection_vec = fit_man.GenerateProjections(event_mixed_hist_list, 1300, 1500, fit_low, fit_high); // 1332 keV Gate, 1172 fit

	time_random_clone_vec = fit_man.CloneProjections(time_random_projection_vec, fit_low, fit_high);
	event_mixed_clone_vec = fit_man.CloneProjections(event_mixed_projection_vec, fit_low, fit_high);

	PlotDistribution(time_random_clone_vec, event_mixed_clone_vec, fit_low, fit_high);

	return 0;
} // main

/******************************************************************************
 * Detects type of input file
 *
 * @param fileName  Name of input file
 *****************************************************************************/
int PlotDistribution(std::vector<TH1D*> correlatedHists, std::vector<TH1D*> eventMixedHists, float fitLow, float fitHigh){
	FitManager fit_man;
	double chi2;
	double chi2_uncorr;
	TRWPeak *fitted_peak;
	TRWPeak *fitted_peak_uncorr;
	TGraphErrors *gg_corr = new TGraphErrors(correlatedHists.size());
	TGraphErrors *gg_uncorr = new TGraphErrors(eventMixedHists.size());
	std::ofstream bad_fits_file("bad_fits.txt");
	std::ofstream fits_file("fits.txt");

	std::vector<double> fAngleCombinations = {15.442, 21.9054, 29.1432, 33.1433, 38.382, 44.57, 47.4453, 48.7411, 51.4734, 55.1704, 59.9782, 60.1024, 62.3396, 62.4924, 63.4231, 68.9567, 71.4314, 73.3582, 73.6291, 75.7736, 80.9423, 81.5464, 83.8936, 86.868, 88.9658, 91.0342, 93.132, 96.1064, 98.4536, 99.0577, 104.226, 106.371, 106.642, 108.569, 111.043, 116.577, 117.508, 117.66, 119.898, 120.022, 124.83, 128.527, 131.259, 132.555, 135.43, 141.618, 146.857, 150.857, 158.095, 164.558, 180.0};

	// setting header for fits Data
	fits_file << "Angle, Corr Area, Corr Area Errror, Uncorr Area, Uncorr Area Error" << std::endl;

	TFile out_file("ProjectedHistograms.root", "RECREATE");
	for (unsigned int i = 0; i < correlatedHists.size(); i++) {
		fitted_peak = fit_man.FitPeak(correlatedHists.at(i), 1172, 1162, 1182);
		fitted_peak_uncorr = fit_man.FitPeak(eventMixedHists.at(i), 1172, 1162, 1182);

		if (i == 0) {
			correlatedHists.at(i)->Write("total_proj");
			eventMixedHists.at(i)->Write("total_proj_uncorr");
		} else {
			correlatedHists.at(i)->Write(Form("corr_%i", i));
			eventMixedHists.at(i)->Write(Form("uncorr_%i", i));
		}

		// writing areas to file
		if (i ==0 ) {
			fits_file << "total, "
			          << fitted_peak->Area() << ", " << fitted_peak->AreaErr() << ", "
			          << fitted_peak_uncorr->Area() << ", " << fitted_peak_uncorr->AreaErr()
			          << std::endl;
		} else {
			fits_file << fAngleCombinations.at(i - 1) << ", "
			          << fitted_peak->Area() << ", " << fitted_peak->AreaErr() << ", "
			          << fitted_peak_uncorr->Area() << ", " << fitted_peak_uncorr->AreaErr()
			          << std::endl;
		}

		chi2 = fitted_peak->GetChi2() / fitted_peak->GetNDF();
		chi2_uncorr = fitted_peak_uncorr->GetChi2() / fitted_peak_uncorr->GetNDF();
		if (chi2 > 10) {
			if (i ==0 ) {
				bad_fits_file << "Total projection" << std::endl;
			} else {
				bad_fits_file << i << " corr " << std::endl;
			}
		}

		if (chi2_uncorr > 10) {
			if (i ==0 ) {
				bad_fits_file << "Total projection uncorr" << std::endl;
			} else {
				bad_fits_file << i << " uncorr " << std::endl;
			}
		}

		// adding points to plot
		gg_corr->SetPoint(i, i, fitted_peak->Area());
		gg_corr->SetPointError(i, 0., fitted_peak->AreaErr());

		gg_uncorr->SetPoint(i, i, fitted_peak_uncorr->Area());
		gg_uncorr->SetPointError(i, 0., fitted_peak_uncorr->AreaErr());

	}
	gg_corr->Write("gg_corr");
	gg_uncorr->Write("gg_uncorr");
	out_file.Close();
	fits_file.close();
	bad_fits_file.close();

	return 1;

} // PlotDistribution

/******************************************************************************
 * Detects type of input file
 *
 * @param fileName  Name of input file
 *****************************************************************************/
int AutoFileDetect(std::string fileName){
	size_t dot_pos = fileName.find_last_of('.');
	std::string ext = fileName.substr(dot_pos + 1);

	if (ext == "root") {
		//OpenRootFile(fileName);
		return 1;
	} else {
		std::cerr << "Discarding unknown file: " << fileName.c_str() << std::endl;
		return 0;
	}
} // End AutoFileDetect

/************************************************************//**
 * Opens Root files
 *
 * @param fileName  Name of ROOT file
 ***************************************************************/
/*
   void OpenRootFile(std::string fileName){
   TFile* in_file = new TFile(fileName.c_str());
   if (in_file->IsOpen()) {
       std::cout << "Opened ROOT file: " << in_file->GetName() << std::endl;
       HistogramManager* hist_man = new HistogramManager();
       TList *trsub_hist_list = hist_man->LoadHistograms(in_file, "TimeRandomSubtacted");

       TH2D* test_hist = (TH2D*)trsub_hist_list->FindObject("gammaGammaSub30");
       TCanvas *c1 = new TCanvas("c1","c1",800,650);
       c1->cd();
       test_hist->Draw("colz");
       c1->SetLogz();
       c1->Update();
       c1->Print("test.png");
       //std::cout << "Press any key to continue ..." << std::endl;
       //std::cin.get();

       // cleaning up
       delete trsub_hist_list;
       delete hist_man;
   } else {
       std::cerr << "Could not open ROOT file: " << in_file->GetName() << std::endl;
   }
   } // end OpenRootFile
 */

/******************************************************************************
 * Initializes GRSISort environment
 *****************************************************************************/
void InitializeGRSIEnv(){
	std::string grsi_path = getenv("GRSISYS");
	if(grsi_path.length() > 0) {
		grsi_path += "/";
	}
	grsi_path += ".grsirc";
	gEnv->ReadFile(grsi_path.c_str(), kEnvChange);

	// suppressed warnings, need to test if actyally needed
	//TParserLibrary::Get()->Load();
} // InitializeGRSIEnv

/******************************************************************************
 * Prints usage message and version
 *****************************************************************************/
void PrintUsage(char* argv[]){
	std::cerr << argv[0] << " Version: " << GenerateDistribution_VERSION_MAJOR
	          << "." << GenerateDistribution_VERSION_MINOR << "\n"
	          << "usage: " << argv[0] << " histogram_file\n"
	          << " histogram_file:     File containing gamma-gamma angular matrices (must end with .root)"
	          << std::endl;
} // end PrintUsage
