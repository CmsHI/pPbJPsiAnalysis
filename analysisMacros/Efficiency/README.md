# Pbp and pPb Same as README in [Accetance]
  ./shell_rootAna.sh

# pp 
  ./shell_rootAna_divideEt_STEP1.sh : run [rootAna_efficiency_divideEvt.C] and outputs in [toBeMerged_divideEvt]
  ./shell_hadd_STEP2.sh : add root files and create [merged***.root] in [toBeMerged_divideEvt]
  ./ : calculate correct efficiency with the [merged***.root] and make actual [***.root]

# Final Output Files
- EffAna_8rap9pt_pp_PR_newcut_Zvtx0_SF1.root
- EffAna_8rap9pt_pp_NP_newcut_Zvtx0_SF1.root
- EffAna_8rap9pt_Pbp_PR_newcut_Zvtx1_SF1.root
- EffAna_8rap9pt_Pbp_NP_newcut_Zvtx1_SF1.root
- EffAna_8rap9pt_pPb_PR_newcut_Zvtx1_SF1.root
- EffAna_8rap9pt_pPb_NP_newcut_Zvtx1_SF1.root
- EffAna_6rap2pt_Pbp_PR_newcut_Zvtx1_SF1.root
- EffAna_6rap2pt_Pbp_NP_newcut_Zvtx1_SF1.root
- EffAna_6rap2pt_pPb_PR_newcut_Zvtx1_SF1.root
- EffAna_6rap2pt_pPb_NP_newcut_Zvtx1_SF1.root
- ... and others for cross-check

