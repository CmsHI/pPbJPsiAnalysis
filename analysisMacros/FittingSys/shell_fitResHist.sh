#int make_fitResHist(int MrapNpt=89, int isPA =3, int accCutType=1, TString szSys="nominal"){

#### pp
root -l -b -q 'make_fitResHist.C+(89,0,2,"sys01_01",0)'
root -l -b -q 'make_fitResHist.C+(89,0,2,"sys01_02",0)'
root -l -b -q 'make_fitResHist.C+(89,0,2,"sys01_03",0)'
root -l -b -q 'make_fitResHist.C+(89,0,2,"sys02_01",0)'
root -l -b -q 'make_fitResHist.C+(89,0,2,"sys03_01",0)'
root -l -b -q 'make_fitResHist.C+(89,0,2,"sys03_02",0)'
root -l -b -q 'make_fitResHist.C+(89,0,2,"sys04_01",0)'

#### pA
root -l -b -q 'make_fitResHist.C+(89,3,2,"sys01_01",0)'
root -l -b -q 'make_fitResHist.C+(89,3,2,"sys01_02",0)'
root -l -b -q 'make_fitResHist.C+(89,3,2,"sys01_03",0)'
root -l -b -q 'make_fitResHist.C+(89,3,2,"sys02_01",0)'
root -l -b -q 'make_fitResHist.C+(89,3,2,"sys03_01",0)'
root -l -b -q 'make_fitResHist.C+(89,3,2,"sys03_02",0)'
root -l -b -q 'make_fitResHist.C+(89,3,2,"sys04_01",0)'
