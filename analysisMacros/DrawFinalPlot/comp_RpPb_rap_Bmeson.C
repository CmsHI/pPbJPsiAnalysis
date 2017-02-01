//// reference : http://hepdata.cedar.ac.uk/view/ins1390110
//// https://arxiv.org/abs/1508.06678 

#include "../SONGKYO.h"
#include "CMS_lumi.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);
void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_RpPb_rap_Bmeson(bool isPrompt = false)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 10;
	int iPos=33;
  
  //// BR and lumi info.
  const Double_t br = 0.0593 ;
  const Double_t brErr = 0.0006;
	const Double_t pp_lumi_pb =27.972; // 28.0/pb
	const Double_t pp_lumi_pb_err = 0.643; // 2.3 %
	const Double_t pPb_lumi_nb = 34.622; // 34.6/nb
	const Double_t pPb_lumi_nb_err = 1.211; // 3.5 %
  const Double_t pp_lumi_mub = pp_lumi_pb *1000*1000;
  const Double_t pPb_lumi_mub_err = pp_lumi_pb_err *1000*1000;
 
  double A_pb =208;
  
	//// read our result
	TFile *inFile;
  if (isPrompt) inFile = new TFile("plot_RpPb/RpPb_rap_isPrompt1.root");
  else inFile= new TFile("plot_RpPb/RpPb_rap_isPrompt0.root");
	TGraphAsymmErrors* g_RpPb_sys_highpt = (TGraphAsymmErrors*)inFile->Get("g_RpPb_sys_highpt"); 
	TGraphAsymmErrors* g_RpPb_highpt = (TGraphAsymmErrors*)inFile->Get("g_RpPb_highpt"); 
  g_RpPb_sys_highpt->SetFillColorAlpha(kRed-10,0.5);
  g_RpPb_sys_highpt->SetLineColor(kPink-6);
  g_RpPb_highpt->SetMarkerSize(1.7);
  
	//double pxshift = 0.15;
	double pxshift = 0.0;
  const int nRap = 8; 
  const int nRapTmp = nRap + 1;
  const int nRapRpPb = 7;
  
  //// ex calculation
  Double_t exlow[nRapRpPb]; //x binWidth
  Double_t exhigh[nRapRpPb]; //x binWidth
  Double_t rapArrNumBF[nRapTmp] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4};// for rap dist.
  for (Int_t iy=0; iy<nRapRpPb; iy++) {
    exlow[iy] = (rapArrNumBF[iy]+rapArrNumBF[iy+1])/2.-rapArrNumBF[iy]; 
    exhigh[iy] = rapArrNumBF[iy+1]-(rapArrNumBF[iy]+rapArrNumBF[iy+1])/2.; 
  }
  
  //// remove pxshift
  double dummyX, dummyY;
  for (int iy=0; iy<nRapRpPb; iy++) { 
    g_RpPb_sys_highpt->GetPoint(iy, dummyX, dummyY);
    g_RpPb_sys_highpt->SetPoint(iy, dummyX-pxshift, dummyY);
    g_RpPb_sys_highpt->SetPointEXlow(iy, exlow[iy]);
    g_RpPb_sys_highpt->SetPointEXhigh(iy, exhigh[iy]);
    g_RpPb_highpt->GetPoint(iy, dummyX, dummyY);
    g_RpPb_highpt->SetPoint(iy, dummyX-pxshift, dummyY);
  }
	
  ///////////////////////////////////////////////////
	///////////////////// Bmeson ////////////////////////
	///////////////////////////////////////////////////
  TCanvas *c1 = new TCanvas("c1","c1",600,600);
	
  const int nRapBmeson = 5;
  double px_Bmeson[nRapBmeson] = {-2.395, -1.465, -0.5, 0.5, 1.465};
  double rppb_Bmeson[nRapBmeson] = {1.47, 1.37, 1.04, 1.10, 1.21};
  double ex_Bmeson[nRapBmeson] = {0,0,0,0,0};
  double exsys_Bmeson[nRapBmeson] = {0.47, 0.465, 0.5, 0.5, 0.465};
  double ey_Bmeson[nRapBmeson] = {0.18, 0.13, 0.11, 0.10, 0.14};
  double eysys_Bmeson[nRapBmeson] = {0.22, 0.20, 0.15, 0.17, 0.18};
  double eysysFONLLlow_Bmeson[nRapBmeson] = {0.37,0.33,0.26,0.27,0.30};
  double eysysFONLLhigh_Bmeson[nRapBmeson] = {0.42,0.37,0.29,0.31,0.34};
  TGraphAsymmErrors* g_RpPb_Bmeson_sys = new TGraphAsymmErrors(nRapBmeson, px_Bmeson, rppb_Bmeson, exsys_Bmeson, exsys_Bmeson, eysys_Bmeson, eysys_Bmeson);	
  TGraphAsymmErrors* g_RpPb_Bmeson_sysFONLL = new TGraphAsymmErrors(nRapBmeson, px_Bmeson, rppb_Bmeson, exsys_Bmeson, exsys_Bmeson, eysysFONLLlow_Bmeson, eysysFONLLhigh_Bmeson);	
  TGraphAsymmErrors* g_RpPb_Bmeson = new TGraphAsymmErrors(nRapBmeson, px_Bmeson, rppb_Bmeson, ex_Bmeson, ex_Bmeson, ey_Bmeson, ey_Bmeson);	
  
  g_RpPb_Bmeson_sysFONLL->GetXaxis()->SetTitle("y_{CM}");
  g_RpPb_Bmeson_sysFONLL->GetXaxis()->CenterTitle();
  g_RpPb_Bmeson_sysFONLL->GetYaxis()->SetTitle("R_{pPb}");
  g_RpPb_Bmeson_sysFONLL->GetYaxis()->CenterTitle();
  g_RpPb_Bmeson_sysFONLL->GetXaxis()->SetLimits(-3.0,2.1);
  g_RpPb_Bmeson_sysFONLL->SetMinimum(0.0);
  g_RpPb_Bmeson_sysFONLL->SetMaximum(2.0);
  g_RpPb_Bmeson_sysFONLL->SetLineColor(kGray);
  g_RpPb_Bmeson_sysFONLL->SetFillColor(kWhite);
  g_RpPb_Bmeson_sysFONLL->SetFillStyle(4000);
  g_RpPb_Bmeson_sysFONLL->SetLineWidth(3);
  g_RpPb_Bmeson_sysFONLL->SetLineStyle(7);
  
  g_RpPb_Bmeson_sys->SetFillColor(kGray);
  
  SetGraphStyleFinal(g_RpPb_Bmeson, 9, 13);
  g_RpPb_Bmeson->SetMarkerSize(1.7);
 
  //////////////////////////////////////////////////////////////
  
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);

  TBox * globbox_br = new TBox(-3.2, 0.969, -3.0, 1.031); //for B+ branching ratio
  globbox_br->SetFillColorAlpha(kWhite,0.5);
  globbox_br->SetLineColor(kBlack);

  ////// actual draw
  g_RpPb_Bmeson_sysFONLL->Draw("A5");
  g_RpPb_Bmeson_sys->Draw("2");
  g_RpPb_sys_highpt->Draw("5");
  solidLine(-3.0,1.,2.1,1.,1,1);
  g_RpPb_highpt->Draw("p");
  g_RpPb_Bmeson->Draw("p");
  
	
  //TLegend *legBL = new TLegend(0.50,0.18,0.90,0.25);
  TLegend *legBL = new TLegend(0.18,0.18,0.77,0.32);
	SetLegendStyle(legBL);
	//legBL->SetTextSize(0.037);
	legBL->SetTextSize(0.043);
  legBL->SetTextFont(42);
	legBL -> AddEntry(g_RpPb_highpt,"Nonprompt J/#psi: 10 < p_{T} < 30 GeV/c","lp");
	legBL -> AddEntry(g_RpPb_Bmeson,"B^{+}: 10 < p_{T} < 60 GeV/c","lp");
	legBL -> Draw();
  
  //globtex->SetTextSize(0.055); 
  //globtex->SetTextFont(42);
  //globtex->DrawLatex(0.212, 0.372, "Open beauty");
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextSize(0.048);
	globtex->SetTextFont(42);
  globtex->DrawLatex(0.92, 0.79, "Open beauty");
  
  CMS_lumi( c1, isPA, iPos );
  c1->Update();

	c1->SaveAs(Form("plot_otherExp/comp_RpPb_rap_Bmeson_isPrompt%d.pdf",(int)isPrompt));
	c1->SaveAs(Form("plot_otherExp/comp_RpPb_rap_Bmeson_isPrompt%d.png",(int)isPrompt));
  
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
		*arr = Form("%.0f < p_{T} < %.0f (GeV/c)", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < p_{T} < %.0f (GeV/c)", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < p_{T} < %.1f (GeV/c)", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f (GeV/c)", binmin, binmax);
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
