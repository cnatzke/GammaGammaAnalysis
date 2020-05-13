#ifndef HISTOGRAM_MANAGER_H
#define HISTOGRAM_MANAGER_H

#include "TFile.h"
#include "TList.h"

class HistogramManager
{
public:

    HistogramManager();
    ~HistogramManager();
    TList* LoadHistograms(TFile* inputFile, const char *dir_name);

private:
	bool CheckFileIntegrity(TFile* inputFile);
};

#endif
