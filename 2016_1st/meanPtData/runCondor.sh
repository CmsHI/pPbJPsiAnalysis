#!/bin/bash

# 8 rapidity bins (0-7)
### ycm array
ycmbins=(1.93 1.5 0.9 0.0 -0.9 -1.5 -1.93 -2.4 -2.87)
nfinal=$((${#ycmbins[*]}-1)) ##8rap
ninit=0
ntot=$((nfinal-init))
echo $nfinal
echo $ninit
echo $ntot
skelcppname=rootAna_meanPt_wBkg_skel
workcppname=cndr_rootAna_meanPt_wBkg_
#outDir=dir_meanPt_n100_8rap9pt
outDir=dir_meanPt_n20_PAS

if [ ! -d "$(pwd)/$outDir" ]; then
	mkdir $(pwd)/$outDir
fi

echo "*** Check the y_CM array"
for ((irap=$ninit; irap < $nfinal; irap++))
#for ((irap=ninit; irap < ${#ycmbins[*]}-1; irap++))
do
   echo $irap"th from "${ycmbins[irap]}"to"${ycmbins[irap+1]}
done

### main
echo "*** Make $ntot cpp files and run!"
for (( irap=$ninit; irap<$nfinal; irap++)) do
echo "------ "$irap"th process ------"
### create cpp
awk -v p=$irap -v p1=${ycmbins[irap]} -v p2=${ycmbins[irap+1]} '{gsub("_ycmnum_",p); gsub("_ycmup_",p1);  gsub("_ycmdown_",p2); print;}' $skelcppname.C > $workcppname$irap.C;
### run condor
./condor_root_simple.sh $workcppname$irap.C+ 
done
condor_q
echo "*** Done!!"


