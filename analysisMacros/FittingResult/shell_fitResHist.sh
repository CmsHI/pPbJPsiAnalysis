#int make_fitResHist(int MrapNpt=62, int isPA =3, int accCutType=2, TString szSys="nominal", int etOpt = 2){
### pp
#root -l -b -q 'make_fitResHist.C+(89,0,2,"nominal",0)'
#root -l -b -q 'make_fitResHist.C+(62,0,2,"nominal",0)'
### pA
#root -l -b -q 'make_fitResHist.C+(89,3,2,"nominal",0)'
root -l -b -q 'make_fitResHist.C+(62,3,2,"nominal",1)'
root -l -b -q 'make_fitResHist.C+(62,3,2,"nominal",2)'
root -l -b -q 'make_fitResHist.C+(62,3,2,"nominal",3)'
