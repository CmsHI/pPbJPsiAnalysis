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

########################################
######### for comparision ###############
########################################

#root -l -b <<EOF
#.L draw_cross_pt_middle.C++
#.q
#EOF

### for LHCb cross
#root -l -b -q 'draw_cross_pt_middle.C+(0,0,0,0,1,true)' 
#root -l -b -q 'draw_cross_pt_middle.C+(0,0,0,0,1,false)' 

root -l -b <<EOF
.L draw_cross_pt_integ_middle.C++
.q
EOF

### for ATLAS cross
#root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,true,193)' 
#root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,false,193)' 

### for ATLAS RpPb
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,0,true,300)' 
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,0,false,300)' 
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,true,300)'  
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,false,300)' 
#### for B RpPb
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,0,false,386)' 
root -l -b -q 'draw_cross_pt_integ_middle.C+(0,0,0,0,1,false,386)' 


###void draw_cross_rap(bool sysByHand=true, bool noPtWeight=true, bool isScale=true, int isPA = 1, bool isPrompt=false)
########################################
################## rap ##################
########################################

root -l -b <<EOF
.L draw_cross_rap.C++
.q
EOF

################## pp ##################
root -l -b -q 'draw_cross_rap.C+(0,0,0,0,true)' 
root -l -b -q 'draw_cross_rap.C+(0,0,0,0,false)' 
################## pA ##################
root -l -b -q 'draw_cross_rap.C+(0,0,0,1,true)' 
root -l -b -q 'draw_cross_rap.C+(0,0,0,1,false)' 

echo ""
echo "cross All Done!!!!!" 
echo ""
