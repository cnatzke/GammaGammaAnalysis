//////////////////////////////////////////////////////////////////////////////////
// Fits peaks
//
// Author:          Connor Natzke (cnatzke@triumf.ca)
// Creation Date:   Tuesday May 12, 2020    T15:39:33-07:00
// Last Update:     Tuesday May 12, 2020    T15:39:33-07:00
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TPeakFitter.h"
#include "TRWPeak.h"
#include "TFile.h"

#include "FitManager.h"

/************************************************************//**
 * Constructor
 ***************************************************************/
FitManager::FitManager(){
	TH1::SetDefaultSumw2(); // correctly propagate errors when summing histograms
	//std::cout << "Fitting Manager intialized" << std::endl;
}

/************************************************************//**
 * Destructor
 ***************************************************************/
FitManager::~FitManager(){
	//std::cout << "Fitting Manager deleted" << std::endl;
}

/************************************************************//**
 * Fits gamma single peaks
 *
 * @param histList List of 2D gamma gamma matrices
 * @param low_proj Low value of projection range
 * @param high_proj High value of projection range
 * @param low_fit Low value of fit range
 * @param high_fit High value of fit range
 ***************************************************************/
std::vector<TH1D*> FitManager::GenerateProjections(TList* histList, float low_proj, float high_proj, float low_fit, float high_fit){

	int verbose = 1;
	TH2D* hist = NULL;
	TH1D* proj_hist = NULL;
	std::vector<TH1D*> proj_vec;

	if (low_proj >= high_proj) {
		std::cout << "Incorrect projection range: " << low_proj << " - " << high_proj << std::endl;
		return proj_vec;
	}
	if (low_fit >= high_fit) {
		std::cout << "Incorrect fitting range: " << low_fit << " - " << high_fit << std::endl;
		return proj_vec;
	}

	// creating projections
	for (int i = 0; i < histList->GetSize(); i++) {
		hist = (TH2D*)histList->At(i);
		hist->GetYaxis()->SetRangeUser(low_proj, high_proj);
		proj_hist = hist->ProjectionX();
		proj_hist->SetName(Form("projection_%d", i));
		proj_hist->GetXaxis()->SetRangeUser(low_fit - 10, high_fit + 10);
		proj_vec.push_back(proj_hist);
	}

	if (verbose > 0 ) {
		std::cout << "Generated " << proj_vec.size() << " projection histograms." << std::endl;
	}

	return proj_vec;
} // GenerateProjections

/************************************************************//**
 * Clones projection histograms
 *
 * @param histList Vector of histograms to be cloned
 ***************************************************************/
std::vector<TH1D*> FitManager::CloneProjections(std::vector<TH1D*> histVec, float lowFit, float highFit)
{

	int verbose = 1;
	std::vector<TH1D*> cloned_hist_vec;
	TH1D* total_proj = NULL;
	TH1D* temp_hist;

	if (verbose > 0 ) {
		std::cout << "Cloning " << histVec.size() << " projection histograms." << std::endl;
	}

	for (unsigned int i = 0; i < histVec.size(); i++) {
		temp_hist = (TH1D*) histVec.at(i)->Clone(Form("projection_binned_%d",i));
		temp_hist->GetXaxis()->SetRangeUser(lowFit - 10, highFit + 10);
		if (i == 0) {
			total_proj = (TH1D*) histVec.at(i)->Clone("total_proj");
		} else {
			total_proj->Add(histVec.at(i));
		}
		cloned_hist_vec.push_back(temp_hist);
	}

	// place total projection at beginning of vector
	cloned_hist_vec.insert(cloned_hist_vec.begin(), total_proj);

	if (verbose > 0 ) {
		std::cout << "Cloned " << cloned_hist_vec.size() - 1 << " projection histograms." << std::endl;
	}

	return cloned_hist_vec;

} // CloneProjections

/************************************************************//**
 * Fits peak
 *
 * @param
 ***************************************************************/
TRWPeak* FitManager::FitPeak(TH1D* inputHist, float peakPos, float lowFit, float highFit)
{
	int verbose = 0;

	TPeakFitter *pf = new TPeakFitter(lowFit, highFit);
	TRWPeak *peak = new TRWPeak(peakPos);

	// using initial Guess
	peak->GetFitFunction()->SetParameter(1, peakPos);
	peak->GetFitFunction()->SetParameter(2, 1.0);

	pf->AddPeak(peak);
	pf->Fit(inputHist, "L");

	if (verbose > 0) {
		std::cout << "====================" << std::endl;
		std::cout << "Fit parameters: " << std::endl;
		for(int i = 0; i < peak->GetFitFunction()->GetNpar(); ++i) {
			double min, max;
			peak->GetFitFunction()->GetParLimits(i, min, max);
			std::cout << i << ": " <<peak->GetFitFunction()->GetParameter(i) << "; " << min << " - " << max << std::endl;
		}
		std::cout << "====================" << std::endl;
		std::cout << "Summed peak area = " << peak->Area() << " +- " << peak->AreaErr() << std::endl;
		peakPos = peak->GetFitFunction()->GetParameter(1);
		double sigma = peak->GetFitFunction()->GetParameter(2);
		std::cout << "Fixing peak position to " << peakPos << ", and sigma to " << sigma << " (FWHM = " << 2.35 * sigma << ")" << std::endl;
	}

	if(peak->Area() < 1.) {
		std::cerr << "Error in fitting: either the fit went wrong, or there isn't enough statistics to do anything" << std::endl;
	}

	return peak;

} // FitPeak
