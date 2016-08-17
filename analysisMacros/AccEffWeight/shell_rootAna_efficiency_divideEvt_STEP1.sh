####################################################################
###########int rootAna_efficiency_divideEvt(int MrapNpt = 89, int isPA =0, int accCutType = 2, bool isPrompt = true, bool useZvtxWeight =false, bool useSF=true, int=initev=0. int nevt=10000000)
####################################################################
########## divide events to be merged later
####### pp prompt : total 60830490
####### pp non-prompt : total 69283445 
####### Pbp prompt : total 8222669
####### Pbp non-prompt : total 14892039
####### pPb prompt : total 8012891
####### pPb non-prompt : total 15565599


### make directory
if [ ! -d "toBeMerged_divideEvt" ]; then
  mkdir toBeMerged_divideEvt
fi

## compile
root -l -b <<EOF
.L rootAna_efficiency_ptWeight_divideEvt.C++
.q
EOF

################################################
###################### pp
################################################
####### pp prompt : total 60830490
for evt in '0,5000000' '5000000,10000000' '10000001,15000000' '15000001,20000000' '20000001,25000000' '25000001,30000000' '30000001,35000000' '35000001,40000000' '40000001,45000000' '45000001,50000000' '50000001,55000000' '55000001,60830490'
do
  echo " *** pp PR Zvtx0_SF1 (evt = " $evt ") ***"  ## actual
  root -l -b -q 'rootAna_efficiency_ptWeight_divideEvt.C(89,0,2,true,false,true,'$evt')'
done

####### pp non-prompt : total 69283445 
for evt in '0,5000000' '5000000,10000000' '10000001,15000000' '15000001,20000000' '20000001,25000000' '25000001,30000000' '30000001,35000000' '35000001,40000000' '40000001,45000000' '45000001,50000000' '50000001,55000000' '55000001,60000000' '60000001,65000000' '65000001,69283445'
do
  echo " *** pp NP Zvtx0_SF1 (evt = " $evt ") ***"  ## actual
  root -l -b -q 'rootAna_efficiency_ptWeight_divideEvt.C(89,0,2,false,false,true,'$evt')'
done

################################################
###################### Pbp
################################################
####### Pbp non-prompt : total 14892039
for evt in '0,5000000' '5000000,10000000' '10000001,14892039'
do
  echo " *** Pbp NP 8rap9pt Zvtx1_SF1 (evt = " $evt ") ***"  ## actual
  root -l -b -q 'rootAna_efficiency_ptWeight_divideEvt.C(89,1,2,false,true,true,'$evt')'
  echo " *** Pbp NP 6rap2pt Zvtx1_SF1 (evt = " $evt ") ***"  ## actual for E_T dependence
  root -l -b -q 'rootAna_efficiency_ptWeight_divideEvt.C(62,1,2,false,true,true,'$evt')' 
done

################################################
###################### Pbp
################################################
####### pPb non-prompt : total 15565599
for evt in '0,5000000' '5000000,10000000' '10000001,15565599'
do
  echo " *** pPb NP 8rap9pt Zvtx1_SF1 (evt = " $evt ") ***"  ## actual
  root -l -b -q 'rootAna_efficiency_ptWeight_divideEvt.C(89,2,2,false,true,true,'$evt')'
  echo " *** pPb NP 6rap2pt Zvtx1_SF1 (evt = " $evt ") ***"  ## actual for E_T dependence
  root -l -b -q 'rootAna_efficiency_ptWeight_divideEvt.C(62,2,2,false,true,true,'$evt')'
done

