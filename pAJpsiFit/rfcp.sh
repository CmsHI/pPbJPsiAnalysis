#!/bin/bash -f

########## Directory where the job submittion performed
submitdir="$(pwd)/Scripts/"
########## Castor directory that contains results
indir_="$(pwd)/Results/"
########## Directory where python & root scripts are located
workdir="/afs/cern.ch/work/k/kyolee/private/CMSSW_5_3_19_Fit/src/pAJpsiFit_cutG"
########## Prefix of jobs
#prefixarr=(ppAnalytic_default_bit2_GGMuLessPVJpsiM ppAnalytic_default_bit2_GGMuLessPVDiMuM)
#(ppAnalytic_rapPlus_default_bit1 ppAnalytic_rapMinus_default_bit1)
prefixarr=$(ls $indir_)
echo $prefixarr

############################################################
eval `scramv1 runtime -sh`

############################################################
########## Copy files from castor and extract it. Run python & root scripts over all files
for prefix in ${prefixarr[@]}; do
  mkdir /tmp/kyolee/$prefix
  cd /tmp/kyolee/$prefix

  indir=$indir_/$prefix
  if [ ! -d $indir ]; then
    echo $indir "is not a directory."
    continue
  fi

  echo "indir: "$indir
  list=$(ls $indir | grep tgz)

  for file in $list; do   # Get files from castor using prefix
    if echo $file | grep -q $prefix; then 
      echo $file
      cp $indir/$file .
      tar zfx ./$file
      rm -f ./$file
#      rm -rf summary
    fi
  done

  # Run python & root script for 1 prefix
  python $workdir/extract.py $prefix ../$prefix
#  root -l $workdir/savehisto.cpp

  # Summarize results
  mkdir /tmp/kyolee/$prefix/summary
#  mv /tmp/kyolee/$prefix/fit_* /tmp/kyolee/$prefix/summary
	mv fit_cppnumbers fit_parameters fit_table fit_errorbins fit_ctauErrorRange /tmp/kyolee/$prefix/summary

  ls $submitdir | grep $prefix | awk 'BEGIN{FS=".csh"}; {print $1}' > $submitdir/$prefix\_submit
  ls /tmp/kyolee/$prefix | grep txt | awk 'BEGIN{FS=".txt"}; {print $1}' > $submitdir/$prefix\_txt
  diff $submitdir/$prefix\_submit $submitdir/$prefix\_txt > $submitdir/diff_$prefix

  tar zfc $indir_/$prefix.tgz /tmp/kyolee/$prefix
done
