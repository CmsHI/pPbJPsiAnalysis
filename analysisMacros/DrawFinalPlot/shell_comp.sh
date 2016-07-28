#### void comp_cross_pt_LHCb(bool isShift=false, bool isPrompt = true, bool isLog=true)
###void comp_cross_pt_ATLAS(bool shifted=false, bool isPrompt = true, bool isLog=true)

######### for comparision ###############
### for LHCb
#root -l 'comp_cross_pt_LHCb.C+(0,true,true)'
#root -l 'comp_cross_pt_LHCb.C+(0,false,true)'
#root -l 'comp_cross_pt_LHCb.C+(0,true,false)'
#root -l 'comp_cross_pt_LHCb.C+(0,false,false)'
### for ATLAS
root -l 'comp_cross_pt_ATLAS.C+(0,true,true)'
root -l 'comp_cross_pt_ATLAS.C+(0,false,true)'
root -l 'comp_cross_pt_ATLAS.C+(0,true,false)'
root -l 'comp_cross_pt_ATLAS.C+(0,false,false)'
