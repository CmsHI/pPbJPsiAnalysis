#void read_Acc_pt_forExcel(TString szBinning="6rap2pt", int etOpt=0, int isPA=1, bool isPrompt=true)
### Acc
root -l -b -q 'read_Acc_pt_forExcel.C+("6rap2pt",1,1,true)'
root -l -b -q 'read_Acc_pt_forExcel.C+("6rap2pt",1,1,false)'
### etOpt=1=2=3
#root -l -b -q 'read_Acc_pt_forExcel.C+("6rap2pt",2,1,true)'
#root -l -b -q 'read_Acc_pt_forExcel.C+("6rap2pt",2,1,false)'
#root -l -b -q 'read_Acc_pt_forExcel.C+("6rap2pt",3,1,true)'
#root -l -b -q 'read_Acc_pt_forExcel.C+("6rap2pt",3,1,false)'

### Eff
root -l -b -q 'read_Eff_pt_forExcel.C+("6rap2pt",1,1,true)'
root -l -b -q 'read_Eff_pt_forExcel.C+("6rap2pt",1,1,false)'
### etOpt=1=2=3
#root -l -b -q 'read_Eff_pt_forExcel.C+("6rap2pt",2,1,true)'
#root -l -b -q 'read_Eff_pt_forExcel.C+("6rap2pt",2,1,false)'
#root -l -b -q 'read_Eff_pt_forExcel.C+("6rap2pt",3,1,true)'
#root -l -b -q 'read_Eff_pt_forExcel.C+("6rap2pt",3,1,false)'

### Fit
root -l -b -q 'read_Fit_pt_forExcel.C+("6rap2pt",1,1,true)'
root -l -b -q 'read_Fit_pt_forExcel.C+("6rap2pt",1,1,false)'
root -l -b -q 'read_Fit_pt_forExcel.C+("6rap2pt",2,1,true)'
root -l -b -q 'read_Fit_pt_forExcel.C+("6rap2pt",2,1,false)'
root -l -b -q 'read_Fit_pt_forExcel.C+("6rap2pt",3,1,true)'
root -l -b -q 'read_Fit_pt_forExcel.C+("6rap2pt",3,1,false)'

### CorrY
root -l -b -q 'read_CorrY_pt_forExcel.C+("6rap2pt",1,1,true)'
root -l -b -q 'read_CorrY_pt_forExcel.C+("6rap2pt",1,1,false)'
root -l -b -q 'read_CorrY_pt_forExcel.C+("6rap2pt",2,1,true)'
root -l -b -q 'read_CorrY_pt_forExcel.C+("6rap2pt",2,1,false)'
root -l -b -q 'read_CorrY_pt_forExcel.C+("6rap2pt",3,1,true)'
root -l -b -q 'read_CorrY_pt_forExcel.C+("6rap2pt",3,1,false)'

### Eff Pbp and pPb separately
#void read_Eff_Pbp_pPb_pt_forExcel(TString szBinning="6rap2pt",int etOpt=0, int isPbp=2, bool isPrompt=true)
root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",1,1,true)'
root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",1,1,false)'
root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",1,2,true)'
root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",1,2,false)'
### etOpt=1=2=3
#root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",2,1,true)'
#root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",2,1,false)'
#root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",2,2,true)'
#root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",2,2,false)'
#root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",3,1,true)'
#root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",3,1,false)'
#root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",3,2,true)'
#root -l -b -q 'read_Eff_Pbp_pPb_pt_forExcel.C+("6rap2pt",3,2,false)'


