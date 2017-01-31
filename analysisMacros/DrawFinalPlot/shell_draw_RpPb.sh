##void draw_RpPb_pt(bool sysByHand=true, bool noPtWeight=true, bool isPrompt=true)
###void draw_RpPb_rap(bool sysByHand=true,  bool noPtWeight=true, bool isPrompt = true)


echo "++++++++++ RpPb start ++++++++++"

root -l -b <<EOF
.L draw_RpPb_pt.C++
.q
EOF
root -l -b -q 'draw_RpPb_pt.C+(0,0,true)' 
root -l -b -q 'draw_RpPb_pt.C+(0,0,false)' 

root -l -b <<EOF
.L draw_RpPb_rap.C++
.q
EOF
root -l -b -q 'draw_RpPb_rap.C+(0,0,true)' 
root -l -b -q 'draw_RpPb_rap.C+(0,0,false)' 

echo ""
echo "RpPb All Done!!!!!" 
echo ""
