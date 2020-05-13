// list of functions:
TH1D* LoadGammaHisto(const char* infilename); // load projection on GRIFFIN side of azE
TH1D* LoadGammaHistoNoBetaTagged(const char* infilename); // load asE_CT
TH1D* LoadElectronHisto2and4BetaTagged(const char* infilename); // load p2E_z and p4E_z
TH1D* LoadElectronHisto2NoBetaTagged(const char* infilename); // load p2E
TH1D* LoadElectronHisto3NoBetaTagged(const char* infilename); // load p3E
TH1D* LoadElectronHisto4NoBetaTagged(const char* infilename); // load p4E
TH1D* LoadElectronHisto5NoBetaTagged(const char* infilename); // load p5E
TH1D* LoadElectronHisto1NoBetaTagged(const char* infilename); // load p1E
TH2D* LoadGammaGammaHisto(const char* infilename); // load aaE_z
TH2D* LoadGammaGammaHistoNoBetaTagged(const char* infilename); // load aaE
TH2D* LoadGammaGammaHistoNoAddback(const char* infilename);  // load ggE_z
TH2D* LoadGammaGammaHistoNoAddbackNoBetaTagged(const char* infilename);  // load ggE
TH1D* LoadGammaGatedGammaProjection(TH2D* aaE_z, int gate);
void DrawTotalGammaProjection(TH2D* aaE_z, int gate); // draw total projection of gamma gamma matrix with fit of the gating energy you are interested in
TH1D* LoadGammaGatedElectronProjection();
void FitElectron(TH1D* pE2, int energyFit, int lowerFitLimit, int higherFitLimit);
void FitElectronLinearBkg(TH1D* pE2, int energyFit, int lowerFitLimit, int higherFitLimit);
void FitGamma(TH1D* aE_z, int energyFit);
void FitGammaTriple(TH1D* aE_z, int energyFit1, int energyFit2, int energyFit3, int lowerFitRange, int upperFitRange);
void FitGammaRadware(TH1D* aE_z, int energyFit);
void FitEu();
void FitBa();
void FitEuBa(); // >>> fit of two overlapping gamma peaks to get areas

