########################################
################ pT
########################################
###void comp_RpPb_pt_Vogt(double ptmax=32, bool isLine=true, bool isSmoothened=false)
root -l -b <<EOF
.L comp_RpPb_pt_Vogt.C++
.q
EOF
root -l -b -q 'comp_RpPb_pt_Vogt.C+(32,true,false)'

###void comp_RpPb_pt_Lansberg(double ptmax=32, bool isLine=true, bool isSmoothened=false, TString szPDF= "nCTEQ15")
root -l -b <<EOF
.L comp_RpPb_pt_Lansberg.C++
.q
EOF
root -l -b -q 'comp_RpPb_pt_Lansberg.C+(32,true,false,"EPS09LO")'
root -l -b -q 'comp_RpPb_pt_Lansberg.C+(32,true,false,"EPS09NLO")'
root -l -b -q 'comp_RpPb_pt_Lansberg.C+(32,true,false,"nCTEQ15")'

###void comp_RpPb_pt_Overlay(double ptmax=32, bool isPoint=true, bool isSmoothened=false)
root -l -b <<EOF
.L comp_RpPb_pt_Overlay.C++
.q
EOF
root -l -b -q 'comp_RpPb_pt_Overlay.C+(32,true,false)'

########################################
################ rap
########################################
###void comp_RpPb_rap_Vogt(bool isLine=true, bool isSmoothened=false)
root -l -b <<EOF
.L comp_RpPb_rap_Vogt.C++
.q
EOF
root -l -b -q 'comp_RpPb_rap_Vogt.C+(true,false)'

###void comp_RpPb_rap_Lansberg(bool isLine=true, bool isSmoothened=false, TString szPDF= "nCTEQ15")
root -l -b <<EOF
.L comp_RpPb_rap_Lansberg.C++
.q
EOF
root -l -b -q 'comp_RpPb_rap_Lansberg.C+(true,false,"EPS09LO")'
root -l -b -q 'comp_RpPb_rap_Lansberg.C+(true,false,"EPS09NLO")'
root -l -b -q 'comp_RpPb_rap_Lansberg.C+(true,false,"nCTEQ15")'

###void comp_RpPb_rap_Overlay(bool isPoint=true, bool isSmoothened=false)
root -l -b <<EOF
.L comp_RpPb_rap_Overlay.C++
.q
EOF
root -l -b -q 'comp_RpPb_rap_Overlay.C+(true,false)'

