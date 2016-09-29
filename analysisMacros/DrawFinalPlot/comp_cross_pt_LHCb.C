#include "../SONGKYO.h"
#include <CMS_lumi.h>

const int nLHCbBin = 8;

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);
void getMeanPtBins( Double_t * binArr, TF1* func);
void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_cross_pt_LHCb(bool isShift=false, bool isPrompt = true, bool isLog=true)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 1; 
	int iPos=0;
	
  //// BR and lumi info.
	const Double_t br = 0.0593 ;
	const Double_t brErr = 0.0006;
	const Double_t pp_lumi_pb =26.3; // 26.3/pb
	const Double_t pp_lumi_pb_err = 2.63; // tmp
	const Double_t pPb_lumi_nb = 34.622; // 34.6/nb
	const Double_t pPb_lumi_nb_err = 1.2;
	Double_t lumi_mub;
	Double_t lumi_mub_err;
  if (isPA==0) {
    lumi_mub = pp_lumi_pb *1000*1000;
    lumi_mub_err = pp_lumi_pb_err *1000*1000;
  }
  else if (isPA==1) {
    lumi_mub = pPb_lumi_nb * 1000;
    lumi_mub_err = pPb_lumi_nb_err * 1000;
  } else {
    cout << "select among isPA = 0 or 1"<< endl; return ;
  }
	cout << "isPA = " << isPA << ", and lumi_mub = " << lumi_mub <<"+-" <<lumi_mub_err <<  endl;
	
	//// read our result
	TFile *inFile;
  if (isPrompt) inFile= new TFile("plot_cross/pA_cross_pt_middle_isPrompt1_isLog0_isScale0.root");
  else inFile= new TFile("plot_cross/pA_cross_pt_middle_isPrompt0_isLog0_isScale0.root");
	TGraphAsymmErrors* g_cross_sys_0 = (TGraphAsymmErrors*)inFile->Get("g_cross_sys_0"); 	
	TGraphAsymmErrors* g_cross_0 = (TGraphAsymmErrors*)inFile->Get("g_cross_0"); 	
	cout << "g_cross_sys_0 = " << g_cross_sys_0 << endl;
	cout << "g_cross_0 = " << g_cross_0 << endl;

	//// put x error bar
	Double_t ex[]= {0.5, 0.5, 0.5, 0.75, 0.5, 0.5, 0.75, 2., 8.};
	for (int ipt=0; ipt<9; ipt++) {
			g_cross_0->SetPointEXlow(ipt, ex[ipt]);
			g_cross_0->SetPointEXhigh(ipt, ex[ipt]);
			g_cross_sys_0->SetPointEXlow(ipt, 0.4);
			g_cross_sys_0->SetPointEXhigh(ipt, 0.4);
	}

	///////////////////////////////////////////////////
	///////////////////// LHCb ////////////////////////
	///////////////////////////////////////////////////
	Double_t lhcb_px[nLHCbBin] = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 10.5};
	Double_t lhcb_ex[nLHCbBin] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 3.5}; //bin width
	Double_t lhcb_py_pr[nLHCbBin] = {69.1*br, 160.2*br, 138.5*br, 85.6*br, 60.4*br, 34.2*br, 17.8*br, 2.8*br};
	Double_t lhcb_py_np[nLHCbBin] = {12.5*br, 15.6*br, 19.3*br, 14.9*br, 10.1*br, 6.6*br, 3.9*br, 0.9*br};
	Double_t lhcb_ey_pr[nLHCbBin] = {7.7*br, 11.9*br, 9.9*br, 7.0*br, 4.5*br, 3.0*br, 1.9*br, 0.2*br};
	Double_t lhcb_ey_np[nLHCbBin] = {2.7*br, 2.7*br, 2.9*br, 2.4*br, 1.6*br, 1.2*br, 0.8*br, 0.1*br};
	
  Double_t lhcb_exsys[nLHCbBin] = {0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4};
	Double_t lhcb_eysys01_pr[nLHCbBin] = {3.7, 12.4, 10.0, 7.6, 4.0, 0.9, 0.6, 0.1};
	Double_t lhcb_eysys02_pr[nLHCbBin] = {3.1, 7.3, 6.3, 3.9, 2.7, 1.5, 0.8, 0.1};
	Double_t lhcb_eysys01_np[nLHCbBin] = {2.4, 2.7, 1.9, 1.3, 0.8, 0.3, 0.4, 0.0};
	Double_t lhcb_eysys02_np[nLHCbBin] = {0.6, 0.7, 0.9, 0.7, 0.5, 0.3, 0.2, 0.0};
	
