###void draw_1D_parameters(TString szBinning="8rap9pt", int isPA=1, bool isPrompt=true, bool isLog=false, TString szParam="coefExp")
###void draw_1D_chi2(TString szBinning="8rap9pt", int isPA=1, bool isPrompt=true, bool isLog=false, bool isNoErr=true, TString szParam="coefExp")

root -l -b <<EOF
.L draw_1D_yield.C++
.q
EOF
#sleep 1m
root -l -b <<EOF
.L draw_1D_parameters.C++
.q
EOF
#sleep 1m
root -l -b <<EOF
.L draw_1D_chi2.C++
.q
EOF
#sleep 1m

##### for yield
#yieldName=(nSig_Raw nBkg_Raw nPrompt_Raw nNonPrompt_Raw bFraction)
yieldName=(bFraction)

##### for parameters
parName=(coefExp fracG1 meanSig sigmaSig1 sigmaSig2 sigWidth alpha enne sigmaNPTrue coefExpNPTrue fracRes meanPRResW meanPRResN sigmaPRResW sigmaPRResN fracCtBkg1 fracCtBkg2 fracCtBkg3 lambdam lambdap lambdasym NLL Resolution)
#parName=(alpha enne)

##### for chi2Prob
chiName=(UnNormChi2_mass nFitParam_mass nFullBinsPull_mass Dof_mass Chi2_mass theChi2Prob_mass UnNormChi2_time nFitParam_time nFullBinsPull_time Dof_time Chi2_time theChi2Prob_time UnNormChi2_side nFitParam_side nFullBinsPull_side Dof_side Chi2_side theChi2Prob_side)
#chiName=(Chi2_side)


### loop

for yield in ${yieldName[@]}
do
  echo " *** loop for yield "
#  root -l 'draw_1D_yield.C+("8rap9pt",0,1,false,"'$yield'")'
  root -l 'draw_1D_yield.C+("8rap9pt",1,1,false,"'$yield'")'
#  root -l -b -q 'draw_1D_yield.C+("8rap9pt",0,1,false,"'$yield'")'
#  root -l -b -q 'draw_1D_yield.C+("8rap9pt",1,1,false,"'$yield'")'
done

for par in ${parName[@]}
do
  echo " *** loop for parameters "
#  root -l 'draw_1D_parameters.C+("8rap9pt",0,1,false,"'$par'")'
#  root -l 'draw_1D_parameters.C+("8rap9pt",1,1,false,"'$par'")'
#  root -l -b -q 'draw_1D_parameters.C+("8rap9pt",0,1,false,"'$par'")'
#  root -l -b -q 'draw_1D_parameters.C+("8rap9pt",1,1,false,"'$par'")'
done
  
for chi in ${chiName[@]}
do
  echo " *** loop for chi2Prob "
#  root -l 'draw_1D_chi2.C+("8rap9pt",0,1,false,true,"'$chi'")'
#  root -l 'draw_1D_chi2.C+("8rap9pt",1,1,false,true,"'$chi'")'
#  root -l -b -q 'draw_1D_chi2.C+("8rap9pt",0,1,false,true,"'$chi'")'
#  root -l -b -q 'draw_1D_chi2.C+("8rap9pt",1,1,false,true,"'$chi'")'
done

### pp
#root -l 'draw_1D_parameters.C+("8rap9pt",0,1,false,"coefExp")'
### pPb
#root -l -b -q 'draw_1D_parameters.C+("8rap9pt",1,1,false,"coefExp")'

