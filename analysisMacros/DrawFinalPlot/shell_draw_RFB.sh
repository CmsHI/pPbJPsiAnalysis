##void draw_RFB_pt(bool sysByHand=true, bool noPtWeight=true, bool isPrompt=true)
###void draw_RFB_rap(bool sysByHand=true,  bool noPtWeight=true, bool isPrompt = true)
#void draw_RFB_ethf(bool sysByHand=true, bool noPtWeight = false, bool isPrompt=false)

##### noPtWeight
#root -l -b -q 'draw_RFB_pt.C+(1,1,true)'
#root -l -b -q 'draw_RFB_pt.C+(1,1,false)'
#root -l -b -q 'draw_RFB_rap.C+(1,1,true)'
#root -l -b -q 'draw_RFB_rap.C+(1,1,false)'
#root -l -b -q 'draw_RFB_ethf.C+(1,1,true)'
#root -l -b -q 'draw_RFB_ethf.C+(1,1,false)'
##### ptWeight
root -l -b -q 'draw_RFB_pt.C+(1,0,true)'
root -l -b -q 'draw_RFB_pt.C+(1,0,false)'
root -l -b -q 'draw_RFB_pt_fineBinTest.C+(1,0,true)'
root -l -b -q 'draw_RFB_pt_fineBinTest.C+(1,0,false)'
root -l -b -q 'draw_RFB_rap.C+(1,0,true)'
root -l -b -q 'draw_RFB_rap.C+(1,0,false)'
root -l -b -q 'draw_RFB_ethf.C+(1,0,true)'
root -l -b -q 'draw_RFB_ethf.C+(1,0,false)'



