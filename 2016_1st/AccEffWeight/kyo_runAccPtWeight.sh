#(rapptbin, isPropmt, isStiff)
##8rap9pt
root -l -b -q 'rootAna_acceptance_ptWeight.C+(89,1,1)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(89,0,1)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(89,1,0)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(89,0,0)'
##8rap3pt
root -l -b -q 'rootAna_acceptance_ptWeight.C+(83,1,1)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(83,0,1)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(83,1,0)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(83,0,0)'
##6rap3pt
root -l -b -q 'rootAna_acceptance_ptWeight.C+(63,1,1)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(63,0,1)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(63,1,0)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(63,0,0)'
##6rap2pt
root -l -b -q 'rootAna_acceptance_ptWeight.C+(62,1,1)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(62,0,1)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(62,1,0)'
root -l -b -q 'rootAna_acceptance_ptWeight.C+(62,0,0)'

