#### void comp_cross_pt_LHCb(bool isShift=false, bool isPrompt = true, bool isLog=true)
###void comp_cross_pt_ATLAS(bool shifted=false, bool isPrompt = true, bool isLog=true)

####### cross ##########
### for LHCb
root -l -b -q 'comp_cross_pt_LHCb.C+(0,true,true)' >&logfiles/comp_cross_PR_pt_LHCb&
root -l -b -q 'comp_cross_pt_LHCb.C+(0,false,true)' >&logfiles/comp_cross_NP_pt_LHCb&
root -l -b -q 'comp_cross_pt_LHCb.C+(0,true,false)' >&logfiles/comp_cross_PR_pt_LHCb_nolog&  
root -l -b -q 'comp_cross_pt_LHCb.C+(0,false,false)' >&logfiles/comp_cross_NP_pt_LHCb_nolog&
### for ATLAS
root -l -b -q 'comp_cross_pt_ATLAS.C+(0,true,true)' >&logfiles/comp_cross_PR_pt_ATLAS&
root -l -b -q 'comp_cross_pt_ATLAS.C+(0,false,true)' >&logfiles/comp_cross_NP_pt_ATLAS&
root -l -b -q 'comp_cross_pt_ATLAS.C+(0,true,false)' >&logfiles/comp_cross_PR_pt_ATLAS_nolog&
root -l -b -q 'comp_cross_pt_ATLAS.C+(0,false,false)' >&logfiles/comp_cross_NP_pt_ATLAS_nolog&

####### RFB ##########
root -l -b -q comp_RFB_pt_LHCb_ALICE_ATLAS.C+ >&logfiles/comp_RFB_pt_LHCb_ALICE_ATLAS&
root -l -b -q comp_RFB_rap_LHCb_ALICE_ATLAS.C+ >&logfiles/comp_RFB_rap_LHCb_ALICE_ATLAS&

###### RpPb ######
root -l -b -q 'comp_RpPb_pt_ATLAS.C+(true)' >&logfiles/comp_RpPb_PR_pt_ATLAS&
root -l -b -q 'comp_RpPb_pt_ATLAS.C+(false)' >&logfiles/comp_RpPb_NP_pt_ATLAS&
root -l -b -q 'comp_RpPb_rap_ATLAS.C+(true)' >&logfiles/comp_RpPb_PR_rap_ATLAS&
root -l -b -q 'comp_RpPb_rap_ATLAS.C+(false)' >&logfiles/comp_RpPb_NP_rap_ATLAS&
root -l -b -q 'comp_RpPb_pt_Bmeson.C+(false)' >&logfiles/comp_RpPb_NP_pt_Bmeson&
root -l -b -q 'comp_RpPb_rap_Bmeson.C+(false)' >&logfiles/comp_RpPb_NP_rap_Bmeson&
root -l -b -q 'comp_RpPb_pt_ALICE.C+(true)' >&logfiles/comp_RpPb_PR_pt_ALICE&

