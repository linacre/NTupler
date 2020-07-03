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
    
    // const double luminosity = 35.922; // 2016 Plots::: NB this is just a label for the plot. It should match the lumi of the histograms!
    // const double luminosity = 41.529; // 2017 Plots::: NB this is just a label for the plot. It should match the lumi of the histograms!
    const double luminosity = 59.740565202; // 2018 Plots::: NB this is just a label for the plot. It should match the lumi of the histograms!
  



    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_plot.cc %s/%s__histos_plot.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));

    std::map<std::string, TH1D*> h16_;
    std::map<std::string, TH1D*> h17_;
    std::map<std::string, TH1D*> h18_;
    //GetHistograms(h16_, 2016);
    //GetHistograms(h16_, 2017);
    //GetHistograms(h16_, 2018);

    CombineHistograms(h16_, h17_, h18_);


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





    // TWO: make plot aesthetics and saving

    std::vector<TH1D*> indiHistoVec = {h16_["S_control_databkgsub"], h16_["predNew_control_data"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["S_tag_data"], h16_["S_tag_mH70_mSusy1200"], h16_["S_tag_mH70_mSusy2000"], h16_["S_tag_mH70_mSusy2800"]};
    //std::vector<TH1D*> indiHistoVec = {h16_["S_tag_mH70_mSusy1200"], h16_["S_tag_mH70_mSusy2000"], h16_["S_tag_mH70_mSusy2800"], h16_["S_tag_2017as2018/mH70_mSusy1200"], h16_["S_tag_2017as2018/mH70_mSusy2000"], h16_["S_tag_2017as2018/mH70_mSusy2800"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["S_tag_mH70_mSusy1200"], h16_["S_tag_mH70_mSusy2000"], h16_["S_tag_mH70_mSusy2800"], h16_["S_tag_jmsD_mH70_mSusy1200"], h16_["S_tag_jmsD_mH70_mSusy2000"], h16_["S_tag_jmsD_mH70_mSusy2800"], h16_["S_tag_jmsU_mH70_mSusy1200"], h16_["S_tag_jmsU_mH70_mSusy2000"], h16_["S_tag_jmsU_mH70_mSusy2800"]};
    //std::vector<TH1D*> indiHistoVec = {h16_["S_tag_mH70_mSusy1200"], h16_["S_tag_mH70_mSusy2000"], h16_["S_tag_mH70_mSusy2800"], h16_["S_tag_jmsD_mH70_mSusy1200"], h16_["S_tag_jmsD_mH70_mSusy2000"], h16_["S_tag_jmsD_mH70_mSusy2800"], h16_["S_tag_2017as2018/mH70_mSusy1200"], h16_["S_tag_2017as2018/mH70_mSusy2000"], h16_["S_tag_2017as2018/mH70_mSusy2800"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["S_tag_mH70_mSusy1200"], h16_["S_tag_mH70_mSusy2000"], h16_["S_tag_mH70_mSusy2800"], h16_["S_tag_jmrU_mH70_mSusy1200"], h16_["S_tag_jmrU_mH70_mSusy2000"], h16_["S_tag_jmrU_mH70_mSusy2800"], h16_["S_tag_2017as2018/mH70_mSusy1200"], h16_["S_tag_2017as2018/mH70_mSusy2000"], h16_["S_tag_2017as2018/mH70_mSusy2800"]};
    // std::vector<TH1D*> stackHistoVec = {h16_["S_tag_WJets"], h16_["S_tag_ZJets"], h16_["S_tag_TTJetsALL"], h16_["S_tag_QCD"]};

    Plotter plot = Plotter(indiHistoVec);
    // Plotter plot = Plotter({}, stackHistoVec);
    // Plotter plot = Plotter(indiHistoVec, stackHistoVec);

    std::vector<std::string> legendNames = {"control", "control prediction"};

    // plot.AddLegend(legendNames, 0.16, 0.38, 0.64, 0.83, 0.028);
    // plot.AddLegend(legendNames, 0.18, 0.38, 0.18, 0.30, 0.040);
    // plot.AddLegend(legendNames, 0.67, 0.88, 0.64, 0.83, 0.028);
    plot.AddLegend(legendNames, 0.67, 0.88, 0.61, 0.80, 0.040); // with ratio box
    // plot.AddLegend2Cols(3, legendNames, 0.70, 0.88, 0.64, 0.83, 0.028);
    
    // plot.AddLatex();
    // plot.AddLatex(luminosity);
    // plot.AddLatex("#it{Preliminary}");
    plot.AddLatex(luminosity, "#it{Preliminary}");
    
    // plot.AddRatioBox("ratio");
    // plot.AddRatioBox("ratio", true);
    // plot.AddRatioBox(0.1, 1.9, "ratio", true);
    // plot.AddRatioBox("true / pred", true);
    plot.AddRatioBox(0.1,2.4, "true / pred", true);
    // plot.AddRatioBox(0.84,1.16, "priv / central", true);
    
    plot.SetErrors();
    // plot.SetErrors("only_stack");
    // plot.SetErrors("only_indi");

    std::vector<std::string> stringVec = {"HT1500-2500", "HT2500-3500", "HT3500+"};
    std::string plotName = "linear";
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);
    
    plotName = "log";
    plot.SetLogY();
    // plot.SetYValueMin(0.15); // REMEMBER THIS PARAM! (only for log)
    plot.SetYValueMin(0.50); // REMEMBER THIS PARAM! (only for log)
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

        preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2019_01_01/MassCutsV09/run2016/";
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

        preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2019_01_01/MassCutsV09/run2017/";
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

        preamble = "/home/ppd/xxt18833/NMSSM/PATNTupler_18/macros/2018_histos_ht";
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

    histoNameVec.push_back("mH70_mSusy1200");
    histoNameVec.push_back("mH70_mSusy2000");
    histoNameVec.push_back("mH70_mSusy2800");

    /*
    histoNameVec.push_back("mH30_mSusy800");
    histoNameVec.push_back("mH50_mSusy800");
    histoNameVec.push_back("mH70_mSusy800");
    histoNameVec.push_back("mH90_mSusy800");
    histoNameVec.push_back("mH125_mSusy800");
    histoNameVec.push_back("mH30_mSusy1200");
    histoNameVec.push_back("mH50_mSusy1200");
    histoNameVec.push_back("mH70_mSusy1200");
    histoNameVec.push_back("mH90_mSusy1200");
    histoNameVec.push_back("mH125_mSusy1200");
    histoNameVec.push_back("mH30_mSusy1600");
    histoNameVec.push_back("mH50_mSusy1600");
    histoNameVec.push_back("mH70_mSusy1600");
    histoNameVec.push_back("mH90_mSusy1600");
    histoNameVec.push_back("mH125_mSusy1600");
    histoNameVec.push_back("mH30_mSusy2000");
    histoNameVec.push_back("mH50_mSusy2000");
    histoNameVec.push_back("mH70_mSusy2000");
    histoNameVec.push_back("mH90_mSusy2000");
    histoNameVec.push_back("mH125_mSusy2000");
    histoNameVec.push_back("mH30_mSusy2200");
    histoNameVec.push_back("mH50_mSusy2200");
    histoNameVec.push_back("mH70_mSusy2200");
    histoNameVec.push_back("mH90_mSusy2200");
    histoNameVec.push_back("mH125_mSusy2200");
    histoNameVec.push_back("mH30_mSusy2400");
    histoNameVec.push_back("mH50_mSusy2400");
    histoNameVec.push_back("mH70_mSusy2400");
    histoNameVec.push_back("mH90_mSusy2400");
    histoNameVec.push_back("mH125_mSusy2400");
    histoNameVec.push_back("mH30_mSusy2600");
    histoNameVec.push_back("mH50_mSusy2600");
    histoNameVec.push_back("mH70_mSusy2600");
    histoNameVec.push_back("mH90_mSusy2600");
    histoNameVec.push_back("mH125_mSusy2600");
    histoNameVec.push_back("mH30_mSusy2800");
    histoNameVec.push_back("mH50_mSusy2800");
    histoNameVec.push_back("mH70_mSusy2800");
    histoNameVec.push_back("mH90_mSusy2800");
    histoNameVec.push_back("mH125_mSusy2800");
    histoNameVec.push_back("CPS_h70s1200");
    histoNameVec.push_back("CPS_h70s2000");
    histoNameVec.push_back("CPS_h70s2600");
    */

    histoNameVec.push_back("data");

    for (size_t iH = 0; iH < histoNameVec.size(); ++iH){

        const std::string histoToUse = histoNameVec[iH];
        std::cout<<histoToUse<<std::endl;
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
        bool subBkg = true;
        //TODO: save both data and databkgsub, and sum over years

        h_[Form("predNew_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();
        for (int iBin = 1; iBin < h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetNbinsX() + 1; ++iBin){
            double corrValue = QcdSidebandCorr::GetCorr(iBin, year);
            double corrError = QcdSidebandCorr::GetCorrErr(iBin, year);
            double UnDValue = h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetBinContent(iBin);
            double UnDError = h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetBinError(iBin);

            if(histoToUse == "data" && subBkg) {
                UnDError *= UnDError;
                for (size_t iB = 0; iB < bgNameVec.size(); ++iB){
                    const std::string bgToUse = bgNameVec[iB];
                    UnDValue -= h_[Form("UnD_tag_%s", bgToUse.c_str())]->GetBinContent(iBin);
                    UnDError += h_[Form("UnD_tag_%s", bgToUse.c_str())]->GetBinError(iBin)*h_[Form("UnD_tag_%s", bgToUse.c_str())]->GetBinError(iBin);
                }
                UnDError = sqrt(UnDError);
            }

            double predValue = corrValue * UnDValue;
            double predError = 0.0;
            if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
            h_[Form("predNew_tag_%s", histoToUse.c_str())]->SetBinContent(iBin, predValue);
            h_[Form("predNew_tag_%s", histoToUse.c_str())]->SetBinError(iBin, predError);
        }

        h_[Form("predNew_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_control_%s", histoToUse.c_str())]->Clone();
        for (int iBin = 1; iBin < h_[Form("predNew_control_%s", histoToUse.c_str())]->GetNbinsX() + 1; ++iBin){
            double corrValue = QcdSidebandCorr::GetCorr(iBin, year);
            double corrError = QcdSidebandCorr::GetCorrErr(iBin, year);
            double UnDValue = h_[Form("predNew_control_%s", histoToUse.c_str())]->GetBinContent(iBin);
            double UnDError = h_[Form("predNew_control_%s", histoToUse.c_str())]->GetBinError(iBin);

            if(histoToUse == "data" && subBkg) {
                UnDError *= UnDError;
                for (size_t iB = 0; iB < bgNameVec.size(); ++iB){
                    const std::string bgToUse = bgNameVec[iB];
                    UnDValue -= h_[Form("UnD_control_%s", bgToUse.c_str())]->GetBinContent(iBin);
                    UnDError += h_[Form("UnD_control_%s", bgToUse.c_str())]->GetBinError(iBin)*h_[Form("UnD_control_%s", bgToUse.c_str())]->GetBinError(iBin);
                }
                UnDError = sqrt(UnDError);
            }

            double predValue = corrValue * UnDValue;
            double predError = 0.0;
            if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
            h_[Form("predNew_control_%s", histoToUse.c_str())]->SetBinContent(iBin, predValue);
            h_[Form("predNew_control_%s", histoToUse.c_str())]->SetBinError(iBin, predError);
        }

        // OLD METHOD OF PREDICTION
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
        std::cout << x.first  // string (key)
                  << ':' 
                  << x.second->Integral() // string's value 
                  << std::endl ;

        TString hist = x.first;
        // if ( hist.EndsWith("TTJets") ) hist += "ALL";
        hist.ReplaceAll("TTJets","TTJetsALL");

        // std::cout << " 2018: "<< h18_[hist.Data()]->Integral();
        // std::cout << " 2017: "<< h17_[hist.Data()]->Integral();

        // TODO: 2017 TTJetsALL is missing systematics, so use subcomponents instead if this is ever needed

        x.second->Add( h17_[hist.Data()] );
        x.second->Add( h18_[hist.Data()] );

        std::cout << hist  // string (key)
                  << ':' 
                  << x.second->Integral() // string's value 
                  << std::endl ;
    }

}
