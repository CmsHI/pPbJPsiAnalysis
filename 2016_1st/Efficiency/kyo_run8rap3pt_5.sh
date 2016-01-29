### no cterr, no TNP, no zvtx
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",1,1,0,0,0)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",1,0,0,0,0)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",0,1,0,0,0)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",0,0,0,0,0)'
### no TNP, no zvtx
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",1,1,1,0,0)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",1,0,1,0,0)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",0,1,1,0,0)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",0,0,1,0,0)'
### no TNP
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",1,1,1,0,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",1,0,1,0,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",0,1,1,0,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",0,0,1,0,1)'
### actual 8rap3pt
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",1,1,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",1,0,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",0,1,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt.C+("8rap3pt",0,0,1,1,1)'
####################################################################
### check1 newacc_neweta
#root -l -b -q 'rootAna_efficiency_8rap3pt_check1_newacc_neweta.C+("8rap3pt",1,1,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check1_newacc_neweta.C+("8rap3pt",1,0,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check1_newacc_neweta.C+("8rap3pt",0,1,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check1_newacc_neweta.C+("8rap3pt",0,0,1,1,1)'
### check2 oldacc_neweta
#root -l -b -q 'rootAna_efficiency_8rap3pt_check2_oldacc_neweta.C+("8rap3pt",1,1,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check2_oldacc_neweta.C+("8rap3pt",1,0,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check2_oldacc_neweta.C+("8rap3pt",0,1,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check2_oldacc_neweta.C+("8rap3pt",0,0,1,1,1)'
### check3 newacc_oldeta
#root -l -b -q 'rootAna_efficiency_8rap3pt_check3_newacc_oldeta.C+("8rap3pt",1,1,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check3_newacc_oldeta.C+("8rap3pt",1,0,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check3_newacc_oldeta.C+("8rap3pt",0,1,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check3_newacc_oldeta.C+("8rap3pt",0,0,1,1,1)'
### check4 oldacc_oldeta
#root -l -b -q 'rootAna_efficiency_8rap3pt_check4_oldacc_oldeta.C+("8rap3pt",1,1,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check4_oldacc_oldeta.C+("8rap3pt",1,0,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check4_oldacc_oldeta.C+("8rap3pt",0,1,1,1,1)'
#root -l -b -q 'rootAna_efficiency_8rap3pt_check4_oldacc_oldeta.C+("8rap3pt",0,0,1,1,1)'
### check5 oldacc_oldeta_private
root -l -b -q 'rootAna_efficiency_8rap3pt_check5_oldacc_oldeta_private.C+("8rap3pt",1,1,1,1,1)'
root -l -b -q 'rootAna_efficiency_8rap3pt_check5_oldacc_oldeta_private.C+("8rap3pt",1,0,1,1,1)'
root -l -b -q 'rootAna_efficiency_8rap3pt_check5_oldacc_oldeta_private.C+("8rap3pt",0,1,1,1,1)'
root -l -b -q 'rootAna_efficiency_8rap3pt_check5_oldacc_oldeta_private.C+("8rap3pt",0,0,1,1,1)'

