###void draw_2D_acc(int MrapNpt=89, int isPA = 0, bool isPrompt=false, int accCutType =2, TString szDir="dir_acc")
#### pp
root -l -b -q 'draw_2D_acc.C+(89,0,true,2,"dir_acc")'
root -l -b -q 'draw_2D_acc.C+(89,0,false,2,"dir_acc")'
root -l -b -q 'draw_2D_acc_fine.C+(89,0,true,2,"dir_acc")'
root -l -b -q 'draw_2D_acc_fine.C+(89,0,false,2,"dir_acc")'
#### pA
root -l -b -q 'draw_2D_acc.C+(89,1,true,2,"dir_acc")'
root -l -b -q 'draw_2D_acc.C+(89,1,false,2,"dir_acc")'
root -l -b -q 'draw_2D_acc_fine.C+(89,1,true,2,"dir_acc")'
root -l -b -q 'draw_2D_acc_fine.C+(89,1,false,2,"dir_acc")'
