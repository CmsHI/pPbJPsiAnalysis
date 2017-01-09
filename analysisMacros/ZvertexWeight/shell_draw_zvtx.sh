###int draw_zvtx(TString szFunction = "G1", int isPA = 1, bool isPrompt = true, bool isNewAccCut = true, bool isOfficial = true){
### pp
root -l -b -q 'draw_zvtx.C+("G1",0,true,true,true)'
#root -l -b -q 'draw_zvtx.C+("G1",0,false,true,true)'
### Pbp
root -l -b -q 'draw_zvtx.C+("G1",1,true,true,true)'
#root -l -b -q 'draw_zvtx.C+("G1",1,false,true,true)'
### pPb
root -l -b -q 'draw_zvtx.C+("G1",2,true,true,true)'
#root -l -b -q 'draw_zvtx.C+("G1",2,false,true,true)'
