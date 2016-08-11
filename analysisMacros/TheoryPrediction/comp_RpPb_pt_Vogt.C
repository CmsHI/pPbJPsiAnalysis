#include "CMS_lumi.h"
#include "../SONGKYO.h"

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_RpPb_pt_Vogt(double ptmax=32, bool isLine=true, bool isSmoothened=false)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 1;  // 0:pp, 1:pPb
	int iPos=0;

  bool isPrompt=true;
	
  ///////////////////////////////////////////////////
	///////// from Ramona
	const int nRapRpPb = 7;
	const int nPtRpPb = 49;
  Double_t theory_px[nRapRpPb][nPtRpPb]; 
	Double_t theory_py[nRapRpPb][nPtRpPb];
  Double_t theory_exlow[nRapRpPb][nPtRpPb];
  Double_t theory_exhigh[nRapRpPb][nPtRpPb];
	Double_t theory_eylow_tmp[nRapRpPb][nPtRpPb];
	Double_t theory_eyhigh_tmp[nRapRpPb][nPtRpPb];
  Double_t theory_eylow[nRapRpPb][nPtRpPb];
	Double_t theory_eyhigh[nRapRpPb][nPtRpPb];
  Double_t ptlimit[nRapRpPb] = {4.0, 6.5, 6.5, 6.5, 6.5, 5.0, 4.0};
  
  ///////////////////////////////////////////////////////////////////
  //// read-in txt
  ///////////////////////////////////////////////////////////////////
  
  string headers;
  TString pxdum, pydum, eylow_tmpdum, eyhigh_tmpdum;
  int counts=0;
  
  /// iy=0 (1.5 < y < 1.93) 
  std::ifstream f0("./fromRamona/kyo_rppb_pt_0.dat",std::ios::in);
  getline(f0, headers); // remove prefix
  counts=0;
  while(!f0.eof()) {
    f0 >> pxdum >> pydum >> eyhigh_tmpdum >> eylow_tmpdum;
    //cout << pxdum << pydum << eyhigh_tmpdum << eylow_tmpdum << endl;
    theory_px[0][counts] =atof(pxdum.Data());
    theory_py[0][counts] =atof(pydum.Data());
    theory_eylow_tmp[0][counts] =atof(eylow_tmpdum.Data());
    theory_eyhigh_tmp[0][counts] =atof(eyhigh_tmpdum.Data());
    counts++;
  } //end of while file open
  
  /// iy=1 
  std::ifstream f1("./fromRamona/kyo_rppb_pt_1.dat",std::ios::in);
  getline(f1, headers); // remove prefix
  counts =0;
  while(!f1.eof()) {
    f1 >> pxdum >> pydum >> eyhigh_tmpdum >> eylow_tmpdum;
    //cout << pxdum << pydum << eyhigh_tmpdum << eylow_tmpdum << endl;
    theory_px[1][counts] =atof(pxdum.Data());
    theory_py[1][counts] =atof(pydum.Data());
    theory_eylow_tmp[1][counts] =atof(eylow_tmpdum.Data());
    theory_eyhigh_tmp[1][counts] =atof(eyhigh_tmpdum.Data());
    counts++;
  } //end of while file open

  /// iy=2 
  std::ifstream f2("./fromRamona/kyo_rppb_pt_2.dat",std::ios::in);
  getline(f2, headers); // remove prefix
  counts =0;
  while(!f2.eof()) {
    f2 >> pxdum >> pydum >> eyhigh_tmpdum >> eylow_tmpdum;
    //cout << pxdum << pydum << eyhigh_tmpdum << eylow_tmpdum << endl;
    theory_px[2][counts] =atof(pxdum.Data());
    theory_py[2][counts] =atof(pydum.Data());
    theory_eylow_tmp[2][counts] =atof(eylow_tmpdum.Data());
    theory_eyhigh_tmp[2][counts] =atof(eyhigh_tmpdum.Data());
    counts++;
  } //end of while file open
 
  /// iy=3 
  std::ifstream f3("./fromRamona/kyo_rppb_pt_3.dat",std::ios::in);
  getline(f3, headers); // remove prefix
  counts =0;
  while(!f3.eof()) {
    f3 >> pxdum >> pydum >> eyhigh_tmpdum >> eylow_tmpdum;
    //cout << pxdum << pydum << eyhigh_tmpdum << eylow_tmpdum << endl;
    theory_px[3][counts] =atof(pxdum.Data());
    theory_py[3][counts] =atof(pydum.Data());
    theory_eylow_tmp[3][counts] =atof(eylow_tmpdum.Data());
    theory_eyhigh_tmp[3][counts] =atof(eyhigh_tmpdum.Data());
    counts++;
  } //end of while file open
  
  /// iy=4 
  std::ifstream f4("./fromRamona/kyo_rppb_pt_4.dat",std::ios::in);
  getline(f4, headers); // remove prefix
  counts =0;
  while(!f4.eof()) {
    f4 >> pxdum >> pydum >> eyhigh_tmpdum >> eylow_tmpdum;
    //cout << pxdum << pydum << eyhigh_tmpdum << eylow_tmpdum << endl;
    theory_px[4][counts] =atof(pxdum.Data());
    theory_py[4][counts] =atof(pydum.Data());
    theory_eylow_tmp[4][counts] =atof(eylow_tmpdum.Data());
    theory_eyhigh_tmp[4][counts] =atof(eyhigh_tmpdum.Data());
    counts++;
  } //end of while file open

  /// iy=5 
  std::ifstream f5("./fromRamona/kyo_rppb_pt_5.dat",std::ios::in);
  getline(f5, headers); // remove prefix
  counts =0;
  while(!f5.eof()) {
    f5 >> pxdum >> pydum >> eyhigh_tmpdum >> eylow_tmpdum;
    //cout << pxdum << pydum << eyhigh_tmpdum << eylow_tmpdum << endl;
    theory_px[5][counts] =atof(pxdum.Data());
    theory_py[5][counts] =atof(pydum.Data());
    theory_eylow_tmp[5][counts] =atof(eylow_tmpdum.Data());
    theory_eyhigh_tmp[5][counts] =atof(eyhigh_tmpdum.Data());
    counts++;
  } //end of while file open
  
  /// iy=6 (-2.4 < y < -1.93) 
  std::ifstream f6("./fromRamona/kyo_rppb_pt_6.dat",std::ios::in);
  getline(f6, headers); // remove prefix
  counts =0;
  while(!f6.eof()) {
    f6 >> pxdum >> pydum >> eyhigh_tmpdum >> eylow_tmpdum;
    //cout << pxdum << pydum << eyhigh_tmpdum << eylow_tmpdum << endl;
    theory_px[6][counts] =atof(pxdum.Data());
    theory_py[6][counts] =atof(pydum.Data());
    theory_eylow_tmp[6][counts] =atof(eylow_tmpdum.Data());
    theory_eyhigh_tmp[6][counts] =atof(eyhigh_tmpdum.Data());
    counts++;
  } //end of while file open

  /////////////////////////////////////////////////////////////////////////////////

 for (int ir = 0 ; ir < nRapRpPb; ir ++ ) {
    for (Int_t ipt=0; ipt<nPtRpPb; ipt++) {
      theory_exlow[ir][ipt] = 0.5;
      theory_exhigh[ir][ipt] = 0.5;
		  //theory_exlow[ir][ipt] = fabs(theory_px[ir][ipt] - theory_exlow_tmp[ir][ipt]);
		  //theory_exhigh[ir][ipt] = fabs(theory_px[ir][ipt] - theory_exhigh_tmp[ir][ipt]);
		  theory_eylow[ir][ipt] = fabs(theory_py[ir][ipt] - theory_eylow_tmp[ir][ipt]);
		  theory_eyhigh[ir][ipt] = fabs(theory_py[ir][ipt] - theory_eyhigh_tmp[ir][ipt]);
      //cout << "theory_px = " << theory_px[ir][ipt] << endl; 	
      //cout << "theory_py = " << theory_py[ir][ipt] << endl; 	
      //cout << "theory_eylow_tmp = " << theory_eylow_tmp[ir][ipt] << endl; 	
      //cout << "theory_eyhigh_tmp = " << theory_eyhigh_tmp[ir][ipt] << endl; 	
    }
  }
 
 //// set unused values as zero 
 
 for (int ir = 0 ; ir < nRapRpPb; ir ++ ) {
    for (Int_t ipt=nPtRpPb-1; ipt>=0; ipt--) {
      if (theory_px[ir][ipt] <ptlimit[ir]-1.) {
        theory_px[ir][ipt] = theory_px[ir][ipt+1];
        theory_py[ir][ipt] = theory_py[ir][ipt+1];
        //theory_exlow[ir][ipt] = 0;
        //theory_exhigh[ir][ipt] = 0;
        theory_eylow[ir][ipt] = theory_eylow[ir][ipt];
        theory_eyhigh[ir][ipt] = theory_eyhigh[ir][ipt];
      }
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  
  TGraphAsymmErrors* g_RpPb_theory[nRapRpPb];
  for (int ir = 0 ; ir < nRapRpPb; ir ++ ) {
    g_RpPb_theory[ir]= new TGraphAsymmErrors(nPtRpPb, theory_px[ir], theory_py[ir], theory_exlow[ir], theory_exhigh[ir], theory_eylow[ir], theory_eyhigh[ir]);	
  }
 /* 
  for (int iy=0; iy<nRapRpPb; iy++) {
	  g_RpPb_theory_dummy[iy]->GetXaxis()->SetLimits(0.,ptmax);
	  g_RpPb_theory_dummy[iy]->SetMinimum(0.0);
	  g_RpPb_theory_dummy[iy]->SetMaximum(1.8);
    g_RpPb_theory_dummy[iy]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    g_RpPb_theory_dummy[iy]->GetXaxis()->CenterTitle();
    g_RpPb_theory_dummy[iy]->GetYaxis()->SetTitle("R_{FB}");
    g_RpPb_theory_dummy[iy]->GetYaxis()->CenterTitle();
    //g_RpPb_theory_dummy[iy]->SetFillColorAlpha(kYellow,0.5);
    g_RpPb_theory_dummy[iy]->SetFillColorAlpha(kWhite,0.5);
	  //g_RpPb_theory_dummy[iy]->SetLineColor(kOrange+7);
	  //g_RpPb_theory_dummy[iy]->SetFillStyle(3004);
    if(!isLine) g_RpPb_theory_dummy[iy]->SetLineWidth(0);
  }
 */ 
  
	///////////////////////////////////////////////////
  //////// experimental points	
  TFile *inFile = new TFile("../DrawFinalPlot/plot_RpPb/RpPb_pt_isPrompt1.root");
  
  TGraphAsymmErrors* g_RpPb_sys[nRapRpPb];
  TGraphAsymmErrors* g_RpPb[nRapRpPb];
  for (int ir = 0 ; ir < nRapRpPb; ir ++ ) {
    g_RpPb_sys[ir] = (TGraphAsymmErrors*)inFile->Get(Form("g_RpPb_sys_%d",ir));
    g_RpPb[ir] = (TGraphAsymmErrors*)inFile->Get(Form("g_RpPb_%d",ir));
  } 
  
	///////////////////////////////////////////////////
	//// Draw
  ///////////////////////////////////////////////////
  
  TCanvas* c_all = new TCanvas("c_all","c_all",1200,680);
  //CMS_lumi( c_all, isPA, iPos );
  c_all->Divide(5,2);
  const int nPad = 10;
  double xmargin = 0.00;
  double ymargin = 0.00;
  double xpad0 = 0.060;
  double xpadw = 0.233;
  TVirtualPad* pad_all[nPad]; // 2 pads for y axis, 8 pads for actual plots
  pad_all[0] = new TPad("pad_all_0", "",0, 0.506, xpad0, 1.0);
  pad_all[1] = new TPad("pad_all_1", "",xpad0, 0.506, xpad0+xpadw, 1.0);
  pad_all[2] = new TPad("pad_all_2", "",xpad0+xpadw, 0.506, xpad0+2*xpadw, 1.0);
  pad_all[3] = new TPad("pad_all_3", "",xpad0+2*xpadw, 0.506, xpad0+3*xpadw, 1.0);
  pad_all[4] = new TPad("pad_all_4", "",xpad0+3*xpadw, 0.506, xpad0+4*xpadw, 1.0);
  pad_all[5] = new TPad("pad_all_5", "",0, 0.0, xpad0, 0.506);
  pad_all[6] = new TPad("pad_all_6", "",xpad0, 0.0, xpad0+xpadw, 0.506);
  pad_all[7] = new TPad("pad_all_7", "",xpad0+xpadw, 0.0, xpad0+2*xpadw, 0.506);
  pad_all[8] = new TPad("pad_all_8", "",xpad0+2*xpadw, 0.0, xpad0+3*xpadw, 0.506);
  pad_all[9] = new TPad("pad_all_9", "",xpad0+3*xpadw, 0.0, xpad0+4*xpadw, 0.506);
  
  double topmargin = 0.14;
  double bottommargin = 0.161;
  for (Int_t iy = 0; iy < nPad; iy++) { 
    pad_all[iy]->Draw();
    if (iy<=4) { 
      pad_all[iy]->SetTopMargin(topmargin); 
      pad_all[iy]->SetBottomMargin(0.0); 
    }
    else { 
      pad_all[iy]->SetTopMargin(0.0); 
      pad_all[iy]->SetBottomMargin(bottommargin); 
    }
    if (iy==0 || iy==5) { 
      //pad_all[iy]->SetLeftMargin(0.20);
      //pad_all[iy]->IsTransparent();
      pad_all[iy]->SetFillStyle(4000);
      pad_all[iy]->SetFrameFillStyle(4000);
      //pad_all[iy]->SetFillColorAlpha(kWhite,1.);
    }
    else {pad_all[iy]->SetLeftMargin(0.0);}
    if (!(iy==4 || iy==9)) { pad_all[iy]->SetRightMargin(0.0);}
    pad_all[iy]->SetLeftMargin(0.0);
    pad_all[iy]->SetRightMargin(0.0);
  }
  
  for (int iy=0; iy<nRapRpPb; iy++) {
    if (iy==0) pad_all[3]->cd();
    else if (iy==1) pad_all[2]->cd();
    else if (iy==2) pad_all[1]->cd();
    else pad_all[iy+3]->cd();
    g_RpPb_sys[iy]->Draw("A5");
	  g_RpPb_theory[iy]->GetXaxis()->SetLimits(ptlimit[iy],ptmax);
	  //g_RpPb_theory[iy]->SetRange(ptlimit[iy],ptmax);
    //g_RpPb_theory[iy]->SetFillColorAlpha(kGray,0.5);
    g_RpPb_theory[iy]->SetFillColorAlpha(kYellow,0.5);
	  g_RpPb_theory[iy]->SetLineColor(kOrange+7);
    g_RpPb[iy]->Draw("p");
    //g_RpPb_theory[iy]->Draw("5");
    g_RpPb_theory[iy]->Draw("3");
    dashedLine(0.,1.,32.,1.,1,1);
  }
    
  c_all->SaveAs("plot_theory/comp_RpPb_pt_Vogt.pdf");
  c_all->SaveAs("plot_theory/comp_RpPb_pt_Vogt.png");
  #if 0 
  ///////////////////////////////////////////////////////////////////////
	
  //TLegend *legBL = new TLegend(0.50, 0.18, 0.70, 0.39);
  TLegend *legBL1 = new TLegend(0.20, 0.18, 0.40, 0.34);
  TLegend *legBL2 = new TLegend(0.20, 0.18, 0.40, 0.34);
  TLegend *legBL3 = new TLegend(0.20, 0.18, 0.40, 0.34);
	SetLegendStyle(legBL1);
	SetLegendStyle(legBL2);
	SetLegendStyle(legBL3);
	legBL1->SetTextSize(0.05);
	legBL2->SetTextSize(0.05);
	legBL3->SetTextSize(0.05);
	
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);
  
  ///////////////// CANVAS 1  
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  c1->cd(); 
  g_RpPb_sys[0]->Draw("A5");
	if (isSmoothened) g_RpPb_theory[0]->Draw("3");
  else g_RpPb_theory[0]->Draw("5");
  g_RpPb[0]->Draw("P");
	dashedLine(0.,1.,32.,1.,1,1);
	
  //// Legend for experiments	
  TLegendEntry *le3=legBL1->AddEntry("le3","1.5 < |y_{CM}| < 1.93","lpf");
	le3->SetFillColor(kGreen-10);
	le3->SetFillStyle(1001);
	le3->SetLineColor(kGreen+3);
	le3->SetMarkerStyle(kFullDiamond);
	le3->SetMarkerColor(kGreen+3);
	le3->SetMarkerSize(3.3);
  TLegendEntry *ent3_thr=legBL1->AddEntry("ent3_thr","EPS09 NLO + CEM (Vogt)","f");
	ent3_thr->SetFillColor(kYellow);
  ent3_thr->SetFillStyle(3001);
//  ent3_thr->SetFillStyle(3004);
  if(!isLine) ent3_thr->SetLineWidth(0);
  ent3_thr->SetLineColor(kOrange+7);
	legBL1->Draw();
  
  //globtex->SetTextAlign(32); //3:right 2:vertical center
	//globtex->SetTextFont(42);
  //globtex->SetTextSize(0.05);
  //globtex->DrawLatex(0.9, 0.23, "1.5 < |y_{CM}| < 1.93");
	
	globtex->SetTextAlign(12); //1:left, 2:vertical center
	globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");

	CMS_lumi( c1, isPA, iPos );
	c1->Update();
  if (isSmoothened) {
    c1->SaveAs("plot_theory/comp_RpPb_pt_Vogt_smoothened_rap1.pdf");
    c1->SaveAs("plot_theory/comp_RpPb_pt_Vogt_smoothened_rap1.png");
  } else {
    c1->SaveAs("plot_theory/comp_RpPb_pt_Vogt_rap1.pdf");
    c1->SaveAs("plot_theory/comp_RpPb_pt_Vogt_rap1.png");
  }
  
  ///////////////// CANVAS 2	
  
  TCanvas* c2 = new TCanvas("c2","c2",600,600);
	c2->cd();
	g_RpPb_sys[1]->Draw("A5");
	if (isSmoothened) g_RpPb_theory[1]->Draw("3");
	else g_RpPb_theory[1]->Draw("5");
	g_RpPb[1]->Draw("P");
	dashedLine(0.,1.,32.,1.,1,1);
  
  //// Legend for experiments	
  TLegendEntry *le2=legBL2->AddEntry("le2","0.9 < |y_{CM}| < 1.5","lpf");
	le2->SetFillColor(kRed-10);
	le2->SetFillStyle(1001);
	le2->SetLineColor(kPink-6);
	le2->SetMarkerStyle(kFullSquare);
	le2->SetMarkerColor(kPink-6);
	le2->SetMarkerSize(2.1);
  TLegendEntry *ent2_thr=legBL2->AddEntry("ent2_thr","EPS09 NLO + CEM (Vogt)","f");
	ent2_thr->SetFillColor(kYellow);
  ent2_thr->SetFillStyle(3001);
//  ent2_thr->SetFillStyle(3004);
  if(!isLine) ent2_thr->SetLineWidth(0);
  ent2_thr->SetLineColor(kOrange+7);
	legBL2->Draw();
  
  //globtex->SetTextAlign(32); //3:right 2:vertical center
	//globtex->SetTextFont(42);
  //globtex->SetTextSize(0.05);
  //globtex->DrawLatex(0.9, 0.23, "0.9 < |y_{CM}| < 1.5");
  
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");
	
  CMS_lumi( c2, isPA, iPos );
	c2->Update();
  
  if (isSmoothened) {
    c2->SaveAs("plot_theory/comp_RpPb_pt_Vogt_smoothened_rap2.pdf");
    c2->SaveAs("plot_theory/comp_RpPb_pt_Vogt_smoothened_rap2.png");
  } else {
    c2->SaveAs("plot_theory/comp_RpPb_pt_Vogt_rap2.pdf");
    c2->SaveAs("plot_theory/comp_RpPb_pt_Vogt_rap2.png");
  }
	
  ///////////////// CANVAS 3	
  
  TCanvas* c3 = new TCanvas("c3","c3",600,600);
	c3->cd();
	g_RpPb_sys[2]->Draw("A5");
	if (isSmoothened) g_RpPb_theory[2]->Draw("3");
	else g_RpPb_theory[2]->Draw("5");
	g_RpPb[2]->Draw("P");
	dashedLine(0.,1.,32.,1.,1,1);
  
  //// Legend for experiments	
  TLegendEntry *le1=legBL3->AddEntry("le1","0 < |y_{CM}| < 0.9","lpf");
	le1->SetFillColor(kBlue-10);
	le1->SetFillStyle(1001);
	le1->SetLineColor(kBlue-3);
	le1->SetMarkerStyle(kFullCircle);
	le1->SetMarkerColor(kBlue-3);
	le1->SetMarkerSize(2.1);
  TLegendEntry *ent1_thr=legBL3->AddEntry("ent1_thr","EPS09 NLO + CEM (Vogt)","f");
	ent1_thr->SetFillColor(kYellow);
  ent1_thr->SetFillStyle(3001);
//  ent1_thr->SetFillStyle(3004);
  if(!isLine) ent1_thr->SetLineWidth(0);
  ent1_thr->SetLineColor(kOrange+7);
	legBL3->Draw();
  
  //globtex->SetTextAlign(32); //3:right 2:vertical center
	//globtex->SetTextFont(42);
  //globtex->SetTextSize(0.05);
  //globtex->DrawLatex(0.9, 0.23, "0.0 < |y_{CM}| < 0.9");

	globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");

  CMS_lumi( c3, isPA, iPos );
	c3->Update();

  if (isSmoothened) {
    c3->SaveAs("plot_theory/comp_RpPb_pt_Vogt_smoothened_rap3.pdf");
    c3->SaveAs("plot_theory/comp_RpPb_pt_Vogt_smoothened_rap3.png");
  } else {
    c3->SaveAs("plot_theory/comp_RpPb_pt_Vogt_rap3.pdf");
    c3->SaveAs("plot_theory/comp_RpPb_pt_Vogt_rap3.png");
  }
#endif
  return;
}

