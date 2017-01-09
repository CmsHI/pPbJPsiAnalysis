# General
- [SONGKYO.h], [Style.C], [Style2D.C] : macros for drawing and styling plots
- [clean.sh] : remove xx.d and xx.so xx.pcm files after running with compiling e.g.) root -l xxx.C+

# STEP 1)
- [./shell_fitResHist.sh] to run [make_fitResHist.C] 
- Input from fitRes in this directory
- Output files (sys01_01, sys01_02, sys01_02, sys02_01, sys03_01, sys03_02, sys04_01)
  - fitResHist_6rap2pt_pA_newcut_nominal_etOpt1_sysXX_XX.root
  - fitResHist_6rap2pt_pA_newcut_nominal_etOpt2_sysXX_XX.root
  - fitResHist_6rap2pt_pA_newcut_nominal_etOpt3_sysXX_XX.root
  - fitResHist_8rap9pt_pA_newcut_nominal_etOpt0_sysXX_XX.root
  - fitResHist_8rap9pt_pp_newcut_nominal_etOpt0_sysXX_XX.root

# STEP 2) 
- [./shell_fitSystematics.sh] to run [make_fitSystematics_max.C]
- This will be used for [TotSyts]
- Output files
  - fitSysErr_6rap2pt_pA_newcut_etOpt1_max.root
  - fitSysErr_6rap2pt_pA_newcut_etOpt2_max.root
  - fitSysErr_6rap2pt_pA_newcut_etOpt3_max.root
  - fitSysErr_8rap9pt_pA_newcut_etOpt0_max.root
  - fitSysErr_8rap9pt_pp_newcut_etOpt1_max.root

#STEP 3)
- [./shell_draw_1D_fitSys.sh] to run [draw_1D_fitSys_total.C] and [draw_1D_fitSys_each.C]

