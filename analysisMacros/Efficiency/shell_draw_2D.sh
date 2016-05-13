###void draw_2D_eff(int MrapNpt=89, int isPA = 1, int accCutType =2, bool isPrompt = false, bool useZvtxWeight=false, bool useSF=false, TString szDir="dir_eff")

### pp
root -l -b -q 'draw_2D_eff.C+(89,0,2,true,false,false,"dir_eff")'
root -l -b -q 'draw_2D_eff.C+(89,0,2,false,false,false,"dir_eff")'
root -l -b -q 'draw_2D_eff_fine.C+(89,0,2,true,false,false,"dir_eff")'
root -l -b -q 'draw_2D_eff_fine.C+(89,0,2,false,false,false,"dir_eff")'
### Pbp
root -l -b -q 'draw_2D_eff.C+(89,1,2,true,false,false,"dir_eff")'
root -l -b -q 'draw_2D_eff.C+(89,1,2,false,false,false,"dir_eff")'
root -l -b -q 'draw_2D_eff_fine.C+(89,1,2,true,false,false,"dir_eff")'
root -l -b -q 'draw_2D_eff_fine.C+(89,1,2,false,false,false,"dir_eff")'

