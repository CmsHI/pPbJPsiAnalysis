#!/bin/bash
#int rootAna_muIDvariables(TString szPA = "pp", bool isScaled = false, bool isSmall =true){
### don't throw them at once! (same C codes.. need time to compile. Use sleep)
./condor_root_simple.sh 'rootAna_muIDvariables.C+(\"pp\",true,false)'
sleep 1m
./condor_root_simple.sh 'rootAna_muIDvariables.C+(\"pp\",false,false)'
sleep 1m
./condor_root_simple.sh 'rootAna_muIDvariables.C+(\"pa\",true,false)'
sleep 1m
./condor_root_simple.sh 'rootAna_muIDvariables.C+(\"pa\",false,false)'
