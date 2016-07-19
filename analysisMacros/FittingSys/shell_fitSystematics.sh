#int make_fitSystematics_max(int MrapNpt=89, int isPA =1, int accCutType=2){
#root -l -b -q 'make_fitSystematics_max.C+(89,0,2)'
#root -l -b -q 'make_fitSystematics_max.C+(89,1,2)'
root -l -b -q 'make_fitSystematics_rms.C+(89,0,2)'
root -l -b -q 'make_fitSystematics_rms.C+(89,1,2)'
