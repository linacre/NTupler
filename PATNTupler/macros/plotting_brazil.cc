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


// [7.7800000, 0.5794000, 0.0688200, 0.0101300, 0.0040540, 0.0016373, 0.0006666, 0.0002707]

std::map<std::string, double> xsecBRNNLL{

{"mH30_mSusy800",  7.7800000*0.868*0.868},
{"mH50_mSusy800",  7.7800000*0.858*0.858},
{"mH70_mSusy800",  7.7800000*0.840*0.840},
{"mH90_mSusy800",  7.7800000*0.816*0.816},
{"mH110_mSusy800", 7.7800000*0.749*0.749},
{"mH125_mSusy800", 7.7800000*0.581*0.581},

{"mH30_mSusy1200", 0.5794000*0.868*0.868},
{"mH35_mSusy1200", 0.5794000*0.867*0.867},
{"mH40_mSusy1200", 0.5794000*0.865*0.865},
{"mH50_mSusy1200", 0.5794000*0.858*0.858},
{"mH70_mSusy1200", 0.5794000*0.840*0.840},
{"mH90_mSusy1200", 0.5794000*0.816*0.816},
{"mH110_mSusy1200",0.5794000*0.749*0.749},
{"mH125_mSusy1200",0.5794000*0.581*0.581},

{"mH30_mSusy1600", 0.0688200*0.868*0.868},
{"mH35_mSusy1600", 0.0688200*0.867*0.867},
{"mH40_mSusy1600", 0.0688200*0.865*0.865},
{"mH50_mSusy1600", 0.0688200*0.858*0.858},
{"mH70_mSusy1600", 0.0688200*0.840*0.840},
{"mH90_mSusy1600", 0.0688200*0.816*0.816},
{"mH110_mSusy1600",0.0688200*0.749*0.749},
{"mH125_mSusy1600",0.0688200*0.581*0.581},

{"mH30_mSusy2000", 0.0101300*0.868*0.868},
{"mH35_mSusy2000", 0.0101300*0.867*0.867},
{"mH40_mSusy2000", 0.0101300*0.865*0.865},
{"mH50_mSusy2000", 0.0101300*0.858*0.858},
{"mH70_mSusy2000", 0.0101300*0.840*0.840},
{"mH90_mSusy2000", 0.0101300*0.816*0.816},
{"mH110_mSusy2000",0.0101300*0.749*0.749},
{"mH125_mSusy2000",0.0101300*0.581*0.581},

{"mH30_mSusy2200", 0.0040540*0.868*0.868},
{"mH35_mSusy2200", 0.0040540*0.867*0.867},
{"mH40_mSusy2200", 0.0040540*0.865*0.865},
{"mH50_mSusy2200", 0.0040540*0.858*0.858},
{"mH70_mSusy2200", 0.0040540*0.840*0.840},
{"mH90_mSusy2200", 0.0040540*0.816*0.816},
{"mH110_mSusy2200",0.0040540*0.749*0.749},
{"mH125_mSusy2200",0.0040540*0.581*0.581},

{"mH30_mSusy2400", 0.0016373*0.868*0.868},
{"mH35_mSusy2400", 0.0016373*0.867*0.867},
{"mH40_mSusy2400", 0.0016373*0.865*0.865},
{"mH50_mSusy2400", 0.0016373*0.858*0.858},
{"mH70_mSusy2400", 0.0016373*0.840*0.840},
{"mH90_mSusy2400", 0.0016373*0.816*0.816},
{"mH110_mSusy2400",0.0016373*0.749*0.749},
{"mH125_mSusy2400",0.0016373*0.581*0.581},

{"mH30_mSusy2600", 0.0006666*0.868*0.868},
{"mH35_mSusy2600", 0.0006666*0.867*0.867},
{"mH40_mSusy2600", 0.0006666*0.865*0.865},
{"mH50_mSusy2600", 0.0006666*0.858*0.858},
{"mH70_mSusy2600", 0.0006666*0.840*0.840},
{"mH90_mSusy2600", 0.0006666*0.816*0.816},
{"mH110_mSusy2600",0.0006666*0.749*0.749},
{"mH125_mSusy2600",0.0006666*0.581*0.581},

{"mH30_mSusy2800", 0.0002707*0.868*0.868},
{"mH35_mSusy2800", 0.0002707*0.867*0.867},
{"mH40_mSusy2800", 0.0002707*0.865*0.865},
{"mH50_mSusy2800", 0.0002707*0.858*0.858},
{"mH70_mSusy2800", 0.0002707*0.840*0.840},
{"mH90_mSusy2800", 0.0002707*0.816*0.816},
{"mH110_mSusy2800",0.0002707*0.749*0.749},
{"mH125_mSusy2800",0.0002707*0.581*0.581},

};


