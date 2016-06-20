#########	(MrapNpt, isPA, accCutType, isPrompt, useZvtxWeight,useSF)

########## old cut
####### pp
#root -l -b -q 'rootAna_efficiency.C+(89,0,1,true,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,0,1,true,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,0,1,true,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,0,1,true,true,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,0,1,false,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,0,1,false,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,0,1,false,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,0,1,false,true,true)'
####### Pbp
#root -l -b -q 'rootAna_efficiency.C+(89,1,1,true,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,1,true,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,1,true,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,1,true,true,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,1,false,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,1,false,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,1,false,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,1,false,true,true)'
####### pPb
#root -l -b -q 'rootAna_efficiency.C+(89,2,1,true,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,1,true,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,1,true,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,1,true,true,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,1,false,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,1,false,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,1,false,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,1,false,true,true)'


########## new cut
####### pp
#root -l -b -q 'rootAna_efficiency.C+(89,0,2,true,false,false)'
root -l -b -q 'rootAna_efficiency.C+(89,0,2,true,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,0,2,false,false,false)'
root -l -b -q 'rootAna_efficiency.C+(89,0,2,false,false,true)'
####### Pbp
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,true,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,true,true)'
####### pPb
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,true,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,true,true)'

