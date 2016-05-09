###int rootAna_zVtxFit_G1(TString szFunction = "G1", int isPA = 1, bool isPrompt = true, bool isNewAccCut = true, bool isOfficial = true){
### pp
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",0,true,true,true)'
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",0,true,false,true)'
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",0,false,true,true)'
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",0,false,false,true)'
### Pbp
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",1,true,true,true)'
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",1,true,false,true)'
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",1,false,true,true)'
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",1,false,false,true)'
### pPb
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",2,true,true,true)'
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",2,true,false,true)'
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",2,false,true,true)'
root -l -b -q 'rootAna_zVtxFit_G1.C+("G1",2,false,false,true)'
