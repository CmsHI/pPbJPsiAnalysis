#include "../SONGKYO.h"

void draw_1D_vsZvtx(int isPA = 2, TString szDir = "dir_vsZvtx")
{
  gROOT->Macro("../Style.C");

  // --- read-in file
  TFile * f01;
  TFile * f02;
  TString f01name="prompt";
  TString f02name="non-prompt";

  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="Pbp";
  else if (isPA==2) szPA="pPb";
  else {cout << "select among isPA = 0, 1, or 2 (pp, Pbp or pPb) "<< endl; return; }
  
  f01 = new TFile(Form("EffAna_8rap9pt_%s_PR_newcut_Zvtx0_SF0_vsZvtx.root",szPA.Data()));
  f02 = new TFile(Form("EffAna_8rap9pt_%s_NP_newcut_Zvtx0_SF0_vsZvtx.root",szPA.Data()));
  
  const int nRap = 8;
  TH1D* h1D_Eff01[nRap];
  TH1D* h1D_Eff02[nRap];
  for (int ir=0; ir <nRap; ir++) {
    h1D_Eff01[ir] = (TH1D*)f01->Get(Form("h1D_Eff_zVtx_%d",ir));
    h1D_Eff02[ir] = (TH1D*)f02->Get(Form("h1D_Eff_zVtx_%d",ir));
    h1D_Eff01[ir]->SetName(Form("h1D_Eff01_%d",ir));
    h1D_Eff02[ir]->SetName(Form("h1D_Eff02_%d",ir));
    cout << ir << "th h1D_Eff01 = " << h1D_Eff01[ir] << endl;
    cout << ir << "th h1D_Eff02 = " << h1D_Eff02[ir] << endl;
    h1D_Eff01[ir]->GetYaxis()->SetRangeUser(0.,1.);
  }
  
  //// for vsZvtx
  const int nrap = nRap;
  Double_t ptLimitPP[9]= {2.0, 4.0, 6.5, 6.5, 6.5, 6.5, 4.0, 2.0};
  Double_t ptLimitPbp[9]= {2.0, 4.0, 6.5, 6.5, 6.5, 5.0, 4.0, 2.0};
  Double_t ptLimitpPb[9] = {2.0, 4.0, 5.0, 6.5, 6.5, 6.5, 4.0, 2.0};
  Double_t ptLimit[nrap];
  for (Int_t iy=0; iy<nRap; iy++){
    if (isPA==0) ptLimit[iy]=ptLimitPP[iy];
    else if (isPA==1) ptLimit[iy]=ptLimitPbp[iy];
    else ptLimit[iy]=ptLimitpPb[iy];
    cout << iy <<"th ptLimit = " << ptLimit[iy] <<endl;
  }
  
  Double_t rapEdge[nRap+1];
  Double_t rapEdgePP[9] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4}; // 8rap9pt
  Double_t rapEdgePbp[9] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // 8rap9pt
  Double_t rapEdgepPb[9] = {-2.4, -1.93, -1.46, -1.03, -0.43, 0.47, 1.37, 1.97, 2.4};
  for (Int_t iy=0; iy<nRap; iy++){ 
    if (isPA==0) rapEdge[iy]=rapEdgePP[iy];
    else if (isPA==1) rapEdge[iy]=rapEdgePbp[iy];
    else rapEdge[iy]=rapEdgepPb[iy];
    cout << iy <<"th rapEdge = " << rapEdge[iy] <<endl;
  }

  TLegend *legMR = new TLegend(0.53,0.50,0.87,0.67,NULL,"brNDC");
  TLegend *legBR = new TLegend(0.53,0.20,0.87,0.37,NULL,"brNDC");
  SetLegendStyle(legMR);
  SetLegendStyle(legBR);

  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  //////////////////////////////////////////////////////////////////
  // --- Draw histograms
  //TCanvas* c1 = new TCanvas("c1","c1",600,600);
  //c1->cd();
  TCanvas* c1 = new TCanvas("c1","c1",1600,800);
  c1->Divide(4,2);

  for (Int_t iy = 0; iy < nRap; iy++) {
    c1->cd(iy+1);
    //SetHistStyle(h1D_Eff01[iy],0,0);
    h1D_Eff01[iy]->GetXaxis()->SetTitle("primary Z vertex (cm)");
    h1D_Eff01[iy]->GetYaxis()->SetTitle("Efficiency");
    h1D_Eff01[iy]->SetMinimum(0.);
    h1D_Eff01[iy]->SetMaximum(1.);
    SetHistStyle(h1D_Eff01[iy],1,0);
    SetHistStyle(h1D_Eff02[iy],2,10);
    h1D_Eff01[iy]->Draw("pe");
    h1D_Eff02[iy]->Draw("pe same");
    //if (iy==0) latex->DrawLatex(0.50,0.87,sampleName);
    //latex->DrawLatex(0.50,0.80,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
    //latex->DrawLatex(0.50,0.73,Form("%.1f < p_{T} <30 GeV/c",ptLimit[iy]));
    latex->DrawLatex(0.53,0.87,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
    latex->DrawLatex(0.53,0.80,Form("%.1f < p_{T} <30 (GeV/c)",ptLimit[iy]));
    if (iy==0){
      legBR->AddEntry(h1D_Eff01[iy],f01name,"lp");
      legBR->AddEntry(h1D_Eff02[iy],f02name,"lp");
      legBR->Draw();
    }
  }

  c1->SaveAs(Form("%s/%s_vsZvtx.pdf",szDir.Data(),szPA.Data()));

  return;

}

