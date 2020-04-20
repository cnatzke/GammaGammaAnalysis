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
#include "TH2.h"
#include "TH1.h"
#include "TObjArray.h"
#include "TParserLibrary.h"
#include "TEnv.h"
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

	// makes time retrival happy
	std::string grsi_path = getenv("GRSISYS");
	if(grsi_path.length() > 0) {
		grsi_path += "/";
	}
	grsi_path += ".grsirc";
	gEnv->ReadFile(grsi_path.c_str(), kEnvChange);

	TParserLibrary::Get()->Load();

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
	TObjArray gammaGammaSubList(0);
	TObjArray gammaGammaMixedList(0);

	float ggHigh = 100.; // max time difference for gamma gamma
	float bgLow = 500.; // min time difference for gamma gamma
	float bgHigh = 2000.; // max time diff for gamma gamma bg

	float gbin = 5500.;
	float gLow = 0.;
	float gHigh = gbin;

	std::cout << "Processing run " << run_number << " with " << gChain->GetNtrees() << " file(s)" << std::endl;

	long analysis_entries = gChain->GetEntries();

	if (gChain->FindBranch(branch_name)) {
		gChain->SetBranchAddress("TGriffin", &fGrif);
		std::cout << "Succesfully found " << branch_name << " branch" << std::endl;
	}

	// display loading message
	DisplayLoadingMessage();

	TH1D *dT_total = new TH1D("dT_total", "Time diff between gammas", 2000, 0, 2000);
	TH1D *dT_coin = new TH1D("dT_coin", "Time diff between gammas", 2000, 0, 2000);
	TH1D *detID = new TH1D("detID", "Detector ID", 100, 0, 100);

	/* Creates a progress bar that has a width of 70,
	 * shows '=' to indicate completion, and blank
	 * space for incomplete
	 */
	ProgressBar progress_bar(analysis_entries, 70, '=', ' ');
	//for (auto i = 0; i < analysis_entries/100; i++) {
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
			detector_vec.push_back(det);
		}

		// Filling histograms
		for (unsigned int g1 = 0; g1 < suppr_en.size(); ++g1) {
			detID->Fill(detector_vec.at(g1));
			// gamma-gamma matrices
			for(unsigned int g2 = 0; g2 < suppr_en.size(); ++g2) {
				if (g1 == g2) continue;

				double angle = pos.at(g1).Angle(pos.at(g2)) * 180. / TMath::Pi();
				if (angle < 0.0001) continue;

				int angleIndex = GetAngleIndex(angle, fAngleCombinations);
				double ggTime = TMath::Abs(gamma_time.at(g1) - gamma_time.at(g2));
				/*
				if (ggTime < ggHigh){
					std::cout << "g1 time: " << gamma_time.at(g1)
					<< " g2 time: " << gamma_time.at(g2)
					<< " dTime: " << ggTime
					<< std::endl;
				}
				*/

				dT_total->Fill(ggTime);

				// check for bad angles
				if (angleIndex == -1) {
					std::cout << "Bad Angle" << std::endl;
					continue;
				}

				// Generating/Retrieving histograms
				TH2F *myhist = ((TH2F *) 0); //NULL
				if (angleIndex < gammaGammaSubList.GetSize()) myhist = ((TH2F *)(gammaGammaSubList.At(angleIndex)));
				if (!myhist) {
					myhist = new TH2F(TString::Format("gammaGammaSub%i", angleIndex), Form("%.1f deg #gamma-#gamma, time-random-bg subtracted", fAngleCombinations[angleIndex]), gbin, gLow, gHigh, gbin, gLow, gHigh);
					gammaGammaSubList.AddAtAndExpand(myhist, angleIndex);
				} // !myhist

				// Filling histogram
				if (ggTime < ggHigh) {
					myhist->Fill(suppr_en.at(g1), suppr_en.at(g2));
					dT_coin->Fill(ggTime);
					//myhist->Fill(suppr_en.at(g2), suppr_en.at(g1));
				}
				else if (bgLow < ggTime && ggTime < bgHigh) {
					myhist->Fill(suppr_en.at(g1), suppr_en.at(g2), -ggHigh/(bgHigh-bgLow));
					//myhist->Fill(suppr_en.at(g2), suppr_en.at(g1), -ggHigh/(bgHigh-bgLow));
				}
			} // grif2

			// EVENT MIXED MATRICES
			// event mixing, we use the last event as second griffin
			checkMix = (int)lastgrifEnergy.size();
			if(checkMix<event_mixing_depth) continue;
			for(auto lg = 0; lg < (checkMix - 1); ++lg) {
				unsigned int multLG = lastgrifEnergy.at(lg).size();

				for(unsigned int g3 = 0; g3 < multLG; ++g3) {
					double angle = pos.at(g1).Angle(lastgrifPosition.at(lg).at(g3)) * 180. / TMath::Pi();
					if (angle < 0.0001) continue;
					int angleIndex = GetAngleIndex(angle, fAngleCombinations);

					// Generating/Retrieving histograms
					TH2F *myhist = ((TH2F *)0);
					if (angleIndex < gammaGammaMixedList.GetSize()) myhist = ((TH2F *)(gammaGammaMixedList.At(angleIndex)));
					if (!myhist) {
						myhist = new TH2F(TString::Format("gammaGammaMixed%i", angleIndex), Form("%.1f deg #gamma-#gamma, event-mixed", fAngleCombinations[angleIndex]), gbin, gLow, gHigh, gbin, gLow, gHigh);
						gammaGammaMixedList.AddAtAndExpand(myhist, angleIndex);
					} // !myhist

					// Filling histogram
					myhist->Fill(suppr_en.at(g1), lastgrifEnergy.at(lg).at(g3));
				} // end g3
			} //end LG

		} // grif1

		if (i % 10000 == 0) {
			progress_bar.display();
		}
		++progress_bar; // iterates progress_bar

		// update "last" event
		lastgrifEnergy.push_back(suppr_en);
		lastgrifPosition.push_back(pos);
		lgsize = static_cast<int>(lastgrifEnergy.size());
		if (lgsize > event_mixing_depth){
			lastgrifEnergy.erase(lastgrifEnergy.begin());
			lastgrifPosition.erase(lastgrifPosition.begin());
		}

		// cleaning up for next event
		suppr_en.clear();
		pos.clear();
		gamma_time.clear();
		detector_vec.clear();
	} // end fill loop

	progress_bar.done();

	//Writing histograms to file
	TFile *out_file = new TFile(Form("gg_%i_histograms.root", run_number), "RECREATE");
	std::cout << "Writing output file: " << out_file->GetName() << std::endl;

	out_file->cd();

	detID->Write();
	dT_coin->Write();
	dT_total->Write();

	TDirectory* dir_TRS = out_file->mkdir("TimeRandomSubtacted");
	dir_TRS->cd();
	gammaGammaSubList.Compress();
	gammaGammaSubList.Write();
	TDirectory* dir_Mixed = out_file->mkdir("EventMixed");
	dir_Mixed->cd();
	gammaGammaMixedList.Compress();
	gammaGammaMixedList.Write();
	out_file->Write();
	delete out_file;

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
			if (mid > 0 && angle > vec[mid - 1]) {
				return GetClosest(mid - 1, mid, fAngleCombinations, angle);
			}

			// repeat for left half
			j = mid;
		}
		// if angle is greater than mid
		else{
			if (mid < vec.size() - 1 && angle < vec[mid + 1]) {
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
