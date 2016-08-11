#include "../SONGKYO.h"

int draw_1D_eff(int MrapNpt=89, int isPA = 1, int accCutType =2, bool isPrompt = true, bool useZvtxWeight=false, bool useSF=false, TString szDir="dir_eff")
{
  gROOT->Macro("../Style.C");
  
  TString szBinning;
  if (MrapNpt==89)  {szBinning = "8rap9pt"; }
  else if (MrapNpt==83) { szBinning = "8rap3pt"; }
  else if (MrapNpt==63) { szBinning = "6rap3pt"; }
  else if (MrapNpt==62) { szBinning = "6rap2pt"; }
  else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return 0; }
  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="Pbp";
  else if (isPA==2) szPA="pPb";
  else {cout << "select among isPA = 0, 1, or 2 (pp, Pbp or pPb) "<< endl; return 0; }
  TString szAccCut;
  if (accCutType==1) szAccCut="oldcut";
  else if (accCutType==2) szAccCut="newcut";
  else {cout << "select among accCutType = 1 or 2"<< endl; return 0; }  
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  const TString szFinal = Form("%s_%s_%s_%s",szBinning.Data(),szPA.Data(),szPrompt.Data(),szAccCut.Data());
  std::cout << "szFinal: " << szFinal << std::endl;

  TFile * f2D = new TFile(Form("EffAna_%s_Zvtx%d_SF%d.root",szFinal.Data(),(int)useZvtxWeight,(int)useSF));

  TH2D* h2D_Den;
  TH2D* h2D_Num;
  TH2D* h2D_Eff;
  h2D_Den = (TH2D*)f2D->Get("h2D_Den_pt_y");
  h2D_Num = (TH2D*)f2D->Get("h2D_Num_pt_y");
  h2D_Eff = (TH2D*)f2D->Get("h2D_Eff_pt_y");
  const int nRap = h2D_Eff->GetNbinsX();
  const int nPt = h2D_Eff->GetNbinsY();

  //// read bin edges (rapidity) and fill to Tree
  int nEdge = nRap+1; 
  double rapEdge[nEdge];
  for (Int_t ie = 0; ie < nEdge; ie++) {
    rapEdge[ie] = h2D_Eff->GetXaxis()->GetBinLowEdge(ie+1);
    cout << "rapEdge["<<ie<<"] = " << rapEdge[ie] << endl;
  }
  TTree* tRap = new TTree("tRap","");
  tRap->Branch("nEdge",&nEdge,"nEdge/I");
  tRap->Branch("rapEdge",&rapEdge,"rapEdge[nEdge]/D");
  tRap->Fill();
  
  //// projection to 1D hist
  TH1D* h1D_DenPt[nRap]; 
  TH1D* h1D_NumPt[nRap]; 
  TH1D* h1D_EffPt[nRap]; 
  for (Int_t iy = 0; iy < nRap; iy++) {
    h1D_DenPt[iy] = h2D_Den->ProjectionY(Form("h1D_DenPt_%d",iy),iy+1,iy+1);
    h1D_NumPt[iy] = h2D_Num->ProjectionY(Form("h1D_NumPt_%d",iy),iy+1,iy+1);
    h1D_EffPt[iy] = h2D_Eff->ProjectionY(Form("h1D_EffPt_%d",iy),iy+1,iy+1);
  }

  //latex box for beam, rapidity, pT info
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  //////////////////////////////////////////////////////////////////
  // --- Draw histograms
  //// 1) Denominator
  TCanvas* c1 = new TCanvas("c1","c1",1600,800);
  c1->Divide(4,2);
  for (Int_t iy = 0; iy < nRap; iy++) {
    c1->cd(iy+1);
    gPad->SetLogy(1); 
    SetHistStyle(h1D_DenPt[iy],1,0);
    h1D_DenPt[iy]->Scale(1,"width");
    h1D_DenPt[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
    h1D_DenPt[iy]->GetYaxis()->SetTitle("Denominator");
    h1D_DenPt[iy]->GetXaxis()->SetRangeUser(0.,30.);
    //h1D_DenPt[iy]->SetMinimum(0.);
    //h1D_DenPt[iy]->SetMaximum(1.);
    h1D_DenPt[iy]->Draw("pe");
    //// draw line at low pT limit
    if (isPA==0) {
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy ==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy ==2 || iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,1);
    }
    else if (isPA==1) {
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,0.,6.5,1.,2,1);
      else if (iy==5)  dashedLine (5.,0.,5.,1.,2,1);
    }
    else if (isPA==2){
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy==2)  dashedLine (5.,0.,5.,1.,2,1);
      else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,1);
    }
    latex->DrawLatex(0.50,0.31,Form("%s",szFinal.Data()));
    latex->DrawLatex(0.50,0.25,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
    //latex->DrawLatex(0.50,0.21,Form("%.2f < y_{CM} < %.2f",-1*rapEdge[iy+1]-0.47,-1*rapEdge[iy]-0.47));
  }
  c1->SaveAs(Form("%s/%s_Zvtx%d_SF%d_DenPt.pdf",szDir.Data(),szFinal.Data(),(int)useZvtxWeight,(int)useSF));
  
  //// 2) Numerator
  TCanvas* c2 = new TCanvas("c2","c2",1600,800);
  c2->Divide(4,2);
  for (Int_t iy = 0; iy < nRap; iy++) {
    c2->cd(iy+1);
    gPad->SetLogy(1); 
    SetHistStyle(h1D_NumPt[iy],1,0);
    h1D_NumPt[iy]->Scale(1,"width");
    h1D_NumPt[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
    h1D_NumPt[iy]->GetYaxis()->SetTitle("Numerator");
    h1D_NumPt[iy]->GetXaxis()->SetRangeUser(0.,30.);
    //h1D_NumPt[iy]->SetMinimum(0.);
    //h1D_NumPt[iy]->SetMaximum(1.);
    h1D_NumPt[iy]->Draw("pe");
    //// draw line at low pT limit
    if (isPA==0) {
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy ==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy ==2 || iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,1);
    }
    else if (isPA==1) {
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,0.,6.5,1.,2,1);
      else if (iy==5)  dashedLine (5.,0.,5.,1.,2,1);
    }
    else if (isPA==2){
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy==2)  dashedLine (5.,0.,5.,1.,2,1);
      else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,1);
    }
    latex->DrawLatex(0.50,0.31,Form("%s",szFinal.Data()));
    latex->DrawLatex(0.50,0.25,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
    //latex->DrawLatex(0.50,0.21,Form("%.2f < y_{CM} < %.2f",-1*rapEdge[iy+1]-0.47,-1*rapEdge[iy]-0.47));
  }
  c2->SaveAs(Form("%s/%s_Zvtx%d_SF%d_NumPt.pdf",szDir.Data(),szFinal.Data(),(int)useZvtxWeight,(int)useSF));
  
  
  //// 3) Efficiency 
  TCanvas* c3 = new TCanvas("c3","c3",1600,800);
  c3->Divide(4,2);
  for (Int_t iy = 0; iy < nRap; iy++) {
    c3->cd(iy+1);
    SetHistStyle(h1D_EffPt[iy],1,0);
    h1D_EffPt[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
    h1D_EffPt[iy]->GetYaxis()->SetTitle("Efficiency");
    h1D_EffPt[iy]->GetXaxis()->SetRangeUser(0.,30.);
    h1D_EffPt[iy]->SetMinimum(0.);
    h1D_EffPt[iy]->SetMaximum(1.);
    h1D_EffPt[iy]->Draw("pe");
    //// draw line at low pT limit
    if (isPA==0) {
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy ==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy ==2 || iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,1);
    }
    else if (isPA==1) {
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,0.,6.5,1.,2,1);
      else if (iy==5)  dashedLine (5.,0.,5.,1.,2,1);
    }
    else if (isPA==2){
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy==2)  dashedLine (5.,0.,5.,1.,2,1);
      else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,1);
    }
    latex->DrawLatex(0.50,0.31,Form("%s",szFinal.Data()));
    latex->DrawLatex(0.50,0.25,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
    //latex->DrawLatex(0.50,0.21,Form("%.2f < y_{CM} < %.2f",-1*rapEdge[iy+1]-0.47,-1*rapEdge[iy]-0.47));
  }
  c3->SaveAs(Form("%s/%s_Zvtx%d_SF%d_EffPt.pdf",szDir.Data(),szFinal.Data(),(int)useZvtxWeight,(int)useSF));
  
  //// save as a root file
  cout << "szFinal = " << szFinal << endl;
  TFile *fOut = new TFile(Form("%s/%s_Zvtx%d_SF%d_EffPt.root",szDir.Data(),szFinal.Data(),(int)useZvtxWeight,(int)useSF),"RECREATE");
  fOut->cd();
  for (Int_t iy = 0; iy < nRap; iy++) {
    h1D_EffPt[iy]->Write();
  }
  tRap->Write();
  fOut->Close();

  return 0;

}
