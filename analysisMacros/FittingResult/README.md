# General
- [SONGKYO.h], [Style.C], [Style2D.C] : macros for drawing and styling plots
- [clean.sh] : remove xx.d and xx.so xx.pcm files after running with compiling e.g.) root -l xxx.C+

# STEP 1)
- ./shell_fitResHist.sh to run make_fitResHist.C 
- Input from fitRes in this directory
- Output files
  - fitResHist_6rap2pt_pA_newcut_nominal_etOpt1.root
  - fitResHist_6rap2pt_pA_newcut_nominal_etOpt2.root
  - fitResHist_6rap2pt_pA_newcut_nominal_etOpt3.root
  - fitResHist_8rap9pt_pA_newcut_nominal_etOpt0.root
  - fitResHist_8rap9pt_pp_newcut_nominal_etOpt0.root

# STEP 2) 
- ./shell_totalHist_noPtWeight.sh to run make_totalHist_pp_noPtWeight.C and make_totalHist_pA_noPtWeight.C
- Input from [Acceptance] and [Efficiency]
- This will be used for [AccEffWeight]
- Output files
  - totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF1_noPtWeight.root
  - totalHist_pp_8rap9pt_newcut_nominal_Zvtx0_SF1_noPtWeight.root
- Output for other study
  - totalHist_pA_8rap9pt_newcut_nominal_Zvtx0_SF0_noPtWeight.root
  - totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF0_noPtWeight.root
  - totalHist_pp_8rap9pt_newcut_nominal_Zvtx0_SF0_noPtWeight.root

#STEP 3)
- ./shell_totalHist.sh to run make_totalHist_pp.C and make_totalHist_pA.C
- Input from [AccEffWeight]
- This will be used for [DrawFinalPlot]
- Output files
  - totalHist_pA_6rap2pt_newcut_nominal_Zvtx1_SF1_etOpt1.root
  - totalHist_pA_6rap2pt_newcut_nominal_Zvtx1_SF1_etOpt1_noPtWeight.root
  - totalHist_pA_6rap2pt_newcut_nominal_Zvtx1_SF1_etOpt2.root
  - totalHist_pA_6rap2pt_newcut_nominal_Zvtx1_SF1_etOpt2_noPtWeight.root
  - totalHist_pA_6rap2pt_newcut_nominal_Zvtx1_SF1_etOpt3.root
  - totalHist_pA_6rap2pt_newcut_nominal_Zvtx1_SF1_etOpt3_noPtWeight.root
  - totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF1_etOpt0.root
  - totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF1_etOpt0_noPtWeight.root
  - totalHist_pp_8rap9pt_newcut_nominal_Zvtx0_SF1_etOpt0.root
  - totalHist_pp_8rap9pt_newcut_nominal_Zvtx0_SF1_etOpt0_noPtWeight.root

# Other STEP)
- ./shell_draw_1D_eff.sh to run efficiency-related plots
- ./shell_draw_1D_param.sh to run draw_1D_parameters.C, draw_1D_yield.C, and draw_1D_chi2.C 


