#int make_totalHist_pA(int MrapNpt=89, int accCutType = 2, bool useZvtxWeight=true, bool useSF = true, bool noPtWeight = true, TString szSys="nominal", int etOpt=0){

########### new cut ##########
#### pp
root -l -b -q 'make_totalHist_pp.C+(89,2,false,true,true,"nominal",0)' # noPtWeight
#root -l -b -q 'make_totalHist_pp.C+(89,2,false,true,false,"nominal",0)' #actual
#### pA
#root -l -b -q 'make_totalHist_pA.C+(89,2,true,true,true,"nominal",0)' #noPtWeight
#root -l -b -q 'make_totalHist_pA.C+(62,2,true,true,true,"nominal",1)' #noPtWeight
#root -l -b -q 'make_totalHist_pA.C+(62,2,true,true,true,"nominal",2)' #noPtWeight
#root -l -b -q 'make_totalHist_pA.C+(62,2,true,true,true,"nominal",3)' #noPtWeight
root -l -b -q 'make_totalHist_pA.C+(89,2,true,true,false,"nominal",0)' #actual
#root -l -b -q 'make_totalHist_pA.C+(62,2,true,true,false,"nominal",1)' #actual
#root -l -b -q 'make_totalHist_pA.C+(62,2,true,true,false,"nominal",2)' #actual
#root -l -b -q 'make_totalHist_pA.C+(62,2,true,true,false,"nominal",3)' #actual

