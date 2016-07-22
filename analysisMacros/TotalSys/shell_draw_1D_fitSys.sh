### void draw_1D_TotSys(int MrapNpt=89, int isPA=1, bool isPrompt=false, double ymax=100, int etOpt = 0)

######### pp
root -l -b -q 'draw_1D_TotSys.C+(89,0,true,12,0)'
root -l -b -q 'draw_1D_TotSys.C+(89,0,false,32,0)'
######### pA
root -l -b -q 'draw_1D_TotSys.C+(89,1,true,12,0)'
root -l -b -q 'draw_1D_TotSys.C+(89,1,false,32,0)'

######### E_T dependence
for iopt in 1 2 3
  do 
  root -l -b -q 'draw_1D_TotSys.C+(62,1,true,8,'$iopt')'
  root -l -b -q 'draw_1D_TotSys.C+(62,1,false,12,'$iopt')'
done

