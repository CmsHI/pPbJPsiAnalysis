###void draw_cross_pt(bool sysByHand=true, bool noPtWeight=true, bool isScale=true, bool isLog=true, int isPA = 1, bool isPrompt=true)


echo "++++++++++ cross start ++++++++++"

########################################
################## pt ##################
########################################

root -l -b <<EOF
.L draw_cross_pt.C++
.q
EOF

################## pp ##################
root -l -b -q 'draw_cross_pt.C+(0,0,1,1,0,true)' >&logfiles/cross_pt_pp_PR&
root -l -b -q 'draw_cross_pt.C+(0,0,1,1,0,false)' >&logfiles/cross_pt_pp_NP&
### no log, no scale
root -l -b -q 'draw_cross_pt.C+(0,0,0,0,0,true)' >&logfiles/cross_pt_pp_PR_nolog_noscale&
root -l -b -q 'draw_cross_pt.C+(0,0,0,0,0,false)' >&logfiles/cross_pt_pp_NP_nolog_noscale&
################## pA ##################
root -l -b -q 'draw_cross_pt.C+(0,0,1,1,1,true)' >&logfiles/cross_pt_pA_PR&
root -l -b -q 'draw_cross_pt.C+(0,0,1,1,1,false)' >&logfiles/cross_pt_pA_NP&
### no log, no scale
root -l -b -q 'draw_cross_pt.C+(0,0,0,0,1,true)' >&logfiles/cross_pt_pA_PR_nolog_noscale&
root -l -b -q 'draw_cross_pt.C+(0,0,0,0,1,false)' >&logfiles/cross_pt_pA_NP_nolog_noscale&

root -l -b <<EOF
.L draw_cross_pt_middle.C++
.q
EOF

########################################
######### for comparision ###############
########################################
#### for pPb cross
root -l -b -q 'draw_cross_pt_middle.C+(0,0,0,0,1,true)' >&logfiles/cross_pt_pA_PR_for_LHCb& 
root -l -b -q 'draw_cross_pt_middle.C+(0,0,0,0,1,false)' >&logfiles/cross_pt_pA_NP__for_LHCb&

root -l -b <<EOF
.L draw_cross_pt_integ_middle.C++
.q
EOF

root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,true,193)' >&logfiles/cross_pt_pA_PR_for_ATLAS&
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,false,193)' >&logfiles/cross_pt_pA_NP_for_ATLAS&

### for ATLAS RpPb
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,0,true,300)' >&logfiles/cross_pt_pp_PR_for_ATLAS_RpPb& 
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,0,false,300)' >&logfiles/cross_pt_pp_NP_for_ATLAS_RpPb&
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,true,300)'  >&logfiles/cross_pt_pA_PR_for_ATLAS_RpPb&
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,false,300)' >&logfiles/cross_pt_pA_NP_for_ATLAS_RpPb&
#### for B RpPb
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,0,false,386)' >&logfiles/cross_pt_pp_NP_for_Bmeson_RpPb& 
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,false,386)' >&logfiles/cross_pt_pA_NP_for_Bmeson_RpPb&


###void draw_cross_rap(bool sysByHand=true, bool noPtWeight=true, bool isScale=true, int isPA = 1, bool isPrompt=false)
########################################
################## rap ##################
########################################

root -l -b <<EOF
.L draw_cross_rap.C++
.q
EOF

################## pp ##################
root -l -b -q 'draw_cross_rap.C+(0,0,0,0,true)' >&logfiles/cross_rap_pp_PR&
root -l -b -q 'draw_cross_rap.C+(0,0,0,0,false)' >&logfiles/cross_rap_pp_NP&
################## pA ##################
root -l -b -q 'draw_cross_rap.C+(0,0,0,1,true)' >&logfiles/cross_rap_pA_PR&
root -l -b -q 'draw_cross_rap.C+(0,0,0,1,false)' >&logfiles/cross_rap_pA_NP&

echo ""
echo "cross All Done!!!!!" 
echo ""
