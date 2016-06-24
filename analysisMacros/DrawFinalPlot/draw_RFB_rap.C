#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void draw_RFB_rap(bool sysByHand=true,  bool noPtWeight=true, bool isPrompt = true)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 1;  // 0:pp, 1:pPb
	int iPos=0;

	double pxshift = 0.06;

	//// BR and lumi info.
	const Double_t br = 0.0593 ;
	const Double_t brErr = 0.0006;
	const Double_t pPb_lumi_nb = 34.622;
	const Double_t pPb_lumi_nb_err = 1.2;
	const Double_t pPb_lumi_mub = pPb_lumi_nb * 1000; // (nb)^{-1} -> {#mub}^{-1}
	const Double_t pPb_lumi_mub_err = pPb_lumi_nb_err * 1000; // (nb)^{-1} -> {#mub}^{-1}
	
	/////////////////////////////////////////////////////////////////////////
	//// systematic uncertainties by hand
	const int nRap = 8;
	const int nPt = 9;
	const int nRapTmp = nRap + 1;
	const int nPtTmp = nPt + 1;
	const int nRapRFB=3;
	
  Double_t pxtmp_lowpt[nRapRFB]; // x point to fill temporarily
	Double_t pxtmp_highpt[nRapRFB]; // x point to fill temporarily
	Double_t pytmp_lowpt[nRapRFB]; // y point to fill temporarily
	Double_t pytmp_highpt[nRapRFB]; // y point to fill temporarily
	Double_t eytmp[nRapRFB]; // y point error to fill temporarily
	Double_t ex[nRapRFB] = {0.0, 0.0, 0.0}; //x stat error (0)
	Double_t exsys[nRapRFB] = {0.03, 0.03, 0.03}; //x sys error (box width)
	Double_t eysys_lowpt[nRapRFB]; //absolute y sys error
	Double_t eysys_highpt[nRapRFB]; //absolute y sys error
	Double_t eysysrelPR_lowpt[nRapRFB] = { //relative y sys error
		0.04694, //0-0.9
		0.04245, //0.9-1.5
		0.04795}; //1.5-1.93; 
	Double_t eysysrelNP_lowpt[nRapRFB] = { //relative y sys error
		0.07412,
		0.06288,
		0.10487};
	Double_t eysysrelPR_highpt[nRapRFB] = { //relative y sys error
		0.03705,
		0.03360,
		0.06486};
	Double_t eysysrelNP_highpt[nRapRFB] = { //relative y sys error
		0.04483,
		0.05592,
		0.09280};
	Double_t eysysrel_lowpt[nRapRFB];
	Double_t eysysrel_highpt[nRapRFB];
	for (int iy=0; iy<nRapRFB; iy++){
		if (isPrompt) { 
			eysysrel_lowpt[iy] = eysysrelPR_lowpt[iy];
			eysysrel_highpt[iy] = eysysrelPR_highpt[iy];
		}
		else { 
			eysysrel_lowpt[iy] = eysysrelNP_lowpt[iy];
			eysysrel_highpt[iy] = eysysrelNP_highpt[iy];
		}
	}
			
	//// 1) y_CM array (from forward to backward)
	//Double_t rapArrNumFB[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
	Double_t rapArrNumBF[nRapTmp] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
