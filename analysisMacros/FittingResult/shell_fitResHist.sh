#int make_fitResHist(int MrapNpt=89, int isPA =3, int accCutType=1){
root -l -b -q 'make_fitResHist.C+(89,0,1)'
root -l -b -q 'make_fitResHist.C+(89,0,2)'
root -l -b -q 'make_fitResHist.C+(89,3,1)'
root -l -b -q 'make_fitResHist.C+(89,3,2)'
