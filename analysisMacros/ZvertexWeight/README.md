# General
- [SONGKYO.h], [Style.C], [Style2D.C] : macros for drawing and styling plots
- [clean.sh] : remove xx.d and xx.so xx.pcm files after running with compiling e.g.) root -l xxx.C+

# STEP 1)
- ./shell_zVtxFit.sh to run rootAna_zVtxFit_G1.C
- Output files in [zVtx201604] : (XXX: pp, Pbp, pPb)
  - zVtx_G1_XXX_isPrompt1_isNewAccCut1_isOfficial1.root
  - zVtx_G1_XXX_isPrompt0_isNewAccCut1_isOfficial1.root
  - zVtxFit_G1_XXX_isPrompt1_isNewAccCut1_isOfficial1.pdf
  - zVtxFit_G1_XXX_isPrompt0_isNewAccCut1_isOfficial1.pdf
  - zVtxFitRatio_G1_XXX_isPrompt1_isNewAccCut1_isOfficial1.pdf
  - zVtxFitRatio_G1_XXX_isPrompt0_isNewAccCut1_isOfficial1.pdf

# STEP 2) 
- !!! only for re-draw pdf files !!! 
- ./shell_draw_zvtx.sh to run draw_zvtx.C
- Output files in [zVtx201604] : (XXX: pp, Pbp, pPb)
  - zVtxFit_G1_XXX_isPrompt1_isNewAccCut1_isOfficial1.pdf
  - zVtxFit_G1_XXX_isPrompt0_isNewAccCut1_isOfficial1.pdf
  - zVtxFitRatio_G1_XXX_isPrompt1_isNewAccCut1_isOfficial1.pdf
  - zVtxFitRatio_G1_XXX_isPrompt0_isNewAccCut1_isOfficial1.pdf

