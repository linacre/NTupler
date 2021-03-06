//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <map>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>

//ROOT HEADERS
#include <TH1D.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TFile.h>

//RAL PARTICLE HEADERS
#include "PlotEntry2D.hh"
#include "Plotter.hh"
#include "MassRegionCuts.hh"
#include "CutVariable.hh"
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"
#include "MacrosOnCondor.h"


//  CREATE THE HISTOGRAMS THAT WE USE TO DO ANALYSIS IN 1D


void CreateHistograms(std::map<std::string,TH1D*>& h_, const std::vector<std::vector<std::string>>& cut2_ak8Dbt, const std::vector<std::vector<int>>& cut4_ht, const unsigned int& numberOfSegments, const std::vector<std::string>& systematicNameVec)
{
    unsigned int numberOfBins = cut4_ht.size() * numberOfSegments;

    for (auto systematicName : systematicNameVec){

        for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
            
            std::string histogramName = "";
            if (cut2_ak8Dbt[iCut2].size() == 4) histogramName += "dbt" + cut2_ak8Dbt[iCut2][0] + cut2_ak8Dbt[iCut2][1] + "And" + cut2_ak8Dbt[iCut2][2] + cut2_ak8Dbt[iCut2][3];
            if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP") histogramName += "dbtDiagUp" + cut2_ak8Dbt[iCut2][1];
            if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_DOWN") histogramName += "dbtDiagDown" + cut2_ak8Dbt[iCut2][1];
            if (systematicName.substr(0,3) == "SF_") histogramName += "_" + systematicName.substr(3);
            else histogramName += "_" + systematicName;

            h_[Form("S_%s", histogramName.c_str())] = new TH1D(Form("S_%s", histogramName.c_str()), ";Search Region Bin Number;Events", numberOfBins, 0, numberOfBins);
            h_[Form("U_%s", histogramName.c_str())] = new TH1D(Form("U_%s", histogramName.c_str()), ";Search Region Bin Number;Events", numberOfBins, 0, numberOfBins);
            h_[Form("D_%s", histogramName.c_str())] = new TH1D(Form("D_%s", histogramName.c_str()), ";Search Region Bin Number;Events", numberOfBins, 0, numberOfBins);
            
        } // closes loop through DBT configurations
    } // closes loop through systematics
}


void WriteHistograms(const std::map<std::string,TH1D*> & h_, const std::string& outputFile)
{
   TFile * outFile = new TFile(outputFile.c_str(),"RECREATE");
   for ( auto & h : h_ ){
        h.second->Write();
    }
   outFile -> Close();
   delete outFile;
}


