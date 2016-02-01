# General
- [SONGKYO.h], [Style.C], [Style2D.C] : macros for drawing and styling plots
- [clean.sh] : remove xxx.d and xxx.so files after running with compiling e.g.) root -l xxx.C+

# Calculate acceptance for each J/psi (pT-y) bin and save as root files
- [shell_rootAna.sh] : shell script to run [rootAna_acceptance.C] and [rootAna_acceptance_PAS.C]
- [rootAna_acceptance.C] : calculate acceptance from TTrees
    - OPT mrapnpt (89,83,63,62) : m= # of rap bins, n = # of pT bins
    - OPT isPrompt (true or false) : prompt or non-prompt MC samples
    - Pbp and pPb are not obtained separately, unlike efficiencies
    - output : [AccAnnas_BINNING_MCSAMPLE_boosted.root] 
- [rootAna_acceptance_PAS.C] : old acc cuts for 2015 PAS
    - output : [AccAnnas_BINNING_MCSAMPLE_boosted_PAS.root]

# Draw 1D and 2D plots
- [shell_draw_1D.sh] : shell script to run draw_1D_acc.C
- [draw_1D_acc.C] : draw pdfs and save roots of Acceptance vs pT for each rapidity bin (in lab frame)
    - input : [AccAnnas_BINNING_MCSAMPLE_boosted.root] 
    - output : dir_acc/xxx_AccPt.pdf and xxx_AccPt.root
- [draw_1D_acc_PAS.C] : old acc cuts for 2015 PAS
    - input : [AccAnnas_BINNING_MCSAMPLE_boosted_PAS.root]
    - output : dir_acc_PAS
- [draw_1D_acc_comp.C] : compare two accpetance values from  
    - input : [dir_acc/xxx_AccPt.root] and [dir_acc_PAS/xxx_AccPt.root]
    - output : [dir_acc_comp_NEW_PAS]
- [drwa_2D_acc.C] : draw 2D plots with analysis binning
    - input : [AccAnnas_BINNING_MCSAMPLE_boosted.root] 
    - output : dir_acc/h2D_xxx.pdf
- [drwa_2D_acc_fine.C] : draw 2D plots with fine grained bins (for AN)
    - input : [AccAnnas_BINNING_MCSAMPLE_boosted.root] 
    - output : dir_acc/h2D_xxx_fine.pdf

