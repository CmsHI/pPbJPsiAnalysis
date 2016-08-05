##void draw_RpPb_pt(bool sysByHand=true, bool noPtWeight=true, bool isPrompt=true)
###void draw_RpPb_rap(bool sysByHand=true,  bool noPtWeight=true, bool isPrompt = true)


echo "++++++++++ RpPb start ++++++++++"

root -l -b <<EOF
.L draw_RpPb_pt.C++
.q
EOF
root -l -b -q 'draw_RpPb_pt.C+(1,0,true)' >&logfiles/RpPb_PR_pt&
root -l -b -q 'draw_RpPb_pt.C+(1,0,false)' >&logfiles/RpPb_NP_pt&

root -l -b <<EOF
.L draw_RpPb_rap.C++
.q
EOF
root -l -b -q 'draw_RpPb_rap.C+(1,0,true)' >&logfiles/RpPb_PR_rap&
root -l -b -q 'draw_RpPb_rap.C+(1,0,false)' >&logfiles/RpPb_NP_rap&

echo ""
echo "RpPb All Done!!!!!" 
echo ""
