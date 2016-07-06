# General
- [SONGKYO.h], [Style.C], [Style2D.C] : macros for drawing and styling plots
- [clean.sh] : remove xx.d and xx.so xx.pcm files after running with compiling e.g.) root -l xxx.C+

# Calculate Acceptance for J/psi (pT-y)
- STEP 1 : draw distributions for signals, backgrounds and MC 
    - [shell_rootAna.sh] : shell script to run [rootAna_muIDvariables.C]
    - [shell_rootAna_condor.sh] : shell script to run [rootAna_muIDvariables.C] using Condor in KUNPL
- STEP 2 : background subtractions
    - run [draw_bkgSubtracted.C] for szPA=pp and pa
    - plots will be savea in [dir_muID]

