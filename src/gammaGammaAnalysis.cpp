//////////////////////////////////////////////////////////////////////////////////
// Extracts the gamma gamma tree and processes events into histograms
//
// Run this script in the parent directory of the experiment
// (e.g. /data4/S9038)
//
// Author:        Connor Natzke (cnatzke@triumf.ca)
// Creation Date: Monday April 6, 2020	T13:33:58-07:00
// Last Update:   Monday April 6, 2020	T13:42:13-07:00
// Usage:
//
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TFile.h"
#include "TGRSIUtilities.h"
#include "TParserLibrary.h" // needed for GetRunNumber
#include "TMath.h"
//#include "TGRSIRunInfo.h"

#include "progress_bar.h"
#include "GammaGammaAnalysis.h"
#include "Notifier.h"

Notifier *notifier = new Notifier;
/************************************************************//**
 * Lists all of the files in a directory matching the run number
 * (i.e. extracts all subruns for processing)
 *
 * @param dir_name   Directory name
 * @param ext        Extension of the files (default = ".root")
 ***************************************************************/
int main(int argc, char **argv) {

	if (argc == 1) { // no inputs given
		PrintUsage(argv);
		return 0;
	}

	for (auto i = 1; i < argc; i++) {
		if(!OpenFile(argv[i])) {
			std::cout << "Please check file extensions" << std::endl;
			return 1;
		}
	}

	if (!gChain) std::cout << "No gChain found" << std::endl;
	if (!gChain->GetEntries()) std::cout << "Found gChain, but no entries retrieved" << std::endl;

	if (!gChain || !gChain->GetEntries()) {
		std::cerr << "Failed to find anything. Exiting" << std::endl;
		return 1;
	}

	int process_check;
	process_check = ProcessData();
	if (process_check != 0) {
		std::cerr << "Data did not process correctly ... exiting" << std::endl;
		return 1;
	}
	return 0;

} // main

/******************************************************************************
 * Process events from gamma-gamma tree
 *
 *****************************************************************************/
int ProcessData(){
	std::string fName = gChain->GetCurrentFile()->GetName();
	const char* branch_name = "fGammaGamma";

	std::cout << "Processing file " << fName << " with " << gChain->GetNtrees() << " file(s)" << std::endl;

	/*
	   GrifSinglesEvent_t fGrif;
	   GrifGammaGammaEvent_t fGammaGamma;

	   TTree *griffin_griffin_tree = new TTree("griffin_griffin_tree", "Crystal Gamma Gamma");
	   griffin_griffin_tree->Branch("fGammaGamma", &fGammaGamma, "grif1Energy/D:grif2Energy/D:grif1ArrayNumber/I:grif2ArrayNumber/I:deltaTime/D:angle/D");
	 */

	long tree_entries = gChain->GetEntries();

	if (gChain->FindBranch(branch_name)) {
		gChain->SetBranchAddress(branch_name, &tree_data);
		std::cout << "Succesfully found " << branch_name << " branch" << std::endl;
	}

	// display loading message
    DisplayLoadingMessage();

	/* Creates a progress bar that has a width of 70,
	 * shows '=' to indicate completion, and blank
	 * space for incomplete
	 */
	ProgressBar progress_bar(tree_entries, 70, '=', ' ');
	for (auto i = 0; i < tree_entries; i++) {
		// retrieve entries from trees
		gChain->GetEntry(i);

		if (i % 10000 == 0) {
			progress_bar.display();
		}
		++progress_bar; // iterates progress_bar
	} // end fill loop

	progress_bar.done();

	/*
	   int output_check = WriteTree(out_file, griffin_singles_tree);
	   if (output_check != 0) {
	    std::cout << "Could not write output file ... exiting" << std::endl;
	    return 1;
	   }
	 */

	return 0;
} // ProcessData

/************************************************************//**
 * Displays humourous loading message
 *
 ***************************************************************/
void DisplayLoadingMessage() {

    std::string line;
    int random = 0;
    int numOfLines = 0;
    std::ifstream File("loadingQuotes.txt");

	int count = 1;
	random = rand() % 19;

	while(getline(File, line))
	{
		++numOfLines;

		if(numOfLines == random)
		{
			std::cout << line << std::endl;
		}
	}
	count++;
} // DisplayLoadingMessage

/************************************************************//**
 * Lists all of the files in a directory matching the run number
 * (i.e. extracts all subruns for processing)
 *
 * @param dir_name   Directory name
 * @param ext        Extension of the files (default = ".root")
 ***************************************************************/
bool OpenFile(std::string fileName) {
	size_t dot_pos = fileName.find_last_of('.');
	std::string ext = fileName.substr(dot_pos + 1);

	if (ext == "root") {
		TFile f(fileName.c_str());
		if (f.Get("griffin_griffin_tree")) {
			if (!gChain) {
				gChain = new TChain("griffin_griffin_tree");
				notifier->AddChain(gChain);
				gChain->SetNotify(notifier);
			}
			gChain->Add(fileName.c_str());
			std::cout << "Added: " << fileName << std::endl;
			return true;
		}
	}
	std::cerr << "Found invalid file: " << fileName.c_str() << std::endl;
	return false;
} // OpenFile

/******************************************************************************
 * Prints usage message and version
 *****************************************************************************/
void PrintUsage(char* argv[]){
	std::cerr << argv[0] << " Version: " << GammaGammaAnalysis_VERSION_MAJOR
	          << "." << GammaGammaAnalysis_VERSION_MINOR << "\n"
	          << "usage: " << argv[0] << " calibration_file analysis_tree [analysis_tree_2 ... ]\n"
	          << " calibration_file: calibration file (must end with .cal)\n"
	          << " analysis_tree:    analysis tree to convert (must end with .root)"
	          << std::endl;
} // end PrintUsage
