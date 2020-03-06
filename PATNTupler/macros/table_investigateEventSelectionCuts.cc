//STL HEADERS
#include <iostream>
#include <iomanip>
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
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"
#include "MacrosOnCondor.h"
#include "MassRegionCuts.hh"

// MAKE CUT FLOW TABLES

int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "cutFlowTables_201718_Signal_ht_mass_separate/"; // where we are going to save the output plots (should include the samples name, and any important features)



    // TWO: set of cut params (first element is the choice for an individual table, second element is the choice for an individual table, etc.)
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"DIAG_UP", "Loose"}, {"DIAG_UP", "Loose"}, {"DIAG_UP", "Loose"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"}, {"Off","Max","Off","Max"}, {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    std::vector<int> cut3_ak8Pt = {300, 300, 300};
    std::vector<std::vector<int>>cut4_ht = { {1500,2500}, {2500,3500}, {3500,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {300,-1}, {300,-1}, {300,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)



    // THREE: lumi settings
    double luminosity = 41.529; // 2016 DATASET



    // FOUR: set the mass regions (use a KEYNAME and keep track of what means what!!!!)
    double S1_Node1 = 40.0;
    double S1_Node2 = 17.6;
    double SMAX_Node1 = 170.7;
    double SMAX_Node2 = 113.7;
    std::vector<double> SN_Nodes = {51.9, 65.1, 78.3, 91.5, 104.7, 117.9, 131.1, 144.3, 157.5};
    MassRegionCuts MassCutsObject = MassRegionCuts("MassCutsV09", S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2, SN_Nodes);
    //const double sideBandScaleFactor = 0.5;
    const std::string massCutObjectName = "MassCutsV09";

    //std::string fatJetA_mass_name = "fatJetA_softDropMassPuppi";
    //std::string fatJetB_mass_name = "fatJetB_softDropMassPuppi";

    const unsigned int numberOfSegments = SN_Nodes.size() + 1;

    //MassRegionCuts MassCutsObject = MassRegionCuts(massCutObjectName.c_str(), S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2, SN_Nodes, fatJetA_mass_name, fatJetB_mass_name, sideBandScaleFactor);

    std::string MassRegionCuts = "( ";

    for (size_t iMassRegion = 0; iMassRegion!=numberOfSegments; ++iMassRegion){
        MassRegionCuts += "( " + MassCutsObject.GetAllCuts()[iMassRegion] + " )";
        if(iMassRegion != numberOfSegments - 1) MassRegionCuts += " || ";
        else MassRegionCuts += " )";
    }

    //std::cout << MassRegionCuts << std::endl;


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

    if (cut2_ak8Dbt.size() != cut3_ak8Pt.size() || cut3_ak8Pt.size() != cut4_ht.size() || cut4_ht.size() != cut5_ak4Pt.size()){
        std::cout << "stepXlabels not all same size, exiting..." << std::endl;
        return 1;
    }

    std::string runInstructionString(argv[1]);
    
    if (runInstructionString == "local"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/table_investigateEventSelectionCuts.cc %s/%s__table_investigateEventSelectionCuts.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/table_investigateEventSelectionCuts.cc %s/%s__table_investigateEventSelectionCuts.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "table_investigateEventSelectionCuts", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
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
    std::string varToPlot = "fatJetA_softDropMassPuppi";    
    TH1D hTemplate("hTemplate", ";fatJetA_SoftDrop_Mass NOT FOR PLOTTING (GeV);NOT FOR PLOTTING", 1, -99991, 99991);

    for (size_t iTab = 0; iTab < cut3_ak8Pt.size(); ++iTab){ // loop through different table configurations

        std::string dbtCut = "";
        if (cut2_ak8Dbt[iTab].size() == 2 && cut2_ak8Dbt[iTab][0] == "DIAG_UP")
            dbtCut = Form("fatJetA_doubleBtagDiscrim >= (-1.0 * fatJetB_doubleBtagDiscrim + 1.0 + %f) ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iTab][1]) );
        if (cut2_ak8Dbt[iTab].size() == 4)
            dbtCut = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iTab][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iTab][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iTab][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iTab][3]) );

        // get the full set of cuts for the table
        std::vector<std::string> cutToApplyVec;
        cutToApplyVec.push_back("weight_isr");
        cutToApplyVec.push_back(Form("weight_isr * (fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d)", cut3_ak8Pt[iTab], cut3_ak8Pt[iTab]));
        cutToApplyVec.push_back(Form("weight_isr * (fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d)", cut3_ak8Pt[iTab], cut3_ak8Pt[iTab], cut5_ak4Pt[iTab][0], cut5_ak4Pt[iTab][1]));
        cutToApplyVec.push_back(Form("weight_isr * (fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d)", cut3_ak8Pt[iTab], cut3_ak8Pt[iTab], cut4_ht[iTab][0], cut4_ht[iTab][1], cut5_ak4Pt[iTab][0], cut5_ak4Pt[iTab][1]));
        cutToApplyVec.push_back(Form("weight_isr * weight_dbtTag * (%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d)", dbtCut.c_str(), cut3_ak8Pt[iTab], cut3_ak8Pt[iTab], cut4_ht[iTab][0], cut4_ht[iTab][1], cut5_ak4Pt[iTab][0], cut5_ak4Pt[iTab][1]));
        cutToApplyVec.push_back(Form("weight_isr * weight_dbtTag * (%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d && %s )", dbtCut.c_str(), cut3_ak8Pt[iTab], cut3_ak8Pt[iTab], cut4_ht[iTab][0], cut4_ht[iTab][1], cut5_ak4Pt[iTab][0], cut5_ak4Pt[iTab][1], MassRegionCuts.c_str() ));

        for (size_t iMassRegion = 0; iMassRegion!=numberOfSegments; ++iMassRegion){
            cutToApplyVec.push_back(Form("weight_isr * weight_dbtTag * (%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d && ( %s ) )", dbtCut.c_str(), cut3_ak8Pt[iTab], cut3_ak8Pt[iTab], cut4_ht[iTab][0], cut4_ht[iTab][1], cut5_ak4Pt[iTab][0], cut5_ak4Pt[iTab][1], MassCutsObject.GetAllCuts()[iMassRegion].c_str() ));
        }

        // get the required count info for the table
        std::vector<std::vector<PlotEntry>> cutFlowObject; // all cut stages, for all samples
        for (unsigned int iC = 0; iC != cutToApplyVec.size(); ++iC){


            std::vector<PlotEntry> givenCutObject; // single cut stage, for all samples

            // FOUR: SAMPLE INFO

/*
            // original 2017 MC with slim jet eta cut of 3.0 instead of 2.4
            givenCutObject.push_back( PlotEntry("mH70_mSusy1200", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApplyVec[iC].c_str(), 0.4951000*0.840*0.840);

            givenCutObject.push_back( PlotEntry("mH70_mSusy2000", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApplyVec[iC].c_str(), 0.0091050*0.840*0.840);

            givenCutObject.push_back( PlotEntry("mH70_mSusy2800", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH70p0_mSusy2800p0_ratio0p99_splitting0p1/flatTree.root", cutToApplyVec[iC].c_str(), 0.0002753*0.840*0.840);
*/

            //reprocessed 2017 MC as 2018 in nTupAnaNMSSM
            givenCutObject.push_back( PlotEntry("mH70_mSusy1200", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/flatTrees_2020_02_05_CMSSW_10_2_12/mc/mH70p0_mSusy1200p0_ratio0p99_splitting0p1/tmp/flatTree_0.root", cutToApplyVec[iC].c_str(), 0.4951000*0.840*0.840);

            givenCutObject.push_back( PlotEntry("mH70_mSusy2000", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/flatTrees_2020_02_05_CMSSW_10_2_12/mc/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/tmp/flatTree_0.root", cutToApplyVec[iC].c_str(), 0.0091050*0.840*0.840);

            givenCutObject.push_back( PlotEntry("mH70_mSusy2800", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/flatTrees_2020_02_05_CMSSW_10_2_12/mc/mH70p0_mSusy2800p0_ratio0p99_splitting0p1/tmp/flatTree_0.root", cutToApplyVec[iC].c_str(), 0.0002753*0.840*0.840);


            //2018 signal
            std::string treepath = "/mercury/data2/linacre/NMSSM/CMSSW_10_2_12/src/NTupler/PATNTupler/main/slimjet24ht/";

            givenCutObject.push_back( PlotEntry("mH70_mSusy1200", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy1200/flatTree.root").c_str(), cutToApplyVec[iC].c_str(), 0.4951000*0.840*0.840);

            givenCutObject.push_back( PlotEntry("mH70_mSusy2000", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy2000/flatTree.root").c_str(), cutToApplyVec[iC].c_str(), 0.0091050*0.840*0.840);

            givenCutObject.push_back( PlotEntry("mH70_mSusy2800", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput((treepath+"nmssmSignalCascadeV05_mH70p0_mSusy2800/flatTree.root").c_str(), cutToApplyVec[iC].c_str(), 0.0002753*0.840*0.840);


/*
            // ********************
            // ********************
            // ********************

            //2017 background
            givenCutObject.push_back( PlotEntry("QCD HT>1000", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017_forTablesOnly/QCD_HT1000to1500/flatTree.root", cutToApplyVec[iC].c_str(), 1005);
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017_forTablesOnly/QCD_HT1500to2000/flatTree.root", cutToApplyVec[iC].c_str(), 101.8);
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017_forTablesOnly/QCD_HT2000toInf/flatTree.root", cutToApplyVec[iC].c_str(), 20.54);

            givenCutObject.push_back( PlotEntry("TT+Jets", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017_forTablesOnly/TTToHadronic/flatTree.root", cutToApplyVec[iC].c_str(), 377.96);
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017_forTablesOnly/TTToSemiLeptonic/flatTree.root", cutToApplyVec[iC].c_str(), 365.34);
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017_forTablesOnly/TTTo2L2Nu/flatTree.root", cutToApplyVec[iC].c_str(), 88.29);

            givenCutObject.push_back( PlotEntry("Z+Jets HT>800", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017_forTablesOnly/ZJets/flatTree.root", cutToApplyVec[iC].c_str(), 18.69);

            givenCutObject.push_back( PlotEntry("W+Jets HT>800", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017_forTablesOnly/WJets/flatTree.root", cutToApplyVec[iC].c_str(), 34.00);


            //2018 background
            std::string treepath = "/mercury/data2/linacre/NMSSM/CMSSW_10_2_12/src/NTupler/PATNTupler/main/slimjet24ht/";

            givenCutObject.push_back( PlotEntry("QCD HT>1000", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput((treepath+"QCD_HT1000to1500_TuneCP5_13TeV-madgraphMLM-pythia8/flatTree.root").c_str(), cutToApplyVec[iC].c_str(), 1005);
            givenCutObject[givenCutObject.size()-1].AddInput((treepath+"QCD_HT1500to2000_TuneCP5_13TeV-madgraphMLM-pythia8/flatTree.root").c_str(), cutToApplyVec[iC].c_str(), 101.8);
            givenCutObject[givenCutObject.size()-1].AddInput((treepath+"QCD_HT2000toInf_TuneCP5_13TeV-madgraphMLM-pythia8/flatTree.root").c_str(), cutToApplyVec[iC].c_str(), 20.54);

            givenCutObject.push_back( PlotEntry("TT+Jets", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput((treepath+"TTToHadronic_TuneCP5_13TeV-powheg-pythia8/flatTree.root").c_str(), cutToApplyVec[iC].c_str(), 377.96);
            givenCutObject[givenCutObject.size()-1].AddInput((treepath+"TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/flatTree.root").c_str(), cutToApplyVec[iC].c_str(), 365.34);
            givenCutObject[givenCutObject.size()-1].AddInput((treepath+"TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/flatTree.root").c_str(), cutToApplyVec[iC].c_str(), 88.29);

            givenCutObject.push_back( PlotEntry("Z+Jets HT>800", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput((treepath+"ZJetsToQQ_HT-800toInf_qc19_4j_TuneCP5_13TeV-madgraphMLM-pythia8/flatTree.root").c_str(), cutToApplyVec[iC].c_str(), 18.69);

            givenCutObject.push_back( PlotEntry("W+Jets HT>800", hTemplate, varToPlot.c_str(), luminosity) );
            givenCutObject[givenCutObject.size()-1].AddInput((treepath+"WJetsToQQ_HT-800toInf_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8/flatTree.root").c_str(), cutToApplyVec[iC].c_str(), 34.00);
*/

            cutFlowObject.push_back(givenCutObject);
        }

        // create the cut flow table
        std::ofstream table;
        table << std::fixed;
        std::string cutName = "";
        if (cut2_ak8Dbt[iTab].size() == 4) cutName += "dbt" + cut2_ak8Dbt[iTab][0] + cut2_ak8Dbt[iTab][1] + "And" + cut2_ak8Dbt[iTab][2] + cut2_ak8Dbt[iTab][3];
        if (cut2_ak8Dbt[iTab].size() == 2 && cut2_ak8Dbt[iTab][0] == "DIAG_UP") cutName += "dbtDiagUp" + cut2_ak8Dbt[iTab][1];
        cutName += Form("_ak8pt%d", cut3_ak8Pt[iTab]);
        if (cut4_ht[iTab][1]==99999) cutName += Form("_ht%dplus",cut4_ht[iTab][0]);
        else cutName += Form("_ht%dto%d",cut4_ht[iTab][0],cut4_ht[iTab][1]);
        cutName += Form("_ak4pt%dn%d", cut5_ak4Pt[iTab][0], cut5_ak4Pt[iTab][1]);
        table.open( Form("%s/cutCount_%.0finvFb__%s.csv", outputDir.c_str(), luminosity, cutName.c_str()) );
        table << "Luminosity = " << luminosity << "fb^-1," << "\n";
        table << "col1: Number of Events\n";
        table << "col2: Fraction from last stage\n";
        table << "bottom: Total fraction\n";
        table << ",";
        for (size_t i = 0; i < cutFlowObject[0].size(); ++i) table << cutFlowObject[0][i].GetPlotEntryName().c_str() << ",,,";
        std::vector<double> holdValueVec(cutFlowObject[0].size(), 0.0);
        std::vector<double> holdValueVec2(cutFlowObject[0].size(), 0.0);

        table << "\nPreCuts:,";
        for (size_t i = 0; i < cutFlowObject[0].size(); ++i){
            table << cutFlowObject[0][i].GetNumberOfEventsBeforeCuts() << ",,,";
            holdValueVec[i] = cutFlowObject[0][i].GetNumberOfEventsBeforeCuts();
        }
        
        table << "\n2 FatJets (pre-selection):,";
        for (size_t i = 0; i < cutFlowObject[0].size(); ++i){
            table << std::setprecision(6) << cutFlowObject[0][i].GetNumberOfEventsAfterCuts() << ",";
            if (cutFlowObject[0][i].GetNumberOfEventsAfterCuts() < 0.00000001) table << "0,,";
            else table << cutFlowObject[0][i].GetNumberOfEventsAfterCuts() / holdValueVec[i] << ",,";
            holdValueVec[i] = cutFlowObject[0][i].GetNumberOfEventsAfterCuts();
        }

        table << "\nAK8PT " << cut3_ak8Pt[iTab] << "(GeV):,";
        for (size_t i = 0; i < cutFlowObject[0].size(); ++i){
            table << std::setprecision(6) << cutFlowObject[1][i].GetNumberOfEventsAfterCuts() << ",";
            if (cutFlowObject[1][i].GetNumberOfEventsAfterCuts() < 0.00000001) table << "0,,";
            else table << cutFlowObject[1][i].GetNumberOfEventsAfterCuts() / holdValueVec[i] << ",,";
            holdValueVec[i] = cutFlowObject[1][i].GetNumberOfEventsAfterCuts();
        }
        
        table << "\nAK4PT " << cut5_ak4Pt[iTab][0] << "&" << cut5_ak4Pt[iTab][1] << "(GeV):,";
        for (size_t i = 0; i < cutFlowObject[0].size(); ++i){
            table << std::setprecision(6) << cutFlowObject[2][i].GetNumberOfEventsAfterCuts() << ",";
            if (cutFlowObject[2][i].GetNumberOfEventsAfterCuts() < 0.00000001) table << "0,,";
            else table << cutFlowObject[2][i].GetNumberOfEventsAfterCuts() / holdValueVec[i] << ",,";
            holdValueVec[i] = cutFlowObject[2][i].GetNumberOfEventsAfterCuts();
        }

        table << "\nHT " << cut4_ht[iTab][0] << "-" << cut4_ht[iTab][1] << "(GeV):,";
        for (size_t i = 0; i < cutFlowObject[0].size(); ++i){
            table << std::setprecision(6) << cutFlowObject[3][i].GetNumberOfEventsAfterCuts() << ",";
            if (cutFlowObject[3][i].GetNumberOfEventsAfterCuts() < 0.00000001) table << "0,,";
            else table << cutFlowObject[3][i].GetNumberOfEventsAfterCuts() / holdValueVec[i] << ",,";
            holdValueVec[i] = cutFlowObject[3][i].GetNumberOfEventsAfterCuts();
        }        

        if (cut2_ak8Dbt[iTab].size() == 4) table << "\nAK8DBT " << cut2_ak8Dbt[iTab][0].c_str() << cut2_ak8Dbt[iTab][1].c_str() << "&" << cut2_ak8Dbt[iTab][2].c_str() << cut2_ak8Dbt[iTab][3].c_str() << ":,";
        if (cut2_ak8Dbt[iTab].size() == 2 && cut2_ak8Dbt[iTab][0] == "DIAG_UP") table << "\nAK8DBT_diagUp" << cut2_ak8Dbt[iTab][1].c_str() << ":,";
        for (size_t i = 0; i < cutFlowObject[0].size(); ++i){
            table << std::setprecision(6) << cutFlowObject[4][i].GetNumberOfEventsAfterCuts() << ",";
            if (cutFlowObject[4][i].GetNumberOfEventsAfterCuts() < 0.00000001) table << "0,,";
            else table << cutFlowObject[4][i].GetNumberOfEventsAfterCuts() / holdValueVec[i] << ",,";
            holdValueVec[i] = cutFlowObject[4][i].GetNumberOfEventsAfterCuts();
        }

        table << "\nAll mass regions:,";
        for (size_t i = 0; i < cutFlowObject[0].size(); ++i){
            table << std::setprecision(6) << cutFlowObject[5][i].GetNumberOfEventsAfterCuts() << ",";
            if (cutFlowObject[5][i].GetNumberOfEventsAfterCuts() < 0.00000001) table << "0,,";
            else table << cutFlowObject[5][i].GetNumberOfEventsAfterCuts() / holdValueVec[i] << ",,";
            holdValueVec2[i] = cutFlowObject[5][i].GetNumberOfEventsAfterCuts();
        }


        table << "\n,,";
        for (size_t i = 0; i < cutFlowObject[0].size(); ++i) table << holdValueVec[i] / cutFlowObject[0][i].GetNumberOfEventsBeforeCuts() << ",,,";
        table << "\n,,";
        for (size_t i = 0; i < cutFlowObject[0].size(); ++i) table << holdValueVec2[i] / cutFlowObject[0][i].GetNumberOfEventsBeforeCuts() << ",,,";

        for (size_t iMassRegion = 0; iMassRegion!=numberOfSegments; ++iMassRegion){
            table << "\nMass region " << iMassRegion << " :,";
            for (size_t i = 0; i < cutFlowObject[0].size(); ++i){
                table << std::setprecision(6) << cutFlowObject[6+iMassRegion][i].GetNumberOfEventsAfterCuts() << ",";
                if (cutFlowObject[6+iMassRegion][i].GetNumberOfEventsAfterCuts() < 0.00000001) table << "0,,";
                else table << cutFlowObject[6+iMassRegion][i].GetNumberOfEventsAfterCuts() / holdValueVec[i] << ",,";
                holdValueVec2[i] = cutFlowObject[6+iMassRegion][i].GetNumberOfEventsAfterCuts();
            }
            table << "\n,,";
            for (size_t i = 0; i < cutFlowObject[0].size(); ++i) table << holdValueVec2[i] / cutFlowObject[0][i].GetNumberOfEventsBeforeCuts() << ",,,";
        }

        table.close();
    } // closes loop through different table configurations

	return 0;
}