#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);
void formEtArr(Double_t min, Double_t max, TString* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void draw_RFB_ethf(bool sysByHand=true, bool noPtWeight = false, bool isPrompt=false)
{
	gROOT->Macro("./tdrstyle_kyo.C");
  //gStyle->SetTitleXOffset(1.13);
  int isPA = 1;	
	//int iPos=0; //outside topleft
	int iPos=33; //right corner

	//double pxshift = 1.3;
	double pxshift = 0.1;

	// BR and lumi info.
	const Double_t br = 0.0593 ;
	const Double_t brErr = 0.0006;
	Double_t pPb_lumi_nb = 34.622;
	Double_t pPb_lumi_nb_err = 1.2;
	const Double_t pPb_lumi_mub = pPb_lumi_nb * 1000; // (nb)^{-1} -> {#mub}^{-1}
	const Double_t pPb_lumi_mub_err = pPb_lumi_nb_err * 1000; // (nb)^{-1} -> {#mub}^{-1}

	/////////////////////////////////////////////////////////////////////////
	// bin center & systematic uncertainties by hand	
	//inh=0 : pT 5-6.5, rap 1.5-1.93 
	//inh=1 : pT 6.5-30, rap 1.5-1.93 
	//inh=2 : pT 6.5-30, rap 0.9-1.5 
	//inh=3 : pT 6.5-30, rap 0.0-0.9 
  const int nRap = 6;
  const int nPt = 2;
	const int nEt = 3;
  const int nRapTmp = nRap+1;
  const int nPtTmp = nPt+1;
	const int nEtTmp = 3+1;
	
  const int nRapRFB = 3;	
  const int nPtRFB = 2;	
	const int nHist = 4;
	
  Double_t pxtmp[nHist][nEt]; //x point to fill temporarily
	Double_t pytmp[nHist][nEt]; //y point to fill temporarily
	Double_t eytmp[nHist][nEt]; //y point error to fill temporarily

  //char* xlabel[nEt] = {"0-20","20-30","30-120"};	
  TString xlabel[nEt] = {"0-20","20-30","30-120"};	
  //Double_t px[nEt] = {9.4, 24.3, 37.2};  //x point (mean E_T)
  Double_t px[nEt] = {0.35, 1.35, 2.35};  //x point (mean E_T)
	Double_t ex[nEt] = {0.,0.,0.}; // x stat error (0)
	//Double_t exsys[nEt] = {0.65, 0.65, 0.65}; //x sys err (bot width)
	Double_t exsys[nEt] = {0.05, 0.05, 0.05}; //x sys err (bot width)
	Double_t eysys[nHist][nEt]; //absolute y sys error
	Double_t eysysrel[nHist][nEt]; //relative y sys error
	Double_t eysysrelPR[nHist][nEt]={
		{ 6.607602e-02, 6.308565e-02, 6.567717e-02}, //1.5-1.93 low
    { 6.208894e-02, 5.714427e-02, 6.547308e-02}, //1.5-1.93
		{ 5.046793e-02, 5.150824e-02, 4.854059e-02}, //0.9-1.5
		{ 4.877380e-02, 5.661788e-02, 5.087595e-02} //0.0-0.9
	};
	Double_t eysysrelNP[nHist][nEt]={
    { 1.118566e-01, 1.325494e-01, 9.796744e-02}, // 1.5-1.93 low
    { 7.919457e-02, 6.651956e-02, 8.322826e-02}, //0.5-1.93
    { 7.027312e-02, 7.035414e-02, 5.352782e-02}, //0.9-1.5
    { 8.805486e-02, 1.057997e-01, 9.301805e-02} //0.0-0.9
	};
	for (int inh=0; inh< nHist; inh++){
    for (int iet=0; iet<nEt; iet++){	
      if (isPrompt) eysysrel[inh][iet] = eysysrelPR[inh][iet];
      else eysysrel[inh][iet] = eysysrelNP[inh][iet];
    }
  }
	
  //// 1) y_CM array (from forward to backward)
	Double_t rapArrNumFB[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93};// for pt dist.
	//Double_t rapArrNumBF[nRapTmp] = {-1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1]; 
		//cout << iy <<"th rapBinW = " << rapBinW[iy] <<endl;
	}
	//// 2) pt array
	//Double_t ptArrNum[nPtTmp] = {5.0, 6.5, 10., 30.}; //6rap3pt
	Double_t ptArrNum[nPtTmp] = {5.0, 6.5, 30.}; //6rap2pt
	Double_t ptBinW[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
		//cout << ipt <<"th ptBinW = " << ptBinW[ipt] <<endl;
	}
	//// 3) ethf array
	Double_t etArrNum[nEtTmp] = {0.0, 20.0, 30.0, 120.0};

	//// array string
	TString rapArr[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
		cout << iy <<"th rapArr = " << rapArr[iy] << endl;
	}
	TString ptArr[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
		cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
	}
	TString etArr[nEt];
	for (Int_t iet=0; iet<nEt; iet++){
		formEtArr(etArrNum[iet], etArrNum[iet+1], &etArr[iet]);
		cout << iet<< "th etArr = "<< etArr[iet] << endl;
	}	

  /////////////////////////////////////////////////////////////////////////	
	//// read-in file
	TFile * f2D[nEt];
	for (Int_t iet=0; iet<nEt; iet++){
    if (noPtWeight) { f2D[iet] = new TFile(Form("../FittingResult/totalHist_pA_6rap2pt_newcut_nominal_Zvtx1_SF1_etOpt%d_noPtWeight.root",iet+1));
    } else { f2D[iet] = new TFile(Form("../FittingResult/totalHist_pA_6rap2pt_newcut_nominal_Zvtx1_SF1_etOpt%d.root",iet+1)); 
    }
	}
	
  //// read-in 2D hist
	TH2D* h2D_CorrY[nEt];
	for (int iet=0; iet<nEt; iet++){
		if (isPrompt) { h2D_CorrY[iet] = (TH2D*)f2D[iet]->Get("h2D_CorrY_PR_pA"); }
		else { h2D_CorrY[iet] = (TH2D*)f2D[iet]->Get("h2D_CorrY_NP_pA"); }
		h2D_CorrY[iet]->SetName(Form("h2D_CorrY_%d",iet)); 
	}	

	const int nbinsX = h2D_CorrY[0]->GetNbinsX();
	const int nbinsY = h2D_CorrY[0]->GetNbinsY();
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };

	//  projection to 1D hist : iy=0 refers to forwards !!! (ordering here)
	TH1D* h1D_CorrY[nEt][nRap]; 
	for (Int_t iet=0; iet<nEt; iet++){
		for (Int_t iy = 0; iy < nRap; iy++) {
			h1D_CorrY[iet][iy] = h2D_CorrY[iet]->ProjectionY(Form("h1D_CorrY_%d_%d",iet,iy),iy+1,iy+1);
			h1D_CorrY[iet][iy]->SetName(Form("h1D_CorrY_%d_%d",iet,iy));
		}
	}
	
  //////////////////////////////////////////////////////////////////
	/////////// calculate RFB
	
	TH1D* h1D_RFB_tmp[nEt][nRap]; // corrYield with merged pT		
	TH1D* h1D_RFB[nEt][nRapRFB]; // actual RFB vs pt	
	
	//// RFB binsetting
	TString rapAbsArr[nRap];
	for (Int_t iy=0; iy<nRapRFB; iy++) {
		formAbsRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapAbsArr[iy]);
		cout << iy <<"th rapAbsArr = " << rapAbsArr[iy] << endl;
	}
	Double_t ptArrRFBNum[nPtRFB+1] = {5.0, 6.5, 30.};

	//// merging pT bins (KYO - byHand)
	double tmpPRval01, tmpPRerr01, tmpPRval02, tmpPRerr02;
	double actPRval01, actPRerr01, actPRval02, actPRerr02;
	for (int iet=0; iet<nEt; iet++){
		for (int iy=0; iy<nRapRFB*2; iy++){
			h1D_RFB_tmp[iet][iy]= new TH1D(Form("h1D_RFB_tmp_%d_%d",iet,iy),Form("h1D_RFB_tmp_%d_%d",iet,iy),nPtRFB,ptArrRFBNum);
			h1D_RFB_tmp[iet][iy]->Sumw2();
			actPRval01=0; actPRval02=0; actPRerr01=0; actPRerr02=0;
			//// 1) pT 5-6.5 GeV
			tmpPRval01=0; tmpPRval02=0; tmpPRerr01=0; tmpPRerr02=0;
			if (iy==0 || iy==nRapRFB*2-1){
				actPRval01=h1D_CorrY[iet][iy]->GetBinContent(1);	
				actPRerr01=h1D_CorrY[iet][iy]->GetBinError(1);	
				h1D_RFB_tmp[iet][iy]->SetBinContent(1,actPRval01);
				h1D_RFB_tmp[iet][iy]->SetBinError(1,actPRerr01);
			} else {
				h1D_RFB_tmp[iet][iy]->SetBinContent(1,0.);
				h1D_RFB_tmp[iet][iy]->SetBinError(1,0.);
			}
			//// 2) pT 6.5-30. GeV
			tmpPRval01=0; tmpPRval02=0; tmpPRerr01=0; tmpPRerr02=0;
			actPRval02=h1D_CorrY[iet][iy]->GetBinContent(2);	
			actPRerr02=h1D_CorrY[iet][iy]->GetBinError(2);	
			h1D_RFB_tmp[iet][iy]->SetBinContent(2,actPRval02);
			h1D_RFB_tmp[iet][iy]->SetBinError(2,actPRerr02);
		}
	}

	//////////////////////////////////////////////////////////////////
  //// actual RFB calculation	 (still vs pT)
	for (int iet=0; iet<nEt; iet++){
		for (int iy=0; iy<nRapRFB; iy++){
			h1D_RFB[iet][iy] = (TH1D*)h1D_RFB_tmp[iet][iy]->Clone(Form("h1D_RFB_%d_%d",iet,iy));
			h1D_RFB[iet][iy]->Divide(h1D_RFB_tmp[iet][2*nRapRFB-iy-1]); 
		}
	}	
	
  ///////////////////////////////////////////////
	//////// --- RFB vs ETHF : 4 histograms (convert from vs pT to vs ET)
	//inh=0 : pT 5-6.5, rap 1.5-1.93 
	//inh=1 : pT 6.5-30, rap 1.5-1.93 
	//inh=2 : pT 6.5-30, rap 0.9-1.5 
	//inh=3 : pT 6.5-30, rap 0.0-0.9 
	double tmpRFBval01, tmpRFBerr01, tmpRFBval02, tmpRFBerr02, tmpRFBval03, tmpRFBerr03;
	TH1D* h1D_RFB_ETHF[nHist];
	
	for (int in=0; in< nHist; in++){
		h1D_RFB_ETHF[in]= new TH1D(Form("h1D_RFB_ETHF_%d",in),Form("h1D_RFB_ETHF_%d",in),nEt,etArrNum);
		h1D_RFB_ETHF[in]->Sumw2();
		tmpRFBval01=0;tmpRFBerr01=0;tmpRFBval02=0;tmpRFBerr02=0;tmpRFBval03=0;tmpRFBerr03=0;
		for (int iet=0; iet<nEt;iet++){
			if (in==0) {
				tmpRFBval01=h1D_RFB[iet][0]->GetBinContent(1);	
				tmpRFBerr01=h1D_RFB[iet][0]->GetBinError(1);	
				h1D_RFB_ETHF[in]->SetBinContent(iet+1,tmpRFBval01);
				h1D_RFB_ETHF[in]->SetBinError(iet+1,tmpRFBerr01);
			}			
			else {
				tmpRFBval01=h1D_RFB[iet][in-1]->GetBinContent(2);	
				tmpRFBerr01=h1D_RFB[iet][in-1]->GetBinError(2);	
				h1D_RFB_ETHF[in]->SetBinContent(iet+1,tmpRFBval01);
				h1D_RFB_ETHF[in]->SetBinError(iet+1,tmpRFBerr01);
			}
		}
	}		
	
	//////////////////////////////////////////////////////////////////
	
	//TLegend *legBL = new TLegend(0.20, 0.16, 0.45, 0.36);
	//TLegend *legUL = new TLegend(0.53, 0.16, 0.78, 0.26);
	TLegend *legBL = new TLegend(0.21, 0.17, 0.46, 0.37);
	TLegend *legUL = new TLegend(0.58, 0.17, 0.83, 0.27);
	SetLegendStyle(legBL);	
	SetLegendStyle(legUL);	
	//legBL->SetTextSize(0.032);
	//legUL->SetTextSize(0.032);
	legBL->SetTextSize(0.037);
	legUL->SetTextSize(0.037);
	
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);
  
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
	c1->cd();
  
  //// dummy hist. for xlabel
  TH1D* hdummy = new TH1D("hdummy","",nEt,0,nEt);
	for (int iet=0; iet<nEt;iet++){
    hdummy->GetXaxis()->SetBinLabel(iet+1,xlabel[iet]);
  	hdummy->GetXaxis()->SetTitle("E_{T}^{HF |#eta|>4} [GeV]");
  	hdummy->GetXaxis()->CenterTitle();
  	hdummy->GetYaxis()->SetTitle("R_{FB}");
  	hdummy->GetYaxis()->CenterTitle();
    //hdummy->SetTitleSize(0.048,"x");
    hdummy->SetLabelSize(0.07,"x");
    cout << "OFFSET" << hdummy->GetLabelOffset("x")<<endl;
    hdummy->SetLabelOffset(0.003,"x");
  	//hdummy->GetXaxis()->SetLimits(0.,50.0);
  	hdummy->GetXaxis()->SetLimits(0.,3.0);
  	//hdummy->SetMinimum(0.5);
  	//hdummy->SetMaximum(1.15);
  	hdummy->SetMinimum(0.0);
  	hdummy->SetMaximum(1.8);
  }
  hdummy->Draw();

	//////////////////////////////////////////////////////////////////
	//// convert to TGraphAsymErrors
	//// 1) sys graph
	TGraphAsymmErrors* gRFB_sys[nHist];
	for (int inh=0; inh< nHist; inh++){
		gRFB_sys[inh] = new TGraphAsymmErrors(h1D_RFB_ETHF[inh]);
		gRFB_sys[inh]->SetName(Form("gRFB_sys_%d",inh));
		for (int iet=0; iet<nEt;iet++){
			gRFB_sys[inh]->GetPoint(iet, pxtmp[inh][iet], pytmp[inh][iet]);
			/////absolute error calculation
			eysys[inh][iet] = eysysrel[inh][iet]*pytmp[inh][iet];
			//gRFB_sys[in]->SetPoint(iet, px[iet], pytmp[in][iet]);
			gRFB_sys[inh]->SetPoint(iet, px[iet]+pxshift*(nHist-1-inh), pytmp[inh][iet]);
			gRFB_sys[inh]->SetPointError(iet, exsys[iet], exsys[iet], eysys[inh][iet], eysys[inh][iet]);
		}
	}
	
  //// 2) RFB graph
	TGraphAsymmErrors*gRFB[nHist]; 
	for (int inh=0; inh< nHist; inh++){
		gRFB[inh] = new TGraphAsymmErrors(h1D_RFB_ETHF[inh]);
		gRFB[inh]->SetName(Form("gRFB_%d",inh));
		for (int iet=0; iet<nEt;iet++){
			gRFB[inh]->GetPoint(iet, pxtmp[inh][iet], pytmp[inh][iet]);
			eytmp[inh][iet] = gRFB[inh] -> GetErrorY(iet);
//			gRFB[inh]->SetPoint(iet, px[iet], pytmp[inh][iet]);
			gRFB[inh]->SetPoint(iet, px[iet]+pxshift*(nHist-1-inh), pytmp[inh][iet]);
			gRFB[inh]->SetPointEXlow(iet, ex[inh]);
			gRFB[inh]->SetPointEXhigh(iet, ex[inh]);
			cout << "" << endl;
			cout << "RFB["<<inh<<"]["<<iet<<"] = " << pytmp[inh][iet]<<endl;
			cout << "stat.["<<inh<<"]["<<iet<<"] = " << eytmp[inh][iet]<<endl;
			cout << "syst.["<<inh<<"]["<<iet<<"] = " << eysys[inh][iet]<<endl;
		}
	}

