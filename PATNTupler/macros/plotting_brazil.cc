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
#include <TTree.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TFile.h>

//RAL PARTICLE HEADERS
#include "Plotter.hh"
#include "TimeStamp.h"



std::map<std::string, double> xsecBR{

{"mH30_mSusy800",  6.4720000*0.868*0.868},
{"mH50_mSusy800",  6.4720000*0.858*0.858},
{"mH70_mSusy800",  6.4720000*0.840*0.840},
{"mH90_mSusy800",  6.4720000*0.816*0.816},
{"mH110_mSusy800", 6.4720000*0.749*0.749},
{"mH125_mSusy800", 6.4720000*0.581*0.581},

{"mH30_mSusy1200", 0.4951000*0.868*0.868},
{"mH35_mSusy1200", 0.4951000*0.867*0.867},
{"mH40_mSusy1200", 0.4951000*0.865*0.865},
{"mH50_mSusy1200", 0.4951000*0.858*0.858},
{"mH70_mSusy1200", 0.4951000*0.840*0.840},
{"mH90_mSusy1200", 0.4951000*0.816*0.816},
{"mH110_mSusy1200",0.4951000*0.749*0.749},
{"mH125_mSusy1200",0.4951000*0.581*0.581},

{"mH30_mSusy1600", 0.0603900*0.868*0.868},
{"mH35_mSusy1600", 0.0603900*0.867*0.867},
{"mH40_mSusy1600", 0.0603900*0.865*0.865},
{"mH50_mSusy1600", 0.0603900*0.858*0.858},
{"mH70_mSusy1600", 0.0603900*0.840*0.840},
{"mH90_mSusy1600", 0.0603900*0.816*0.816},
{"mH110_mSusy1600",0.0603900*0.749*0.749},
{"mH125_mSusy1600",0.0603900*0.581*0.581},

{"mH30_mSusy2000", 0.0091050*0.868*0.868},
{"mH35_mSusy2000", 0.0091050*0.867*0.867},
{"mH40_mSusy2000", 0.0091050*0.865*0.865},
{"mH50_mSusy2000", 0.0091050*0.858*0.858},
{"mH70_mSusy2000", 0.0091050*0.840*0.840},
{"mH90_mSusy2000", 0.0091050*0.816*0.816},
{"mH110_mSusy2000",0.0091050*0.749*0.749},
{"mH125_mSusy2000",0.0091050*0.581*0.581},

{"mH30_mSusy2200", 0.0036780*0.868*0.868},
{"mH35_mSusy2200", 0.0036780*0.867*0.867},
{"mH40_mSusy2200", 0.0036780*0.865*0.865},
{"mH50_mSusy2200", 0.0036780*0.858*0.858},
{"mH70_mSusy2200", 0.0036780*0.840*0.840},
{"mH90_mSusy2200", 0.0036780*0.816*0.816},
{"mH110_mSusy2200",0.0036780*0.749*0.749},
{"mH125_mSusy2200",0.0036780*0.581*0.581},

{"mH30_mSusy2400", 0.0015050*0.868*0.868},
{"mH35_mSusy2400", 0.0015050*0.867*0.867},
{"mH40_mSusy2400", 0.0015050*0.865*0.865},
{"mH50_mSusy2400", 0.0015050*0.858*0.858},
{"mH70_mSusy2400", 0.0015050*0.840*0.840},
{"mH90_mSusy2400", 0.0015050*0.816*0.816},
{"mH110_mSusy2400",0.0015050*0.749*0.749},
{"mH125_mSusy2400",0.0015050*0.581*0.581},

{"mH30_mSusy2600", 0.0006167*0.868*0.868},
{"mH35_mSusy2600", 0.0006167*0.867*0.867},
{"mH40_mSusy2600", 0.0006167*0.865*0.865},
{"mH50_mSusy2600", 0.0006167*0.858*0.858},
{"mH70_mSusy2600", 0.0006167*0.840*0.840},
{"mH90_mSusy2600", 0.0006167*0.816*0.816},
{"mH110_mSusy2600",0.0006167*0.749*0.749},
{"mH125_mSusy2600",0.0006167*0.581*0.581},

{"mH30_mSusy2800", 0.0002753*0.868*0.868},
{"mH35_mSusy2800", 0.0002753*0.867*0.867},
{"mH40_mSusy2800", 0.0002753*0.865*0.865},
{"mH50_mSusy2800", 0.0002753*0.858*0.858},
{"mH70_mSusy2800", 0.0002753*0.840*0.840},
{"mH90_mSusy2800", 0.0002753*0.816*0.816},
{"mH110_mSusy2800",0.0002753*0.749*0.749},
{"mH125_mSusy2800",0.0002753*0.581*0.581},

{"mH30_mSquark800",  2.0480000*0.868*0.868},
{"mH50_mSquark800",  2.0480000*0.858*0.858},
{"mH70_mSquark800",  2.0480000*0.840*0.840},
{"mH90_mSquark800",  2.0480000*0.816*0.816},
{"mH110_mSquark800", 2.0480000*0.749*0.749},
{"mH125_mSquark800", 2.0480000*0.581*0.581},

{"mH30_mSquark1200", 0.2039000*0.868*0.868},
{"mH35_mSquark1200", 0.2039000*0.867*0.867},
{"mH40_mSquark1200", 0.2039000*0.865*0.865},
{"mH50_mSquark1200", 0.2039000*0.858*0.858},
{"mH70_mSquark1200", 0.2039000*0.840*0.840},
{"mH90_mSquark1200", 0.2039000*0.816*0.816},
{"mH110_mSquark1200",0.2039000*0.749*0.749},
{"mH125_mSquark1200",0.2039000*0.581*0.581},

{"mH30_mSquark1600", 0.0296800*0.868*0.868},
{"mH35_mSquark1600", 0.0296800*0.867*0.867},
{"mH40_mSquark1600", 0.0296800*0.865*0.865},
{"mH50_mSquark1600", 0.0296800*0.858*0.858},
{"mH70_mSquark1600", 0.0296800*0.840*0.840},
{"mH90_mSquark1600", 0.0296800*0.816*0.816},
{"mH110_mSquark1600",0.0296800*0.749*0.749},
{"mH125_mSquark1600",0.0296800*0.581*0.581},

{"mH30_mSquark2000", 0.0050450*0.868*0.868},
{"mH35_mSquark2000", 0.0050450*0.867*0.867},
{"mH40_mSquark2000", 0.0050450*0.865*0.865},
{"mH50_mSquark2000", 0.0050450*0.858*0.858},
{"mH70_mSquark2000", 0.0050450*0.840*0.840},
{"mH90_mSquark2000", 0.0050450*0.816*0.816},
{"mH110_mSquark2000",0.0050450*0.749*0.749},
{"mH125_mSquark2000",0.0050450*0.581*0.581},

{"mH30_mSquark2200", 0.0021320*0.868*0.868},
{"mH35_mSquark2200", 0.0021320*0.867*0.867},
{"mH40_mSquark2200", 0.0021320*0.865*0.865},
{"mH50_mSquark2200", 0.0021320*0.858*0.858},
{"mH70_mSquark2200", 0.0021320*0.840*0.840},
{"mH90_mSquark2200", 0.0021320*0.816*0.816},
{"mH110_mSquark2200",0.0021320*0.749*0.749},
{"mH125_mSquark2200",0.0021320*0.581*0.581},

{"mH30_mSquark2400", 0.0009076*0.868*0.868},
{"mH35_mSquark2400", 0.0009076*0.867*0.867},
{"mH40_mSquark2400", 0.0009076*0.865*0.865},
{"mH50_mSquark2400", 0.0009076*0.858*0.858},
{"mH70_mSquark2400", 0.0009076*0.840*0.840},
{"mH90_mSquark2400", 0.0009076*0.816*0.816},
{"mH110_mSquark2400",0.0009076*0.749*0.749},
{"mH125_mSquark2400",0.0009076*0.581*0.581},

{"mH30_mSquark2600", 0.0003843*0.868*0.868},
{"mH35_mSquark2600", 0.0003843*0.867*0.867},
{"mH40_mSquark2600", 0.0003843*0.865*0.865},
{"mH50_mSquark2600", 0.0003843*0.858*0.858},
{"mH70_mSquark2600", 0.0003843*0.840*0.840},
{"mH90_mSquark2600", 0.0003843*0.816*0.816},
{"mH110_mSquark2600",0.0003843*0.749*0.749},
{"mH125_mSquark2600",0.0003843*0.581*0.581},

{"mH30_mSquark2800", 0.0001849*0.868*0.868},
{"mH35_mSquark2800", 0.0001849*0.867*0.867},
{"mH40_mSquark2800", 0.0001849*0.865*0.865},
{"mH50_mSquark2800", 0.0001849*0.858*0.858},
{"mH70_mSquark2800", 0.0001849*0.840*0.840},
{"mH90_mSquark2800", 0.0001849*0.816*0.816},
{"mH110_mSquark2800",0.0001849*0.749*0.749},
{"mH125_mSquark2800",0.0001849*0.581*0.581}

};




