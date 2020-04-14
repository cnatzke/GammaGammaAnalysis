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

	for (auto i = 1; i < argc; i++) AutoFileDetect(argv[i]);

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
	int run_number = GetRunNumber(fName.c_str());
	const char* branch_name = "TGriffin";

	std::cout << "Processing run " << run_number << " with " << gChain->GetNtrees() << " file(s)" << std::endl;

	long analysis_entries = gChain->GetEntries();

	if (gChain->FindBranch(branch_name)) {
		gChain->SetBranchAddress("TGriffin", &griffin_data);
		std::cout << "Succesfully found " << branch_name << " branch" << std::endl;
	}

	// display loading message
    DisplayLoadingMessage();

	/* Creates a progress bar that has a width of 70,
	 * shows '=' to indicate completion, and blank
	 * space for incomplete
	 */
	ProgressBar progress_bar(analysis_entries, 70, '=', ' ');
	for (auto i = 0; i < analysis_entries; i++) {
		// retrieve entries from trees
		gChain->GetEntry(i);

		for (auto g1 = 0; g1 < griffin_data->GetSuppressedMultiplicity(fGriffinBgo); ++g1) {
			auto *grif1 = griffin_data->GetSuppressedHit(g1);

			if (grif1->GetKValue() != 700) {continue;} // remove GRIFFIN pileup events

			// gamma-gamma matrices
			for(auto g2 = 0; g2 < griffin_data->GetSuppressedMultiplicity(fGriffinBgo); ++g2) {
				if (g1 == g2) continue;
				auto *grif2 = griffin_data->GetSuppressedHit(g2);

				double angle = grif1->GetPosition(145.0).Angle(grif2->GetPosition(145.0)) * 180. / TMath::Pi();
				if (angle < 0.0001) continue;

				int angleIndex = GetAngleIndex(angle, fAngleCombinations);

				// check for bad angles
				if (angleIndex == -1){
					std::cout << "Bad Angle" << std::endl;
					continue;
				}
				/*j
				if (i < 50 && angleIndex != -1) {
					std::cout << "Total number of angles: " << fAngleCombinations.size() << std::endl;
					std::cout << "Angle: " << angle << " AngleIndex: " << angleIndex << " Closest Angle: " << fAngleCombinations[angleIndex] << std::endl;
				}

				fGammaGamma.grif1Energy = grif1->GetEnergy();
				fGammaGamma.grif2Energy = grif2->GetEnergy();
				fGammaGamma.grif1ArrayNumber = grif1->GetArrayNumber();
				fGammaGamma.grif2ArrayNumber = grif2->GetArrayNumber();
				fGammaGamma.deltaTime = TMath::Abs(grif1->GetTime() - grif2->GetTime());
				fGammaGamma.angle = angle;

				griffin_griffin_tree->Fill();
				*/
			} // grif2
		} // grif1

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
	random = rand() % 131;

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

///******************************************************************************
// * Returns the angular index
// *
// * @param angle The angle between two gammas
// * @param vec Vector of angles
// *****************************************************************************/
int GetAngleIndex(double angle, std::vector<double> vec){

	// corner cases
	if (angle <= vec.front()) { return 0;}
	if (angle >= vec.back() - 1.) { return vec.size() - 1;}

	// binary search
	int i = 0, j = vec.size(), mid = 0;
	while ( i < j ) {
		mid = (i + j) / 2;

		if (vec[mid] == angle) return vec[mid];

		// searching left half
		if (angle < vec[mid]) {
			// if angle is greater than previous to mid, return closest of two
			if (mid > 0 && angle > vec[mid - 1]){
				return GetClosest(mid - 1, mid, fAngleCombinations, angle);
			}

			// repeat for left half
			j = mid;
		}
		// if angle is greater than mid
		else{
			if (mid < vec.size() - 1 && angle < vec[mid + 1]){
				return GetClosest(mid, mid + 1, fAngleCombinations, angle);
			}

			// update i
			i = mid + 1;
		}
	}
	// Only single element left after search
	return mid;
} // GetAngleIndex

///******************************************************************************
// * Returns the value closest to the target
// * Assumes val2 is greater than val1 and target lies inbetween the two
// *
// * @param val1 First value to compare
// * @param val2 Second value to compare
// * @param vec Vector of values
// * @param target Target value
// *****************************************************************************/
int GetClosest(int val1, int val2, std::vector<double> vec, double target){
	if ((target - vec[val1]) >= (vec[val2] - target))
		return val2;
	else
		return val1;
} // GetClosest

/******************************************************************************
 * Detects type of input file
 *
 *****************************************************************************/
void AutoFileDetect(std::string fileName){
	size_t dot_pos = fileName.find_last_of('.');
	std::string ext = fileName.substr(dot_pos + 1);

	if (ext == "root") {
		OpenRootFile(fileName);
	}
	else if (ext == "cal") {
		notifier->AddCalFile(fileName);
	} else {
		std::cerr << "Discarding unknown file: " << fileName.c_str() << std::endl;
	}
} // End AutoFileDetect

/************************************************************//**
 * Opens Root files
 *
 * @param dir_name   Directory name
 * @param ext        Extension of the files (default = ".root")
 ***************************************************************/
void OpenRootFile(std::string fileName){
	TFile f(fileName.c_str());
	if (f.Get("AnalysisTree")) {
		if (!gChain) {
			gChain = new TChain("AnalysisTree");
			notifier->AddChain(gChain);
			gChain->SetNotify(notifier);
		}
		gChain->Add(fileName.c_str());
		std::cout << "Added: " << fileName << std::endl;
	}
} // end OpenRootFile

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
