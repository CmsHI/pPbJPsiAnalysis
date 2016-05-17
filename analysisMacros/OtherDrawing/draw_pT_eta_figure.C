#include "../SONGKYO.h"

double tfAcceptable(double *x, double *par);
//double tfAcceptable(double *x);

int draw_pT_eta_figure() {

  gROOT->Macro("../Style.C");
  gStyle->SetTitleXOffset(1.10);
  gStyle->SetTitleYOffset(1.15);

  TCanvas* c1 = new TCanvas("c1","",600,600);
  c1->SetGridx();
  c1->SetGridy();

  //Dummy hist
  TH1D* hDummy = new TH1D("hDummy","hDummy",48,-2.4,2.4);
  hDummy->GetXaxis()->SetRangeUser(-3.,3.);
  hDummy->GetYaxis()->SetRangeUser(0.,4.0);
  hDummy->GetXaxis()->SetTitle("#eta_{lab}^{#mu}");
  hDummy->GetXaxis()->CenterTitle();
  hDummy->GetYaxis()->SetTitle("p_{T}^{#mu} (GeV/c)");
  //hDummy->GetYaxis()->SetNdivisions(520);
  hDummy->GetYaxis()->SetNdivisions(510);
  hDummy->Draw();

  // Draw sgl mu acc cut lines

/*  
  // 1) old acceptance cut
  TLine* tOrig1 = new TLine(-2.2 ,0.64, -2.2, 0.8);
  tOrig1->SetLineWidth(4);
  tOrig1->SetLineColor(kGreen+1);
  TLine* tOrig2 = new TLine(2.2 ,0.64, 2.2, 0.8);
  tOrig2->SetLineWidth(4);
  tOrig2->SetLineColor(kGreen+1);
  TLine* tOrig3 = new TLine(-1.3 ,1.475, -1.3, 3.3);
  tOrig3->SetLineWidth(4);
  tOrig3->SetLineColor(kGreen+1);
  TLine* tOrig4 = new TLine(1.3 ,1.475, 1.3, 3.3);
  tOrig4->SetLineWidth(4);
  tOrig4->SetLineColor(kGreen+1);
  //solidLine(-1.3,1.475,-1.3,3.3,2,3.);
  //solidLine(1.3,1.475,1.3,3.3,2,3.);
  TF1* fOrig_v1 = new TF1("fOrig_v1","0.8",-2.4,-2.2);
  TF1* fOrig_v2 = new TF1("fOrig_v2","0.8",2.2,2.4);
  TF1* fOrig_v3 = new TF1("fOrig_v3","3.3",-1.3,1.3);
  TF1* fOrig_v4 = new TF1("fOrig_v4","2.9/cosh(x)",-2.2,-1.3);
  TF1* fOrig_v5 = new TF1("fOrig_v5","2.9/cosh(x)",1.3,2.2);
  fOrig_v1->SetLineColor(kGreen+1);
  fOrig_v2->SetLineColor(kGreen+1);
  fOrig_v3->SetLineColor(kGreen+1);
  fOrig_v4->SetLineColor(kGreen+1);
  fOrig_v5->SetLineColor(kGreen+1);
*/

  // 2) new acceptance cut
  TLine* tCutG1 = new TLine(-1.2 ,2.6, -1.2, 3.3);
  tCutG1->SetLineWidth(4);
  tCutG1->SetLineColor(kRed);
  TLine* tCutG2 = new TLine(1.2 ,2.6, 1.2, 3.3);
  tCutG2->SetLineWidth(4);
  tCutG2->SetLineColor(kRed);
  TLine* tCutG3 = new TLine(-2.1 ,1.3, -2.1, 1.6);
  tCutG3->SetLineWidth(4);
  tCutG3->SetLineColor(kRed);
  TLine* tCutG4 = new TLine(2.1 ,1.3, 2.1, 1.6);
  tCutG4->SetLineWidth(4);
  tCutG4->SetLineColor(kRed);
  TF1* fCutG_v1 = new TF1("fCutG_v1","1.3",-2.4,-2.1);
  TF1* fCutG_v2 = new TF1("fCutG_v2","1.3",2.1,2.4);
  TF1* fCutG_v3 = new TF1("fCutG_v3","3.3",-1.2,1.2);
  //TF1* fCutG_v4 = new TF1("fCutG_v4","-(1.0/0.9)*(x)+1.2*(1.0/0.9)+2.6",1.2,2.1);
  //TF1* fCutG_v5 = new TF1("fCutG_v5","-(1.0/0.9)*(-x)+1.2*(1.0/0.9)+2.6",-2.1,-1.2);
  TF1* fCutG_v4 = new TF1("fCutG_v4","3.93-1.11*(x)",1.2,2.1);
  TF1* fCutG_v5 = new TF1("fCutG_v5","3.93-1.11*(-x)",-2.1,-1.2);
  fCutG_v1->SetLineColor(kRed);
  fCutG_v2->SetLineColor(kRed);
  fCutG_v3->SetLineColor(kRed);
  fCutG_v4->SetLineColor(kRed);
  fCutG_v5->SetLineColor(kRed);
  fCutG_v1->SetLineWidth(4);
  fCutG_v2->SetLineWidth(4);
  fCutG_v3->SetLineWidth(4);
  fCutG_v4->SetLineWidth(4);
  fCutG_v5->SetLineWidth(4);
  
  /*
  // cf) Cut 2015 Glb
  TF1* fCutGlb_v1 = new TF1("fCutG_v3","3.4",-1.0,1.0);
  TF1* fCutGlb_v2 = new TF1("fCutG_v4","5.8-2.4*(x)",1.0,1.5);
  TF1* fCutGlb_v3 = new TF1("fCutG_v5","5.8-2.4*(-x)",-1.5,-1.0);
  TF1* fCutGlb_v4 = new TF1("fCutG_v2","3.3667-7.0/9.0*(x)",1.5,2.4);
  TF1* fCutGlb_v5 = new TF1("fCutG_v1","3.3667-7.0/9.0*(-x)",-2.4,-1.5);
  fCutGlb_v1->SetLineStyle(7);
  fCutGlb_v2->SetLineStyle(7);
  fCutGlb_v3->SetLineStyle(7);
  fCutGlb_v4->SetLineStyle(7);
  fCutGlb_v5->SetLineStyle(7);
  */

  ////// pre-filter for cross-check
  TF1* fAfter = new TF1("fAfter","2.5/cosh(x)",-2.4, 2.4);
  fAfter->SetLineColor(kBlue);
  fAfter->SetLineWidth(4);
  
  TF1* fAfter32 = new TF1("fAfter32","3.2/cosh(x)",-2.4, 2.4);
  fAfter32->SetLineColor(kViolet);
  fAfter32->SetLineWidth(4);
  
  TF1* fAfter12 = new TF1("fAfter12","1.2",-2.4, 2.4);
  fAfter12->SetLineColor(kCyan);
  fAfter12->SetLineWidth(4);

  ///////////////////////// drawing //////////////////////////
  /*
  fOrig_v1->Draw("same");
  fOrig_v2->Draw("same");
  fOrig_v3->Draw("same");
  fOrig_v4->Draw("same");
  fOrig_v5->Draw("same");
  tOrig1->Draw("same");
  tOrig2->Draw("same");
  tOrig3->Draw("same");
  tOrig4->Draw("same");
*/  
  fCutG_v1->Draw("same");
  fCutG_v2->Draw("same");
  fCutG_v3->Draw("same");
  fCutG_v4->Draw("same");
  fCutG_v5->Draw("same");
  tCutG1->Draw("same");
  tCutG2->Draw("same");
  tCutG3->Draw("same");
  tCutG4->Draw("same");
/*
  fCutGlb_v1->Draw("same");
  fCutGlb_v2->Draw("same");
  fCutGlb_v3->Draw("same");
  fCutGlb_v4->Draw("same");
  fCutGlb_v5->Draw("same");
  
*/
  fAfter->Draw("same");

//  TLegend *legB = new TLegend(0.38,0.19,0.64,0.30,NULL,"brNDC");
  TLegend *legB = new TLegend(0.33,0.19,0.64,0.30,NULL,"brNDC");
  SetLegendStyle(legB);
  legB->SetTextSize(0.030);
  //legB->AddEntry(fOrig_v1,"(old cut)","l");
  legB->AddEntry(fCutG_v1,"muon acceptance cut","l");
  //legB->AddEntry(fCutGlb_v1,"GlbTrk acceptance cut","l");
  legB->AddEntry(fAfter,"pre-filter at GEN level","l");
  legB->Draw();

  c1->SaveAs("pT_eta_figure_two_prefilter_cutG.pdf");
  c1->SaveAs("pT_eta_figure_two_prefilter_cutG.png");
  //c1->SaveAs("pT_eta_figure_three_prefilter_old_cutG.pdf");
  //c1->SaveAs("pT_eta_figure_three_prefilter_old_cutG.png");
  
  //TF1* fTmp = new TF1("fTmp","cosh(x)",-2.4, 2.4);
  //fTmp->Draw("same");
  
  return 0;
}

double tfAcceptable(double *x, double *par){
//double tfAcceptable(double *x){
  return par[0]*exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2]) ) );
//  return 2.9/cosh(x[0]);
}

