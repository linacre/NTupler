import os
import sys
import re
import time

# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/macros/runCombinedBatch.py
# FROM THE LOCATION WHERE YOU WISH TO GET THE OUTPUT
# runs on all mHXX_mSusyYYYY directories.

#############################
#############################
#############################
### ## # USER INPUTS # ## ###

# A COMMA SEPARATED LIST OF NUISANCES THAT YOU WISH TO FREEZE.

nuisancesToFreeze = "" # >>> no freezing a thing
# nuisancesToFreeze = "signalPdfBOTH"
# nuisancesToFreeze = "isrReweightBOTH"
# nuisancesToFreeze = "SigDbtTag2016,SigDbtTag2017"
# nuisancesToFreeze = "TtDbtTag2016,TtDbtTag2017"
# nuisancesToFreeze = "XS_TTJets2016,XS_TTJets2017"
# nuisancesToFreeze = "XS_ZJets2016,XS_ZJets2017"
# nuisancesToFreeze = "XS_WJets2016,XS_WJets2017"
# nuisancesToFreeze = "luminosity2016,luminosity2017"
# nuisancesToFreeze = "jecAKXUncBOTH"
# nuisancesToFreeze = "jerAKXUnc2016,jerAKXUnc2017"
# nuisancesToFreeze = "jmsUncBOTH"
# nuisancesToFreeze = "jmrUncBOTH"
# nuisancesToFreeze = "prefire2016,prefire2017"
# nuisancesToFreeze = "ch1_F,ch2_F,ch3_F,ch4_F,ch5_F,ch6_F,ch7_F,ch8_F,ch9_F,ch10_F,ch11_F,ch12_F,ch13_F,ch14_F,ch15_F,ch16_F,ch17_F,ch18_F,ch19_F,ch20_F,ch21_F,ch22_F,ch23_F,ch24_F,ch25_F,ch26_F,ch27_F,ch28_F,ch29_F,ch30_F,ch31_F,ch32_F,ch33_F,ch34_F,ch35_F,ch36_F,ch37_F,ch38_F,ch39_F,ch40_F,ch41_F,ch42_F,ch43_F,ch44_F,ch45_F,ch46_F,ch47_F,ch48_F,ch49_F,ch50_F,ch51_F,ch52_F,ch53_F,ch54_F,ch55_F,ch56_F,ch57_F,ch58_F,ch59_F,ch60_F"

#############################
#############################
#############################
#############################

cmsswBase = os.popen("echo $CMSSW_BASE", "r").readline()
cmsswBase = cmsswBase.rstrip()

inputDir = os.getcwd()

signalDirs = os.popen("ls %s" % inputDir, "r").readlines()

timeStamp = str(time.localtime().tm_year) + "_" + str(time.localtime().tm_mon) + "_" + str(time.localtime().tm_mday) + "_"
timeStamp += str(time.localtime().tm_hour) + "_" + str(time.localtime().tm_min) + "_" + str(time.localtime().tm_sec)
batchDir = "batchFiles_" + timeStamp
os.mkdir(batchDir)

for signalDir in signalDirs:

    # IF YOU WANT TO BE SELECTIVE
    # if (signalDir[0:4] != "mH40"):
    #   continue

    # IF YOU WANT TO BE SELECTIVE
    # if (signalDir != "mH40_mSusy2400\n"):
    #   continue

    signalDir = signalDir.rstrip()
    c1 = -1
    c2 = -1
    c3 = len(signalDir)
    keyword = ""

    if (signalDir[-1] != "0"):
        continue

    if signalDir[0:2] != "mH":
        continue

    for i in range(2, len(signalDir)):
        if (signalDir[i] == "_"):
            c1 = i
            break

    for i in range(c1, len(signalDir)):
        if (signalDir[i:i+6] == "_mSusy"):
            c2 = i + 6
            keyword = "mSusy"
            break
        if (signalDir[i:i+8] == "_mSquark"):
            c2 = i + 8
            keyword = "mSquark"
            break

    for i in range(c2, len(signalDir)):
        if (signalDir[i].isdigit() == False):
            c3 = i
            break

    if (c1 == -1 or c2 == -1):
        continue

    higgsMass = signalDir[2:c1]
    susyMass = signalDir[c2:c3]
    fileToUseTXT = os.path.join(inputDir,signalDir,"allbins.txt")
    fileToUseROOT = os.path.join(inputDir,signalDir,"allbins.root")
    
    print "RUNNING COMBINE FOR mH" + higgsMass + " " + keyword + susyMass
    print "nuisance to freeze: " + nuisancesToFreeze

    condorFileName = signalDir + ".condor"
    jobFileName = signalDir + ".sh"

    f = open("%s/%s" % (batchDir,condorFileName), 'w')
    f.write("Universe                = vanilla\n")
    f.write("Executable              = %s/%s\n" % (batchDir,jobFileName) )
    f.write("Log                     = %s/%s.log\n" % (batchDir,signalDir) )
    f.write("Output                  = %s/%s.out\n" % (batchDir,signalDir) )
    f.write("Error                   = %s/%s.err\n" % (batchDir,signalDir) )
    f.write("Request_memory          = 1 GB\n")
    f.write("should_transfer_files   = YES\n")
    f.write("when_to_transfer_output = ON_EXIT_OR_EVICT\n")
    f.write("periodic_hold                   = (CurrentTime - EnteredCurrentStatus > 10000)\n")
    f.write("periodic_release                = (CurrentTime - EnteredCurrentStatus > 60)\n")
    f.write("periodic_remove                 = False\n")
    f.write("#\n")
    f.write("Getenv                  = True\n")
    f.write("Queue\n")
    f.close()

    g = open("%s/%s" % (batchDir,jobFileName), 'w')
    g.write("#!/bin/bash\n")
    g.write("cd %s\n" % cmsswBase)
    g.write("eval `scramv1 runtime -sh`\n")
    g.write("cd %s\n" % inputDir)
    g.write("text2workspace.py %s\n" % fileToUseTXT)
    if (len(nuisancesToFreeze) == 0):
        g.write("combine -M AsymptoticLimits --mass %s --keyword-value %s=%s %s\n" % (higgsMass, keyword, susyMass, fileToUseROOT) )
    else:
        g.write("combine -M AsymptoticLimits --freezeParameter %s --mass %s --keyword-value %s=%s %s\n" % (nuisancesToFreeze, higgsMass, keyword, susyMass, fileToUseROOT) )
    g.write("cd %s\n" % (signalDir) )
    #g.write("combine -M FitDiagnostics --saveShapes --saveWithUncertainties --saveOverallShapes --plots --mass %s --keyword-value %s=%s %s\n" % (higgsMass, keyword, susyMass, fileToUseTXT) )
    g.write("combine -M FitDiagnostics --saveShapes --saveWithUncertainties --saveOverallShapes --preFitValue 0 --mass %s --keyword-value %s=%s %s\n" % (higgsMass, keyword, susyMass, fileToUseTXT) )
    g.write("python /opt/ppd/scratch-2021/xxt18833/CMSSW_10_2_12/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a fitDiagnostics.root -g plots.root &> diffNuisances.log &\n")
    g.write("combineTool.py -M Impacts -d %s -m %s --doInitialFit --robustFit 1 --setParameterRanges r=-3,3\n" % (fileToUseROOT, higgsMass) )
    g.write("cd ..\n")
    g.close()
    os.chmod("%s/%s" % (batchDir,jobFileName), 0755)

    print "condor_submit %s/%s" % (batchDir,condorFileName)
    #os.system("condor_submit %s/%s" % (batchDir,condorFileName) )
    print ""
    print ""
