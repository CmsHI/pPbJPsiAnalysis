#!/bin/bash
#
# created by Genie
# modified by JungWoo
#
# - condor_root_simple.sh : submit root macro to condor without making initial directory.
#
# 1) If you have input file, then change WHEN_YOU_HAVE_INPUT_FILES to input file 
# name and remove '#' at the front of 'Input'.
#
# 2) If you want notification to your e-mail, then change CHANGE_THIS_TO_YOUR_EMAIL 
# to your e-mail address and remove '#' at the front of 'Notify_user'.
#
# 3) If you don't want any of log, error or output file, comment them out by
# writting '#' at the front.
#
# 4) If you don't need initial directory, or any other changes should be made,
# write your own submit file!


dir=`pwd`
outfile=$1_`date +"%Y%m%d"`
subfile=subfile_$outfile
root=`which root`


cat > $subfile << EOF
Executable   = $root
Arguments    = $dir/$1 $2 $3 $4 $5 $6 $7 $8 $9

Log          = $outfile.log
Error        = $outfile.err
Output       = $outfile.out
#Input        = WHEN_YOU_HAVE_INPUT_FILES

Universe     = vanilla
GetEnv       = True
Request_cpus = 1
Notification = Always
#Notify_user  = CHANGE_THIS_TO_YOUR_EMAIL

Queue
EOF


condor_submit $subfile
rm $subfile