/*	
	for (int inh=0; inh< nHist; inh++){
  	gRFB_sys[inh]->GetXaxis()->SetTitle("E_{T}^{HF |#eta|>4} [GeV]");
  	gRFB_sys[inh]->GetXaxis()->CenterTitle();
  	gRFB_sys[inh]->GetYaxis()->SetTitle("R_{FB}");
  	gRFB_sys[inh]->GetYaxis()->CenterTitle();
  	//gRFB_sys[inh]->GetXaxis()->SetLimits(0.,50.0);
  	gRFB_sys[inh]->GetXaxis()->SetLimits(0.,3.0);
  	//gRFB_sys[inh]->SetMinimum(0.5);
  	//gRFB_sys[inh]->SetMaximum(1.15);
  	gRFB_sys[inh]->SetMinimum(0.0);
  	gRFB_sys[inh]->SetMaximum(1.8);
  }
*/
	gRFB_sys[0]->SetFillColorAlpha(kMagenta-10,0.5);
	gRFB_sys[0]->SetLineColor(kViolet-6);
	gRFB_sys[1]->SetFillColorAlpha(kGreen-10,0.5);
	gRFB_sys[1]->SetLineColor(kGreen+3);
	gRFB_sys[2]->SetFillColorAlpha(kBlue-10,0.5);
	gRFB_sys[2]->SetLineColor(kBlue-2);
	gRFB_sys[3]->SetFillColorAlpha(kRed-10,0.5);
	gRFB_sys[3]->SetLineColor(kPink-6);

	SetGraphStyleFinal(gRFB[0], 8, 6); //1.5-1.93 low
	SetGraphStyleFinal(gRFB[1], 0, 5); //1.5-1.93
	SetGraphStyleFinal(gRFB[2], 2, 3); //0.9-1.5
	SetGraphStyleFinal(gRFB[3], 1, 0); //0-0.9
	gRFB[0]->SetMarkerSize(1.4);
	gRFB[1]->SetMarkerSize(2.1);
	gRFB[2]->SetMarkerSize(1.4);
	gRFB[3]->SetMarkerSize(1.4);

  legBL->SetHeader("6.5 < p_{T} < 30 GeV/c");	
  //TLegendEntry *le1=legBL->AddEntry("le1","0 < |y_{CM}| < 0.9, 6.5 < p_{T} < 30 GeV/c","lpf");
  TLegendEntry *le1=legBL->AddEntry("le1","0 < |y_{CM}| < 0.9","lpf");
	le1->SetFillColorAlpha(kRed-10,0.5);
	le1->SetFillStyle(1001);
	le1->SetLineColor(kPink-6);
	le1->SetMarkerStyle(kFullCircle);
	le1->SetMarkerColor(kPink-6);
	le1->SetMarkerSize(1.7);
	TLegendEntry *le2=legBL->AddEntry("le2","0.9 < |y_{CM}| < 1.5","lpf");
	le2->SetFillColorAlpha(kBlue-10,0.5);
	le2->SetFillStyle(1001);
	le2->SetLineColor(kBlue-2);
	le2->SetMarkerStyle(kFullSquare);
	le2->SetMarkerColor(kBlue-2);
	le2->SetMarkerSize(1.7);
	TLegendEntry *le3=legBL->AddEntry("le3","1.5 < |y_{CM}| < 1.93","lpf");
	le3->SetFillColorAlpha(kGreen-10,0.5);
	le3->SetFillStyle(1001);
	le3->SetLineColor(kGreen+3);
	le3->SetMarkerStyle(kFullDiamond);
	le3->SetMarkerColor(kGreen+3);
	le3->SetMarkerSize(2.5);
	//legBL->Draw();
  
  legUL->SetHeader("5 < p_{T} < 6.5 GeV/c");	
  TLegendEntry *le4=legUL->AddEntry("le4","1.5 < |y_{CM}| < 1.93","lpf");
	le4->SetFillColorAlpha(kMagenta-10,0.5);
	le4->SetFillStyle(1001);
	le4->SetLineColor(kViolet-6);
	le4->SetMarkerStyle(kFullCross);
	le4->SetMarkerColor(kViolet-6);
	le4->SetMarkerSize(2.1);
	//legUL->Draw();

  
  ///////////////////////////////////////////////////////////////////
  //// step by step	
  ///////////////////////////////////////////////////////////////////
  
  //// STEP1
  //gRFB_sys[3]->Draw("A5");
  gRFB_sys[3]->Draw("5");
	solidLine(0.,1.,3.,1.,1,1);
	gRFB[3]->Draw("P");
	//solidLine(0.,1.,50.,1.,1,1);
	CMS_lumi( c1, isPA, iPos );
	c1->Update();
	globtex->SetTextAlign(32); //1:left, 2:vertical center
	globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.92, 0.76, "Prompt J/#psi");
	else globtex->DrawLatex(0.92, 0.76, "Nonprompt J/#psi");
	legBL->Draw();
	legUL->Draw();

  if (noPtWeight) {
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_noPtWeight_STEP1.pdf",(int)isPrompt));
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_noPtWeight_STEP1.png",(int)isPrompt));
  } else {
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_STEP1.pdf",(int)isPrompt));
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_STEP1.png",(int)isPrompt));
  }

  //// STEP2
  gRFB_sys[2]->Draw("5");
	gRFB[2]->Draw("P");
  if (noPtWeight) {
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_noPtWeight_STEP2.pdf",(int)isPrompt));
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_noPtWeight_STEP2.png",(int)isPrompt));
  } else {
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_STEP2.pdf",(int)isPrompt));
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_STEP2.png",(int)isPrompt));
  }
  
  //// STEP3
  gRFB_sys[1]->Draw("5");
	gRFB[1]->Draw("P");
  if (noPtWeight) {
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_noPtWeight_STEP3.pdf",(int)isPrompt));
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_noPtWeight_STEP3.png",(int)isPrompt));
  } else {
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_STEP3.pdf",(int)isPrompt));
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_STEP3.png",(int)isPrompt));
  }
  
  //// STEP4 && Final
  gRFB_sys[0]->Draw("5");
	gRFB[0]->Draw("P");
  if (noPtWeight) {
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_noPtWeight.pdf",(int)isPrompt));
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d_noPtWeight.png",(int)isPrompt));
  } else {
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d.pdf",(int)isPrompt));
  	c1->SaveAs(Form("plot_RFB/RFB_ethf_isPrompt%d.png",(int)isPrompt));
  }


  ///////////////////////////////////////////////////////////////////
	//// save as a root file
	TFile *outFile;
  if (noPtWeight) outFile = new TFile(Form("plot_RFB/RFB_ethf_isPrompt%d_noPtWeight.root",(int)isPrompt),"RECREATE");
	else outFile = new TFile(Form("plot_RFB/RFB_ethf_isPrompt%d.root",(int)isPrompt),"RECREATE");
	outFile->cd();
	for (int inh=0; inh< nHist; inh++){
		gRFB_sys[inh]->Write();	
		gRFB[inh]->Write();	
	}
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

void formEtArr(Double_t min, Double_t max, TString* arr) {
	*arr = Form("%.0f < E_{T}^{HF |#eta|>4} < %.0f GeV",min,max);
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

