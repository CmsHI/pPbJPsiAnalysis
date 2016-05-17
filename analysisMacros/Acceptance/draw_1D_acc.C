#include "../SONGKYO.h"

//// for acceptance, use Pbp only
int draw_1D_acc(int MrapNpt=89, int isPA = 0, bool isPrompt=false, int accCutType =2, TString szDir="dir_acc")
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
  else if (isPA==1) szPA="pA";
  else {cout << "select among isPA = 0 or 1 only (pA instead of Pbp or pPb) "<< endl; return 0; }
  TString szAccCut;
  if (accCutType==1) szAccCut="oldcut";
  else if (accCutType==2) szAccCut="newcut";
  else {cout << "select among accCutType = 1 or 2"<< endl; return 0; }  
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  const TString szFinal = Form("%s_%s_%s_%s",szBinning.Data(),szPA.Data(),szPrompt.Data(),szAccCut.Data());
  std::cout << "szFinal: " << szFinal << std::endl;

  TFile* f2D = new TFile(Form("AccAna_%s.root",szFinal.Data()));

  TH2D* h2D_Acc;
  if (isPA==0) h2D_Acc = (TH2D*)f2D->Get("h2D_Acc_pt_y");
  else h2D_Acc = (TH2D*)f2D->Get("h2D_Acc_pt_y_Pbp");
  const int nRap = h2D_Acc->GetNbinsX();
  const int nPt = h2D_Acc->GetNbinsY();

  //// read bin edges (rapidity) and fill to Tree
  int nEdge = nRap+1; 
  double rapEdge[nEdge];
  for (Int_t ie = 0; ie < nEdge; ie++) {
    rapEdge[ie] = h2D_Acc->GetXaxis()->GetBinLowEdge(ie+1);
    cout << "rapEdge["<<ie<<"] = " << rapEdge[ie] << endl;
  }
  TTree* tRap = new TTree("tRap","");
  tRap->Branch("nEdge",&nEdge,"nEdge/I");
  tRap->Branch("rapEdge",&rapEdge,"rapEdge[nEdge]/D");
  tRap->Fill();
  
  //// projection to 1D hist
  TH1D* h1D_AccPt[nRap]; 
  for (Int_t iy = 0; iy < nRap; iy++) {
    h1D_AccPt[iy] = h2D_Acc->ProjectionY(Form("h1D_AccPt_%d",iy),iy+1,iy+1);
  }

  //latex box for beam, rapidity, pT info
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  //////////////////////////////////////////////////////////////////
  // --- Draw histograms
  TCanvas* c1 = new TCanvas("c1","c1",1600,800);
  c1->Divide(4,2);
  for (Int_t iy = 0; iy < nRap; iy++) {
    c1->cd(iy+1);
    SetHistStyle(h1D_AccPt[iy],1,0);
    h1D_AccPt[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
    h1D_AccPt[iy]->GetYaxis()->SetTitle("Acceptance");
    h1D_AccPt[iy]->GetXaxis()->SetRangeUser(0.,30.);
    h1D_AccPt[iy]->SetMinimum(0.);
    h1D_AccPt[iy]->SetMaximum(1.);
    h1D_AccPt[iy]->Draw("pe");
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
  c1->SaveAs(Form("%s/%s_AccPt.pdf",szDir.Data(),szFinal.Data()));
  
  //// save as a root file
  cout << "szFinal = " << szFinal << endl;
  TFile *fOut = new TFile(Form("%s/%s_AccPt.root",szDir.Data(),szFinal.Data()),"RECREATE");
  fOut->cd();
  for (Int_t iy = 0; iy < nRap; iy++) {
    h1D_AccPt[iy]->Write();
  }
  tRap->Write();
  fOut->Close();

  return 0;

}
