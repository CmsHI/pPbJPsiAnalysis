### no zvtx, no TNP
#root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",1,1,0,1,0,0,0)'
#root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",1,0,0,1,0,0,0)'
#root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",0,1,0,1,0,0,0)'
#root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",0,0,0,1,0,0,0)'
### no TNP
root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",1,1,0,1,0,0,1)'
root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",1,0,0,1,0,0,1)'
root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",0,1,0,1,0,0,1)'
root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",0,0,0,1,0,0,1)'
### actual
root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",1,1,0,1,1,0,1)'
root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",1,0,0,1,1,0,1)'
root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",0,1,0,1,1,0,1)'
root -l -b -q 'rootAna_efficiency_counting_hist.C+("8rap9pt2gev",0,0,0,1,1,0,1)'
