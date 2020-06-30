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

#include <TMatrixD.h>
#include <TVectorD.h>

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
    
    const std::string inputFile = "combinedDataCards_ht_XSjmsryear_newZJ_2017as2018_0.98_allSig_ecalfilter_QCDlb0.1tunedubtuned5_bkg10pc_unccorrelated_maxunc2_jmrsymuncor_symall1.00.01_/mH70_mSusy2000/FitDiagnostics_robust/fitDiagnostics.root";

    const std::string outputDir = "MoneyPlot_ecalfilter_mH70_mSusy2000_robustFit_covar";
    // const double luminosity = 35.922; // 2016 Plots::: NB this is just a label for the plot.
    // const double luminosity = 41.529; // 2017 Plots::: NB this is just a label for the plot.
    // const double luminosity = 59.740565202; // 2018 Plots::: NB this is just a label for the plot. It should match the lumi of the histograms!
    const double luminosity = 35.922 + 41.529 + 59.740565202; // WARNING: an approximate combination is performed, adding the fitted errors in quadrature. This is reasonable because the bkg nuisance parameters are not strongly correlated between years.

    // TWO: the main settings
    std::string massType = "S"; // S type mass regions
    // std::string massType = "UnD"; // U+D type mass regions
    // std::string massType = "both"; // WARNING: only to be used for central values (not error bars), as the combination is performed by adding the fitted errors in quadrature. There are non-negligible correlations between the "S" and "UnD" predicted yields.
    
    // unsigned int yearOfRun = 2016;
    // unsigned int yearOfRun = 2017;
    // unsigned int yearOfRun = 2018;
    unsigned int yearOfRun = 0; // WARNING: an approximate combination is performed, adding the fitted errors in quadrature. This is reasonable because the bkg nuisance parameters are not strongly correlated between years.
    
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
    TH1D * h_backgroundVar = new TH1D("h_backgroundVar", ";Search Region Bin Number;Events", 30, 0, 30);
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






int chIndex[2][91] = {0};

