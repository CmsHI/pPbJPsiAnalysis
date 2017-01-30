###void check_forZooRpA(double maxrap=300, bool isPrompt = false)
## |yCM| < 1.5
root -l -b -q 'check_forZooRpA.C+(300,true)'
root -l -b -q 'check_forZooRpA.C+(300,false)'
## |yCM| < 1.93
root -l -b -q 'check_forZooRpA.C+(386,true)'
root -l -b -q 'check_forZooRpA.C+(386,false)'

