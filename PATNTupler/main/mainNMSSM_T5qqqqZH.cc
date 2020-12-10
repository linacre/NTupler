//STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <random>

// Unix headers
#include <sys/stat.h>

//ROOT HEADERS
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
#include <TString.h>
#include <TLorentzVector.h> 
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

//RAL PARTICLE HEADERS
#include "NtpReader.hh"
#include "GoodLumiChecker.hh"
#include "TriggerPathToIndex.hh"
#include "deltaR.h"
#include "DoubleBTagSF.h"
#include "IsrWeights.h"
#include "SoftDropPuppiSF.h"

using std::cout;
using std::ifstream;
using std::ofstream;



/*
Compilation:
within main directory run
$ gmake -j 8 <executableNameSetInGNUmakefile>

Executing:
./<executableNameSetInGNUmakefile> <outputRootFilename> <pathToListOfInputFiles> <sampleType> <runInstructions> <lumiJson>

NB:
runInstructions = "local" (for local runs)
runInstructions = "batch" (for running on batch systems)
the lumi json file does not need to be included
*/


// function to check whether jet passes looseID
bool isJetLooseID(const ran::NtJet jet)
{
	const double absEta = fabs( jet.eta() );
	const double NHF  = jet.neutralHadronEnergyFraction();
	const double NEMF = jet.neutralEmEnergyFraction();
	const double CHF  = jet.chargedHadronEnergyFraction();
	// const double MUF  = jet.muonEnergyFraction();
	const double CEMF = jet.chargedEmEnergyFraction();
	const int NumConst = jet.chargedMultiplicity()+jet.neutralMultiplicity();
	const int NumNeutralParticles =jet.neutralMultiplicity();
	const int CHM      = jet.chargedMultiplicity(); 

	if (absEta <= 2.7){
		if ( (NHF<0.99 && NEMF<0.99 && NumConst>1) && ((absEta<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || absEta>2.4) )
			return true;
		else
			return false;
	}

	else if (absEta <= 3.0){
		if ( NHF<0.98 && NEMF>0.01 && NumNeutralParticles>2 )
			return true;
		else
			return false;
	}

	else{
		if ( NEMF<0.90 && NumNeutralParticles>10 )
			return true;
		else
			return false;
	}

}


namespace tsw {

/*!
 * \class TreeHandlerBase
 * \brief Base class for tree handlers that implements all common functionality: saveToFile method, destruction of all trees
 */
class TreeHandlerBase{
public:
	// CTOR & DTOR
	TreeHandlerBase(const std::string& mainTreeName, const std::string& mainTreeDescription, const std::string& fileName);
	virtual ~TreeHandlerBase();

	// PUBLIC METHODS
	void setEventCounter(unsigned int nEvtsTotal, std::map<std::string,UInt_t> totNumEvtsAnalyzedMotherMass){
		totNumEvtsAnalyzed_ = nEvtsTotal;
		numEvtsPass_ = mainAnaTree_->GetEntries();
		totNumEvtsAnalyzedMotherMass_ = totNumEvtsAnalyzedMotherMass;
		std::cout << "Number of events run over: " << nEvtsTotal << "   Number of events passed: " << numEvtsPass_ << std::endl;
		for (auto const& x : totNumEvtsAnalyzedMotherMass_)
		{
			std::cout << x.first  // string (key)
					<< ':' 
					<< x.second // string's value 
					<< std::endl ;
		}
	}
	void saveToFile();

	std::string fileName() const { return fileName_; }

protected:
	TTree* addTree(const std::string& treeName, const std::string& treeDescription ); ///< Returns TTree with specified name/description on heap and registers it for saving to file

	static bool isMadgraphDrellYan(const std::string& fileName) { return (fileName.find("DY")!=std::string::npos && (fileName.find("MG")!=std::string::npos || fileName.find("MADGRAPH")!=std::string::npos) ); }

private:
	// PRIVATE MEMBERS
	std::string fileName_;
	TFile* outputFile_;
	TTree* eventCountTree_;

	UInt_t numEvtsPass_;
	UInt_t totNumEvtsAnalyzed_;
	//std::vector<UInt_t> totNumEvtsAnalyzedMotherMass_;
	std::map<std::string,UInt_t> totNumEvtsAnalyzedMotherMass_;

protected:
	// PROTECTED MEMBERS
	TTree* mainAnaTree_;
	std::vector<TTree*> otherTrees_;
};


TreeHandlerBase::TreeHandlerBase(const std::string& mainTreeName, const std::string& mainTreeDescription, const std::string& fileName) :
	fileName_( fileName ),
	outputFile_( new TFile(fileName.c_str(), "RECREATE") ),
	eventCountTree_( new TTree("eventCountTree", "Tree containing orig event counts per file.") ),
	numEvtsPass_(0), totNumEvtsAnalyzed_(0),
	mainAnaTree_( new TTree(mainTreeName.c_str(), mainTreeDescription.c_str()) )
{
	mainAnaTree_->SetDirectory( outputFile_ );
	eventCountTree_->SetDirectory( outputFile_ );

	eventCountTree_->Branch("nEvtsPass", &numEvtsPass_, "nEvtsPass/i");
	eventCountTree_->Branch("nEvtsRunOver", &totNumEvtsAnalyzed_, "nEvtsRunOver/i");
	eventCountTree_->Branch("nEvtsRunOverMotherMass","std::map<std::string,UInt_t>",&totNumEvtsAnalyzedMotherMass_);
}

TreeHandlerBase::~TreeHandlerBase()
{
	delete outputFile_; // TTrees themselves should not be deleted since they reside in the file (??)
}

void TreeHandlerBase::saveToFile()
{
	// Firstly fill eventCountTree_
	eventCountTree_->Fill();

	// Then, write TTrees to file ...
	outputFile_->cd();
	mainAnaTree_->Write();
	eventCountTree_->Write();
	for(size_t idx=0; idx<otherTrees_.size(); idx++)
		otherTrees_.at(idx)->Write();
	outputFile_->Close();
}

TTree* TreeHandlerBase::addTree(const std::string& treeName, const std::string& treeDescription )
{
	TTree* treePtr = new TTree( treeName.c_str(), treeDescription.c_str() );
	treePtr->SetDirectory( outputFile_ );
	otherTrees_.push_back(treePtr);
	return treePtr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FatDoubleBJetPairTree: Simple class for generating trees containing info about
//                        two fat jets with highest double-b tag discriminant from each event containing at least two fat jets

class FatDoubleBJetPairTree : public TreeHandlerBase {
private:
	// PRIVATE MEMBERS

	// For the event & kinematic branches ...
	Double_t treeVar_weight_combined_;
	
	Double_t treeVar_weight_dbtTag_;
	Double_t treeVar_weight_dbtTagUp_;
	Double_t treeVar_weight_dbtTagDown_;

	Double_t treeVar_weight_isr_;
	Double_t treeVar_weight_isrUp_;
	Double_t treeVar_weight_isrDown_;

	Float_t treeVar_weight_prefire_;
	Float_t treeVar_weight_prefireUp_;
	Float_t treeVar_weight_prefireDown_;

	Float_t treeVar_weight_scaleUp_;
	Float_t treeVar_weight_scaleDown_;

	UInt_t treeVar_runNum_;
	UInt_t treeVar_lumiSec_;
	UInt_t treeVar_evtNum_;

	Bool_t treeVar_trgDecision_;
	UInt_t treeVar_nPU_;
	Double_t treeVar_nTrueInt_;
	UInt_t treeVar_nGluino_;

	UInt_t treeVar_nHiggs2bb_;
	UInt_t treeVar_numHiggses_;
	Float_t treeVar_motherMass_;
	Float_t treeVar_lspMass_;

	UInt_t treeVar_nPrefireCandidates_;

	TLorentzVector* treeVar_fatJetA_p4Ptr_; TLorentzVector treeVar_fatJetA_p4_;
	TLorentzVector* treeVar_fatJetA_p4Ptr_jecAK8UncUp_; TLorentzVector treeVar_fatJetA_p4_jecAK8UncUp_;
	TLorentzVector* treeVar_fatJetA_p4Ptr_jecAK8UncDown_; TLorentzVector treeVar_fatJetA_p4_jecAK8UncDown_;
	TLorentzVector* treeVar_fatJetA_p4Ptr_jerAK8UncUp_; TLorentzVector treeVar_fatJetA_p4_jerAK8UncUp_;
	TLorentzVector* treeVar_fatJetA_p4Ptr_jerAK8UncDown_; TLorentzVector treeVar_fatJetA_p4_jerAK8UncDown_;

	TLorentzVector* treeVar_fatJetB_p4Ptr_; TLorentzVector treeVar_fatJetB_p4_;
	TLorentzVector* treeVar_fatJetB_p4Ptr_jecAK8UncUp_; TLorentzVector treeVar_fatJetB_p4_jecAK8UncUp_;
	TLorentzVector* treeVar_fatJetB_p4Ptr_jecAK8UncDown_; TLorentzVector treeVar_fatJetB_p4_jecAK8UncDown_;
	TLorentzVector* treeVar_fatJetB_p4Ptr_jerAK8UncUp_; TLorentzVector treeVar_fatJetB_p4_jerAK8UncUp_;
	TLorentzVector* treeVar_fatJetB_p4Ptr_jerAK8UncDown_; TLorentzVector treeVar_fatJetB_p4_jerAK8UncDown_;

	Double_t treeVar_fatJetA_doubleBtagDiscrim_;
	Double_t treeVar_fatJetA_mass_;
	Double_t treeVar_fatJetA_softDropMassCHS_;
	Double_t treeVar_fatJetA_softDropMassPuppi_;
	Double_t treeVar_fatJetA_softDropMassPuppi_jmsUncUp_;
	Double_t treeVar_fatJetA_softDropMassPuppi_jmsUncDown_;
	Double_t treeVar_fatJetA_softDropMassPuppi_jmrUncUp_;
	Double_t treeVar_fatJetA_softDropMassPuppi_jmrUncDown_;
	Double_t treeVar_fatJetA_prunedMass_;
	Float_t treeVar_fatJetA_electronEnergyFraction_;
	Float_t treeVar_fatJetA_muonEnergyFraction_;
	Float_t treeVar_fatJetA_nSubjettinessTau1_;
	Float_t treeVar_fatJetA_nSubjettinessTau2_;
	Double_t treeVar_fatJetB_doubleBtagDiscrim_;
	Double_t treeVar_fatJetB_mass_;
	Double_t treeVar_fatJetB_softDropMassCHS_;
	Double_t treeVar_fatJetB_softDropMassPuppi_;
	Double_t treeVar_fatJetB_softDropMassPuppi_jmsUncUp_;
	Double_t treeVar_fatJetB_softDropMassPuppi_jmsUncDown_;
	Double_t treeVar_fatJetB_softDropMassPuppi_jmrUncUp_;
	Double_t treeVar_fatJetB_softDropMassPuppi_jmrUncDown_;
	Double_t treeVar_fatJetB_prunedMass_;
	Float_t treeVar_fatJetB_electronEnergyFraction_;
	Float_t treeVar_fatJetB_muonEnergyFraction_;
	Float_t treeVar_fatJetB_nSubjettinessTau1_;
	Float_t treeVar_fatJetB_nSubjettinessTau2_;

	Float_t treeVar_ht_;
	Float_t treeVar_ht_jecAK4UncUp_;
	Float_t treeVar_ht_jecAK4UncDown_;
	Float_t treeVar_ht_jerAK4UncUp_;
	Float_t treeVar_ht_jerAK4UncDown_;

	UInt_t treeVar_nrSlimJets_;
	
	TLorentzVector* treeVar_jetA_p4Ptr_; TLorentzVector treeVar_jetA_p4_;
	TLorentzVector* treeVar_jetA_p4Ptr_jecAK4UncUp_; TLorentzVector treeVar_jetA_p4_jecAK4UncUp_;
	TLorentzVector* treeVar_jetA_p4Ptr_jecAK4UncDown_; TLorentzVector treeVar_jetA_p4_jecAK4UncDown_;
	TLorentzVector* treeVar_jetA_p4Ptr_jerAK4UncUp_; TLorentzVector treeVar_jetA_p4_jerAK4UncUp_;
	TLorentzVector* treeVar_jetA_p4Ptr_jerAK4UncDown_; TLorentzVector treeVar_jetA_p4_jerAK4UncDown_;

	TLorentzVector* treeVar_jetB_p4Ptr_; TLorentzVector treeVar_jetB_p4_;
	TLorentzVector* treeVar_jetB_p4Ptr_jecAK4UncUp_; TLorentzVector treeVar_jetB_p4_jecAK4UncUp_;
	TLorentzVector* treeVar_jetB_p4Ptr_jecAK4UncDown_; TLorentzVector treeVar_jetB_p4_jecAK4UncDown_;
	TLorentzVector* treeVar_jetB_p4Ptr_jerAK4UncUp_; TLorentzVector treeVar_jetB_p4_jerAK4UncUp_;
	TLorentzVector* treeVar_jetB_p4Ptr_jerAK4UncDown_; TLorentzVector treeVar_jetB_p4_jerAK4UncDown_;

	Double_t treeVar_muon_maxPt_;
	Double_t treeVar_muon_sumPt_;

public:
	FatDoubleBJetPairTree(const std::string& treeName, const std::string& fileName) :
		TreeHandlerBase(treeName, "Tree of double-b fat jet candidates ("+treeName+")", fileName),
		
		treeVar_fatJetA_p4Ptr_( &treeVar_fatJetA_p4_ ),
		treeVar_fatJetA_p4Ptr_jecAK8UncUp_( &treeVar_fatJetA_p4_jecAK8UncUp_ ),
		treeVar_fatJetA_p4Ptr_jecAK8UncDown_( &treeVar_fatJetA_p4_jecAK8UncDown_ ),
		treeVar_fatJetA_p4Ptr_jerAK8UncUp_( &treeVar_fatJetA_p4_jerAK8UncUp_ ),
		treeVar_fatJetA_p4Ptr_jerAK8UncDown_( &treeVar_fatJetA_p4_jerAK8UncDown_ ),

		treeVar_fatJetB_p4Ptr_( &treeVar_fatJetB_p4_ ),
		treeVar_fatJetB_p4Ptr_jecAK8UncUp_( &treeVar_fatJetB_p4_jecAK8UncUp_ ),
		treeVar_fatJetB_p4Ptr_jecAK8UncDown_( &treeVar_fatJetB_p4_jecAK8UncDown_ ),
		treeVar_fatJetB_p4Ptr_jerAK8UncUp_( &treeVar_fatJetB_p4_jerAK8UncUp_ ),
		treeVar_fatJetB_p4Ptr_jerAK8UncDown_( &treeVar_fatJetB_p4_jerAK8UncDown_ ),

		treeVar_jetA_p4Ptr_( &treeVar_jetA_p4_ ),
		treeVar_jetA_p4Ptr_jecAK4UncUp_( &treeVar_jetA_p4_jecAK4UncUp_ ),
		treeVar_jetA_p4Ptr_jecAK4UncDown_( &treeVar_jetA_p4_jecAK4UncDown_ ),
		treeVar_jetA_p4Ptr_jerAK4UncUp_( &treeVar_jetA_p4_jerAK4UncUp_ ),
		treeVar_jetA_p4Ptr_jerAK4UncDown_( &treeVar_jetA_p4_jerAK4UncDown_ ),

		treeVar_jetB_p4Ptr_( &treeVar_jetB_p4_ ),
		treeVar_jetB_p4Ptr_jecAK4UncUp_( &treeVar_jetB_p4_jecAK4UncUp_ ),
		treeVar_jetB_p4Ptr_jecAK4UncDown_( &treeVar_jetB_p4_jecAK4UncDown_ ),
		treeVar_jetB_p4Ptr_jerAK4UncUp_( &treeVar_jetB_p4_jerAK4UncUp_ ),
		treeVar_jetB_p4Ptr_jerAK4UncDown_( &treeVar_jetB_p4_jerAK4UncDown_ )
	{
		mainAnaTree_->Branch("weight_combined",     &treeVar_weight_combined_,     "weight_combined/D");
		
		mainAnaTree_->Branch("weight_dbtTag",     &treeVar_weight_dbtTag_,     "weight_dbtTag/D");
		mainAnaTree_->Branch("weight_dbtTagUp",   &treeVar_weight_dbtTagUp_,   "weight_dbtTagUp/D");
		mainAnaTree_->Branch("weight_dbtTagDown", &treeVar_weight_dbtTagDown_, "weight_dbtTagDown/D");

		mainAnaTree_->Branch("weight_isr",     &treeVar_weight_isr_,     "weight_isr/D");
		mainAnaTree_->Branch("weight_isrUp",   &treeVar_weight_isrUp_,   "weight_isrUp/D");
		mainAnaTree_->Branch("weight_isrDown", &treeVar_weight_isrDown_, "weight_isrDown/D");

		mainAnaTree_->Branch("weight_prefire",     &treeVar_weight_prefire_,     "weight_prefire/F");
		mainAnaTree_->Branch("weight_prefireUp",   &treeVar_weight_prefireUp_,   "weight_prefireUp/F");
		mainAnaTree_->Branch("weight_prefireDown", &treeVar_weight_prefireDown_, "weight_prefireDown/F");

		mainAnaTree_->Branch("weight_scaleUp",   &treeVar_weight_scaleUp_,   "weight_scaleUp/F");
		mainAnaTree_->Branch("weight_scaleDown", &treeVar_weight_scaleDown_, "weight_scaleDown/F");

		mainAnaTree_->Branch("run",    &treeVar_runNum_,   "run/i");
		mainAnaTree_->Branch("lumi",   &treeVar_lumiSec_,   "lumi/i");
		mainAnaTree_->Branch("evtNum", &treeVar_evtNum_,   "evtNum/i");

		mainAnaTree_->Branch("trgDecision", &treeVar_trgDecision_, "trgDecision/O");
		mainAnaTree_->Branch("nPU", &treeVar_nPU_, "nPU/i");
		mainAnaTree_->Branch("nTrueInt", &treeVar_nTrueInt_, "nTrueInt/D");
		mainAnaTree_->Branch("nGluino", &treeVar_nGluino_, "nGluino/i");

		mainAnaTree_->Branch("nHiggs2bb", &treeVar_nHiggs2bb_, "nHiggs2bb/I");
		mainAnaTree_->Branch("numHiggses", &treeVar_numHiggses_, "numHiggses/I");
		mainAnaTree_->Branch("motherMass", &treeVar_motherMass_, "motherMass/F");
		mainAnaTree_->Branch("lspMass", &treeVar_lspMass_, "lspMass/F");
		
		mainAnaTree_->Branch("nPrefireCandidates", &treeVar_nPrefireCandidates_, "nPrefireCandidates/i");

		mainAnaTree_->Branch("fatJetA_p4", &treeVar_fatJetA_p4Ptr_);
		mainAnaTree_->Branch("fatJetA_p4_jecAK8UncUp", &treeVar_fatJetA_p4Ptr_jecAK8UncUp_);
		mainAnaTree_->Branch("fatJetA_p4_jecAK8UncDown", &treeVar_fatJetA_p4Ptr_jecAK8UncDown_);
		mainAnaTree_->Branch("fatJetA_p4_jerAK8UncUp", &treeVar_fatJetA_p4Ptr_jerAK8UncUp_);
		mainAnaTree_->Branch("fatJetA_p4_jerAK8UncDown", &treeVar_fatJetA_p4Ptr_jerAK8UncDown_);

		mainAnaTree_->Branch("fatJetB_p4", &treeVar_fatJetB_p4Ptr_);
		mainAnaTree_->Branch("fatJetB_p4_jecAK8UncUp", &treeVar_fatJetB_p4Ptr_jecAK8UncUp_);
		mainAnaTree_->Branch("fatJetB_p4_jecAK8UncDown", &treeVar_fatJetB_p4Ptr_jecAK8UncDown_);
		mainAnaTree_->Branch("fatJetB_p4_jerAK8UncUp", &treeVar_fatJetB_p4Ptr_jerAK8UncUp_);
		mainAnaTree_->Branch("fatJetB_p4_jerAK8UncDown", &treeVar_fatJetB_p4Ptr_jerAK8UncDown_);

		mainAnaTree_->Branch("fatJetA_doubleBtagDiscrim", &treeVar_fatJetA_doubleBtagDiscrim_,   "fatJetA_doubleBtagDiscrim/D");
		mainAnaTree_->Branch("fatJetB_doubleBtagDiscrim", &treeVar_fatJetB_doubleBtagDiscrim_,   "fatJetB_doubleBtagDiscrim/D");

		mainAnaTree_->Branch("fatJetA_mass", &treeVar_fatJetA_mass_,   "fatJetA_mass/D");
		mainAnaTree_->Branch("fatJetB_mass", &treeVar_fatJetB_mass_,   "fatJetB_mass/D");

		mainAnaTree_->Branch("fatJetA_prunedMass", &treeVar_fatJetA_prunedMass_,   "fatJetA_prunedMass/D");
		mainAnaTree_->Branch("fatJetB_prunedMass", &treeVar_fatJetB_prunedMass_,   "fatJetB_prunedMass/D");

		mainAnaTree_->Branch("fatJetA_softDropMassCHS", &treeVar_fatJetA_softDropMassCHS_,   "fatJetA_softDropMassCHS/D");
		mainAnaTree_->Branch("fatJetB_softDropMassCHS", &treeVar_fatJetB_softDropMassCHS_,   "fatJetB_softDropMassCHS/D");

		mainAnaTree_->Branch("fatJetA_softDropMassPuppi", &treeVar_fatJetA_softDropMassPuppi_,   "fatJetA_softDropMassPuppi/D");
		mainAnaTree_->Branch("fatJetA_softDropMassPuppi_jmsUncUp", &treeVar_fatJetA_softDropMassPuppi_jmsUncUp_,   "fatJetA_softDropMassPuppi_jmsUncUp/D");
		mainAnaTree_->Branch("fatJetA_softDropMassPuppi_jmsUncDown", &treeVar_fatJetA_softDropMassPuppi_jmsUncDown_,   "fatJetA_softDropMassPuppi_jmsUncDown/D");
		mainAnaTree_->Branch("fatJetA_softDropMassPuppi_jmrUncUp", &treeVar_fatJetA_softDropMassPuppi_jmrUncUp_,   "fatJetA_softDropMassPuppi_jmrUncUp/D");
		mainAnaTree_->Branch("fatJetA_softDropMassPuppi_jmrUncDown", &treeVar_fatJetA_softDropMassPuppi_jmrUncDown_,   "fatJetA_softDropMassPuppi_jmrUncDown/D");
		
		mainAnaTree_->Branch("fatJetB_softDropMassPuppi", &treeVar_fatJetB_softDropMassPuppi_,   "fatJetB_softDropMassPuppi/D");
		mainAnaTree_->Branch("fatJetB_softDropMassPuppi_jmsUncUp", &treeVar_fatJetB_softDropMassPuppi_jmsUncUp_,   "fatJetB_softDropMassPuppi_jmsUncUp/D");
		mainAnaTree_->Branch("fatJetB_softDropMassPuppi_jmsUncDown", &treeVar_fatJetB_softDropMassPuppi_jmsUncDown_,   "fatJetB_softDropMassPuppi_jmsUncDown/D");
		mainAnaTree_->Branch("fatJetB_softDropMassPuppi_jmrUncUp", &treeVar_fatJetB_softDropMassPuppi_jmrUncUp_,   "fatJetB_softDropMassPuppi_jmrUncUp/D");
		mainAnaTree_->Branch("fatJetB_softDropMassPuppi_jmrUncDown", &treeVar_fatJetB_softDropMassPuppi_jmrUncDown_,   "fatJetB_softDropMassPuppi_jmrUncDown/D");

		mainAnaTree_->Branch("fatJetA_electronEnergyFraction", &treeVar_fatJetA_electronEnergyFraction_, "fatJetA_electronEnergyFraction/F");
		mainAnaTree_->Branch("fatJetB_electronEnergyFraction", &treeVar_fatJetB_electronEnergyFraction_, "fatJetB_electronEnergyFraction/F");

		mainAnaTree_->Branch("fatJetA_muonEnergyFraction", &treeVar_fatJetA_muonEnergyFraction_, "fatJetA_muonEnergyFraction/F");
		mainAnaTree_->Branch("fatJetB_muonEnergyFraction", &treeVar_fatJetB_muonEnergyFraction_, "fatJetB_muonEnergyFraction/F");

		mainAnaTree_->Branch("fatJetA_nSubjettinessTau1", &treeVar_fatJetA_nSubjettinessTau1_, "fatJetA_nSubjettinessTau1/F");
		mainAnaTree_->Branch("fatJetA_nSubjettinessTau2", &treeVar_fatJetA_nSubjettinessTau2_, "fatJetA_nSubjettinessTau2/F");
		mainAnaTree_->Branch("fatJetB_nSubjettinessTau1", &treeVar_fatJetB_nSubjettinessTau1_, "fatJetB_nSubjettinessTau1/F");
		mainAnaTree_->Branch("fatJetB_nSubjettinessTau2", &treeVar_fatJetB_nSubjettinessTau2_, "fatJetB_nSubjettinessTau2/F");

		mainAnaTree_->Branch("ht", &treeVar_ht_, "ht/F");
		mainAnaTree_->Branch("ht_jecAK4UncUp", &treeVar_ht_jecAK4UncUp_, "ht_jecAK4UncUp/F");
		mainAnaTree_->Branch("ht_jecAK4UncDown", &treeVar_ht_jecAK4UncDown_, "ht_jecAK4UncDown/F");
		mainAnaTree_->Branch("ht_jerAK4UncUp", &treeVar_ht_jerAK4UncUp_, "ht_jerAK4UncUp/F");
		mainAnaTree_->Branch("ht_jerAK4UncDown", &treeVar_ht_jerAK4UncDown_, "ht_jerAK4UncDown/F");

		mainAnaTree_->Branch("nrSlimJets", &treeVar_nrSlimJets_, "nrSlimJets/i");
		
		mainAnaTree_->Branch("slimJetA_p4", &treeVar_jetA_p4Ptr_);
		mainAnaTree_->Branch("slimJetA_p4_jecAK4UncUp", &treeVar_jetA_p4Ptr_jecAK4UncUp_);
		mainAnaTree_->Branch("slimJetA_p4_jecAK4UncDown", &treeVar_jetA_p4Ptr_jecAK4UncDown_);
		mainAnaTree_->Branch("slimJetA_p4_jerAK4UncUp", &treeVar_jetA_p4Ptr_jerAK4UncUp_);
		mainAnaTree_->Branch("slimJetA_p4_jerAK4UncDown", &treeVar_jetA_p4Ptr_jerAK4UncDown_);

		mainAnaTree_->Branch("slimJetB_p4", &treeVar_jetB_p4Ptr_);
		mainAnaTree_->Branch("slimJetB_p4_jecAK4UncUp", &treeVar_jetB_p4Ptr_jecAK4UncUp_);
		mainAnaTree_->Branch("slimJetB_p4_jecAK4UncDown", &treeVar_jetB_p4Ptr_jecAK4UncDown_);
		mainAnaTree_->Branch("slimJetB_p4_jerAK4UncUp", &treeVar_jetB_p4Ptr_jerAK4UncUp_);
		mainAnaTree_->Branch("slimJetB_p4_jerAK4UncDown", &treeVar_jetB_p4Ptr_jerAK4UncDown_);

		mainAnaTree_->Branch("muon_maxPt", &treeVar_muon_maxPt_,   "muon_maxPt/D");
		mainAnaTree_->Branch("muon_sumPt", &treeVar_muon_sumPt_,   "muon_sumPt/D");
	}

	~FatDoubleBJetPairTree(){}

	void fillTree(const std::string& sampleType, const ran::EventInfo& evtInfo, const ran::NtFatJet& fatJetA, const ran::NtFatJet& fatJetB, const float& ht, const float& ht_jecAK4UncUp, const float& ht_jecAK4UncDown, const float& ht_jerAK4UncUp, const float& ht_jerAK4UncDown, const std::vector<ran::NtJet>& slimJets, const bool& trigDecision, const int& nPU, const float& nTrueInt, int nISR, const int& nGluino,
const int& nHiggs2bb,
const int& numHiggses,
const float& motherMass,
const float& lspMass,
	const double& D_factor, const unsigned int& yearOfRun, const double& muon_maxPt, const double& muon_sumPt, const float& corr_prefire, const float& corr_prefireUp, const float& corr_prefireDown, const unsigned int& nPrefireCandidates, const float& corr_scaleUp, const float& corr_scaleDown)
	{
		
		// DO THE WEIGHTS

		if (sampleType == "SIGNAL"){

			treeVar_weight_dbtTag_ = DoubleBTagSF::getDbtTagScaleFactor_signal(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);
			treeVar_weight_dbtTagUp_ = DoubleBTagSF::getDbtTagScaleFactorUp_signal(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);
			treeVar_weight_dbtTagDown_ = DoubleBTagSF::getDbtTagScaleFactorDown_signal(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);

			// calculate isr weights
			if (nISR > 6) nISR = 6;
			double corr_isr = D_factor * IsrWeights::GetWeight(nISR, yearOfRun);
			treeVar_weight_isr_ = corr_isr;
			treeVar_weight_isrUp_ = corr_isr + 0.5 * (corr_isr - 1);
			treeVar_weight_isrDown_ = corr_isr - 0.5 * (corr_isr - 1);

			// pre-fire weights
			treeVar_weight_prefire_ = corr_prefire;
			treeVar_weight_prefireUp_ = corr_prefireUp;
			treeVar_weight_prefireDown_ = corr_prefireDown;

			// scale uncertainties
			treeVar_weight_scaleUp_ = corr_scaleUp;
			treeVar_weight_scaleDown_ = corr_scaleDown;

			// combined nominal scale factor weights
			treeVar_weight_combined_ = treeVar_weight_dbtTag_ * treeVar_weight_isr_ * treeVar_weight_prefire_;	
		}
		
		else if (sampleType == "TTJETS"){

			treeVar_weight_dbtTag_ = DoubleBTagSF::getDbtTagScaleFactor_ttbar(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);
			treeVar_weight_dbtTagUp_ = DoubleBTagSF::getDbtTagScaleFactorUp_ttbar(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);
			treeVar_weight_dbtTagDown_ = DoubleBTagSF::getDbtTagScaleFactorDown_ttbar(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);

			treeVar_weight_isr_ = 1.0;
			treeVar_weight_isrUp_ = 1.0;
			treeVar_weight_isrDown_ = 1.0;

			// pre-fire weights
			treeVar_weight_prefire_ = corr_prefire;
			treeVar_weight_prefireUp_ = corr_prefireUp;
			treeVar_weight_prefireDown_ = corr_prefireDown;

			// scale uncertainties
			treeVar_weight_scaleUp_ = corr_scaleUp;
			treeVar_weight_scaleDown_ = corr_scaleDown;
			
			// combined nominal scale factor weights
			treeVar_weight_combined_ = treeVar_weight_dbtTag_ * treeVar_weight_prefire_;
		}

		else if (sampleType == "OTHER_MC"){

			treeVar_weight_dbtTag_ = 1.0;
			treeVar_weight_dbtTagUp_ = 1.0;
			treeVar_weight_dbtTagDown_ = 1.0;

			treeVar_weight_isr_ = 1.0;
			treeVar_weight_isrUp_ = 1.0;
			treeVar_weight_isrDown_ = 1.0;

			// pre-fire weights
			treeVar_weight_prefire_ = corr_prefire;
			treeVar_weight_prefireUp_ = corr_prefireUp;
			treeVar_weight_prefireDown_ = corr_prefireDown;

			// scale uncertainties
			treeVar_weight_scaleUp_ = corr_scaleUp;
			treeVar_weight_scaleDown_ = corr_scaleDown;

			// combined nominal scale factor weights
			treeVar_weight_combined_ = treeVar_weight_prefire_;		
		}

		else{
			treeVar_weight_dbtTag_ = 1.0;
			treeVar_weight_dbtTagUp_ = 1.0;
			treeVar_weight_dbtTagDown_ = 1.0;

			treeVar_weight_isr_ = 1.0;
			treeVar_weight_isrUp_ = 1.0;
			treeVar_weight_isrDown_ = 1.0;

			treeVar_weight_prefire_ = 1.0;
			treeVar_weight_prefireUp_ = 1.0;
			treeVar_weight_prefireDown_ = 1.0;

			treeVar_weight_scaleUp_ = 1.0;
			treeVar_weight_scaleDown_ = 1.0;

			// combined nominal scale factor weights
			treeVar_weight_combined_ = 1.0;
		}


		// DO THE VARIABLES

		treeVar_runNum_ = evtInfo.runNum;
		treeVar_lumiSec_ = evtInfo.lumiSec;
		treeVar_evtNum_ = evtInfo.evtNum;

		treeVar_trgDecision_ = trigDecision;
		treeVar_nPU_ = nPU;
		treeVar_nTrueInt_ = nTrueInt;
		treeVar_nGluino_ = nGluino;

		treeVar_nHiggs2bb_ = nHiggs2bb;
		treeVar_numHiggses_ = numHiggses;
		treeVar_motherMass_ = motherMass;
		treeVar_lspMass_ = lspMass;

		treeVar_nPrefireCandidates_ = nPrefireCandidates;

		treeVar_fatJetA_p4_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));
		treeVar_fatJetB_p4_.SetPtEtaPhiE(fatJetB.pt(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()));

		treeVar_fatJetA_doubleBtagDiscrim_ = fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags();
		treeVar_fatJetB_doubleBtagDiscrim_ = fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags();

		treeVar_fatJetA_mass_ = fatJetA.mass();
		treeVar_fatJetB_mass_ = fatJetB.mass();

		treeVar_fatJetA_prunedMass_ = fatJetA.CHSpruned_mass();
		treeVar_fatJetB_prunedMass_ = fatJetB.CHSpruned_mass();

		treeVar_fatJetA_softDropMassCHS_ = fatJetA.CHSsoftdrop_mass();
		treeVar_fatJetB_softDropMassCHS_ = fatJetB.CHSsoftdrop_mass();

		treeVar_fatJetA_nSubjettinessTau1_ = fatJetA.NjettinessAK8_tau1();
		treeVar_fatJetA_nSubjettinessTau2_ = fatJetA.NjettinessAK8_tau2();
		treeVar_fatJetB_nSubjettinessTau1_ = fatJetB.NjettinessAK8_tau1();
		treeVar_fatJetB_nSubjettinessTau2_ = fatJetB.NjettinessAK8_tau2();

		treeVar_fatJetA_electronEnergyFraction_ = fatJetA.electronEnergyFraction();
		treeVar_fatJetB_electronEnergyFraction_ = fatJetB.electronEnergyFraction();

		treeVar_fatJetA_muonEnergyFraction_ = fatJetA.muonEnergyFraction();
		treeVar_fatJetB_muonEnergyFraction_ = fatJetB.muonEnergyFraction();

		treeVar_ht_ = ht;
		treeVar_nrSlimJets_ = slimJets.size();
		
		treeVar_muon_maxPt_ = muon_maxPt;
		treeVar_muon_sumPt_ = muon_sumPt;

		if (sampleType != "DATA"){
			treeVar_fatJetA_p4_jecAK8UncUp_.SetPtEtaPhiE(fatJetA.pt() * (1.0 + fatJetA.jecUncertainty()), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()) * (1.0 + fatJetA.jecUncertainty()) );
			treeVar_fatJetA_p4_jecAK8UncDown_.SetPtEtaPhiE(fatJetA.pt() * (1.0 - fatJetA.jecUncertainty()), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()) * (1.0 - fatJetA.jecUncertainty()) );
			treeVar_fatJetA_p4_jerAK8UncUp_.SetPtEtaPhiE(fatJetA.pt() * fatJetA.jerUncUp(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()) * fatJetA.jerUncUp() );
			treeVar_fatJetA_p4_jerAK8UncDown_.SetPtEtaPhiE(fatJetA.pt() * fatJetA.jerUncDown(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()) * fatJetA.jerUncDown() );

			treeVar_fatJetB_p4_jecAK8UncUp_.SetPtEtaPhiE(fatJetB.pt() * (1.0 + fatJetB.jecUncertainty()), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()) * (1.0 + fatJetB.jecUncertainty()) );
			treeVar_fatJetB_p4_jecAK8UncDown_.SetPtEtaPhiE(fatJetB.pt() * (1.0 - fatJetB.jecUncertainty()), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()) * (1.0 - fatJetB.jecUncertainty()) );
			treeVar_fatJetB_p4_jerAK8UncUp_.SetPtEtaPhiE(fatJetB.pt() * fatJetB.jerUncUp(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()) * fatJetB.jerUncUp() );
			treeVar_fatJetB_p4_jerAK8UncDown_.SetPtEtaPhiE(fatJetB.pt() * fatJetB.jerUncDown(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()) * fatJetB.jerUncDown() );

			treeVar_ht_jecAK4UncUp_ = ht_jecAK4UncUp;
			treeVar_ht_jecAK4UncDown_ = ht_jecAK4UncDown;
			treeVar_ht_jerAK4UncUp_ = ht_jerAK4UncUp;
			treeVar_ht_jerAK4UncDown_ = ht_jerAK4UncDown;

			if (slimJets.size() > 1){
				treeVar_jetA_p4_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecAK4UncUp_.SetPtEtaPhiE(slimJets.at(0).pt() * (1.0 + slimJets.at(0).jecUncertainty()), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * (1.0 + slimJets.at(0).jecUncertainty()) );
				treeVar_jetA_p4_jecAK4UncDown_.SetPtEtaPhiE(slimJets.at(0).pt() * (1.0 - slimJets.at(0).jecUncertainty()), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * (1.0 - slimJets.at(0).jecUncertainty()) );
				treeVar_jetA_p4_jerAK4UncUp_.SetPtEtaPhiE(slimJets.at(0).pt() * slimJets.at(0).jerUncUp(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * slimJets.at(0).jerUncUp() );
				treeVar_jetA_p4_jerAK4UncDown_.SetPtEtaPhiE(slimJets.at(0).pt() * slimJets.at(0).jerUncDown(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * slimJets.at(0).jerUncDown() );

				treeVar_jetB_p4_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
				treeVar_jetB_p4_jecAK4UncUp_.SetPtEtaPhiE(slimJets.at(1).pt() * (1.0 + slimJets.at(1).jecUncertainty()), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) * (1.0 + slimJets.at(1).jecUncertainty()) );
				treeVar_jetB_p4_jecAK4UncDown_.SetPtEtaPhiE(slimJets.at(1).pt() * (1.0 - slimJets.at(1).jecUncertainty()), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) * (1.0 - slimJets.at(1).jecUncertainty()) );
				treeVar_jetB_p4_jerAK4UncUp_.SetPtEtaPhiE(slimJets.at(1).pt() * slimJets.at(1).jerUncUp(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) * slimJets.at(1).jerUncUp() );
				treeVar_jetB_p4_jerAK4UncDown_.SetPtEtaPhiE(slimJets.at(1).pt() * slimJets.at(1).jerUncDown(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) * slimJets.at(1).jerUncDown() );
			}
			else if (slimJets.size() == 1){
				treeVar_jetA_p4_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecAK4UncUp_.SetPtEtaPhiE(slimJets.at(0).pt() * (1.0 + slimJets.at(0).jecUncertainty()), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * (1.0 + slimJets.at(0).jecUncertainty()) );
				treeVar_jetA_p4_jecAK4UncDown_.SetPtEtaPhiE(slimJets.at(0).pt() * (1.0 - slimJets.at(0).jecUncertainty()), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * (1.0 - slimJets.at(0).jecUncertainty()) );
				treeVar_jetA_p4_jerAK4UncUp_.SetPtEtaPhiE(slimJets.at(0).pt() * slimJets.at(0).jerUncUp(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * slimJets.at(0).jerUncUp() );
				treeVar_jetA_p4_jerAK4UncDown_.SetPtEtaPhiE(slimJets.at(0).pt() * slimJets.at(0).jerUncDown(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * slimJets.at(0).jerUncDown() );

				treeVar_jetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			}
			else {
				treeVar_jetA_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jecAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jecAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jerAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jerAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);

				treeVar_jetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			}

			// fatJetA SoftDropMass+PUPPI
			std::random_device rdA;
			std::mt19937 e2A(rdA());
			double mcMassResolutionToUseA = SoftDropPuppiSF::get_mcMassResolution(yearOfRun);
			if (mcMassResolutionToUseA > 0.333333 * fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun)) mcMassResolutionToUseA = 0.333333 * fabs(fatJetA.PUPPIsoftdrop_mass()) * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun);
			std::normal_distribution<> distA(0, mcMassResolutionToUseA);
			double gaussRandomA = distA(e2A);
			
			// nominal resolution and JMS
			if (SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) > 1.0){
				double smearingValue = gaussRandomA * sqrt(SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) - 1);
				treeVar_fatJetA_softDropMassPuppi_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
				treeVar_fatJetA_softDropMassPuppi_jmsUncUp_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) + smearingValue > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) + smearingValue : 0.0;
				treeVar_fatJetA_softDropMassPuppi_jmsUncDown_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) + smearingValue > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) + smearingValue : 0.0;
			}
			else {
				treeVar_fatJetA_softDropMassPuppi_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) : 0.0;
				treeVar_fatJetA_softDropMassPuppi_jmsUncUp_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) : 0.0;
				treeVar_fatJetA_softDropMassPuppi_jmsUncDown_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) : 0.0;
			}
			
			// JMR_UP
			if (SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) > 1.0){
				double smearingValue = gaussRandomA * sqrt(SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) - 1);
				treeVar_fatJetA_softDropMassPuppi_jmrUncUp_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
			}
			else {
				treeVar_fatJetA_softDropMassPuppi_jmrUncUp_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) : 0.0;
			}
			
			// JMR_DOWN
			if (SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) > 1.0){
				double smearingValue = gaussRandomA * sqrt(SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) - 1);
				treeVar_fatJetA_softDropMassPuppi_jmrUncDown_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
			}
			else {
				treeVar_fatJetA_softDropMassPuppi_jmrUncDown_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) : 0.0;
			}

			// fatJetB SoftDropMass+PUPPI
			std::random_device rdB;
			std::mt19937 e2B(rdB());
			double mcMassResolutionToUseB = SoftDropPuppiSF::get_mcMassResolution(yearOfRun);
			if (mcMassResolutionToUseB > 0.333333 * fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun)) mcMassResolutionToUseB = 0.333333 * fabs(fatJetB.PUPPIsoftdrop_mass()) * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun);
			std::normal_distribution<> distB(0, mcMassResolutionToUseB);
			double gaussRandomB = distB(e2B);
			
			// nominal resolution and JMS
			if (SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) > 1.0){
				double smearingValue = gaussRandomB * sqrt(SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) - 1);
				treeVar_fatJetB_softDropMassPuppi_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
				treeVar_fatJetB_softDropMassPuppi_jmsUncUp_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) + smearingValue > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) + smearingValue : 0.0;
				treeVar_fatJetB_softDropMassPuppi_jmsUncDown_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) + smearingValue > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) + smearingValue : 0.0;
			}
			else {
				treeVar_fatJetB_softDropMassPuppi_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) : 0.0;
				treeVar_fatJetB_softDropMassPuppi_jmsUncUp_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) : 0.0;
				treeVar_fatJetB_softDropMassPuppi_jmsUncDown_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) : 0.0;
			}
			
			// JMR_UP
			if (SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) > 1.0){
				double smearingValue = gaussRandomB * sqrt(SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) - 1);
				treeVar_fatJetB_softDropMassPuppi_jmrUncUp_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
			}
			else {
				treeVar_fatJetB_softDropMassPuppi_jmrUncUp_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) : 0.0;
			}
			
			// JMR_DOWN
			if (SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) > 1.0){
				double smearingValue = gaussRandomB * sqrt(SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) - 1);
				treeVar_fatJetB_softDropMassPuppi_jmrUncDown_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
			}
			else {
				treeVar_fatJetB_softDropMassPuppi_jmrUncDown_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) : 0.0;
			}


		} // closes 'if' sampleType does NOT equal DATA
		
		else {
			treeVar_fatJetA_p4_jecAK8UncUp_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));
			treeVar_fatJetA_p4_jecAK8UncDown_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));
			treeVar_fatJetA_p4_jerAK8UncUp_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));
			treeVar_fatJetA_p4_jerAK8UncDown_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));

			treeVar_fatJetB_p4_jecAK8UncUp_.SetPtEtaPhiE(fatJetB.pt(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()));
			treeVar_fatJetB_p4_jecAK8UncDown_.SetPtEtaPhiE(fatJetB.pt(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()));
			treeVar_fatJetB_p4_jerAK8UncUp_.SetPtEtaPhiE(fatJetB.pt(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()));
			treeVar_fatJetB_p4_jerAK8UncDown_.SetPtEtaPhiE(fatJetB.pt(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()));

			treeVar_ht_jecAK4UncUp_ = ht;
			treeVar_ht_jecAK4UncDown_ = ht;
			treeVar_ht_jerAK4UncUp_ = ht;
			treeVar_ht_jerAK4UncDown_ = ht;

			if (slimJets.size() > 1){
				treeVar_jetA_p4_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecAK4UncUp_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecAK4UncDown_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jerAK4UncUp_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jerAK4UncDown_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );

				treeVar_jetB_p4_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
				treeVar_jetB_p4_jecAK4UncUp_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
				treeVar_jetB_p4_jecAK4UncDown_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
				treeVar_jetB_p4_jerAK4UncUp_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
				treeVar_jetB_p4_jerAK4UncDown_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
			}
			else if (slimJets.size() == 1){
				treeVar_jetA_p4_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecAK4UncUp_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecAK4UncDown_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jerAK4UncUp_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jerAK4UncDown_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );

				treeVar_jetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			}
			else {
				treeVar_jetA_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jecAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jecAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jerAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jerAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				
				treeVar_jetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerAK4UncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerAK4UncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			}		

			const double fatJetA_softDropMassPuppi_ALL = fatJetA.PUPPIsoftdrop_mass() > 0.0 ? fatJetA.PUPPIsoftdrop_mass() : 0.0;
			treeVar_fatJetA_softDropMassPuppi_ = fatJetA_softDropMassPuppi_ALL;
			treeVar_fatJetA_softDropMassPuppi_jmsUncUp_ = fatJetA_softDropMassPuppi_ALL;
			treeVar_fatJetA_softDropMassPuppi_jmsUncDown_ = fatJetA_softDropMassPuppi_ALL;
			treeVar_fatJetA_softDropMassPuppi_jmrUncUp_ = fatJetA_softDropMassPuppi_ALL;
			treeVar_fatJetA_softDropMassPuppi_jmrUncDown_ = fatJetA_softDropMassPuppi_ALL;

			const double fatJetB_softDropMassPuppi_ALL = fatJetB.PUPPIsoftdrop_mass() > 0.0 ? fatJetB.PUPPIsoftdrop_mass() : 0.0;
			treeVar_fatJetB_softDropMassPuppi_ = fatJetB_softDropMassPuppi_ALL;
			treeVar_fatJetB_softDropMassPuppi_jmsUncUp_ = fatJetB_softDropMassPuppi_ALL;
			treeVar_fatJetB_softDropMassPuppi_jmsUncDown_ = fatJetB_softDropMassPuppi_ALL;
			treeVar_fatJetB_softDropMassPuppi_jmrUncUp_ = fatJetB_softDropMassPuppi_ALL;
			treeVar_fatJetB_softDropMassPuppi_jmrUncDown_ = fatJetB_softDropMassPuppi_ALL;

		} // closes 'else' sampleType is DATA

		// And finally fill the tree ...
		mainAnaTree_->Fill();
	}
};

}



