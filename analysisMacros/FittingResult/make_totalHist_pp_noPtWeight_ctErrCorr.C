#include "../SONGKYO.h"

/////// main func. ///////
int make_totalHist_pp_noPtWeight_ctErrCorr(int MrapNpt=89, int accCutType = 2, bool useZvtxWeight=false, bool useSF = false, TString szSys="nominal"){
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
  //////////////////////////////////////////////////////////////////////////////////////
  ////// read in Acceptance file
  // *** without pt weight
  TFile *fAccPR = new TFile(Form("../Acceptance/AccAna_%s_pp_PR_%s.root",szBinning.Data(),szAccCut.Data()));
  TFile *fAccNP = new TFile(Form("../Acceptance/AccAna_%s_pp_NP_%s.root",szBinning.Data(),szAccCut.Data()));
  TH2D* h2D_Acc_PR_pp = (TH2D*)fAccPR->Get("h2D_Acc_pt_y");
  TH2D* h2D_Acc_NP_pp = (TH2D*)fAccNP->Get("h2D_Acc_pt_y");
  h2D_Acc_PR_pp->SetName("h2D_Acc_PR_pp");
  h2D_Acc_NP_pp->SetName("h2D_Acc_NP_pp");
  cout << "2 Acc TH2D : "<<h2D_Acc_PR_pp<<" "<<h2D_Acc_NP_pp<<endl;
  
  TH2D* h2D_Acc_Den_PR_pp = (TH2D*)fAccPR->Get("h2D_Den_pt_y");
  TH2D* h2D_Acc_Den_NP_pp = (TH2D*)fAccNP->Get("h2D_Den_pt_y");
  h2D_Acc_Den_PR_pp->SetName("h2D_Acc_Den_PR_pp");
  h2D_Acc_Den_NP_pp->SetName("h2D_Acc_Den_NP_pp");
  TH2D* h2D_Acc_Num_PR_pp = (TH2D*)fAccPR->Get("h2D_Num_pt_y");
  TH2D* h2D_Acc_Num_NP_pp = (TH2D*)fAccNP->Get("h2D_Num_pt_y");
  h2D_Acc_Num_PR_pp->SetName("h2D_Acc_Num_PR_pp");
  h2D_Acc_Num_NP_pp->SetName("h2D_Acc_Num_NP_pp");

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  ////// read in Efficiency file
  // *** without pt weight
  TFile *fEffPRpp = new TFile(Form("../Efficiency/EffAna_%s_pp_PR_%s_Zvtx%d_SF%d.root",szBinning.Data(),szAccCut.Data(),(int)useZvtxWeight,(int)useSF));
  TFile *fEffNPpp = new TFile(Form("../Efficiency/EffAna_%s_pp_NP_%s_Zvtx%d_SF%d.root",szBinning.Data(),szAccCut.Data(),(int)useZvtxWeight,(int)useSF));
  TH2D* h2D_Eff_PR_pp = (TH2D*)fEffPRpp->Get("h2D_Eff_pt_y");
  TH2D* h2D_Eff_NP_pp = (TH2D*)fEffNPpp->Get("h2D_Eff_pt_y");
  h2D_Eff_PR_pp->SetName("h2D_Eff_PR_pp");
  h2D_Eff_NP_pp->SetName("h2D_Eff_NP_pp");
  cout << "2 Eff TH2D : "<<h2D_Eff_PR_pp<<" "<<h2D_Eff_NP_pp<<endl;
  TH2D* h2D_Eff_Den_PR_pp = (TH2D*)fEffPRpp->Get("h2D_Den_pt_y");
  TH2D* h2D_Eff_Den_NP_pp = (TH2D*)fEffNPpp->Get("h2D_Den_pt_y");
  h2D_Eff_Den_PR_pp->SetName("h2D_Eff_Den_PR_pp");
  h2D_Eff_Den_NP_pp->SetName("h2D_Eff_Den_NP_pp");
  
  TH2D* h2D_Eff_Num_PR_pp = (TH2D*)fEffPRpp->Get("h2D_Num_pt_y");
  TH2D* h2D_Eff_Num_NP_pp = (TH2D*)fEffNPpp->Get("h2D_Num_pt_y");
  h2D_Eff_Num_PR_pp->SetName("h2D_Eff_Num_PR_pp");
  h2D_Eff_Num_NP_pp->SetName("h2D_Eff_Num_NP_pp");

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  ////// read in from fit file
  TFile* fFitpp = new TFile(Form("./fitResHist_%s_pp_%s_%s.root",szBinning.Data(),szAccCut.Data(),szSys.Data()));
  TH2D* h2D_Fit_PR_pp = (TH2D*)fFitpp->Get("h2D_nPrompt_Raw");  
  TH2D* h2D_Fit_NP_pp = (TH2D*)fFitpp->Get("h2D_nNonPrompt_Raw"); 
  TH2D* h2D_Fit_nSig_pp = (TH2D*)fFitpp->Get("h2D_nSig_Raw"); 
  TH2D* h2D_Fit_nBkg_pp = (TH2D*)fFitpp->Get("h2D_nBkg_Raw"); 
  //TH2D* h2D_Fit_PR_pp = (TH2D*)fFitpp->Get("h2D_nPrompt");  
  //TH2D* h2D_Fit_NP_pp = (TH2D*)fFitpp->Get("h2D_nNonPrompt"); 
  //TH2D* h2D_Fit_nSig_pp = (TH2D*)fFitpp->Get("h2D_nSig"); 
  //TH2D* h2D_Fit_nBkg_pp = (TH2D*)fFitpp->Get("h2D_nBkg"); 
  TH2D* h2D_Fit_bFrac_pp = (TH2D*)fFitpp->Get("h2D_bFraction"); 
  TH2D* h2D_Fit_ctErrmin_pp = (TH2D*)fFitpp->Get("h2D_ctErrmin"); 
  TH2D* h2D_Fit_ctErrmax_pp = (TH2D*)fFitpp->Get("h2D_ctErrmax"); 
  TH2D* h2D_Fit_NoCutEntry_pp = (TH2D*)fFitpp->Get("h2D_NoCutEntry"); 
  TH2D* h2D_Fit_CutEntry_pp = (TH2D*)fFitpp->Get("h2D_CutEntry"); 
  TH2D* h2D_Fit_CutRatio_pp = (TH2D*)fFitpp->Get("h2D_CutRatio"); 
  h2D_Fit_PR_pp->SetName("h2D_Fit_PR_pp");
  h2D_Fit_NP_pp->SetName("h2D_Fit_NP_pp");
  h2D_Fit_nSig_pp->SetName("h2D_Fit_nSig_pp");
  h2D_Fit_nBkg_pp->SetName("h2D_Fit_nBkg_pp");
  h2D_Fit_bFrac_pp->SetName("h2D_Fit_bFrac_pp");
  h2D_Fit_ctErrmin_pp->SetName("h2D_Fit_ctErrmin_pp");
  h2D_Fit_ctErrmax_pp->SetName("h2D_Fit_ctErrmax_pp");
  h2D_Fit_NoCutEntry_pp->SetName("h2D_Fit_NoCutEntry_pp");
  h2D_Fit_CutEntry_pp->SetName("h2D_Fit_CutEntry_pp");
  h2D_Fit_CutRatio_pp->SetName("h2D_Fit_CutRatio_pp");
  
  cout << "2 yield TH2D : "<<h2D_Fit_PR_pp<<" "<<h2D_Fit_NP_pp<<endl;
  cout << "1 nSig TH2D : "<<h2D_Fit_nSig_pp<<" "<<endl;
  cout << "1 nBkg TH2D : "<<h2D_Fit_nBkg_pp<<" "<<endl;
  cout << "1 bFrac TH2D : "<<h2D_Fit_bFrac_pp<<" "<<endl;
  cout << "1 ctErrmin TH2D : "<<h2D_Fit_ctErrmin_pp<<" "<<endl;
  cout << "1 ctErrmax TH2D : "<<h2D_Fit_ctErrmax_pp<<" "<<endl;
  cout << "1 NoCutEntry TH2D : "<<h2D_Fit_NoCutEntry_pp<<" "<<endl;
  cout << "1 CutEntry TH2D : "<<h2D_Fit_CutEntry_pp<<" "<<endl;
  cout << "1 CutRatio TH2D : "<<h2D_Fit_CutRatio_pp<<" "<<endl;
  
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  ////// [Data] calculate corrected yield = rawYield/(Acc*Eff)
  TH2D* h2D_CorrY_PR_pp = (TH2D*)h2D_Fit_PR_pp->Clone("h2D_CorrY_PR_pp");
  h2D_CorrY_PR_pp->SetName("h2D_CorrY_PR_pp");
  h2D_CorrY_PR_pp->Divide(h2D_Acc_PR_pp);
  h2D_CorrY_PR_pp->Divide(h2D_Eff_PR_pp);
  TH2D* h2D_CorrY_NP_pp = (TH2D*)h2D_Fit_NP_pp->Clone("h2D_CorrY_NP_pp");
  h2D_CorrY_NP_pp->SetName("h2D_CorrY_NP_pp");
  h2D_CorrY_NP_pp->Divide(h2D_Acc_NP_pp);
  h2D_CorrY_NP_pp->Divide(h2D_Eff_NP_pp);
  
  ////////////////////////////////////////////////
  ////// [MC] corrected yield RECO/(Acc*Eff)
  TH2D* h2D_MCCorrY_PR_pp = (TH2D*)h2D_Eff_Num_PR_pp->Clone("h2D_MCCorrY_PR_pp");
  h2D_MCCorrY_PR_pp->SetName("h2D_MCCorrY_PR_pp");
  h2D_MCCorrY_PR_pp->Divide(h2D_Acc_PR_pp);
  h2D_MCCorrY_PR_pp->Divide(h2D_Eff_PR_pp);
  TH2D* h2D_MCCorrY_NP_pp = (TH2D*)h2D_Eff_Num_NP_pp->Clone("h2D_MCCorrY_NP_pp");
  h2D_MCCorrY_NP_pp->SetName("h2D_MCCorrY_NP_pp");
  h2D_MCCorrY_NP_pp->Divide(h2D_Acc_NP_pp);
  h2D_MCCorrY_NP_pp->Divide(h2D_Eff_NP_pp);
  
  ////////////////////////////////////////////////
  ////// save as a root file
  TFile *outFile = new TFile(Form("totalHist_pp_%s_Zvtx%d_SF%d_noPtWeight_ctErrCorr.root",szFinal.Data(),(int)useZvtxWeight,(int)useSF),"RECREATE");
  std::cout << "szFinal: " << szFinal << std::endl;
  cout << "totalHist_pp_"<<szFinal<<"_Zvtx"<<(int)useZvtxWeight<<"_SF"<<(int)useSF<<"_noPtWeight_ctErrCorr.root has been created :) " <<endl; 

  outFile->cd();
  //Acc 
  h2D_Acc_PR_pp->Write();
  h2D_Acc_NP_pp->Write();
  //Eff
  h2D_Eff_PR_pp->Write();
  h2D_Eff_NP_pp->Write();
  //raw yield
  h2D_Fit_PR_pp->Write(); 
  h2D_Fit_NP_pp->Write(); 
  //CorrYield
  h2D_CorrY_PR_pp->Write(); 
  h2D_CorrY_NP_pp->Write(); 
  
  // DIRECTORY : num, den distributions for acc & eff
  TDirectoryFile *otherMCInfo = new TDirectoryFile("otherMCInfo","otherMCInfo");
  otherMCInfo->cd();
  // acc den & num
  h2D_Acc_Den_PR_pp->Write();
  h2D_Acc_Den_NP_pp->Write();
  h2D_Acc_Num_PR_pp->Write();
  h2D_Acc_Num_NP_pp->Write();
  // eff den & num
  h2D_Eff_Den_PR_pp->Write();
  h2D_Eff_Den_NP_pp->Write();
  h2D_Eff_Num_PR_pp->Write();
  h2D_Eff_Num_NP_pp->Write();
  // MC CorrYield
  h2D_MCCorrY_PR_pp->Write(); 
  h2D_MCCorrY_NP_pp->Write(); 
  otherMCInfo->Write();
  
  // DIRECTORY : other information for fittng
  outFile->cd();
  TDirectoryFile *otherFitInfo = new TDirectoryFile("otherFitInfo","otherFitInfo");
  otherFitInfo->cd();
  h2D_Fit_nSig_pp->Write(); 
  h2D_Fit_nBkg_pp->Write(); 
  h2D_Fit_bFrac_pp->Write();  
  h2D_Fit_ctErrmin_pp->Write(); 
  h2D_Fit_ctErrmax_pp->Write(); 
  h2D_Fit_NoCutEntry_pp->Write(); 
  h2D_Fit_CutEntry_pp->Write(); 
  h2D_Fit_CutRatio_pp->Write(); 
  otherFitInfo->Write();
    
  outFile->Close();
  
  return 0;

} // end of main func

