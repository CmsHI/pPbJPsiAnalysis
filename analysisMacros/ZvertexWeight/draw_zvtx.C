#include "../SONGKYO.h"
#include <TChain.h>
#include <TF1.h>

double tfGaus(double *x, double *par);
double tf2Gaus(double *x, double *par);
double tfDivideGaus(double *x, double *par);
double tfLine(double *x, double *par);

int draw_zvtx(TString szFunction = "G1", int isPA = 0, bool isPrompt = true, bool isNewAccCut = true, bool isOfficial = true){

  gROOT->Macro("../Style.C");
  double nbin = 60;
  double binmax = 30;
  double fitmax1 = 8;
  double fitmax2 = 15;

  TString szPA;
  if (isPA==0) szPA = "pp";
  else if (isPA==1) szPA = "Pbp";
  else if (isPA==2) szPA = "pPb";
  else { cout << "select isPA among 0, 1 and 2 " << endl; return 0; }
  
  TFile* f01 = new TFile(Form("zVtx201604/zVtx_%s_%s_isPrompt%d_isNewAccCut%d_isOfficial%d.root",szFunction.Data(),szPA.Data(),(int)isPrompt,(int)isNewAccCut,(int)isOfficial));

  TCanvas* c01 = new TCanvas("c01","distributions",600,600); c01->SetGridx(); c01->SetGridy();
  TCanvas* c02 = new TCanvas("c02","ratios",600,600); c02->SetGridx(); c02->SetGridy();

  cout << "nbin = " << nbin << endl;
  TH1D* h1D_01 = (TH1D*)f01->Get("h1D_01");
  TH1D* h1D_02 = (TH1D*)f01->Get("h1D_02");
  TH1D* hRatio = (TH1D*)f01->Get("hRatio");
  
  //define fitting function
  TF1* g1 = (TF1*)f01->Get("g1");
  TF1* g2 = (TF1*)f01->Get("g2");
  TF1* gRatio = (TF1*)f01->Get("gRatio");
 
  TLegend *legU1 = new TLegend(0.17,0.79,0.50,0.93,NULL,"brNDC");
  SetLegendStyle(legU1);
  TLegend *legU2 = new TLegend(0.63,0.79,0.90,0.93,NULL,"brNDC");
  SetLegendStyle(legU2);
  
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  //Draw
  c01->cd();
  gPad->SetLogy(0);
  SetHistStyle(h1D_01,1,10);
  SetHistStyle(h1D_02,2,13);
  h1D_01->SetFillColor(kRed+2);
  h1D_02->SetFillColor(kBlue+1);
  h1D_01->SetAxisRange(0.0,0.1,"Y");

  h1D_01->GetXaxis()->SetTitle("Z_{vtx} (cm)");
  h1D_01->Draw("pe");
  g1->SetLineColor(kRed+2);
  g1->Draw("same");
  h1D_02->Draw("pe same");
  g2->SetLineColor(kBlue+1);
  g2->Draw("same");
    
  legU1->AddEntry(h1D_01,Form("%s_Data",szPA.Data()),"lp");
  if (isPrompt) legU2->AddEntry(h1D_02,Form("%s_PRMC",szPA.Data()),"lp");
  legU1->Draw();
  legU2->Draw();
  
  Double_t par01[3]; //for h01
  Double_t par02[3]; //for h02
  g1->GetParameters(&par01[0]);
  g2->GetParameters(&par02[0]);
  latex->DrawLatex(0.20, 0.80, Form("mean : %.2f",par01[1])); 
  latex->DrawLatex(0.20, 0.74, Form("sigma : %.2f",par01[2]));
  latex->DrawLatex(0.67, 0.80, Form("mean : %.2f",par02[1])); 
  latex->DrawLatex(0.67, 0.74, Form("sigma : %.2f",par02[2]));
    
  c01->SaveAs(Form("zVtx201604/zVtxFit_%s_%s_isPrompt%d_isNewAccCut%d_isOfficial%d.pdf",szFunction.Data(),szPA.Data(),(int)isPrompt,(int)isNewAccCut,(int)isOfficial));
  //c01->Clear();
  //legU->Clear();

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // divide two TF
  c02->cd();

  //ratio
  gPad->SetLogy(0);
  hRatio->GetYaxis()->SetRangeUser(0.,2.0);
  hRatio->GetYaxis()->SetTitle("Ratio = Data/MC");
  SetHistStyle(hRatio,1,0);
  hRatio->SetMarkerSize(1.4);
  hRatio->SetMarkerColor(kBlack);
  hRatio->SetLineColor(kBlack);
  hRatio->Draw("pe");
  gRatio->Draw("same");
  
  dashedLine(-binmax,1.,binmax,1.,1,1);
  
  c02->SaveAs(Form("zVtx201604/zVtxFitRatio_%s_%s_isPrompt%d_isNewAccCut%d_isOfficial%d.pdf",szFunction.Data(),szPA.Data(),(int)isPrompt,(int)isNewAccCut,(int)isOfficial));

  //Save as a root file
  /*
  TFile* outFile = new TFile(Form("zVtx201604/zVtx_%s_%s_isPrompt%d_isNewAccCut%d_isOfficial%d.root",szFunction.Data(),szPA.Data(),(int)isPrompt,(int)isNewAccCut,(int)isOfficial),"RECREATE");
  outFile->cd();
  h1D_01->Write();
  h1D_02->Write();
  hRatio->Write();
  g1->Write();
  g2->Write();
  gRatio->Write();
  outFile->Close();
*/
  return 0; 

}

double tfGaus(double *x, double *par) {
  return par[0]*exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2]) ) );
}
double tf2Gaus(double *x, double *par) {
  return par[0]*exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2])) ) + par[3]*exp( -0.5*( ((x[0]-par[4])*(x[0]-par[4]))/(par[5]*par[5])) );
}

double tfDivideGaus(double *x, double *par) {
  return (par[0]*exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2]) ) )) / (par[3]*exp( -0.5*( ((x[0]-par[4])*(x[0]-par[4]))/(par[5]*par[5]) )) );
}


double tfLine(double *x, double *par) {
  return par[0]*x[0]+par[1];
}
