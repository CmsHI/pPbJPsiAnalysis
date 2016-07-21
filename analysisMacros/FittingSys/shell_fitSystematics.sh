###int make_fitSystematics_rms(int MrapNpt=89, int isPA =1, int accCutType=2, int etOpt = 2){
#root -l -b -q 'make_fitSystematics_max.C+(89,0,2,0)'
#root -l -b -q 'make_fitSystematics_max.C+(89,1,2,0)'
#root -l -b -q 'make_fitSystematics_rms.C+(89,0,2,0)'
#root -l -b -q 'make_fitSystematics_rms.C+(89,1,2,0)'
root -l -b -q 'make_fitSystematics_rms.C+(62,1,2,1)'
root -l -b -q 'make_fitSystematics_rms.C+(62,1,2,2)'
root -l -b -q 'make_fitSystematics_rms.C+(62,1,2,3)'
