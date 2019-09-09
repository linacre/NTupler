# fancy limit plotter from Alex Titterton
# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/macros/limits_2d_sigInj.py

import os
import ROOT
from scipy import interpolate
import numpy as np
import matplotlib
matplotlib.use('agg')
from matplotlib import rc
matplotlib.rcParams['mathtext.fontset'] = 'custom'
matplotlib.rcParams['mathtext.rm'] = 'Bitstream Vera Sans'
matplotlib.rcParams['mathtext.it'] = 'Bitstream Vera Sans:italic'
matplotlib.rcParams['mathtext.bf'] = 'Bitstream Vera Sans:bold'
matplotlib.rcParams['text.latex.preamble'].append(r'\usepackage{amsmath}')
#matplotlib.rcParams['text.usetex'] = True
#matplotlib.rcParams['text.latex.unicode'] = True
#matplotlib.rcParams['text.dvipnghack'] = True
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.colors as mcol

import sys
import argparse as a


#############################
#############################
#############################
### ## # USER INPUTS # ## ###

mSusyVec = [1200, 1600, 2000, 2200, 2400, 2600, 2800]
mHiggsVec = [30, 35, 40, 50, 70, 90, 110, 125]
inputDirStandard = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_01_01/withGluino/allSys/"
inputDirComparison01 = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_01_01/withGluino/allSys_SIGNAL_INJECTION_mH70mSusy1600/"
inputDirComparison02 = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_01_01/withGluino/allSys_SIGNAL_INJECTION_mH70mSusy2000/"
inputDirComparison03 = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_01_01/withGluino/allSys_SIGNAL_INJECTION_mH70mSusy2400/"
outputDir = inputDirComparison03 + "/a_limit_plot_comparison_SIGNALINJ/"

standardLabel = ""
legendTextSize = 13.0

plotTitle = '77.5 fb$^{-1}$ (13 TeV)'

#############################
#############################
#############################
#############################

def interp(data, method='linear'):
    x = data[:,0]
    y = data[:,1]
    z = data[:,2]
    
    xi = np.linspace(x.min(), x.max(), 100)
    yi = np.linspace(y.min(), y.max(), 100)
    zi = mlab.griddata(x, y, z, xi, yi, interp=method)
    
    return xi, yi, zi

def interp2(data, method='linear', n_p=10):
    x = data[:,0]
    y = data[:,1]
    z = data[:,2]

    nx = np.unique(x)
    ny = np.unique(y)

    minn = min(len(nx), len(ny))
    
    xi = np.linspace(x.min(), x.max(), n_p)
    yi = np.linspace(y.min(), y.max(), n_p)
    xi, yi = np.meshgrid(xi,yi)
    zi = interpolate.griddata((x, y), z, (xi, yi), method=method)

    #zi = np.clip(zi, None, 6.)
    
    return xi, yi, zi

os.system("mkdir -p %s" % outputDir)
os.system("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/limits_2d_sigInj.py %s/limits_2d_sigInj_latestCopy.py" % outputDir)
f_obs = open("tmpLimits_obs.txt", 'w')

for mSusy in mSusyVec:
    for mHiggs in mHiggsVec:

        rootFile = "higgsCombineTest.AsymptoticLimits.mH" + str(mHiggs) + ".mSusy" + str(mSusy) + ".root"
        rootFile = os.path.join(inputDirStandard, rootFile)
        if os.path.isfile(rootFile) == False:
            print ""
            print "THIS PLOT IS MISSING: mH" + str(mHiggs) + "_mSusy" + str(mSusy)
            print ""
            continue
        f = ROOT.TFile(rootFile)
        T = f.Get("limit")
        T.GetEntry(5)
        f_obs.write("%d   %d   %f\n" % (mSusy, mHiggs, T.limit))        
f_obs.close()
obs = np.loadtxt("tmpLimits_obs.txt")
os.system("rm tmpLimits_obs.txt")
xk, yk, zk = interp(obs, 'linear') # observed line

#########################################

plt_obs = plt.contour(xk, yk, zk, [1.0], colors='k', linewidths=2.0)
plt_obs.collections[0].set_label('Observed')