chIndex[0][10] = 1;
chIndex[1][10] = 2;
chIndex[0][11] = 3;
chIndex[1][11] = 4;
chIndex[0][12] = 5;
chIndex[1][12] = 6;
chIndex[0][13] = 7;
chIndex[1][13] = 8;
chIndex[0][14] = 9;
chIndex[1][14] = 10;
chIndex[0][15] = 11;
chIndex[1][15] = 12;
chIndex[0][16] = 13;
chIndex[1][16] = 14;
chIndex[0][17] = 15;
chIndex[1][17] = 16;
chIndex[0][18] = 17;
chIndex[1][18] = 18;
chIndex[0][19] = 19;
chIndex[1][19] = 20;
chIndex[0][1] = 21;
chIndex[1][1] = 22;
chIndex[0][20] = 23;
chIndex[1][20] = 24;
chIndex[0][21] = 25;
chIndex[1][21] = 26;
chIndex[0][22] = 27;
chIndex[1][22] = 28;
chIndex[0][23] = 29;
chIndex[1][23] = 30;
chIndex[0][24] = 31;
chIndex[1][24] = 32;
chIndex[0][25] = 33;
chIndex[1][25] = 34;
chIndex[0][26] = 35;
chIndex[1][26] = 36;
chIndex[0][27] = 37;
chIndex[1][27] = 38;
chIndex[0][28] = 39;
chIndex[1][28] = 40;
chIndex[0][29] = 41;
chIndex[1][29] = 42;
chIndex[0][2] = 43;
chIndex[1][2] = 44;
chIndex[0][30] = 45;
chIndex[1][30] = 46;
chIndex[0][31] = 47;
chIndex[1][31] = 48;
chIndex[0][32] = 49;
chIndex[1][32] = 50;
chIndex[0][33] = 51;
chIndex[1][33] = 52;
chIndex[0][34] = 53;
chIndex[1][34] = 54;
chIndex[0][35] = 55;
chIndex[1][35] = 56;
chIndex[0][36] = 57;
chIndex[1][36] = 58;
chIndex[0][37] = 59;
chIndex[1][37] = 60;
chIndex[0][38] = 61;
chIndex[1][38] = 62;
chIndex[0][39] = 63;
chIndex[1][39] = 64;
chIndex[0][3] = 65;
chIndex[1][3] = 66;
chIndex[0][40] = 67;
chIndex[1][40] = 68;
chIndex[0][41] = 69;
chIndex[1][41] = 70;
chIndex[0][42] = 71;
chIndex[1][42] = 72;
chIndex[0][43] = 73;
chIndex[1][43] = 74;
chIndex[0][44] = 75;
chIndex[1][44] = 76;
chIndex[0][45] = 77;
chIndex[1][45] = 78;
chIndex[0][46] = 79;
chIndex[1][46] = 80;
chIndex[0][47] = 81;
chIndex[1][47] = 82;
chIndex[0][48] = 83;
chIndex[1][48] = 84;
chIndex[0][49] = 85;
chIndex[1][49] = 86;
chIndex[0][4] = 87;
chIndex[1][4] = 88;
chIndex[0][50] = 89;
chIndex[1][50] = 90;
chIndex[0][51] = 91;
chIndex[1][51] = 92;
chIndex[0][52] = 93;
chIndex[1][52] = 94;
chIndex[0][53] = 95;
chIndex[1][53] = 96;
chIndex[0][54] = 97;
chIndex[1][54] = 98;
chIndex[0][55] = 99;
chIndex[1][55] = 100;
chIndex[0][56] = 101;
chIndex[1][56] = 102;
chIndex[0][57] = 103;
chIndex[1][57] = 104;
chIndex[0][58] = 105;
chIndex[1][58] = 106;
chIndex[0][59] = 107;
chIndex[1][59] = 108;
chIndex[0][5] = 109;
chIndex[1][5] = 110;
chIndex[0][60] = 111;
chIndex[1][60] = 112;
chIndex[0][61] = 113;
chIndex[1][61] = 114;
chIndex[0][62] = 115;
chIndex[1][62] = 116;
chIndex[0][63] = 117;
chIndex[1][63] = 118;
chIndex[0][64] = 119;
chIndex[1][64] = 120;
chIndex[0][65] = 121;
chIndex[1][65] = 122;
chIndex[0][66] = 123;
chIndex[1][66] = 124;
chIndex[0][67] = 125;
chIndex[1][67] = 126;
chIndex[0][68] = 127;
chIndex[1][68] = 128;
chIndex[0][69] = 129;
chIndex[1][69] = 130;
chIndex[0][6] = 131;
chIndex[1][6] = 132;
chIndex[0][70] = 133;
chIndex[1][70] = 134;
chIndex[0][71] = 135;
chIndex[1][71] = 136;
chIndex[0][72] = 137;
chIndex[1][72] = 138;
chIndex[0][73] = 139;
chIndex[1][73] = 140;
chIndex[0][74] = 141;
chIndex[1][74] = 142;
chIndex[0][75] = 143;
chIndex[1][75] = 144;
chIndex[0][76] = 145;
chIndex[1][76] = 146;
chIndex[0][77] = 147;
chIndex[1][77] = 148;
chIndex[0][78] = 149;
chIndex[1][78] = 150;
chIndex[0][79] = 151;
chIndex[1][79] = 152;
chIndex[0][7] = 153;
chIndex[1][7] = 154;
chIndex[0][80] = 155;
chIndex[1][80] = 156;
chIndex[0][81] = 157;
chIndex[1][81] = 158;
chIndex[0][82] = 159;
chIndex[1][82] = 160;
chIndex[0][83] = 161;
chIndex[1][83] = 162;
chIndex[0][84] = 163;
chIndex[1][84] = 164;
chIndex[0][85] = 165;
chIndex[1][85] = 166;
chIndex[0][86] = 167;
chIndex[1][86] = 168;
chIndex[0][87] = 169;
chIndex[1][87] = 170;
chIndex[0][88] = 171;
chIndex[1][88] = 172;
chIndex[0][89] = 173;
chIndex[1][89] = 174;
chIndex[0][8] = 175;
chIndex[1][8] = 176;
chIndex[0][90] = 177;
chIndex[1][90] = 178;
chIndex[0][9] = 179;
chIndex[1][9] = 180;




    for (unsigned int iM = 0; iM < (massTypes.size() ? massTypes.size() : 1); ++iM) {

    for (unsigned int iY = 0; iY < (yearsOfRun.size() ? yearsOfRun.size() : 1); ++iY)
    {

        if( massTypes.size() ) massType = massTypes.at(iM);

        if( yearsOfRun.size() ) yearOfRun = yearsOfRun.at(iY);

        for (unsigned int iBin=iBinMin; iBin<=iBinMax; iBin++){

            unsigned int iChan = iBin;
            if (yearOfRun == 2017) iChan = iChan + 30;
            if (yearOfRun == 2018) iChan = iChan + 60;


            // set up flag for the bins this bin is being combined with (so we can add the corresponding covariances)
            int binYearIsCombined[91] = {0};
            if( yearsOfRun.size() ) {
                for (int ic = 1; ic <= 90; ++ic) if( ic%30 == iChan%30 ) binYearIsCombined[ic] = 1;
            }
            else binYearIsCombined[iChan] = 1;

            int binIsCombined[2][91] = {0};
            if( massTypes.size() ) {
                for (int ic = 1; ic <= 90; ++ic) {
                    binIsCombined[0][ic] = binYearIsCombined[ic];
                    binIsCombined[1][ic] = binYearIsCombined[ic];
                }
            }
            else for (int ic = 1; ic <= 90; ++ic) binIsCombined[massType=="S"?0:1][ic] = binYearIsCombined[ic];


            std::string directory = "shapes_" + fitType + "/ch" + std::to_string(iChan) + "_mass_" + massType + "/";
            std::string basedirectory = "shapes_" + fitType + "/";

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

            TH1D * h_totalE_dummy = (TH1D*)f->Get(Form("%stotal", directory.c_str()));


            // TODO: make this consistent for prefit and fit_s (where overall_total includes signal)
            // best option for prefit: run fitdiagnostics with bkg only?
            TH2F * h_backgroundCovar_dummy = (TH2F*)f->Get(Form("%soverall_total_covar", basedirectory.c_str()));

            TH1F * h_background_dummy = (TH1F*)f->Get(Form("%stotal_background", basedirectory.c_str()));

            TH1F * h_total_dummy = (TH1F*)f->Get(Form("%stotal_overall", basedirectory.c_str()));

            // std::cout<<sqrt(h_backgroundCovar_dummy->GetBinContent(chIndex[massType=="S"?0:1][iChan],chIndex[massType=="S"?0:1][iChan]))<<" "<<h_background_dummy->GetBinError(chIndex[massType=="S"?0:1][iChan])<<" "<<h_backgroundError_dummy->GetBinError(1)<<std::endl;

            double bgE = h_backgroundError_dummy->GetBinError(1);
            std::cout<<"bgE: "<<chIndex[massType=="S"?0:1][iChan]<<" "<<sqrt(h_backgroundCovar_dummy->GetBinContent(chIndex[massType=="S"?0:1][iChan],chIndex[massType=="S"?0:1][iChan])) / bgE - 1. <<" "<<h_background_dummy->GetBinError(chIndex[massType=="S"?0:1][iChan]) / bgE - 1. <<std::endl;


            double totE = h_totalE_dummy->GetBinError(1);
            std::cout<<"totE: "<<chIndex[massType=="S"?0:1][iChan]<<" "<<sqrt(h_backgroundCovar_dummy->GetBinContent(chIndex[massType=="S"?0:1][iChan],chIndex[massType=="S"?0:1][iChan])) / totE - 1. <<" "<<h_total_dummy->GetBinError(chIndex[massType=="S"?0:1][iChan]) / totE - 1. <<std::endl;

            std::cout<<"    "<<bgE<<" "<<totE<<std::endl;




            for (int imt = 0; imt <= 1; ++imt) for (int ic = 1; ic <= 90; ++ic) {
                if( binIsCombined[imt][ic] ) h_backgroundVar->AddBinContent(iBin,  h_backgroundCovar_dummy->GetBinContent(chIndex[massType=="S"?0:1][iChan],chIndex[imt][ic])  );
            }

            //TODO: manually check cov calc for one bin

            // ignoring covariance terms
            // // h_backgroundVar->AddBinContent(iBin,  h_backgroundCovar_dummy->GetBinContent(chIndex[massType=="S"?0:1][iChan],chIndex[massType=="S"?0:1][iChan])  );
            // for (int imt = 0; imt <= 1; ++imt) for (int ic = 1; ic <= 90; ++ic) {
            //     if( binIsCombined[imt][ic] ) if( iChan == ic && (massType=="S"?0:1) == imt ) h_backgroundVar->AddBinContent(iBin,  h_backgroundCovar_dummy->GetBinContent(chIndex[massType=="S"?0:1][iChan],chIndex[imt][ic])  );
            // }

            /*
            const int nbins = h_backgroundCovar_dummy->GetNbinsX();
            TMatrixD covarianceM(nbins, nbins);

            for(int i=0;i<nbins;i++)   for(int j=0;j<nbins;j++)  {
                covarianceM(i,j) = h_backgroundCovar_dummy->GetBinContent(i+1,j+1);
            }

            covarianceM.NormByDiag(TMatrixDDiag(covarianceM));

            TH2D* h_backgroundCorr = new TH2D(("corr"+std::to_string(chIndex[massType=="S"?0:1][iChan])).c_str(), ("corr"+std::to_string(chIndex[massType=="S"?0:1][iChan])).c_str(), nbins, 0, nbins, nbins, 0, nbins);

            for(int i=0;i<nbins;i++)   for(int j=0;j<nbins;j++)  {
                h_backgroundCorr->SetBinContent(i+1,j+1,covarianceM(i,j));
            }
            */


            TGraphAsymmErrors * h_data_dummy = (TGraphAsymmErrors*)f->Get(Form("%sdata", directory.c_str()));
            h_data->AddBinContent(iBin, h_data_dummy->Eval(1));
        }

    } //iY

    } //iM

    // for (unsigned int iBin=iBinMin; iBin<=iBinMax; iBin++) std::cout<<h_backgroundVar->GetBinContent(iBin)<<" "<<h_backgroundError->GetBinError(iBin)*h_backgroundError->GetBinError(iBin)  <<" "<<h_backgroundVar->GetBinContent(iBin) /  h_backgroundError->GetBinError(iBin)/h_backgroundError->GetBinError(iBin) - 1. <<std::endl;

    for (unsigned int iBin=iBinMin; iBin<=iBinMax; iBin++) h_backgroundVar->SetBinContent(iBin, sqrt( h_backgroundVar->GetBinContent(iBin) ) );

    for (unsigned int iBin=iBinMin; iBin<=iBinMax; iBin++) std::cout<<h_backgroundVar->GetBinContent(iBin)<<" "<<h_backgroundError->GetBinError(iBin) <<" "<<h_backgroundVar->GetBinContent(iBin) /  h_backgroundError->GetBinError(iBin) - 1. <<std::endl;

    for (unsigned int iBin=iBinMin; iBin<=iBinMax; iBin++) h_backgroundError->SetBinError(iBin, h_backgroundVar->GetBinContent(iBin) );

    //TODO: plot ratio of pre/post fit QCD (and other bkg)

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