const char* LoadInfilenameS1716(int MassNumber){
	const char* filename;
	if(MassNumber==78) (filename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass78/Histo/S1716_14586-14610all.root");
	if(MassNumber==76) (filename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass76/Histo/S1716_14492-14500all.root");
	if(MassNumber==74) (filename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass74/Histo/S1716_14544-14545all.root");
	return filename;
}

const char* LoadInfilenameS1716singles(int MassNumber){
	const char* filename;
	if(MassNumber==78) (filename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass78/Histo/S1716singles_14586-14610all.root");
	if(MassNumber==76) (filename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass76/Histo/S1716singles_14492-14500all.root");
	//if(MassNumber==74)(filename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass74/Histo/S1716singles_14544-14545all.root");
	if(MassNumber==74) (filename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass74/Histo/SinglesRebinned/S1716singles_14544-14545all.root");

	// this is not really a Ge file, it is 207Bi source data
	if(MassNumber==207) (filename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Calibrations/Histos/PACEScalibration/SimpleElectron14387_000.root");
	return filename;
}

void CallFitGamma_GammaGated(int MassNumber, int gate, int energyFit){
	const char* infilename = LoadInfilenameS1716(MassNumber);
	TH2D * aaE_z = LoadGammaGammaHisto(infilename);
	TH1D * aE_z_gated = LoadGammaGatedGammaProjection(aaE_z,gate);
	FitGamma(aE_z_gated, energyFit);
}

void CallFitGamma_GammaGatedNoBetaTagged(int MassNumber, int gate, int energyFit){
	const char* infilename = LoadInfilenameS1716(MassNumber);
	TH2D * ggE_z = LoadGammaGammaHistoNoBetaTagged(infilename);
	TH1D * gE_z_gated = LoadGammaGatedGammaProjection(ggE_z,gate);
	FitGamma(gE_z_gated, energyFit);
}

void CallFitGamma_GammaGatedNoAddback(int MassNumber, int gate, int energyFit){
	const char* infilename = LoadInfilenameS1716singles(MassNumber);
	TH2D * ggE_z = LoadGammaGammaHistoNoAddback(infilename);
	TH1D * gE_z_gated = LoadGammaGatedGammaProjection(ggE_z,gate);
	FitGamma(gE_z_gated, energyFit);
}

void CallFitGamma_GammaGatedNoAddbackNoBetaTagged(int MassNumber, int gate, int energyFit){
	const char* infilename = LoadInfilenameS1716singles(MassNumber);
	TH2D * ggE = LoadGammaGammaHistoNoAddbackNoBetaTagged(infilename);
	TH1D * gE_gated = LoadGammaGatedGammaProjection(ggE,gate);
	FitGamma(gE_gated, energyFit);
}

void CallFitGamma_GammaSingles(int MassNumber, int energyFit){
	const char* infilename = LoadInfilenameS1716(MassNumber);
	TH1D * aE_z = LoadGammaHisto(infilename);
	FitGamma(aE_z, energyFit);
}

void CallFitGamma_GammaSinglesNoBetaTagged(int MassNumber, int energyFit){
	const char* infilename = LoadInfilenameS1716(MassNumber);
	TH1D * aE = LoadGammaHistoNoBetaTagged(infilename);
	FitGamma(aE, energyFit);
}

void CallFitGammaTriple_GammaSinglesNoBetaTagged(int MassNumber, int energyFit1, int energyFit2, int energyFit3, int lowerFitRange, int upperFitRange){
	const char* infilename = LoadInfilenameS1716(MassNumber);
	TH1D * aE = LoadGammaHistoNoBetaTagged(infilename);
	FitGammaTriple(aE, energyFit1, energyFit2, energyFit3, lowerFitRange, upperFitRange);
}

void CallFitGammaRadware_GammaSingles(int MassNumber, int energyFit){
	const char* infilename = LoadInfilenameS1716(MassNumber);
	TH1D * aE_z = LoadGammaHisto(infilename);
	FitGammaRadware(aE_z, energyFit);
}

void CallDrawTotalGammaProjection(int MassNumber, int gate){
	const char* infilename = LoadInfilenameS1716(MassNumber);
	TH2D * aaE_z = LoadGammaGammaHisto(infilename);
	DrawTotalGammaProjection(aaE_z, gate);
}

void CallDrawTotalGammaProjectionNoAddback(int MassNumber, int gate){
	const char* infilename = LoadInfilenameS1716singles(MassNumber);
	TH2D * ggE_z = LoadGammaGammaHistoNoAddback(infilename);
	DrawTotalGammaProjection(ggE_z, gate);
}

void CallDrawTotalGammaProjectionNoBetaTagged(int MassNumber, int gate){
	const char* infilename = LoadInfilenameS1716(MassNumber);
	TH2D * aaE = LoadGammaGammaHistoNoBetaTagged(infilename);
	DrawTotalGammaProjection(aaE, gate);
}

// lowerFitLimit and higherFitLimit are numbers of CHANNELS to define the fit region around the centroid
void CallFitElectron_ElectronSinglesNoBetaTagged(int MassNumber, int energyFit, int lowerFitLimit, int higherFitLimit){
	const char* infilename = LoadInfilenameS1716singles(MassNumber);
	//TH1D * pE2 = LoadElectronHisto2NoBetaTagged(infilename);
	TH1D * pE2 = LoadElectronHisto5NoBetaTagged(infilename);
	FitElectron(pE2, energyFit, lowerFitLimit, higherFitLimit);
}

// lowerFitLimit and higherFitLimit are numbers of CHANNELS to define the fit region around the centroid
void CallFitElectronLinearBkg_ElectronSinglesNoBetaTagged(int MassNumber, int energyFit, int lowerFitLimit, int higherFitLimit){
	const char* infilename = LoadInfilenameS1716singles(MassNumber);
	//TH1D * pE2 = LoadElectronHisto2NoBetaTagged(infilename);
	TH1D * pE2 = LoadElectronHisto5NoBetaTagged(infilename);
	FitElectronLinearBkg(pE2, energyFit, lowerFitLimit, higherFitLimit);
}

TH1D* LoadGammaHisto(const char* infilename){
	//const char* infilename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass78/Histo/S1716_14586-14587all.root";
	//const char* infilename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass78/Histo/S1716_14586-14610all.root";
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH2D * azE = (TH2D*)infile->Get("azE");
	TH2D * azE_bg = (TH2D*)infile->Get("azE_bg");
	TH1D * aE_z = azE->ProjectionX();
	TH1D * aE_zbg = azE_bg->ProjectionX();
	aE_z->Add(aE_zbg,-100./300.);

	aE_z->SetTitle("GRIFFIN addback suppressed and CT corrected #beta-tagged");
	aE_z->SetName("aE_z");

	aE_z->Draw("E");

	return aE_z;
}

TH1D* LoadGammaHistoNoBetaTagged(const char* infilename){
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH1D * asE_CT = (TH1D*)infile->Get("asE_CT");

	asE_CT->SetTitle("GRIFFIN addback suppressed and CT corrected in Singles");
	asE_CT->SetName("asE_CT");

	asE_CT->Draw("E");

	return asE_CT;
}

TH1D* LoadElectronHisto2and4BetaTagged(const char* infilename){
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH1D * pE2_z = (TH1D*)infile->Get("pE2_z");
	TH1D * pE2_zbg = (TH1D*)infile->Get("pE2_zbg");
	pE2_z->Add(pE2_zbg,-40./2000.);
	TH1D * pE4_z = (TH1D*)infile->Get("pE4_z");
	TH1D * pE4_zbg = (TH1D*)infile->Get("pE4_zbg");
	pE4_z->Add(pE4_zbg,-40./2000.);

	pE2_z->Add(pE4_z,+1);
	pE2_z->SetTitle("PACES 2 and 4 #beta-tagged");
	pE2_z->SetName("pE2-4_z");

	pE2_z->Draw();

	return pE2_z;
}

TH1D* LoadElectronHisto2NoBetaTagged(const char* infilename){
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH1D * pE2 = (TH1D*)infile->Get("pE2");

	pE2->SetTitle("PACES 2 singles");
	pE2->Draw("E");

	return pE2;
}

TH1D* LoadElectronHisto3NoBetaTagged(const char* infilename){
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH1D * pE3 = (TH1D*)infile->Get("pE3");

	pE3->SetTitle("PACES 3 singles");
	pE3->Draw("E");

	return pE3;
}

TH1D* LoadElectronHisto4NoBetaTagged(const char* infilename){
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH1D * pE4 = (TH1D*)infile->Get("pE4");

	pE4->SetTitle("PACES 4 singles");
	pE4->Draw("E");

	return pE4;
}

TH1D* LoadElectronHisto5NoBetaTagged(const char* infilename){
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH1D * pE5 = (TH1D*)infile->Get("pE5");

	pE5->SetTitle("PACES 5 singles");
	pE5->Draw("E");

	return pE5;
}

TH1D* LoadElectronHisto1NoBetaTagged(const char* infilename){
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH1D * pE1 = (TH1D*)infile->Get("pE1");

	pE1->SetTitle("PACES 1 singles");
	pE1->Draw("E");

	return pE1;
}

TH2D* LoadGammaGammaHisto(const char* infilename){
	//const char* infilename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass78/Histo/S1716_14586-14587all.root";
	//const char* infilename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass78/Histo/S1716_14586-14610all.root";
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH2D * aaE_z = (TH2D*)infile->Get("aaE_z");
	TH2D * aaE_zbg = (TH2D*)infile->Get("aaE_zbg");
	aaE_z->Add(aaE_zbg,-200./3000.);

	aaE_z->SetTitle("GRIFFIN addback suppressed and CT corrected in coincidence and #beta-tagged");

	return aaE_z;
}

TH2D* LoadGammaGammaHistoNoBetaTagged(const char* infilename){
	//const char* infilename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass78/Histo/S1716_14586-14610all.root";
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH2D * aaE = (TH2D*)infile->Get("aaE");
	TH2D * aaE_bg = (TH2D*)infile->Get("aaE_bg");
	aaE->Add(aaE_bg,-200./3000.);

	aaE->SetTitle("GRIFFIN addback suppressed and CT corrected in coincidence");

	return aaE;
}

TH2D* LoadGammaGammaHistoNoAddback(const char* infilename){
	//const char* infilename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass78/Histo/S1716singles_14586-14610all.root";
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH2D * ggE_z = (TH2D*)infile->Get("ggE_z");
	TH2D * ggE_zbg = (TH2D*)infile->Get("ggE_zbg");
	ggE_z->Add(ggE_zbg,-200./3000.);

	ggE_z->SetTitle("GRIFFIN-GRIFFIN suppressed in coincidence and #beta-tagged");

	return ggE_z;
}

TH2D* LoadGammaGammaHistoNoAddbackNoBetaTagged(const char* infilename){
	//const char* infilename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Mass78/Histo/S1716singles_14586-14610all.root";
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	TH2D * ggE = (TH2D*)infile->Get("ggE");
	TH2D * ggE_bg = (TH2D*)infile->Get("ggE_bg");
	ggE->Add(ggE_bg,-200./3000.);

	ggE->SetTitle("GRIFFIN-GRIFFIN suppressed in coincidence");

	return ggE;
}

TH1D* LoadGammaGatedGammaProjection(TH2D* aaE_z, int gate){
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

// Mass 78
//   int gatesize = 5; // width of the gate on each side
// Mass 74
	int gatesize = 5; // width of the gate on each side
	int bkgsize = 15; // width of the background region on each side of the gate peak
	TH1D * aa_pro = aaE_z->ProjectionX();

	aa_pro->Draw();

// find centroid of the gate energy
// Mass 78
	TF1 *f1 = new TF1("f1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",gate-15,gate+15);
// Mass 76
//   TF1 *f1 = new TF1("f1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",gate-10,gate+15);
// Mass 74
//   TF1 *f1 = new TF1("f1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",gate-10,gate+5);
	f1->SetParameters(1e6,gate,0.5,0.5,2,100,200); // initial parameter values
	f1->SetParLimits(1,gate-2,gate+2); // centroid
	f1->SetParLimits(2,0.5,5); // sigma
	f1->SetParLimits(3,0.0,1); // relative height of second gaussian, which is shorter than the first one (0,1)
	f1->SetParLimits(4,1,10); // relative sigma of second gaussian, which is broader than the first one (>1)
	aa_pro->Fit("f1","R"); // "QR" makes the fit quiet, i.e. no output

// build the gated spectrum
	TAxis *axis = aa_pro->GetXaxis();
	TH1D * aa_gated = aaE_z->ProjectionY("aa_gated",axis->FindBin(f1->GetParameter(1)-gatesize),axis->FindBin(f1->GetParameter(1)+gatesize));
	aa_gated->SetTitle("Gamma Gated Projection");
	TH1D * aa_sub = aaE_z->ProjectionY("aa_sub",axis->FindBin(f1->GetParameter(1)-gatesize),axis->FindBin(f1->GetParameter(1)+gatesize));
	aa_sub->SetTitle("Gamma Gated Background Subtracted Projection");
	TH1D * aa_backLeft = aaE_z->ProjectionY("aa_backLeft",axis->FindBin(f1->GetParameter(1)-gatesize-bkgsize-4),axis->FindBin(f1->GetParameter(1)-gatesize-4));
	aa_backLeft->SetTitle("Unscaled Left Background Projection");
// Mass 78
	TH1D * aa_backRight = aaE_z->ProjectionY("aa_backRight",axis->FindBin(f1->GetParameter(1)+gatesize+4),axis->FindBin(f1->GetParameter(1)+gatesize+4+bkgsize));
// Mass 74
//   TH1D * aa_backRight = aaE_z->ProjectionY("aa_backRight",axis->FindBin(f1->GetParameter(1)+gatesize+15),axis->FindBin(f1->GetParameter(1)+gatesize+15+bkgsize));
	aa_backRight->SetTitle("Unscaled Right Background Projection");

	aa_sub->Add(aa_backLeft,-gatesize/bkgsize);
	aa_sub->Add(aa_backRight,-gatesize/bkgsize);

	return aa_sub;
}

void DrawTotalGammaProjection(TH2D* aaE_z, int gate){
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms

	int gatesize = 10; // width of the gate on each side
	int bkgsize = 15; // width of the background region on each side of the gate peak
	TH1D * aa_pro = aaE_z->ProjectionX();

	aa_pro->Draw();

// find centroid of the gate energy
// Mass 78
//   TF1 *f1 = new TF1("f1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",gate-15,gate+15);
// Mass 76
//   TF1 *f1 = new TF1("f1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",gate-10,gate+15);
// Mass 74
	TF1 *f1 = new TF1("f1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",gate-10,gate+5);
	f1->SetParameters(1e6,gate,0.5,0.5,2,100,200); // initial parameter values
	f1->SetParLimits(1,gate-2,gate+2); // centroid
	f1->SetParLimits(2,0.5,5); // sigma
	f1->SetParLimits(3,0.0,1); // relative height of second gaussian, which is shorter than the first one (0,1)
	f1->SetParLimits(4,1,10); // relative sigma of second gaussian, which is broader than the first one (>1)
	aa_pro->Fit("f1","R"); // "QR" makes the fit quiet, i.e. no output
}

void FitElectron(TH1D* pE2, int energyFit, int lowerFitLimit, int higherFitLimit){
	int x_1;
	x_1 = energyFit;

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(500000);
	// RADWARE FITTING FUNCTION with background defined as (1-CDF) of the Radware peak - see eq. 4.10 of James' thesis
	//TF1 *f1 = new TF1("f1","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4]))) + [5] + [6]*(1-0.5*(ROOT::Math::erfc((([1]-x)/([2]*TMath::Sqrt(2)))) + exp(((x-[1])/([3]))+(([2]^2)/(2*[3]^2)))*(1-[4])*ROOT::Math::erfc(([3]*(x-[1])+[2]^2)/[3]*[2]*TMath::Sqrt(2))))",x_1-18,x_1+8);

	// RADWARE FITTING FUNCTION with background defined as (1-CDF) of one-Gaussian peak - see eq. 4.1 of James' thesis
	TF1 *f1 = new TF1("f1","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4]))) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-lowerFitLimit,x_1+higherFitLimit);
	f1->SetParameters(1e6,x_1, 2, 5, 0.2,100,200);
	f1->SetParLimits(1, x_1 - 2, x_1 + 2); //centroid
	f1->SetParLimits(2, 0.2, 5); //sigma of gaussian distribution
	f1->SetParLimits(3, 0.1, 100); //magnitude of step in background noise
	f1->SetParLimits(4, 0.01, 1); //mixing ratio between the gaussian term and the gaussian convoluted with exp

	//pE2->Fit("f1","R"); // "QR" makes the fit quiet, i.e. no output
	TFitResultPtr r = pE2->Fit("f1","RS"); // TFitResultPtr contains the TFitResult
	TMatrixDSym cov = r->GetCovarianceMatrix();  //  to access the covariance matrix
	Double_t chi2   = r->Chi2();                 // to retrieve the fit chi2
	Int_t ndf = r->Ndf();
	r->Print("V");                               // print full information of fit including covariance matrix
	cout << "Chi-squared / ndf: " << chi2/ndf << endl;

	// BACKGROUND FUNCTION DRAWING
	TF1 *fBg = new TF1("fBg","[5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-lowerFitLimit,x_1+higherFitLimit);
	fBg->SetParameter(5,f1->GetParameter(5));
	fBg->SetParameter(6,f1->GetParameter(6));
	fBg->SetParameter(1,f1->GetParameter(1));
	fBg->SetParameter(2,f1->GetParameter(2));
	fBg->SetLineColor(kBlack);
	fBg->SetLineStyle(2);//9
	fBg->Draw("same");

	TF1 *fPeak =  new TF1("fPeak","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4])))",x_1-lowerFitLimit,x_1+higherFitLimit);
	fPeak->SetParameter(0,f1->GetParameter(0));
	fPeak->SetParameter(1,f1->GetParameter(1));
	fPeak->SetParameter(2,f1->GetParameter(2));
	fPeak->SetParameter(3,f1->GetParameter(3));
	fPeak->SetParameter(4,f1->GetParameter(4));

	// PEAK AREA EVALUATION
	int firstbin = pE2->FindBin(f1->GetParameter(1) - 8 * f1->GetParameter(2)); // first and last bin of the histogram, whatever the bin width is; histo integrals need bin numbers
	int lastbin  = pE2->FindBin(f1->GetParameter(1) + 8 * f1->GetParameter(2));

	double firstedge = pE2->GetBinLowEdge(firstbin); // gives the energy value at the first bin (if it's 1 keV/ch it is the same, if it is 0.5 keV/ch it isn't); function integrals need energy values
	double lastedge = pE2->GetBinLowEdge(lastbin) + pE2->GetBinWidth(lastbin);

	cout << "firstbin = " <<  firstbin << endl;
	cout << "lastbin = " <<  lastbin << endl;
	cout << "firstedge = " <<  firstedge << endl;
	cout << "lastedge = " <<  lastedge << endl;

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF FITTING FUNCTION (from Jack)
	// (total function minus background function integral)
	double bg_sub_integral = f1->Integral(firstedge,lastedge)/pE2->GetBinWidth(1) - fBg->Integral(firstedge,lastedge)/pE2->GetBinWidth(1);
	double bg_sub_err = TMath::Sqrt( f1->Integral(firstedge,lastedge)/pE2->GetBinWidth(1) + fBg->Integral(firstedge,lastedge)/pE2->GetBinWidth(1));

	cout << "The area of the fitting function minus bkg is " << bg_sub_integral << " +/- " << bg_sub_err << " (" << 100*bg_sub_err/bg_sub_integral << "%)" << endl;

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF THE HISTOGRAM MINUS BACKGROUND FUNCTION (from Jack)
	bg_sub_integral = pE2->Integral(firstbin,lastbin) - fBg->Integral(firstedge,lastedge)/pE2->GetBinWidth(1);
	bg_sub_err = TMath::Sqrt(pE2->Integral(firstbin,lastbin) + fBg->Integral(firstedge,lastedge)/pE2->GetBinWidth(1));

	cout << "The integral of the peak minus bkg is " << bg_sub_integral << " +/- " << bg_sub_err << " (" << 100*bg_sub_err/bg_sub_integral << "%)" << endl;

}

void FitElectronLinearBkg(TH1D* pE2, int energyFit, int lowerFitLimit, int higherFitLimit){
	int x_1;
	x_1 = energyFit;

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(500000);
	// RADWARE FITTING FUNCTION with background defined as (1-CDF) of one-Gaussian peak + linear term
	TF1 *f1 = new TF1("f1","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4]))) + [5] + [7]*x + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-lowerFitLimit,x_1+higherFitLimit);
	f1->SetParameters(1e6,x_1, 2, 5, 0.2,100,200);
	f1->SetParLimits(1, x_1 - 2, x_1 + 2); //centroid
	f1->SetParLimits(2, 0.2, 5); //sigma of gaussian distribution
	f1->SetParLimits(3, 0.1, 100); //magnitude of step in background noise
	f1->SetParLimits(4, 0.01, 1); //mixing ratio between the gaussian term and the gaussian convoluted with exp

	//pE2->Fit("f1","R"); // "QR" makes the fit quiet, i.e. no output
	TFitResultPtr r = pE2->Fit("f1","RS"); // TFitResultPtr contains the TFitResult
	TMatrixDSym cov = r->GetCovarianceMatrix();  //  to access the covariance matrix
	Double_t chi2   = r->Chi2();                 // to retrieve the fit chi2
	Int_t ndf = r->Ndf();
	r->Print("V");                               // print full information of fit including covariance matrix
	cout << "Chi-squared / ndf: " << chi2/ndf << endl;

	// BACKGROUND FUNCTION DRAWING
	TF1 *fBg = new TF1("fBg","[5] + [7]*x + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-lowerFitLimit,x_1+higherFitLimit);
	fBg->SetParameter(5,f1->GetParameter(5));
	fBg->SetParameter(6,f1->GetParameter(6));
	fBg->SetParameter(1,f1->GetParameter(1));
	fBg->SetParameter(2,f1->GetParameter(2));
	fBg->SetParameter(7,f1->GetParameter(7));
	fBg->SetLineColor(kBlack);
	fBg->SetLineStyle(2);//9
	fBg->Draw("same");

	TF1 *fPeak =  new TF1("fPeak","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4])))",x_1-lowerFitLimit,x_1+higherFitLimit);
	fPeak->SetParameter(0,f1->GetParameter(0));
	fPeak->SetParameter(1,f1->GetParameter(1));
	fPeak->SetParameter(2,f1->GetParameter(2));
	fPeak->SetParameter(3,f1->GetParameter(3));
	fPeak->SetParameter(4,f1->GetParameter(4));

	// PEAK AREA EVALUATION
	int firstbin = pE2->FindBin(f1->GetParameter(1) - 8 * f1->GetParameter(2)); // first and last bin of the histogram, whatever the bin width is; histo integrals need bin numbers
	int lastbin  = pE2->FindBin(f1->GetParameter(1) + 8 * f1->GetParameter(2));

	double firstedge = pE2->GetBinLowEdge(firstbin); // gives the energy value at the first bin (if it's 1 keV/ch it is the same, if it is 0.5 keV/ch it isn't); function integrals need energy values
	double lastedge = pE2->GetBinLowEdge(lastbin) + pE2->GetBinWidth(lastbin);

	cout << "firstbin = " <<  firstbin << endl;
	cout << "lastbin = " <<  lastbin << endl;
	cout << "firstedge = " <<  firstedge << endl;
	cout << "lastedge = " <<  lastedge << endl;

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF FITTING FUNCTION (from Jack)
	// (total function minus background function integral)
	double bg_sub_integral = f1->Integral(firstedge,lastedge)/pE2->GetBinWidth(1) - fBg->Integral(firstedge,lastedge)/pE2->GetBinWidth(1);
	double bg_sub_err = TMath::Sqrt( f1->Integral(firstedge,lastedge)/pE2->GetBinWidth(1) + fBg->Integral(firstedge,lastedge)/pE2->GetBinWidth(1));

	cout << "The area of the fitting function minus bkg is " << bg_sub_integral << " +/- " << bg_sub_err << " (" << 100*bg_sub_err/bg_sub_integral << "%)" << endl;

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF THE HISTOGRAM MINUS BACKGROUND FUNCTION (from Jack)
	bg_sub_integral = pE2->Integral(firstbin,lastbin) - fBg->Integral(firstedge,lastedge)/pE2->GetBinWidth(1);
	bg_sub_err = TMath::Sqrt(pE2->Integral(firstbin,lastbin) + fBg->Integral(firstedge,lastedge)/pE2->GetBinWidth(1));

	cout << "The integral of the peak minus bkg is " << bg_sub_integral << " +/- " << bg_sub_err << " (" << 100*bg_sub_err/bg_sub_integral << "%)" << endl;

}

void FitGamma(TH1D* aE_z, int energyFit){
	//TH1D * aE_z = LoadGammaHisto();

	int x_1;
	x_1 = energyFit;

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(500000);
	//TF1 *f1 = new TF1("f1","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4]))) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-20,x_1+20);

	// FITTING FUNCTION
	// two gaussians + step function:
	TF1 *f1 = new TF1("f1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-12,x_1+12); //x_1-15,x_1+20);
	f1->SetParameters(1e6,x_1,0.5,0.5,2,100,200); // initial parameter values
	f1->SetParLimits(1,x_1-2,x_1+2); // centroid
	f1->SetParLimits(2,0.5,5); // sigma
	f1->SetParLimits(3,0.0,1); // relative height of second gaussian, which is shorter than the first one (0,1)
	f1->SetParLimits(4,1,10); // relative sigma of second gaussian, which is broader than the first one (>1)
	aE_z->Fit("f1","R"); // "Q" makes the fit quiet, i.e. no output; "R" limits the range of the fit to the range of the function
	TFitResultPtr r = aE_z->Fit("f1","RS"); // TFitResultPtr contains the TFitResult
	TMatrixDSym cov = r->GetCovarianceMatrix();  //  to access the covariance matrix
	Double_t chi2   = r->Chi2();                 // to retrieve the fit chi2
	Int_t ndf = r->Ndf();
	//Double_t par0   = r->Parameter(0);            // retrieve the value for the parameter 0
	//Double_t err0   = r->ParError(0);             // retrieve the error for the parameter 0
	r->Print("V");                               // print full information of fit including covariance matrix
	cout << "Chi-squared / ndf: " << chi2/ndf << endl;

	// BACKGROUND FUNCTION DRAWING
	TF1 *fBg = new TF1("fBg","[5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-25,x_1+20);
	fBg->SetParameter(5,f1->GetParameter(5));
	fBg->SetParameter(6,f1->GetParameter(6));
	fBg->SetParameter(1,f1->GetParameter(1));
	fBg->SetParameter(2,f1->GetParameter(2));
	fBg->SetLineColor(kBlue);
	fBg->SetLineStyle(9);
	fBg->Draw("same");

	// TWO GAUSSIANS DRAWING
	//TF1 *fGaus1 = new TF1("fGaus1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) )",x_1-25,x_1+20);
	//fGaus1->SetParameter(0,f1->GetParameter(0));
	//fGaus1->SetParameter(1,f1->GetParameter(1));
	//fGaus1->SetParameter(2,f1->GetParameter(2));
	//fGaus1->SetLineColor(kBlue);
	//fGaus1->Draw("same");
	//TF1 *fGaus2 = new TF1("fGaus2","[0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) )",x_1-25,x_1+20);
	//fGaus2->SetParameter(0,f1->GetParameter(0));
	//fGaus2->SetParameter(1,f1->GetParameter(1));
	//fGaus2->SetParameter(2,f1->GetParameter(2));
	//fGaus2->SetParameter(3,f1->GetParameter(3));
	//fGaus2->SetParameter(4,f1->GetParameter(4));
	//fGaus2->SetLineColor(kBlue);
	//fGaus2->Draw("same");

	// PEAK AREA EVALUATION
	int firstbin = aE_z->FindBin(f1->GetParameter(1) - 8 * f1->GetParameter(2)); // first and last bin of the histogram, whatever the bin width is; histo integrals need bin numbers
	int lastbin  = aE_z->FindBin(f1->GetParameter(1) + 8 * f1->GetParameter(2));

	double firstedge = aE_z->GetBinLowEdge(firstbin); // gives the energy value at the first bin (if it's 1 keV/ch it is the same, if it is 0.5 keV/ch it isn't); function integrals need energy values
	double lastedge = aE_z->GetBinLowEdge(lastbin) + aE_z->GetBinWidth(lastbin);

	cout << "firstbin = " <<  firstbin << endl;
	cout << "lastbin = " <<  lastbin << endl;
	cout << "firstedge = " <<  firstedge << endl;
	cout << "lastedge = " <<  lastedge << endl;

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF FITTING FUNCTION (from Jack)
	// (total function minus background function integral)
	double bg_sub_integral = f1->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1) - fBg->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1);
	double bg_sub_err = TMath::Sqrt( f1->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1) + fBg->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1));

	cout << "The area of the fitting function minus bkg is " << bg_sub_integral << " +/- " << bg_sub_err << " (" << 100*bg_sub_err/bg_sub_integral << "%)" << endl;

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF THE HISTOGRAM MINUS BACKGROUND FUNCTION (from Jack)
	bg_sub_integral = aE_z->Integral(firstbin,lastbin) - fBg->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1);
	bg_sub_err = TMath::Sqrt(aE_z->Integral(firstbin,lastbin) + fBg->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1));

	cout << "The integral of the peak minus bkg is " << bg_sub_integral << " +/- " << bg_sub_err << " (" << 100*bg_sub_err/bg_sub_integral << "%)" << endl;

	// ====================================== //
	// remember to write to file those histo with the fits, so that you can recall them; also you should write in a textfile or something all the fit results, to let them be accessible
}

double multiPeakFunction(double *x, double *par) {
	double xcur = x[0];
	double backOffs = par[0];
	double sigma = par[1];
	//double Hstep = par[2];
	double Hsteprelative = par[2];
	double sum = 0;
	sum += backOffs;
	for (int i=0; i<3; i++) { // need to generalize for generic nPeaks but I don't know how
		double H = par[3+4*i];
		double mu = par[4+4*i];
		double Hr = par[5+4*i];
		double sigmar = par[6+4*i];
		//sum += H * exp(-pow((xcur - mu),2)/(2*pow(sigma,2)))  +  H * Hr * exp(-pow((xcur - mu),2)/(2*pow((sigma*sigmar),2))) +  Hstep * (0.5*(1-(ROOT::Math::erf(((xcur - mu)/(TMath::Sqrt(sigma))))))) ;
		sum += H * exp(-pow((xcur - mu),2)/(2*pow(sigma,2)))  +  H * Hr * exp(-pow((xcur - mu),2)/(2*pow((sigma*sigmar),2))) +  Hsteprelative * H * (0.5*(1-(ROOT::Math::erf(((xcur - mu)/(TMath::Sqrt(sigma)))))));
	}
	return sum;
}

void FitGammaTriple(TH1D* aE_z, int energyFit1, int energyFit2, int energyFit3, int lowerFitRange, int upperFitRange){

	int centroids[3] = {energyFit1, energyFit2, energyFit3}; // triple peak fit
	int nPeaks = 3; // triple peak fit
	int numPar = nPeaks*4 + 3;

	TF1 *f1 = new TF1("f1",multiPeakFunction,lowerFitRange,upperFitRange,numPar);
	f1->SetParameter(0,100); // initial value for the background offset
	f1->SetParameter(1,1.0); // initial value for sigma
	f1->SetParLimits(1,0.5,5);
	//f1->SetParameter(2,200); // initial value for Hstep
	f1->SetParameter(2,0.1); // initial value for Hsteprelative
	f1->SetParLimits(2,0.,1.); // limits for Hsteprelative
	for(int i = 0; i < nPeaks; i++) {
		f1->SetParameter(3+i*4,1e6); // initial value for H
		f1->SetParameter(4+i*4,centroids[i]); // initial value for mu
		f1->SetParLimits(4+i*4, centroids[i]-2, centroids[i]+2);
		f1->SetParameter(5+i*4,0.5); // initial value for Hr
		f1->SetParLimits(5+i*4,0,1);
		f1->SetParameter(6+i*4,1); // initial value for sigmar
		f1->SetParLimits(6+i*4,1,10);
	}

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(500000);
	// FITTING FUNCTION
	//aE_z->Fit("f1","R"); // "Q" makes the fit quiet, i.e. no output; "R" limits the range of the fit to the range of the function
	TFitResultPtr r = aE_z->Fit("f1","RS"); // TFitResultPtr contains the TFitResult
	//TMatrixDSym cov = r->GetCovarianceMatrix();   //  to access the covariance matrix
	Double_t chi2   = r->Chi2();                 // to retrieve the fit chi2
	Int_t ndf = r->Ndf();
	r->Print("V");                               // print full information of fit including covariance matrix
	cout << "Chi-squared / ndf: " << chi2/ndf << endl;

	// BACKGROUND FUNCTION DRAWING
	//TF1 *fBg = new TF1("fBg","[5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([0]*2^(0.5))))))) +[7]*(0.5*(1-(ROOT::Math::erf(((x-[2])/([0]*2^(0.5))))))) + [8]*(0.5*(1-(ROOT::Math::erf(((x-[3])/([0]*2^(0.5)))))))",lowerFitRange,upperFitRange);
	TF1 *fBg = new TF1("fBg","[5] + [9]*[6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([0]*2^(0.5))))))) + [10]*[7]*(0.5*(1-(ROOT::Math::erf(((x-[2])/([0]*2^(0.5))))))) + [11]*[8]*(0.5*(1-(ROOT::Math::erf(((x-[3])/([0]*2^(0.5)))))))",lowerFitRange,upperFitRange);
	fBg->SetParameter(5,f1->GetParameter(0)); // background offset
	fBg->SetParameter(6,f1->GetParameter(2)); // Hsteprelative
	fBg->SetParameter(7,f1->GetParameter(2)); // Hsteprelative
	fBg->SetParameter(8,f1->GetParameter(2)); // Hsteprelative
	fBg->SetParameter(0,f1->GetParameter(1)); // sigma
	fBg->SetParameter(1,f1->GetParameter(4)); // mu1
	fBg->SetParameter(2,f1->GetParameter(4+4)); // mu2
	fBg->SetParameter(3,f1->GetParameter(4+8)); // mu3
	fBg->SetParameter(9,f1->GetParameter(3)); // H1
	fBg->SetParameter(10,f1->GetParameter(3+4)); // H2
	fBg->SetParameter(11,f1->GetParameter(3+8)); // H3
	fBg->SetLineColor(kBlack);
	fBg->SetLineStyle(9);
	fBg->Draw("same");

	// SEPARATE THREE PEAKS FUNCTIONS
	TF1 *fPeak1 = new TF1("fPeak1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) ",energyFit1-8*f1->GetParameter(1), energyFit1+8*f1->GetParameter(1));
	fPeak1->SetParameter(0,f1->GetParameter(3)); // H1
	fPeak1->SetParameter(1,f1->GetParameter(4)); // mu1
	fPeak1->SetParameter(2,f1->GetParameter(1)); // sigma
	fPeak1->SetParameter(3,f1->GetParameter(5)); // Hr1
	fPeak1->SetParameter(4,f1->GetParameter(6)); // sigmar1
	fPeak1->SetLineColor(kBlack);
	fPeak1->SetLineStyle(2);
	fPeak1->Draw("same");

	TF1 *fPeak2 = new TF1("fPeak2","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) ",energyFit2-8*f1->GetParameter(1), energyFit2+8*f1->GetParameter(1));
	fPeak2->SetParameter(0,f1->GetParameter(3+4)); // H2
	fPeak2->SetParameter(1,f1->GetParameter(4+4)); // mu2
	fPeak2->SetParameter(2,f1->GetParameter(1)); // sigma
	fPeak2->SetParameter(3,f1->GetParameter(5+4)); // Hr2
	fPeak2->SetParameter(4,f1->GetParameter(6+4)); // sigmar2
	fPeak2->SetLineColor(kBlack);
	fPeak2->SetLineStyle(2);
	fPeak2->Draw("same");

	TF1 *fPeak3 = new TF1("fPeak3","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) ",energyFit3-8*f1->GetParameter(1), energyFit3+8*f1->GetParameter(1));
	fPeak3->SetParameter(0,f1->GetParameter(3+8)); // H3
	fPeak3->SetParameter(1,f1->GetParameter(4+8)); // mu3
	fPeak3->SetParameter(2,f1->GetParameter(1)); // sigma
	fPeak3->SetParameter(3,f1->GetParameter(5+8)); // Hr3
	fPeak3->SetParameter(4,f1->GetParameter(6+8)); // sigmar3
	fPeak3->SetLineColor(kBlack);
	fPeak3->SetLineStyle(2);
	fPeak3->Draw("same");

	// PEAK AREA EVALUATION of PEAK 1
	int firstbin = aE_z->FindBin(f1->GetParameter(4) - 8 * f1->GetParameter(1)); // first and last bin of the histogram, whatever the bin width is; histo integrals need bin numbers
	int lastbin  = aE_z->FindBin(f1->GetParameter(4) + 8 * f1->GetParameter(1));

	double firstedge = aE_z->GetBinLowEdge(firstbin); // gives the energy value at the first bin (if it's 1 keV/ch it is the same, if it is 0.5 keV/ch it isn't); function integrals need energy values
	double lastedge = aE_z->GetBinLowEdge(lastbin) + aE_z->GetBinWidth(lastbin);

	cout << "firstbin = " <<  firstbin << endl;
	cout << "lastbin = " <<  lastbin << endl;
	cout << "firstedge = " <<  firstedge << endl;
	cout << "lastedge = " <<  lastedge << endl;

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF FITTING FUNCTION (from Jack)
	// (total function minus background function integral)
	double bg_sub_integral = fPeak1->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1);
	double bg_sub_err = TMath::Sqrt( fPeak1->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1) + fBg->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1));

	cout << "The area of the fitting function minus bkg is " << std::scientific << bg_sub_integral << " +/- " << bg_sub_err << " (" << 100*bg_sub_err/bg_sub_integral << "%)" << endl;

}

void FitGammaRadware(TH1D* aE_z, int energyFit){
	//TH1D * aE_z = LoadGammaHisto();

	int x_1;
	x_1 = energyFit;

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(500000);
	// RADWARE FITTING FUNCTION with background defined as (1-CDF) see eq. 4.10 of James' thesis
	TF1 *f1 = new TF1("f1","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4]))) + [5] + [6]*(1-0.5*(ROOT::Math::erfc((([1]-x)/([2]*TMath::Sqrt(2)))) + exp(((x-[1])/([3]))+(([2]^2)/(2*[3]^2)))*(1-[4])*ROOT::Math::erfc(([3]*(x-[1])+[2]^2)/[3]*[2]*TMath::Sqrt(2))))",x_1-20,x_1+20);
	//TF1 *f1 = new TF1("f1","[0]*((((exp(-((x-[1])^2/(2*[2]^2)))))*[4])+(exp((x-[1])/([3]))*(ROOT::Math::erfc(((x-[1])/([2]*2^(0.5)))+([2]/([3]*2^(0.5)))))*(1-[4]))) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-15,x_1+20);
	f1->SetParameters(1e6,x_1, 2, 5, 0.2,100,200);
	f1->SetParLimits(1, x_1 - 2, x_1 + 2); //centroid
	f1->SetParLimits(2, 0.2, 5); //sigma of gaussian distribution
	f1->SetParLimits(3, 0.1, 100); //magnitude of step in background noise
	f1->SetParLimits(4, 0.01, 1); //mixing ratio between the gaussian term and the gaussian convoluted with exp

	aE_z->Fit("f1","R"); // "QR" makes the fit quiet, i.e. no output
	cout << "Chi-squared of the fit: " << f1->GetChisquare() << endl;

	// BACKGROUND FUNCTION DRAWING
	TF1 *fBg = new TF1("fBg","[5] + [6]*(1-0.5*(ROOT::Math::erfc((([1]-x)/([2]*TMath::Sqrt(2)))) + exp(((x-[1])/([3]))+(([2]^2)/(2*[3]^2)))*(1-[4])*ROOT::Math::erfc(([3]*(x-[1])+[2]^2)/[3]*[2]*TMath::Sqrt(2))))",x_1-20,x_1+20);
	fBg->SetParameter(5,f1->GetParameter(5));
	fBg->SetParameter(6,f1->GetParameter(6));
	fBg->SetParameter(1,f1->GetParameter(1));
	fBg->SetParameter(2,f1->GetParameter(2));
	fBg->SetParameter(3,f1->GetParameter(3));
	fBg->SetParameter(4,f1->GetParameter(4));
	fBg->SetLineColor(kBlue);
	fBg->SetLineStyle(9);
	fBg->Draw("same");

	// PEAK AREA EVALUATION
	int firstbin = aE_z->FindBin(f1->GetParameter(1) - 8 * f1->GetParameter(2)); // first and last bin of the histogram, whatever the bin width is; histo integrals need bin numbers
	int lastbin  = aE_z->FindBin(f1->GetParameter(1) + 8 * f1->GetParameter(2));

	double firstedge = aE_z->GetBinLowEdge(firstbin); // gives the energy value at the first bin (if it's 1 keV/ch it is the same, if it is 0.5 keV/ch it isn't); function integrals need energy values
	double lastedge = aE_z->GetBinLowEdge(lastbin) + aE_z->GetBinWidth(lastbin);

	cout << "firstbin = " <<  firstbin << endl;
	cout << "lastbin = " <<  lastbin << endl;
	cout << "firstedge = " <<  firstedge << endl;
	cout << "lastedge = " <<  lastedge << endl;

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF FITTING FUNCTION (from Jack)
	// (total function minus background function integral)
	double bg_sub_integral = f1->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1) - fBg->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1);
	double bg_sub_err = TMath::Sqrt( f1->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1) + fBg->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1));

	cout << "The area of the fitting function minus bkg is " << bg_sub_integral << " +/- " << bg_sub_err << " (" << 100*bg_sub_err/bg_sub_integral << "%)" << endl;

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF THE HISTOGRAM MINUS BACKGROUND FUNCTION (from Jack)
	bg_sub_integral = aE_z->Integral(firstbin,lastbin) - fBg->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1);
	bg_sub_err = TMath::Sqrt(aE_z->Integral(firstbin,lastbin) + fBg->Integral(firstedge,lastedge)/aE_z->GetBinWidth(1));

	cout << "The integral of the peak minus bkg is " << bg_sub_integral << " +/- " << bg_sub_err << " (" << 100*bg_sub_err/bg_sub_integral << "%)" << endl;

}

