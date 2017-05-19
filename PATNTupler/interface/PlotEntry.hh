#ifndef PLOTENTRY_HH
#define PLOTENTRY_HH


class PlotEntry{

public:
    // constructor
    PlotEntry(const std::string, const TH1F&, const std::string&); // name of entry (eg for legends), histogram template, name of variable we wish to plot (this is for data)
	PlotEntry(const std::string, const TH1F&, const std::string&, const double&); // name of entry (eg for legends), histogram template, name of variable we wish to plot, luminosity (this is for MC)
	
	void AddInput(const std::string&, const std::string&); // flatTree address, selection cut (this is for data)
	void AddInput(const std::string&, const std::string&, const double&); // flatTree address, selection cut, sample cross-section (this is for MC)
	std::string GetPlotEntryName() const;
	TH1F* GetHistogram() const;
	double GetNumberOfEventsBeforeCuts() const;
	double GetNumberOfEventsAfterCuts() const;

private:
	std::string plotEntryName;
	double luminosity;
	std::string variableToPlot;
	TH1F hNull;
	TH1F* hTotal;
	double numberOfEventsBeforeCuts;
};


//--------constructor---------//
PlotEntry::PlotEntry(const std::string plotEntryNameDummy, const TH1F& hTemplate, const std::string& variableToPlotDummy) :
	plotEntryName(plotEntryNameDummy),
	numberOfEventsBeforeCuts(0.0),
	luminosity(0.0),
	hNull(hTemplate),
	variableToPlot(variableToPlotDummy)
{
	Int_t nBins = hTemplate.GetNbinsX();
	hTotal = new TH1F("hTotal", Form("%s;%s;%s", hTemplate.GetTitle(), hTemplate.GetXaxis()->GetTitle(), hTemplate.GetYaxis()->GetTitle()), nBins, hTemplate.GetBinLowEdge(1), hTemplate.GetBinLowEdge(nBins+1));
}

PlotEntry::PlotEntry(const std::string plotEntryNameDummy, const TH1F& hTemplate, const std::string& variableToPlotDummy, const double& luminosityDummy) :
	plotEntryName(plotEntryNameDummy),
	numberOfEventsBeforeCuts(0.0),
	luminosity(luminosityDummy),
	hNull(hTemplate),
	variableToPlot(variableToPlotDummy)
{
	Int_t nBins = hTemplate.GetNbinsX();
	hTotal = new TH1F("hTotal", Form("%s;%s;%s", hTemplate.GetTitle(), hTemplate.GetXaxis()->GetTitle(), hTemplate.GetYaxis()->GetTitle()), nBins, hTemplate.GetBinLowEdge(1), hTemplate.GetBinLowEdge(nBins+1));
}


//-----------public-----------//
void PlotEntry::AddInput(const std::string& flatTreeAddress, const std::string& selectionCut)
{
	TFile * f = TFile::Open(flatTreeAddress.c_str());
    TTree * evT = (TTree*)f->Get("eventCountTree");
    Int_t evTEntries = (Int_t)evT->GetEntries();
	UInt_t nEvtsRunOverForInputEntry;
	evT->SetBranchAddress("nEvtsRunOver",&nEvtsRunOverForInputEntry);        
    UInt_t nEvtsRunOverForInputTotal = 0;
    for (Int_t ievT=0; ievT<evTEntries; ++ievT){
        evT->GetEntry(ievT);
        numberOfEventsBeforeCuts += nEvtsRunOverForInputEntry;
    }

	TTree * T = (TTree*)f->Get("doubleBFatJetPairTree");
	TH1F hContainer = hNull; // make a copy of the empty histogram to fill with TTreeDraw
	hContainer.SetName("hContainer");
	std::string drawStringA = Form("%s>>hContainer", variableToPlot.c_str());
	std::string drawStringB = Form("%s", selectionCut.c_str());
    std::cout << "Filling for TTree: " << flatTreeAddress << std::endl;
	std::cout << "Variable used: " << variableToPlot << std::endl;
	std::cout << "Cut applied: " << drawStringB << std::endl;
	std::cout << "NB: no event weighting" << std::endl;
	T->Draw(drawStringA.c_str(), drawStringB.c_str(), "");
	for (int iBin = 0; iBin < hContainer.GetNbinsX()+2; ++iBin) hTotal->AddBinContent(iBin, hContainer.GetBinContent(iBin));
	std::cout << std::endl;
}

void PlotEntry::AddInput(const std::string& flatTreeAddress, const std::string& selectionCut, const double& crossSection)
{
	TFile * f = TFile::Open(flatTreeAddress.c_str());
    TTree * evT = (TTree*)f->Get("eventCountTree");
    Int_t evTEntries = (Int_t)evT->GetEntries();
	UInt_t nEvtsRunOverForInputEntry;
	evT->SetBranchAddress("nEvtsRunOver",&nEvtsRunOverForInputEntry);        
    UInt_t nEvtsRunOverForInputTotal = 0;
    for (Int_t ievT=0; ievT<evTEntries; ++ievT){
        evT->GetEntry(ievT);
        nEvtsRunOverForInputTotal += nEvtsRunOverForInputEntry;
    }
    numberOfEventsBeforeCuts += 1000.0 * crossSection * luminosity;
    double eventWeighting = 1000.0 * crossSection * luminosity / nEvtsRunOverForInputTotal;

	TTree * T = (TTree*)f->Get("doubleBFatJetPairTree");
	TH1F hContainer = hNull; // make a copy of the empty histogram to fill with TTreeDraw
	hContainer.SetName("hContainer");
	std::string drawStringA = Form("%s>>hContainer", variableToPlot.c_str());
	std::string drawStringB = Form("%f*(%s)", eventWeighting, selectionCut.c_str());
    std::cout << "Filling for TTree: " << flatTreeAddress << std::endl;
	std::cout << "Variable used: " << variableToPlot << std::endl;
	std::cout << "Event Weighting * Cut applied: " << drawStringB << std::endl;
	T->Draw(drawStringA.c_str(), drawStringB.c_str(), "");
	for (int iBin = 0; iBin < hContainer.GetNbinsX()+2; ++iBin)	hTotal->AddBinContent(iBin, hContainer.GetBinContent(iBin));
	std::cout << std::endl;
}


//-----------private----------//
std::string PlotEntry::GetPlotEntryName() const {return plotEntryName;}
TH1F* PlotEntry::GetHistogram() const {return hTotal;}
double PlotEntry::GetNumberOfEventsBeforeCuts() const {return numberOfEventsBeforeCuts;}

double PlotEntry::GetNumberOfEventsAfterCuts() const
{
	double numberOfEventsAfterCuts = 0.0;
	for (int iBin = 0; iBin < hTotal->GetNbinsX()+2; ++iBin) numberOfEventsAfterCuts += hTotal->GetBinContent(iBin);
	return numberOfEventsAfterCuts;
}

#endif