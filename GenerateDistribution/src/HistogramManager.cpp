//////////////////////////////////////////////////////////////////////////////////
// Checks ROOT file and loads required histograms
//
// Author:          Connor Natzke (cnatzke@triumf.ca)
// Creation Date:   Tuesday May 12, 2020    T15:39:33-07:00
// Last Update:     Tuesday May 12, 2020    T15:39:33-07:00
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TDirectory.h"
#include "TKey.h"
#include "HistogramManager.h"

/************************************************************//**
 * Constructor
 ***************************************************************/
HistogramManager::HistogramManager(){
	//std::cout << "Histogram Manager intialized" << std::endl;
}

/************************************************************//**
 * Destructor
 ***************************************************************/
HistogramManager::~HistogramManager(){
	//std::cout << "Histogram Manager deleted" << std::endl;
}

/************************************************************//**
 * Checks for required directories
 *
 * @param verbose Verbosity level
 ***************************************************************/
TList* HistogramManager::LoadHistograms(TFile* inputFile, const char *dir_name){

	TList *hist_list = new TList();

	// returns list of -1's if file is bad
	if (!CheckFileIntegrity(inputFile)) return hist_list;

	TDirectory *hist_dir = dynamic_cast<TDirectory*>(inputFile->Get(dir_name));

	TObject *obj;
	TKey *key;
	TIter next( hist_dir->GetListOfKeys());
	while ((key = (TKey *) next())) {
		obj = hist_dir->Get(key->GetName()); // copy object to memory
        hist_list->Add(obj);
		printf(" found object:%s\n",key->GetName());
	}

	return hist_list;
} // LoadHistograms

/************************************************************//**
 * Checks for required directories
 *
 * @param verbose Verbosity level
 ***************************************************************/
bool HistogramManager::CheckFileIntegrity(TFile* inputFile){
	if (inputFile->GetDirectory("TimeRandomSubtacted") && inputFile->GetDirectory("EventMixed")) {
		std::cout << "Found time-random subtracted histograms" << std::endl;
		std::cout << "Found event mixed histograms" << std::endl;
		return true;
	} else {
		std::cout << "Could not find required histograms ... exiting" << std::endl;
		return false;
	}
} // CheckFileIntegrity
