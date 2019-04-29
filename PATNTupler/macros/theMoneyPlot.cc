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
    const std::string inputFile = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_04_23/fitDiagnostics/mH30_mSusy2800/fitDiagnostics.root";
    const std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_04_23/fitDiagnostics/mH30_mSusy2800/testing01/";
    const double luminosity = 35.867; // 2016 Plots::: NB this is just a label for the plot.
    // const double luminosity = 41.370; // 2017 Plots::: NB this is just a label for the plot.
  

    // TWO: the main settings
    const std::string massType = "S"; // S type mass regions
    // const std::string massType = "UnD"; // U+D type mass regions
    
    // const unsigned int yearOfRun = 2016;
    const unsigned int yearOfRun = 2017;
    
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

    // NB: this is hard coded and dependent on setup
    TH1D * h_QCD = new TH1D("h_QCD", ";Search Region Bin Number;Events", 30, 0, 30);
    TH1D * h_TTJets = new TH1D("h_TTJets", ";Search Region Bin Number;Events", 30, 0, 30);
    TH1D * h_ZJets = new TH1D("h_ZJets", ";Search Region Bin Number;Events", 30, 0, 30);
    TH1D * h_WJets = new TH1D("h_WJets", ";Search Region Bin Number;Events", 30, 0, 30);
    TH1D * h_backgroundError = new TH1D("h_backgroundError", ";Search Region Bin Number;Events", 30, 0, 30);
    TH1D * h_data = new TH1D("h_data", ";Search Region Bin Number;Events", 30, 0, 30);

    unsigned int iBinMin = 1;
    unsigned int iBinMax = 30;

    for (unsigned int iBin=iBinMin; iBin<=iBinMax; iBin++){

        unsigned int iChan = iBin;
        if (yearOfRun == 2017) iChan = iChan + 30;

        std::string directory = "shapes_" + fitType + "/ch" + std::to_string(iChan) + "_mass_" + massType + "/";

        TH1D * h_QCD_dummy = (TH1D*)f->Get(Form("%sQCD", directory.c_str()));
        h_QCD->SetBinContent(iBin, h_QCD_dummy->GetBinContent(1));

        if (yearOfRun==2016){
            TH1D * h_TTJets_dummy = (TH1D*)f->Get(Form("%sTTJets", directory.c_str()));
            if (h_TTJets_dummy != NULL) h_TTJets->SetBinContent(iBin, h_TTJets_dummy->GetBinContent(1));
        }
        if (yearOfRun==2017){
            TH1D * h_TTJets0L_dummy = (TH1D*)f->Get(Form("%sTTJets0L", directory.c_str()));
            TH1D * h_TTJets1L_dummy = (TH1D*)f->Get(Form("%sTTJets1L", directory.c_str()));
            TH1D * h_TTJets2L_dummy = (TH1D*)f->Get(Form("%sTTJets2L", directory.c_str()));
            double totalTTJetsYield = 0.0;
            if (h_TTJets0L_dummy != NULL) totalTTJetsYield += h_TTJets0L_dummy->GetBinContent(1);
            if (h_TTJets1L_dummy != NULL) totalTTJetsYield += h_TTJets1L_dummy->GetBinContent(1);
            if (h_TTJets2L_dummy != NULL) totalTTJetsYield += h_TTJets2L_dummy->GetBinContent(1);
            h_TTJets->SetBinContent(iBin, totalTTJetsYield);
        }

        TH1D * h_ZJets_dummy = (TH1D*)f->Get(Form("%sZJets", directory.c_str()));
        if (h_ZJets_dummy != NULL) h_ZJets->SetBinContent(iBin, h_ZJets_dummy->GetBinContent(1));

        TH1D * h_WJets_dummy = (TH1D*)f->Get(Form("%sWJets", directory.c_str()));
        if (h_WJets_dummy != NULL) h_WJets->SetBinContent(iBin, h_WJets_dummy->GetBinContent(1));

        TH1D * h_backgroundError_dummy = (TH1D*)f->Get(Form("%stotal_background", directory.c_str()));
        h_backgroundError->SetBinContent(iBin, h_backgroundError_dummy->GetBinContent(1));
        h_backgroundError->SetBinError(iBin, h_backgroundError_dummy->GetBinError(1));

        TGraphAsymmErrors * h_data_dummy = (TGraphAsymmErrors*)f->Get(Form("%sdata", directory.c_str()));
        h_data->SetBinContent(iBin, h_data_dummy->Eval(1));
    }

    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.


    // THREE: make plot aesthetics and saving
    std::vector<TH1D*> indiHistoVec = {h_data};
    std::vector<TH1D*> stackHistoVec = {h_WJets, h_ZJets, h_TTJets, h_QCD, h_backgroundError};
    Plotter plot = Plotter(indiHistoVec, stackHistoVec);

    std::vector<std::string> legendNames = {"data", "WJets", "ZJets", "TTJets", "QCD", "unc."};

    // plot.AddLegend(legendNames, 0.16, 0.38, 0.64, 0.83, 0.028);
    // plot.AddLegend(legendNames, 0.18, 0.38, 0.18, 0.30, 0.040);
    // plot.AddLegend(legendNames, 0.67, 0.88, 0.64, 0.83, 0.028);
    // plot.AddLegend(legendNames, 0.67, 0.88, 0.61, 0.80, 0.040); // with ratio box
    plot.AddLegend2Cols(0, legendNames, 0.67, 0.95, 0.55, 0.80, 0.045);
    
    // plot.AddLatex(luminosity);
    plot.AddLatex(luminosity, "#it{Preliminary}");    
    plot.AddRatioBox(0.1, 1.9, "N_{obs} / N_{pred}", true);

    std::vector<std::string> stringVec = {" HT1500-2500 GeV", " HT2500-3500 GeV", " HT3500+ GeV"};
    std::string plotName = "linear";
    plot.SaveSpec02(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);
    
    plotName = "log";
    plot.SetLogY();
    plot.SetYValueMin(0.15); // REMEMBER THIS PARAM! (only for log)
    plot.SaveSpec02(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);


    return 0;
}