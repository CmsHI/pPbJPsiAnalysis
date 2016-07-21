#void AccEffSys(int isPA=0, int rapPt = 89, int opt=0, int zVtx = 0)

#pp
root -l -b -q 'AccEffSys.C+(0,89,0,0)'

#pA
root -l -b -q 'AccEffSys.C+(1,89,0,1)'
root -l -b -q 'AccEffSys.C+(1,62,1,1)'
root -l -b -q 'AccEffSys.C+(1,62,2,1)'
root -l -b -q 'AccEffSys.C+(1,62,3,1)'
