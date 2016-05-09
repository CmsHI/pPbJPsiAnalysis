###int draw_1D_eff(int MrapNpt=89, int isPA = 0, int accCutType =2, bool isPrompt = false, bool useZvtxWeight=false, bool useSF=false, char* szDir="dir_eff")

##### new cut
### pp
#root -l -b -q 'draw_1D_eff.C+(89,0,2,true,false,false,"dir_eff")'
#root -l -b -q 'draw_1D_eff.C+(89,0,2,true,true,false,"dir_eff")'
#root -l -b -q 'draw_1D_eff.C+(89,0,2,true,true,true,"dir_eff")'
root -l -b -q 'draw_1D_eff.C+(89,0,2,false,false,false,"dir_eff")'
root -l -b -q 'draw_1D_eff.C+(89,0,2,false,true,false,"dir_eff")'
#root -l -b -q 'draw_1D_eff.C+(89,0,2,false,true,true,"dir_eff")'
### Pbp
#root -l -b -q 'draw_1D_eff.C+(89,1,2,true,false,false,"dir_eff")'
#root -l -b -q 'draw_1D_eff.C+(89,1,2,true,true,false,"dir_eff")'
### pPb
#root -l -b -q 'draw_1D_eff.C+(89,2,2,true,false,false,"dir_eff")'
#root -l -b -q 'draw_1D_eff.C+(89,2,2,true,true,false,"dir_eff")'