void FitEu(){
	const char* infilename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Calibrations/Histos/S1716finalSelectors/S1716_14423_000.root";
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms
	TH1D * asE_CT = (TH1D*)infile->Get("asE_CT");
	asE_CT->Draw();

	float x_1;
	x_1 = 344;

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(500000);
	// FITTING FUNCTION
	// two gaussians + step function:
	TF1 *f1 = new TF1("f1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-12,x_1+8);
	f1->SetParameters(1e6,x_1,0.5,0.5,2,100,200); // initial parameter values
	f1->SetParLimits(1,x_1-2,x_1+2); // centroid
	f1->SetParLimits(2,0.5,5); // sigma
	f1->SetParLimits(3,0.0,1); // relative height of second gaussian, which is shorter than the first one (0,1)
	f1->SetParLimits(4,1,10); // relative sigma of second gaussian, which is broader than the first one (>1)
	asE_CT->Fit("f1","R"); // "QR" makes the fit quiet, i.e. no output

	// BACKGROUND FUNCTION DRAWING
	TF1 *fBg = new TF1("fBg","[5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-25,x_1+20);
	fBg->SetParameter(5,f1->GetParameter(5));
	fBg->SetParameter(6,f1->GetParameter(6));
	fBg->SetParameter(1,f1->GetParameter(1));
	fBg->SetParameter(2,f1->GetParameter(2));
	fBg->SetLineColor(kBlue);
	fBg->SetLineStyle(9);
	fBg->Draw("same");

	// PEAK AREA EVALUATION
	int firstbin = asE_CT->FindBin(f1->GetParameter(1) - 8 * f1->GetParameter(2)); // first and last bin of the histogram, whatever the bin width is; histo integrals need bin numbers
	int lastbin  = asE_CT->FindBin(f1->GetParameter(1) + 8 * f1->GetParameter(2));

	double firstedge = asE_CT->GetBinLowEdge(firstbin); // gives the energy value at the first bin (if it's 1 keV/ch it is the same, if it is 0.5 keV/ch it isn't); function integrals need energy values
	double lastedge  = asE_CT->GetBinLowEdge(lastbin) + asE_CT->GetBinWidth(lastbin);

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF FITTING FUNCTION (from Jack)
	// (total function minus background function integral)
	double bg_sub_integral = f1->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1) - fBg->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1);
	double bg_sub_err = TMath::Sqrt( f1->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1) + fBg->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1));

	cout << "The area of the fitting function minus bkg is " << bg_sub_integral << " +/- " << bg_sub_err << " (" << 100*bg_sub_err/bg_sub_integral << "%)" << endl;
}

