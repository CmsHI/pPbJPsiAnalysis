#include "../SONGKYO.h"

/////// main func. ///////
int make_totalHist_PbppPb_noPtWeight_privFit_privMC(int MrapNpt=89, int accCutType = 1, bool useZvtxWeight=true, bool useSF = true, TString szSys="nominal"){
  using namespace std;
  
  TString szBinning;
  if (MrapNpt==89)  {szBinning = "8rap9pt"; }
  else if (MrapNpt==83) { szBinning = "8rap3pt"; }
  else if (MrapNpt==63) { szBinning = "6rap3pt"; }
  else if (MrapNpt==62) { szBinning = "6rap2pt"; }
  else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return 0; }
  TString szAccCut;
  if (accCutType==1) szAccCut="oldcut";
  else if (accCutType==2) szAccCut="newcut";
  else {cout << "select among accCutType = 0 or 1"<< endl; return 0; }
  const TString szFinal = Form("%s_%s_%s",szBinning.Data(),szAccCut.Data(),szSys.Data());
  std::cout << "szFinal: " << szFinal << std::endl;

  //////////////////////////////////////////////////////////////////////////////////////
  //// Definition of the binning
  int nRap, nPt;
  if (MrapNpt==89) { nRap = 8; nPt = 9; }
  else if (MrapNpt==83) { nRap = 8; nPt = 3; }
  else if (MrapNpt==63) { nRap = 6; nPt = 3;} 
  else { nRap = 6; nPt = 2;} 

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  ////// read in Acceptance file
  // *** without pt weight
  TFile *fAccPR = new TFile(Form("../Acceptance/AccAna_%s_pA_PR_%s.root",szBinning.Data(),szAccCut.Data()));  
  TFile *fAccNP = new TFile(Form("../Acceptance/AccAna_%s_pA_NP_%s.root",szBinning.Data(),szAccCut.Data()));  
  TH2D* h2D_Acc_PR_Pbp = (TH2D*)fAccPR->Get("h2D_Acc_pt_y_Pbp");
  TH2D* h2D_Acc_PR_pPb = (TH2D*)fAccPR->Get("h2D_Acc_pt_y_pPb");
  TH2D* h2D_Acc_NP_Pbp = (TH2D*)fAccNP->Get("h2D_Acc_pt_y_Pbp");
  TH2D* h2D_Acc_NP_pPb = (TH2D*)fAccNP->Get("h2D_Acc_pt_y_pPb");
  h2D_Acc_PR_Pbp->SetName("h2D_Acc_PR_Pbp");
  h2D_Acc_PR_pPb->SetName("h2D_Acc_PR_pPb");
  h2D_Acc_NP_Pbp->SetName("h2D_Acc_NP_Pbp");
  h2D_Acc_NP_pPb->SetName("h2D_Acc_NP_pPb");
  cout << "4 Acc TH2D : "<<h2D_Acc_PR_Pbp<<" "<<h2D_Acc_PR_pPb<<" "<<h2D_Acc_NP_Pbp<<" "<<h2D_Acc_NP_pPb<<endl;
  
  TH2D* h2D_Acc_Den_PR_Pbp = (TH2D*)fAccPR->Get("h2D_Den_pt_y_Pbp");
  TH2D* h2D_Acc_Den_PR_pPb = (TH2D*)fAccPR->Get("h2D_Den_pt_y_pPb"); 
  TH2D* h2D_Acc_Den_NP_Pbp = (TH2D*)fAccNP->Get("h2D_Den_pt_y_Pbp");
  TH2D* h2D_Acc_Den_NP_pPb = (TH2D*)fAccNP->Get("h2D_Den_pt_y_pPb");
  h2D_Acc_Den_PR_Pbp->SetName("h2D_Acc_Den_PR_Pbp");
  h2D_Acc_Den_PR_pPb->SetName("h2D_Acc_Den_PR_pPb");
  h2D_Acc_Den_NP_Pbp->SetName("h2D_Acc_Den_NP_Pbp");
  h2D_Acc_Den_NP_pPb->SetName("h2D_Acc_Den_NP_pPb");
  TH2D* h2D_Acc_Num_PR_Pbp = (TH2D*)fAccPR->Get("h2D_Num_pt_y_Pbp");
  TH2D* h2D_Acc_Num_PR_pPb = (TH2D*)fAccPR->Get("h2D_Num_pt_y_pPb");
  TH2D* h2D_Acc_Num_NP_Pbp = (TH2D*)fAccNP->Get("h2D_Num_pt_y_Pbp");
  TH2D* h2D_Acc_Num_NP_pPb = (TH2D*)fAccNP->Get("h2D_Num_pt_y_pPb");
  h2D_Acc_Num_PR_Pbp->SetName("h2D_Acc_Num_PR_Pbp");
  h2D_Acc_Num_PR_pPb->SetName("h2D_Acc_Num_PR_pPb");
  h2D_Acc_Num_NP_Pbp->SetName("h2D_Acc_Num_NP_Pbp");
  h2D_Acc_Num_NP_pPb->SetName("h2D_Acc_Num_NP_pPb");

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  ////// read in Efficiency file
  // *** without pt weight
  TFile *fEffPRPbp = new TFile(Form("../Efficiency/EffAna_%s_Pbp_PR_%s_Zvtx%d_SF%d_privtest.root",szBinning.Data(),szAccCut.Data(),(int)useZvtxWeight,(int)useSF));
  TFile *fEffPRpPb = new TFile(Form("../Efficiency/EffAna_%s_pPb_PR_%s_Zvtx%d_SF%d_privtest.root",szBinning.Data(),szAccCut.Data(),(int)useZvtxWeight,(int)useSF));
  TFile *fEffNPPbp = new TFile(Form("../Efficiency/EffAna_%s_Pbp_NP_%s_Zvtx%d_SF%d_privtest.root",szBinning.Data(),szAccCut.Data(),(int)useZvtxWeight,(int)useSF));
  TFile *fEffNPpPb = new TFile(Form("../Efficiency/EffAna_%s_pPb_NP_%s_Zvtx%d_SF%d_privtest.root",szBinning.Data(),szAccCut.Data(),(int)useZvtxWeight,(int)useSF));
  TH2D* h2D_Eff_PR_Pbp = (TH2D*)fEffPRPbp->Get("h2D_Eff_pt_y");
  TH2D* h2D_Eff_PR_pPb = (TH2D*)fEffPRpPb->Get("h2D_Eff_pt_y");
  TH2D* h2D_Eff_NP_Pbp = (TH2D*)fEffNPPbp->Get("h2D_Eff_pt_y");
  TH2D* h2D_Eff_NP_pPb = (TH2D*)fEffNPpPb->Get("h2D_Eff_pt_y");
  h2D_Eff_PR_Pbp->SetName("h2D_Eff_PR_Pbp");
  h2D_Eff_PR_pPb->SetName("h2D_Eff_PR_pPb");
  h2D_Eff_NP_Pbp->SetName("h2D_Eff_NP_Pbp");
  h2D_Eff_NP_pPb->SetName("h2D_Eff_NP_pPb");
  cout << "4 Eff TH2D : "<<h2D_Eff_PR_Pbp<<" "<<h2D_Eff_PR_pPb<<" "<<h2D_Eff_NP_Pbp<<" "<<h2D_Eff_NP_pPb<<endl;
  TH2D* h2D_Eff_Den_PR_Pbp = (TH2D*)fEffPRPbp->Get("h2D_Den_pt_y");
  TH2D* h2D_Eff_Den_PR_pPb = (TH2D*)fEffPRpPb->Get("h2D_Den_pt_y");
  TH2D* h2D_Eff_Den_NP_Pbp = (TH2D*)fEffNPPbp->Get("h2D_Den_pt_y");
  TH2D* h2D_Eff_Den_NP_pPb = (TH2D*)fEffNPpPb->Get("h2D_Den_pt_y");
  h2D_Eff_Den_PR_Pbp->SetName("h2D_Eff_Den_PR_Pbp");
  h2D_Eff_Den_PR_pPb->SetName("h2D_Eff_Den_PR_pPb");
  h2D_Eff_Den_NP_Pbp->SetName("h2D_Eff_Den_NP_Pbp");
  h2D_Eff_Den_NP_pPb->SetName("h2D_Eff_Den_NP_pPb");
  
  TH2D* h2D_Eff_Num_PR_Pbp = (TH2D*)fEffPRPbp->Get("h2D_Num_pt_y");
  TH2D* h2D_Eff_Num_PR_pPb = (TH2D*)fEffPRpPb->Get("h2D_Num_pt_y");
  TH2D* h2D_Eff_Num_NP_Pbp = (TH2D*)fEffNPPbp->Get("h2D_Num_pt_y");
  TH2D* h2D_Eff_Num_NP_pPb = (TH2D*)fEffNPpPb->Get("h2D_Num_pt_y");
  h2D_Eff_Num_PR_Pbp->SetName("h2D_Eff_Num_PR_Pbp");
  h2D_Eff_Num_PR_pPb->SetName("h2D_Eff_Num_PR_pPb");
  h2D_Eff_Num_NP_Pbp->SetName("h2D_Eff_Num_NP_Pbp");
  h2D_Eff_Num_NP_pPb->SetName("h2D_Eff_Num_NP_pPb");

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  ////// read in from fit file
  TFile* fFitPbp = new TFile(Form("./fitResHist_%s_Pbp_%s_%s_privtest.root",szBinning.Data(),szAccCut.Data(),szSys.Data()));
  TFile* fFitpPb = new TFile(Form("./fitResHist_%s_pPb_%s_%s_privtest.root",szBinning.Data(),szAccCut.Data(),szSys.Data()));
  TH2D* h2D_Fit_PR_Pbp = (TH2D*)fFitPbp->Get("h2D_nPrompt_Raw");  
  TH2D* h2D_Fit_NP_Pbp = (TH2D*)fFitPbp->Get("h2D_nNonPrompt_Raw"); 
  TH2D* h2D_Fit_nSig_Pbp = (TH2D*)fFitPbp->Get("h2D_nSig_Raw"); 
  TH2D* h2D_Fit_nBkg_Pbp = (TH2D*)fFitPbp->Get("h2D_nBkg_Raw"); 
  TH2D* h2D_Fit_bFrac_Pbp = (TH2D*)fFitPbp->Get("h2D_bFraction"); 
  TH2D* h2D_Fit_ctErrmin_Pbp = (TH2D*)fFitPbp->Get("h2D_ctErrmin"); 
  TH2D* h2D_Fit_ctErrmax_Pbp = (TH2D*)fFitPbp->Get("h2D_ctErrmax"); 
  TH2D* h2D_Fit_NoCutEntry_Pbp = (TH2D*)fFitPbp->Get("h2D_NoCutEntry"); 
  TH2D* h2D_Fit_CutEntry_Pbp = (TH2D*)fFitPbp->Get("h2D_CutEntry"); 
  TH2D* h2D_Fit_CutRatio_Pbp = (TH2D*)fFitPbp->Get("h2D_CutRatio"); 
  TH2D* h2D_Fit_PR_pPb = (TH2D*)fFitpPb->Get("h2D_nPrompt_Raw");  
  TH2D* h2D_Fit_NP_pPb = (TH2D*)fFitpPb->Get("h2D_nNonPrompt_Raw"); 
  TH2D* h2D_Fit_nSig_pPb = (TH2D*)fFitpPb->Get("h2D_nSig_Raw"); 
  TH2D* h2D_Fit_nBkg_pPb = (TH2D*)fFitpPb->Get("h2D_nBkg_Raw"); 
  TH2D* h2D_Fit_bFrac_pPb = (TH2D*)fFitpPb->Get("h2D_bFraction"); 
  TH2D* h2D_Fit_ctErrmin_pPb = (TH2D*)fFitpPb->Get("h2D_ctErrmin"); 
  TH2D* h2D_Fit_ctErrmax_pPb = (TH2D*)fFitpPb->Get("h2D_ctErrmax"); 
  TH2D* h2D_Fit_NoCutEntry_pPb = (TH2D*)fFitpPb->Get("h2D_NoCutEntry"); 
  TH2D* h2D_Fit_CutEntry_pPb = (TH2D*)fFitpPb->Get("h2D_CutEntry"); 
  TH2D* h2D_Fit_CutRatio_pPb = (TH2D*)fFitpPb->Get("h2D_CutRatio"); 
  h2D_Fit_PR_Pbp->SetName("h2D_Fit_PR_Pbp");
  h2D_Fit_PR_pPb->SetName("h2D_Fit_PR_pPb");
  h2D_Fit_NP_Pbp->SetName("h2D_Fit_NP_Pbp");
  h2D_Fit_NP_pPb->SetName("h2D_Fit_NP_pPb");
  h2D_Fit_nSig_Pbp->SetName("h2D_Fit_nSig_Pbp");
  h2D_Fit_nSig_pPb->SetName("h2D_Fit_nSig_pPb");
  h2D_Fit_nBkg_Pbp->SetName("h2D_Fit_nBkg_Pbp");
  h2D_Fit_nBkg_pPb->SetName("h2D_Fit_nBkg_pPb");
  h2D_Fit_bFrac_Pbp->SetName("h2D_Fit_bFrac_Pbp");
  h2D_Fit_bFrac_pPb->SetName("h2D_Fit_bFrac_pPb");
  h2D_Fit_ctErrmin_Pbp->SetName("h2D_Fit_ctErrmin_Pbp");
  h2D_Fit_ctErrmin_pPb->SetName("h2D_Fit_ctErrmin_pPb");
  h2D_Fit_ctErrmax_Pbp->SetName("h2D_Fit_ctErrmax_Pbp");
  h2D_Fit_ctErrmax_pPb->SetName("h2D_Fit_ctErrmax_pPb");
  h2D_Fit_NoCutEntry_Pbp->SetName("h2D_Fit_NoCutEntry_Pbp");
  h2D_Fit_NoCutEntry_pPb->SetName("h2D_Fit_NoCutEntry_pPb");
  h2D_Fit_CutEntry_Pbp->SetName("h2D_Fit_CutEntry_Pbp");
  h2D_Fit_CutEntry_pPb->SetName("h2D_Fit_CutEntry_pPb");
  h2D_Fit_CutRatio_Pbp->SetName("h2D_Fit_CutRatio_Pbp");
  h2D_Fit_CutRatio_pPb->SetName("h2D_Fit_CutRatio_pPb");
  
  cout << "4 yield TH2D : "<<h2D_Fit_PR_Pbp<<" "<<h2D_Fit_PR_pPb<<" "<<h2D_Fit_NP_Pbp<<" "<<h2D_Fit_NP_pPb<<endl;
  cout << "2 nSig TH2D : "<<h2D_Fit_nSig_Pbp<<" "<<h2D_Fit_nSig_pPb<<endl;
  cout << "2 nBkg TH2D : "<<h2D_Fit_nBkg_Pbp<<" "<<h2D_Fit_nBkg_pPb<<endl;
  cout << "2 bFrac TH2D : "<<h2D_Fit_bFrac_Pbp<<" "<<h2D_Fit_bFrac_pPb<<endl;
  cout << "2 ctErrmin TH2D : "<<h2D_Fit_ctErrmin_Pbp<<" "<<h2D_Fit_ctErrmin_pPb<<endl;
  cout << "2 ctErrmax TH2D : "<<h2D_Fit_ctErrmax_Pbp<<" "<<h2D_Fit_ctErrmax_pPb<<endl;
  cout << "2 NoCutEntry TH2D : "<<h2D_Fit_NoCutEntry_Pbp<<" "<<h2D_Fit_NoCutEntry_pPb<<endl;
  cout << "2 CutEntry TH2D : "<<h2D_Fit_CutEntry_Pbp<<" "<<h2D_Fit_CutEntry_pPb<<endl;
  cout << "2 CutRatio TH2D : "<<h2D_Fit_CutRatio_Pbp<<" "<<h2D_Fit_CutRatio_pPb<<endl;
  
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  ////// [Data] calculate corrected yield = rawYield/(Acc*Eff)
  TH2D* h2D_CorrY_PR_Pbp = (TH2D*)h2D_Fit_PR_Pbp->Clone("h2D_CorrY_PR_Pbp");
  h2D_CorrY_PR_Pbp->SetName("h2D_CorrY_PR_Pbp");
  h2D_CorrY_PR_Pbp->Divide(h2D_Acc_PR_Pbp);
  h2D_CorrY_PR_Pbp->Divide(h2D_Eff_PR_Pbp);
  TH2D* h2D_CorrY_NP_Pbp = (TH2D*)h2D_Fit_NP_Pbp->Clone("h2D_CorrY_NP_Pbp");
  h2D_CorrY_NP_Pbp->SetName("h2D_CorrY_NP_Pbp");
  h2D_CorrY_NP_Pbp->Divide(h2D_Acc_NP_Pbp);
  h2D_CorrY_NP_Pbp->Divide(h2D_Eff_NP_Pbp);
  TH2D* h2D_CorrY_PR_pPb = (TH2D*)h2D_Fit_PR_pPb->Clone("h2D_CorrY_PR_pPb");
  h2D_CorrY_PR_pPb->SetName("h2D_CorrY_PR_pPb");
  h2D_CorrY_PR_pPb->Divide(h2D_Acc_PR_pPb);
  h2D_CorrY_PR_pPb->Divide(h2D_Eff_PR_pPb);
  TH2D* h2D_CorrY_NP_pPb = (TH2D*)h2D_Fit_NP_pPb->Clone("h2D_CorrY_NP_pPb");
  h2D_CorrY_NP_pPb->SetName("h2D_CorrY_NP_pPb");
  h2D_CorrY_NP_pPb->Divide(h2D_Acc_NP_pPb);
  h2D_CorrY_NP_pPb->Divide(h2D_Eff_NP_pPb);
  
  ////////////////////////////////////////////////
  ////// [MC] corrected yield RECO/(Acc*Eff)
  TH2D* h2D_MCCorrY_PR_Pbp = (TH2D*)h2D_Eff_Num_PR_Pbp->Clone("h2D_MCCorrY_PR_Pbp");
  h2D_MCCorrY_PR_Pbp->SetName("h2D_MCCorrY_PR_Pbp");
  h2D_MCCorrY_PR_Pbp->Divide(h2D_Acc_PR_Pbp);
  h2D_MCCorrY_PR_Pbp->Divide(h2D_Eff_PR_Pbp);
  TH2D* h2D_MCCorrY_NP_Pbp = (TH2D*)h2D_Eff_Num_NP_Pbp->Clone("h2D_MCCorrY_NP_Pbp");
  h2D_MCCorrY_NP_Pbp->SetName("h2D_MCCorrY_NP_Pbp");
  h2D_MCCorrY_NP_Pbp->Divide(h2D_Acc_NP_Pbp);
  h2D_MCCorrY_NP_Pbp->Divide(h2D_Eff_NP_Pbp);
  TH2D* h2D_MCCorrY_PR_pPb = (TH2D*)h2D_Eff_Num_PR_pPb->Clone("h2D_MCCorrY_PR_pPb");
  h2D_MCCorrY_PR_pPb->SetName("h2D_MCCorrY_PR_pPb");
  h2D_MCCorrY_PR_pPb->Divide(h2D_Acc_PR_pPb);
  h2D_MCCorrY_PR_pPb->Divide(h2D_Eff_PR_pPb);
  TH2D* h2D_MCCorrY_NP_pPb = (TH2D*)h2D_Eff_Num_NP_pPb->Clone("h2D_MCCorrY_NP_pPb");
  h2D_MCCorrY_NP_pPb->SetName("h2D_MCCorrY_NP_pPb");
  h2D_MCCorrY_NP_pPb->Divide(h2D_Acc_NP_pPb);
  h2D_MCCorrY_NP_pPb->Divide(h2D_Eff_NP_pPb);
  
  
  ////////////////////////////////////////////////
  ////// save as a root file
  TFile *outFile = new TFile(Form("totalHist_PbppPb_%s_Zvtx%d_SF%d_noPtWeight_privFit_privMC.root",szFinal.Data(),(int)useZvtxWeight,(int)useSF),"RECREATE");
  std::cout << "szFinal: " << szFinal << std::endl;
  cout << "totalHist_PbppPb_"<<szFinal<<"_Zvtx"<<(int)useZvtxWeight<<"_SF"<<(int)useSF<<"_noPtWeight_privFit_privMC.root has been created :) " <<endl; 

  outFile->cd();
  //Acc 
  h2D_Acc_PR_Pbp->Write();
  h2D_Acc_PR_pPb->Write();
  h2D_Acc_NP_Pbp->Write();
  h2D_Acc_NP_pPb->Write();
  //Eff
  h2D_Eff_PR_Pbp->Write();
  h2D_Eff_PR_pPb->Write();
  h2D_Eff_NP_Pbp->Write();
  h2D_Eff_NP_pPb->Write();
  //raw yield
  h2D_Fit_PR_Pbp->Write();  
  h2D_Fit_PR_pPb->Write();  
  h2D_Fit_NP_Pbp->Write();  
  h2D_Fit_NP_pPb->Write();  
  //CorrYield
  h2D_CorrY_PR_Pbp->Write();  
  h2D_CorrY_PR_pPb->Write();  
  h2D_CorrY_NP_Pbp->Write();  
  h2D_CorrY_NP_pPb->Write();  
  
  // DIRECTORY : num, den distributions for acc & eff
  TDirectoryFile *otherMCInfo = new TDirectoryFile("otherMCInfo","otherMCInfo");
  otherMCInfo->cd();
  // acc den & num
  h2D_Acc_Den_PR_Pbp->Write();
  h2D_Acc_Den_PR_pPb->Write();
  h2D_Acc_Den_NP_Pbp->Write();
  h2D_Acc_Den_NP_pPb->Write();
  h2D_Acc_Num_PR_Pbp->Write();
  h2D_Acc_Num_PR_pPb->Write();
  h2D_Acc_Num_NP_Pbp->Write();
  h2D_Acc_Num_NP_pPb->Write();
  // eff den & num
  h2D_Eff_Den_PR_Pbp->Write();
  h2D_Eff_Den_PR_pPb->Write();
  h2D_Eff_Den_NP_Pbp->Write();
  h2D_Eff_Den_NP_pPb->Write();
  h2D_Eff_Num_PR_Pbp->Write();
  h2D_Eff_Num_PR_pPb->Write();
  h2D_Eff_Num_NP_Pbp->Write();
  h2D_Eff_Num_NP_pPb->Write();
  // MC CorrYield
  h2D_MCCorrY_PR_Pbp->Write();  
  h2D_MCCorrY_PR_pPb->Write();  
  h2D_MCCorrY_NP_Pbp->Write();  
  h2D_MCCorrY_NP_pPb->Write();  
  otherMCInfo->Write();
  
  // DIRECTORY : other information for fittng
  outFile->cd();
  TDirectoryFile *otherFitInfo = new TDirectoryFile("otherFitInfo","otherFitInfo");
  otherFitInfo->cd();
  h2D_Fit_nSig_Pbp->Write();  
  h2D_Fit_nSig_pPb->Write();  
  h2D_Fit_nBkg_Pbp->Write();  
  h2D_Fit_nBkg_pPb->Write();  
  h2D_Fit_bFrac_Pbp->Write(); 
  h2D_Fit_bFrac_pPb->Write(); 
  h2D_Fit_ctErrmin_Pbp->Write();  
  h2D_Fit_ctErrmin_pPb->Write();  
  h2D_Fit_ctErrmax_Pbp->Write();  
  h2D_Fit_ctErrmax_pPb->Write();  
  h2D_Fit_NoCutEntry_Pbp->Write();  
  h2D_Fit_NoCutEntry_pPb->Write();  
  h2D_Fit_CutEntry_Pbp->Write();  
  h2D_Fit_CutEntry_pPb->Write();  
  h2D_Fit_CutRatio_Pbp->Write();  
  h2D_Fit_CutRatio_pPb->Write();  
  otherFitInfo->Write();
    
  outFile->Close();
  
  return 0;

} // end of main func