// MAKES BRAZIL PLOTS

int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    // ONE: input directory (where the combined root files are)
    // const std::string inputDir = "/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/combinedDataCards_20200527/combinedDataCards_ht_XSjmsryear_newZJ_2017as2018_0.98_allSig_ecalfilter_QCDlb0.1tunedubtuned5_bkg10pc_unccorrelated_maxunc2_jmrsymuncor_symall1.00.01";
    
    // const std::string inputDir = "/mercury/data2/linacre/NMSSM/Analysis_boostedNmssmHiggs/combinedDataCards_20200527/combinedDataCards_ht_XSjmsryear_newZJ_2017as2018_0.98_allSig_ecalfilter_QCDlb0.1tunedubtuned5_bkg10pc_unccorrelated_maxunc2_jmrsymuncor_symall1.00.01";
    const std::string inputDir = "/opt/ppd/scratch-2021/xxt18833/Analysis_boostedNmssmHiggs/combinedDataCards_20201213/combinedDataCards_ht_XSjmsryear_newZJ_2017as2018sqfix_0.98_allSig_ecalfilter_QCDlb0.1tunedubtuned5_bkg10pc_unccorrelated_maxunc2_jmrsymuncor_symall1.00.01";
    // const std::string inputDir = "/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/combinedDataCards_20200120/combinedDataCards_final_2018_2017as2018_0.98_allSigs";
    // const std::string inputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_01_01/withGluino/allSys/";

    // TWO: plot output directory
    std::string outputDir = "brazilplots_blinded_fixedsq_xsec";
    // const std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2019_01_01/brazilplots/mSusy2400/";

    // THREE: higgs and SUSY masses (one of which should have a single entry)
    const std::vector<int> higgsMasses = {30, 35, 40, 50, 70, 90, 110, 125};
    //const std::vector<int> susyMasses = {2600};
    // const std::vector<int> higgsMasses = {90};
    // const std::vector<int> susyMasses = {1200, 1600, 2000, 2200, 2400, 2600, 2800};

    const std::vector<int> susyMasses = {1200, 1600, 2000, 2200, 2400, 2600, 2800};

    // FOUR: luminosity label
    const double luminosity = 35.922+41.529+59.740565202;

    // FIVE: plot observed line ?
    // const bool plotObserved = true;
    const bool plotObserved = false;
    const bool plotSigma = true;
    const bool plotSquark = false;
    const bool varyHiggsMass = true;
    const bool fixedYRange = false;

    if(fixedYRange) outputDir = outputDir + "_fixedYRange";


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////   

    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_brazil.cc %s/%s__plotting_brazil.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));

    int nPlots = 0;
    if(varyHiggsMass) nPlots = susyMasses.size();
    else nPlots = higgsMasses.size();

    for (int j = 0; j < nPlots; ++j) {

    // find out whether mH or mSusy is varied
    int fixedMass = -1;
    std::vector<int> variedMass;

    std::string xAxisTitle = "";
    if (!varyHiggsMass){
        fixedMass = higgsMasses[j];
        variedMass = susyMasses;
        xAxisTitle = "M_{SUSY} (GeV)";
    }
    else {
        fixedMass = susyMasses[j];
        variedMass = higgsMasses;
        xAxisTitle = "M_{H} (GeV)";   
    }


    const unsigned int nEntries = variedMass.size();
    
    std::vector<double> x_vec;
    std::vector<double> y_vec;
    std::vector<double> yObs_vec;
    std::vector<double> yErrUp1Sig_vec;
    std::vector<double> yErrDown1Sig_vec;
    std::vector<double> yErrUp2Sig_vec;
    std::vector<double> yErrDown2Sig_vec;
    std::vector<double> null_vec(nEntries, 0.0);

    double maxLimitValue = -1.0;
    double minLimitValue = 9999.99;
    for (auto massPoint : variedMass){

        std::string filePath = inputDir + "/higgsCombineTest.AsymptoticLimits.mH";
        if (varyHiggsMass) filePath += std::to_string(massPoint) + (plotSquark ? ".mSquark" : ".mSusy") + std::to_string(fixedMass) + ".root";
        else filePath += std::to_string(fixedMass) + (plotSquark ? ".mSquark" : ".mSusy") + std::to_string(massPoint) + ".root";

        std::string mapString = "mH";
        if (varyHiggsMass) mapString += std::to_string(massPoint) + (plotSquark ? "_mSquark" : "_mSusy") + std::to_string(fixedMass);
        else mapString += std::to_string(fixedMass) + (plotSquark ? "_mSquark" : "_mSusy") + std::to_string(massPoint);

        double xBR = xsecBR[mapString];
        if (!plotSigma) xBR = 1;
        std::cout<<mapString<<" "<<xBR<<std::endl;



        TFile * f = new TFile(filePath.c_str());
        TTree * T = (TTree*)f->Get("limit");
        double limitValue;
        T->SetBranchAddress("limit", &limitValue);

        x_vec.push_back(massPoint);

        T->GetEntry(2);
        y_vec.push_back(xBR*(limitValue));
        double medianLimit = limitValue;

        T->GetEntry(0);
        yErrDown2Sig_vec.push_back(xBR*(medianLimit - limitValue));
        if (limitValue*xBR < minLimitValue) minLimitValue = limitValue*xBR;

        T->GetEntry(1);
        yErrDown1Sig_vec.push_back(xBR*(medianLimit - limitValue));

        T->GetEntry(3);
        yErrUp1Sig_vec.push_back(xBR*(limitValue - medianLimit));

        T->GetEntry(4);
        yErrUp2Sig_vec.push_back(xBR*(limitValue - medianLimit));
        if (limitValue*xBR > maxLimitValue) maxLimitValue = limitValue*xBR;

        T->GetEntry(5);
        yObs_vec.push_back(xBR*(limitValue));
        if (plotObserved && limitValue*xBR > maxLimitValue) maxLimitValue = limitValue*xBR;
        if (plotObserved && limitValue*xBR < minLimitValue) minLimitValue = limitValue*xBR;
    }     

    TGraphAsymmErrors * g_obs = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(yObs_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]));
    g_obs->GetXaxis()->SetTitle(xAxisTitle.c_str());
    // g_obs->GetYaxis()->SetTitle("95% upper CL of r");
    if (plotSigma) g_obs->GetYaxis()->SetTitle("95% CL upper limit of #sigma#timesBR [pb]");
    else g_obs->GetYaxis()->SetTitle("95% upper CL of #sigma / #sigma_{theory}");
    TGraphAsymmErrors * g_exp = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(y_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]));
    TGraphAsymmErrors * g_expErr1Sig = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(y_vec[0]), &(null_vec[0]), &(null_vec[0]), &(yErrDown1Sig_vec[0]), &(yErrUp1Sig_vec[0]));
    TGraphAsymmErrors * g_expErr2Sig = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(y_vec[0]), &(null_vec[0]), &(null_vec[0]), &(yErrDown2Sig_vec[0]), &(yErrUp2Sig_vec[0]));

    if(fixedYRange) {
        maxLimitValue = 0.05;
        minLimitValue = 0.0002;
    }

    // the vector order goes: observed, expected, 1sigma, 2sigma
    Plotter brazilPlot = Plotter({g_obs, g_exp, g_expErr1Sig, g_expErr2Sig}, plotObserved);
    // brazilPlot.AddLegend(0.20, 0.45, 0.63, 0.86);
    brazilPlot.AddLegend(0.60, 0.85, 0.63, 0.86);
    brazilPlot.AddLatex(luminosity, "#it{Preliminary}");
    brazilPlot.SaveBrazil(Form("%s/linear_%s_fixedMass%d.pdf", outputDir.c_str(), plotSquark ? "squark" : "susy", fixedMass), 0.0, 1.05 * maxLimitValue);
    brazilPlot.SetLogY();
    brazilPlot.SaveBrazil(Form("%s/log_%s_fixedMass%d.pdf", outputDir.c_str(), plotSquark ? "squark" : "susy", fixedMass), 0.85 * minLimitValue, 1.15 * maxLimitValue);

    }

    return 0;
}