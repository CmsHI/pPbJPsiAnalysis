#########	(MrapNpt, isPA, accCutType, isPrompt, useZvtxWeight,useSF)

########## new cut
####### pp
#root -l -b -q 'rootAna_efficiency.C+(89,0,2,true,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,0,2,true,false,true)' ## actual
#root -l -b -q 'rootAna_efficiency.C+(89,0,2,true,true,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,0,2,false,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,0,2,false,false,true)' ## actual
#root -l -b -q 'rootAna_efficiency.C+(89,0,2,false,true,true)'
####### Pbp
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,true,true,true)' ##actual
#root -l -b -q 'rootAna_efficiency.C+(62,1,2,true,true,true)' ##actual ET
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,1,2,false,true,true)' ## actual
#root -l -b -q 'rootAna_efficiency.C+(62,1,2,false,true,true)' ##actual ET
####### pPb
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,true,true,true)' ## actual
#root -l -b -q 'rootAna_efficiency.C+(62,2,2,true,true,true)' ## actual ET
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,false,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,true,false)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,false,true)'
#root -l -b -q 'rootAna_efficiency.C+(89,2,2,false,true,true)' ## actual
#root -l -b -q 'rootAna_efficiency.C+(62,2,2,false,true,true)' ## actual ET


####################################################################
###########int rootAna_efficiency_divideEvt(int MrapNpt = 89, int isPA =0, int accCutType = 2, bool isPrompt = true, bool useZvtxWeight =false, bool useSF=true, int=initev=0. int nevt=10000000)
####################################################################
########## divide events to be merged later
root -l -b <<EOF
.L rootAna_efficiency_divideEvt.C++
.q
EOF
####### pp prompt (total 45005396)
#root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,true,false,true,0,10000000)' ## actual
#root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,true,false,true,10000001,20000000)' ## actual
#root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,true,false,true,20000001,30000000)' ## actual
#root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,true,false,true,30000001,40000000)' ## actual
#root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,true,false,true,40000001,45005396)' ## actual

####### pp non-prompt (total 53547109) 
#root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,false,false,true,0,10000000)' ## actual
#root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,false,false,true,10000001,20000000)' ## actual
#root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,false,false,true,20000001,30000000)' ## actual
root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,false,false,true,30000001,39113286)' ## actual
#root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,false,false,true,39130000,40000000)' ## actual
root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,false,false,true,40000001,45310562)' ## actual
#root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,false,false,true,45350000,50000000)' ## actual
#root -l -b -q 'rootAna_efficiency_divideEvt.C+(89,0,2,false,false,true,50000001,53547109)' ## actual