void CMS_lumi( TPad* pad, int iPeriod, int iPosX )
{            
  bool outOfFrame    = false;
  if( iPosX/10==0 ) { outOfFrame = true; }
  int alignY_=3;
  int alignX_=2;
  if( iPosX/10==0 ) alignX_=1;
  if( iPosX==0    ) alignX_=1;
  if( iPosX==0    ) alignY_=1;
  if( iPosX/10==1 ) alignX_=1;
  if( iPosX/10==2 ) alignX_=2;
  if( iPosX/10==3 ) alignX_=3;
  //if( iPosX == 0  ) relPosX = 0.12;
  if( iPosX == 0  ) relPosX = 0.15; // KYO
  int align_ = 10*alignX_ + alignY_;

  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  //  float e = 0.025;

  pad->cd();

  TString lumiText;
  if( iPeriod==0 ) {
    lumiText += lumi_pp502TeV;
    lumiText += " (pp 5.02 TeV)";
  } else if( iPeriod==1 || iPeriod==2 || iPeriod==3){
    lumiText += lumi_pPb502TeV;
    lumiText += " (pPb 5.02 TeV)";
  } else {
    lumiText += "LumiText Not Selected";
  }
   
  cout << lumiText << endl;

  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);    

  //float extraTextSize = extraOverCmsTextSize*cmsTextSize;
  float extraTextSize = extraOverCmsTextSize*cmsTextSize*1.1;

  latex.SetTextFont(42);
  latex.SetTextAlign(31); 
  latex.SetTextSize(lumiTextSize*t);    
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);
  if( outOfFrame ) {
    latex.SetTextFont(cmsTextFont);
    latex.SetTextAlign(11); 
    latex.SetTextSize(cmsTextSize*t);    
    latex.DrawLatex(l,1-t+lumiTextOffset*t,cmsText);
  }
  
  pad->cd();

  float posX_=0;
  if( iPosX%10<=1 ) { posX_ =   l + relPosX*(1-l-r); }
  else if( iPosX%10==2 ) { posX_ =  l + 0.5*(1-l-r); }
  else if( iPosX%10==3 ) { posX_ =  1-r - relPosX*(1-l-r); }
  float posY_ = 1-t - relPosY*(1-t-b);
  if( !outOfFrame ) {
    if( drawLogo ) {
	    posX_ =   l + 0.045*(1-l-r)*W/H;
	    posY_ = 1-t - 0.045*(1-t-b);
	    float xl_0 = posX_;
	    float yl_0 = posY_ - 0.15;
	    float xl_1 = posX_ + 0.15*H/W;
	    float yl_1 = posY_;
	    //TASImage* CMS_logo = new TASImage("CMS-BW-label.png");
	    TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
	    pad_logo->Draw();
	    pad_logo->cd();
	    //CMS_logo->Draw("X");
	    pad_logo->Modified();
	    pad->cd();
	  } else {
	    latex.SetTextFont(cmsTextFont);
	    latex.SetTextSize(cmsTextSize*t);
	    latex.SetTextAlign(align_);
	    latex.DrawLatex(posX_, posY_, cmsText);
	    if( writeExtraText ) {
	      latex.SetTextFont(extraTextFont);
	      latex.SetTextAlign(align_);
	      latex.SetTextSize(extraTextSize*t);
	      latex.DrawLatex(posX_, posY_- relExtraDY*cmsTextSize*t, extraText);
	    }
	  }
  } else if( writeExtraText ) {
    if( iPosX==0) {
  	  posX_ =   l +  relPosX*(1-l-r);
  	  posY_ =   1-t+lumiTextOffset*t;
  	}
    latex.SetTextFont(extraTextFont);
    latex.SetTextSize(extraTextSize*t);
    latex.SetTextAlign(align_);
    latex.DrawLatex(posX_, posY_, extraText);      
  }
  return;
}

