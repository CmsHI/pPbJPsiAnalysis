#void draw_1D_fitSys_total(int MrapNpt=89, int isPA=1, bool isPrompt=false, double ymax=100, bool isMax=true, int etOpt = 2)
#void draw_1D_fitSys_each(int MrapNpt=89, int isPA=0, bool isPrompt=true, double ymax=6, TString szSys="sys02", bool isMax=false, int etOpt = 0)

######### pp
root -l -b -q 'draw_1D_fitSys_total.C+(89,0,true,6,true,0)'
root -l -b -q 'draw_1D_fitSys_total.C+(89,0,false,40,true,0)'
root -l -b -q 'draw_1D_fitSys_each.C+(89,0,true,5,"sys01",true,0)' 
root -l -b -q 'draw_1D_fitSys_each.C+(89,0,false,10,"sys01",true,0)' 
root -l -b -q 'draw_1D_fitSys_each.C+(89,0,true,5,"sys02",true,0)' 
root -l -b -q 'draw_1D_fitSys_each.C+(89,0,false,10,"sys02",true,0)' 
root -l -b -q 'draw_1D_fitSys_each.C+(89,0,true,5,"sys03",true,0)' 
root -l -b -q 'draw_1D_fitSys_each.C+(89,0,false,22,"sys03",true,0)' 
root -l -b -q 'draw_1D_fitSys_each.C+(89,0,true,5,"sys04",true,0)' 
root -l -b -q 'draw_1D_fitSys_each.C+(89,0,false,40,"sys04",true,0)' 
######### pA
root -l -b -q 'draw_1D_fitSys_total.C+(89,1,true,6,true,0)'
root -l -b -q 'draw_1D_fitSys_total.C+(89,1,false,40,true,0)'
root -l -b -q 'draw_1D_fitSys_each.C+(89,1,true,5,"sys01",true,0)'
root -l -b -q 'draw_1D_fitSys_each.C+(89,1,false,10,"sys01",true,0)'
root -l -b -q 'draw_1D_fitSys_each.C+(89,1,true,5,"sys02",true,0)'
root -l -b -q 'draw_1D_fitSys_each.C+(89,1,false,10,"sys02",true,0)'
root -l -b -q 'draw_1D_fitSys_each.C+(89,1,true,5,"sys03",true,0)'
root -l -b -q 'draw_1D_fitSys_each.C+(89,1,true,5,"sys03",true,0)'
root -l -b -q 'draw_1D_fitSys_each.C+(89,1,false,22,"sys03",true,0)'
root -l -b -q 'draw_1D_fitSys_each.C+(89,1,true,5,"sys04",true,0)'
root -l -b -q 'draw_1D_fitSys_each.C+(89,1,false,40,"sys04",true,0)'

######### E_T dependence
for iopt in 1 2 3
  do 
  root -l -b -q 'draw_1D_fitSys_total.C+(62,1,true,6,true,'$iopt')'
  root -l -b -q 'draw_1D_fitSys_total.C+(62,1,false,12,true,'$iopt')'
  root -l -b -q 'draw_1D_fitSys_each.C+(62,1,true,6,"sys01",true,'$iopt')'
  root -l -b -q 'draw_1D_fitSys_each.C+(62,1,false,12,"sys01",true,'$iopt')'
  root -l -b -q 'draw_1D_fitSys_each.C+(62,1,true,6,"sys02",true,'$iopt')'
  root -l -b -q 'draw_1D_fitSys_each.C+(62,1,false,12,"sys02",true,'$iopt')'
  root -l -b -q 'draw_1D_fitSys_each.C+(62,1,true,6,"sys03",true,'$iopt')'
  root -l -b -q 'draw_1D_fitSys_each.C+(62,1,false,12,"sys03",true,'$iopt')'
  root -l -b -q 'draw_1D_fitSys_each.C+(62,1,true,6,"sys04",true,'$iopt')'
  root -l -b -q 'draw_1D_fitSys_each.C+(62,1,false,12,"sys04",true,'$iopt')'
done

