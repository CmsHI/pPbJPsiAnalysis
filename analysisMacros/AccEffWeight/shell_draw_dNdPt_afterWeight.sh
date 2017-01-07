###void draw_dNdPt(int isPA=1, bool isPrompt=true, bool noPtWeight=true, bool isLog=false)
### check after ptWeight (for AN APPENDIX)
root -l -b -q 'draw_dNdPt.C+(0,true,false,false)'
root -l -b -q 'draw_dNdPt.C+(0,false,false,false)'
root -l -b -q 'draw_dNdPt.C+(1,true,false,false)'
root -l -b -q 'draw_dNdPt.C+(1,false,false,false)'
