###void draw_cross_pt(bool sysByHand=true, bool noPtWeight=true, bool isScale=true, bool isLog=true, int isPA = 1, bool isPrompt=true)
###void draw_cross_pt_integ_middle(bool sysByHand=true, bool noPtWeight=true, bool isScale=true, bool isLog=true, int isPA = 1, bool isPrompt=true)
###void draw_cross_rap(bool sysByHand=true, bool noPtWeight=true, bool isScale=true, int isPA = 1, bool isPrompt=false)

########################################
################## pt ##################
########################################

################## pp ##################
root -l -b -q 'draw_cross_pt.C+(0,0,1,1,0,true)'
root -l -b -q 'draw_cross_pt.C+(0,0,1,1,0,false)'
### no log, no scale
root -l -b -q 'draw_cross_pt.C+(0,0,0,0,0,true)'
root -l -b -q 'draw_cross_pt.C+(0,0,0,0,0,false)'
################## pA ##################
root -l -b -q 'draw_cross_pt.C+(0,0,1,1,1,true)'
root -l -b -q 'draw_cross_pt.C+(0,0,1,1,1,false)'
### no log, no scale
root -l -b -q 'draw_cross_pt.C+(0,0,0,0,1,true)'
root -l -b -q 'draw_cross_pt.C+(0,0,0,0,1,false)'
######### for comparision ###############
### no log, no scale (middle for LHCb, integ_middle for ATLAS - 193 for cross, 150 for RpPb)
#### for pPb cross
root -l -b -q 'draw_cross_pt_middle.C+(0,0,0,0,1,true)' ## for LHCb cross
root -l -b -q 'draw_cross_pt_middle.C+(0,0,0,0,1,false)'
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,true,193)' ### for ATLAS cross
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,false,193)'
### for ATLAS RpPb
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,0,true,300)' ### pp for ATLAS RpPb
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,0,false,300)'
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,true,300)' ### pPb for ATLAS RpPb
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,false,300)'
#### for B RpPb
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,0,false,386)' ### pp for B RpPb (non-prompt only)
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,false,386)' ### pPb for B RpPb (non-prompt only)


###void draw_cross_rap(bool sysByHand=true, bool noPtWeight=true, bool isScale=true, int isPA = 1, bool isPrompt=false)
########################################
################## rap ##################
########################################

################## pp ##################
root -l -b -q 'draw_cross_rap.C+(0,0,0,0,true)'
root -l -b -q 'draw_cross_rap.C+(0,0,0,0,false)'
################## pA ##################
root -l -b -q 'draw_cross_rap.C+(0,0,0,1,true)'
root -l -b -q 'draw_cross_rap.C+(0,0,0,1,false)'




