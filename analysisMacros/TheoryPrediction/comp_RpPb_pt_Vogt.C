#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_RpPb_pt_Vogt(double ptmax=32, bool isLine=true, bool isSmoothened=false)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 10;  // 0:pp, 1:pPb, 10 : pp & pPb together for RpPb plot
	int iPos=33;

  bool isPrompt=true;
	
  ///////////////////////////////////////////////////
	///////// from Ramona
  const int nRap = 8; 
  const int nRapTmp = nRap + 1;
	const int nRapRpPb = 7;
	const int nPtRpPb = 49;
  Double_t theory_px[nRapRpPb][nPtRpPb]; 
	Double_t theory_py[nRapRpPb][nPtRpPb];
	//Double_t theory_exlow_tmp[nRapRpPb][nPtRpPb];
	//Double_t theory_exhigh_tmp[nRapRpPb][nPtRpPb];
  Double_t theory_exlow[nRapRpPb][nPtRpPb];
  Double_t theory_exhigh[nRapRpPb][nPtRpPb];
	Double_t theory_eylow_tmp[nRapRpPb][nPtRpPb];
	Double_t theory_eyhigh_tmp[nRapRpPb][nPtRpPb];
  Double_t theory_eylow[nRapRpPb][nPtRpPb];
	Double_t theory_eyhigh[nRapRpPb][nPtRpPb];
  Double_t ptlimit[nRapRpPb] = {4.0, 6.5, 6.5, 6.5, 6.5, 5.0, 4.0};
  
  //// 1) y_CM array (from forward to backward)
  Double_t rapArrNumFB[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
  //Double_t rapArrNumBF[nRapTmp] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
  //// array string
  TString rapArr[nRap];
  for (Int_t iy=0; iy<nRap; iy++) {
    formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
    cout << iy <<"th rapArr = " << rapArr[iy] << endl;
  }
  
  ///////////////////////////////////////////////////////////////////
  //// read-in txt
  ///////////////////////////////////////////////////////////////////
  
  /// iy=0 (1.5 < y < 1.93) 
  TString inText[nRapRpPb];
  for (int iy=0; iy<nRapRpPb; iy++) {
    inText[iy] = Form("./fromRamona/kyo_rppb_pt_%1d.dat",iy);
    cout << "inText["<<iy<<"] = " << inText[iy] << endl;
  }

  string headers;
  TString pxdum, pydum, eylow_tmpdum, eyhigh_tmpdum;
  int counts=0;
  
  for (int iy=0; iy<nRapRpPb; iy++) {
    cout << endl << " ************* iy = " << iy << endl;
    std::ifstream f0(inText[iy].Data(),std::ios::in);
    getline(f0, headers); // remove prefix
    counts=0;
    while(!f0.eof()) {
      f0 >> pxdum >> pydum >> eyhigh_tmpdum >> eylow_tmpdum;
      cout << pxdum <<"\t"<< pydum <<"\t"<< eyhigh_tmpdum <<"\t"<< eylow_tmpdum << endl;
      theory_px[iy][counts] =atof(pxdum.Data());
      theory_py[iy][counts] =atof(pydum.Data());
      theory_eylow_tmp[iy][counts] =atof(eylow_tmpdum.Data());
      theory_eyhigh_tmp[iy][counts] =atof(eyhigh_tmpdum.Data());
      counts++;
    } //end of while file open
  } 
 
  /////////////////////////////////////////////////////////////////////////////////
  //// set proper ex and ey
  for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
    //cout << endl << " ************* iy = " << iy << endl;
    for (Int_t ipt=0; ipt<nPtRpPb; ipt++) {
		  //theory_exlow[iy][ipt] = fabs(theory_px[iy][ipt] - theory_exlow_tmp[iy][ipt]);
		  //theory_exhigh[iy][ipt] = fabs(theory_px[iy][ipt] - theory_exhigh_tmp[iy][ipt]);
      theory_exlow[iy][ipt] = 0.5;
      theory_exhigh[iy][ipt] = 0.5;
		  theory_eylow[iy][ipt] = fabs(theory_py[iy][ipt] - theory_eylow_tmp[iy][ipt]);
		  theory_eyhigh[iy][ipt] = fabs(theory_py[iy][ipt] - theory_eyhigh_tmp[iy][ipt]);
      //cout << "theory_px = " << theory_px[iy][ipt] << ", theory_py = " << theory_py[iy][ipt] << endl; 	
      //cout << "theory_eylow_tmp = " << theory_eylow_tmp[iy][ipt] <<", theory_eyhigh_tmp = " << theory_eyhigh_tmp[iy][ipt] << endl; 	
    }
  }
 
 //// set unused values as zero 
 for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
    for (Int_t ipt=nPtRpPb-1; ipt>=0; ipt--) {
      if (theory_px[iy][ipt] <ptlimit[iy]-1.) {
        theory_px[iy][ipt] = theory_px[iy][ipt+1];
        theory_py[iy][ipt] = theory_py[iy][ipt+1];
        //theory_exlow[iy][ipt] = 0;
        //theory_exhigh[iy][ipt] = 0;
        theory_eylow[iy][ipt] = theory_eylow[iy][ipt+1];
        theory_eyhigh[iy][ipt] = theory_eyhigh[iy][ipt+1];
      }
    }
  }
  
 
  
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  
  TGraphAsymmErrors* g_RpPb_theory[nRapRpPb];
  for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
    g_RpPb_theory[iy]= new TGraphAsymmErrors(nPtRpPb, theory_px[iy], theory_py[iy], theory_exlow[iy], theory_exhigh[iy], theory_eylow[iy], theory_eyhigh[iy]);	
    g_RpPb_theory[iy]->SetName(Form("g_RpPb_theory_%d",iy));
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
  TFile *inFile = new TFile("../DrawFinalPlot/plot_RpPb/RpPb_pt_isPrompt1.root","READ");
  
  TGraphAsymmErrors* g_RpPb_sys[nRapRpPb];
  TGraphAsymmErrors* g_RpPb[nRapRpPb];
  for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
    g_RpPb_sys[iy] = (TGraphAsymmErrors*)inFile->Get(Form("g_RpPb_sys_%d",iy));
    g_RpPb[iy] = (TGraphAsymmErrors*)inFile->Get(Form("g_RpPb_%d",iy));
  } 
  
	///////////////////////////////////////////////////
	//// Draw
  ///////////////////////////////////////////////////
	
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.075);

  //// Draw 
	//TBox* emptybox = new TBox(0.0, 0.0, 0.97, 0.86);
	TBox* emptybox = new TBox(0.0, 0.0, 1.0, 0.86);
	emptybox->SetFillColor(kWhite);
	emptybox->SetFillStyle(4000);
	emptybox->SetLineColor(kBlack);
	emptybox->SetLineWidth(1);
  
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
    pad_all[iy]->SetLeftMargin(0.0);
    pad_all[iy]->SetRightMargin(0.0);
  }
  
  for (int iy=0; iy<nRapRpPb; iy++) {
    //// reverse the order for forward
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
    if (isSmoothened) g_RpPb_theory[iy]->Draw("3");
    else g_RpPb_theory[iy]->Draw("5");
    dashedLine(0.,1.,32.,1.,1,1);
    globtex->SetTextAlign(32); //3:right 2:vertical center
	  globtex->SetTextFont(42);
	  //if (iy==2 || iy==3) globtex->SetTextSize(0.063);
	  //else globtex->SetTextSize(0.078);
	  globtex->SetTextSize(0.078);
    if (iy<3) globtex->DrawLatex(0.9, 0.09, rapArr[iy].Data());
    else globtex->DrawLatex(0.9, 0.25, rapArr[iy].Data());
  }
  pad_all[4]->cd();
  emptybox->Draw("l");
	
  globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextSize(0.09);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.08, 0.28, "Prompt J/#psi");
	else globtex->DrawLatex(0.08, 0.28, "Non-prompt J/#psi");
  globtex->SetTextAlign(12); //1:left 2:vertical center
	globtex->SetTextSize(0.07);
	globtex->SetTextFont(42);
  globtex->DrawLatex(0.08, 0.18, "Global uncertainty : 5.3 \%");


  //////////////////// y axis
      //g_RpPb_sys[iy]->GetYaxis()->SetTitle("R_{pPb}");
      //g_RpPb_sys[iy]->GetYaxis()->SetTitleOffset(1.1);
      //g_RpPb_sys[iy]->GetYaxis()->SetTitleSize(0.075);
      //g_RpPb_sys[iy]->GetYaxis()->SetLabelSize(0.055);
  //// 1) Forward 
  cout << pad_all[1]->GetUxmin() << endl;
  cout << pad_all[1]->GetUxmax() << endl;
  pad_all[0]->cd();
  //cout << g_RpPb_sys[2]->GetYaxis()->GetXmin() << endl;
  TGaxis *yaxis01 = new TGaxis(1.0,0.0,1.0,1.0-topmargin,0.0,1.8,510,"");
  yaxis01->SetTitleFont(42);
  yaxis01->SetLabelFont(42);
  yaxis01->SetTitle("R_{pPb}");
  yaxis01->SetTitleOffset(1.1);
  yaxis01->CenterTitle(1);
  yaxis01->SetTitleSize(0.075*(0.29/0.07));
  yaxis01->SetLabelSize(0.055*(0.29/0.07));
  yaxis01->Draw();  
  
  TGaxis *yaxis02 = new TGaxis(1.0,0.0+bottommargin,1.0,1.0,0.0,1.8,510,"");
  yaxis02->SetTitleFont(42);
  yaxis02->SetLabelFont(42);
  yaxis02->SetTitle("R_{pPb}");
  yaxis02->SetTitleOffset(1.1);
  yaxis02->CenterTitle(1);
  yaxis02->SetTitleSize(0.075*(0.29/0.07));
  yaxis02->SetLabelSize(0.055*(0.29/0.07));
  pad_all[5]->cd();
  yaxis02->Draw();  

  //c_all->cd();
  //globtex->SetTextColor(kRed);
	//globtex->SetTextSize(0.1);
  //globtex->DrawLatex(0.5, 0.5, "XIA");

  CMS_lumi( c_all, isPA, iPos );
    
  c_all->SaveAs(Form("plot_theory/comp_RpPb_pt_isSmoothened%d_Vogt.pdf",(int)isSmoothened));
  c_all->SaveAs(Form("plot_theory/comp_RpPb_pt_isSmoothened%d_Vogt.png",(int)isSmoothened));
 
  ///////////////////////////////////////////////////////////////////
  // save as a root file
  TFile* outFile = new TFile(Form("plot_theory/comp_RpPb_pt_isSmoothened%d_Vogt.root",(int)isSmoothened),"RECREATE");
  outFile->cd();
  for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
    g_RpPb_theory[iy]->Write();
  } 
   
  return;
}

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
  //latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);
  latex.DrawLatex(1-r+0.01,1-t+lumiTextOffset*t+0.01,lumiText);//KYO
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
      if (iPosX==33) {posX_ += 0.03; posY_-=0.01; } // KYO
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

