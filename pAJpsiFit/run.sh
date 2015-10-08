#!/bin/bash


#################################################
############ for Pbp (1st run)
#################################################

### for v1 and v2 merging
./runBatch_Pbp.sh Fit2DDataAll /afs/cern.ch/work/k/kyolee/private/pAJpsi_rooDataSet_20150910/outRoo_Data_Pbp_cutG_v1/outRoo_Data_Pbp_cutG_v1.root /afs/cern.ch/work/k/kyolee/private/pAJpsi_rooDataSet_20150910/outRoo_Data_Pbp_cutG_v2/outRoo_Data_Pbp_cutG_v2.root Pbp_cutG >& log_Pbp_cutG & 

#################################################
############ for pPb (2nd run) 
#################################################
./runBatch_pPb.sh Fit2DDataAll /afs/cern.ch/work/k/kyolee/private/pAJpsi_rooDataSet_20150910/outRoo_Data_pPb_cutG/outRoo_Data_pPb_cutG.root pPb_cutG >& log_pPb_cutG & 

