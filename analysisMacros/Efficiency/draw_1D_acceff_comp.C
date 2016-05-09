#include "../SONGKYO.h"

// no ordering in rap (just y_lab)
void draw_1D_acceff_comp(TString szDir = "dir_acceff_comp", bool isNoErr=false)
{
  gROOT->Macro("../Style.C");

  //// read-in file (ratio would be f01Eff/f02Eff )
 /* 
  int isPA =0; //0:pp, 1:Pbp, 2:pPb for drawing dashed line at low pT limit
  TFile* f01Acc = new TFile("../Acceptance/dir_acc/8rap9pt_pp_PR_newcut_AccPt.root");
  TFile* f02Acc = new TFile("../Acceptance/dir_acc/8rap9pt_pp_PR_newcut_AccPt.root");
  TFile* f01Eff = new TFile("../Efficiency/dir_eff/8rap9pt_pp_PR_newcut_Zvtx0_SF0_EffPt.root");
  TFile* f02Eff = new TFile("../Efficiency/dir_eff/8rap9pt_pp_PR_newcut_Zvtx0_SF0_EffPt.root");
  TString szF01="ppPRpure";
  TString szF02="ppNPpure";
*/
  int isPA =1; //0:pp, 1:Pbp, 2:pPb for drawing dashed line at low pT limit
  TFile* f01Acc = new TFile("../Acceptance/dir_acc/8rap9pt_pA_PR_newcut_AccPt.root");
  TFile* f02Acc = new TFile("../Acceptance/dir_acc/8rap9pt_pA_PR_newcut_AccPt.root");
  TFile* f01Eff = new TFile("../Efficiency/dir_eff/8rap9pt_Pbp_PR_newcut_Zvtx0_SF0_EffPt.root");
  TFile* f02Eff = new TFile("../Efficiency/dir_eff/8rap9pt_Pbp_PR_newcut_Zvtx0_SF0_EffPt.root");
  TString szF01="paPRpure";
  TString szF02="paNPpure";
  
  double ratiomin=0.9; 
  double ratiomax=1.1; 
  //double ratiomin=0.7; 
  //double ratiomax=1.3; 
  //// end of "to-be-modified" region

  const TString szFinal = Form("%s_%s",szF01.Data(),szF02.Data());
  cout << "szFinal = " << szFinal << endl;

  //// read-in tree
  TTree *tr = (TTree*)f01Eff->Get("tRap");
  int nEdge;
  double rapEdge[531];
  TBranch *b_nEdge;
  TBranch *b_rapEdge;
  tr->SetBranchAddress("nEdge",&nEdge,&b_nEdge);
  tr->SetBranchAddress("rapEdge",&rapEdge,&b_rapEdge);
  cout << "tr entries = " << tr->GetEntries() << endl;
  for (int ev=0; ev <tr->GetEntries(); ev++){
    tr->GetEntry(ev);
  }
  cout << "nEdge = " << nEdge << endl;
  for (Int_t i=0; i< nEdge; i++) {
    cout << "rapEdge["<<i<<"] = " << rapEdge[i] << endl;
  }
  //// read-in histos
  const int nRap = nEdge -1 ; 
  cout << "nRap = " << nRap << endl;
  TH1D* h1D_01_Acc[nRap]; 
  TH1D* h1D_02_Acc[nRap]; 
  TH1D* h1D_01_Eff[nRap]; 
  TH1D* h1D_02_Eff[nRap]; 
  TH1D* h1D_01[nRap]; 
  TH1D* h1D_02[nRap]; 
  TH1D *hRatio[nRap]; //f01Eff/f02Eff
  for (int iy=0; iy < nRap; iy ++){
    h1D_01_Acc[iy] = (TH1D*)f01Acc->Get(Form("h1D_AccPt_%d",iy));
    h1D_02_Acc[iy] = (TH1D*)f02Acc->Get(Form("h1D_AccPt_%d",iy));
    h1D_01_Eff[iy] = (TH1D*)f01Eff->Get(Form("h1D_EffPt_%d",iy));
    h1D_02_Eff[iy] = (TH1D*)f02Eff->Get(Form("h1D_EffPt_%d",iy));
    cout << "h1D_01_Acc["<<iy<<"] = " << h1D_01_Acc[iy] << endl;
    cout << "h1D_02_Acc["<<iy<<"] = " << h1D_02_Acc[iy] << endl;
    cout << "h1D_01_Eff["<<iy<<"] = " << h1D_01_Eff[iy] << endl;
    cout << "h1D_02_Eff["<<iy<<"] = " << h1D_02_Eff[iy] << endl;
    h1D_01[iy]=(TH1D*)h1D_01_Acc[iy]->Clone(Form("h1D_01_%d",iy)); 
    h1D_02[iy]=(TH1D*)h1D_02_Acc[iy]->Clone(Form("h1D_02_%d",iy)); 
    h1D_01[iy]->Multiply(h1D_01_Eff[iy]); 
    h1D_02[iy]->Multiply(h1D_02_Eff[iy]); 
    hRatio[iy]=(TH1D*)h1D_01[iy]->Clone(Form("hRatio_%d",iy));
    hRatio[iy]->Divide(h1D_02[iy]);
  }
  
  //////////////////////////////////////////////////////////////////
  //// Draw histograms
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  TCanvas* c1 = new TCanvas("c1","c1",1600,800);
  c1->Divide(4,2);
  TCanvas* c2 = new TCanvas("c2","c2",1600,800);
  c2->Divide(4,2);
  TLegend *legBR = new TLegend(0.50,0.30,0.80,0.41,NULL,"brNDC");
  SetLegendStyle(legBR);

  for (Int_t iy = 0; iy < nRap; iy++) {
    //// 1)  distributions
    c1->cd(iy+1);
    SetHistStyle(h1D_01[iy],3,0);
    SetHistStyle(h1D_02[iy],4,10);
    h1D_01[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
    h1D_01[iy]->GetYaxis()->SetTitle("Efficiency");
    //h1D_01[iy]->SetMinimum(0.);
    h1D_01[iy]->SetMaximum(1.);
    //h1D_01[iy]->SetMaximum(0.1);
    h1D_01[iy]->GetXaxis()->SetRangeUser(0., 30.);
    h1D_01[iy]->Draw("pe");
    //h1D_02[iy]->Draw("pe same");
    //// draw line at lot pT limit
    if (isPA==0) {
      if (iy==7 || iy==0) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy ==6 || iy==1) dashedLine (3.,0.,3.,1.,2,1);
      else if (iy ==5 || iy==4 || iy==3 || iy==2)  dashedLine (6.5,0.,6.5,1.,2,1);
    }
    else if (isPA==1) {
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,2,1);
      else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,0.,6.5,1.,2,1);
      else if (iy==5)  dashedLine (5.,0.,5.,1.,2,1);
    }
    else if (isPA==2){
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,2,1);
      else if (iy==2)  dashedLine (5.,0.,5.,1.,2,1);
      else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,1);
    }
    latex->DrawLatex(0.53,0.25,szFinal.Data());
    latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
    if (iy==0) {
      legBR->AddEntry(h1D_01[iy],szF01.Data(),"lp");
      legBR->AddEntry(h1D_02[iy],szF02.Data(),"lp");
      //legBR->Draw();
    }
    
    //// 2) ratio f01Eff/f02Eff
    c2->cd(iy+1);
    SetHistStyle(hRatio[iy],5,0);
    hRatio[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
    hRatio[iy]->GetYaxis()->SetTitle(Form("[ %s ]/[ %s ]",szF01.Data(),szF02.Data()));
    hRatio[iy]->SetMinimum(ratiomin);
    hRatio[iy]->SetMaximum(ratiomax);
    hRatio[iy]->GetXaxis()->SetRangeUser(0.,30.);
    if (isNoErr) {
      for (int ipt=0; ipt<9; ipt++) {
        hRatio[iy]->SetBinError(ipt+1,0.);
      }
    }
    hRatio[iy]->Draw("pe");
    dashedLine(0.,1.,30.,1.,1,1);
    
    //// draw line at lot pT limit
    if (isPA==0) {
      if (iy==1) dashedLine (2.,ratiomin,2.,ratiomax,2,1);
      else if (iy ==2 || iy==7) dashedLine (3.,ratiomin,3.,ratiomax,2,1);
      else if (iy==3)  dashedLine (5.,ratiomin,5.,ratiomax,2,1);
      else if (iy==4 || iy==5 || iy==6)  dashedLine (6.5,ratiomin,6.5,ratiomax,2,1);
    }
    else if (isPA==1) {
      if (iy==0 || iy==7) dashedLine (2.,ratiomin,2.,ratiomax,2,1);
      else if (iy==1 || iy==6) dashedLine (3.,ratiomin,3.,ratiomax,2,1);
      else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,ratiomin,6.5,ratiomax,2,1);
      else if (iy==5)  dashedLine (5.,ratiomin,5.,ratiomax,2,1);
    }
    else if (isPA==2){
      if (iy==0 || iy==7) dashedLine (2.,ratiomin,2.,ratiomax,2,1);
      else if (iy==1 || iy==6) dashedLine (3.,ratiomin,3.,ratiomax,2,1);
      else if (iy==2)  dashedLine (5.,ratiomin,5.,ratiomax,2,1);
      else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,ratiomin,6.5,ratiomax,2,1);
    }
    latex->DrawLatex(0.53,0.25,szFinal.Data());
    latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));

  }

  c1->SaveAs(Form("%s/comp_acceff_%s.pdf",szDir.Data(),szFinal.Data()));
  //c1->SaveAs(Form("%s/comp_acceff_%s_zoomed.pdf",szDir.Data(),szFinal.Data()));
  c2->SaveAs(Form("%s/ratio_acceff_%s.pdf",szDir.Data(),szFinal.Data()));

  return;

}

