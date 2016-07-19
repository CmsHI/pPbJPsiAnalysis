#########	(MrapNpt, isPA, accCutType, isPrompt, useZvtxWeight,useSF)

########## new cut
####### pp
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,0,2,true,false,false)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,0,2,true,false,true)'
root -l -b -q 'rootAna_efficiency_ptWeight.C+(62,0,2,true,false,true)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,0,2,false,false,false)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,0,2,false,false,true)'
root -l -b -q 'rootAna_efficiency_ptWeight.C+(62,0,2,false,false,true)'
####### Pbp
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,1,2,true,false,false)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,1,2,true,true,false)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,1,2,true,false,true)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,1,2,true,true,true)'
root -l -b -q 'rootAna_efficiency_ptWeight.C+(62,1,2,true,true,true)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,1,2,false,false,false)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,1,2,false,true,false)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,1,2,false,false,true)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,1,2,false,true,true)'
root -l -b -q 'rootAna_efficiency_ptWeight.C+(62,1,2,false,true,true)'
####### pPb
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,2,2,true,false,false)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,2,2,true,true,false)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,2,2,true,false,true)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,2,2,true,true,true)'
root -l -b -q 'rootAna_efficiency_ptWeight.C+(62,2,2,true,true,true)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,2,2,false,false,false)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,2,2,false,true,false)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,2,2,false,false,true)'
#root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,2,2,false,true,true)'
root -l -b -q 'rootAna_efficiency_ptWeight.C+(62,2,2,false,true,true)'

