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
#include "PlotEntry2D.hh"
#include "Plotter.hh"
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"
#include "MacrosOnCondor.h"

// COMPARE VARIABLES AS YOU LOOP THROUGH CUTS (in 2D)

int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    // std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2018_08_03/2016_80X/2d_doubleBTag/mH70_mSusy2000/fullKinCuts/"; // where we are going to save the output plots (should include the samples name, and any important features)
    std::string outputDir = "plotting_compare_2D_normalised";

    // TWO: set of cut params, each combination = new plot
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Loose","Max","Loose","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"DIAG_UP", "Loose"} }; // Top Diagnol Corner Crossing Axis at...--> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    std::vector<int> cut3_ak8Pt = {300};
    // std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {2500,3500}, {3500,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    std::vector<std::vector<int>> cut4_ht = { {1500,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {300,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)



    // THREE: histogram settings
    // double luminosity = 35.867; // 2016 DATASET
    double luminosity = 41.370; // 2017 DATASET

    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA SoftDropMass (GeV);fatJetB SoftDropMass (GeV)", 400, 0, 205, 400, 0, 205);
    TH2D hTemplate = TH2D("hTemplate", ";fatJetA Mass (GeV);fatJetB Mass (GeV)", 215, -0.005508812, 205.190359608, 215, -0.005508812, 205.190359608);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA Mass (GeV);fatJetB Mass (GeV)", 72, -0.9599082, 205.190359608, 72, -0.9599082, 205.190359608);
    std::string varXAxis = "fatJetA_softDropMassPuppi";
    std::string varYAxis = "fatJetB_softDropMassPuppi";
    // std::string varXAxis = "fatJetA_softDropMassPuppi_jmrUncUp";
    // std::string varYAxis = "fatJetB_softDropMassPuppi_jmrUncUp";

    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA doubleBtagDiscriminator;fatJetB doubleBtagDiscriminator", 1, 0.8, 1.0, 1, 0.6, 1.0); // x
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA doubleBtagDiscriminator;fatJetB doubleBtagDiscriminator", 1, 0.8, 1.0, 1, 0.3, 0.6); // y
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA doubleBtagDiscriminator;fatJetB doubleBtagDiscriminator", 1, 0.8, 1.0, 1, 0.3, 1.0); // z
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA doubleBtagDiscriminator;fatJetB doubleBtagDiscriminator", 1, 0.8, 1.0, 7, 0.3, 1.0); // all
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA doubleBtagDiscriminator;fatJetB doubleBtagDiscriminator", 400, -1.0, 1.0, 400, -1.0, 1.0); // NORMAL
    // std::string varXAxis = "fatJetA_doubleBtagDiscrim";
    // std::string varYAxis = "fatJetB_doubleBtagDiscrim";

    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA doubleBtagDiscriminator;fatJetA p_{T} (GeV)", 400, -1.0, 1.0, 400, 0, 3000.0);
    // std::string varXAxis = "fatJetA_doubleBtagDiscrim";
    // std::string varYAxis = "fatJetA_p4.Pt()";

    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA p_{T} (GeV);fatJetB p_{T} (GeV)", 100, 0, 1000, 100, 0, 1000);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA p_{T} (GeV);fatJetB p_{T} (GeV)", 300, 0, 3000, 300, 0, 3000);
    // std::string varXAxis = "fatJetA_p4.Pt()";
    // std::string varYAxis = "fatJetB_p4.Pt()";

    // TH2D hTemplate = TH2D("hTemplate", ";leadingAK4Jet p_{T} (GeV);secondaryAK4Jet p_{T} (GeV)", 300, 0, 3000, 300, 0, 3000);
    // std::string varXAxis = "slimJetA_p4.Pt()";
    // std::string varYAxis = "slimJetB_p4.Pt()";

    // TH2D hTemplate = TH2D("hTemplate", ";leadingAK4Jet p_{T} (GeV);fatJetA p_{T} (GeV)", 300, 0, 3000, 300, 0, 3000);
    // std::string varXAxis = "slimJetA_p4.Pt()";
    // std::string varYAxis = "fatJetA_p4.Pt()";

    // TH2D hTemplate = TH2D("hTemplate", ";secondaryAK4Jet p_{T} (GeV);fatJetA p_{T} (GeV)", 300, 0, 3000, 300, 0, 3000);
    // std::string varXAxis = "slimJetB_p4.Pt()";
    // std::string varYAxis = "fatJetA_p4.Pt()";

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
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_compare2D.cc %s/%s__plotting_compare2D.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_compare2D.cc %s/%s__plotting_compare2D.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "plotting_compare2D", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
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
    std::string varToPlot = varYAxis + ":" + varXAxis;
    std::string varToPlotSaveName = varYAxis + "VS" + varXAxis;
    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::string dbtCut = "";
                    if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP")
                        dbtCut = Form("fatJetA_doubleBtagDiscrim >= (-1.0 * fatJetB_doubleBtagDiscrim + 1.0 + %f) ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]) );
                    if (cut2_ak8Dbt[iCut2].size() == 4)
                        dbtCut = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]) );
                    std::string cutToApply = Form("%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", dbtCut.c_str(), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);


             

      
                    // FOUR: samples to use

                    // PlotEntry2D plot2d = PlotEntry2D("data", hTemplate, varToPlot.c_str()); // NO LUMI
                    // varToPlotSaveName = "data" + varToPlotSaveName;
                    // plot2d.AddInput("/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/JetHT_Run2017TOTAL/flatTree.root", cutToApply.c_str());
                    // plot2d.NormalisePlot();

                    // PlotEntry2D plot2d = PlotEntry2D("WJets", hTemplate, varToPlot.c_str(), luminosity);
                    // varToPlotSaveName = "WJets" + varToPlotSaveName;
                    // plot2d.AddInput("/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/WJets_ht1200plus/flatTree.root", cutToApply.c_str(), 34.00, "weight_combined");
                    // plot2d.NormalisePlot();

                    // PlotEntry2D plot2d = PlotEntry2D("ZJets", hTemplate, varToPlot.c_str(), luminosity);
                    // varToPlotSaveName = "ZJets" + varToPlotSaveName;
                    // plot2d.AddInput("/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/ZJets_ht1200plus/flatTree.root", cutToApply.c_str(), 18.69, "weight_combined");
                    // plot2d.NormalisePlot();

                    // double PDGhadBF = 67.41/100.;
                    // PlotEntry2D plot2d = PlotEntry2D("TTJets_ALL", hTemplate, varToPlot.c_str(), luminosity);
                    // varToPlotSaveName = "ttbar" + varToPlotSaveName;
                    // plot2d.AddInput("/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/TTToHadronic_ht1200plus/flatTree.root", cutToApply.c_str(), 831.76*PDGhadBF*PDGhadBF, "weight_combined");
                    // plot2d.AddInput("/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/TTToSemiLeptonic_ht1200plus/flatTree.root", cutToApply.c_str(), 831.76*PDGhadBF*(1.-PDGhadBF)*2., "weight_combined");
                    // plot2d.AddInput("/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/TTTo2L2Nu_ht1200plus/flatTree.root", cutToApply.c_str(), 831.76*(1.-PDGhadBF)*(1.-PDGhadBF), "weight_combined");
                    // plot2d.NormalisePlot();

                    // PlotEntry2D plot2d = PlotEntry2D("Multijet", hTemplate, varToPlot.c_str(), luminosity);
                    // varToPlotSaveName = "Multijet" + varToPlotSaveName;
                    // plot2d.AddInput("/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/QCD_HT1000to1500_ht1499plus/flatTree.root", cutToApply.c_str(), 1005, "weight_combined");
                    // plot2d.AddInput("/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/QCD_HT1500to2000_ht1499plus/flatTree.root", cutToApply.c_str(), 101.8, "weight_combined");
                    // plot2d.AddInput("/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/QCD_HT2000toInf_ht1499plus/flatTree.root", cutToApply.c_str(), 20.54, "weight_combined");
                    // plot2d.NormalisePlot();

                    PlotEntry2D plot2d = PlotEntry2D("mH125_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    varToPlotSaveName = "mH125_mSusy2000" + varToPlotSaveName;
                    plot2d.AddInput("/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2019_01_01/2017/mH125p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 1, "weight_combined");
                    plot2d.NormalisePlot();


                    // FIVE: Mass Regions Object (if you want to superimpose on plot)
                    double S1_Node1 = 40.0;
                    double S1_Node2 = 17.6;
                    double SMAX_Node1 = 170.7;
                    double SMAX_Node2 = 113.7;
                    std::vector<double> SN_Nodes = {51.9, 65.1, 78.3, 91.5, 104.7, 117.9, 131.1, 144.3, 157.5};
                    MassRegionCuts MassCutsObject = MassRegionCuts("MassCutsV09", S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2, SN_Nodes);

                    // double S1_Node1 = 195.0;
                    // double S1_Node2 = 100.0;
                    // double SMAX_Node1 = 201.0;
                    // double SMAX_Node2 = 170.0;
                    // std::vector<double> SN_Nodes = {197.0, 199.0};
                    // double sideBandScaleFactor = 1.0;
                    // MassRegionCuts MassCutsObject = MassRegionCuts("MassCutsSpecialV06", S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2, SN_Nodes, "fatJetA_softDropMassPuppi", "fatJetB_softDropMassPuppi", sideBandScaleFactor);




                    // SIX: plot aesthetics
                    Plotter plot = Plotter({plot2d});
                    plot.AddLatex();
                    // plot.AddLatex(luminosity);
                    // plot.AddLatex(luminosity, "#it{Preliminary}"); // DATA
                    std::string saveName = varToPlotSaveName;
                    if (cut2_ak8Dbt[iCut2].size() == 4) saveName += "__dbt" + cut2_ak8Dbt[iCut2][0] + cut2_ak8Dbt[iCut2][1] + "And" + cut2_ak8Dbt[iCut2][2] + cut2_ak8Dbt[iCut2][3];
                    if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP") saveName += "__dbtDiagUp" + cut2_ak8Dbt[iCut2][1];
                    saveName += Form("_ak8pt%d", cut3_ak8Pt[iCut3]);
                    if (cut4_ht[iCut4][1]==99999) saveName += Form("_ht%dplus",cut4_ht[iCut4][0]);
                    else saveName += Form("_ht%dto%d",cut4_ht[iCut4][0],cut4_ht[iCut4][1]);
                    saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    // saveName += "APPEND_SAVE_NAME"; // OPTIONAL: can manually append the automatic savename
                    // saveName = "MANUAL_SAVE_NAME"; // OPTIONAL: can manually overwrite the automatic savename
                    std::string outputFile = outputDir + "/" + saveName + ".pdf";
                    plot.Save2D(outputFile.c_str());
                    outputFile = outputDir + "/" + saveName + "_withGrid.pdf";
                    plot.Save2D(outputFile.c_str(), MassCutsObject); // if want mass grids on plot
                    plot.SetLogZ(); // toggle ON or OFF
                    outputFile = outputDir + "/" + saveName + "_logZ.pdf";
                    plot.Save2D(outputFile.c_str());
                    outputFile = outputDir + "/" + saveName + "_withGrid_logZ.pdf";
                    plot.Save2D(outputFile.c_str(), MassCutsObject); // if want mass grids on plot





                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states

    return 0;
}