std::map<int, double> xsecUnc{
{ 800, 0.066310276},
{1200, 0.079022196},
{1600, 0.093790377},
{2000, 0.113829758},
{2200, 0.127247992},
{2400, 0.143077412},
{2600, 0.160332877},
{2800, 0.181553335},
};


// sqtest 1g and 2g samples were processed using the same xsec weights as for the standard 0g samples.
// cat macro2.C
// {cout<<doubleBFatJetPairTree->GetEntries("nGluino==1") / double(doubleBFatJetPairTree->GetEntries("nGluino==0"))<<endl;}

std::map<std::string, double> sqtest_1gcorrection{

{"mH110_mSquark1200", 1.25465},
{"mH110_mSquark1600", 0.919378},
{"mH110_mSquark2000", 0.683306},
{"mH110_mSquark2200", 0.586471},
{"mH110_mSquark2400", 0.502796},
{"mH110_mSquark2600", 0.429027},
{"mH110_mSquark2800", 0.366982},
{"mH110_mSquark800", 1.7795},
{"mH125_mSquark1200", 1.25496},
{"mH125_mSquark1600", 0.92198},
{"mH125_mSquark2000", 0.678543},
{"mH125_mSquark2200", 0.585258},
{"mH125_mSquark2400", 0.502751},
{"mH125_mSquark2600", 0.428513},
{"mH125_mSquark2800", 0.367339},
{"mH125_mSquark800", 1.78731},
{"mH30_mSquark1200", 1.2622},
{"mH30_mSquark1600", 0.916749},
{"mH30_mSquark2000", 0.685581},
{"mH30_mSquark2200", 0.585582},
{"mH30_mSquark2400", 0.504697},
{"mH30_mSquark2600", 0.432282},
{"mH30_mSquark2800", 0.36845},
{"mH30_mSquark800", 1.77633},
{"mH35_mSquark1200", 1.248},
{"mH35_mSquark1600", 0.909693},
{"mH35_mSquark2000", 0.682491},
{"mH35_mSquark2200", 0.587282},
{"mH35_mSquark2400", 0.506144},
{"mH35_mSquark2600", 0.431195},
{"mH35_mSquark2800", 0.367447},
{"mH40_mSquark1200", 1.2508},
{"mH40_mSquark1600", 0.918081},
{"mH40_mSquark2000", 0.67759},
{"mH40_mSquark2200", 0.586723},
{"mH40_mSquark2400", 0.507654},
{"mH40_mSquark2600", 0.43379},
{"mH40_mSquark2800", 0.368846},
{"mH50_mSquark1200", 1.2575},
{"mH50_mSquark1600", 0.918352},
{"mH50_mSquark2000", 0.683646},
{"mH50_mSquark2200", 0.587502},
{"mH50_mSquark2400", 0.50544},
{"mH50_mSquark2600", 0.432106},
{"mH50_mSquark2800", 0.368057},
{"mH50_mSquark800", 1.78965},
{"mH70_mSquark1200", 1.25237},
{"mH70_mSquark1600", 0.92164},
{"mH70_mSquark2000", 0.680694},
{"mH70_mSquark2200", 0.586938},
{"mH70_mSquark2400", 0.50899},
{"mH70_mSquark2600", 0.430842},
{"mH70_mSquark2800", 0.367924},
{"mH70_mSquark800", 1.78792},
{"mH90_mSquark1200", 1.25509},
{"mH90_mSquark1600", 0.923682},
{"mH90_mSquark2000", 0.682324},
{"mH90_mSquark2200", 0.585892},
{"mH90_mSquark2400", 0.505804},
{"mH90_mSquark2600", 0.431891},
{"mH90_mSquark2800", 0.367486},
{"mH90_mSquark800", 1.78613}

};

