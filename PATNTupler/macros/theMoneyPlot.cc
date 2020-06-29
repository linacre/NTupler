//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <map>
#include <fstream>
#include <sys/stat.h>

//ROOT HEADERS
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphAsymmErrors.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TFile.h>

//RAL PARTICLE HEADERS
#include "PlotEntry2D.hh"
#include "Plotter.hh"
#include "MassRegionCuts.hh"
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"
#include "QcdSidebandCorr.h"

// MAKES PLOTS USING COMBINED OUTPUT

int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////


    // ONE: input file name, save info, and lumi
    //const std::string inputFile = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_04_23/fitDiagnostics/mH30_mSusy2800/fitDiagnostics.root";
    // const std::string inputFile = "/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/combinedDataCards_20200120/combinedDataCards_final_2017as2018_0.98_allSigs/fitDiagnostics/mH30_mSusy2800_Joe_old/fitDiagnostics.root";
    //const std::string inputFile = "/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/combinedDataCards_20200120/combinedDataCards_final_201617_allSig/fitDiagnostics/mH70_mSusy2800/fitDiagnostics.root";
    //const std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_04_23/fitDiagnostics/mH30_mSusy2800/testing03/";
    
    const std::string inputFile = "combinedDataCards_ht_XSjmsryear_newZJ_2017as2018_0.98_allSig_ecalfilter_QCDlb0.1tunedubtuned5_bkg10pc_unccorrelated_maxunc2_jmrsymuncor_symall1.00.01_/mH70_mSusy2800/fitDiagnostics.root";

    const std::string outputDir = "MoneyPlot";
    // const double luminosity = 35.922; // 2016 Plots::: NB this is just a label for the plot.
    // const double luminosity = 41.529; // 2017 Plots::: NB this is just a label for the plot.
    const double luminosity = 59.740565202; // 2018 Plots::: NB this is just a label for the plot. It should match the lumi of the histograms!
    // const double luminosity = 35.922 + 41.529 + 59.740565202; // WARNING: an approximate combination is performed, adding the fitted errors in quadrature. This is reasonable because the bkg nuisance parameters are not strongly correlated between years.

    // TWO: the main settings
    std::string massType = "S"; // S type mass regions
    // std::string massType = "UnD"; // U+D type mass regions
    // std::string massType = "both"; // WARNING: only to be used for central values (not error bars), as the combination is performed by adding the fitted errors in quadrature. There are non-negligible correlations between the "S" and "UnD" predicted yields.
    
    // unsigned int yearOfRun = 2016;
    // unsigned int yearOfRun = 2017;
    unsigned int yearOfRun = 2018;
    // unsigned int yearOfRun = 0; // WARNING: an approximate combination is performed, adding the fitted errors in quadrature. This is reasonable because the bkg nuisance parameters are not strongly correlated between years.
    
    // const std::string fitType = "prefit";
    const std::string fitType = "fit_b";
    // const std::string fitType = "fit_s";


    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/theMoneyPlot.cc %s/%s__theMoneyPlot.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.

    TFile * f = new TFile(inputFile.c_str());

    // TH1::SetDefaultSumw2();

    // NB: this is hard coded and dependent on setup
    TH1D * h_QCD = new TH1D("h_QCD", ";Search Region Bin Number;Events", 30, 0, 30);
    TH1D * h_TTJets = new TH1D("h_TTJets", ";Search Region Bin Number;Events", 30, 0, 30);
    TH1D * h_ZJets = new TH1D("h_ZJets", ";Search Region Bin Number;Events", 30, 0, 30);
    TH1D * h_WJets = new TH1D("h_WJets", ";Search Region Bin Number;Events", 30, 0, 30);
    TH1D * h_backgroundError = new TH1D("h_backgroundError", ";Search Region Bin Number;Events", 30, 0, 30);
    TH1D * h_data = new TH1D("h_data", ";Search Region Bin Number;Events", 30, 0, 30);

    h_backgroundError->Sumw2(); // to avoid bug with AddBinContent adding an error for the first bin

    unsigned int iBinMin = 1;
    unsigned int iBinMax = 30;


    std::vector<std::string> massTypes;
    if(massType == "both") {
        massTypes.push_back("S");
        massTypes.push_back("UnD");
    }

    std::vector<int> yearsOfRun;
    if(yearOfRun == 0) {
        yearsOfRun.push_back(2016);
        yearsOfRun.push_back(2017);
        yearsOfRun.push_back(2018);
    }

    for (unsigned int iM = 0; iM < (massTypes.size() ? massTypes.size() : 1); ++iM) {

    for (unsigned int iY = 0; iY < (yearsOfRun.size() ? yearsOfRun.size() : 1); ++iY)
    {

        if( massTypes.size() ) massType = massTypes.at(iM);

        if( yearsOfRun.size() ) yearOfRun = yearsOfRun.at(iY);

        for (unsigned int iBin=iBinMin; iBin<=iBinMax; iBin++){

            unsigned int iChan = iBin;
            if (yearOfRun == 2017) iChan = iChan + 30;
            if (yearOfRun == 2018) iChan = iChan + 60;

            std::string directory = "shapes_" + fitType + "/ch" + std::to_string(iChan) + "_mass_" + massType + "/";

            TH1D * h_QCD_dummy = (TH1D*)f->Get(Form("%sQCD", directory.c_str()));
            h_QCD->AddBinContent(iBin, h_QCD_dummy->GetBinContent(1));

            if (yearOfRun==2016){
                TH1D * h_TTJets_dummy = (TH1D*)f->Get(Form("%sTTJets", directory.c_str()));
                if (h_TTJets_dummy != NULL) h_TTJets->AddBinContent(iBin, h_TTJets_dummy->GetBinContent(1));
            }
            if (yearOfRun==2017){
                TH1D * h_TTJets0L_dummy = (TH1D*)f->Get(Form("%sTTJets0L", directory.c_str()));
                TH1D * h_TTJets1L_dummy = (TH1D*)f->Get(Form("%sTTJets1L", directory.c_str()));
                TH1D * h_TTJets2L_dummy = (TH1D*)f->Get(Form("%sTTJets2L", directory.c_str()));
                double totalTTJetsYield = 0.0;
                if (h_TTJets0L_dummy != NULL) totalTTJetsYield += h_TTJets0L_dummy->GetBinContent(1);
                if (h_TTJets1L_dummy != NULL) totalTTJetsYield += h_TTJets1L_dummy->GetBinContent(1);
                if (h_TTJets2L_dummy != NULL) totalTTJetsYield += h_TTJets2L_dummy->GetBinContent(1);
                h_TTJets->AddBinContent(iBin, totalTTJetsYield);
            }
            if (yearOfRun==2018){
                TH1D * h_TTJets0L_dummy = (TH1D*)f->Get(Form("%sTTJets0L", directory.c_str()));
                TH1D * h_TTJets1L_dummy = (TH1D*)f->Get(Form("%sTTJets1L", directory.c_str()));
                TH1D * h_TTJets2L_dummy = (TH1D*)f->Get(Form("%sTTJets2L", directory.c_str()));
                double totalTTJetsYield = 0.0;
                if (h_TTJets0L_dummy != NULL) totalTTJetsYield += h_TTJets0L_dummy->GetBinContent(1);
                if (h_TTJets1L_dummy != NULL) totalTTJetsYield += h_TTJets1L_dummy->GetBinContent(1);
                if (h_TTJets2L_dummy != NULL) totalTTJetsYield += h_TTJets2L_dummy->GetBinContent(1);
                h_TTJets->AddBinContent(iBin, totalTTJetsYield);
            }

            TH1D * h_ZJets_dummy = (TH1D*)f->Get(Form("%sZJets", directory.c_str()));
            if (h_ZJets_dummy != NULL) h_ZJets->AddBinContent(iBin, h_ZJets_dummy->GetBinContent(1));

            TH1D * h_WJets_dummy = (TH1D*)f->Get(Form("%sWJets", directory.c_str()));
            if (h_WJets_dummy != NULL) h_WJets->AddBinContent(iBin, h_WJets_dummy->GetBinContent(1));

            TH1D * h_backgroundError_dummy = (TH1D*)f->Get(Form("%stotal_background", directory.c_str()));
            h_backgroundError->AddBinContent(iBin, h_backgroundError_dummy->GetBinContent(1));
            h_backgroundError->SetBinError(iBin, sqrt( h_backgroundError->GetBinError(iBin)*h_backgroundError->GetBinError(iBin) + h_backgroundError_dummy->GetBinError(1)*h_backgroundError_dummy->GetBinError(1) ) );

            TGraphAsymmErrors * h_data_dummy = (TGraphAsymmErrors*)f->Get(Form("%sdata", directory.c_str()));
            h_data->AddBinContent(iBin, h_data_dummy->Eval(1));
        }

    } //iY

    } //iM

    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.


    // THREE: make plot aesthetics and saving
    std::vector<TH1D*> indiHistoVec = {h_data};
    std::vector<TH1D*> stackHistoVec = {h_WJets, h_ZJets, h_TTJets, h_QCD, h_backgroundError};
    Plotter plot = Plotter(indiHistoVec, stackHistoVec);

    std::vector<std::string> legendNames = {"data", "WJets", "ZJets", "TTJets", "QCD", "unc."};
    plot.AddLegend2Cols(0, legendNames, 0.67, 0.95, 0.55, 0.80, 0.045);
    
    // plot.AddLatex(luminosity);
    plot.AddLatex(luminosity, "#it{Preliminary}");    
    plot.AddRatioBox(0.0, 2.7, "data / pred", true);

    std::vector<std::string> stringVec = {" HT1500-2500 GeV", " HT2500-3500 GeV", " HT3500+ GeV"};
    std::string plotName = "linear_"+( yearsOfRun.size() ? "combined" : std::to_string(yearOfRun) )+"_"+( massTypes.size() ? "both" : massType )+"_"+fitType;
    plot.SaveSpec02(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);
    
    plotName = "log_"+( yearsOfRun.size() ? "combined" : std::to_string(yearOfRun) )+"_"+( massTypes.size() ? "both" : massType )+"_"+fitType;
    plot.SetLogY();
    plot.SetYValueMin(0.13); // REMEMBER THIS PARAM! (only for log)
    plot.SaveSpec02(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);

    return 0;
}