##void draw_RFB_pt(bool sysByHand=true, bool noPtWeight=true, bool isPrompt=true)
###void draw_RFB_rap(bool sysByHand=true,  bool noPtWeight=true, bool isPrompt = true)
#void draw_RFB_ethf(bool sysByHand=true, bool noPtWeight = false, bool isPrompt=false)

echo "++++++++++ RFB start ++++++++++"

root -l -b <<EOF
.L draw_RFB_pt.C++
.q
EOF
root -l -b -q 'draw_RFB_pt.C+(1,0,true)' 
root -l -b -q 'draw_RFB_pt.C+(1,0,false)' 

root -l -b <<EOF
.L draw_RFB_pt_mergedBin.C++
.q
EOF
root -l -b -q 'draw_RFB_pt_mergedBin.C+(1,0,true)'
root -l -b -q 'draw_RFB_pt_mergedBin.C+(1,0,false)'

root -l -b <<EOF
.L draw_RFB_rap.C++
.q
EOF
root -l -b -q 'draw_RFB_rap.C+(1,0,true)' 
root -l -b -q 'draw_RFB_rap.C+(1,0,false)' 

root -l -b <<EOF
.L draw_RFB_ethf.C++
.q
EOF
root -l -b -q 'draw_RFB_ethf.C+(1,0,true)' 
root -l -b -q 'draw_RFB_ethf.C+(1,0,false)' 

echo ""
echo "RFB All Done!!!!!" 
echo ""

