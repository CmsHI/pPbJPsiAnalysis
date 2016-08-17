#include "CMS_lumi.h"
#include "../SONGKYO.h"

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

//// EPS09LO, EPS09NLO, nCTEQ15
void comp_RpPb_rap_Lansberg(bool isLine=true, bool isSmoothened=true, TString szPDF= "nCTEQ15")
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 10;  // 0:pp, 1:pPb, 10 : pp & pPb together for RpPb plot
	int iPos=33;

  bool isPrompt=true;
	
  ///////////////////////////////////////////////////
	///////// from Ramona
	const int nRapRpPb = 7;
	//const int nPtRpPb = 49;
	//const int nPtRpPb = 7; // 4, 5, 6.5, 7.5, 8.5, 10, 14,  30 GeV
	const int nPtRpPb = 2; // 6.5, 10, 30 GeV
  Double_t theory_px[nPtRpPb][nRapRpPb]; 
	Double_t theory_py[nPtRpPb][nRapRpPb];
	Double_t theory_exlow_tmp[nPtRpPb][nRapRpPb];
	Double_t theory_exhigh_tmp[nPtRpPb][nRapRpPb];
  Double_t theory_exlow[nPtRpPb][nRapRpPb];
  Double_t theory_exhigh[nPtRpPb][nRapRpPb];
	Double_t theory_eylow_tmp[nPtRpPb][nRapRpPb];
	Double_t theory_eyhigh_tmp[nPtRpPb][nRapRpPb];
  Double_t theory_eylow[nPtRpPb][nRapRpPb];
	Double_t theory_eyhigh[nPtRpPb][nRapRpPb];
  //Double_t ptlimit[nRapRpPb] = {4.0, 6.5, 6.5, 6.5, 6.5, 5.0, 4.0};
  //int ipt_init[nRapRpPb] = {0, 2, 2, 2, 2, 1, 0}; // take into acount different pT limit for each rapidity bin (0=4.0, 1=5.0, 2=6.5 GeV)
  
  ///////////////////////////////////////////////////////////////////
  //// read-in txt
  ///////////////////////////////////////////////////////////////////
  
  /// iy=0 (1.5 < y < 1.93) 
  TString inText[nPtRpPb];
  for (int ipt=0; ipt<nPtRpPb; ipt++) {
    inText[ipt] = Form("./fromLansberg/yCM_%1d_%s.dat",ipt+1,szPDF.Data());
    cout << "inText["<<ipt<<"] = " << inText[ipt] << endl;
  }
  
  string headers;
  TString pxmindum, pxmaxdum, ppbdum, eylow_tmpdum, eyhigh_tmpdum, ppdum;
  int counts=0;
  double pxdumnum, pydumnum, exlow_tmpdumnum, exhigh_tmpdumnum, eylow_tmpdumnum, eyhigh_tmpdumnum;
  
  for (int ipt=0; ipt<nPtRpPb; ipt++) {
    cout << endl << " ************* ipt = " << ipt << endl;
    std::ifstream f0(inText[ipt].Data(),std::ios::in);
    getline(f0, headers); // remove prefix
    getline(f0, headers); // remove prefix
    counts=0;
    while(!f0.eof()) {
      f0 >> pxmindum >> pxmaxdum >> ppbdum >> eylow_tmpdum >> eyhigh_tmpdum >> ppdum;
      cout << pxmindum <<"\t"<< pxmaxdum <<"\t"<< ppbdum <<"\t"<< eylow_tmpdum <<"\t"<< eyhigh_tmpdum<<"\t"<< ppdum << endl;
      theory_px[ipt][counts] =(atof(pxmindum) + atof(pxmaxdum))/2.;
      theory_py[ipt][counts] = (atof(ppbdum) / atof(ppdum) );
      theory_exlow_tmp[ipt][counts] = atof(pxmindum);
      theory_exhigh_tmp[ipt][counts] = atof(pxmaxdum);
      theory_eylow_tmp[ipt][counts] = ( atof(eylow_tmpdum) / atof(ppdum) );
      theory_eyhigh_tmp[ipt][counts] = ( atof(eyhigh_tmpdum) / atof(ppdum) );
      if (counts==0 && ipt==0) { //// KYO : something wrong in saving into array?
        pxdumnum = (atof(pxmindum) + atof(pxmaxdum))/2.;
        pydumnum = ( atof(ppbdum) / atof(ppdum) );
        exlow_tmpdumnum = atof(pxmindum);
        exhigh_tmpdumnum = atof(pxmaxdum);
        eylow_tmpdumnum = ( atof(eylow_tmpdum) / atof(ppdum) );
        eyhigh_tmpdumnum = ( atof(eyhigh_tmpdum) / atof(ppdum) );
      }
      counts++;
      cout << "counts = " << counts << endl;
    } //end of while file open
  } 
  /////////////////////////////////////////////////////////////////////////////////
  
  //// KYO : something wrong in saving into array?
  theory_px[0][0] = pxdumnum;
  theory_py[0][0] = pydumnum;
  theory_exlow_tmp[0][0] = exlow_tmpdumnum;
  theory_exhigh_tmp[0][0] = exhigh_tmpdumnum;
  theory_eylow_tmp[0][0] = eylow_tmpdumnum;
  theory_eyhigh_tmp[0][0] = eyhigh_tmpdumnum;
  
  //// set proper ex and ey
  for (int ipt = 0 ; ipt < nPtRpPb; ipt ++ ) {
    cout << endl << " ************* ipt = " << ipt << endl;
    for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
		  theory_exlow[ipt][iy] = fabs(theory_px[ipt][iy] - theory_exlow_tmp[ipt][iy]);
		  theory_exhigh[ipt][iy] = fabs(theory_px[ipt][iy] - theory_exhigh_tmp[ipt][iy]);
      //theory_exlow[ipt][iy] = 0.5;
      //theory_exhigh[ipt][iy] = 0.5;
		  theory_eylow[ipt][iy] = fabs(theory_py[ipt][iy] - theory_eylow_tmp[ipt][iy]);
		  theory_eyhigh[ipt][iy] = fabs(theory_py[ipt][iy] - theory_eyhigh_tmp[ipt][iy]);
      cout << "theory_px = " << theory_px[ipt][iy] << ", theory_py = " << theory_py[ipt][iy] << endl; 	
      cout << "theory_exlow_tmp = " << theory_exlow_tmp[ipt][iy] <<", theory_exhigh_tmp = " << theory_exhigh_tmp[ipt][iy] << endl; 	
      cout << "theory_eylow_tmp = " << theory_eylow_tmp[ipt][iy] <<", theory_eyhigh_tmp = " << theory_eyhigh_tmp[ipt][iy] << endl; 	
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  
  TGraphAsymmErrors* g_RpPb_theory[nPtRpPb];
  for (int ipt = 0 ; ipt < nPtRpPb; ipt ++ ) {
    g_RpPb_theory[ipt]= new TGraphAsymmErrors(nRapRpPb, theory_px[ipt], theory_py[ipt], theory_exlow[ipt], theory_exhigh[ipt], theory_eylow[ipt], theory_eyhigh[ipt]);	
    g_RpPb_theory[ipt]->SetName(Form("g_RpPb_theory_%d",ipt));
    g_RpPb_theory[ipt]->SetFillColorAlpha(kYellow,0.5);
    g_RpPb_theory[ipt]->SetLineColor(kOrange+7);
  }
	g_RpPb_theory[0]->SetFillColor(kRed+1);
	g_RpPb_theory[0]->SetLineColor(kRed+1);
  if (isSmoothened) g_RpPb_theory[0]->SetFillStyle(3254);
  else g_RpPb_theory[0]->SetFillStyle(3004);
	g_RpPb_theory[1]->SetFillColor(kGreen+1);
	g_RpPb_theory[1]->SetLineColor(kGreen+1);
  if (isSmoothened) g_RpPb_theory[1]->SetFillStyle(3245);
  else g_RpPb_theory[1]->SetFillStyle(3005);
 
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
  TFile *inFile = new TFile("../DrawFinalPlot/plot_RpPb/RpPb_rap_isPrompt1.root");
  
  TGraphAsymmErrors* g_RpPb_sys[nRapRpPb];
  TGraphAsymmErrors* g_RpPb_sys_line[nRapRpPb];
  TGraphAsymmErrors* g_RpPb[nRapRpPb];
  g_RpPb_sys[0] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_sys_lowpt");
  g_RpPb_sys_line[0] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_sys_lowpt_line");
  g_RpPb[0] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_lowpt");
  g_RpPb_sys[1] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_sys_highpt");
  g_RpPb_sys_line[1] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_sys_highpt_line");
  g_RpPb[1] = (TGraphAsymmErrors*)inFile->Get("g_RpPb_highpt");
  g_RpPb_sys[0]->SetName("g_RpPb_sys_lowpt");
  g_RpPb_sys_line[0]->SetName("g_RpPb_sys_lowpt_line");
  g_RpPb_sys[1]->SetName("g_RpPb_sys_highpt");
  g_RpPb_sys_line[1]->SetName("g_RpPb_sys_highpt_line");
  g_RpPb_sys[0]->SetFillColorAlpha(kRed-10,0.5);
  g_RpPb_sys_line[0]->SetFillColorAlpha(kRed-10,0.);
  g_RpPb_sys_line[0]->SetLineColor(kPink-6);
  g_RpPb_sys[1]->SetFillColorAlpha(kGreen-10,0.5);
  g_RpPb_sys_line[1]->SetFillColorAlpha(kGreen-10,0.);
  g_RpPb_sys_line[1]->SetLineColor(kGreen+3);

	///////////////////////////////////////////////////
	//// Draw
  ///////////////////////////////////////////////////
	
	TLegend *legBR = new TLegend(0.45, 0.18, 0.70, 0.32);
	SetLegendStyle(legBR);
	legBR->SetTextSize(0.05);
  
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.075);

  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  c1->cd();
  
  g_RpPb_sys[0]->Draw("A2"); 
  g_RpPb_sys[1]->Draw("2"); 
  g_RpPb_sys_line[0]->Draw("5"); 
  g_RpPb_sys_line[1]->Draw("5"); 
  if (isSmoothened) {
    g_RpPb_theory[0]->Draw("3");
//    g_RpPb_theory[0]->Draw("C");
    g_RpPb_theory[1]->Draw("3");
  } else {
    g_RpPb_theory[0]->Draw("5");
    g_RpPb_theory[1]->Draw("5");
  }
  g_RpPb[0]->Draw("P"); 
  g_RpPb[1]->Draw("P"); 
  
  dashedLine(-2.5,1.,2.1,1.,1,1);

	TLegendEntry *le1=legBR->AddEntry("le1","6.5 < p_{T} < 10 GeV/c","lpf");
  le1->SetFillColor(kRed-10);
  le1->SetFillStyle(1001);
  le1->SetLineColor(kPink-6);
  le1->SetLineWidth(1);
  le1->SetMarkerStyle(kFullSquare);
  le1->SetMarkerColor(kPink-6);
  le1->SetMarkerSize(1.9);
	//legBR->Draw();
	TLegendEntry *le2=legBR->AddEntry("le2","10 < p_{T} < 30 GeV/c","lpf");
  le2->SetFillColor(kGreen-10);
  le2->SetFillStyle(1001);
  le2->SetLineColor(kGreen+3);
  le2->SetMarkerStyle(kFullDiamond);
  le2->SetMarkerColor(kGreen+3);
  le2->SetMarkerSize(3.1);
	legBR->Draw();
	
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");
	globtex->SetTextSize(0.035);
	globtex->SetTextFont(42);
  globtex->DrawLatex(0.21, 0.78, "Global uncertainty : 5.3 \%");
	
  CMS_lumi( c1, isPA, iPos );
    
  c1->SaveAs(Form("plot_theory/comp_RpPb_rap_isSmoothened%d_Lansberg_%s.pdf",(int)isSmoothened,szPDF.Data()));
  c1->SaveAs(Form("plot_theory/comp_RpPb_rap_isSmoothened%d_Lansberg_%s.png",(int)isSmoothened,szPDF.Data()));
  
  ///////////////////////////////////////////////////////////////////
  // save as a root file
  TFile* outFile = new TFile(Form("plot_theory/comp_RpPb_rap_isSmoothened%d_Lansberg_%s.root",(int)isSmoothened,szPDF.Data()),"RECREATE");
  outFile->cd();
  for (int ipt = 0 ; ipt < nPtRpPb; ipt ++ ) {
    g_RpPb_theory[ipt]->Write();
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

