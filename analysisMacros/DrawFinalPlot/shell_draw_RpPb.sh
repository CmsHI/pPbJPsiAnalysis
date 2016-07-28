##void draw_RpPb_pt(bool sysByHand=true, bool noPtWeight=true, bool isPrompt=true)
###void draw_RpPb_rap(bool sysByHand=true,  bool noPtWeight=true, bool isPrompt = true)

##### noPtWeight
#root -l -b -q 'draw_RpPb_pt.C+(1,1,true)'
#root -l -b -q 'draw_RpPb_pt.C+(1,1,false)'
#root -l -b -q 'draw_RpPb_rap.C+(1,1,true)'
#root -l -b -q 'draw_RpPb_rap.C+(1,1,false)'

##### ptWeight
root -l -b -q 'draw_RpPb_pt.C+(1,0,true)'
root -l -b -q 'draw_RpPb_pt.C+(1,0,false)'
root -l -b -q 'draw_RpPb_rap.C+(1,0,true)'
root -l -b -q 'draw_RpPb_rap.C+(1,0,false)'

