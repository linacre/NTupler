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

// MAKES PLOTS USING HISTOGRAMS

void GetHistograms(std::map<std::string,TH1D*>&, int year);
void CombineHistograms(std::map<std::string,TH1D*>&, std::map<std::string,TH1D*>&, std::map<std::string,TH1D*>&);

int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////




    // ONE: save info & luminosity
    const std::string outputDir = "./histos_plot_combined/"; // where we are going to save the output plots (should include the samples name, and any important features)
    //const std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2018_08_03/2016_80X/oneDimensionRepresentation/DATA/control/predNew_calcForHighestTwoHtBins/"; // where we are going to save the output plots (should include the samples name, and any important features)
    
    const int year = 0;
    double luminosity = 0.;

    if(year==2016) luminosity = 35.922; // 2016 Plots::: NB this is just a label for the plot. It should match the lumi of the histograms!
    else if(year==2017)  luminosity = 41.529; // 2017 Plots::: NB this is just a label for the plot. It should match the lumi of the histograms!
    else if(year==2018) luminosity = 59.740565202; // 2018 Plots::: NB this is just a label for the plot. It should match the lumi of the histograms!
    else luminosity = 35.922 + 41.529 + 59.740565202;

    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_plot.cc %s/%s__histos_plot.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));

    std::map<std::string, TH1D*> h16_;
    std::map<std::string, TH1D*> h17_;
    std::map<std::string, TH1D*> h18_;

    if(year==2016) GetHistograms(h16_, 2016);
    else if(year==2017) GetHistograms(h16_, 2017);
    else if(year==2018) GetHistograms(h16_, 2018);
    else CombineHistograms(h16_, h17_, h18_);


    // explanation of terminology
    // 1. S, U, D --> refers to mass space. pred(Old)(New) is the prediction of S. UnD is the sum U+D.
    // 2. tag, anti, control --> refers to 2*DBT space
    // 3. sample name on the end
    // 4. OPTIONAL, add systematic name

    // FOR F_{i} PLOTS 2016: DON"T DELETE
    // h16_["S_anti_data"]->Divide(h16_["UnD_anti_data"]);
    // h16_["S_anti_data"]->GetYaxis()->SetTitle("F_{i}");
    // h16_["S_anti_QCD"]->Divide(h16_["UnD_anti_QCD"]);
    // h16_["S_anti_QCD"]->GetYaxis()->SetTitle("F_{i}");
    // TH1D * h16 = new TH1D("h16", "", 30, 0, 30);
    // for (unsigned int i = 1; i < 31; ++i){
        // h16->SetBinContent(i, QcdSidebandCorr::GetCorr(i, 2016));
        // h16->SetBinError(i, QcdSidebandCorr::GetCorrErr(i, 2016));
        // std::cout << Form("%.10f", h16_["S_anti_data"]->GetBinContent(i) ) << std::endl;
        // std::cout << Form("%.10f", h16_["S_anti_data"]->GetBinError(i) ) << std::endl;
    // }
    
    // FOR F_{i} PLOTS 2017: DON"T DELETE
    // h17_["S_anti_data"]->Divide(h17_["UnD_anti_data"]);
    // h17_["S_anti_data"]->GetYaxis()->SetTitle("F_{i}");
    // h17_["S_anti_QCD"]->Divide(h17_["UnD_anti_QCD"]);
    // h17_["S_anti_QCD"]->GetYaxis()->SetTitle("F_{i}");
    // TH1D * h17 = new TH1D("h17", "", 30, 0, 30);
    // for (unsigned int i = 1; i < 31; ++i){
    //     h17->SetBinContent(i, QcdSidebandCorr::GetCorr(i, 2017));
    //     h17->SetBinError(i, QcdSidebandCorr::GetCorrErr(i, 2017));
    //     std::cout << Form("%.10f", h17_["S_anti_data"]->GetBinContent(i) ) << std::endl;
    //     std::cout << Form("%.10f", h17_["S_anti_data"]->GetBinError(i) ) << std::endl;
    // }
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.

    // FOR F_{i} PLOTS 2018: DON"T DELETE
    // h18_["S_anti_data"]->Divide(h18_["UnD_anti_data"]);
    // h18_["S_anti_data"]->GetYaxis()->SetTitle("F_{i}");
    // h18_["S_anti_QCD"]->Divide(h18_["UnD_anti_QCD"]);
    // h18_["S_anti_QCD"]->GetYaxis()->SetTitle("F_{i}");
    // TH1D * h18 = new TH1D("h18", "", 30, 0, 30);
    // for (unsigned int i = 1; i < 31; ++i){
    //     h18->SetBinContent(i, QcdSidebandCorr::GetCorr(i, 2018));
    //     h18->SetBinError(i, QcdSidebandCorr::GetCorrErr(i, 2018));
    //     std::cout << Form("%.10f", h18_["S_anti_data"]->GetBinContent(i) ) << std::endl;
    //     std::cout << Form("%.10f", h18_["S_anti_data"]->GetBinError(i) ) << std::endl;
    // }
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.

    bool hackBinSyst = true;
    if(hackBinSyst) {
        /*
        TH1D* h_backgroundError = (TH1D*) h16_["predNew_tag_databkgsub"]->Clone();
        h_backgroundError->Sumw2(); // to avoid bug with AddBinContent adding an error for the first bin
        h_backgroundError->Add(h16_["S_tag_WJets"]);
        h_backgroundError->Add(h16_["S_tag_ZJets"]);
        h_backgroundError->Add(h16_["S_tag_TTJets"]);

        for (unsigned int iBin=iBinMin; iBin<=iBinMax; iBin++){
            // h_backgroundError->SetBinError(iBin,
        }
        */

        // hack to add bkg systs. Currently for combination only. Numbers from running theMoneyPlot.cc prefit, using input with no gmN nuisances and with F_i syst set to 0.001. (combinedDataCards_ht_XSjmsryear_newZJ_0.98_allSig_ecalfilter_QCDlb0.1tunedubtuned5_bkg10pc_unccorrelated_maxunc2_jmrsymuncor_symall1.00.01_nogmN_epsQCDsyst)
        double binsyst[31] = {0.};

        if(year==2016) {
            binsyst[1] = 12.9794;
            binsyst[2] = 8.24222;
            binsyst[3] = 1.44102;
            binsyst[4] = 2.98228;
            binsyst[5] = 7.99931;
            binsyst[6] = 8.3336;
            binsyst[7] = 6.43502;
            binsyst[8] = 6.22131;
            binsyst[9] = 6.43374;
            binsyst[10] = 6.12792;
            binsyst[11] = 0.784928;
            binsyst[12] = 0.186652;
            binsyst[13] = 0.890018;
            binsyst[14] = 0.850534;
            binsyst[15] = 1.50782;
            binsyst[16] = 0.810448;
            binsyst[17] = 0.583139;
            binsyst[18] = 0.744434;
            binsyst[19] = 1.33891;
            binsyst[20] = 1.53306;
            binsyst[21] = 0.00897116;
            binsyst[22] = 0.225159;
            binsyst[23] = 9.53351e-05;
            binsyst[24] = 0.00314198;
            binsyst[25] = 0.223477;
            binsyst[26] = 0.199062;
            binsyst[27] = 0.000100095;
            binsyst[28] = 1.18159;
            binsyst[29] = 0.271708;
            binsyst[30] = 0.000105277;
        }

        else if(year==2017) {
            binsyst[1] = 1.43297;
            binsyst[2] = 1.4879;
            binsyst[3] = 2.11821;
            binsyst[4] = 3.17609;
            binsyst[5] = 5.60133;
            binsyst[6] = 6.31703;
            binsyst[7] = 7.72194;
            binsyst[8] = 7.65671;
            binsyst[9] = 7.73159;
            binsyst[10] = 6.9635;
            binsyst[11] = 0.236294;
            binsyst[12] = 0.801837;
            binsyst[13] = 0.424796;
            binsyst[14] = 0.513209;
            binsyst[15] = 2.19642;
            binsyst[16] = 1.12647;
            binsyst[17] = 0.803422;
            binsyst[18] = 1.5277;
            binsyst[19] = 0.696753;
            binsyst[20] = 0.579429;
            binsyst[21] = 0.0179639;
            binsyst[22] = 0.00257456;
            binsyst[23] = 0.000743929;
            binsyst[24] = 0.474111;
            binsyst[25] = 0.206776;
            binsyst[26] = 0.179879;
            binsyst[27] = 9.66867e-05;
            binsyst[28] = 0.000104281;
            binsyst[29] = 0.313318;
            binsyst[30] = 0.465596;
        }

        else if(year==2018) {
            binsyst[1] = 1.7946;
            binsyst[2] = 2.76497;
            binsyst[3] = 3.71427;
            binsyst[4] = 6.73973;
            binsyst[5] = 10.4517;
            binsyst[6] = 10.9534;
            binsyst[7] = 11.4674;
            binsyst[8] = 10.8178;
            binsyst[9] = 9.70231;
            binsyst[10] = 13.8218;
            binsyst[11] = 0.594545;
            binsyst[12] = 1.02884;
            binsyst[13] = 0.845175;
            binsyst[14] = 0.415549;
            binsyst[15] = 1.14191;
            binsyst[16] = 2.3712;
            binsyst[17] = 0.832957;
            binsyst[18] = 0.902674;
            binsyst[19] = 0.915287;
            binsyst[20] = 1.14371;
            binsyst[21] = 0.000756673;
            binsyst[22] = 0.109403;
            binsyst[23] = 0.0948404;
            binsyst[24] = 0.222716;
            binsyst[25] = 0.239899;
            binsyst[26] = 0.447813;
            binsyst[27] = 0.54743;
            binsyst[28] = 0.0296198;
            binsyst[29] = 0.000103;
            binsyst[30] = 9.53281e-05;
        }

        else {
            binsyst[1] = 13.2859;
            binsyst[2] = 9.31793;
            binsyst[3] = 5.83484;
            binsyst[4] = 10.5405;
            binsyst[5] = 18.0168;
            binsyst[6] = 20.2324;
            binsyst[7] = 19.6461;
            binsyst[8] = 19.0355;
            binsyst[9] = 18.3439;
            binsyst[10] = 20.1528;
            binsyst[11] = 1.03107;
            binsyst[12] = 1.54672;
            binsyst[13] = 1.46996;
            binsyst[14] = 1.09348;
            binsyst[15] = 3.70189;
            binsyst[16] = 3.25264;
            binsyst[17] = 1.46044;
            binsyst[18] = 2.1315;
            binsyst[19] = 2.17547;
            binsyst[20] = 2.15608;
            binsyst[21] = 0.0207331;
            binsyst[22] = 0.249534;
            binsyst[23] = 0.0949082;
            binsyst[24] = 0.548751;
            binsyst[25] = 0.436571;
            binsyst[26] = 0.533465;
            binsyst[27] = 0.547433;
            binsyst[28] = 1.18591;
            binsyst[29] = 0.42598;
            binsyst[30] = 0.465595;
        }

        for (unsigned int iBin=1; iBin<=30; iBin++){
            // std::cout<<binsyst[iBin]/h16_["predNew_tag_databkgsub"]->GetBinError(iBin)<<std::endl;
            // double bgvar = h16_["S_tag_WJets"]->GetBinError(iBin)*h16_["S_tag_WJets"]->GetBinError(iBin) + h16_["S_tag_ZJets"]->GetBinError(iBin)*h16_["S_tag_ZJets"]->GetBinError(iBin) + h16_["S_tag_TTJets"]->GetBinError(iBin)*h16_["S_tag_TTJets"]->GetBinError(iBin);
            // std::cout<<sqrt(bgvar)<<" "<<sqrt( binsyst[iBin]*binsyst[iBin] + bgvar )  <<std::endl;

            h16_["S_tag_ZJets"]->SetBinError(iBin, sqrt( h16_["S_tag_ZJets"]->GetBinError(iBin)*h16_["S_tag_ZJets"]->GetBinError(iBin) +  binsyst[iBin]*binsyst[iBin] ) );
        }
    }

    // TWO: make plot aesthetics and saving

    // std::vector<TH1D*> indiHistoVec = {h16_["S_control_databkgsub"], h16_["predNew_control_databkgsub"]};

    std::vector<TH1D*> indiHistoVec = {h16_["S_tag_mH90_mSusy2600"], h16_["S_tag_mH90_mSusy1200"], h16_["S_tag_mH90_mSusy2000"]}; // for pre-fit
    // std::vector<TH1D*> indiHistoVec = {h16_["S_tag_mH110_mSusy2400"], h16_["S_tag_mH110_mSusy1600"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["S_tag_mH50_mSusy2200"], h16_["S_tag_mH90_mSusy2200"], h16_["S_tag_mH125_mSusy2200"]}; // for post-fit

    //std::vector<TH1D*> indiHistoVec = {h16_["S_tag_mH70_mSusy1200"], h16_["S_tag_mH70_mSusy2000"], h16_["S_tag_mH70_mSusy2800"], h16_["S_tag_2017as2018/mH70_mSusy1200"], h16_["S_tag_2017as2018/mH70_mSusy2000"], h16_["S_tag_2017as2018/mH70_mSusy2800"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["S_tag_mH70_mSusy1200"], h16_["S_tag_mH70_mSusy2000"], h16_["S_tag_mH70_mSusy2800"], h16_["S_tag_jmsD_mH70_mSusy1200"], h16_["S_tag_jmsD_mH70_mSusy2000"], h16_["S_tag_jmsD_mH70_mSusy2800"], h16_["S_tag_jmsU_mH70_mSusy1200"], h16_["S_tag_jmsU_mH70_mSusy2000"], h16_["S_tag_jmsU_mH70_mSusy2800"]};
    //std::vector<TH1D*> indiHistoVec = {h16_["S_tag_mH70_mSusy1200"], h16_["S_tag_mH70_mSusy2000"], h16_["S_tag_mH70_mSusy2800"], h16_["S_tag_jmsD_mH70_mSusy1200"], h16_["S_tag_jmsD_mH70_mSusy2000"], h16_["S_tag_jmsD_mH70_mSusy2800"], h16_["S_tag_2017as2018/mH70_mSusy1200"], h16_["S_tag_2017as2018/mH70_mSusy2000"], h16_["S_tag_2017as2018/mH70_mSusy2800"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["S_tag_mH70_mSusy1200"], h16_["S_tag_mH70_mSusy2000"], h16_["S_tag_mH70_mSusy2800"], h16_["S_tag_jmrU_mH70_mSusy1200"], h16_["S_tag_jmrU_mH70_mSusy2000"], h16_["S_tag_jmrU_mH70_mSusy2800"], h16_["S_tag_2017as2018/mH70_mSusy1200"], h16_["S_tag_2017as2018/mH70_mSusy2000"], h16_["S_tag_2017as2018/mH70_mSusy2800"]};
    // std::vector<TH1D*> stackHistoVec = {h16_["S_tag_WJets"], h16_["S_tag_ZJets"], h16_["S_tag_TTJets"], h16_["S_tag_QCD"]};
    std::vector<TH1D*> stackHistoVec = {h16_["S_tag_WJets"], h16_["S_tag_ZJets"], h16_[year > 2016 ? "S_tag_TTJetsALL":"S_tag_TTJets"], h16_["predNew_tag_databkgsub"]};

    // Plotter plot = Plotter(indiHistoVec);
    // Plotter plot = Plotter({}, stackHistoVec);
    // Plotter plot = Plotter(indiHistoVec, stackHistoVec);
    Plotter plot = Plotter(indiHistoVec, stackHistoVec, *h16_["S_tag_data"]);

    // std::vector<std::string> legendNames = {"control", "control prediction"};

    // plot.AddLegend(legendNames, 0.16, 0.38, 0.64, 0.83, 0.028);
    // plot.AddLegend(legendNames, 0.18, 0.38, 0.18, 0.30, 0.040);
    // plot.AddLegend(legendNames, 0.67, 0.88, 0.64, 0.83, 0.028);
    // plot.AddLegend(legendNames, 0.67, 0.88, 0.61, 0.80, 0.040); // with ratio box
    // plot.AddLegend2Cols(3, legendNames, 0.70, 0.88, 0.64, 0.83, 0.028);

    std::vector<std::string> legendNames = {"m_{S}2600", "m_{S}1200", "m_{S}2000", "WJets", "ZJets", "TTJets", "QCD"};  // for pre-fit
    // std::vector<std::string> legendNames = {"m_{H}50", "m_{H}90", "m_{H}125", "WJets", "ZJets", "TTJets", "QCD"};  // for post-fit
    plot.AddLegend2Cols(0, legendNames, 0.67, 0.95, 0.55, 0.80, 0.045);
    
    // plot.AddLatex();
    // plot.AddLatex(luminosity);
    // plot.AddLatex("#it{Preliminary}");
    plot.AddLatex(luminosity, "#it{Preliminary}");
    
    // plot.AddRatioBox("ratio");
    // plot.AddRatioBox("ratio", true);
    // plot.AddRatioBox(0.1, 1.9, "ratio", true);
    // plot.AddRatioBox("true / pred", true);
    // plot.AddRatioBox(0.1,2.4, "true / pred", true);
    // plot.AddRatioBox(0.84,1.16, "priv / central", true);

    plot.AddRatioBox(0.3,1.8, "data / pred", true);
    
    // plot.SetErrors();
    plot.SetErrors("only_stack");
    // plot.SetErrors("only_indi");

    std::vector<std::string> stringVec = {"HT1500-2500", "HT2500-3500", "HT3500+"};
    std::string plotName = "linear";
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);
    
    plotName = "log";
    plot.SetLogY();
    // plot.SetYValueMin(0.15); // REMEMBER THIS PARAM! (only for log)
    // plot.SetYValueMin(0.50); // REMEMBER THIS PARAM! (only for log)
    plot.SetYValueMin(0.40); // REMEMBER THIS PARAM! (only for log)
    // plot.SetYValueMin(1.10); // REMEMBER THIS PARAM! (only for log)
    // plot.SetYValueMin(3.10); // REMEMBER THIS PARAM! (only for log)
    // plot.SetYValueMin(12.10); // REMEMBER THIS PARAM! (only for log)
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);





    return 0;
}


