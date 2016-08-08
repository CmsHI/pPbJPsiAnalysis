#########	(MrapNpt, isPA, accCutType, isPrompt, useZvtxWeight,useSF)

####### pp
root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,0,2,true,false,true)' #actual
root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,0,2,false,false,true)' #actual
####### Pbp
root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,1,2,true,true,true)' #actual
root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,1,2,false,true,true)' #actual
####### pPb
root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,2,2,true,true,true)' #actual
root -l -b -q 'rootAna_efficiency_ptWeight.C+(89,2,2,false,true,true)' #actual

### for ET dependence
####### Pbp
root -l -b -q 'rootAna_efficiency_ptWeight.C+(62,1,2,true,true,true)' #actual
root -l -b -q 'rootAna_efficiency_ptWeight.C+(62,1,2,false,true,true)' #actual
####### pPb
root -l -b -q 'rootAna_efficiency_ptWeight.C+(62,2,2,true,true,true)' #actual
root -l -b -q 'rootAna_efficiency_ptWeight.C+(62,2,2,false,true,true)' #actual
