#include "../SONGKYO.h"

void make_EffAna_fromMerged(int MrapNpt=89, int isPA = 0, int accCutType =2, bool isPrompt = true, bool useZvtxWeight=false, bool useSF=false)
{
  TString szBinning;
  if (MrapNpt==89)  {szBinning = "8rap9pt"; }
  else if (MrapNpt==83) { szBinning = "8rap3pt"; }
  else if (MrapNpt==63) { szBinning = "6rap3pt"; }
  else if (MrapNpt==62) { szBinning = "6rap2pt"; }
  else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return; }
  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="Pbp";
  else if (isPA==2) szPA="pPb";
  else {cout << "select among isPA = 0, 1, or 2 (pp, Pbp or pPb) "<< endl; return; }
  TString szAccCut;
  if (accCutType==1) szAccCut="oldcut";
  else if (accCutType==2) szAccCut="newcut";
  else {cout << "select among accCutType = 1 or 2"<< endl; return; }  
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  const TString szFinal = Form("%s_%s_%s_%s",szBinning.Data(),szPA.Data(),szPrompt.Data(),szAccCut.Data());
  std::cout << "szFinal: " << szFinal << std::endl;

  TFile * f2D = new TFile(Form("./toBeMerged_divideEvt/merged_EffAna_%s_Zvtx%d_SF%d_ptWeight.root",szFinal.Data(),(int)useZvtxWeight,(int)useSF));
  //cout << "f2D = " << f2D->GetName() << endl;
  
  ///////////////////////////////////////////////////////////////
  TH1D* h1D_zVtx;
  h1D_zVtx = (TH1D*)f2D->Get("h1D_zVtx");
  
  TH2D* h2D_Den_pt_y_noPtWeight;
  TH2D* h2D_Num_pt_y_noPtWeight;
  TH2D* h2D_Eff_pt_y_noPtWeight;
  h2D_Den_pt_y_noPtWeight = (TH2D*)f2D->Get("h2D_Den_pt_y_noPtWeight");
  h2D_Num_pt_y_noPtWeight = (TH2D*)f2D->Get("h2D_Num_pt_y_noPtWeight");
  h2D_Eff_pt_y_noPtWeight = (TH2D*)h2D_Num_pt_y_noPtWeight->Clone("h2D_Eff_pt_y_noPtWeight");
  // (Num/Den) to get efficiency (B : binomial error)
  h2D_Eff_pt_y_noPtWeight->Divide(h2D_Num_pt_y_noPtWeight,h2D_Den_pt_y_noPtWeight,1,1,"B"); 
  
  TH2D* h2D_Den_pt_y;
  TH2D* h2D_Num_pt_y;
  TH2D* h2D_Eff_pt_y;
  h2D_Den_pt_y = (TH2D*)f2D->Get("h2D_Den_pt_y");
  h2D_Num_pt_y = (TH2D*)f2D->Get("h2D_Num_pt_y");
  h2D_Eff_pt_y = (TH2D*)h2D_Num_pt_y->Clone("h2D_Eff_pt_y");
  // (Num/Den) to get efficiency (B : binomial error)
  h2D_Eff_pt_y->Divide(h2D_Num_pt_y,h2D_Den_pt_y,1,1,"B"); 
 
  cout << "XIA" <<endl; 
  TH2D* h2D_Den_pt_y_fine;
  TH2D* h2D_Num_pt_y_fine;
  TH2D* h2D_Eff_pt_y_fine;
  if (MrapNpt==89) {
    h2D_Den_pt_y_fine = (TH2D*)f2D->Get("h2D_Den_pt_y_fine");
    h2D_Num_pt_y_fine = (TH2D*)f2D->Get("h2D_Num_pt_y_fine");
    h2D_Eff_pt_y_fine = (TH2D*)h2D_Num_pt_y_fine->Clone("h2D_Eff_pt_y_fine");
    // (Num/Den) to get efficiency (B : binomial error)
    h2D_Eff_pt_y_fine->Divide(h2D_Num_pt_y_fine,h2D_Den_pt_y_fine,1,1,"B"); 
  }  
  cout << "XIA" <<endl; 
  
  ////////////////////////////////////////////////////////////////////////////
  // Save the data!
  
  TFile *outFile = new TFile(Form("EffAna_%s_Zvtx%d_SF%d_ptWeight.root",szFinal.Data(), (int)useZvtxWeight,(int)useSF),"RECREATE");
  std::cout << "szFinal: " << szFinal << std::endl;
  outFile->cd();
  h2D_Den_pt_y_noPtWeight->Write();
  h2D_Num_pt_y_noPtWeight->Write();
  h2D_Eff_pt_y_noPtWeight->Write();
  h2D_Den_pt_y->Write();
  h2D_Num_pt_y->Write();
  h2D_Eff_pt_y->Write();
  h1D_zVtx->Write();
  if (MrapNpt==89) {
    h2D_Den_pt_y_fine->Write();
    h2D_Num_pt_y_fine->Write();
    h2D_Eff_pt_y_fine->Write();
  }
  outFile->Close();

  delete h2D_Den_pt_y; 
  delete h2D_Num_pt_y; 
  delete h2D_Eff_pt_y; 
  if (MrapNpt==89) {
    delete h2D_Den_pt_y_fine; 
    delete h2D_Num_pt_y_fine; 
    delete h2D_Eff_pt_y_fine; 
  }
  delete h1D_zVtx;
  
  return;

}

