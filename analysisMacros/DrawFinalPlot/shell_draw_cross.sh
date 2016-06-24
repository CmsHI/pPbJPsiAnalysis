###void draw_cross_pt(bool sysByHand=true, bool noPtWeight=true, bool isScale=true, bool isLog=true, int isPA = 1, bool isPrompt=false)
###void draw_cross_pt_integ_middle(bool sysByHand=true, bool noPtWeight=true, bool isScale=true, bool isLog=true, int isPA = 1, bool isPrompt=false)
###void draw_cross_rap(bool sysByHand=true, bool noPtWeight=true, bool isScale=false, int isPA = 1, bool isPrompt=false)

########################################
################## pt ##################
########################################

################## pp ##################
#root -l 'draw_cross_pt.C+(1,1,1,1,0,true)'
#root -l 'draw_cross_pt.C+(1,1,1,1,0,false)'
### no log, no scale
#root -l 'draw_cross_pt.C+(1,1,0,0,0,true)'
#root -l 'draw_cross_pt.C+(1,1,0,0,0,false)'
################## pA ##################
#root -l 'draw_cross_pt.C+(1,1,1,1,1,true)'
#root -l 'draw_cross_pt.C+(1,1,1,1,1,false)'
### no log, no scale
#root -l 'draw_cross_pt.C+(1,1,0,0,1,true)'
#root -l 'draw_cross_pt.C+(1,1,0,0,1,false)'
######### for comparision ###############
### no log, no scale (middle for LHCb, integ_middle for ATLAS)
#root -l 'draw_cross_pt_middle.C+(1,1,0,0,1,true)'
#root -l 'draw_cross_pt_middle.C+(1,1,0,0,1,false)'
#root -l 'draw_cross_pt_integ_middle.C+(1,1,0,0,1,true)'
#root -l 'draw_cross_pt_integ_middle.C+(1,1,0,0,1,false)'

########################################
################## rap ##################
########################################

################## pp ##################
root -l 'draw_cross_rap.C+(1,1,0,0,true)'
#root -l 'draw_cross_rap.C+(1,1,0,0,false)'
################## pA ##################
root -l 'draw_cross_rap.C+(1,1,0,1,true)'
#root -l 'draw_cross_rap.C+(1,1,0,1,false)'




