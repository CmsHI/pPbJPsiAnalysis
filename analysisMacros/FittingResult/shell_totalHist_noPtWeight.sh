#int make_totalHist_pp_noPtWeight(int MrapNpt=89, int accCutType = 2, bool useZvtxWeight=false, bool useSF = false, TString szSys="nominal"){

########### new cut ##########
#### pp
#root -l -b -q 'make_totalHist_pp_noPtWeight.C+(89,2,false,false,"nominal")'
root -l -b -q 'make_totalHist_pp_noPtWeight.C+(89,2,false,true,"nominal")'
root -l -b -q 'make_totalHist_pp_noPtWeight_tag3.C+(89,2,false,true,"nominal")'
root -l -b -q 'make_totalHist_pp_noPtWeight_eta45_tag5.C+(89,2,false,true,"nominal")'
#### pA
#root -l -b -q 'make_totalHist_pA_noPtWeight.C+(89,2,false,false,"nominal")'
#root -l -b -q 'make_totalHist_pA_noPtWeight.C+(89,2,true,false,"nominal")'
root -l -b -q 'make_totalHist_pA_noPtWeight.C+(89,2,true,true,"nominal")'
root -l -b -q 'make_totalHist_pA_noPtWeight_tag3.C+(89,2,true,true,"nominal")'
root -l -b -q 'make_totalHist_pA_noPtWeight_eta45_tag5.C+(89,2,true,true,"nominal")'

