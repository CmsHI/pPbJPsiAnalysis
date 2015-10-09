# no zvtx, no TNP
#root -l -b -q 'draw_1D.C+(1,1,0,1,0,0,0,"draw_1D_vertex")'
#root -l -b -q 'draw_1D.C+(1,0,0,1,0,0,0,"draw_1D_vertex")'
#root -l -b -q 'draw_1D.C+(0,1,0,1,0,0,0,"draw_1D_vertex")'
#root -l -b -q 'draw_1D.C+(0,0,0,1,0,0,0,"draw_1D_vertex")'
# no TNP
root -l -b -q 'draw_1D.C+(1,1,0,1,0,0,1,"draw_1D_tnpCalo")'
root -l -b -q 'draw_1D.C+(1,0,0,1,0,0,1,"draw_1D_tnpCalo")'
root -l -b -q 'draw_1D.C+(0,1,0,1,0,0,1,"draw_1D_tnpCalo")'
root -l -b -q 'draw_1D.C+(0,0,0,1,0,0,1,"draw_1D_tnpCalo")'
# actual
root -l -b -q 'draw_1D.C+(1,1,0,1,1,0,1,"draw_1D_tnpCalo")'
root -l -b -q 'draw_1D.C+(1,0,0,1,1,0,1,"draw_1D_tnpCalo")'
root -l -b -q 'draw_1D.C+(0,1,0,1,1,0,1,"draw_1D_tnpCalo")'
root -l -b -q 'draw_1D.C+(0,0,0,1,1,0,1,"draw_1D_tnpCalo")'