void FitBa(){
	const char* infilename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Calibrations/Histos/S1716finalSelectors/S1716_14420_000.root";
	TFile *infile = new TFile(infilename,"READ");
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms
	TH1D * asE_CT = (TH1D*)infile->Get("asE_CT");
	asE_CT->Draw();

	float x_1;
	x_1 = 356;

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(500000);
	// FITTING FUNCTION
	// two gaussians + step function:
	TF1 *f1 = new TF1("f1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-15,x_1+8);
	f1->SetParameters(1e6,x_1,0.5,0.5,2,100,200); // initial parameter values
	f1->SetParLimits(1,x_1-2,x_1+2); // centroid
	f1->SetParLimits(2,0.5,5); // sigma
	f1->SetParLimits(3,0.0,1); // relative height of second gaussian, which is shorter than the first one (0,1)
	f1->SetParLimits(4,1,10); // relative sigma of second gaussian, which is broader than the first one (>1)
	asE_CT->Fit("f1","R"); // "QR" makes the fit quiet, i.e. no output

	// BACKGROUND FUNCTION DRAWING
	TF1 *fBg = new TF1("fBg","[5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))",x_1-25,x_1+20);
	fBg->SetParameter(5,f1->GetParameter(5));
	fBg->SetParameter(6,f1->GetParameter(6));
	fBg->SetParameter(1,f1->GetParameter(1));
	fBg->SetParameter(2,f1->GetParameter(2));
	fBg->SetLineColor(kBlue);
	fBg->SetLineStyle(9);
	fBg->Draw("same");

	// PEAK AREA EVALUATION
	int firstbin = asE_CT->FindBin(f1->GetParameter(1) - 8 * f1->GetParameter(2)); // first and last bin of the histogram, whatever the bin width is; histo integrals need bin numbers
	int lastbin  = asE_CT->FindBin(f1->GetParameter(1) + 8 * f1->GetParameter(2));

	double firstedge = asE_CT->GetBinLowEdge(firstbin); // gives the energy value at the first bin (if it's 1 keV/ch it is the same, if it is 0.5 keV/ch it isn't); function integrals need energy values
	double lastedge  = asE_CT->GetBinLowEdge(lastbin) + asE_CT->GetBinWidth(lastbin);

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF FITTING FUNCTION (from Jack)
	// (total function minus background function integral)
	double bg_sub_integral = f1->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1) - fBg->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1);
	double bg_sub_err = TMath::Sqrt( f1->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1) + fBg->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1));

	cout << "The area of the fitting function minus bkg is " << bg_sub_integral << " +/- " << bg_sub_err << " (" << 100*bg_sub_err/bg_sub_integral << "%)" << endl;
}

