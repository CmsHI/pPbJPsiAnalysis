#include "../SONGKYO.h"
#include "CMS_lumi.h"

const int nAtlasBin = 5;

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);
void getMeanPtBins( Double_t* binArr, TF1* func);
void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_RpPb_pt_ATLAS(bool isPrompt = true)
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
	TFile *inFile_pp;
  if (isPrompt) inFile_pp = new TFile("plot_cross/pp_cross_pt_integ_middle_isPrompt1_isLog0_isScale0_maxrap300.root");
  else inFile_pp= new TFile("plot_cross/pp_cross_pt_integ_middle_isPrompt0_isLog0_isScale0_maxrap300.root");
	TGraphAsymmErrors* g_cross_fwrap_pp = (TGraphAsymmErrors*)inFile_pp->Get("g_cross_fwrap"); 
	TGraphAsymmErrors* g_cross_sys_fwrap_pp = (TGraphAsymmErrors*)inFile_pp->Get("g_cross_sys_fwrap"); 

	//// read our result
	TFile *inFile_pA;
  if (isPrompt) inFile_pA = new TFile("plot_cross/pA_cross_pt_integ_middle_isPrompt1_isLog0_isScale0_maxrap300.root");
  else inFile_pA= new TFile("plot_cross/pA_cross_pt_integ_middle_isPrompt0_isLog0_isScale0_maxrap300.root");
	TGraphAsymmErrors* g_cross_fwrap_pA = (TGraphAsymmErrors*)inFile_pA->Get("g_cross_fwrap"); 
	TGraphAsymmErrors* g_cross_sys_fwrap_pA = (TGraphAsymmErrors*)inFile_pA->Get("g_cross_sys_fwrap"); 

  const int nPt = 5;
  double dummy1;
  double pxtmp[nPt];
  double pytmp_pp[nPt]; double pytmp_pA[nPt]; double rppb[nPt];
  double eytmp_pp[nPt]; double eytmp_pA[nPt]; double ey_rppb[nPt];
  double eysystmp_pp[nPt]; double eysystmp_pA[nPt]; double eysys_rppb[nPt];
  for (int ipt=0; ipt<nPt; ipt++) {
    g_cross_fwrap_pp->GetPoint(ipt+4, pxtmp[ipt], pytmp_pp[ipt]); 
    g_cross_fwrap_pA->GetPoint(ipt+4, pxtmp[ipt], pytmp_pA[ipt]); 
    eytmp_pp[ipt] = g_cross_fwrap_pp->GetErrorY(ipt+4); 
    eytmp_pA[ipt] = g_cross_fwrap_pA->GetErrorY(ipt+4);
    eysystmp_pp[ipt] = g_cross_sys_fwrap_pp->GetErrorY(ipt+4); 
    eysystmp_pA[ipt] = g_cross_sys_fwrap_pA->GetErrorY(ipt+4);
  }
  for (int ipt=0; ipt<nPt; ipt++) {
    DivideValue(pytmp_pA[ipt],eytmp_pA[ipt],pytmp_pp[ipt],eytmp_pp[ipt],&rppb[ipt],&ey_rppb[ipt]);  //actual values
    DivideValue(pytmp_pA[ipt],eysystmp_pA[ipt],pytmp_pp[ipt],eysystmp_pp[ipt],&dummy1,&eysys_rppb[ipt]); //syst. 
    rppb[ipt]/=A_pb; 
    ey_rppb[ipt]/=A_pb; 
    eysys_rppb[ipt]/=A_pb; 
    cout << " "<<endl;
    cout << "rppb["<<ipt<<"] ="  << rppb[ipt] << endl;
    cout << "stat.["<<ipt<<"] ="  << ey_rppb[ipt] << endl;
    cout << "syst.["<<ipt<<"] ="  << eysys_rppb[ipt] << endl;
  }
	Double_t exsys[nPt] = {0.4,0.4,0.4,0.4,0.4};
	Double_t exlow[nPt];
	Double_t exhigh[nPt];
	Double_t ptArrNum[nPt+1] = {6.5, 7.5, 8.5, 10., 14., 30.};
	
  //// ex calculation
  for (Int_t ipt=0; ipt<nPt; ipt++) {
    exlow[ipt] = pxtmp[ipt]-ptArrNum[ipt];
    exhigh[ipt] = ptArrNum[ipt+1]-pxtmp[ipt];
  }
	
  ///////////////////////////////////////////////////
	///////////////////// ATLAS ////////////////////////
	///////////////////////////////////////////////////
	TFile *inFileATLAS;
  if (isPrompt) inFileATLAS = new TFile("ATLAS_Jpsi_RpPb/RpPb_PromptJpsi_dpT.root");
  else inFileATLAS= new TFile("ATLAS_Jpsi_RpPb/RpPb_NonPromptJpsi_dpT.root");
  cout << "inFileATLAS = " << inFileATLAS << endl;
	TGraphAsymmErrors* g_RpPb_ATLAS_sys;
	TGraphAsymmErrors* g_RpPb_ATLAS;
  if (isPrompt) { 
     g_RpPb_ATLAS_sys = (TGraphAsymmErrors*)inFileATLAS->Get("RpPb_p_syst"); 
     g_RpPb_ATLAS = (TGraphAsymmErrors*)inFileATLAS->Get("RpPb_p"); 
  }else {
     g_RpPb_ATLAS_sys = (TGraphAsymmErrors*)inFileATLAS->Get("RpPb_np_syst"); 
     g_RpPb_ATLAS = (TGraphAsymmErrors*)inFileATLAS->Get("RpPb_np"); 
  }
  cout << "g_RpPb_ATLAS_sys = " << g_RpPb_ATLAS_sys << endl;
  cout << "g_RpPb_ATLAS = " << g_RpPb_ATLAS << endl;
  
  
  //////////////////////////////////////////////////////////////
  
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);

  TCanvas *c1 = new TCanvas("c1","c1",600,600);
  
  g_RpPb_ATLAS_sys->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  g_RpPb_ATLAS_sys->GetXaxis()->CenterTitle();
  g_RpPb_ATLAS_sys->GetYaxis()->SetTitle("R_{pPb}");
  g_RpPb_ATLAS_sys->GetYaxis()->CenterTitle();
  g_RpPb_ATLAS_sys->GetXaxis()->SetTitleOffset(1.15);
  g_RpPb_ATLAS_sys->GetXaxis()->SetLimits(0.,32.);
  g_RpPb_ATLAS_sys->SetMinimum(0.0);
  g_RpPb_ATLAS_sys->SetMaximum(2.0);
  g_RpPb_ATLAS_sys->SetLineColor(kGray);
  g_RpPb_ATLAS_sys->SetFillColor(kWhite);
  g_RpPb_ATLAS_sys->SetFillStyle(4000);
  g_RpPb_ATLAS_sys->SetLineWidth(3);
  g_RpPb_ATLAS_sys->SetLineStyle(7);
  
  g_RpPb_ATLAS_sys->SetFillColor(kGray);
  
  SetGraphStyleFinal(g_RpPb_ATLAS, 9, 10);
  g_RpPb_ATLAS->SetMarkerSize(1.7);
  
  //TGraphAsymmErrors* g_RpPb_sys = new TGraphAsymmErrors(nPt, pxtmp, rppb, exsys, exsys, eysys_rppb, eysys_rppb);	
  TGraphAsymmErrors* g_RpPb_sys = new TGraphAsymmErrors(nPt, pxtmp, rppb, exlow, exhigh, eysys_rppb, eysys_rppb);	
  TGraphAsymmErrors* g_RpPb = new TGraphAsymmErrors(nPt, pxtmp, rppb, exsys, exsys, ey_rppb, ey_rppb);	
  g_RpPb_sys->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  g_RpPb_sys->GetXaxis()->CenterTitle();
  g_RpPb_sys->GetYaxis()->SetTitle("R_{pPb}");
  g_RpPb_sys->GetYaxis()->CenterTitle();
  g_RpPb_sys->GetXaxis()->SetLimits(0.,30.0);
  g_RpPb_sys->SetMinimum(0.0);
  g_RpPb_sys->SetMaximum(2.0);
  g_RpPb_sys->SetFillColor(kGreen-10);

  SetGraphStyleFinal(g_RpPb, 0, 5);
  //g_RpPb->SetMarkerSize(2.1);
  g_RpPb->SetMarkerSize(1.8);
  
  ////// actual draw
  g_RpPb_ATLAS_sys->Draw("A5");
  g_RpPb_sys->Draw("5");
  dashedLine(0.,1.,32.,1.,1,1);
  g_RpPb_ATLAS->Draw("p");
  g_RpPb->Draw("p");
  
	
  //TLegend *legBL = new TLegend(0.19,0.18,0.54,0.25);
  TLegend *legBL = new TLegend(0.18,0.18,0.77,0.32);
	SetLegendStyle(legBL);
	legBL->SetTextSize(0.043);
  legBL->SetTextFont(42);
	legBL -> AddEntry(g_RpPb,"-1.5 < y_{CM} < 1.5","lp");
	legBL -> AddEntry(g_RpPb_ATLAS,"ATLAS: -1.5 < y_{CM} < 1.5","lp");
	legBL -> Draw();
  
  globtex->SetTextSize(0.055); 
  globtex->SetTextFont(42);
  if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
  else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");
  
  CMS_lumi( c1, isPA, iPos );
  c1->Update();

	c1->SaveAs(Form("plot_otherExp/comp_RpPb_pt_ATLAS_isPrompt%d.pdf",(int)isPrompt));
	c1->SaveAs(Form("plot_otherExp/comp_RpPb_pt_ATLAS_isPrompt%d.png",(int)isPrompt));
  
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



void getMeanPtBins( Double_t * binArr, TF1* func) {
  Double_t atlas_Bin[nAtlasBin+1] = {8, 9.5, 11.5, 14, 20, 30};
  
  TH1D* firhist = (TH1D*)func->GetHistogram();
  for ( int i=1 ; i<= nAtlasBin ; i++) {
    float lowPtCut = atlas_Bin[i-1];
    float highPtCut = atlas_Bin[i];
    
    TH1D* tempHist = (TH1D*)firhist->Clone("tempHist");
    for ( int xbin = 1 ;xbin<=tempHist->GetNbinsX() ; xbin++) {
      if ( (tempHist->GetBinCenter(xbin) > highPtCut) || (tempHist->GetBinCenter(xbin) < lowPtCut) \
	   )
      tempHist->SetBinContent(xbin,0);
    }
    float meanPt = tempHist->GetMean();
    cout << " meanPt = " << meanPt << endl;
    binArr[i-1] = meanPt;
    delete tempHist;
    
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
        posX_ -= 0.03; posY_-=0.03; 
        latex.SetTextSize(cmsTextSize*t*1.5);
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
