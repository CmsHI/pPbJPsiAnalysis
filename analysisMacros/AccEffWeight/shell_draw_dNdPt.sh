###void draw_dNdPt(int isPA=1, bool isPrompt=true, bool noPtWeight=true, bool isLog=false)
### for dN/dPt fit
root -l 'draw_dNdPt.C+(0,true,true,false)'
root -l 'draw_dNdPt.C+(0,false,true,false)'
root -l 'draw_dNdPt.C+(1,true,true,false)'
root -l 'draw_dNdPt.C+(1,false,true,false)'
### check after ptWeight (for AN)
root -l 'draw_dNdPt.C+(0,true,false,false)'
root -l 'draw_dNdPt.C+(0,false,false,false)'
root -l 'draw_dNdPt.C+(1,true,false,false)'
root -l 'draw_dNdPt.C+(1,false,false,false)'
