#########	(MrapNpt, isPA, accCutType, isPrompt, useZvtxWeight,useSF)

########## new cut
####### pp
root -l -b -q 'rootAna_efficiency_vsZvtx.C+(89,0,2,true,false,false)'
root -l -b -q 'rootAna_efficiency_vsZvtx.C+(89,0,2,false,false,false)'
####### Pbp
root -l -b -q 'rootAna_efficiency_vsZvtx.C+(89,1,2,true,false,false)'
root -l -b -q 'rootAna_efficiency_vsZvtx.C+(89,1,2,false,false,false)'
####### pPb
root -l -b -q 'rootAna_efficiency_vsZvtx.C+(89,2,2,true,false,false)'
root -l -b -q 'rootAna_efficiency_vsZvtx.C+(89,2,2,false,false,false)'

