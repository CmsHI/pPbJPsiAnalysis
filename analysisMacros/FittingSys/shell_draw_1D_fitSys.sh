#void draw_1D_fitSys_total(int isPA=1, bool isPrompt=false, double ymax=100, bool isMax=true)
#void draw_1D_fitSys_each_8rap9pt(int isPA=0, bool isPrompt=true, double ymax=100, TString szSys="sys02", bool isMax=true)

######### pp
#root -l 'draw_1D_fitSys_total.C+(89,0,true,6,false,0)'
#root -l 'draw_1D_fitSys_total.C+(89,0,false,30,false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,0,true,5,"sys01",false,0)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,0,false,10,"sys01",false,0)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,0,true,5,"sys02",false,0)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,0,false,10,"sys02",false,0)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,0,true,5,"sys03",false,0)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,0,false,22,"sys03",false,0)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,0,true,5,"sys04",false,0)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,0,false,30,"sys04",false,0)' #rms
######### pA
#root -l 'draw_1D_fitSys_total.C+(89,1,true,6,false,0)'
#root -l 'draw_1D_fitSys_total.C+(89,1,false,30,false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,true,5,"sys01",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,false,10,"sys01",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,true,5,"sys02",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,false,10,"sys02",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,true,5,"sys03",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,true,5,"sys03",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,false,22,"sys03",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,true,5,"sys04",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,false,30,"sys04",false,0)'

######### E_T dependence
for iopt in 1 2
  do 
  root -l 'draw_1D_fitSys_total.C+(62,1,true,6,false,'$iopt')'
  root -l 'draw_1D_fitSys_total.C+(62,1,false,30,false,'$iopt')'
done

#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,true,5,"sys01",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,false,10,"sys01",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,true,5,"sys02",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,false,10,"sys02",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,true,5,"sys03",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,true,5,"sys03",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,false,22,"sys03",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,true,5,"sys04",false,0)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(89,1,false,30,"sys04",false,0)'