int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    TString sampleString(argv[2]?argv[2]:"QCD");

    // TWO: do we want to blind the S mass region? (true for data, false for everything else)
    const bool blind_S_region = false;
    const bool is2018squark = false; // enable hack to select nGluino=0 component for 2018 'squark' samples
    const bool disableSys = true;

    // ONE: save info
    std::string outputDir;
    if (sampleString == "signal") outputDir = "/opt/ppd/scratch-2021/xxt18833/Analysis_boostedNmssmHiggs/histos_2020_12_10/run2018/SLIMXYZ/with_Sys/"; // where we are going to save the output plots (should include the samples name + binning maybe)
    else outputDir = "histos_" + std::string(disableSys ? "noSys_" : "") + std::string(sampleString); // where we are going to save the output plots (should include the samples name + binning maybe)
    std::cout<<"Using outputDir "<<outputDir<<std::endl;

    // THREE: set the cut params.
    std::vector<std::vector<std::string>> cut2_ak8Dbt;
    cut2_ak8Dbt.push_back({"DIAG_UP", "Loose"}); // TAG: Top Diagnol Corner Crossing Axis at...--> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    cut2_ak8Dbt.push_back({"DIAG_DOWN", "Loose"}); // New validation region defined from the bottom half (cut diagonally) of the 'tag' square.
    // cut2_ak8Dbt.push_back({"Loose", "Max", "Loose", "Max"}); // tagPRIME for TTJets special
    cut2_ak8Dbt.push_back({"Off","Loose","Off","Loose"}); // ANTI: 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    cut2_ak8Dbt.push_back({"Loose","Med2","Off","IDBTCv23"}); // CONTROL_A: 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    cut2_ak8Dbt.push_back({"Off","IDBTCv23","Loose","Med2"}); // CONTROL_B: 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    cut2_ak8Dbt.push_back({"Loose","Max","IDBTCv23","Loose"}); // CONTROL_C: 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    cut2_ak8Dbt.push_back({"IDBTCv23","Loose","Loose","Max"}); // CONTROL_D: 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"

    const int cut3_ak8Pt = 300;
    const std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {2500,3500}, {3500,99999} }; // HT BIN
    // const std::vector<std::vector<int>> cut4_ht = { {1500,99999} }; // HT BIN
    const std::vector<int> cut5_ak4Pt = {300,-1}; // 1st element for leading pt, 2nd element for secondary pt
    // const std::vector<int> cut5_ak4Pt = {-1,-1}; // 1st element for leading pt, 2nd element for secondary pt


    // FOUR: set the mass regions (use a KEYNAME and keep track of what means what!!!!)
    double S1_Node1 = 40.0;
    double S1_Node2 = 17.6;
    double SMAX_Node1 = 170.7;
    double SMAX_Node2 = 113.7;
    std::vector<double> SN_Nodes = {51.9, 65.1, 78.3, 91.5, 104.7, 117.9, 131.1, 144.3, 157.5};
    MassRegionCuts MassCutsObject = MassRegionCuts("MassCutsV09", S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2, SN_Nodes);
    const double sideBandScaleFactor = 0.5;
    const std::string massCutObjectName = "MassCutsV09";

    // const double S1_Node1 = 195.0;
    // const double S1_Node2 = 100.0;
    // const double SMAX_Node1 = 201.0;
    // const double SMAX_Node2 = 170.0;
    // const std::vector<double> SN_Nodes = {197.0, 199.0};
    // const double sideBandScaleFactor = 1.0;
    // const std::string massCutObjectName = "MassCutsSpecialV06";


    // FIVE: set up the luminosity
    // const double luminosity = 35.922; // 2016 DATASET
    // const double luminosity = 41.529; // 2017 DATASET
    const double luminosity = 59.740565202; // 2018 DATASET


    // SIX: systematics to run over. only need to use them for TAG dbt. start with 'SF_' for scale factors.
    const std::vector<std::string> systematicNameVec_nosys = {"NOSYS"}; // >> data & no systematic
    const std::vector<std::string> systematicNameVec_vjets = {"NOSYS", "jecAKXUncUp", "jecAKXUncDown", "jerAKXUncUp", "jerAKXUncDown", "jmsUncUp", "jmsUncDown", "jmrUncUp", "jmrUncDown", "SF_isrUp", "SF_isrDown", "SF_prefireUp", "SF_prefireDown", "SF_scaleUp", "SF_scaleDown"}; // >> WJets and ZJets
    const std::vector<std::string> systematicNameVec_ttjets = {"NOSYS", "jecAKXUncUp", "jecAKXUncDown", "jerAKXUncUp", "jerAKXUncDown", "jmsUncUp", "jmsUncDown", "jmrUncUp", "jmrUncDown", "SF_dbtTagUp", "SF_dbtTagDown", "SF_isrUp", "SF_isrDown", "SF_prefireUp", "SF_prefireDown", "SF_scaleUp", "SF_scaleDown"}; // >> TTJets
    const std::vector<std::string> systematicNameVec_signal = {"NOSYS", "jecAKXUncUp", "jecAKXUncDown", "jerAKXUncUp", "jerAKXUncDown", "jmsUncUp", "jmsUncDown", "jmrUncUp", "jmrUncDown", "SF_dbtTagUp", "SF_dbtTagDown", "SF_isrUp", "SF_isrDown", "SF_prefireUp", "SF_prefireDown"}; // >> SIGNAL
    const std::vector<std::string> systematicNameVec_centralsignal = {"NOSYS", "jecAKXUncUp", "jecAKXUncDown", "jerAKXUncUp", "jerAKXUncDown", "jmsUncUp", "jmsUncDown", "jmrUncUp", "jmrUncDown", "SF_dbtTagUp", "SF_dbtTagDown", "SF_isrUp", "SF_isrDown", "SF_prefireUp", "SF_prefireDown", "SF_scaleUp", "SF_scaleDown"}; // >> CENTRAL SIGNAL

    std::vector<std::string> systematicNameVec_temp;
    if(disableSys) systematicNameVec_temp = systematicNameVec_nosys;
    else if(sampleString=="data" || sampleString=="QCD") systematicNameVec_temp = systematicNameVec_nosys;
    else if(sampleString=="signal") systematicNameVec_temp = systematicNameVec_centralsignal;
    else if(sampleString.BeginsWith("TTJets")) systematicNameVec_temp = systematicNameVec_ttjets;
    else if(sampleString.EndsWith("Jets")) systematicNameVec_temp = systematicNameVec_vjets;
    else if(sampleString.Contains("mH")) systematicNameVec_temp = systematicNameVec_centralsignal;
    else if(sampleString.Contains("T5qqqqZH")) systematicNameVec_temp = systematicNameVec_centralsignal;
    else systematicNameVec_temp = systematicNameVec_nosys;

    const std::vector<std::string> systematicNameVec = systematicNameVec_temp;


    // SEVEN: variables to use AND the systematics that affect their distributions
    CutVariable fatJetA_pt_CV = CutVariable("fatJetA_p4.Pt()", {"jecAKXUncUp", "jecAKXUncDown", "jerAKXUncUp", "jerAKXUncDown"});
    CutVariable fatJetB_pt_CV = CutVariable("fatJetB_p4.Pt()", {"jecAKXUncUp", "jecAKXUncDown", "jerAKXUncUp", "jerAKXUncDown"});
    CutVariable fatJetA_mass_CV = CutVariable("fatJetA_softDropMassPuppi", {"jmsUncUp", "jmsUncDown", "jmrUncUp", "jmrUncDown"});
    CutVariable fatJetB_mass_CV = CutVariable("fatJetB_softDropMassPuppi", {"jmsUncUp", "jmsUncDown", "jmrUncUp", "jmrUncDown"});
    CutVariable fatJetA_dbt_CV = CutVariable("fatJetA_doubleBtagDiscrim", {});
    CutVariable fatJetB_dbt_CV = CutVariable("fatJetB_doubleBtagDiscrim", {});
    CutVariable slimJetA_pt_CV = CutVariable("slimJetA_p4.Pt()", {"jecAKXUncUp", "jecAKXUncDown", "jerAKXUncUp", "jerAKXUncDown"});
    CutVariable slimJetB_pt_CV = CutVariable("slimJetB_p4.Pt()", {"jecAKXUncUp", "jecAKXUncDown", "jerAKXUncUp", "jerAKXUncDown"});
    CutVariable ht_CV = CutVariable("ht", {"jecAKXUncUp", "jecAKXUncDown", "jerAKXUncUp", "jerAKXUncDown"});


    // EIGHT: >>>BELOW<<<


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    if (argc < 2){
        std::cout << "Haven't provided valid argument, the format should be:" << std::endl;
        std::cout << argv[0] << " <runInstructionString>" << std::endl;
        std::cout << "runInstructionString = 'local' or 'batch'" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }
    
    std::string runInstructionString(argv[1]);
    
    if (runInstructionString == "local"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_create.cc %s/%s__histos_create.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_create.cc %s/%s__histos_create.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "histos_create", outputDir.c_str());
        return 0;
    }
    else if (runInstructionString == "batchRUN"){
        outputDir = ".";
    }
    else{
        std::cout << "Haven't provided valid argument, the format should be:" << std::endl;
        std::cout << argv[0] << " <runInstructionString>" << std::endl;
        std::cout << "runInstructionString = 'local' or 'batch'" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    
    const unsigned int numberOfSegments = SN_Nodes.size() + 1;
    std::map<std::string, TH1D*> h_;
    CreateHistograms(h_, cut2_ak8Dbt, cut4_ht, numberOfSegments, systematicNameVec);

    for (size_t iSys = 0; iSys < systematicNameVec.size(); ++iSys){
        const std::string systematicName = systematicNameVec[iSys];

        std::string fatJetA_pt_name = fatJetA_pt_CV.GetCutVariableName(systematicName.c_str());
        std::string fatJetB_pt_name = fatJetB_pt_CV.GetCutVariableName(systematicName.c_str());
        std::string fatJetA_mass_name = fatJetA_mass_CV.GetCutVariableName(systematicName.c_str());
        std::string fatJetB_mass_name = fatJetB_mass_CV.GetCutVariableName(systematicName.c_str());
        std::string fatJetA_dbt_name = fatJetA_dbt_CV.GetCutVariableName(systematicName.c_str());
        std::string fatJetB_dbt_name = fatJetB_dbt_CV.GetCutVariableName(systematicName.c_str());
        std::string slimJetA_pt_name = slimJetA_pt_CV.GetCutVariableName(systematicName.c_str());
        std::string slimJetB_pt_name = slimJetB_pt_CV.GetCutVariableName(systematicName.c_str());
        std::string ht_name = ht_CV.GetCutVariableName(systematicName.c_str());

        // HACK to correlate AK4<->AK8 JECs and JERs
        std::replace(fatJetA_pt_name.begin(), fatJetA_pt_name.end(), 'X', '8');
        std::replace(fatJetB_pt_name.begin(), fatJetB_pt_name.end(), 'X', '8');
        std::replace(slimJetA_pt_name.begin(), slimJetA_pt_name.end(), 'X', '4');
        std::replace(slimJetB_pt_name.begin(), slimJetB_pt_name.end(), 'X', '4');
        std::replace(ht_name.begin(), ht_name.end(), 'X', '4');
        //std::cout<<fatJetA_pt_name<<std::endl;
        //std::cout<<fatJetB_pt_name<<std::endl;
        //std::cout<<slimJetA_pt_name<<std::endl;
        //std::cout<<slimJetB_pt_name<<std::endl;
        //std::cout<<ht_name<<std::endl;

        MassRegionCuts MassCutsObject = MassRegionCuts(massCutObjectName.c_str(), S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2, SN_Nodes, fatJetA_mass_name, fatJetB_mass_name, sideBandScaleFactor);

        std::string SF_weight = "DUMMY"; // only use this on samples with non-trivial scale factors
        if (systematicName.substr(0,3) != "SF_") SF_weight = "weight_combined";
        else if (systematicName.substr(0,8) == "SF_scale"){
            if (systematicName.substr(8) == "Up") SF_weight = "weight_combined * weight_scaleUp";
            else SF_weight = "weight_combined * weight_scaleDown";
        }
        else{
            std::string SF_name = systematicName.substr(3);
            if ( SF_name.substr(SF_name.size()-2) == "Up") SF_weight = Form("weight_combined * (weight_%s / weight_%s)", SF_name.c_str(), SF_name.substr(0, SF_name.size()-2).c_str());
            else if ( SF_name.substr(SF_name.size()-4) == "Down") SF_weight = Form("weight_combined * (weight_%s / weight_%s)", SF_name.c_str(), SF_name.substr(0, SF_name.size()-4).c_str());
            else {
                std::cout << "ERROR: scale factor systematic " << systematicName << " is not Up or Down... NOT DOIN IT..." << std::endl;
                continue;
            }
        }

        const unsigned int numberOfCutsForCodeTodo = (MassCutsObject.Get_SN_Nodes().size() + 1) * cut4_ht.size() * cut2_ak8Dbt.size() * 3;
        unsigned int counter = 1;
        for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
            
            size_t iMassRegionStart = 0;
            if (blind_S_region == true) iMassRegionStart = numberOfSegments;
            for (size_t iMassRegion = iMassRegionStart; iMassRegion!=3*numberOfSegments; ++iMassRegion){

                std::string histogramName = "";
                if (cut2_ak8Dbt[iCut2].size() == 4) histogramName += "dbt" + cut2_ak8Dbt[iCut2][0] + cut2_ak8Dbt[iCut2][1] + "And" + cut2_ak8Dbt[iCut2][2] + cut2_ak8Dbt[iCut2][3];
                if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP") histogramName += "dbtDiagUp" + cut2_ak8Dbt[iCut2][1];
                if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_DOWN") histogramName += "dbtDiagDown" + cut2_ak8Dbt[iCut2][1];

                if (iMassRegion < numberOfSegments) histogramName = "S_" + histogramName;
                else if (iMassRegion < 2*numberOfSegments) histogramName = "U_" + histogramName;
                else histogramName = "D_" + histogramName;
                if (systematicName.substr(0,3) == "SF_") histogramName += "_" + systematicName.substr(3);
                else histogramName += "_" + systematicName;

                for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){

                    std::string dbtCut = "";
                    if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP")
                        dbtCut = Form("%s >= (-1.0 * %s + 1.0 + %f) ", fatJetA_dbt_name.c_str(), fatJetB_dbt_name.c_str(), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]) );

                    if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_DOWN")
                        dbtCut = Form("%s < (-1.0 * %s + 1.0 + %f) && %s>=%f && %s>=%f", fatJetA_dbt_name.c_str(), fatJetB_dbt_name.c_str(), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), fatJetA_dbt_name.c_str(), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), fatJetB_dbt_name.c_str(), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]) );

                    if (cut2_ak8Dbt[iCut2].size() == 4)
                        dbtCut = Form("%s>=%f && %s<%f && %s>=%f && %s<%f ", fatJetA_dbt_name.c_str(), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), fatJetA_dbt_name.c_str(), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), fatJetB_dbt_name.c_str(), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), fatJetB_dbt_name.c_str(), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]) );
                    std::string cutToApply = Form("%s && %s>%d && %s>%d && %s>=%d && %s<%d && %s>%d && %s>%d", dbtCut.c_str(), fatJetA_pt_name.c_str(), cut3_ak8Pt, fatJetB_pt_name.c_str(), cut3_ak8Pt, ht_name.c_str(), cut4_ht[iCut4][0], ht_name.c_str(), cut4_ht[iCut4][1], slimJetA_pt_name.c_str(), cut5_ak4Pt[0], slimJetB_pt_name.c_str(), cut5_ak4Pt[1]);

                    if(sampleString=="data") cutToApply += " && trgDecision==1";

                    if(sampleString.Contains("T5qqqqZH")) cutToApply += " && motherMass==1500";  // TODO: also try with no nHiggs cut

                    if(is2018squark && sampleString=="signal") cutToApply += " && nGluino==0"; // hack to select nGluino=0 component for 2018 'squark' samples

                    cutToApply += " && " + MassCutsObject.GetAllCuts()[iMassRegion];
                    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA_MassType (GeV);fatJetB_MassType (GeV)", 600, 0, 300, 600, 0, 300);
                    TH2D hTemplate = TH2D("hTemplate", ";fatJetA_MassType (GeV);fatJetB_MassType (GeV)", 100, 0, 200, 100, 0, 200);
                    std::string varToPlot = fatJetB_mass_name + ":" + fatJetA_mass_name;
                    PlotEntry2D plotEntry = PlotEntry2D("plotEntry", hTemplate, varToPlot.c_str(), luminosity);

                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    // EIGHT: sample info - SOME SAMPLES NEED SYSTEMATIC WEIGHTS (Signal, TTJets, ...)
                    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. 
                    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.                    
                    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
                    // 2016 (80X)

                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2016/QCD_HT1000to1500_ht1499plus/flatTree.root", cutToApply.c_str(), 1206, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2016/QCD_HT1500to2000_ht1499plus/flatTree.root", cutToApply.c_str(), 120.4, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2016/QCD_HT2000toInf_ht1499plus/flatTree.root", cutToApply.c_str(), 25.25, SF_weight.c_str());

                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2016/TTJets_ht1200plus/flatTree.root", cutToApply.c_str(), 831.76, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2016/ZJets_ht1200plus/flatTree.root", cutToApply.c_str(), 52.79, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2016/WJets_ht1200plus/flatTree.root", cutToApply.c_str(), 95.14, SF_weight.c_str());

                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2016/CPS_h70s1200/flatTree.root", cutToApply.c_str(), 0.4951000*0.840*0.840, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2016/CPS_h70s2000/flatTree.root", cutToApply.c_str(), 0.0091050*0.840*0.840, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2016/CPS_h70s2600/flatTree.root", cutToApply.c_str(), 0.0006167*0.840*0.840, SF_weight.c_str());

                    // SIGNAL 2016
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2016/NAMEXYZ/flatTree.root", cutToApply.c_str(), 987654321.0, SF_weight.c_str());

                    // 2016 GOLDEN JSON DATASET
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2016/JetHT_Run2016TOTAL/flatTree.root", cutToApply.c_str());

                    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
                    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
                    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. 
                    // 2017 (94X)

                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/QCD_HT1000to1500_ht1499plus/flatTree.root", cutToApply.c_str(), 1005, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/QCD_HT1500to2000_ht1499plus/flatTree.root", cutToApply.c_str(), 101.8, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/QCD_HT2000toInf_ht1499plus/flatTree.root", cutToApply.c_str(), 20.54, SF_weight.c_str());

                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/TTToHadronic_ht1200plus/flatTree.root", cutToApply.c_str(), 377.96, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/TTToSemiLeptonic_ht1200plus/flatTree.root", cutToApply.c_str(), 365.34, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/TTTo2L2Nu_ht1200plus/flatTree.root", cutToApply.c_str(), 88.29, SF_weight.c_str());

                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/ZJets_ht1200plus/flatTree.root", cutToApply.c_str(), 18.69, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/WJets_ht1200plus/flatTree.root", cutToApply.c_str(), 34.00, SF_weight.c_str());

                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/CPS_h70s1200/flatTree.root", cutToApply.c_str(), 0.4951000*0.840*0.840, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/CPS_h70s2000/flatTree.root", cutToApply.c_str(), 0.0091050*0.840*0.840, SF_weight.c_str());
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/CPS_h70s2600/flatTree.root", cutToApply.c_str(), 0.0006167*0.840*0.840, SF_weight.c_str());

                    // SIGNAL 2017
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/NAMEXYZ/flatTree.root", cutToApply.c_str(), 987654321.0, SF_weight.c_str());

                    // 2017 GOLDEN JSON DATASET
                    // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/JetHT_Run2017TOTAL/flatTree.root", cutToApply.c_str());

                    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
                    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
                    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.




                    // 2018 (10_2X)

                    std::string treepath = "/mercury/data2/linacre/NMSSM/CMSSW_10_2_12/src/NTupler/PATNTupler/main/slimjet24ht/";

                    if(sampleString=="QCD") {
                        plotEntry.AddInput((treepath+"QCD_HT1000to1500_TuneCP5_13TeV-madgraphMLM-pythia8/flatTree.root").c_str(), cutToApply.c_str(), 1005, SF_weight.c_str());
                        plotEntry.AddInput((treepath+"QCD_HT1500to2000_TuneCP5_13TeV-madgraphMLM-pythia8/flatTree.root").c_str(), cutToApply.c_str(), 101.8, SF_weight.c_str());
                        plotEntry.AddInput((treepath+"QCD_HT2000toInf_TuneCP5_13TeV-madgraphMLM-pythia8/flatTree.root").c_str(), cutToApply.c_str(), 20.54, SF_weight.c_str());
                    }

                    else if(sampleString.BeginsWith("TTJets")) {
                        double PDGhadBF = 67.41/100.;
                        if(sampleString=="TTJets0L" || sampleString=="TTJetsALL") plotEntry.AddInput((treepath+"TTToHadronic_TuneCP5_13TeV-powheg-pythia8/flatTree.root").c_str(), cutToApply.c_str(), 831.76*PDGhadBF*PDGhadBF, SF_weight.c_str());
                        if(sampleString=="TTJets1L" || sampleString=="TTJetsALL") plotEntry.AddInput((treepath+"TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/flatTree.root").c_str(), cutToApply.c_str(), 831.76*PDGhadBF*(1.-PDGhadBF)*2., SF_weight.c_str());
                        if(sampleString=="TTJets2L" || sampleString=="TTJetsALL") plotEntry.AddInput((treepath+"TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/flatTree.root").c_str(), cutToApply.c_str(), 831.76*(1.-PDGhadBF)*(1.-PDGhadBF), SF_weight.c_str());
                    }

                    else if(sampleString=="WJets") {
                        plotEntry.AddInput((treepath+"WJetsToQQ_HT-800toInf_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8/flatTree.root").c_str(), cutToApply.c_str(), 34.00, SF_weight.c_str());
                    }

                    else if(sampleString=="ZJets") {
                        plotEntry.AddInput((treepath+"ZJetsToQQ_HT-800toInf_qc19_4j_TuneCP5_13TeV-madgraphMLM-pythia8/flatTree.root").c_str(), cutToApply.c_str(), 18.69, SF_weight.c_str());
                    }

                    // SUSY scan https://github.com/TreeMaker/TreeMaker/blob/c95b539b050f05789e54d6ec5ec8a61132d75077/Production/test/data/dict_xsec_T1_NNLO.txt
                    // 1000	0.385E+00
                    // 1500	0.157E-01
                    // 1600	0.887E-02
                    // 2200	0.356E-03
                    else if(sampleString.Contains("T5qqqqZH")) {
                        plotEntry.AddInputFromGrid("/mercury/data2/linacre/NMSSM/CMSSW_10_2_12/src/NTupler/PATNTupler/main/T5qqqqZH/T5qqqqZH_2018.root", cutToApply.c_str(), 0.157E-01, SF_weight.c_str());
                    }

                    else if(sampleString.BeginsWith("WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8")) {
                        plotEntry.AddInput("/mercury/data2/linacre/NMSSM/CMSSW_10_2_12/src/NTupler/PATNTupler/main/slimjet24ht_new/WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8/flatTree.root", cutToApply.c_str(), 51.723, SF_weight.c_str());
                    }
                    else if(sampleString.BeginsWith("WW_TuneCP5_PSweights_13TeV-pythia8")) {
                        plotEntry.AddInput("/mercury/data2/linacre/NMSSM/CMSSW_10_2_12/src/NTupler/PATNTupler/main/slimjet24ht_new/WW_TuneCP5_PSweights_13TeV-pythia8/flatTree.root", cutToApply.c_str(), 51.723/0.6741/0.6741, SF_weight.c_str());
                    }
                    else if(sampleString.BeginsWith("WZ_TuneCP5_PSweights_13TeV-pythia8")) {
                        plotEntry.AddInput("/mercury/data2/linacre/NMSSM/CMSSW_10_2_12/src/NTupler/PATNTupler/main/slimjet24ht_new/WZ_TuneCP5_PSweights_13TeV-pythia8/flatTree.root", cutToApply.c_str(), 47.13, SF_weight.c_str());
                    }
                    else if(sampleString.BeginsWith("ZZTo4bQ01j_5f_amcatNLO_FXFX_pythia8")) {
                        plotEntry.AddInput("/mercury/data2/linacre/NMSSM/CMSSW_10_2_12/src/NTupler/PATNTupler/main/slimjet24ht_new/ZZTo4bQ01j_5f_amcatNLO_FXFX_pythia8/flatTree.root", cutToApply.c_str(), 16.523*0.1512*0.1512, SF_weight.c_str());
                    }
                    else if(sampleString.BeginsWith("ZZ_TuneCP5_13TeV-pythia8")) {
                        plotEntry.AddInput("/mercury/data2/linacre/NMSSM/CMSSW_10_2_12/src/NTupler/PATNTupler/main/slimjet24ht_new/ZZ_TuneCP5_13TeV-pythia8/flatTree.root", cutToApply.c_str(), 16.523, SF_weight.c_str());
                    }

                    // SIGNAL 2018
                    else if(sampleString=="mH70_mSusy1200") {
                        plotEntry.AddInput((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy1200/flatTree.root").c_str(), cutToApply.c_str(), 0.4951000*0.840*0.840, SF_weight.c_str());
                    }
                    else if(sampleString=="mH70_mSusy2000") {
                        plotEntry.AddInput((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy2000/flatTree.root").c_str(), cutToApply.c_str(), 0.0091050*0.840*0.840, SF_weight.c_str());
                    }
                    else if(sampleString=="mH70_mSusy2800") {
                        plotEntry.AddInput((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy2800/flatTree.root").c_str(), cutToApply.c_str(), 0.0002753*0.840*0.840, SF_weight.c_str());
                    }

/*
                    // SIGNAL 2017 as 2018
                    std::string SF_weight_copy = SF_weight;
                    SF_weight_copy += "*( nPU > 80 ? 0.944603013 : 1.007035333 - nPU*0.000780404 )";
                    std::cout<<"SF_weight_copy: "<<SF_weight_copy<<std::endl;


                    if(sampleString=="mH70_mSusy1200") {
                        plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.4951000*0.840*0.840, SF_weight_copy.c_str());
                    }
                    else if(sampleString=="mH70_mSusy2000") {
                        plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.0091050*0.840*0.840, SF_weight_copy.c_str());
                    }
                    else if(sampleString=="mH70_mSusy2800") {
                        plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy2800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.0002753*0.840*0.840, SF_weight_copy.c_str());
                    }

                    //plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/NAMEXYZ/flatTree.root", cutToApply.c_str(), 987654321.0, SF_weight_copy.c_str());
*/

                    // 2018 GOLDEN JSON DATASET
                    else if(sampleString=="data") {
                        plotEntry.AddInput((treepath+"JetHT_ALL/flatTree.root").c_str(), cutToApply.c_str());
                        //plotEntry.AddInput((treepath+"JetHT_RunA/flatTree.root").c_str(), cutToApply.c_str());
                        //plotEntry.AddInput((treepath+"JetHT_RunB/flatTree.root").c_str(), cutToApply.c_str());
                        //plotEntry.AddInput((treepath+"JetHT_RunC/flatTree.root").c_str(), cutToApply.c_str());
                        //plotEntry.AddInput((treepath+"JetHT_RunD/flatTree.root").c_str(), cutToApply.c_str());
                    }


                    // SIGNAL 2018 for batch submission
                    else if(sampleString=="signal") {
                        // hack to select nGluino=0 component for 2018 'squark' samples
                        if(is2018squark) plotEntry.AddInput2018squark("/opt/ppd/scratch-2021/xxt18833/Analysis_boostedNmssmHiggs/flatTrees_2020_02_05_CMSSW_10_2_12/mc/NAMEXYZ/tmp/flatTree_0.root", cutToApply.c_str(), 987654321.0, SF_weight.c_str());
                        else plotEntry.AddInput("/opt/ppd/scratch-2021/xxt18833/Analysis_boostedNmssmHiggs/flatTrees_2020_02_05_CMSSW_10_2_12/mc/NAMEXYZ/tmp/flatTree_0.root", cutToApply.c_str(), 987654321.0, SF_weight.c_str());
                    }


                    else {
                        std::cout << "No such sample "<<std::endl;
                    }





                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////

                    int binToFill = iCut4 * numberOfSegments + iMassRegion % numberOfSegments + 1;
                    h_[histogramName.c_str()]->SetBinContent(binToFill, plotEntry.GetNumberOfEventsAfterCuts());
                    h_[histogramName.c_str()]->SetBinError(binToFill, plotEntry.GetNumberOfEventsAfterCutsStatError());  


                    // // this section creates plots to check we trust what is going on
                    // std::string plotSaveName = outputDir + "/plot_" + histogramName + "_binNum" + std::to_string(binToFill) + ".pdf";
                    // Plotter plot = Plotter({plotEntry});
                    // plot.AddLatex(luminosity);
                    // plot.Save2D(plotSaveName.c_str(), MassCutsObject); // with the grid
                    

                    std::cout << "Systematic " << iSys+1 << " of " << systematicNameVec.size() << " ::: DONE " << counter << " of " << numberOfCutsForCodeTodo << " fills" << std::endl;
                    std::cout << std::endl;
                    counter++;

                } // closes loop through the HT bins
            } // closes loop through S,U,D mass regions
        } // closes loop through DBT configurations
    } // closes loop through systematics

    std::string saveName = massCutObjectName;
    saveName += Form("_ak8pt%d", cut3_ak8Pt);
    saveName += "_ht";
    for (size_t i = 0; i < cut4_ht.size(); ++i) saveName += Form("%dx", cut4_ht[i][0]);
    saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0], cut5_ak4Pt[1]);
    saveName += Form("_lumi%.0f", luminosity);
    saveName += ".root";
    std::string outputFile = outputDir + "/" + saveName;
    WriteHistograms(h_, outputFile);
    std::cout << "OutputFile is here: " << outputFile << std::endl;
    return 0;
}
