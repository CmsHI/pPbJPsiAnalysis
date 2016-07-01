##void draw_RFB_pt(bool sysByHand=true, bool noPtWeight=true, bool isPrompt=true)
###void draw_RFB_rap(bool sysByHand=true,  bool noPtWeight=true, bool isPrompt = true)
root -l -b -q 'draw_RFB_pt.C+(1,1,true)'
root -l -b -q 'draw_RFB_pt.C+(1,1,false)'
root -l -b -q 'draw_RFB_rap.C+(1,1,true)'
root -l -b -q 'draw_RFB_rap.C+(1,1,false)'

root -l -b -q 'draw_RFB_pt_tag3.C+(1,1,true)'
root -l -b -q 'draw_RFB_pt_tag3.C+(1,1,false)'
root -l -b -q 'draw_RFB_rap_tag3.C+(1,1,true)'
root -l -b -q 'draw_RFB_rap_tag3.C+(1,1,false)'

root -l -b -q 'draw_RFB_pt_eta45_tag5.C+(1,1,true)'
root -l -b -q 'draw_RFB_pt_eta45_tag5.C+(1,1,false)'
root -l -b -q 'draw_RFB_rap_eta45_tag5.C+(1,1,true)'
root -l -b -q 'draw_RFB_rap_eta45_tag5.C+(1,1,false)'