std::map<std::string, double> sqtest_2gcorrection{

{"mH110_mSquark1200", 0.225691},
{"mH110_mSquark1600", 0.118251},
{"mH110_mSquark2000", 0.0612226},
{"mH110_mSquark2200", 0.0444784},
{"mH110_mSquark2400", 0.0324109},
{"mH110_mSquark2600", 0.0232086},
{"mH110_mSquark2800", 0.0165693},
{"mH110_mSquark800", 0.49085},
{"mH125_mSquark1200", 0.229665},
{"mH125_mSquark1600", 0.115479},
{"mH125_mSquark2000", 0.0602498},
{"mH125_mSquark2200", 0.0435949},
{"mH125_mSquark2400", 0.0311047},
{"mH125_mSquark2600", 0.023447},
{"mH125_mSquark2800", 0.0165372},
{"mH125_mSquark800", 0.491361},
{"mH30_mSquark1200", 0.228871},
{"mH30_mSquark1600", 0.118219},
{"mH30_mSquark2000", 0.0606368},
{"mH30_mSquark2200", 0.0442951},
{"mH30_mSquark2400", 0.0318727},
{"mH30_mSquark2600", 0.0233398},
{"mH30_mSquark2800", 0.0166615},
{"mH30_mSquark800", 0.48954},
{"mH35_mSquark1200", 0.228829},
{"mH35_mSquark1600", 0.115035},
{"mH35_mSquark2000", 0.0605089},
{"mH35_mSquark2200", 0.0439493},
{"mH35_mSquark2400", 0.0316568},
{"mH35_mSquark2600", 0.0233319},
{"mH35_mSquark2800", 0.016392},
{"mH40_mSquark1200", 0.229358},
{"mH40_mSquark1600", 0.115256},
{"mH40_mSquark2000", 0.0606738},
{"mH40_mSquark2200", 0.0435109},
{"mH40_mSquark2400", 0.0315787},
{"mH40_mSquark2600", 0.0233327},
{"mH40_mSquark2800", 0.0172244},
{"mH50_mSquark1200", 0.228666},
{"mH50_mSquark1600", 0.11647},
{"mH50_mSquark2000", 0.0611821},
{"mH50_mSquark2200", 0.0447392},
{"mH50_mSquark2400", 0.0312867},
{"mH50_mSquark2600", 0.0236132},
{"mH50_mSquark2800", 0.0169157},
{"mH50_mSquark800", 0.489377},
{"mH70_mSquark1200", 0.227368},
{"mH70_mSquark1600", 0.117622},
{"mH70_mSquark2000", 0.0608617},
{"mH70_mSquark2200", 0.0434102},
{"mH70_mSquark2400", 0.0324316},
{"mH70_mSquark2600", 0.023202},
{"mH70_mSquark2800", 0.0169797},
{"mH70_mSquark800", 0.489234},
{"mH90_mSquark1200", 0.226798},
{"mH90_mSquark1600", 0.116989},
{"mH90_mSquark2000", 0.0610911},
{"mH90_mSquark2200", 0.04411},
{"mH90_mSquark2400", 0.0315945},
{"mH90_mSquark2600", 0.0235709},
{"mH90_mSquark2800", 0.0168128},
{"mH90_mSquark800", 0.4877}

};

// MAKES BRAZIL PLOTS

