#void read_Acc_pt_forExcel(TString szBinning="8rap9pt", int isPA=1, bool isPrompt=true)
### Acc
root -l -b -q 'read_Acc_pt_forExcel.C+("8rap9pt",0,true)'
root -l -b -q 'read_Acc_pt_forExcel.C+("8rap9pt",0,false)'
root -l -b -q 'read_Acc_pt_forExcel.C+("8rap9pt",1,true)'
root -l -b -q 'read_Acc_pt_forExcel.C+("8rap9pt",1,false)'

### Eff
root -l -b -q 'read_Eff_pt_forExcel.C+("8rap9pt",0,true)'
root -l -b -q 'read_Eff_pt_forExcel.C+("8rap9pt",0,false)'
root -l -b -q 'read_Eff_pt_forExcel.C+("8rap9pt",1,true)'
root -l -b -q 'read_Eff_pt_forExcel.C+("8rap9pt",1,false)'

### Fit
root -l -b -q 'read_Fit_pt_forExcel.C+("8rap9pt",0,true)'
root -l -b -q 'read_Fit_pt_forExcel.C+("8rap9pt",0,false)'
root -l -b -q 'read_Fit_pt_forExcel.C+("8rap9pt",1,true)'
root -l -b -q 'read_Fit_pt_forExcel.C+("8rap9pt",1,false)'

### CorrY
root -l -b -q 'read_CorrY_pt_forExcel.C+("8rap9pt",0,true)'
root -l -b -q 'read_CorrY_pt_forExcel.C+("8rap9pt",0,false)'
root -l -b -q 'read_CorrY_pt_forExcel.C+("8rap9pt",1,true)'
root -l -b -q 'read_CorrY_pt_forExcel.C+("8rap9pt",1,false)'

### Eff Pbp and pPb separately
#void read_Eff_Pbp_pPb_pt_forExcel(TString szBinning="8rap9pt", int isPbp=2, bool isPrompt=true)
root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("8rap9pt",1,true)'
root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("8rap9pt",1,false)'
root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("8rap9pt",2,true)'
root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("8rap9pt",2,false)'


