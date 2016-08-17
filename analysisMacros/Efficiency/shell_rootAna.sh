#########	(MrapNpt, isPA, accCutType, isPrompt, useZvtxWeight,useSF)

################################################
####################### Pbp
################################################
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,false,true)'
root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,true,true)' ##actual
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,false,true)'
root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,true,true)' ## actual
##### E_T dependence
root -l -b -q 'rootAna_efficiency.C+(62,1,2,true,true,true)' ##actual E_T
root -l -b -q 'rootAna_efficiency.C+(62,1,2,false,true,true)' ##actual E_T

################################################
####################### pPb
################################################
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,false,true)'
root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,true,true)' ## actual
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,false,true)'
root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,true,true)' ## actual
##### E_T dependence
root -l -b -q 'rootAna_efficiency.C+(62,2,2,true,true,true)' ## actual E_T
root -l -b -q 'rootAna_efficiency.C+(62,2,2,false,true,true)' ## actual E_T


################################################
###################### pp ( use "divideEvt" instead of this script)
################################################
####root -l -b -q 'rootAna_efficiency.C+(89,0,2,true,false,false)'
####root -l -b -q 'rootAna_efficiency.C+(89,0,2,true,false,true)' ## actual
####root -l -b -q 'rootAna_efficiency.C+(89,0,2,true,true,true)'
####root -l -b -q 'rootAna_efficiency.C+(89,0,2,false,false,false)'
####root -l -b -q 'rootAna_efficiency.C+(89,0,2,false,false,true)' ## actual
####root -l -b -q 'rootAna_efficiency.C+(89,0,2,false,true,true)'
