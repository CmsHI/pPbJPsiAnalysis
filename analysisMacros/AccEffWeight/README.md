# General
- [SONGKYO.h], [Style.C], [Style2D.C] : macros for drawing and styling plots
- [clean.sh] : remove xx.d and xx.so xx.pcm files after running with compiling e.g.) root -l xxx.C+

# STEP1 : get dN/dPt distributions and ratios (Data/MC), and fit them
  - ./shell_draw_dNdPt.sh : to run [draw_dNdPt.C] and reults are saved in [dir_dNdPt]
  - ./shell_fitRatio_dNdPt.sh : to run [fitRatio_dNdPt.C] and results are saved in [dir_fitRatio]

#STEP2 : reweight accpetnace and efficiencies using fit functions obtained from STEP1
  - 1) Acceptance 
    - ./shell_rootAna_acceptance.sh : to run [rootAna_acceptance_ptWeight.C]
  - 2) Effieicney
    - For Pbp and pPb prompt -> ./shell_rootAna_efficiency.sh to run [rootAna_efficiency_ptWeight.C]
    - For Pbp and pPb non-prompt, or pp -> ./shell_rootAna_efficiency_divideEvt.sh -> shell_hadd_STEP2.sh -> shell_makeEffAna_STEP3.sh
    - (Detailed descriptions are in [Efficiency] directory)
  
# Final Output Files
- AccAna_8rap9pt_pp_PR_newcut_ptWeight.root
- AccAna_8rap9pt_pp_NP_newcut_ptWeight.root
- AccAna_8rap9pt_pA_PR_newcut_ptWeight.root
- AccAna_8rap9pt_pA_NP_newcut_ptWeight.root
- AccAna_6rap2pt_pA_PR_newcut_ptWeight.root
- AccAna_6rap2pt_pA_NP_newcut_ptWeight.root
- EffAna_8rap9pt_pp_PR_newcut_Zvtx0_SF1_ptWeight.root
- EffAna_8rap9pt_pp_NP_newcut_Zvtx0_SF1_ptWeight.root
- EffAna_8rap9pt_Pbp_PR_newcut_Zvtx1_SF1_ptWeight.root
- EffAna_8rap9pt_Pbp_NP_newcut_Zvtx1_SF1_ptWeight.root
- EffAna_8rap9pt_pPb_PR_newcut_Zvtx1_SF1_ptWeight.root
- EffAna_8rap9pt_pPb_NP_newcut_Zvtx1_SF1_ptWeight.root
- EffAna_6rap2pt_Pbp_PR_newcut_Zvtx1_SF1_ptWeight.root
- EffAna_6rap2pt_Pbp_NP_newcut_Zvtx1_SF1_ptWeight.root
- EffAna_6rap2pt_pPb_PR_newcut_Zvtx1_SF1_ptWeight.root
- EffAna_6rap2pt_pPb_NP_newcut_Zvtx1_SF1_ptWeight.root

