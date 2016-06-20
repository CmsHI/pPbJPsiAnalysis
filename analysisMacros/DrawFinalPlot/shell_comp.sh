###void comp_cross_pt_LHCb(bool isShift=false, bool isPrompt = true, bool isLog=true)

######### for comparision ###############
### no log, no scale (middle for LHCb, integ_middle for ATLAS)
root -l 'comp_cross_pt_LHCb.C+(0,true,true)'
root -l 'comp_cross_pt_LHCb.C+(0,false,true)'
root -l 'comp_cross_pt_LHCb.C+(0,true,false)'
root -l 'comp_cross_pt_LHCb.C+(0,false,false)'
