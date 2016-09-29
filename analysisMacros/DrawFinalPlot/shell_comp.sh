#### void comp_cross_pt_LHCb(bool isShift=false, bool isPrompt = true, bool isLog=true)
###void comp_cross_pt_ATLAS(bool shifted=false, bool isPrompt = true, bool isLog=true)

####### cross ##########
### for LHCb
#root -l -b -q 'comp_cross_pt_LHCb.C+(0,true,true)'
#root -l -b -q 'comp_cross_pt_LHCb.C+(0,false,true)'
#root -l -b -q 'comp_cross_pt_LHCb.C+(0,true,false)'
#root -l -b -q 'comp_cross_pt_LHCb.C+(0,false,false)'
### for ATLAS
#root -l -b -q 'comp_cross_pt_ATLAS.C+(0,true,true)'
#root -l -b -q 'comp_cross_pt_ATLAS.C+(0,false,true)'
#root -l -b -q 'comp_cross_pt_ATLAS.C+(0,true,false)'
#root -l -b -q 'comp_cross_pt_ATLAS.C+(0,false,false)'

####### RFB ##########
root -l -b -q comp_RFB_pt_LHCb_ALICE_ATLAS.C+
root -l -b -q comp_RFB_rap_LHCb_ALICE_ATLAS.C+

###### RpPb ######
root -l -b -q 'comp_RpPb_pt_ATLAS.C+(true)'
root -l -b -q 'comp_RpPb_pt_ATLAS.C+(false)'
root -l -b -q 'comp_RpPb_rap_ATLAS.C+(true)'
root -l -b -q 'comp_RpPb_rap_ATLAS.C+(false)'
root -l -b -q 'comp_RpPb_pt_Bmeson.C+(false)'
root -l -b -q 'comp_RpPb_rap_Bmeson.C+(false)'
root -l -b -q 'comp_RpPb_pt_ALICE.C+(true)'

