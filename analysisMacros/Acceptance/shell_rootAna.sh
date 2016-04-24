#int rootAna_acceptance(int MrapNpt=89, int isPA = 0, int accCutType =2, bool isPrompt=true){

##### old cut
### pp
#root -l -b -q 'rootAna_acceptance.C+(89,0,1,true)'
#root -l -b -q 'rootAna_acceptance.C+(89,0,1,false)'
root -l -b -q 'rootAna_acceptance.C+(83,0,1,true)'
root -l -b -q 'rootAna_acceptance.C+(83,0,1,false)'
#root -l -b -q 'rootAna_acceptance.C+(63,0,1,true)'
#root -l -b -q 'rootAna_acceptance.C+(63,0,1,false)'
#root -l -b -q 'rootAna_acceptance.C+(62,0,1,true)'
#root -l -b -q 'rootAna_acceptance.C+(62,0,1,false)'
### Pbp
#root -l -b -q 'rootAna_acceptance.C+(89,1,1,true)'
#root -l -b -q 'rootAna_acceptance.C+(89,1,1,false)'
root -l -b -q 'rootAna_acceptance.C+(83,1,1,true)'
root -l -b -q 'rootAna_acceptance.C+(83,1,1,false)'
#root -l -b -q 'rootAna_acceptance.C+(63,1,1,true)'
#root -l -b -q 'rootAna_acceptance.C+(63,1,1,false)'
#root -l -b -q 'rootAna_acceptance.C+(62,1,1,true)'
#root -l -b -q 'rootAna_acceptance.C+(62,1,1,false)'



##### new cut
### pp
#root -l -b -q 'rootAna_acceptance.C+(89,0,2,true)'
#root -l -b -q 'rootAna_acceptance.C+(89,0,2,false)'
root -l -b -q 'rootAna_acceptance.C+(83,0,2,true)'
root -l -b -q 'rootAna_acceptance.C+(83,0,2,false)'
#root -l -b -q 'rootAna_acceptance.C+(63,0,2,true)'
#root -l -b -q 'rootAna_acceptance.C+(63,0,2,false)'
#root -l -b -q 'rootAna_acceptance.C+(62,0,2,true)'
#root -l -b -q 'rootAna_acceptance.C+(62,0,2,false)'
### Pbp
#root -l -b -q 'rootAna_acceptance.C+(89,1,2,true)'
#root -l -b -q 'rootAna_acceptance.C+(89,1,2,false)'
root -l -b -q 'rootAna_acceptance.C+(83,1,2,true)'
root -l -b -q 'rootAna_acceptance.C+(83,1,2,false)'
#root -l -b -q 'rootAna_acceptance.C+(63,1,2,true)'
#root -l -b -q 'rootAna_acceptance.C+(63,1,2,false)'
#root -l -b -q 'rootAna_acceptance.C+(62,1,2,true)'
#root -l -b -q 'rootAna_acceptance.C+(62,1,2,false)'