plt.xlabel('M$_{SUSY}}$ (GeV)', fontsize=16)
plt.ylabel('M$_{H}$ (GeV)', fontsize=16)
plt.title('CMS $Preliminary$', loc='left', fontsize=17, fontweight='bold')
plt.title(plotTitle, loc='right', fontsize=17)

# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*
# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*
# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*

# now do the same for the comparison limits
f_obs = open("tmpLimits_obs.txt", 'w')

for mSusy in mSusyVec:
    for mHiggs in mHiggsVec:

        rootFile = "higgsCombineTest.AsymptoticLimits.mH" + str(mHiggs) + ".mSusy" + str(mSusy) + ".root"
        rootFile = os.path.join(inputDirComparison01, rootFile)
        if os.path.isfile(rootFile) == False:
            print ""
            print "THIS PLOT IS MISSING: mH" + str(mHiggs) + "_mSusy" + str(mSusy)
            print ""
            continue
        f = ROOT.TFile(rootFile)
        T = f.Get("limit")
        T.GetEntry(5)
        f_obs.write("%d   %d   %f\n" % (mSusy, mHiggs, T.limit))
        
f_obs.close()

obs = np.loadtxt("tmpLimits_obs.txt")
os.system("rm tmpLimits_obs.txt")

xk, yk, zk = interp(obs, 'linear') # observed line

plt_obs = plt.contour(xk, yk, zk, [1.0], colors='b', linewidths=2.0)
plt_obs.collections[0].set_label('mH70_mSusy1600 injection')

# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*
# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*
# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*

# now do the same for the comparison limits
f_obs = open("tmpLimits_obs.txt", 'w')

for mSusy in mSusyVec:
    for mHiggs in mHiggsVec:

        rootFile = "higgsCombineTest.AsymptoticLimits.mH" + str(mHiggs) + ".mSusy" + str(mSusy) + ".root"
        rootFile = os.path.join(inputDirComparison02, rootFile)
        if os.path.isfile(rootFile) == False:
            print ""
            print "THIS PLOT IS MISSING: mH" + str(mHiggs) + "_mSusy" + str(mSusy)
            print ""
            continue
        f = ROOT.TFile(rootFile)
        T = f.Get("limit")
        T.GetEntry(5)
        f_obs.write("%d   %d   %f\n" % (mSusy, mHiggs, T.limit))
        
f_obs.close()

obs = np.loadtxt("tmpLimits_obs.txt")
os.system("rm tmpLimits_obs.txt")

xk, yk, zk = interp(obs, 'linear') # observed line

plt_obs = plt.contour(xk, yk, zk, [1.0], colors='r',  linewidths=2.0)
plt_obs.collections[0].set_label('mH70_mSusy2000 injection')

# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*
# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*
# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*

# now do the same for the comparison limits
f_obs = open("tmpLimits_obs.txt", 'w')

for mSusy in mSusyVec:
    for mHiggs in mHiggsVec:

        rootFile = "higgsCombineTest.AsymptoticLimits.mH" + str(mHiggs) + ".mSusy" + str(mSusy) + ".root"
        rootFile = os.path.join(inputDirComparison03, rootFile)
        if os.path.isfile(rootFile) == False:
            print ""
            print "THIS PLOT IS MISSING: mH" + str(mHiggs) + "_mSusy" + str(mSusy)
            print ""
            continue
        f = ROOT.TFile(rootFile)
        T = f.Get("limit")
        T.GetEntry(5)
        f_obs.write("%d   %d   %f\n" % (mSusy, mHiggs, T.limit))
        
f_obs.close()

obs = np.loadtxt("tmpLimits_obs.txt")
os.system("rm tmpLimits_obs.txt")

xk, yk, zk = interp(obs, 'linear') # observed line

plt_obs = plt.contour(xk, yk, zk, [1.0], colors='g', linewidths=2.0)
plt_obs.collections[0].set_label('mH70_mSusy2400 injection')

# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*
# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*
# ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.* ,.*

plt.legend(loc='upper left', prop={'size': legendTextSize})
plt.show()
plt.savefig("%s/limit_plot_sigInj.pdf" % outputDir)