###void fitRatio_dNdPt(int isPA=1, bool isPrompt=true){
root -l -b -q 'fitRatio_dNdPt.C+(0,true)'
root -l -b -q 'fitRatio_dNdPt.C+(0,false)'
root -l -b -q 'fitRatio_dNdPt.C+(1,true)'
root -l -b -q 'fitRatio_dNdPt.C+(1,false)'
