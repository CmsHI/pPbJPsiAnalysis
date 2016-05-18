#int make_totalHist_pp_noPtWeight_ctErrCorr(int MrapNpt=89, int accCutType = 2, bool useZvtxWeight=false, bool useSF = false){
#### pp
#root -l -b -q 'make_totalHist_pp_noPtWeight_ctErrCorr.C+(89,1,false,false)'
#root -l -b -q 'make_totalHist_pp_noPtWeight_ctErrCorr.C+(89,1,false,true)'
#root -l -b -q 'make_totalHist_pp_noPtWeight_ctErrCorr.C+(89,2,false,false)'
#root -l -b -q 'make_totalHist_pp_noPtWeight_ctErrCorr.C+(89,2,false,true)'
#### pA
#root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,1,false,false)'
#root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,1,true,false)'
root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,1,true,true)'
#root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,2,false,false)'
#root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,2,true,false)'
root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,2,true,true)'

