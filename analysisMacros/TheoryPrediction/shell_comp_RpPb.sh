########################################
################ pT
########################################
###void comp_RpPb_pt_Vogt(double ptmax=32, bool isLine=true, bool isSmoothened=false)
root -l -b <<EOF
.L comp_RpPb_pt_Vogt.C++
.q
EOF
root -l -b -q 'comp_RpPb_pt_Vogt.C+(32,true,false)'
root -l -b -q 'comp_RpPb_pt_Vogt.C+(32,true,true)' ### isSmoothened

###void comp_RpPb_pt_Lansberg(double ptmax=32, bool isLine=true, bool isSmoothened=false, TString szPDF= "nCTEQ15")
root -l -b <<EOF
.L comp_RpPb_pt_Lansberg.C++
.q
EOF
root -l -b -q 'comp_RpPb_pt_Lansberg.C+(32,true,false,"EPS09LO")'
root -l -b -q 'comp_RpPb_pt_Lansberg.C+(32,true,true,"EPS09LO")' ###isSmoothened 
root -l -b -q 'comp_RpPb_pt_Lansberg.C+(32,true,false,"EPS09NLO")'
root -l -b -q 'comp_RpPb_pt_Lansberg.C+(32,true,true,"EPS09NLO")' ###isSmoothened
root -l -b -q 'comp_RpPb_pt_Lansberg.C+(32,true,false,"nCTEQ15")'
root -l -b -q 'comp_RpPb_pt_Lansberg.C+(32,true,true,"nCTEQ15")' ###isSmoothened

###void comp_RpPb_pt_Overlay(double ptmax=32, bool isPoint=true, bool isSmoothened=false)
root -l -b <<EOF
.L comp_RpPb_pt_Overlay.C++
.q
EOF
root -l -b -q 'comp_RpPb_pt_Overlay.C+(32,true,false)'
root -l -b -q 'comp_RpPb_pt_Overlay.C+(32,true,true)' ### isSmoothened

########################################
################ rap
########################################
###void comp_RpPb_rap_Vogt(bool isLine=true, bool isSmoothened=false)
root -l -b <<EOF
.L comp_RpPb_rap_Vogt.C++
.q
EOF
root -l -b -q 'comp_RpPb_rap_Vogt.C+(true,false)'
root -l -b -q 'comp_RpPb_rap_Vogt.C+(true,true)' ### isSmoothened

###void comp_RpPb_rap_Lansberg(bool isLine=true, bool isSmoothened=false, TString szPDF= "nCTEQ15")
root -l -b <<EOF
.L comp_RpPb_rap_Lansberg.C++
.q
EOF
root -l -b -q 'comp_RpPb_rap_Lansberg.C+(true,false,"EPS09LO")'
root -l -b -q 'comp_RpPb_rap_Lansberg.C+(true,true,"EPS09LO")' ## isSmoothened
root -l -b -q 'comp_RpPb_rap_Lansberg.C+(true,false,"EPS09NLO")'
root -l -b -q 'comp_RpPb_rap_Lansberg.C+(true,true,"EPS09NLO")' ## isSmoothened
root -l -b -q 'comp_RpPb_rap_Lansberg.C+(true,false,"nCTEQ15")'
root -l -b -q 'comp_RpPb_rap_Lansberg.C+(true,true,"nCTEQ15")' ## isSmoothened

###void comp_RpPb_rap_Overlay(bool isPoint=true, bool isSmoothened=false)
root -l -b <<EOF
.L comp_RpPb_rap_Overlay.C++
.q
EOF
root -l -b -q 'comp_RpPb_rap_Overlay.C+(true,false)'
root -l -b -q 'comp_RpPb_rap_Overlay.C+(true,true)' ### isSmoothened