//	const Int_t nRap = sizeof(rapArrNumBF)/sizeof(Double_t)-1;
//	cout << "nRap = " << nRap << endl;
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		rapBinW[iy] = rapArrNumBF[iy+1]-rapArrNumBF[iy]; 
	}
	//// 2) pt array
	Double_t ptArrNum[nPtTmp] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
	Double_t ptBinW[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
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

  /////////////////////////////////////////////////////////////////////////
	//// read-in file
	TFile * f2D;
  if (noPtWeight) f2D = new TFile("../FittingResult/totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF1_noPtWeight.root");
  else f2D = new TFile("../FittingResult/totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF1.root");
	//// read-in 2D hist
	TH2D* h2D_CorrY;
	if (isPrompt) h2D_CorrY = (TH2D*)f2D->Get("h2D_CorrY_PR_pA");
	else h2D_CorrY = (TH2D*)f2D->Get("h2D_CorrY_NP_pA");
	h2D_CorrY->SetName("h2D_CorrY");
  const int nbinsX = h2D_CorrY->GetNbinsX();
	const int nbinsY = h2D_CorrY->GetNbinsY();
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
	
  //// projection to 1D hist : iy=0 refers to forwards !!! (ordering here && in CM)
	TH1D* h1D_CorrY_lab[nPt]; // in y_lab(1st)
	TH1D* h1D_CorrY[nPt]; // in y_CM
	int tmpbin; double tmpval=0; double tmperr=0;
	for (Int_t ipt = 0; ipt < nPt; ipt++) {
		h1D_CorrY_lab[ipt] = h2D_CorrY->ProjectionX(Form("h1D_CorrY_lab_%d",ipt),ipt+1,ipt+1);
		h1D_CorrY[ipt] = new TH1D(Form("h1D_CorrY_%d",ipt),Form("h1D_CorrY_%d",ipt),nRap,rapArrNumBF);
		h1D_CorrY[ipt]->Sumw2();
		for (Int_t iy=0; iy<nRap; iy++){
			tmpval = h1D_CorrY_lab[ipt]->GetBinContent(iy+1);
			tmperr = h1D_CorrY_lab[ipt]->GetBinError(iy+1);
			h1D_CorrY[ipt]->SetBinContent(nRap-iy,tmpval);
			h1D_CorrY[ipt]->SetBinError(nRap-iy,tmperr);
			//h1D_CorrY[ipt]->SetBinContent(iy+1,tmpval);
			//h1D_CorrY[ipt]->SetBinError(iy+1,tmperr);
		}
	}
	
	//////////////////////////////////////////////////////////////////
	//// calculate RFB
	
	const int lowpt_init=4;
	const int highpt_init=7;	

	//// pt bin merging
	cout << "1) low pT bin starts from : " << ptArr[lowpt_init].Data() << endl;
	for (Int_t ipt = lowpt_init+1; ipt < highpt_init; ipt++) {
		h1D_CorrY[lowpt_init]->Add(h1D_CorrY[ipt]);
		cout << ", merging : " << ptArr[ipt].Data() << endl; 
	}
	cout << "2) high pT bin starts from : " << ptArr[highpt_init].Data() << endl;
	for (Int_t ipt = highpt_init+1; ipt < nPt; ipt++) {
		h1D_CorrY[highpt_init]->Add(h1D_CorrY[ipt]);
		cout << " , merging : " << ptArr[ipt].Data() << endl; 
	}
		
	//// |yCM| for drawing (actual binning)
	Double_t rapArrRFBNum[nRapRFB+1] = {0., 0.9, 1.5, 1.93};// for rap dist.

	//////////////////////////////////////////////////////////////////
	//// actual RFB calculation
	TH1D* h1D_RFB[nPt]; //byHand	
	int rap_init = 2;

	double tmpFWval[nRapRFB], tmpBWval[nRapRFB]; 
	double tmpFWerr[nRapRFB], tmpBWerr[nRapRFB];
	double actval[nRapRFB], acterr[nRapRFB];
	for (Int_t ipt = 0; ipt < nPt; ipt++) {
		if (ipt != lowpt_init && ipt != highpt_init) continue; // one bin for lowpT, and one bin for high pT
		h1D_RFB[ipt] = new TH1D(Form("h1D_RFB_%d",ipt),Form("h1D_RFB_%d",ipt),nRapRFB,rapArrRFBNum);
		h1D_RFB[ipt]->Sumw2();
		for (int iy=0; iy <nRapRFB; iy ++ ){ 
			tmpFWval[iy] = h1D_CorrY[ipt]->GetBinContent(nRap-iy); 
			tmpFWerr[iy] = h1D_CorrY[ipt]->GetBinError(nRap-iy); 
			tmpBWval[iy] = h1D_CorrY[ipt]->GetBinContent(rap_init+iy+1); 
			tmpBWerr[iy] = h1D_CorrY[ipt]->GetBinError(rap_init+iy+1); 
			DivideValue(tmpFWval[iy],tmpFWerr[iy],tmpBWval[iy],tmpBWerr[iy],&actval[iy],&acterr[iy]);
			h1D_RFB[ipt]->SetBinContent(nRapRFB-iy,actval[iy]);	
			h1D_RFB[ipt]->SetBinError(nRapRFB-iy,acterr[iy]);	
		}
	}	
	
	//////////////////////////////////////////////////////////////////
	//TLegend *legBR = new TLegend(0.51, 0.17, 0.71, 0.38);
	TLegend *legBR = new TLegend(0.45, 0.18, 0.70, 0.32);
	SetLegendStyle(legBR);
	legBR->SetTextSize(0.05);
	 	
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);
	
  TString ptArr_lowpt;
	TString ptArr_highpt;
	formPtArr(ptArrNum[lowpt_init], ptArrNum[highpt_init], &ptArr_lowpt);
	formPtArr(ptArrNum[highpt_init], ptArrNum[nPt], &ptArr_highpt);
  
  TCanvas* c1 = new TCanvas("c1","c1",200,10,600,600);
	c1->cd();
	
	//////////////////////////////////////////////
	//// convert to TGraphErrors	
	
	//// 1) RFB_lowpt
	TGraphAsymmErrors* gRFB_lowpt = new TGraphAsymmErrors(h1D_RFB[lowpt_init]); 
	gRFB_lowpt->SetName("gRFB_lowpt");
	for (int iy=0; iy <nRapRFB; iy ++ ){ 
		gRFB_lowpt->	SetPointEXlow(iy, ex[iy]);
		gRFB_lowpt->	SetPointEXhigh(iy, ex[iy]);
		gRFB_lowpt->GetPoint(iy, pxtmp_lowpt[iy], pytmp_lowpt[iy]);
		eytmp[iy] = gRFB_lowpt-> GetErrorY(iy);
	}	
	
	//// 2) sys_lowpt
	TGraphAsymmErrors* gRFB_sys_lowpt = new TGraphAsymmErrors(h1D_RFB[lowpt_init]); 
	gRFB_sys_lowpt->SetName("gRFB_sys_lowpt");
	for (int iy=0; iy <nRapRFB; iy ++ ){ 
		//// absolute error calculation.
		eysys_lowpt[iy]=eysysrel_lowpt[iy]*pytmp_lowpt[iy];
		gRFB_sys_lowpt->SetPointError(iy, exsys[iy], exsys[iy], eysys_lowpt[iy], eysys_lowpt[iy]);
    cout << "" << endl;
		cout << "RFB_lowpt["<<iy<<"] = " << pytmp_lowpt[iy]<<endl;
		cout << "stat._lowpt["<<iy<<"] = " << eytmp[iy]<<endl;
		cout << "sys._lowpt["<<iy<<"] = " << eysys_lowpt[iy]<<endl;
	}	

	//// 3) RFB_highpt
	TGraphAsymmErrors* gRFB_highpt = new TGraphAsymmErrors(h1D_RFB[highpt_init]); 
	gRFB_highpt->SetName("gRFB_highpt");
	for (int iy=0; iy <nRapRFB; iy ++ ){ 
		gRFB_highpt->	SetPointEXlow(iy, ex[iy]);
		gRFB_highpt->	SetPointEXhigh(iy, ex[iy]);
		gRFB_highpt->GetPoint(iy, pxtmp_highpt[iy], pytmp_highpt[iy]);
		gRFB_highpt->SetPoint(iy, pxtmp_highpt[iy]+pxshift, pytmp_highpt[iy]);
		eytmp[iy] = gRFB_highpt-> GetErrorY(iy);
	}	
	
	//// 4) sys_highpt
	TGraphAsymmErrors* gRFB_sys_highpt = new TGraphAsymmErrors(h1D_RFB[highpt_init]); 
	gRFB_sys_highpt->SetName("gRFB_sys_highpt");
	for (int iy=0; iy <nRapRFB; iy ++ ){ 
		//abs err calcul.
		eysys_highpt[iy]=eysysrel_highpt[iy]*pytmp_highpt[iy];
		gRFB_sys_highpt->GetPoint(iy, pxtmp_highpt[iy], pytmp_highpt[iy]);
		gRFB_sys_highpt->SetPoint(iy, pxtmp_highpt[iy]+pxshift, pytmp_highpt[iy]);
		gRFB_sys_highpt->SetPointError(iy, exsys[iy], exsys[iy], eysys_highpt[iy], eysys_highpt[iy]);
    cout << "" << endl;
		cout << "RFB_highpt["<<iy<<"] = " << pytmp_highpt[iy]<<endl;
		cout << "stat._highpt["<<iy<<"] = " << eytmp[iy]<<endl;
		cout << "sys._highpt["<<iy<<"] = " << eysys_highpt[iy]<<endl;
	}	
	
	gRFB_sys_lowpt->GetXaxis()->SetTitle("|y_{CM}|");	
	gRFB_sys_lowpt->GetXaxis()->CenterTitle();	
	gRFB_sys_lowpt->GetYaxis()->SetTitle("R_{FB}");	
	gRFB_sys_lowpt->GetYaxis()->CenterTitle();	
	gRFB_sys_lowpt->GetXaxis()->SetLimits(0.,2.);	
	gRFB_sys_lowpt->SetMinimum(0.5);
	gRFB_sys_lowpt->SetMaximum(1.15);
	gRFB_sys_lowpt->SetFillColor(kRed-10);	
	gRFB_sys_lowpt->Draw("A2");
	gRFB_sys_highpt->SetFillColor(kTeal-9);
	gRFB_sys_highpt->Draw("2");
	
	SetGraphStyleFinal(gRFB_lowpt, 1, 3);
	gRFB_lowpt->Draw("P");	
	SetGraphStyleFinal(gRFB_highpt, 0, 5);
	gRFB_highpt->SetMarkerSize(3.3);
	gRFB_highpt->Draw("P");	
	
	dashedLine(0.,1.,2.,1.,1,1);
	TLegendEntry *le1=legBR->AddEntry("le1",Form("  %s", ptArr_lowpt.Data()),"lpf");
  le1->SetFillColor(kRed-10);
  le1->SetFillStyle(1001);
  le1->SetLineColor(kPink-6);
  le1->SetMarkerStyle(kFullSquare);
  le1->SetMarkerColor(kPink-6);
  le1->SetMarkerSize(2.1);
	//legBR->Draw();
	TLegendEntry *le2=legBR->AddEntry("le2",Form("  %s", ptArr_highpt.Data()),"lpf");
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
	CMS_lumi( c1, isPA, iPos );
	c1->Update();

  if (noPtWeight) {
  	c1->SaveAs(Form("plot_RFB/RFB_rap_isPrompt%d_noPtWeight.pdf",(int)isPrompt));
  	c1->SaveAs(Form("plot_RFB/RFB_rap_isPrompt%d_noPtWeight.png",(int)isPrompt));
  } else {
  	c1->SaveAs(Form("plot_RFB/RFB_rap_isPrompt%d.pdf",(int)isPrompt));
  	c1->SaveAs(Form("plot_RFB/RFB_rap_isPrompt%d.png",(int)isPrompt));
  }
	
	///////////////////////////////////////////////////////////////////
	//// save as a root file
	TFile *outFile;
  if (noPtWeight) outFile= new TFile(Form("plot_RFB/RFB_rap_isPrompt%d_noPtWeight.root",(int)isPrompt),"RECREATE");
  else outFile= new TFile(Form("plot_RFB/RFB_rap_isPrompt%d.root",(int)isPrompt),"RECREATE");
	outFile->cd();
	gRFB_sys_lowpt->Write();	
	gRFB_lowpt->Write();	
	gRFB_sys_highpt->Write();	
	gRFB_highpt->Write();	
	outFile->Close();
	
	return;

} // end of main func.

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
		*arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < p_{T} < %.1f GeV/c", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
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

