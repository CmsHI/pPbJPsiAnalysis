#!/bin/bash

dirName="toBeMerged_divideEvt"
szFinalArr=("8rap9pt_pp_PR_newcut_Zvtx0_SF1_ptWeight" "8rap9pt_pp_NP_newcut_Zvtx0_SF1_ptWeight" "8rap9pt_Pbp_NP_newcut_Zvtx1_SF1_ptWeight" "8rap9pt_pPb_NP_newcut_Zvtx1_SF1_ptWeight" "6rap2pt_Pbp_NP_newcut_Zvtx1_SF1_ptWeight" "6rap2pt_pPb_NP_newcut_Zvtx1_SF1_ptWeight")
#szFinalArr=("8rap9pt_pp_PR_newcut_Zvtx0_SF1_ptWeight")

##### loop
for szFinal in ${szFinalArr[@]}; do
  echo " *****************************************************"
  echo " ***** "$szFinal" *****"
  echo " *****************************************************"
  ls $dirName/EffAna_$szFinal\_initev*.root > $dirName/list_$szFinal.txt
  #ls $dirName/EffAna_$szFinal\_initev*.root
  echo "   ***** 1) list_$szFinal.txt created *****"; #echo -e;
  if [ "$dirName/merged_EffAna_$szFinal.root" ]; then
    rm $dirName/merged_EffAna_$szFinal.root
  fi
  echo "   ***** 2) existing merged_EffAna_$szFinal.root removed ***** "; #echo -e;
  hadd $dirName/merged_EffAna_$szFinal.root @$dirName/list_$szFinal.txt
  echo "   ***** 3) merged_EffAna_$szFinal.root created ***** ";echo -e;
done

echo " All done!!!!!! :D "

