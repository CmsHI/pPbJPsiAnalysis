#void draw_1D_fitSys_total_8rap9pt(int isPA=1, bool isPrompt=false, double ymax=100, bool isMax=true)
#void draw_1D_fitSys_each_8rap9pt(int isPA=0, bool isPrompt=true, double ymax=100, TString szSys="sys02", bool isMax=true)

### pp
#root -l 'draw_1D_fitSys_total_8rap9pt.C+(0,true,6,false)'
#root -l 'draw_1D_fitSys_total_8rap9pt.C+(0,false,30,false)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(0,true,5,"sys01",false)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(0,false,10,"sys01",false)' #rms
root -l 'draw_1D_fitSys_each_8rap9pt.C+(0,true,5,"sys02",false)' #rms
root -l 'draw_1D_fitSys_each_8rap9pt.C+(0,false,10,"sys02",false)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(0,true,5,"sys03",false)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(0,false,30,"sys03",false)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(0,true,5,"sys04",false)' #rms
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(0,false,30,"sys04",false)' #rms
### pA
#root -l 'draw_1D_fitSys_total_8rap9pt.C+(1,true,6,false)'
#root -l 'draw_1D_fitSys_total_8rap9pt.C+(1,false,30,false)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(1,true,5,"sys01",false)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(1,false,10,"sys01",false)'
root -l 'draw_1D_fitSys_each_8rap9pt.C+(1,true,5,"sys02",false)'
root -l 'draw_1D_fitSys_each_8rap9pt.C+(1,false,10,"sys02",false)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(1,true,5,"sys03",false)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(1,true,5,"sys03",false)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(1,false,30,"sys03",false)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(1,true,5,"sys04",false)'
#root -l 'draw_1D_fitSys_each_8rap9pt.C+(1,false,30,"sys04",false)'

