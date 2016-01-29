#mkdir draw_compRatio_two_cutG
root -l draw_acc_compRatio_two.C+
root -l draw_eff_compRatio_two.C+
root -l draw_acceff_compRatio_two.C+
root -l draw_rawYield_compRatio_two.C+
root -l 'draw_corrYield_compRatio_two.C+(true, true, "draw_compRatio_two_cutG", true, false)'
root -l 'draw_corrYield_compRatio_two.C+(true, true, "draw_compRatio_two_cutG", true, true)'
