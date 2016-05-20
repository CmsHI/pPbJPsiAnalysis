#int make_totalHist_pp_noPtWeight_ctErrCorr(int MrapNpt=89, int accCutType = 2, bool useZvtxWeight=false, bool useSF = false, TString szSys="nominal"){
#### pp
root -l -b -q 'make_totalHist_pp_noPtWeight_ctErrCorr.C+(89,1,false,false,"nominal")'
#root -l -b -q 'make_totalHist_pp_noPtWeight_ctErrCorr.C+(89,1,false,true,"nominal")'
root -l -b -q 'make_totalHist_pp_noPtWeight_ctErrCorr.C+(89,2,false,false,"nominal")'
#root -l -b -q 'make_totalHist_pp_noPtWeight_ctErrCorr.C+(89,2,false,true,"nominal")'
#### pA
root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,1,false,false,"nominal")'
root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,1,true,false,"nominal")'
root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,1,true,true,"nominal")'
root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,2,false,false,"nominal")'
root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,2,true,false,"nominal")'
root -l -b -q 'make_totalHist_pA_noPtWeight_ctErrCorr.C+(89,2,true,true,"nominal")'

