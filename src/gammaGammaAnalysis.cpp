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
	float ggHigh = 100.; // max time difference for gamma gamma
	float ggLow = 500.; // min time difference for gamma gamma
	float bgHigh = 2000.; // max time diff for gamma gamma bg

	float gbin = 5500;
	float gx = 0;
	float gy = gbin;


	std::cout << "Processing run " << run_number << " with " << gChain->GetNtrees() << " file(s)" << std::endl;

	long analysis_entries = gChain->GetEntries();

	if (gChain->FindBranch(branch_name)) {
		gChain->SetBranchAddress("TGriffin", &fGrif);
		std::cout << "Succesfully found " << branch_name << " branch" << std::endl;
	}

	TObjArray HList(0);

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

		// Filling required Lists and preproccessing data
		for (auto j = 0; j < fGrif->GetSuppressedMultiplicity(fGriffinBgo); ++j) {
			det = fGrif->GetSuppressedHit(j)->GetArrayNumber();
	        if(fGrif->GetSuppressedHit(j)->GetKValue()!=700) {continue;} // removes GRIFFIN hits pileup events
	        // Applying energy calibration
	        //energyTmp = offset[det-1] + gain[det-1]*fGrif->GetSuppressedHit(i)->GetCharge() + non_lin[det-1]*fGrif->GetSuppressedHit(i)->GetCharge()*fGrif->GetSuppressedHit(i)->GetCharge();
	        //energyTmp += ((double) rand() / RAND_MAX - 0.5);
	        energyTmp = fGrif->GetSuppressedHit(j)->GetEnergy();
	        suppr_en.push_back(energyTmp);
	        pos.push_back(fGrif->GetSuppressedHit(j)->GetPosition(145.0));
	        gamma_time.push_back(fGrif->GetSuppressedHit(j)->GetTime());
		}

		for (auto g1 = 0; g1 < suppr_en.size(); ++g1) {

			// gamma-gamma matrices
			for(auto g2 = 0; g2 < suppr_en.size(); ++g2) {
				if (g1 == g2) continue;

	            double angle = pos.at(g1).Angle(pos.at(g2)) * 180. / TMath::Pi();
				if (angle < 0.0001) continue;

				int angleIndex = GetAngleIndex(angle, fAngleCombinations);
				double ggTime = TMath::Abs(gamma_time.at(g1) - gamma_time.at(g2));

				// check for bad angles
				if (angleIndex == -1){
					std::cout << "Bad Angle" << std::endl;
					continue;
				}

				// Generating/Retrieving histograms
  				TH1F *myhist = ((TH1F *)0);
				if (angleIndex < HList.GetSize()) myhist = ((TH1F *)(HList.At(index)));
				if (!myhist){
					myhist = new TH1F(TString::Format("gammaGammaSub%i", angleIndex),
									  Form("%.1f^{o}: #gamma-#gamma, time-random-bg subtracted", fAngleCombinations[i]),
									  gbin, glow, ghigh,
									  gbin, glow, ghigh);
					HList.AddAtAndExpand(myhist, index);
				} // !myhist

				// Filling histogram
				if (ggTime < ggHigh){
					//myhist->Fill(suppr_en.at(g1), suppr_en.at(g2));
				}
				else if (bgLow < ggTime && ggTime < bgHigh){
					//myhist->Fill(suppr_en.at(g1), suppr_en.at(g2), -ggHigh/(bgHigh-bgLow));
				}

			} // grif2
		} // grif1

		if (i % 10000 == 0) {
			progress_bar.display();
		}
		++progress_bar; // iterates progress_bar
	} // end fill loop

	progress_bar.done();

	//Writing histograms to file
	TFile *out_file = new TFile(Form("gg_%i", run_number), "RECREATE");
	//out_file->cd();
	//HList.Write();

	return 0;
} // ProcessData

/*
for (loop over events, elements, tracks, hits)
{
// Note: you must be sure that 0 <= index <= SomeReasonableMaxValue
// because the “HList” total RAM usage will be about
// (1 … 2) * SomeReasonableMaxValue * sizeof(a_pointer)
Int_t index = (*elementId).at(trackIt).at(hitIt);
if (index < 0) continue; // just a precaution

  TH1F *myhist = ((TH1F *)0);
  if (index < HList.GetSize()) myhist = ((TH1F *)(HList.At(index)));
  if (!myhist)
    {
      myhist = new TH1F(TString::Format("h_element_%d", index),
                        "Response time;t[ns];Number of hits",
                        300, 0., 30.);
      HList.AddAtAndExpand(myhist, index);
    } // if (!myhist) ...

  myhist->Fill(time, 1.);
} // for (loop over events, elements, tracks, hits) ...

HList.Write();[/code]
*/

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