//Functor for ordering leptons by pt
struct compareElectronPt{
	bool operator()(std::vector<ran::NtElectron>::const_iterator lhs, std::vector<ran::NtElectron>::const_iterator rhs){
		return (lhs->pt() > rhs->pt()); //Sort highest pt first
	}
};

struct compareMuonPt{
	bool operator()(const ran::NtMuon* lhs, const ran::NtMuon* rhs){
		return (lhs->pt() > rhs->pt()); //Sort highest pt first
	}
};

template <typename T>
struct compareParticlePt{
	bool operator()(const T* lhs, const T* rhs){
		return (lhs->pt() > rhs->pt()); //Sort highest pt first
	}
};

inline bool does_file_exist (const std::string& name) {
	struct stat buffer;   
	return (stat (name.c_str(), &buffer) == 0); 
}

std::string getOutputDirFromOutputFile(std::string outputFile)
{
    std::string outputDirectory = "DID_NOT_PROVIDE_OUTPUT_DIR";
    
    // strip the output directory from the outputfile path
    std::string forwardSlash = "/";
    for (size_t c = outputFile.size()-1; c > 0; --c){
        if (outputFile[c] == forwardSlash[0]){
            outputDirectory = outputFile.substr(0, c+1);
            break;
        }
    }
    return outputDirectory;
}



