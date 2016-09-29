#include "../SONGKYO.h"
#include "CMS_lumi.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);
void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_RpPb_pt_ALICE(bool isPrompt = true)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 10;
	int iPos=33;
  
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
  
  //// read our result
	TFile *inFile;
  if (isPrompt) inFile = new TFile("plot_RpPb/RpPb_pt_isPrompt1.root");
  else inFile= new TFile("plot_RpPb/RpPb_pt_isPrompt0.root");
	TGraphAsymmErrors* g_RpPb_sys_fw = (TGraphAsymmErrors*)inFile->Get("g_RpPb_sys_0"); 
	TGraphAsymmErrors* g_RpPb_fw = (TGraphAsymmErrors*)inFile->Get("g_RpPb_0"); 
	TGraphAsymmErrors* g_RpPb_sys_bw = (TGraphAsymmErrors*)inFile->Get("g_RpPb_sys_6"); 
	TGraphAsymmErrors* g_RpPb_bw = (TGraphAsymmErrors*)inFile->Get("g_RpPb_6"); 
  g_RpPb_sys_fw->SetFillColorAlpha(kRed-10,0.5);
  g_RpPb_sys_bw->SetFillColorAlpha(kRed-10,0.5);
  g_RpPb_fw->SetMarkerSize(1.4);
  g_RpPb_bw->SetMarkerSize(1.4);
	
	///////////////////////////////////////////////////
	///////////////////// ALICE ////////////////////////
	///////////////////////////////////////////////////
	
  const int nPtALICE = 7;
  double px_ALICE[nPtALICE] = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 7.0};

  double rppb_ALICE_fw[nPtALICE] = {0.61, 0.64, 0.68, 0.76, 0.87, 0.91, 0.98};
  double rppb_ALICE_bw[nPtALICE] = {0.96, 1.06, 1.11, 1.16, 1.17, 1.13, 1.27};
  double ex_ALICE[nPtALICE] = {0,0,0,0,0};
  double exsys_ALICE[nPtALICE] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0};
  double ey_ALICE_fw[nPtALICE] = {0.01, 0.01, 0.01, 0.01, 0.02, 0.02, 0.02};
  double ey_ALICE_bw[nPtALICE] = {0.02, 0.01, 0.01, 0.02, 0.02, 0.03, 0.03};
  double eysys01_ALICE_fw[nPtALICE] = {0.05, 0.05, 0.05, 0.06, 0.07, 0.08, 0.09}; //uncorrel.
  double eysys01_ALICE_bw[nPtALICE] = {0.09, 0.10, 0.10, 0.10, 0.11, 0.12, 0.14}; //uncorrel.
  double eysys02_ALICE_fw[nPtALICE] = {0.02, 0.02, 0.03, 0.03, 0.02, 0.03, 0.05}; //partially correl.
  double eysys02_ALICE_bw[nPtALICE] = {0.03, 0.04, 0.04, 0.04, 0.03, 0.02, 0.08}; //partially correl.
  TGraphAsymmErrors* g_RpPb_ALICE_fw_sys01 = new TGraphAsymmErrors(nPtALICE, px_ALICE, rppb_ALICE_fw, exsys_ALICE, exsys_ALICE, eysys01_ALICE_fw, eysys01_ALICE_fw);	
  TGraphAsymmErrors* g_RpPb_ALICE_bw_sys01 = new TGraphAsymmErrors(nPtALICE, px_ALICE, rppb_ALICE_bw, exsys_ALICE, exsys_ALICE, eysys01_ALICE_bw, eysys01_ALICE_bw);	
  TGraphAsymmErrors* g_RpPb_ALICE_fw_sys02 = new TGraphAsymmErrors(nPtALICE, px_ALICE, rppb_ALICE_fw, exsys_ALICE, exsys_ALICE, eysys02_ALICE_fw, eysys02_ALICE_fw);	
  TGraphAsymmErrors* g_RpPb_ALICE_bw_sys02 = new TGraphAsymmErrors(nPtALICE, px_ALICE, rppb_ALICE_bw, exsys_ALICE, exsys_ALICE, eysys02_ALICE_bw, eysys02_ALICE_bw);	
  TGraphAsymmErrors* g_RpPb_ALICE_fw = new TGraphAsymmErrors(nPtALICE, px_ALICE, rppb_ALICE_fw, ex_ALICE, ex_ALICE, ey_ALICE_fw, ey_ALICE_fw);	
  TGraphAsymmErrors* g_RpPb_ALICE_bw = new TGraphAsymmErrors(nPtALICE, px_ALICE, rppb_ALICE_bw, ex_ALICE, ex_ALICE, ey_ALICE_bw, ey_ALICE_bw);	
  
  g_RpPb_ALICE_fw_sys01->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  g_RpPb_ALICE_fw_sys01->GetXaxis()->CenterTitle();
  g_RpPb_ALICE_fw_sys01->GetYaxis()->SetTitle("R_{pPb}");
  g_RpPb_ALICE_fw_sys01->GetYaxis()->CenterTitle();
  g_RpPb_ALICE_fw_sys01->GetXaxis()->SetLimits(0.,32.0);
  //g_RpPb_ALICE_fw_sys01->SetMinimum(0.5);
  g_RpPb_ALICE_fw_sys01->SetMinimum(0.0);
  g_RpPb_ALICE_fw_sys01->SetMaximum(2.0);
  g_RpPb_ALICE_fw_sys01->SetLineColor(kGray);
  g_RpPb_ALICE_fw_sys01->SetFillColor(kWhite);
  g_RpPb_ALICE_fw_sys01->SetFillStyle(4000);
  g_RpPb_ALICE_fw_sys01->SetLineWidth(3);
  g_RpPb_ALICE_fw_sys01->SetLineStyle(7);
  
  g_RpPb_ALICE_fw_sys02->SetFillColor(kGray);
  SetGraphStyleFinal(g_RpPb_ALICE_fw, 9, 10);
  g_RpPb_ALICE_fw->SetMarkerSize(1.4);
 
  g_RpPb_ALICE_bw_sys01->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  g_RpPb_ALICE_bw_sys01->GetXaxis()->CenterTitle();
  g_RpPb_ALICE_bw_sys01->GetYaxis()->SetTitle("R_{pPb}");
  g_RpPb_ALICE_bw_sys01->GetYaxis()->CenterTitle();
  g_RpPb_ALICE_bw_sys01->GetXaxis()->SetLimits(0.,32.0);
  g_RpPb_ALICE_bw_sys01->SetMinimum(0.0);
  g_RpPb_ALICE_bw_sys01->SetMaximum(2.0);
  g_RpPb_ALICE_bw_sys01->SetLineColor(kGray);
  g_RpPb_ALICE_bw_sys01->SetFillColor(kWhite);
  g_RpPb_ALICE_bw_sys01->SetFillStyle(4000);
  g_RpPb_ALICE_bw_sys01->SetLineWidth(3);
  g_RpPb_ALICE_bw_sys01->SetLineStyle(7);
  
  g_RpPb_ALICE_bw_sys02->SetFillColor(kGray);
  SetGraphStyleFinal(g_RpPb_ALICE_bw, 9, 10);
  g_RpPb_ALICE_bw->SetMarkerSize(1.4);
 
  //////////////////////////////////////////////////////////////
  
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);

  ////// actual draw
  TCanvas *c_fw = new TCanvas("c_fw","c_fw",600,600);
  
  g_RpPb_ALICE_fw_sys01->Draw("A5");
  g_RpPb_ALICE_fw_sys02->Draw("2");
  g_RpPb_sys_fw->Draw("5");
  solidLine(0.,1.,32.,1.,1,1);
  g_RpPb_ALICE_fw->Draw("p");
  g_RpPb_fw->Draw("p");
  
	
  TLegend *legBL_fw = new TLegend(0.47,0.21,0.8,0.32);
	SetLegendStyle(legBL_fw);
	legBL_fw->SetTextSize(0.037);
  legBL_fw->SetTextFont(42);
	legBL_fw -> AddEntry(g_RpPb_fw, "1.5 < y_{CM} < 1.93","lp");
	legBL_fw -> AddEntry(g_RpPb_ALICE_fw,"ALICE: 2.03 < y_{CM} < 3.53","lp");
	legBL_fw -> Draw();
	globtex->SetTextSize(0.037);
	globtex->SetTextFont(42);
	globtex->DrawLatex(0.65, 0.177, "inclusive J/#psi");

  globtex->SetTextSize(0.055); 
  globtex->SetTextFont(42);
  if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
  else globtex->DrawLatex(0.21, 0.84, "Nonprompt J/#psi");
  
  CMS_lumi( c_fw, isPA, iPos );
  c_fw->Update();

	c_fw->SaveAs(Form("plot_otherExp/comp_RpPb_pt_ALICE_isPrompt%d_fw.pdf",(int)isPrompt));
	c_fw->SaveAs(Form("plot_otherExp/comp_RpPb_pt_ALICE_isPrompt%d_fw.png",(int)isPrompt));
 
  /////////////////////////////////////////////////////////////////////////////////
 
  ////// actual draw
  TCanvas *c_bw = new TCanvas("c_bw","c_bw",600,600);
  
  g_RpPb_ALICE_bw_sys01->Draw("A5");
  g_RpPb_ALICE_bw_sys02->Draw("2");
  g_RpPb_sys_bw->Draw("5");
  solidLine(0.,1.,32.,1.,1,1);
  g_RpPb_ALICE_bw->Draw("p");
  g_RpPb_bw->Draw("p");
  
	
  TLegend *legBL_bw = new TLegend(0.47,0.21,0.8,0.32);
	SetLegendStyle(legBL_bw);
	legBL_bw->SetTextSize(0.037);
  legBL_bw->SetTextFont(42);
	legBL_bw -> AddEntry(g_RpPb_bw, "-2.4 < y_{CM} < -1.87","lp");
	legBL_bw -> AddEntry(g_RpPb_ALICE_bw,"ALICE: -4.46 < y_{CM} < -2.96","lp");
	legBL_bw -> Draw();
	globtex->SetTextSize(0.037);
	globtex->SetTextFont(42);
	globtex->DrawLatex(0.65, 0.177, "inclusive J/#psi");

  globtex->SetTextSize(0.055); 
  globtex->SetTextFont(42);
  if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
  else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");
  
  CMS_lumi( c_bw, isPA, iPos );
  c_bw->Update();

	c_bw->SaveAs(Form("plot_otherExp/comp_RpPb_pt_ALICE_isPrompt%d_bw.pdf",(int)isPrompt));
	c_bw->SaveAs(Form("plot_otherExp/comp_RpPb_pt_ALICE_isPrompt%d_bw.png",(int)isPrompt));
  
  return;

}

void formRapArr(Double_t binmin, Double_t binmax, TString* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < y_{CM} < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < y_{CM} < %.2f", binmin, binmax);
	}
}

void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < |y_{CM}| < %.2f", binmin, binmax);
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
  //latex.DrawLatex(1-r+0.01,1-t+lumiTextOffset*t+0.01,lumiText);//KYO
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
      //if (iPosX==33) {posX_ -= 0.03; posY_-=0.03; } // KYO
      //if (iPosX==33) {posX_ += 0.03; posY_-=0.01; } // KYO RpPb_pt
      if (iPosX==33) {
        posX_ -= 0.01; posY_-=0.02; 
        latex.SetTextSize(cmsTextSize*t*1.3);
      } // KYO
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
