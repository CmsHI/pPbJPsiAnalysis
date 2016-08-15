###void make_EffAna_fromMerged(int MrapNpt=89, int isPA = 0, int accCutType =2, bool isPrompt = true, bool useZvtxWeight=false, bool useSF=false)

### pp
root -l -b -q 'make_EffAna_fromMerged.C(89,0,2,true,false,false)'
#root -l -b -q 'make_EffAna_fromMerged.C(89,0,2,true,false,true)' #actual
root -l -b -q 'make_EffAna_fromMerged.C(89,0,2,false,false,false)'
#root -l -b -q 'make_EffAna_fromMerged.C(89,0,2,false,false,true)' #actual