int main(int argc, char** argv){

	std::map<std::string,UInt_t> totNumEvtsAnalyzedMotherMass;

    ///////////////////////////////////////////////////
	// Set Run Parameters
    unsigned int outputEvery = 20000;
    ///////////////////////////////////////////////////

    // Initial setup from command line inputs /////////
    if (argc != 6){
        std::cout << "Provided wrong number of arguments, the format should be:" << std::endl;
        std::cout << argv[0] << " <outputRootFilename> <pathToListOfInputFiles> <sampleType> <yearOfRun> <runInstructionString>" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }

    char* outputRootFile = argv[1];

    // Determine whether we are running locally or on batch system
    bool batchMode;
    std::string runInstructions(argv[5]);
    if (runInstructions == "local") batchMode = false;
    else if (runInstructions == "batch") batchMode = true;
    else {
        std::cout << "You have not provided a correct input for the runInstructions" << std::endl;
        std::cout << "Your options are 'local' or 'batch'" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }

    // Check that the .list file exists
    std::string listFilename(argv[2]);
    if (!does_file_exist(listFilename)){
        std::cout << "File provided containing list of input files,  " + listFilename + "  does not exist" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }

    // Check you have provided a valid sample type - used for different scale factors
    std::string sampleType(argv[3]);
    if (sampleType != "DATA" && sampleType != "SIGNAL" && sampleType != "TTJETS" && sampleType != "OTHER_MC"){
        std::cout << "SampleType provided is not valid" << std::endl;
        std::cout << "Use either DATA, SIGNAL, TTJETS, OTHER_MC" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }

    // Check you have provided a valid yearOfRun - used for different scale factors
    unsigned int yearOfRun;
    std::string yearOfRunString(argv[4]);
    if (yearOfRunString == "2016") yearOfRun = 2016;
    else if (yearOfRunString == "2017") yearOfRun = 2017;
    else if (yearOfRunString == "2018") yearOfRun = 2018;
    else{
        std::cout << "yearOfRun provided is not valid" << std::endl;
        std::cout << "Use either 2016, 2017 or 2018" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }

    if (batchMode == false){
        // Check the directory of the outputRootFile does not yet exist
        // Then make this directory
        // Then copy the code used into the same directory (this could be out of sync if you edit after compilation)
        // Then copy the list of input files used into the same directory
        std::string outputDirectory = getOutputDirFromOutputFile(outputRootFile);
        if (outputDirectory == "DID_NOT_PROVIDE_OUTPUT_DIR"){
			std::cout << "Did not provide a new directory for the output file" << std::endl;
            std::cout << "Exiting..." << std::endl;
            return -1;        	
        }
        bool makeDir = !(std::system(Form("mkdir %s",outputDirectory.c_str())));
        if (makeDir == false){
            std::cout << "The chosen output directory cannot be created, it may already exist or not have a parent" << std::endl;
            std::cout << "Exiting..." << std::endl;
            return -1;
        }
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/main/mainNMSSM.cc %s",outputDirectory.c_str()));
        std::system(Form("cp %s %s",listFilename.c_str(),outputDirectory.c_str()));
    }



	  // ecalBadCalibFilter
	  std::vector<TString> ecalBadCalibList;
	  std::vector<long long int> ecalBadCalibNumbers;
	  Int_t necalBadCalib = 0;
	  Int_t badrun, badlumi;
	  long int badevent;
	  Int_t ncols, nlines;
	  nlines = 0;
	  FILE *fp = fopen("Run2018_JetHT.txt", "r");
	  while (1)
	  {
	    ncols = fscanf(fp, "%d%*c %d%*c %ld", &badrun, &badlumi, &badevent);
	    if (ncols < 0) break;

	    if(ncols==3) {

	    	TString eventID = "";
	    	eventID += (badrun);
	    	eventID += ":";
	    	eventID += (badlumi);
	    	eventID += ":";
	    	eventID += (badevent);

	    	long long int evtID = badevent + badlumi*10000000000 + (badrun-315256)*10000000000*10000;

	    	ecalBadCalibList.push_back(eventID);
	    	ecalBadCalibNumbers.push_back(evtID);

	      // //if (nlines < 1000000000) {
	      // if (badevent > 4000000000) {
	      // 	//printf(" ecalBadCalibFilter: %d:%d:%d \n", badrun, badlumi, badevent);
	      // 	std::cout<<eventID<<" "<<evtID<<std::endl;
	      // 	//std::cout<<eventID<<std::endl;
	      // }

	      nlines++;

	    }

	  }
	  fclose (fp);

	  std::cout<<"ecalBadCalibList size: "<<ecalBadCalibList.size()<<" "<<ecalBadCalibNumbers.size()<<std::endl;
	  std::cout<<"ecalBadCalibList first and last: "<<ecalBadCalibList.at(0)<<" "<<ecalBadCalibList.at(ecalBadCalibList.size()-1)<<std::endl;
	  std::cout<<"ecalBadCalibNumbers first and last: "<<ecalBadCalibNumbers.at(0)<<" "<<ecalBadCalibNumbers.at(ecalBadCalibNumbers.size()-1)<<std::endl;


    // find number of files that we are running over
	ifstream totalFileCount(argv[2]);
	unsigned int numberOfFiles = 0;
	std::string line;
	while (getline(totalFileCount, line)) numberOfFiles++;

	// Setup for main analysis - read list of input files 
	tsw::FatDoubleBJetPairTree doubleBFatJetPairTree("doubleBFatJetPairTree", outputRootFile);
	std::cout << "Reading file list: " << argv[2] << "\n";
	ifstream inputFiles(argv[2]);
	size_t evtIdx = 0;
	std::string inputFilePath;
	unsigned int fileCount = 0;
	while (getline(inputFiles, inputFilePath)){

		fileCount++;
		// Set up TTreeReader for this input file
		std::cout << std::endl << " *** NEW INPUT FILE: " << inputFilePath << std::endl;;
		TFile* inputFile = TFile::Open(inputFilePath.c_str());
		std::cout << ">>> [file has been loaded]" << std::endl;
		TTreeReader treeReader("demo/EventDataTree", inputFile);

		TTreeReaderValue<ran::EventInfo> evtInfo(treeReader, "evtInfo");
		// if (evtInfo.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for event info branch (status = " << evtInfo.GetSetupStatus() << ")" << std::endl;
			// return 1;
		// }
		// TTreeReaderValue<std::vector<ran::ElectronStruct>> eleBranchValue(treeReader, "electronCollection");
		// if (eleBranchValue.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for electronCollection branch (status = " << eleBranchValue.GetSetupStatus() << ")" << std::endl;
			// return 1;
		// }
		TTreeReaderValue<std::vector<ran::MuonStruct>> muonBranchValue(treeReader, "muonCollection");
		// if (muonBranchValue.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for muonCollection branch (status = " << muonBranchValue.GetSetupStatus() << ")" << std::endl; 
			// return 1;
		// }
		TTreeReaderValue<std::vector<ran::FatJetStruct>> fatJetBranchValue(treeReader, "fatjetCollection");
		// if (fatJetBranchValue.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for fat jet branch (status = " << fatJetBranchValue.GetSetupStatus() << ")" << std::endl; 
			// return 1;
		// }
		TTreeReaderValue<std::vector<ran::JetStruct>> jetBranchValue(treeReader, "jetCollection");
		// if (jetBranchValue.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for jet branch (status = " << jetBranchValue.GetSetupStatus() << ")" << std::endl;
			// return 1;
		// }

		TTreeReaderValue<std::vector<unsigned int>> recordedTriggerValue(treeReader, "recordedTriggers");

		TTreeReaderValue<int> nPU_tree(treeReader, "nPU");
		TTreeReaderValue<float> nTrueInt_tree(treeReader, "nTrueInt");
		TTreeReaderValue<int> nISR_tree(treeReader, "nISR");
		TTreeReaderValue<int> nGluino_tree(treeReader, "nGluino");

		TTreeReaderValue<int> nHiggs2bb_tree(treeReader, "nHiggs2bb");
		TTreeReaderValue<int> numHiggses_tree(treeReader, "numHiggses");
		TTreeReaderValue<float> motherMass_tree(treeReader, "motherMass");
		TTreeReaderValue<float> lspMass_tree(treeReader, "lspMass");

		TTreeReaderValue<float> prefire_tree(treeReader, "prefweight");
		TTreeReaderValue<float> prefireUp_tree(treeReader, "prefweightup");
		TTreeReaderValue<float> prefireDown_tree(treeReader, "prefweightdown");

		TTreeReaderValue<float> scaleUp_tree(treeReader, "scaleWeightUp");
		TTreeReaderValue<float> scaleDown_tree(treeReader, "scaleWeightDown");

		// Get the 'D' factor for ISR - NOTE THAT THIS IS PER INPUT FILE, NOT THE FULL SAMPLE !
		double D_factor = 0.0;
		if (sampleType == "SIGNAL"){
			TTree * T = (TTree*)inputFile->Get("demo/EventDataTree");
			TH1D * h_nISR = new TH1D("h_nISR", "", 6, 0, 6);
			T->Draw("nISR>>h_nISR");
			double D_numerator = h_nISR->GetEntries();
			double D_denominator = 0.0;
			for (unsigned int i = 0; i < 7; ++i){
				unsigned int iBin = i + 1;
				D_denominator += IsrWeights::GetWeight(i, yearOfRun) * h_nISR->GetBinContent(iBin);
			}
			D_factor = D_numerator / D_denominator;
			std::cout << "D_factor: " << D_factor << std::endl; 
		}
	

		// Loop over the events
		while (treeReader.Next()) {

			// const std::vector<ran::NtElectron> electronVec(eleBranchValue->begin(), eleBranchValue->end());
			const std::vector<ran::NtMuon> muonVec(muonBranchValue->begin(), muonBranchValue->end());
			const std::vector<ran::NtJet> jetVec(jetBranchValue->begin(), jetBranchValue->end());
			std::vector<ran::NtFatJet> fatJetVec(fatJetBranchValue->begin(), fatJetBranchValue->end());
			// std::sort(fatJetVec.begin(), fatJetVec.end(), [](const ran::NtFatJet& a, const ran::NtFatJet& b) {return b.pt() < a.pt();} );
			std::sort(fatJetVec.begin(), fatJetVec.end(), [](const ran::NtFatJet& a, const ran::NtFatJet& b) {return b.pfBoostedDoubleSecondaryVertexAK8BJetTags() < a.pfBoostedDoubleSecondaryVertexAK8BJetTags();} );

			const std::vector<unsigned int> recordedTriggerVec(recordedTriggerValue->begin(), recordedTriggerValue->end());
			bool doesEventPassTrigger = false;
			for (const unsigned int& recordedTrigger : recordedTriggerVec){
				if (recordedTrigger != 0){
					doesEventPassTrigger = true;
					break;
				}
			}

			if (sampleType == "DATA"){

				TString eventID = "";
				eventID += (evtInfo->runNum);
				eventID += ":";
				eventID += (evtInfo->lumiSec);
				eventID += ":";
				eventID += (evtInfo->evtNum);

				long long int evtID = evtInfo->evtNum + evtInfo->lumiSec*10000000000 + (evtInfo->runNum-315256)*10000000000*10000;

				bool failed_ecalBadCalib = false;

				for (unsigned int i_list = 0; i_list < ecalBadCalibNumbers.size(); ++i_list)
				{
					if (evtID == ecalBadCalibNumbers.at(i_list)) {

						failed_ecalBadCalib = true;
						++necalBadCalib;

					printf(" event failed ecalBadCalibFilter: %d:%d:%u  Total: %d\n", evtInfo->runNum, evtInfo->lumiSec, evtInfo->evtNum, necalBadCalib);
					if (eventID != ecalBadCalibList.at(i_list)) {
						std::cout<<"ERROR: "<<evtID<<" "<<ecalBadCalibNumbers.at(i_list)<<" "<<eventID<<" "<<ecalBadCalibList.at(i_list)<<std::endl;
					}

					continue;

					}
				}

				if(failed_ecalBadCalib) doesEventPassTrigger = false;
			}


			const int nPU = *nPU_tree;
			const float nTrueInt = *nTrueInt_tree;
			const int nISR = *nISR_tree;
			const int nGluino = *nGluino_tree;

			const int nHiggs2bb = *nHiggs2bb_tree;
			const int numHiggses = *numHiggses_tree;
			const float motherMass = *motherMass_tree;
			const float lspMass = *lspMass_tree;

			// std::cout<<motherMass<<std::endl;

			const float corr_prefire = *prefire_tree;
			const float corr_prefireUp = *prefireUp_tree;
			const float corr_prefireDown = *prefireDown_tree;
		
			const float corr_scaleUp = *scaleUp_tree;
			const float corr_scaleDown = *scaleDown_tree;

			if (sampleType != "DATA" && yearOfRun >= 2017 && nPU < 1) continue; // to veto the zeroPU events in 94X simulation
			// if (nPU < 28) continue; // HACK: if you only want to use a sample of particular PU
			// if (nGluino != 0) continue; // HACK: if you only want to select squark-squark samples

			// Muon Information
			double muon_maxPt = 0.0;
			double muon_sumPt = 0.0;
			for (const ran::NtMuon& muon : muonVec) {
				muon_sumPt += muon.pt();
				if (muon.pt() > muon_maxPt) muon_maxPt = muon.pt();
			}

			// HT calculation: Only consider jets with |eta| < 2.4, pt > 40.0
			double ht = 0.0;
			double ht_jecAK4UncUp = 0.0;
			double ht_jecAK4UncDown = 0.0;
			double ht_jerAK4UncUp = 0.0;
			double ht_jerAK4UncDown = 0.0;
			unsigned int nPrefireCandidates = 0;
			for (const ran::NtJet& jet : jetVec) {

				if (isJetLooseID(jet)==false)
					continue;

				if ( jet.pt()*(jet.chargedEmEnergyFraction()+jet.neutralEmEnergyFraction()) > 30.0 && fabs(jet.eta()) > 2.25 && fabs(jet.eta()) < 2.40)
					nPrefireCandidates++;

				if ( fabs(jet.eta()) <= 2.4 ){

					if ( jet.pt() >= 40.0 ) ht += jet.pt();
					if ( jet.pt() * ( 1.0 + jet.jecUncertainty() ) >= 40.0 ) ht_jecAK4UncUp += jet.pt() * ( 1.0 + jet.jecUncertainty() );
					if ( jet.pt() * ( 1.0 - jet.jecUncertainty() ) >= 40.0 ) ht_jecAK4UncDown += jet.pt() * ( 1.0 - jet.jecUncertainty() );
					if ( jet.pt() * jet.jerUncUp() >= 40.0 ) ht_jerAK4UncUp += jet.pt() * jet.jerUncUp();
					if ( jet.pt() * jet.jerUncDown() >= 40.0 ) ht_jerAK4UncDown += jet.pt() * jet.jerUncDown();
				}
			}

			std::vector<ran::NtFatJet> centralFatJetVec; // get the *central* fatJets
			for (const ran::NtFatJet& fatJet : fatJetVec) {
				if (fabs(fatJet.eta()) < 2.4 && fatJet.PUPPIsoftdrop_mass() > -1.0) centralFatJetVec.push_back(fatJet);
			}			

			if (centralFatJetVec.size() >= 2) {
			// if (centralFatJetVec.size() >= 2 && ht > 1200.0) { // HACK: INCLUDE HT CUT TO KEEP SOME DATASETS TRIM
			// if (centralFatJetVec.size() >= 2 && ht > 1499.0) { // HACK: INCLUDE HT CUT TO KEEP SOME DATASETS TRIM
				const ran::NtFatJet& fatJetA = centralFatJetVec.at(0);
				const ran::NtFatJet& fatJetB = centralFatJetVec.at(1);

				std::vector<ran::NtJet> slimJets;
				for (const ran::NtJet& jet : jetVec) {
					if (fabs(jet.eta())>2.4 || isJetLooseID(jet)==false)
						continue;
					if (deltaR2(jet.eta(), jet.phi(), fatJetA.eta(), fatJetA.phi()) < (1.4 * 1.4))
						continue;
					else if (deltaR2(jet.eta(), jet.phi(), fatJetB.eta(), fatJetB.phi()) < (1.4 * 1.4))
						continue;
					slimJets.push_back(jet);
				}
				std::sort(slimJets.begin(), slimJets.end(), [](const ran::NtJet& a, const ran::NtJet& b) {return b.pt() < a.pt();} );

				// Fat Jets ordered such that 1/2 events have fatJetA with highest DBT discriminator score, the other half have fatJetB with the highest DBT score
				if (evtIdx % 2 == 0) doubleBFatJetPairTree.fillTree(sampleType, *evtInfo, fatJetA, fatJetB, ht, ht_jecAK4UncUp, ht_jecAK4UncDown, ht_jerAK4UncUp, ht_jerAK4UncDown, slimJets, doesEventPassTrigger, nPU, nTrueInt, nISR, nGluino,
nHiggs2bb,
numHiggses,
motherMass,
lspMass,				
				 D_factor, yearOfRun, muon_maxPt, muon_sumPt, corr_prefire, corr_prefireUp, corr_prefireDown, nPrefireCandidates, corr_scaleUp, corr_scaleDown);
				else doubleBFatJetPairTree.fillTree(sampleType, *evtInfo, fatJetB, fatJetA, ht, ht_jecAK4UncUp, ht_jecAK4UncDown, ht_jerAK4UncUp, ht_jerAK4UncDown, slimJets, doesEventPassTrigger, nPU, nTrueInt, nISR, nGluino,
nHiggs2bb,
numHiggses,
motherMass,
lspMass,				
				 D_factor, yearOfRun, muon_maxPt, muon_sumPt, corr_prefire, corr_prefireUp, corr_prefireDown, nPrefireCandidates, corr_scaleUp, corr_scaleDown);

				// Fat Jets ordered by DBT discriminator score
				// doubleBFatJetPairTree.fillTree(sampleType, *evtInfo, fatJetA, fatJetB, ht, ht_jecAK4UncUp, ht_jecAK4UncDown, ht_jerAK4UncUp, ht_jerAK4UncDown, slimJets, doesEventPassTrigger, nPU, nTrueInt, nISR, nGluino, D_factor, yearOfRun, muon_maxPt, muon_sumPt, corr_prefire, corr_prefireUp, corr_prefireDown, nPrefireCandidates, corr_scaleUp, corr_scaleDown);
			}
			// event counter
            if (outputEvery!=0 ? (evtIdx % outputEvery == 0) : false){
				std::cout << "  File " << fileCount << " of " << numberOfFiles;
				std::cout << ": processing event: " << evtIdx << std::endl;
			}
			evtIdx++;
			totNumEvtsAnalyzedMotherMass[std::to_string(int(motherMass))]++;
		}

		std::cout << "Out of the event loop for file '" << inputFilePath << "'" << std::endl;
		delete inputFile;
	}

	std::cout << "Finished reading all input files now; processed " << evtIdx << " events in total" << std::endl;

	doubleBFatJetPairTree.setEventCounter(evtIdx, totNumEvtsAnalyzedMotherMass);
	doubleBFatJetPairTree.saveToFile();

	return 0;
}