void GetHistograms(std::map<std::string,TH1D*>& h_, int year)
{
    // histos locations

    std::string preamble;
    std::string postamble;
    std::vector<std::string> histoNameVec;
    std::vector<std::string> bgNameVec;

    if(year==2016) {

        // preamble = "/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/histos_2019_01_01/MassCutsV09/run2016/";
        preamble = "/mercury/data2/linacre/NMSSM/2016/Joe/run2016/";
        postamble = "MassCutsV09_ak8pt300_ht1500x2500x3500x_ak4pt300n-1_lumi36.root";

        histoNameVec.push_back("QCD");
        histoNameVec.push_back("TTJets");
        histoNameVec.push_back("ZJets");
        histoNameVec.push_back("WJets");

        // histoNameVec.push_back("mH70_mSusy2000_nPuIs23orLess");
        // histoNameVec.push_back("mH70_mSusy2000_nPuIs24orMore");
        // histoNameVec.push_back("mH70_mSusy2200_nPuIs23orLess");
        // histoNameVec.push_back("mH70_mSusy2200_nPuIs24orMore");

        bgNameVec.push_back("TTJets");
    }

    if(year==2017) {

        preamble = "/mercury/data2/linacre/Joe_backupfromScratch/xap79297/Analysis_boostedNmssmHiggs/histos_2019_01_01/MassCutsV09/run2017/";
        postamble = "MassCutsV09_ak8pt300_ht1500x2500x3500x_ak4pt300n-1_lumi42.root";

        histoNameVec.push_back("QCD");
        histoNameVec.push_back("TTJetsALL");
        // histoNameVec.push_back("TTJets0L");
        // histoNameVec.push_back("TTJets1L");
        // histoNameVec.push_back("TTJets2L");
        histoNameVec.push_back("ZJets");
        histoNameVec.push_back("WJets");

        bgNameVec.push_back("TTJetsALL");
    }

    if(year==2018) {

        preamble = "/home/ppd/xxt18833/NMSSM/PATNTupler_18/macros/2018_histos_ht/2017as2018/";
        postamble = "MassCutsV09_ak8pt300_ht1500x2500x3500x_ak4pt300n-1_lumi60.root";

        histoNameVec.push_back("QCD");
        histoNameVec.push_back("TTJetsALL");
        // histoNameVec.push_back("TTJets0L");
        // histoNameVec.push_back("TTJets1L");
        // histoNameVec.push_back("TTJets2L");
        histoNameVec.push_back("ZJets");
        histoNameVec.push_back("WJets");

        // histoNameVec.push_back("2017as2018/mH70_mSusy1200");
        // histoNameVec.push_back("2017as2018/mH70_mSusy2000");
        // histoNameVec.push_back("2017as2018/mH70_mSusy2800");

        bgNameVec.push_back("TTJetsALL");
    }

    bgNameVec.push_back("ZJets");
    bgNameVec.push_back("WJets");

    /*
    histoNameVec.push_back("mH70_mSusy1200");
    histoNameVec.push_back("mH70_mSusy2000");
    histoNameVec.push_back("mH70_mSusy2800");
    */

    // /*
    histoNameVec.push_back("mH30_mSusy800");
    // histoNameVec.push_back("mH40_mSusy800");
    histoNameVec.push_back("mH50_mSusy800");
    histoNameVec.push_back("mH70_mSusy800");
    histoNameVec.push_back("mH90_mSusy800");
    histoNameVec.push_back("mH110_mSusy800");
    histoNameVec.push_back("mH125_mSusy800");
    histoNameVec.push_back("mH30_mSusy1200");
    histoNameVec.push_back("mH40_mSusy1200");
    histoNameVec.push_back("mH50_mSusy1200");
    histoNameVec.push_back("mH70_mSusy1200");
    histoNameVec.push_back("mH90_mSusy1200");
    histoNameVec.push_back("mH110_mSusy1200");
    histoNameVec.push_back("mH125_mSusy1200");
    histoNameVec.push_back("mH30_mSusy1600");
    histoNameVec.push_back("mH40_mSusy1600");
    histoNameVec.push_back("mH50_mSusy1600");
    histoNameVec.push_back("mH70_mSusy1600");
    histoNameVec.push_back("mH90_mSusy1600");
    histoNameVec.push_back("mH110_mSusy1600");
    histoNameVec.push_back("mH125_mSusy1600");
    histoNameVec.push_back("mH30_mSusy2000");
    histoNameVec.push_back("mH40_mSusy2000");
    histoNameVec.push_back("mH50_mSusy2000");
    histoNameVec.push_back("mH70_mSusy2000");
    histoNameVec.push_back("mH90_mSusy2000");
    histoNameVec.push_back("mH110_mSusy2000");
    histoNameVec.push_back("mH125_mSusy2000");
    histoNameVec.push_back("mH30_mSusy2200");
    histoNameVec.push_back("mH40_mSusy2200");
    histoNameVec.push_back("mH50_mSusy2200");
    histoNameVec.push_back("mH70_mSusy2200");
    histoNameVec.push_back("mH90_mSusy2200");
    histoNameVec.push_back("mH110_mSusy2200");
    histoNameVec.push_back("mH125_mSusy2200");
    histoNameVec.push_back("mH30_mSusy2400");
    histoNameVec.push_back("mH40_mSusy2400");
    histoNameVec.push_back("mH50_mSusy2400");
    histoNameVec.push_back("mH70_mSusy2400");
    histoNameVec.push_back("mH90_mSusy2400");
    histoNameVec.push_back("mH110_mSusy2400");
    histoNameVec.push_back("mH125_mSusy2400");
    histoNameVec.push_back("mH30_mSusy2600");
    histoNameVec.push_back("mH40_mSusy2600");
    histoNameVec.push_back("mH50_mSusy2600");
    histoNameVec.push_back("mH70_mSusy2600");
    histoNameVec.push_back("mH90_mSusy2600");
    histoNameVec.push_back("mH110_mSusy2600");
    histoNameVec.push_back("mH125_mSusy2600");
    histoNameVec.push_back("mH30_mSusy2800");
    histoNameVec.push_back("mH40_mSusy2800");
    histoNameVec.push_back("mH50_mSusy2800");
    histoNameVec.push_back("mH70_mSusy2800");
    histoNameVec.push_back("mH90_mSusy2800");
    histoNameVec.push_back("mH110_mSusy2800");
    histoNameVec.push_back("mH125_mSusy2800");
    // histoNameVec.push_back("CPS_h70s1200");
    // histoNameVec.push_back("CPS_h70s2000");
    // histoNameVec.push_back("CPS_h70s2600");
    // */

    histoNameVec.push_back("data");

    for (size_t iH = 0; iH < histoNameVec.size(); ++iH){

        const std::string histoToUse = histoNameVec[iH];
        // std::cout<<histoToUse<<std::endl;
        TFile * f = new TFile(Form("%s/%s/%s", preamble.c_str(), histoToUse.c_str(), postamble.c_str()));
        // explanation of terminology
        // 1. S, U, D --> refers to mass space. pred is the prediction of S. UnD is the sum U+D.
        // 2. tag, anti, control --> refers to 2*DBT space
        // 3. sample name on the end

        h_[Form("S_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtDiagUpLoose_NOSYS");
        h_[Form("U_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtDiagUpLoose_NOSYS");
        h_[Form("D_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtDiagUpLoose_NOSYS");

        h_[Form("S_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtOffLooseAndOffLoose_NOSYS");
        h_[Form("U_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtOffLooseAndOffLoose_NOSYS");
        h_[Form("D_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtOffLooseAndOffLoose_NOSYS");

        h_[Form("S_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtLooseMed2AndOffIDBTCv23_NOSYS");
        h_[Form("U_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtLooseMed2AndOffIDBTCv23_NOSYS");
        h_[Form("D_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtLooseMed2AndOffIDBTCv23_NOSYS");
        h_[Form("S_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("S_dbtOffIDBTCv23AndLooseMed2_NOSYS"));
        h_[Form("U_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("U_dbtOffIDBTCv23AndLooseMed2_NOSYS"));
        h_[Form("D_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("D_dbtOffIDBTCv23AndLooseMed2_NOSYS"));

        h_[Form("UnD_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_tag_%s", histoToUse.c_str())]->Add(h_[Form("D_tag_%s", histoToUse.c_str())]);

        h_[Form("UnD_anti_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_anti_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_anti_%s", histoToUse.c_str())]->Add(h_[Form("D_anti_%s", histoToUse.c_str())]);

        h_[Form("UnD_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_control_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_control_%s", histoToUse.c_str())]->Add(h_[Form("D_control_%s", histoToUse.c_str())]);
   
        if(histoToUse == "data") {
                h_[Form("S_tag_%sbkgsub", histoToUse.c_str())] = (TH1D*)h_[Form("S_tag_%s", histoToUse.c_str())]->Clone();
                h_[Form("UnD_tag_%sbkgsub", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();

                h_[Form("S_anti_%sbkgsub", histoToUse.c_str())] = (TH1D*)h_[Form("S_anti_%s", histoToUse.c_str())]->Clone();
                h_[Form("UnD_anti_%sbkgsub", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_anti_%s", histoToUse.c_str())]->Clone();

                h_[Form("S_control_%sbkgsub", histoToUse.c_str())] = (TH1D*)h_[Form("S_control_%s", histoToUse.c_str())]->Clone();
                h_[Form("UnD_control_%sbkgsub", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_control_%s", histoToUse.c_str())]->Clone();


            for (size_t iB = 0; iB < bgNameVec.size(); ++iB){
                const std::string bgToUse = bgNameVec[iB];

                h_[Form("S_tag_%sbkgsub", histoToUse.c_str())]->Add( h_[Form("S_tag_%s", bgToUse.c_str())], -1);
                h_[Form("UnD_tag_%sbkgsub", histoToUse.c_str())]->Add( h_[Form("UnD_tag_%s", bgToUse.c_str())], -1);

                h_[Form("S_anti_%sbkgsub", histoToUse.c_str())]->Add( h_[Form("S_anti_%s", bgToUse.c_str())], -1);
                h_[Form("UnD_anti_%sbkgsub", histoToUse.c_str())]->Add( h_[Form("UnD_anti_%s", bgToUse.c_str())], -1);

                h_[Form("S_control_%sbkgsub", histoToUse.c_str())]->Add( h_[Form("S_control_%s", bgToUse.c_str())], -1);
                h_[Form("UnD_control_%sbkgsub", histoToUse.c_str())]->Add( h_[Form("UnD_control_%s", bgToUse.c_str())], -1);

            }
        }

        // NEW METHOD OF PREDICTION

        h_[Form("predNew_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();
        if(histoToUse == "data") h_[Form("predNew_tag_%sbkgsub", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();
        for (int iBin = 1; iBin < h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetNbinsX() + 1; ++iBin){
            double corrValue = QcdSidebandCorr::GetCorr(iBin, year);
            double corrError = QcdSidebandCorr::GetCorrErr(iBin, year);
            double UnDValue = h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetBinContent(iBin);
            double UnDError = h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetBinError(iBin);

            double predValue = corrValue * UnDValue;
            double predError = 0.0;
            if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
            h_[Form("predNew_tag_%s", histoToUse.c_str())]->SetBinContent(iBin, predValue);
            h_[Form("predNew_tag_%s", histoToUse.c_str())]->SetBinError(iBin, predError);

            if(histoToUse == "data") {
                UnDError *= UnDError;
                for (size_t iB = 0; iB < bgNameVec.size(); ++iB){
                    const std::string bgToUse = bgNameVec[iB];
                    UnDValue -= h_[Form("UnD_tag_%s", bgToUse.c_str())]->GetBinContent(iBin);
                    UnDError += h_[Form("UnD_tag_%s", bgToUse.c_str())]->GetBinError(iBin)*h_[Form("UnD_tag_%s", bgToUse.c_str())]->GetBinError(iBin);
                }
                // if(UnDValue < 0.) UnDValue = 0.;
                UnDError = sqrt(UnDError);

                predValue = corrValue * UnDValue;
                predError = 0.0;
                if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
                h_[Form("predNew_tag_%sbkgsub", histoToUse.c_str())]->SetBinContent(iBin, predValue);
                h_[Form("predNew_tag_%sbkgsub", histoToUse.c_str())]->SetBinError(iBin, predError);

            }

        }

        h_[Form("predNew_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_control_%s", histoToUse.c_str())]->Clone();
        if(histoToUse == "data") h_[Form("predNew_control_%sbkgsub", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_control_%s", histoToUse.c_str())]->Clone();
        for (int iBin = 1; iBin < h_[Form("predNew_control_%s", histoToUse.c_str())]->GetNbinsX() + 1; ++iBin){
            double corrValue = QcdSidebandCorr::GetCorr(iBin, year);
            double corrError = QcdSidebandCorr::GetCorrErr(iBin, year);
            double UnDValue = h_[Form("predNew_control_%s", histoToUse.c_str())]->GetBinContent(iBin);
            double UnDError = h_[Form("predNew_control_%s", histoToUse.c_str())]->GetBinError(iBin);

            double predValue = corrValue * UnDValue;
            double predError = 0.0;
            if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
            h_[Form("predNew_control_%s", histoToUse.c_str())]->SetBinContent(iBin, predValue);
            h_[Form("predNew_control_%s", histoToUse.c_str())]->SetBinError(iBin, predError);

            if(histoToUse == "data") {
                UnDError *= UnDError;
                for (size_t iB = 0; iB < bgNameVec.size(); ++iB){
                    const std::string bgToUse = bgNameVec[iB];
                    UnDValue -= h_[Form("UnD_control_%s", bgToUse.c_str())]->GetBinContent(iBin);
                    UnDError += h_[Form("UnD_control_%s", bgToUse.c_str())]->GetBinError(iBin)*h_[Form("UnD_control_%s", bgToUse.c_str())]->GetBinError(iBin);
                }
                // if(UnDValue < 0.) UnDValue = 0.;
                UnDError = sqrt(UnDError);

                predValue = corrValue * UnDValue;
                predError = 0.0;
                if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
                h_[Form("predNew_control_%sbkgsub", histoToUse.c_str())]->SetBinContent(iBin, predValue);
                h_[Form("predNew_control_%sbkgsub", histoToUse.c_str())]->SetBinError(iBin, predError);

            }

        }

        // OLD METHOD OF PREDICTION (note, no bkgsub implemented)
        h_[Form("predOld_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("predOld_tag_%s", histoToUse.c_str())]->Multiply(h_[Form("S_anti_%s", histoToUse.c_str())]);
        h_[Form("predOld_tag_%s", histoToUse.c_str())]->Divide(h_[Form("UnD_anti_%s", histoToUse.c_str())]);

        h_[Form("predOld_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_control_%s", histoToUse.c_str())]->Clone();
        h_[Form("predOld_control_%s", histoToUse.c_str())]->Multiply(h_[Form("S_anti_%s", histoToUse.c_str())]);
        h_[Form("predOld_control_%s", histoToUse.c_str())]->Divide(h_[Form("UnD_anti_%s", histoToUse.c_str())]);

        // SYSTEMATIC VARIATIONS (for TAG histograms)
        std::vector<std::string> nonTrivialSysVec;
        nonTrivialSysVec.push_back("jecAKXUncUp");
        nonTrivialSysVec.push_back("jecAKXUncDown");
        nonTrivialSysVec.push_back("jerAKXUncUp");
        nonTrivialSysVec.push_back("jerAKXUncDown");
        nonTrivialSysVec.push_back("jmsUncUp");
        nonTrivialSysVec.push_back("jmsUncDown");
        nonTrivialSysVec.push_back("jmrUncUp");
        nonTrivialSysVec.push_back("jmrUncDown");
        nonTrivialSysVec.push_back("dbtTagUp");
        nonTrivialSysVec.push_back("dbtTagDown");
        nonTrivialSysVec.push_back("isrUp");
        nonTrivialSysVec.push_back("isrDown");

        for (auto nonTrivialSys : nonTrivialSysVec){

            if ( (TH1D*)f->Get(Form("S_dbtDiagUpLoose_%s", nonTrivialSys.c_str())) == NULL ) continue;
            h_[Form("S_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())] = (TH1D*)f->Get(Form("S_dbtDiagUpLoose_%s", nonTrivialSys.c_str()));
            h_[Form("UnD_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())] = (TH1D*)f->Get(Form("U_dbtDiagUpLoose_%s", nonTrivialSys.c_str()));
            h_[Form("UnD_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())]->Add((TH1D*)f->Get(Form("D_dbtDiagUpLoose_%s", nonTrivialSys.c_str())));

        } // closes loop through nonTrivialSysVec

    } // closes loop through histoNameVec
}



void CombineHistograms(std::map<std::string,TH1D*>& h16_, std::map<std::string,TH1D*>& h17_, std::map<std::string,TH1D*>& h18_)
{
    GetHistograms(h16_, 2016);
    GetHistograms(h17_, 2017);
    GetHistograms(h18_, 2018);

    for (auto const& x : h16_)
    {
        // std::cout << x.first  // string (key)
        //           << ':' 
        //           << x.second->Integral() // string's value 
        //           << std::endl ;

        TString hist = x.first;
        // if ( hist.EndsWith("TTJets") ) hist += "ALL";
        hist.ReplaceAll("TTJets","TTJetsALL");

        // std::cout << " 2018: "<< h18_[hist.Data()]->Integral();
        // std::cout << " 2017: "<< h17_[hist.Data()]->Integral();

        // TODO: 2017 TTJetsALL is missing systematics, so use subcomponents instead if this is ever needed

        x.second->Add( h17_[hist.Data()] );
        x.second->Add( h18_[hist.Data()] );

        // std::cout << hist  // string (key)
        //           << ':' 
        //           << x.second->Integral() // string's value 
        //           << std::endl ;
    }

}
