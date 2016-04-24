# General
- [SONGKYO.h], [Style.C], [Style2D.C] : macros for drawing and styling plots
- [clean.sh] : remove xx.d and xx.so xx.pcm files after running with compiling e.g.) root -l xxx.C+

# Calculate Acceptance for J/psi (pT-y)
- [shell_rootAna.sh] : shell script to run [rootAna_acceptance.C] and [rootAna_acceptance_PAS.C]
- [rootAna_acceptance.C] : calculate acceptance from oniaTrees
- [rootAna_acceptance_nonOnia_pA.C] : calculate acceptance from non-oniaTrees (pA only, not used now)
    - Pbp and pPb are obtained at the same time (isPA==1)

# Draw 1D and 2D plots
- [shell_draw_1D.sh] : shell script to run draw_1D_acc.C
- [draw_1D_acc.C] : draw Acceptance vs pT for each y_lab and save as a root file
    - input : [AccAna_BINNING_MCSAMPLE_boosted.root] 
    - output : dir_acc/xxx_AccPt.pdf and xxx_AccPt.root
- [draw_1D_acc_comp.C] : compare two accpetance values from  
    - input : [dir_acc/xxx_AccPt.root] and [dir_acc_PAS/xxx_AccPt.root]
    - output : [dir_acc_comp_NEW_PAS]
- [drwa_2D_acc.C] : draw 2D plots with analysis binning
    - input : [AccAna_BINNING_MCSAMPLE_boosted.root] 
    - output : dir_acc/h2D_xxx.pdf
- [drwa_2D_acc_fine.C] : draw 2D plots with fine grained bins (for AN)
    - input : [AccAna_BINNING_MCSAMPLE_boosted.root] 
    - output : dir_acc/h2D_xxx_fine.pdf

