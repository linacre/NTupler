//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <fstream>
#include <sys/stat.h>

//ROOT HEADERS
#include <TH1D.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TLegend.h>

//RAL PARTICLE HEADERS
#include "PlotEntry.hh"
#include "Plotter.hh"
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"
#include "MacrosOnCondor.h"

// COMPARE VARIABLES FROM DIFFERENT DATASETS AS YOU LOOP THROUGH CUTS

int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////




    // ONE: save info
    std::string outputDir = "plotting_compare_SigEffCorrection"; // where we are going to save the output plots (should include the samples name, and any important features)




    // TWO: set of cut params, each combination = new plot
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    std::vector<std::vector<std::string>> cut2_ak8Dbt_SR = { {"DIAG_UP", "Loose"} }; // Top Diagnol Corner Crossing Axis at...--> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    //std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"DIAG_UP", "Loose"} }; // Top Diagnol Corner Crossing Axis at...--> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    //std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Loose","Off","Loose"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    std::vector<int> cut3_ak8Pt = {300};
    std::vector<std::vector<int>> cut4_ht = { {1500,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    //std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {2500,3500}, {3500,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {300,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)
    //std::vector<int> cut6_nTrueInt = {30,35};
    std::vector<int> cut6_nPU = {0,12};
    //std::vector<int> cut6_nrSlimJets = {7,9};




    // THREE: plot histogram settings
    //double luminosity = 35.867; // 2016 DATASET (data plots should not be using this object)
    double luminosity = 41.370; // 2017 DATASET
    // double luminosity = 59.740565202; // 2018 DATASET


    //std::string varToPlot = "fatJetA_doubleBtagDiscrim";
    //std::string varToPlot = "fatJetA_softDropMassPuppi";
    //std::string varToPlot = "fatJetA_mass";
    //std::string varToPlot = "fatJetA_nSubjettinessTau1";
    //std::string varToPlot = "fatJetA_nSubjettinessTau2";
    //std::string varToPlot = "fatJetA_p4.Pt()";
    //std::string varToPlot = "fatJetA_p4.Eta()";
    //std::string varToPlot = "slimJetA_p4.Pt()";
    std::string varToPlot = "slimJetA_p4.Eta()";
    //std::string varToPlot = "fatJetB_doubleBtagDiscrim";
    //std::string varToPlot = "fatJetB_softDropMassPuppi";
    //std::string varToPlot = "fatJetB_mass";
    //std::string varToPlot = "fatJetB_p4.Pt()";
    //std::string varToPlot = "fatJetB_p4.Eta()";
    //std::string varToPlot = "slimJetB_p4.Pt()";
    //std::string varToPlot = "slimJetB_p4.Eta()";
    //std::string varToPlot = "ht";
    //std::string varToPlot = "nPU";
    //std::string varToPlot = "nrSlimJets";
    //std::string varToPlot = "nTrueInt";
    //std::string varToPlot = "nGluino";
    //std::string varToPlot = "nPrefireCandidates";
    //std::string varToPlot = "trgDecision";

    //TH1D hTemplate("hTemplate", ";fatJetA doubleBtagDiscriminator;AU", 40, -1, 1);
    //TH1D hTemplate("hTemplate", ";fatJetA SoftDropMass (GeV);AU", 40, 0, 200);
    //TH1D hTemplate("hTemplate", ";fatJetA Mass (GeV);AU", 40, 0, 200);
    //TH1D hTemplate("hTemplate", ";fatJetA nSubjettinessTau1;AU", 40, 0, 0.7);
    //TH1D hTemplate("hTemplate", ";fatJetA nSubjettinessTau2;AU", 40, 0, 0.35);
    //TH1D hTemplate("hTemplate", ";fatJetA p_{T} (GeV);AU", 60, 0, 3000);
    //TH1D hTemplate("hTemplate", ";fatJetA #eta;AU", 70, -3.5, 3.5);
    //TH1D hTemplate("hTemplate", ";leadingAK4Jet p_{T} (GeV);AU", 60, 0, 3000);
    TH1D hTemplate("hTemplate", ";leadingAK4Jet #eta;AU", 70, -3.5, 3.5);    
    //TH1D hTemplate("hTemplate", ";fatJetB doubleBtagDiscriminator;AU", 5, -1, 1);
    //TH1D hTemplate("hTemplate", ";fatJetB SoftDropMass (GeV);AU", 40, 0, 200);
    //TH1D hTemplate("hTemplate", ";fatJetB Mass (GeV);AU", 40, 0, 200);
    //TH1D hTemplate("hTemplate", ";fatJetB p_{T} (GeV);AU", 60, 0, 3000);
    //TH1D hTemplate("hTemplate", ";fatJetB #eta;AU", 70, -3.5, 3.5);
    //TH1D hTemplate("hTemplate", ";subleadingAK4Jet p_{T} (GeV);AU", 60, 0, 3000);
    //TH1D hTemplate("hTemplate", ";subleadingAK4Jet #eta;AU", 70, -3.5, 3.5);
    //TH1D hTemplate("hTemplate", ";H_{T} (GeV);AU", 90, 0, 9000);
    //TH1D hTemplate("hTemplate", ";nPU;AU", 90, 0, 90);
    //TH1D hTemplate("hTemplate", ";nrSlimJets;AU", 20, 0, 20);
    //TH1D hTemplate("hTemplate", ";nTrueInt;AU", 90, 0, 90);
    //TH1D hTemplate("hTemplate", ";nGluino;AU", 4, 0, 4);
    //TH1D hTemplate("hTemplate", ";nPrefireCandidates;AU", 5, 0, 5);
    //TH1D hTemplate("hTemplate", ";trgDecision;AU", 2, 0, 2);

    std::string varToPlot_SReff = "nPU";
    TH1D hNumerator("hNumerator", ";nPU;SR efficiency;AU", 8, 0, 80);

    //std::string varToPlot_SReff = "abs(fatJetA_p4.Eta())";
    //TH1D hNumerator("hNumerator", ";fatJetA #eta;SR efficiency;AU", 7, 0, 3.5);

    //std::string varToPlot_SReff = "abs(fatJetB_p4.Eta())";
    //TH1D hNumerator("hNumerator", ";fatJetB #eta;SR efficiency;AU", 7, 0, 3.5);


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    if (argc != 2){
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
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_2017to2018SigEffCorrection.cc %s/%s__plotting_2017to2018SigEffCorrection.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_2017to2018SigEffCorrection.cc %s/%s__plotting_2017to2018SigEffCorrection.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "plotting_compare_SigEffCorrection", "/opt/ppd/scratch/xxt18833/jobLogs/macros/");
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

    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::vector<PlotEntry> plotEntryVec;
                    std::vector<PlotEntry> plotEntryVec2;
                    std::vector<PlotEntry> plotEntryNumerator;
                    std::vector<PlotEntry> plotEntryDenominator;
                    
                    std::string dbtCut = "";
                    std::string dbtCut_SR = Form("fatJetA_doubleBtagDiscrim >= (-1.0 * fatJetB_doubleBtagDiscrim + 1.0 + %f) ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt_SR[iCut2][1]) );;
                    if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP")
                        dbtCut = Form("fatJetA_doubleBtagDiscrim >= (-1.0 * fatJetB_doubleBtagDiscrim + 1.0 + %f) ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]) );
                    if (cut2_ak8Dbt[iCut2].size() == 4)
                        dbtCut = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]) );
                    std::string cutToApply = Form("%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", dbtCut.c_str(), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    //std::string cutToApply = Form("%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d && nTrueInt>=%d && nTrueInt<%d", dbtCut.c_str(), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1], cut6_nTrueInt[0], cut6_nTrueInt[1]);
                    //std::string cutToApply = Form("%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d && nPU>=%d && nPU<%d", dbtCut.c_str(), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1], cut6_nPU[0], cut6_nPU[1]);
                    //std::string cutToApply = Form("%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d && nrSlimJets>=%d && nrSlimJets<%d", dbtCut.c_str(), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1], cut6_nrSlimJets[0], cut6_nrSlimJets[1]);
                    
                    std::string cutToApply_Numerator = Form("%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", dbtCut.c_str(), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    //std::string cutToApply_Denominator = Form("%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", dbtCut_SR.c_str(), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);


      
                    // FOUR: samples to use
                    std::string treepath = "/mercury/data2/linacre/NMSSM/CMSSW_10_2_12/src/NTupler/PATNTupler/main/slimjet24ht/";


                    PlotEntry mH70_mSusy1200_2018 = PlotEntry("mH70_mSusy1200_2018", hTemplate, varToPlot.c_str(), luminosity);
                    mH70_mSusy1200_2018.AddInput((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy1200/flatTree.root").c_str(), cutToApply.c_str(), 1, "weight_combined");
                    mH70_mSusy1200_2018.NormalisePlot();
                    plotEntryVec2.push_back(mH70_mSusy1200_2018);

                    PlotEntry mH70_mSusy2000_2018 = PlotEntry("mH70_mSusy2000_2018", hTemplate, varToPlot.c_str(), luminosity);
                    mH70_mSusy2000_2018.AddInput((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy2000/flatTree.root").c_str(), cutToApply.c_str(), 1, "weight_combined");
                    mH70_mSusy2000_2018.NormalisePlot();
                    plotEntryVec2.push_back(mH70_mSusy2000_2018);

                    PlotEntry mH70_mSusy2800_2018 = PlotEntry("mH70_mSusy2800_2018", hTemplate, varToPlot.c_str(), luminosity);
                    mH70_mSusy2800_2018.AddInput((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy2800/flatTree.root").c_str(), cutToApply.c_str(), 1, "weight_combined");
                    mH70_mSusy2800_2018.NormalisePlot();
                    plotEntryVec2.push_back(mH70_mSusy2800_2018);


                    //reprocessed 2017 MC as 2018 in nTupAnaNMSSM
                    PlotEntry mH70_mSusy1200_2017 = PlotEntry("mH70_mSusy1200_2017", hTemplate, varToPlot.c_str(), luminosity);
                    mH70_mSusy1200_2017.AddInput("/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/flatTrees_2020_02_05_CMSSW_10_2_12/mc/mH70p0_mSusy1200p0_ratio0p99_splitting0p1/tmp/flatTree_0.root", cutToApply.c_str(), 1, "weight_combined");
                    mH70_mSusy1200_2017.NormalisePlot();
                    plotEntryVec2.push_back(mH70_mSusy1200_2017);

                    PlotEntry mH70_mSusy2000_2017 = PlotEntry("mH70_mSusy2000_2017", hTemplate, varToPlot.c_str(), luminosity);
                    mH70_mSusy2000_2017.AddInput("/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/flatTrees_2020_02_05_CMSSW_10_2_12/mc/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/tmp/flatTree_0.root", cutToApply.c_str(), 1, "weight_combined");
                    mH70_mSusy2000_2017.NormalisePlot();
                    plotEntryVec2.push_back(mH70_mSusy2000_2017);

                    PlotEntry mH70_mSusy2800_2017 = PlotEntry("mH70_mSusy2800_2017", hTemplate, varToPlot.c_str(), luminosity);
                    mH70_mSusy2800_2017.AddInput("/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/flatTrees_2020_02_05_CMSSW_10_2_12/mc/mH70p0_mSusy2800p0_ratio0p99_splitting0p1/tmp/flatTree_0.root", cutToApply.c_str(), 1, "weight_combined");
                    mH70_mSusy2800_2017.NormalisePlot();
                    plotEntryVec2.push_back(mH70_mSusy2800_2017);


/*
                    //original 2017 MC with slim jet eta cut of 3.0 instead of 2.4
                    PlotEntry mH70_mSusy1200_2017 = PlotEntry("mH70_mSusy1200_2017", hTemplate, varToPlot.c_str(), luminosity);
                    mH70_mSusy1200_2017.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 1, "weight_combined");
                    mH70_mSusy1200_2017.NormalisePlot();
                    plotEntryVec2.push_back(mH70_mSusy1200_2017);

                    PlotEntry mH70_mSusy2000_2017 = PlotEntry("mH70_mSusy2000_2017", hTemplate, varToPlot.c_str(), luminosity);
                    mH70_mSusy2000_2017.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 1, "weight_combined");
                    mH70_mSusy2000_2017.NormalisePlot();
                    plotEntryVec2.push_back(mH70_mSusy2000_2017);

                    PlotEntry mH70_mSusy2800_2017 = PlotEntry("mH70_mSusy2800_2017", hTemplate, varToPlot.c_str(), luminosity);
                    mH70_mSusy2800_2017.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy2800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 1, "weight_combined");
                    mH70_mSusy2800_2017.NormalisePlot();
                    plotEntryVec2.push_back(mH70_mSusy2800_2017);
*/








                    PlotEntry Numerator_mH70_mSusy1200_2018 = PlotEntry("mH70_mSusy1200_2018", hNumerator, varToPlot_SReff.c_str(), luminosity);
                    Numerator_mH70_mSusy1200_2018.AddInputEfficiency((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy1200/flatTree.root").c_str(), cutToApply_Numerator.c_str(), dbtCut_SR.c_str(), true);
                    plotEntryNumerator.push_back(Numerator_mH70_mSusy1200_2018);

                    PlotEntry Numerator_mH70_mSusy2000_2018 = PlotEntry("mH70_mSusy2000_2018", hNumerator, varToPlot_SReff.c_str(), luminosity);
                    Numerator_mH70_mSusy2000_2018.AddInputEfficiency((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy2000/flatTree.root").c_str(), cutToApply_Numerator.c_str(), dbtCut_SR.c_str(), true);
                    plotEntryNumerator.push_back(Numerator_mH70_mSusy2000_2018);

                    PlotEntry Numerator_mH70_mSusy2800_2018 = PlotEntry("mH70_mSusy2800_2018", hNumerator, varToPlot_SReff.c_str(), luminosity);
                    Numerator_mH70_mSusy2800_2018.AddInputEfficiency((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy2800/flatTree.root").c_str(), cutToApply_Numerator.c_str(), dbtCut_SR.c_str(), true);
                    plotEntryNumerator.push_back(Numerator_mH70_mSusy2800_2018);


                    //reprocessed 2017 MC as 2018 in nTupAnaNMSSM
                    PlotEntry Numerator_mH70_mSusy1200_2017 = PlotEntry("mH70_mSusy1200_2017", hNumerator, varToPlot_SReff.c_str(), luminosity);
                    Numerator_mH70_mSusy1200_2017.AddInputEfficiency("/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/flatTrees_2020_02_05_CMSSW_10_2_12/mc/mH70p0_mSusy1200p0_ratio0p99_splitting0p1/tmp/flatTree_0.root", cutToApply_Numerator.c_str(), dbtCut_SR.c_str(), true);
                    plotEntryNumerator.push_back(Numerator_mH70_mSusy1200_2017);

                    PlotEntry Numerator_mH70_mSusy2000_2017 = PlotEntry("mH70_mSusy2000_2017", hNumerator, varToPlot_SReff.c_str(), luminosity);
                    Numerator_mH70_mSusy2000_2017.AddInputEfficiency("/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/flatTrees_2020_02_05_CMSSW_10_2_12/mc/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/tmp/flatTree_0.root", cutToApply_Numerator.c_str(), dbtCut_SR.c_str(), true);
                    plotEntryNumerator.push_back(Numerator_mH70_mSusy2000_2017);

                    PlotEntry Numerator_mH70_mSusy2800_2017 = PlotEntry("mH70_mSusy2800_2017", hNumerator, varToPlot_SReff.c_str(), luminosity);
                    Numerator_mH70_mSusy2800_2017.AddInputEfficiency("/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/flatTrees_2020_02_05_CMSSW_10_2_12/mc/mH70p0_mSusy2800p0_ratio0p99_splitting0p1/tmp/flatTree_0.root", cutToApply_Numerator.c_str(), dbtCut_SR.c_str(), true);
                    plotEntryNumerator.push_back(Numerator_mH70_mSusy2800_2017);


/*
                    //original 2017 MC with slim jet eta cut of 3.0 instead of 2.4
                    PlotEntry Numerator_mH70_mSusy1200_2017 = PlotEntry("mH70_mSusy1200_2017", hNumerator, varToPlot_SReff.c_str(), luminosity);
                    Numerator_mH70_mSusy1200_2017.AddInputEfficiency("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply_Numerator.c_str(), dbtCut_SR.c_str(), true);
                    plotEntryNumerator.push_back(Numerator_mH70_mSusy1200_2017);

                    PlotEntry Numerator_mH70_mSusy2000_2017 = PlotEntry("mH70_mSusy2000_2017", hNumerator, varToPlot_SReff.c_str(), luminosity);
                    Numerator_mH70_mSusy2000_2017.AddInputEfficiency("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply_Numerator.c_str(), dbtCut_SR.c_str(), true);
                    plotEntryNumerator.push_back(Numerator_mH70_mSusy2000_2017);

                    PlotEntry Numerator_mH70_mSusy2800_2017 = PlotEntry("mH70_mSusy2800_2017", hNumerator, varToPlot_SReff.c_str(), luminosity);
                    Numerator_mH70_mSusy2800_2017.AddInputEfficiency("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy2800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply_Numerator.c_str(), dbtCut_SR.c_str(), true);
                    plotEntryNumerator.push_back(Numerator_mH70_mSusy2800_2017);
*/







                    // FIVE: plot aesthetics
                    // Plotter plot = Plotter(plotEntryVec);
                    Plotter plot = Plotter(plotEntryVec2);
                    //Plotter plot = Plotter(plotEntryVec2, plotEntryVec);
                    // Plotter plot = Plotter({}, plotEntryVec);

                    // plot.AddLegend(0.74, 0.88, 0.77, 0.87); // top right (thin 2)
                    // plot.AddLegend(0.65, 0.88, 0.77, 0.87); // top right (wide 2)
                    // plot.AddLegend(0.47, 0.88, 0.73, 0.87); // top right (extra wide 2)
                    // plot.AddLegend(0.28, 0.51, 0.20, 0.35); // bottom centre (extra wide 2)
                    // plot.AddLegend(0.74, 0.88, 0.70, 0.87); // top right (thin 4)
                    // plot.AddLegend(0.60, 0.88, 0.70, 0.87); // top right (wide 4)
                    // plot.AddLegend(0.50, 0.88, 0.70, 0.87); // top right (extra wide 4)
                    // plot.AddLegend(0.33, 0.88, 0.70, 0.87); // top right (extra wide 4)
                    // plot.AddLegend(0.33, 0.88, 0.50, 0.67); // top right (extra wide 4)
                    plot.AddLegend2Cols(0, 0.43, 0.89, 0.77, 0.87, 0.03); // 2 COLUMNS
                    // plot.AddLegend(0.20, 0.60, 0.20, 0.33); // bottom left (wide 2)
                    // plot.AddLegend(0.20, 0.60, 0.17, 0.37); // bottom left (wide 4)
                    // plot.AddLegend(0.55, 0.88, 0.17, 0.37); // bottom right (wide 4)
                    // plot.AddLegend(0.22, 0.44, 0.77, 0.87); // top left (wide 2)
                    // plot.AddLegend(0.22, 0.45, 0.70, 0.87); // top left (wide 4)

                    // plot.AddLatex(); // simulation only - normalised
                    plot.AddLatex(luminosity); // simulation only - NOT normalised
                    // plot.AddLatex(luminosity, "#it{Preliminary}"); // DATA
                    // plot.AddLatex("#it{Preliminary}"); // DATA
                    
                    // plot.AddRatioBox(0.1, 1.9, "ratio", true);
                    // plot.AddRatioBox(0.929, 1.109);
                    // plot.AddRatioBox();
                    plot.SetErrors();

                    plot.CalculateKS(3);







                    std::string saveName = varToPlot;
                    if (cut2_ak8Dbt.size()>1) saveName += "__dbtVary";
                    else{
                        if (cut2_ak8Dbt[0].size() == 4) saveName += "__dbt" + cut2_ak8Dbt[0][0] + cut2_ak8Dbt[0][1] + "And" + cut2_ak8Dbt[0][2] + cut2_ak8Dbt[0][3];
                        if (cut2_ak8Dbt[0].size() == 2 && cut2_ak8Dbt[0][0] == "DIAG_UP") saveName += "__dbtDiagUp" + cut2_ak8Dbt[0][1];
                    }
                    saveName += Form("_ak8pt%d", cut3_ak8Pt[iCut3]);
                    if (cut4_ht[iCut4][1]==99999) saveName += Form("_ht%dplus",cut4_ht[iCut4][0]);
                    else saveName += Form("_ht%dto%d",cut4_ht[iCut4][0],cut4_ht[iCut4][1]);
                    saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    // saveName += "APPEND_SAVE_NAME"; // OPTIONAL: can manually append the automatic savename
                    // saveName = "MANUAL_SAVE_NAME"; // OPTIONAL: can manually overwrite the automatic savename
                    std::string outputFile = outputDir + "/" + saveName + ".pdf";
                    plot.Save(outputFile.c_str());
                    plot.SetLogY();
                    outputFile = outputDir + "/" + saveName + "__logY.pdf";
                    plot.Save(outputFile.c_str());







                    // SR eff plot
                    Plotter plot_SReff = Plotter(plotEntryNumerator);
                    plot_SReff.AddLegend(0.50, 0.88, 0.14, 0.37); // top right (extra wide 2)
                    plot_SReff.AddLatex(); // FOR SIMULATION
                    //plot_SReff.AddLatex(luminosity, "#it{Preliminary}"); // FOR DATA

                    std::string saveName_SReff = "_SReff";
                    std::string outputFile_SReff = outputDir + "/" + varToPlot_SReff + saveName_SReff + ".pdf";
                    plot_SReff.Save(outputFile_SReff.c_str());

                    plot_SReff.DivideHistos();
                    Plotter plot_SReff_DR = Plotter(plotEntryNumerator);
                    //plot_SReff_DR.AddLegend(0.50, 0.88, 0.14, 0.37); // top right (extra wide 2)
                    plot_SReff_DR.AddLegend2Cols(0, 0.16, 0.89, 0.14, 0.25, 0.036); // 2 COLUMNS
                    plot_SReff_DR.AddLatex(); // FOR SIMULATION
                    //plot_SReff_DR.AddLatex(luminosity, "#it{Preliminary}"); // FOR DATA


                    outputFile_SReff = outputDir + "/" + varToPlot_SReff + saveName_SReff + "_doubleRatio.pdf";
                    plot_SReff_DR.Save(outputFile_SReff.c_str());





                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states
    return 0;
}