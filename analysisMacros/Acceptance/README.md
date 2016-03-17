# General
- [SONGKYO.h], [Style.C], [Style2D.C] : macros for drawing and styling plots
- [clean.sh] : remove xxx.d and xxx.so files after running with compiling e.g.) root -l xxx.C+

# Calculate acceptance for each J/psi (pT-y) bin and save as root files
- [shell_rootAna.sh] : shell script to run [rootAna_acceptance.C] and [rootAna_acceptance_PAS.C]
- [rootAna_acceptance.C] : calculate acceptance from TTrees (pp, pA both)
- [rootAna_acceptance_nonOnia_pA.C] : input files are non-onia trees from HC

# Draw 1D and 2D plots
- [shell_draw_1D.sh] : shell script to run draw_1D_acc.C
- [draw_1D_acc.C] : draw pdfs and save roots of Acceptance vs pT for each rapidity bin (in lab frame)
    - input : [AccAna_xxx.root] 
    - output : dir_acc/xxx_AccPt.pdf and xxx_AccPt.root
- [draw_1D_acc_comp.C] : compare two accpetance values from  
    - input : [dir_acc/xxx_AccPt.root] 
    - output : [dir_acc_comp]
- [drwa_2D_acc.C] : draw 2D plots with analysis binning
    - input : [AccAna_xxx.root] 
    - output : dir_acc/h2D_xxx.pdf
- [drwa_2D_acc_fine.C] : draw 2D plots with fine grained bins (for AN)
    - input : [AccAna_xxx.root] 
    - output : dir_acc/h2D_xxx_fine.pdf

