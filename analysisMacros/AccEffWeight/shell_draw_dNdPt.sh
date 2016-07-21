###void draw_dNdPt(int isPA=1, bool isPrompt=true, bool noPtWeight=true, bool isLog=false)
root -l -b -q 'draw_dNdPt.C+(0,true,true,false)'
root -l -b -q 'draw_dNdPt.C+(0,false,true,false)'
root -l -b -q 'draw_dNdPt.C+(1,true,true,false)'
root -l -b -q 'draw_dNdPt.C+(1,false,true,false)'
