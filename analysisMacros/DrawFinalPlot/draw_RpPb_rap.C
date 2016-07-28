#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void draw_RpPb_rap(bool sysByHand=false, bool noPtWeight=false, bool isPrompt=false)
{
  gROOT->Macro("./tdrstyle_kyo.C");
  //cmsTextFont   = 42;  // for b.hong
  //extraTextFont = 42;  // for b.hong
  int isPA = 10;  // 0:pp, 1:pPb, 10 : pp & pPb together for RpPb plot
	int iPos=33;//right corner
//	int iPos=11;//left corner
//	int iPos=0.;//outOfFrame
  
	double pxshift = 0.15;
  
  //// BR and lumi info.
  const Double_t br = 0.0593 ;
  const Double_t brErr = 0.0006;
	const Double_t pp_lumi_pb =28.538; // 28.5/pb
	const Double_t pp_lumi_pb_err = 1.1; // 4 %
	const Double_t pPb_lumi_nb = 34.622; // 34.6/nb
	const Double_t pPb_lumi_nb_err = 1.2; // 3.5 %
  const Double_t pp_lumi_mub = pp_lumi_pb *1000*1000;
  const Double_t pPb_lumi_mub_err = pp_lumi_pb_err *1000*1000;
 
  double A_pb =208;

  /////////////////////////////////////////////////////////////////////////
  // bin center & systematic uncertainties by hand
  // for 1.5 - 1.93 
  const int nRap = 8; 
  const int nPt = 9;
  const int nRapTmp = nRap + 1;
  const int nPtTmp = nPt + 1;
  const int nRapRpPb = 7;  

  Double_t dummy1, dummy2, dummy3, dummy4;
    
  Double_t pxtmp_lowpt[nRapRpPb]; //x point to fill remporarily
  Double_t pytmp_lowpt[nRapRpPb]; //y point to fill remporarily
  Double_t eytmp_lowpt[nRapRpPb]; //y point error to fill remporarily
  Double_t eysys_lowpt[nRapRpPb]; //absolute y sys error
  Double_t pxtmp_highpt[nRapRpPb]; //x point to fill remporarily
  Double_t pytmp_highpt[nRapRpPb]; //y point to fill remporarily
  Double_t eytmp_highpt[nRapRpPb]; //y point error to fill remporarily
  Double_t eysys_highpt[nRapRpPb]; //absolute y sys error
  
  Double_t pxtmp_lowpt_pA[nRap]; //x point to fill remporarily
  Double_t pytmp_lowpt_pA[nRap]; //y point to fill remporarily
  Double_t eytmp_lowpt_pA[nRap]; //y point error to fill remporarily
  Double_t eysys_lowpt_pA[nRap]; //absolute y sys error
  Double_t pxtmp_highpt_pA[nRap]; //x point to fill remporarily
  Double_t pytmp_highpt_pA[nRap]; //y point to fill remporarily
  Double_t eytmp_highpt_pA[nRap]; //y point error to fill remporarily
  Double_t eysys_highpt_pA[nRap]; //absolute y sys error
  
  Double_t pxtmp_lowpt_pp[nRap]; //x point to fill remporarily
  Double_t pytmp_lowpt_pp[nRap]; //y point to fill remporarily
  Double_t eytmp_lowpt_pp[nRap]; //y point error to fill remporarily
  Double_t eysys_lowpt_pp[nRap]; //absolute y sys error
  Double_t pxtmp_highpt_pp[nRap]; //x point to fill remporarily
  Double_t pytmp_highpt_pp[nRap]; //y point to fill remporarily
  Double_t eytmp_highpt_pp[nRap]; //y point error to fill remporarily
  Double_t eysys_highpt_pp[nRap]; //absolute y sys error
  
	Double_t ex[nRapRpPb] = {0,0,0,0,0,0,0}; // x stat error
  //Double_t exsys[nRapRpPb] = {0.08,0.08,0.08,0.08,0.08,0.08,0.08}; // x sys error
  Double_t exsys[nRapRpPb] = {0.075,0.075,0.075,0.075,0.075,0.075,0.075}; // x sys error
  
  //// 1) y_CM array (from forward to backward)
  //Double_t rapArrNumFB[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
  Double_t rapArrNumBF[nRapTmp] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4};// for rap dist.
  Double_t rapBinW[nRap];
  for (Int_t iy=0; iy<nRap; iy++) {
    rapBinW[iy] = rapArrNumBF[iy+1]-rapArrNumBF[iy];
    //cout << iy <<"th rapBinW = " << rapBinW[iy] <<endl;
  }
  //// 2) pt array
  Double_t ptArrNum[nPtTmp] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
  Double_t ptBinW[nPt];
  for (Int_t ipt=0; ipt<nPt; ipt++) {
    ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
    //cout << ipt <<"th ptBinW = " << ptBinW[ipt] <<endl;
  }
  //// array string
  TString rapArr[nRap];
  for (Int_t iy=0; iy<nRap; iy++) {
    formRapArr(rapArrNumBF[iy], rapArrNumBF[iy+1], &rapArr[iy]);
    cout << iy <<"th rapArr = " << rapArr[iy] << endl;
  }
  TString ptArr[nPt];
  for (Int_t ipt=0; ipt<nPt; ipt++) {
    formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
    cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
  }

  //// read our result
  TFile *inFile_pA, *inFile_pp;
  if (noPtWeight) {
    if (isPrompt) {
      inFile_pA= new TFile("plot_cross/pA_cross_rap_isPrompt1_isScale0_noPtWeight.root");
      inFile_pp= new TFile("plot_cross/pp_cross_rap_isPrompt1_isScale0_noPtWeight.root");
    } else {
      inFile_pA= new TFile("plot_cross/pA_cross_rap_isPrompt0_isScale0_noPtWeight.root");
      inFile_pp= new TFile("plot_cross/pp_cross_rap_isPrompt0_isScale0_noPtWeight.root");
    }
  } else {
    if (isPrompt) {
      inFile_pA= new TFile("plot_cross/pA_cross_rap_isPrompt1_isScale0.root");
      inFile_pp= new TFile("plot_cross/pp_cross_rap_isPrompt1_isScale0.root");
    } else {
      inFile_pA= new TFile("plot_cross/pA_cross_rap_isPrompt0_isScale0.root");
      inFile_pp= new TFile("plot_cross/pp_cross_rap_isPrompt0_isScale0.root");
    }
  }
  TGraphAsymmErrors* g_cross_sys_lowpt_pA;
  TGraphAsymmErrors* g_cross_lowpt_pA;
  TGraphAsymmErrors* g_cross_sys_lowpt_pp;
  TGraphAsymmErrors* g_cross_lowpt_pp;
  TGraphAsymmErrors* g_cross_sys_highpt_pA;
  TGraphAsymmErrors* g_cross_highpt_pA;
  TGraphAsymmErrors* g_cross_sys_highpt_pp;
  TGraphAsymmErrors* g_cross_highpt_pp;
  g_cross_sys_lowpt_pA = (TGraphAsymmErrors*)inFile_pA->Get("g_cross_sys_lowpt");
  g_cross_lowpt_pA = (TGraphAsymmErrors*)inFile_pA->Get("g_cross_lowpt");
  g_cross_sys_highpt_pA = (TGraphAsymmErrors*)inFile_pA->Get("g_cross_sys_highpt");
  g_cross_highpt_pA = (TGraphAsymmErrors*)inFile_pA->Get("g_cross_highpt");
  g_cross_sys_lowpt_pp = (TGraphAsymmErrors*)inFile_pp->Get("g_cross_sys_lowpt");
  g_cross_lowpt_pp = (TGraphAsymmErrors*)inFile_pp->Get("g_cross_lowpt");
  g_cross_sys_highpt_pp = (TGraphAsymmErrors*)inFile_pp->Get("g_cross_sys_highpt");
  g_cross_highpt_pp = (TGraphAsymmErrors*)inFile_pp->Get("g_cross_highpt");
  //cout << "GetN() : " <<g_cross_sys_pA->GetN() << endl;
  
  //////////////////////////////////////////////////////////////////
  //// Calcaulate RpPb

  //// read out py, ey values
  for (Int_t iy = 0; iy < nRap; iy++) { 
    g_cross_sys_lowpt_pA->GetPoint(iy, pxtmp_lowpt_pA[iy], pytmp_lowpt_pA[iy]);
    eytmp_lowpt_pA[iy] = g_cross_lowpt_pA-> GetErrorY(iy);
    eysys_lowpt_pA[iy] = g_cross_sys_lowpt_pA-> GetErrorY(iy);
    g_cross_sys_highpt_pA->GetPoint(iy, pxtmp_highpt_pA[iy], pytmp_highpt_pA[iy]);
    pxtmp_highpt_pA[iy] += pxshift;
    eytmp_highpt_pA[iy] = g_cross_highpt_pA-> GetErrorY(iy);
    eysys_highpt_pA[iy] = g_cross_sys_highpt_pA-> GetErrorY(iy);
    g_cross_sys_lowpt_pp->GetPoint(iy, pxtmp_lowpt_pp[iy], pytmp_lowpt_pp[iy]);
    eytmp_lowpt_pp[iy] = g_cross_lowpt_pp-> GetErrorY(iy);
    eysys_lowpt_pp[iy] = g_cross_sys_lowpt_pp-> GetErrorY(iy);
    g_cross_sys_highpt_pp->GetPoint(iy, pxtmp_highpt_pp[iy], pytmp_highpt_pp[iy]);
    pxtmp_highpt_pp[iy] += pxshift;
    eytmp_highpt_pp[iy] = g_cross_highpt_pp-> GetErrorY(iy);
    eysys_highpt_pp[iy] = g_cross_sys_highpt_pp-> GetErrorY(iy);
  } 
  
  //// NOTE :: Divide different rapidity ranges in pPb and pp!! (due to y shift)  
  for (Int_t iy = 0; iy < nRapRpPb; iy++) { 
    DivideValue(pytmp_lowpt_pA[iy+1], eytmp_lowpt_pA[iy+1], pytmp_lowpt_pp[iy], eytmp_lowpt_pp[iy], &pytmp_lowpt[iy], &eytmp_lowpt[iy]); //actual values
    DivideValue(pytmp_lowpt_pA[iy+1], eysys_lowpt_pA[iy+1], pytmp_lowpt_pp[iy], eysys_lowpt_pp[iy], &dummy1, &eysys_lowpt[iy]); // syst.
    //// nomarlize by A_pb = 208.
    pytmp_lowpt[iy] = pytmp_lowpt[iy]/A_pb;
    eytmp_lowpt[iy] = eytmp_lowpt[iy]/A_pb;
    eysys_lowpt[iy] = eysys_lowpt[iy]/A_pb;
    cout << "" << endl;
    cout << "R_pPb_lowpt["<<iy<<"] = "<< pytmp_lowpt[iy] << endl;; 
    cout << "stat._lowpt["<<iy<<"] = " << eytmp_lowpt[iy]<<endl;
    cout << "sys_lowpt.["<<iy<<"] = " << eysys_lowpt[iy]<<endl;
    DivideValue(pytmp_highpt_pA[iy+1], eytmp_highpt_pA[iy+1], pytmp_highpt_pp[iy], eytmp_highpt_pp[iy], &pytmp_highpt[iy], &eytmp_highpt[iy]); //actual values
    DivideValue(pytmp_highpt_pA[iy+1], eysys_highpt_pA[iy+1], pytmp_highpt_pp[iy], eysys_highpt_pp[iy], &dummy1, &eysys_highpt[iy]); // syst.
    //// nomarlize by A_pb = 208.
    pytmp_highpt[iy] = pytmp_highpt[iy]/A_pb;
    eytmp_highpt[iy] = eytmp_highpt[iy]/A_pb;
    eysys_highpt[iy] = eysys_highpt[iy]/A_pb;
    cout << "" << endl;
    cout << "R_pPb_highpt["<<iy<<"] = "<< pytmp_highpt[iy] << endl;; 
    cout << "stat._highpt["<<iy<<"] = " << eytmp_highpt[iy]<<endl;
    cout << "sys_highpt.["<<iy<<"] = " << eysys_highpt[iy]<<endl;
  }
  TGraphAsymmErrors* g_RpPb_sys_lowpt;
  TGraphAsymmErrors* g_RpPb_lowpt;
  TGraphAsymmErrors* g_RpPb_sys_highpt;
  TGraphAsymmErrors* g_RpPb_highpt;
   
  g_RpPb_sys_lowpt = new TGraphAsymmErrors(nRapRpPb, pxtmp_lowpt_pp, pytmp_lowpt, exsys, exsys, eysys_lowpt, eysys_lowpt);
  g_RpPb_lowpt = new TGraphAsymmErrors(nRapRpPb, pxtmp_lowpt_pp, pytmp_lowpt, ex, ex, eytmp_lowpt, eytmp_lowpt);
  g_RpPb_sys_highpt = new TGraphAsymmErrors(nRapRpPb, pxtmp_highpt_pp, pytmp_highpt, exsys, exsys, eysys_highpt, eysys_highpt);
  g_RpPb_highpt = new TGraphAsymmErrors(nRapRpPb, pxtmp_highpt_pp, pytmp_highpt, ex, ex, eytmp_highpt, eytmp_highpt);
  g_RpPb_sys_lowpt->GetXaxis()->SetTitle("y_{CM}");
  g_RpPb_sys_lowpt->GetXaxis()->CenterTitle();
  g_RpPb_sys_lowpt->GetYaxis()->SetTitle("R_{pPb}");
  g_RpPb_sys_lowpt->GetYaxis()->CenterTitle();
  //g_RpPb_sys_lowpt->GetXaxis()->SetLimits(-2.4,1.93);
  g_RpPb_sys_lowpt->GetXaxis()->SetLimits(-2.4,2.1);
  //g_RpPb_sys_lowpt->GetXaxis()->SetLimits(-2.4,2.4);
  //g_RpPb_sys_lowpt->SetMinimum(0.5);
  //g_RpPb_sys_lowpt->SetMaximum(1.5);
  g_RpPb_sys_lowpt->SetMinimum(0.4);
  g_RpPb_sys_lowpt->SetMaximum(1.6);

  g_RpPb_sys_lowpt->SetFillColor(kRed-9);
  g_RpPb_sys_highpt->SetFillColor(kTeal-9);
  SetGraphStyleFinal(g_RpPb_lowpt,1,3);
  SetGraphStyleFinal(g_RpPb_highpt,0,5);
  g_RpPb_highpt->SetMarkerSize(3.3);

  //////////////////////////////////////////////////////////////////
  //// draw

	TLegend *legBR = new TLegend(0.45, 0.18, 0.70, 0.32);
	SetLegendStyle(legBR);
	legBR->SetTextSize(0.05);
	 	
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);

  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  c1->cd();
  
  g_RpPb_sys_lowpt->Draw("A2");
  g_RpPb_sys_highpt->Draw("2");
  g_RpPb_lowpt->Draw("P");
  g_RpPb_highpt->Draw("P");
  dashedLine(-2.4,1.,1.93,1.,1,1);

	TLegendEntry *le1=legBR->AddEntry("le1","6.5 < p_{T} < 10 GeV/c","lpf");
  le1->SetFillColor(kRed-10);
  le1->SetFillStyle(1001);
  le1->SetLineColor(kPink-6);
  le1->SetMarkerStyle(kFullSquare);
  le1->SetMarkerColor(kPink-6);
  le1->SetMarkerSize(2.1);
	//legBR->Draw();
	TLegendEntry *le2=legBR->AddEntry("le2","10 < p_{T} < 30 GeV/c","lpf");
  le2->SetFillColor(kTeal-9);
  le2->SetFillStyle(1001);
  le2->SetLineColor(kGreen+3);
  le2->SetMarkerStyle(kFullDiamond);
  le2->SetMarkerColor(kGreen+3);
  le2->SetMarkerSize(3.3);
	legBR->Draw();
	
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");
	globtex->SetTextSize(0.035);
	globtex->SetTextFont(42);
  globtex->DrawLatex(0.21, 0.78, "Global uncertainty : 5.3 \%");
	
  CMS_lumi( c1, isPA, iPos );

  if (noPtWeight) { 
    c1->SaveAs(Form("plot_RpPb/RpPb_rap_isPrompt%d_noPtWeight.pdf",(int)isPrompt));
    c1->SaveAs(Form("plot_RpPb/RpPb_rap_isPrompt%d_noPtWeight.png",(int)isPrompt));
  } else {
    c1->SaveAs(Form("plot_RpPb/RpPb_rap_isPrompt%d.pdf",(int)isPrompt));
    c1->SaveAs(Form("plot_RpPb/RpPb_rap_isPrompt%d.png",(int)isPrompt));
  }
  
  ///////////////////////////////////////////////////////////////////
  //// save as a root file
  TFile *outFile;
  if (noPtWeight) outFile = new TFile(Form("plot_RpPb/RpPb_rap_isPrompt%d_noPtWeight.root",(int)isPrompt),"RECREATE");
  else outFile = new TFile(Form("plot_RpPb/RpPb_rap_isPrompt%d.root",(int)isPrompt),"RECREATE");
  outFile->cd();
  g_RpPb_sys_lowpt->Write();  
  g_RpPb_lowpt->Write();  
  g_RpPb_sys_highpt->Write();  
  g_RpPb_highpt->Write();  
  outFile->Close();

  return;
} // end of main func.

void formRapArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( binmin == 1.93 || binmin == -1.93 || binmin == -2.87) {
		*arr = Form("%.2f < y_{CM} < %.1f", binmin, binmax);
  }
  else if ( binmax == 1.93 || binmax ==  -1.93 || binmax == -2.87 ) {
		*arr = Form("%.1f < y_{CM} < %.2f", binmin, binmax);
  }
  else if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < y_{CM} < %.0f", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.1f < y_{CM} < %.0f", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < y_{CM} < %.1f", binmin, binmax);
  } else {
    *arr = Form("%.1f < y_{CM} < %.1f", binmin, binmax);
  }
}

void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( binmin == 1.93 || binmin == -1.93 || binmin == -2.87) {
    *arr = Form("%.2f < |y_{CM}| < %.1f", binmin, binmax);
  }
  else if ( binmax == 1.93 || binmax ==  -1.93 || binmax == -2.87 ) {
    *arr = Form("%.1f < |y_{CM}| < %.2f", binmin, binmax);
  }
  else if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.1f < |y_{CM}| < %.0f", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < |y_{CM}| < %.1f", binmin, binmax);
  } else {
    *arr = Form("%.1f < |y_{CM}| < %.1f", binmin, binmax);
  }
}

void formPtArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < p_{T} < %.0f [GeV/c]", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.1f < p_{T} < %.0f [GeV/c]", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < p_{T} < %.1f [GeV/c]", binmin, binmax);
  } else {
    *arr = Form("%.1f < p_{T} < %.1f [GeV/c]", binmin, binmax);
  }
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
  } else if( iPeriod==10){
    lumiText += "pPb ";
    lumiText += lumi_pPb502TeV;
    lumiText += ", pp ";
    lumiText += lumi_pp502TeV;
    lumiText += " (5.02 TeV)";
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
      //cout << "posX_ = " << posX_ << ", posY_ = " << posY_ << endl;
      if (iPosX==33) {posX_ -= 0.03; posY_-=0.03; } // KYO
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
