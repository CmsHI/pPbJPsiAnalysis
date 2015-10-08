#!/bin/bash
if [ $# -ne 4 ]; then
  echo "Usage: $0 [Executable] [Input root File] [Input root File 2] [Prefix] "
  exit;
fi

executable=$(pwd)/$1
datasets=$2
datasets2=$3
prefix=$4

################################################################ 
########## Script parameter setting
################################################################ 
# Storage for batch jobs (should be under /afs)
storage=$(pwd)/Results/$prefix
scripts=$(pwd)/Scripts
if [ ! -d "$(pwd)/Results" ]; then
  mkdir $(pwd)/Results
fi
if [ ! -d "$storage" ]; then
  mkdir $storage
fi
if [ ! -d "$scripts" ]; then
  mkdir $scripts
fi

ctaurange=1.5-3.0
fracfree=0
ispbpb=2 # 0:pp, 1:PbPb, 2:Pbp, 3:pPb
is2Widths=1
isPEE=1
usedPhi=0 # 0: RAA, 1: v2 (this determines whether dphi angles will be presented on the plots or not)
isMerge=1 # 1 merging the dataset from two files (v1 and v2)

# Non-prompt MC
mc1=/afs/cern.ch/work/k/kyolee/private/pAJpsi_rooDataSet_20150910/outRoo_NPMC_Pbp_mcTwoWay_cutG/outRoo_NPMC_Pbp_mcTwoWay_cutG.root
# Prompt MC
mc2=/afs/cern.ch/work/k/kyolee/private/pAJpsi_rooDataSet_20150910/outRoo_PRMC_Pbp_mcTwoWay_cutG/outRoo_PRMC_Pbp_mcTwoWay_cutG.root

# read the ctauErrFile or not
#readct=1 #0:read from txt 1:calculated in the fitting code 2:constant value
readct=0 #0:read from txt 1:calculated in the fitting code 2:constant value
cterrfile=$(pwd)/outCtErr/fit_ctauErrorRange_Pbp.txt

mSigF="sigCB2WNG1" # Mass signal function name (options: sigCB2WNG1 (default), signalCB3WN)
mBkgF="expFunct" # Mass background function name (options: expFunct (default), polFunct)

#weight=0  #0: Do NOT weight(dataJpsi), 1: Do weight(dataJpsiWeight)
weight=1  #0: Do NOT weight(dataJpsi), 1: Do weight(dataJpsiWeight) <- MC z vertex weighted!
eventplane="etHFm" # Name of eventplane (etHFp, etHFm, etHF(default))
runOpt=4 # Inclusive mass fit (options: 4(default), 3(Constrained fit), 5(_mb in 2010 analysis))
anaBct=1 #0: do b-fit(not-analytic fit for b-lifetime), 1: do b-fit(analytic fit for b-lifetime), 2: do NOT b-fit
#0: 2 Resolution functions & fit on data, 1: 1 Resolution function & fit on data,
#2: 2 Resolution functions & fit on PRMC, 3: 1 Resolution function & fit on PRMC
resOpt=2
ctauBkg=0 #0: 1 ctau bkg, 1: 2 ctau bkg with signal region fitting, 2: 2 ctau bkg with step function
isMultiplicity=0 #0:centrality 1:Ntrack 2:ET^{HF}

########## Except dphibins, rap, pt, centrality bins doesn't need "integrated range" bins in the array.
########## Ex ) DO NOT USE rapbins=(0.0-2.4) or ptbins=(6.5-30.0) or centbins=(0.0-100.0)
########## dphibins always needs "0.000-1.571" both for Raa and v2. Add other dphibins if you need
dphibins=(0.000-1.571) #0.000-0.393 0.393-0.785 0.785-1.178 1.178-1.571)

rapbins=(-2.4--1.97 -1.97--1.37 -1.37--0.47 -0.47-0.43 0.43-1.03 1.03-1.46 1.46-1.93 1.93-2.4)
#rapbins=(1.93-2.4)
#ptbins=(0.0-3.0 3.0-4.0 4.0-5.0 5.0-6.5 6.5-7.5 7.5-8.5 8.5-10.0 10.0-14.0 14.0-30.0)
#ptbins=(0.0-1.5 1.5-3.0 3.0-4.0 4.0-5.0 5.0-6.5 6.5-7.5 7.5-8.5 8.5-10.0 10.0-14.0 14.0-30.0)
#ptbins=(2.0-3.0 3.0-4.0 4.0-5.0 5.0-6.5 6.5-7.5 7.5-8.5 8.5-10.0 10.0-14.0 14.0-30.0)
ptbins=(3.0-4.0 4.0-5.0 5.0-6.5 6.5-7.5 7.5-8.5 8.5-10.0 10.0-14.0 14.0-30.0)

#ethfbins=(0.0-20.0 20.0-25.0 25.0-30.0 30.0-40.0 40.0-120.0)
#ethfETHF=(0.0-20.0 20.0-30.0 30.0-120.0)
ethfbins=(0.0-120.0)
ntrkbins=(0.0-350.0)

################################################################ 
########## Information print out
################################################################ 
txtrst=$(tput sgr0)
txtred=$(tput setaf 2)  #1 Red, 2 Green, 3 Yellow, 4 Blue, 5 Purple, 6 Cyan, 7 White
txtbld=$(tput bold)     #dim (Half-bright mode), bold (Bold characters)

echo "Run fits with ${txtbld}${txtred}$executable${txtrst} on ${txtbld}${txtred}$datasets${txtrst}."
if [ "$storage" != "" ]; then
  echo "Results will be placed on ${txtbld}${txtred}$storage${txtrst}."
fi

################################################################ 
########## Function for progream running
################################################################ 
function program {
  ### Arguments
  rap=$1
  pt=$2
	ntrk=$3
	ethf=$4
  shift; shift; shift; shift;
  centarr=(${@})

  for cent in ${centarr[@]}; do
    for dphi in ${dphibins[@]}; do
      work=$prefix"_rap"$rap"_pT"$pt"_cent"$cent"_dPhi"$dphi"_ntrk"$ntrk"_ET"$ethf; # Output file name has this prefix
      workMB=$prefix"_rap"$rap"_pT"$pt"_cent0.0-100.0_dPhi0.000-1.571_ntrk"$ntrk"_ET"$ethf; 
      workPHI=$prefix"_rap"$rap"_pT"$pt"_cent"$cent"_dPhi0.000-1.571_ntrk"$ntrk"_ET"$ethf; 

      echo "Processing: "$work
      printf "#!/bin/csh\n" > $scripts/$work.csh
#      printf "source /afs/cern.ch/sw/lcg/external/gcc/4.3.2/x86_64-slc5/setup.csh; source /afs/cern.ch/user/m/miheejo/thisroot.csh\n" >> $scripts/$work.csh
#      printf "cd %s\n" $(pwd) >> $scripts/$work.csh
#      printf "eval \`scramv1 runtime -csh\`\n" >> $scripts/$work.csh
#      printf "cd -\n" >> $scripts/$work.csh
# orig
#			printf "source /afs/cern.ch/sw/lcg/external/gcc/4.7/x86_64-slc6/setup.csh; source /afs/cern.ch/work/m/miheejo/public/root_v5.28.00d/bin/thisroot.csh\n" >> $scripts/$work.csh
			printf "source /afs/cern.ch/sw/lcg/external/gcc/4.7/x86_64-slc6/setup.csh; source /afs/cern.ch/work/k/kyolee/public/root_v5.28.00d/bin/thisroot.csh\n" >> $scripts/$work.csh
      printf "cp %s/%s.csh %s/fit2DData.h %s/fit2DData_all.cpp .\n" $scripts $work $(pwd) $(pwd) >> $scripts/$work.csh

      if [ "$cent" == "0.0-100.0" ]; then
        if [ "$dphi" == "0.000-1.571" ]; then
          script="$executable -q $isMerge $datasets2 -f $datasets $weight -m $mc1 $mc2 -v $mSigF $mBkgF -d $prefix -r $eventplane $usedPhi -u $resOpt -a $anaBct $ctauBkg -b $ispbpb $isPEE $is2Widths -p $pt -y $rap -t $cent -s $dphi -l $ctaurange -x $runOpt $readct $cterrfile -z $fracfree -j $isMultiplicity -h $ethf -n $ntrk >& $work.log;"
          echo $script >> $scripts/$work.csh
        elif [ "$dphi" != "0.000-1.571" ]; then
          script="$executable -q $isMerge $datasets2 -f $datasets $weight -m $mc1 $mc2 -v $mSigF $mBkgF -d $prefix -r $eventplane $usedPhi -u $resOpt -a 2 $ctauBkg -b $ispbpb $isPEE $is2Widths -p $pt -y $rap -t $cent -s 0.000-1.571 -l $ctaurange -x $runOpt $readct $cterrfile  -z $fracfree -j $isMultiplicity -h $ethf -n $ntrk >& $workPHI.log;"
          echo $script >> $scripts/$work.csh
          script="$executable -q $isMerge $datasets2 -f $datasets $weight -m $mc1 $mc2 -v $mSigF $mBkgF -d $prefix -r $eventplane $usedPhi -u $resOpt -a $anaBct $ctauBkg -b $ispbpb $isPEE $is2Widths -p $pt -y $rap -t $cent -s $dphi -l $ctaurange -x $runOpt $readct $cterrfile -z $fracfree -j $isMultiplicity -h $ethf -n $ntrk >& $work.log;"
          echo $script >> $scripts/$work.csh
        fi
      elif [ "$cent" != "0.0-100.0" ]; then
        if [ "$dphi" == "0.000-1.571" ]; then
          script="$executable -q $isMerge $datasets2 -f $datasets $weight -m $mc1 $mc2 -v $mSigF $mBkgF -d $prefix -r $eventplane $usedPhi -u $resOpt -a 2 $ctauBkg -b $ispbpb $isPEE $is2Widths -p $pt -y $rap -t 0.0-100.0 -s 0.000-1.571 -l $ctaurange -x $runOpt $readct $cterrfile -z $fracfree -j $isMultiplicity -h $ethf -n $ntrk >& $workMB.log;"
          echo $script >> $scripts/$work.csh
          script="$executable -q $isMerge $datasets2 -f $datasets $weight -m $mc1 $mc2 -v $mSigF $mBkgF -d $prefix -r $eventplane $usedPhi -u $resOpt -a $anaBct $ctauBkg -b $ispbpb $isPEE $is2Widths -p $pt -y $rap -t $cent -s $dphi -l $ctaurange -x $runOpt $readct $cterrfile -z $fracfree -j $isMultiplicity -h $ethf -n $ntrk >& $work.log;"
          echo $script >> $scripts/$work.csh
        elif [ "$dphi" != "0.000-1.571" ]; then
          script="$executable -q $isMerge $datasets2 -f $datasets $weight -m $mc1 $mc2 -v $mSigF $mBkgF -d $prefix -r $eventplane $usedPhi -u $resOpt -a 2 $ctauBkg -b $ispbpb $isPEE $is2Widths -p $pt -y $rap -t 0.0-100.0 -s 0.000-1.571 -l $ctaurange -x $runOpt $readct $cterrfile -z $fracfree -j $isMultiplicity -h $ethf -n $ntrk >& $workMB.log;"
          echo $script >> $scripts/$work.csh
          script="$executable -q $isMerge $datasets2 -f $datasets $weight -m $mc1 $mc2 -v $mSigF $mBkgF -d $prefix -r $eventplane $usedPhi -u $resOpt -a 2 $ctauBkg -b $ispbpb $isPEE $is2Widths -p $pt -y $rap -t $cent -s 0.000-1.571 -l $ctaurange -x $runOpt $readct $cterrfile -z $fracfree -j $isMultiplicity -h $ethf -n $ntrk >& $workPHI.log;"
          echo $script >> $scripts/$work.csh
          script="$executable -q $isMerge $datasets2 -f $datasets $weight -m $mc1 $mc2 -v $mSigF $mBkgF -d $prefix -r $eventplane $usedPhi -u $resOpt -a $anaBct $ctauBkg -b $ispbpb $isPEE $is2Widths -p $pt -y $rap -t $cent -s $dphi -l $ctaurange -x $runOpt $readct $cterrfile -z $fracfree -j $isMultiplicity -h $ethf -n $ntrk >& $work.log;"
          echo $script >> $scripts/$work.csh
        fi
      fi

      printf "tar zcvf %s.tgz %s* fit2DData.h fit2DData_all.cpp\n" $work $work >> $scripts/$work.csh
      printf "cp %s.tgz %s\n" $work $storage >> $scripts/$work.csh
      #bsub -R "pool>10000" -u songkyo.lee@cer.c -q 1nd -J $work < $scripts/$work.csh
      #bsub -R "pool>10000" -u songkyo.lee@cer.c -q 1nw -J $work < $scripts/$work.csh
    done
  done
}

################################################################ 
########## Running script with pre-defined binnings
################################################################ 

### 8rap8pt
for rap in ${rapbins[@]}; do
	for pt in ${ptbins[@]}; do
		program $rap $pt $ntrkbins $ethfbins 0.0-100.0
	done
done

program -2.4--1.97 2.0-3.0 $ntrkbins $ethfbins 0.0-100.0
program 1.93-2.4 2.0-3.0 $ntrkbins $ethfbins 0.0-100.0

