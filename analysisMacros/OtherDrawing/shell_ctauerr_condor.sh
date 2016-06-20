#!/bin/bash
###void ctauerr_differential(TString szPA = "pa", bool isSmall =true, bool isAddNP=true, double rapmin=-2.4, double rapmax=-1.97, double ptmin=2.0, double ptmax=3.0)
### don't throw them at once! (same C codes.. need time to compile. Use sleep, or EOF)
root -l -b <<EOF
.L ctauerr_differential.C++
.q
EOF
#sleep 1m

### binning in y_{lab}
ppRapBins=('-2.4,-1.93' '-1.93,-1.50' '-1.50,-0.90' '-0.90,0.00' '0.00,0.90' '0.90,1.50' '1.50,1.93' '1.93,2.40')
paRapBins=('-2.4,-1.97' '-1.97,-1.37' '-1.37,-0.47' '-0.47,0.43' '0.43,1.03' '1.03,1.46' '1.46,1.93' '1.93,2.40')
ptBins=('2.0,3.0' '3.0,4.0' '4.0,5.0' '5.0,6.5' '6.5,7.5' '7.5,8.5' '8.5,10.0' '10.0,14.0' '14.0,30.0')
#pt='6.5,7.5'

### options
isSmall=false
isAddNP=true

#### pT-y differential

for pt in ${ptBins[@]}
do
  ### pp
  for rap in ${ppRapBins[@]}
  do
    ./condor_root_simple.sh 'ctauerr_differential.C+(\"pp\",'$isSmall','$isAddNP','$rap','$pt')'
  done
  ### pa
  for rap in ${paRapBins[@]}
  do
    ./condor_root_simple.sh 'ctauerr_differential.C+(\"pa\",'$isSmall','$isAddNP','$rap','$pt')'
  done
done

#### pT-y integrated
#./condor_root_simple.sh 'ctauerr_differential.C+(\"pp\",'$isSmall','$isAddNP',-2.4,2.4,0.0,30.0)'
#./condor_root_simple.sh 'ctauerr_differential.C+(\"pa\",'$isSmall','$isAddNP',-2.4,2.4,0.0,30.0)'