void FitEuBa(){
	TH1::SetDefaultSumw2(); // to propagate correctly errors when summing (or subtracting!!) two histograms
	const char* infilename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Calibrations/Histos/S1716finalSelectors/S1716_14420_000.root";
	TFile *infile = new TFile(infilename,"READ");
	TH1D * asE_CT = (TH1D*)infile->Get("asE_CT");
	infilename = "/tig/megalania_data2/cporzio/S1716_Sept2019/Calibrations/Histos/S1716finalSelectors/S1716_14423_000.root";
	TFile *infile2 = new TFile(infilename,"READ");
	TH1D * asE_CT2 = (TH1D*)infile2->Get("asE_CT");
	asE_CT->Add(asE_CT2,+1);
	asE_CT->Draw();

	float x_1, x_2;
	x_1 = 344;
	x_2 = 356;

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(500000);
	// FITTING FUNCTION
	// two gaussians + step function:
	TF1 *f1 = new TF1("f1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) ) + [5] + [6]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5))))))) + [7]*( exp(-((x-[8])^2/(2*[2]^2))) ) + [7]*[9]*( exp(-((x-[8])^2/(2*[2]^2*[10]^2))) ) + [11]*(0.5*(1-(ROOT::Math::erf(((x-[8])/([2]*2^(0.5)))))))  ",x_1-12,x_2+8);
	cout << "N parameters of two peaks function: " << f1->GetNpar() << endl;
	// f1->SetParameters(1e6,x_1,0.5,0.5,2,100,200,1e6,x_2,0.5,2,200); // initial parameter values for 12 parameters
	f1->SetParameters(1e6,x_1,0.5,0.5,2,100,200,1e6,x_2,0.5,2); // initial parameter values for 11 parameters
	f1->SetParLimits(1,x_1-2,x_1+2); // centroid
	f1->SetParLimits(2,0.5,5); // sigma
	f1->SetParLimits(3,0.0,1); // relative height of second gaussian, which is shorter than the first one (0,1)
	f1->SetParLimits(4,1,10); // relative sigma of second gaussian, which is broader than the first one (>1)
	f1->SetParLimits(8,x_2-2,x_2+2); // centroid
	f1->SetParLimits(9,0.0,1); // relative height of second gaussian, which is shorter than the first one (0,1)
	f1->SetParLimits(10,1,10); // relative sigma of second gaussian, which is broader than the first one (>1)
	asE_CT->Fit("f1","R"); // "QR" makes the fit quiet, i.e. no output

	// FIRST PEAK FUNCTION DRAWING
	TF1 *fPeak1 = new TF1("fPeak1","[0]*( exp(-((x-[1])^2/(2*[2]^2))) ) + [0]*[3]*( exp(-((x-[1])^2/(2*[2]^2*[4]^2))) )",x_1-12,x_2+8);
	fPeak1->SetParameter(0,f1->GetParameter(0));
	fPeak1->SetParameter(1,f1->GetParameter(1));
	fPeak1->SetParameter(2,f1->GetParameter(2));
	fPeak1->SetParameter(3,f1->GetParameter(3));
	fPeak1->SetParameter(4,f1->GetParameter(4));
	fPeak1->SetLineColor(kBlack);
	fPeak1->Draw("same");

	// FIRST PEAK FUNCTION DRAWING
	TF1 *fPeak2 = new TF1("fPeak2","[7]*( exp(-((x-[8])^2/(2*[2]^2))) ) + [7]*[9]*( exp(-((x-[8])^2/(2*[2]^2*[10]^2))) )",x_1-12,x_2+8);
	fPeak2->SetParameter(7,f1->GetParameter(7));
	fPeak2->SetParameter(8,f1->GetParameter(8));
	fPeak2->SetParameter(9,f1->GetParameter(9));
	fPeak2->SetParameter(10,f1->GetParameter(10));
	fPeak2->SetParameter(2,f1->GetParameter(2));
	fPeak2->SetLineColor(kBlack);
	fPeak2->Draw("same");

	// PEAK AREA EVALUATION
	int firstbin = asE_CT->FindBin(f1->GetParameter(1) - 8 * f1->GetParameter(2)); // first and last bin of the histogram, whatever the bin width is; histo integrals need bin numbers
	int lastbin  = asE_CT->FindBin(f1->GetParameter(8) + 8 * f1->GetParameter(2));

	double firstedge = asE_CT->GetBinLowEdge(firstbin); // gives the energy value at the first bin (if it's 1 keV/ch it is the same, if it is 0.5 keV/ch it isn't); function integrals need energy values
	double lastedge  = asE_CT->GetBinLowEdge(lastbin) + asE_CT->GetBinWidth(lastbin);

	// PEAK AREA EVALUATION THROUGH INTEGRAL OF FITTING FUNCTION
	// (separately for the two peaks functions. No background.)
	double peak1_integral = fPeak1->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1);
	double peak1_integral_err = TMath::Sqrt( fPeak1->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1));
	double peak2_integral = fPeak2->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1);
	double peak2_integral_err = TMath::Sqrt( fPeak2->Integral(firstedge,lastedge)/asE_CT->GetBinWidth(1));

	cout << "The area of the peak 1 is " << peak1_integral << " +/- " << peak1_integral_err << " (" << 100*peak1_integral_err/peak1_integral << "%)" << endl;
	cout << "The area of the peak 2 is " << peak2_integral << " +/- " << peak2_integral_err << " (" << 100*peak2_integral_err/peak2_integral << "%)" << endl;
}
