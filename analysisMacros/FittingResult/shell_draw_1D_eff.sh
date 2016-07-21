#void draw_1D_eff_SF1_SF0(TString szBinning="8rap9pt", int isPA=0, bool isPrompt=false, bool isLog=false, bool isNoErr=true)
#void draw_1D_eff_Zvtx1_Zvtx0_pA(TString szBinning="8rap9pt", bool isPrompt=false, bool isLog=false, bool isNoErr=true)

root -l 'draw_1D_eff_SF1_SF0.C+("8rap9pt",0,true,false,true)'
root -l 'draw_1D_eff_SF1_SF0.C+("8rap9pt",0,false,false,true)'
root -l 'draw_1D_eff_SF1_SF0.C+("8rap9pt",1,true,false,true)'
root -l 'draw_1D_eff_SF1_SF0.C+("8rap9pt",1,false,false,true)'

#root -l 'draw_1D_eff_Zvtx1_Zvtx0_pA.C+("8rap9pt",true,false,true)'
#root -l 'draw_1D_eff_Zvtx1_Zvtx0_pA.C+("8rap9pt",false,false,true)'