int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    // ONE: input directory (where the combined root files are)
    // const std::string inputDir = "/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/combinedDataCards_20200527/combinedDataCards_ht_XSjmsryear_newZJ_2017as2018_0.98_allSig_ecalfilter_QCDlb0.1tunedubtuned5_bkg10pc_unccorrelated_maxunc2_jmrsymuncor_symall1.00.01";
    
    // const std::string inputDir = "/mercury/data2/linacre/NMSSM/Analysis_boostedNmssmHiggs/combinedDataCards_20200527/combinedDataCards_ht_XSjmsryear_newZJ_2017as2018_0.98_allSig_ecalfilter_QCDlb0.1tunedubtuned5_bkg10pc_unccorrelated_maxunc2_jmrsymuncor_symall1.00.01";
    const std::string inputDir = "/opt/ppd/scratch-2021/xxt18833/Analysis_boostedNmssmHiggs/combinedDataCards_20210225/combinedDataCards_ht_XSjmsryear_newZJ_2017as2018sqfix_0.98_allSig_ecalfilter_QCDlb0.0tunedubtuned5_bkg10pc_unccorrelated_maxunc2_jmrsymuncor_symall1.00.01/Asymptotic_copy";

    // const std::string inputDir = "/opt/ppd/scratch-2021/xxt18833/Analysis_boostedNmssmHiggs/combinedDataCards_20210204/combinedDataCards_ht_XSjmsryear_newZJ_2018_2017as2018sqtest2g_0.98_allSig_ecalfilter_QCDlb0.1tunedubtuned5_bkg10pc_unccorrelated_maxunc2_jmrsymuncor_symall1.00.01";
    // const std::string inputDir = "/opt/ppd/scratch/xxt18833/Analysis_boostedNmssmHiggs/combinedDataCards_20200120/combinedDataCards_final_2018_2017as2018_0.98_allSigs";
    // const std::string inputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_01_01/withGluino/allSys/";

    // TWO: plot output directory
    std::string outputDir = "brazilplots_unblinded_NNLLth";
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
    const bool plotObserved = true;
    // const bool plotObserved = false;
    const bool plotSigma = true;
    const bool plotSquark = false;
    const bool varyHiggsMass = true;
    const bool fixedYRange = true;

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
    std::vector<double> yTh_vec;
    std::vector<double> yThUp1Sig_vec;
    std::vector<double> yThDown1Sig_vec;
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
        double xBRNNLL = 1;
        if (!plotSigma) xBR = 1;
        if(plotSigma && !plotSquark) xBRNNLL = xsecBRNNLL[mapString];
        std::cout<<mapString<<" "<<xBR<<" "<<xBRNNLL<<std::endl;

        // if(plotSquark) xBR *= sqtest_2gcorrection[mapString];

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

        yTh_vec.push_back(xBRNNLL);

        yThDown1Sig_vec.push_back( xBRNNLL * xsecUnc[varyHiggsMass ? fixedMass : massPoint] );
        yThUp1Sig_vec.push_back( xBRNNLL * xsecUnc[varyHiggsMass ? fixedMass : massPoint] );
    }     

    TGraphAsymmErrors * g_obs = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(yObs_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]));
    g_obs->GetXaxis()->SetTitle(xAxisTitle.c_str());
    // g_obs->GetYaxis()->SetTitle("95% upper CL of r");
    if (plotSigma) g_obs->GetYaxis()->SetTitle("95% CL upper limit of #sigma#timesBR [pb]");
    else g_obs->GetYaxis()->SetTitle("95% upper CL of #sigma / #sigma_{theory}");
    TGraphAsymmErrors * g_exp = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(y_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]));
    TGraphAsymmErrors * g_expErr1Sig = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(y_vec[0]), &(null_vec[0]), &(null_vec[0]), &(yErrDown1Sig_vec[0]), &(yErrUp1Sig_vec[0]));
    TGraphAsymmErrors * g_expErr2Sig = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(y_vec[0]), &(null_vec[0]), &(null_vec[0]), &(yErrDown2Sig_vec[0]), &(yErrUp2Sig_vec[0]));

    TGraphAsymmErrors * g_th = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(yTh_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]));
    TGraphAsymmErrors * g_thErr1Sig = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(yTh_vec[0]), &(null_vec[0]), &(null_vec[0]), &(yThDown1Sig_vec[0]), &(yThUp1Sig_vec[0]));

    if(fixedYRange) {
        maxLimitValue = 0.06;
        minLimitValue = 0.00006;
    }

    // the vector order goes: observed, expected, 1sigma, 2sigma, th, 1sigma
    Plotter brazilPlot = Plotter({g_obs, g_exp, g_expErr1Sig, g_expErr2Sig, g_th, g_thErr1Sig}, plotObserved);
    // brazilPlot.AddLegend(0.20, 0.45, 0.63, 0.86);
    brazilPlot.AddLegend(0.60, 0.85, 0.63, 0.86);
    brazilPlot.AddLatex(luminosity, "#it{Preliminary}");
    brazilPlot.SaveBrazil(Form("%s/linear_%s_fixedMass%d_%s.pdf", outputDir.c_str(), plotSquark ? "squark" : "susy", fixedMass, plotSigma ? "xsec" : "mu"), 0.0, 1.05 * maxLimitValue);
    brazilPlot.SetLogY();
    brazilPlot.SaveBrazil(Form("%s/log_%s_fixedMass%d_%s.pdf", outputDir.c_str(), plotSquark ? "squark" : "susy", fixedMass, plotSigma ? "xsec" : "mu"), 0.85 * minLimitValue, 1.15 * maxLimitValue);

    }

    return 0;
}