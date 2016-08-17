#include "CMS_lumi.h"
#include "../SONGKYO.h"

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

//// EPS09LO, EPS09NLO, nCTEQ15
void comp_RpPb_pt_Lansberg(double ptmax=32, bool isLine=true, bool isSmoothened=false, TString szPDF= "nCTEQ15")
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 10;  // 0:pp, 1:pPb, 10 : pp & pPb together for RpPb plot
	int iPos=0;

  bool isPrompt=true;
	
  ///////////////////////////////////////////////////
	///////// from Ramona
	const int nRapRpPb = 7;
	//const int nPtRpPb = 49;
	const int nPtRpPb = 7; // 4, 5, 6.5, 7.5, 8.5, 10, 14,  30 GeV
  Double_t theory_px[nRapRpPb][nPtRpPb]; 
	Double_t theory_py[nRapRpPb][nPtRpPb];
	Double_t theory_exlow_tmp[nRapRpPb][nPtRpPb];
	Double_t theory_exhigh_tmp[nRapRpPb][nPtRpPb];
  Double_t theory_exlow[nRapRpPb][nPtRpPb];
  Double_t theory_exhigh[nRapRpPb][nPtRpPb];
	Double_t theory_eylow_tmp[nRapRpPb][nPtRpPb];
	Double_t theory_eyhigh_tmp[nRapRpPb][nPtRpPb];
  Double_t theory_eylow[nRapRpPb][nPtRpPb];
	Double_t theory_eyhigh[nRapRpPb][nPtRpPb];
  Double_t ptlimit[nRapRpPb] = {4.0, 6.5, 6.5, 6.5, 6.5, 5.0, 4.0};
  int ipt_init[nRapRpPb] = {0, 2, 2, 2, 2, 1, 0}; // take into acount different pT limit for each rapidity bin (0=4.0, 1=5.0, 2=6.5 GeV)
  
  ///////////////////////////////////////////////////////////////////
  //// read-in txt
  ///////////////////////////////////////////////////////////////////
  
  /// iy=0 (1.5 < y < 1.93) 
  TString inText[nRapRpPb];
  for (int iy=0; iy<nRapRpPb; iy++) {
    inText[iy] = Form("./fromLansberg/pt_%1d_%s.dat",iy+1,szPDF.Data());
    cout << "inText["<<iy<<"] = " << inText[iy] << endl;
  }
  
  string headers;
  TString pxmindum, pxmaxdum, ppbdum, eylow_tmpdum, eyhigh_tmpdum, ppdum;
  int counts=0;
  
  for (int iy=0; iy<nRapRpPb; iy++) {
    cout << endl << " ************* iy = " << iy << endl;
    std::ifstream f0(inText[iy].Data(),std::ios::in);
    getline(f0, headers); // remove prefix
    getline(f0, headers); // remove prefix
    counts=0;
    while(!f0.eof()) {
      f0 >> pxmindum >> pxmaxdum >> ppbdum >> eylow_tmpdum >> eyhigh_tmpdum >> ppdum;
      //cout << pxmindum <<"\t"<< pxmaxdum <<"\t"<< ppbdum <<"\t"<< eylow_tmpdum <<"\t"<< eyhigh_tmpdum<<"\t"<< ppdum << endl;
      theory_px[iy][counts+ipt_init[iy]] =(atof(pxmindum) + atof(pxmaxdum))/2.;
      theory_py[iy][counts+ipt_init[iy]] = (atof(ppbdum) / atof(ppdum) );
      theory_exlow_tmp[iy][counts+ipt_init[iy]] = atof(pxmindum);
      theory_exhigh_tmp[iy][counts+ipt_init[iy]] = atof(pxmaxdum);
      theory_eylow_tmp[iy][counts+ipt_init[iy]] = ( atof(eylow_tmpdum) / atof(ppdum) );
      theory_eyhigh_tmp[iy][counts+ipt_init[iy]] = ( atof(eyhigh_tmpdum) / atof(ppdum) );
      counts++;
      //cout << "counts = " << counts << endl;
    } //end of while file open
  } 
 
  /////////////////////////////////////////////////////////////////////////////////
  
  //// set unused values as zero 
  for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
    for (Int_t ipt=nPtRpPb-1; ipt>=0; ipt--) {
      if (ipt <ipt_init[iy]) {
        theory_px[iy][ipt] = theory_px[iy][ipt+1];
        theory_py[iy][ipt] = theory_py[iy][ipt+1];
        theory_exlow_tmp[iy][ipt] = theory_exlow_tmp[iy][ipt+1];
        theory_exhigh_tmp[iy][ipt] = theory_exhigh_tmp[iy][ipt+1];
        theory_eylow_tmp[iy][ipt] = theory_eylow_tmp[iy][ipt+1];
        theory_eyhigh_tmp[iy][ipt] = theory_eyhigh_tmp[iy][ipt+1];
      }
    }
  }

  //// set proper ex and ey
  for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
    cout << endl << " ************* iy = " << iy << endl;
    for (Int_t ipt=0; ipt<nPtRpPb; ipt++) {
		  theory_exlow[iy][ipt] = fabs(theory_px[iy][ipt] - theory_exlow_tmp[iy][ipt]);
		  theory_exhigh[iy][ipt] = fabs(theory_px[iy][ipt] - theory_exhigh_tmp[iy][ipt]);
      //theory_exlow[iy][ipt] = 0.5;
      //theory_exhigh[iy][ipt] = 0.5;
		  theory_eylow[iy][ipt] = fabs(theory_py[iy][ipt] - theory_eylow_tmp[iy][ipt]);
		  theory_eyhigh[iy][ipt] = fabs(theory_py[iy][ipt] - theory_eyhigh_tmp[iy][ipt]);
      cout << "theory_px = " << theory_px[iy][ipt] << ", theory_py = " << theory_py[iy][ipt] << endl; 	
      cout << "theory_eylow_tmp = " << theory_eylow_tmp[iy][ipt] <<", theory_eyhigh_tmp = " << theory_eyhigh_tmp[iy][ipt] << endl; 	
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
  TFile *inFile = new TFile("../DrawFinalPlot/plot_RpPb/RpPb_pt_isPrompt1.root");
  
  TGraphAsymmErrors* g_RpPb_sys[nRapRpPb];
  TGraphAsymmErrors* g_RpPb[nRapRpPb];
  for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
    g_RpPb_sys[iy] = (TGraphAsymmErrors*)inFile->Get(Form("g_RpPb_sys_%d",iy));
    g_RpPb[iy] = (TGraphAsymmErrors*)inFile->Get(Form("g_RpPb_%d",iy));
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
    if (isSmoothened) g_RpPb_theory[iy]->Draw("3");
    else g_RpPb_theory[iy]->Draw("5");
    dashedLine(0.,1.,32.,1.,1,1);
  }
    
  c_all->SaveAs(Form("plot_theory/comp_RpPb_pt_isSmoothened%d_Lansberg_%s.pdf",(int)isSmoothened,szPDF.Data()));
  c_all->SaveAs(Form("plot_theory/comp_RpPb_pt_isSmoothened%d_Lansberg_%s.png",(int)isSmoothened,szPDF.Data()));
 
  ///////////////////////////////////////////////////////////////////
  // save as a root file
  TFile* outFile = new TFile(Form("plot_theory/comp_RpPb_pt_isSmoothened%d_Lansberg_%s.root",(int)isSmoothened,szPDF.Data()),"RECREATE");
  outFile->cd();
  for (int iy = 0 ; iy < nRapRpPb; iy ++ ) {
    g_RpPb_theory[iy]->Write();
  } 
  
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

