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
#include "TFile.h"
#include "TGRSIUtilities.h"
#include "TParserLibrary.h" // needed for GetRunNumber
#include "TMath.h"
#include "TParserLibrary.h"
#include "TEnv.h"
#include "TList.h"
#include "TH2.h"
#include "TCanvas.h"

#include "GenerateDistribution.h"
#include "HistogramManager.h"

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
	int file_type = 0;
	TList *time_random_hist_list;

	InitializeGRSIEnv();
	file_type = AutoFileDetect(argv[1]);
	// use for multiple input files
	//for (auto i = 1; i < argc; i++) AutoFileDetect(argv[i]);

	if (file_type == 1){
		time_random_hist_list = hist_man->LoadHistograms(argv[1], "TimeRandomSubtacted");
	} else {
		return 1;
	}

	return 0;
} // main

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
