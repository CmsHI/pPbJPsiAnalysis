###void make_EffAna_fromMerged(int MrapNpt=89, int isPA = 0, int accCutType =2, bool isPrompt = true, bool useZvtxWeight=false, bool useSF=false)

### pp prompt
root -l -b -q 'make_EffAna_fromMerged.C(89,0,2,true,false,true)' #actual
### pp non-prompt
root -l -b -q 'make_EffAna_fromMerged.C(89,0,2,false,false,true)' #actual
### Pbp non-prompt
root -l -b -q 'make_EffAna_fromMerged.C(89,1,2,false,true,true)' #actual
root -l -b -q 'make_EffAna_fromMerged.C(62,1,2,false,true,true)' #actual E_T
### pPb non-prompt
root -l -b -q 'make_EffAna_fromMerged.C(89,2,2,false,true,true)' #actual
root -l -b -q 'make_EffAna_fromMerged.C(62,2,2,false,true,true)' #actual E_T
