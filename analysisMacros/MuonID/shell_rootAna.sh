#int rootAna_muIDvariables(TString szPA = "pp", bool isScaled = false, bool isSmall =true){
root -l -b -q 'rootAna_muIDvariables.C+("pp", true, false)'
root -l -b -q 'rootAna_muIDvariables.C+("pp", false, false)'
root -l -b -q 'rootAna_muIDvariables.C+("pa", true, false)'
root -l -b -q 'rootAna_muIDvariables.C+("pa", false, false)'