//	const Int_t nBin_lhcb = sizeof(lhcb_px)/sizeof(Double_t);
//	cout << "nBin_lhcb : " << nBin_lhcb << endl; 
  
	//// total systematic uncertainties = sqrt( (sys01)^2 + (sys02)^2 )
	Double_t dummy01[nLHCbBin]={0,0,0,0,0,0,0,0};
	Double_t dummy02[nLHCbBin]={0,0,0,0,0,0,0,0};
	Double_t dummy03[nLHCbBin];
	Double_t lhcb_eysys_pr[nLHCbBin];
	Double_t lhcb_eysys_np[nLHCbBin];
	
  for (int ipt =0; ipt< nLHCbBin; ipt ++) {
		AddValue(dummy01[ipt], lhcb_eysys01_pr[ipt], dummy02[ipt], lhcb_eysys02_pr[ipt], &dummy03[ipt], &lhcb_eysys_pr[ipt]);
		lhcb_eysys_pr[ipt]=lhcb_eysys_pr[ipt]*br;
		AddValue(dummy01[ipt], lhcb_eysys01_np[ipt], dummy02[ipt], lhcb_eysys02_np[ipt], &dummy03[ipt], &lhcb_eysys_np[ipt]);
		lhcb_eysys_np[ipt]=lhcb_eysys_np[ipt]*br;
	}
  
  //// select pr or np 
  Double_t lhcb_py[nLHCbBin];  
	Double_t lhcb_ey[nLHCbBin];
	Double_t lhcb_eysys[nLHCbBin];
  for (int ipt =0; ipt< nLHCbBin; ipt ++) {
    if (isPrompt) {
      lhcb_py[ipt] = lhcb_py_pr[ipt];
      lhcb_ey[ipt] = lhcb_ey_pr[ipt];
      lhcb_eysys[ipt] = lhcb_eysys_pr[ipt];
    }else {
      lhcb_py[ipt] = lhcb_py_np[ipt];
      lhcb_ey[ipt] = lhcb_ey_np[ipt];
      lhcb_eysys[ipt] = lhcb_eysys_np[ipt];
    }
  }

	////////////////////////////////////////////////////////////
	//// Draw Plots

	TLegend *legUL = new TLegend(0.175,0.82,0.54,0.89);
	TLegend *legBL = new TLegend(0.175,0.195,0.56,0.32);
	TLegend *legUC01 = new TLegend(0.28,0.70,0.56,0.83);
	TLegend *legUC02 = new TLegend(0.63,0.70,0.89,0.83);
	SetLegendStyle(legUL);
	SetLegendStyle(legBL);
	SetLegendStyle(legUC01);
	SetLegendStyle(legUC02);
	legUL->SetTextSize(0.037);
	legBL->SetTextSize(0.037);
	legUC01->SetTextSize(0.037);
	legUC02->SetTextSize(0.037);
  legUL->SetTextFont(42);
  legBL->SetTextFont(42);
  legUC01->SetTextFont(42);
  legUC02->SetTextFont(42);

	TLatex* latex = new TLatex();
	latex->SetNDC();
	if (isLog) latex->SetTextAlign(12);
	else latex->SetTextAlign(32);
	latex->SetTextSize(0.04);
	
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	//globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);

	////////////////////// LHCb ///////////////////
	TGraphAsymmErrors *g_cross_lhcb_sys = new TGraphAsymmErrors(nLHCbBin, lhcb_px, lhcb_py, lhcb_exsys, lhcb_exsys, lhcb_eysys, lhcb_eysys);	
	g_cross_lhcb_sys->SetFillColor(kGray);
	TGraphAsymmErrors *g_cross_lhcb = new TGraphAsymmErrors(nLHCbBin, lhcb_px, lhcb_py, lhcb_ex, lhcb_ex, lhcb_ey, lhcb_ey);	
	SetGraphStyleFinal(g_cross_lhcb,9,10);
  g_cross_lhcb->SetMarkerSize(1.4);

  /////////////////////////////////
  //// x point shift ( not used now)
	TF1 *expoFunc = new TF1("expoFunc","[0]*exp(-[1]*x)", 4, 14);
	expoFunc->SetParameter(0,10000);
	expoFunc->SetParameter(1,1.5);
	expoFunc->SetLineColor(6);
	//g_cross_lhcb->Fit("expoFunc","R LL M");
	g_cross_lhcb->Fit("expoFunc","R LL M N");
	double lhcb_px_shift[nLHCbBin];
	getMeanPtBins( lhcb_px_shift, expoFunc);
	TGraphAsymmErrors* g_cross_lhcb_shifted = new TGraphAsymmErrors(nLHCbBin, lhcb_px_shift, lhcb_py, lhcb_ex, lhcb_ex, lhcb_ey, lhcb_ey);
	SetGraphStyleFinal(g_cross_lhcb_shifted,9,10);
	TGraphAsymmErrors* g_cross_lhcb_sys_shifted = new TGraphAsymmErrors(nLHCbBin, lhcb_px_shift, lhcb_py, lhcb_exsys, lhcb_exsys, lhcb_eysys, lhcb_eysys);
  g_cross_lhcb_sys_shifted->SetFillColor(kGray);
  /////////////////////////////////

	TCanvas *c1 = new TCanvas("c1","", 200, 10, 600, 600);
	c1->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
  
  //// CMS
	g_cross_sys_0->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	g_cross_sys_0->GetXaxis()->CenterTitle();
  g_cross_sys_0->GetXaxis()->SetLimits(0.0,30.0);
	g_cross_sys_0->GetYaxis()->SetTitle("B x d^{2}#sigma/dp_{T}dy [#mub/(GeV/c)]");
	if (isLog){
		if (isPrompt) {
      g_cross_sys_0->SetMinimum(0.001);
		  g_cross_sys_0->SetMaximum(100.);
    } else {
      g_cross_sys_0->SetMinimum(0.001);
		  g_cross_sys_0->SetMaximum(10.);
    }
	}
	else {
		if (isPrompt) {
      g_cross_sys_0->SetMinimum(-1.0);
  		g_cross_sys_0->SetMaximum(20.);
    } else {
      g_cross_sys_0->SetMinimum(-0.1);
		  g_cross_sys_0->SetMaximum(2.5);
    }
	}
	g_cross_sys_0->SetFillColor(kTeal-9);
	g_cross_sys_0->Draw("A2");
  SetGraphStyleFinal(g_cross_0,0,5);
	g_cross_0->SetMarkerSize(2.1);
	g_cross_0->Draw("P");

	if ( isShift)  {
	  g_cross_lhcb_sys_shifted->Draw("2");
	  g_cross_lhcb_shifted->Draw("P");
	}
	else {  
	  g_cross_lhcb_sys->Draw("2");
	  g_cross_lhcb->Draw("P");
	}
	g_cross_0->Draw("P");
  

	if (!isLog) { solidLine(0.0,0.0,30.0,0.0,1,1); }
	
	if (isLog){	
		legUL -> AddEntry(g_cross_0,"1.5 < y_{CM} < 1.93","lp");
		legUL -> Draw();
		legBL -> SetHeader("LHCb");
		legBL -> AddEntry(g_cross_lhcb,"1.5 < y_{CM} < 2","lp");
		legBL -> Draw();
	}
	else {
		legUC01 -> SetHeader("CMS");
		legUC01 -> AddEntry(g_cross_0,"1.5 < y_{CM} < 1.93","lp");
 		legUC01-> Draw();
		legUC02 -> SetHeader("LHCb");
		legUC02 -> AddEntry(g_cross_lhcb,"1.5 < y_{CM} < 2","lp");
 		legUC02-> Draw();
	}	
	globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.91, 0.86, "Prompt J/#psi");
	else globtex->DrawLatex(0.91, 0.86, "Nonprompt J/#psi");
	
	CMS_lumi( c1, isPA, iPos );
	c1->Update();
	c1->SaveAs(Form("plot_otherExp/comp_cross_pt_LHCb_isPrompt%d_isLog%d_isShift%d.pdf",(int)isPrompt,(int)isLog,(int)isShift));
	c1->SaveAs(Form("plot_otherExp/comp_cross_pt_LHCb_isPrompt%d_isLog%d_isShift%d.png",(int)isPrompt,(int)isLog,(int)isShift));
	legUL->Clear();
	legBL->Clear();
	legUC01->Clear();
	legUC02->Clear();
	//c1->Clear();

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
  Double_t lhcb_Bin[nLHCbBin+1] = {0,1,2,3,4,5,6,7,14};
  
  TH1D* firhist = (TH1D*)func->GetHistogram();
  for ( int i=1 ; i<= nLHCbBin ; i++) {
    float lowPtCut = lhcb_Bin[i-1];
    float highPtCut = lhcb_Bin[i];

    TH1D* tempHist = (TH1D*)firhist->Clone("tempHist");
    for ( int xbin = 1 ;xbin<=tempHist->GetNbinsX() ; xbin++) {
      if ( (tempHist->GetBinCenter(xbin) > highPtCut) || (tempHist->GetBinCenter(xbin) < lowPtCut) )
	tempHist->SetBinContent(xbin,0);
    }
    float meanPt = tempHist->GetMean();
    if ( meanPt <0.1 )   // fit is not applied
      meanPt = (lowPtCut+highPtCut)/2.;

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

