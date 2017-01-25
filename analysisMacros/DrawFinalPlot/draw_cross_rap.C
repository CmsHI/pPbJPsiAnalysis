#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void draw_cross_rap(bool sysByHand=false, bool noPtWeight=false, bool isScale=false, int isPA = 0, bool isPrompt=true)
{
	gROOT->Macro("./tdrstyle_kyo.C");
  gStyle->SetTitleYOffset(1.38); //KYO
  if (isPA==0) gStyle->SetTitleYOffset(1.5); //KYO
	//int iPos=0.;//outOfFrame
	int iPos=33;//right corner

	//// pileup rejection!!
	Double_t pileReg;
  if (isPA==0) pileReg = 1;
  else pileReg = 128234./123240.;
	//const Double_t pileRegRelErr = 0.23;
	
	//// zvtx correction!!
  //const Double_t zvtxCor = 1.064; // not used anymore

	//// BR and lumi info.
	const Double_t br = 0.0593 ;
	const Double_t brErr = 0.0006;
	const Double_t pp_lumi_pb =27.972; // 28.0/pb
	const Double_t pp_lumi_pb_err = 0.643; // 2.3 %
	const Double_t pPb_lumi_nb = 34.622; // 34.6/nb
	const Double_t pPb_lumi_nb_err = 1.211; // 3.5 %
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
    cout << "select among isPA = 0, or 1"<< endl; return ;
  }
	cout << "isPA = " << isPA << ", and lumi_mub = " << lumi_mub <<"+-" <<lumi_mub_err <<  endl;
	cout << " *** pileReg = " << pileReg << endl;

	/////////////////////////////////////////////////////////////////////////
	//// bin center & systematic uncertainties by hand  
	const int nRap = 8;	
	const int nPt = 9;
	const int nRapTmp = nRap + 1;
	const int nPtTmp = nPt + 1;
	const int nRapRFB = 3;	
	const int nPtRFB = 3;
	
	Double_t pxtmp_lowpt[nRap]; //x point to fill remporarily
	Double_t pxtmp_highpt[nRap]; //x point to fill remporarily
	Double_t pytmp_lowpt[nRap]; //y point to fill remporarily
	Double_t pytmp_highpt[nRap]; //y point to fill remporarily
	Double_t eytmp_lowpt[nRap]; //y point error to fill remporarily
	Double_t eytmp_highpt[nRap]; //y point error to fill remporarily

	Double_t ex[nRap] = {0.0 ,0.0 ,0.0 ,0.0 ,0.0 ,0.0 ,0.0 ,0.0}; // x stat error
	Double_t exlow[nRap]; // x binWidth
	Double_t exhigh[nRap]; //x binWidth
	Double_t exsys[nRap] = {0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08}; // x sys error
	//Double_t eysysrel_lowpt[nRap]; //relative y sys error
	//Double_t eysysrel_highpt[nRap]; //relative y sys error
	Double_t eysys_lowpt[nRap]; //absolute y sys error
	Double_t eysys_highpt[nRap]; //absolute y sys error
	
  //// scaling for drawing
	double	scaleF_low, scaleF_high;
	if (isScale) { scaleF_low = 1.0; scaleF_high = 2.0; }
	else { scaleF_low = 1.0; scaleF_high = 1.0; }
	cout << " *** scaleF_low : " <<scaleF_low<<", scaleF_high = "<<scaleF_high<<endl;
	
  //// 1) y_CM array (from backward to forward)
	//Double_t rapArrNumFB_pA[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
	Double_t rapArrNumBF_pA[nRapTmp] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
	//Double_t rapArrNumFB_pp[nRapTmp] = {2.4, 1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4};// for pt dist.
	Double_t rapArrNumBF_pp[nRapTmp] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4};// for rap dist.
	Double_t rapArrNumBF[nRapTmp];
	for (Int_t iy=0; iy<nRapTmp; iy++) {
		if (isPA==0) { rapArrNumBF[iy] = rapArrNumBF_pp[iy]; }
		else { rapArrNumBF[iy] = rapArrNumBF_pA[iy]; }
	}
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
    //rapBinW[iy] = rapArrNumBF[iy]-rapArrNumBF[iy+1];
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
		//formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
		formRapArr(rapArrNumBF[iy], rapArrNumBF[iy+1], &rapArr[iy]);
		cout << iy <<"th rapArr = " << rapArr[iy] << endl;
	}
	TString ptArr[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
		cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
	}
  
  //// ex calculation
  for (Int_t iy=0; iy<nRap; iy++) {
    exlow[iy] = (rapArrNumBF[iy]+rapArrNumBF[iy+1])/2.-rapArrNumBF[iy]; 
    exhigh[iy] = rapArrNumBF[iy+1]-(rapArrNumBF[iy]+rapArrNumBF[iy+1])/2.; 
  }

	//////////////////////////////////////////////////////////////	
	//// read-in sys. file 
	TFile * fSys;
  if (isPA==0) fSys = new TFile("../TotalSys/TotSys_8rap9pt_pp_etOpt0.root");
  else fSys = new TFile("../TotalSys/TotSys_8rap9pt_pA_etOpt0.root");
	TH2D* h2D_SysErr;
  if (isPrompt) h2D_SysErr = (TH2D*)fSys->Get("hTotalPR");
	else h2D_SysErr = (TH2D*)fSys->Get("hTotalNP");

	//////////////////////////////////////////////////////////////	
	//// read-in corr-yield file
	TFile * f2D;
  if (isPA==0) {
    if (noPtWeight) f2D = new TFile("../FittingResult/totalHist_pp_8rap9pt_newcut_nominal_Zvtx0_SF1_etOpt0_noPtWeight.root");
    else f2D = new TFile("../FittingResult/totalHist_pp_8rap9pt_newcut_nominal_Zvtx0_SF1_etOpt0.root");
  } else {
    if (noPtWeight) f2D = new TFile("../FittingResult/totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF1_etOpt0_noPtWeight.root");
    else f2D = new TFile("../FittingResult/totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF1_etOpt0.root");

  }
	// --- read-in 2D hist for corrected yield
	TH2D* h2D_CorrY;
	if (isPA==0) {
    if (isPrompt) h2D_CorrY = (TH2D*)f2D->Get("h2D_CorrY_PR_pp");
  	else h2D_CorrY = (TH2D*)f2D->Get("h2D_CorrY_NP_pp");
  } else {
    if (isPrompt) h2D_CorrY = (TH2D*)f2D->Get("h2D_CorrY_PR_pA");
  	else h2D_CorrY = (TH2D*)f2D->Get("h2D_CorrY_NP_pA");
  }
	const int nbinsX = h2D_CorrY->GetNbinsX();
	const int nbinsY = h2D_CorrY->GetNbinsY();
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };

  //////////////////////////////////////////////////////////////////////////////////////
	////  projection to 1D hist : iy=0 refers to """"BACKWARD""""" !!! (ordering here)
	TH1D* h1D_CorrY_lab[nPt]; // in y_lab(1st)
	TH1D* h1D_CorrY[nPt];  // in y_CM
	TH1D* h1D_SysErr_lab[nPt];
	TH1D* h1D_SysErr[nPt];
	double tmpval=0;
	double tmperr=0;
	for (Int_t ipt = 0; ipt < nPt; ipt++) {
		h1D_CorrY_lab[ipt] = h2D_CorrY->ProjectionX(Form("h1D_CorrY_lab_%d",ipt),ipt+1,ipt+1);
		h1D_SysErr_lab[ipt] = h2D_SysErr->ProjectionX(Form("h1D_SysErr_lab_%d",ipt),ipt+1,ipt+1);
    h1D_CorrY[ipt] = new TH1D(Form("h1D_CorrY_%d",ipt),Form("h1D_CorrY_%d",ipt),nRap,rapArrNumBF);
		h1D_CorrY[ipt]->Sumw2();
		h1D_SysErr[ipt] = new TH1D(Form("h1D_SysErr_%d",ipt),Form("h1D_SysErr_%d",ipt),nRap,rapArrNumBF);
		h1D_SysErr[ipt]->Sumw2();
		
		for (Int_t iy=0; iy<nRap; iy++){
      //// CorrY
			tmpval = h1D_CorrY_lab[ipt]->GetBinContent(iy+1);
			tmperr = h1D_CorrY_lab[ipt]->GetBinError(iy+1);
			if (isPA==0) {
  			h1D_CorrY[ipt]->SetBinContent(iy+1,tmpval);
  			h1D_CorrY[ipt]->SetBinError(iy+1,tmperr);
      } else {
        h1D_CorrY[ipt]->SetBinContent(nRap-iy,tmpval);
  			h1D_CorrY[ipt]->SetBinError(nRap-iy,tmperr);
      }
      //// Sys
			tmpval = h1D_SysErr_lab[ipt]->GetBinContent(iy+1);
			tmperr = h1D_SysErr_lab[ipt]->GetBinError(iy+1);
			if (isPA==0) {
        h1D_SysErr[ipt]->SetBinContent(iy+1,tmpval);
			  h1D_SysErr[ipt]->SetBinError(iy+1,tmperr);
      } else {
        h1D_SysErr[ipt]->SetBinContent(nRap-iy,tmpval);
			  h1D_SysErr[ipt]->SetBinError(nRap-iy,tmperr);
		  }
		}
	}
	
	//////////////////////////////////////////////////////////////////
  ///////////////////////// pT bin merging /////////////////////////
  //////////////////////////////////////////////////////////////////

	const int lowpt_init=4;
	const int highpt_init=7;	

/*	
	//// find the MAX of sys among pt bins to be merged
	for (Int_t iy=0; iy<nRap; iy++){
		eysysrel_lowpt[iy]=-532;
		for (Int_t ipt = lowpt_init; ipt < highpt_init; ipt++) {
			if(eysysrel_lowpt[iy]<h1D_SysErr[ipt]->GetBinContent(iy+1)) eysysrel_lowpt[iy] = h1D_SysErr[ipt]->GetBinContent(iy+1);
		}
	}	
	for (Int_t iy=0; iy<nRap; iy++){
		eysysrel_highpt[iy]=-532;
		for (Int_t ipt = highpt_init; ipt < nPt; ipt++) {
			if(eysysrel_highpt[iy]<h1D_SysErr[ipt]->GetBinContent(iy+1)) eysysrel_highpt[iy] = h1D_SysErr[ipt]->GetBinContent(iy+1);
		}
	}	
*/
  //// take proper error propagation for sys
	double tmpsys[nRap][nPt];
  
  for (Int_t iy=0; iy<nRap; iy++){
		eysys_lowpt[iy]= 0;
		for (Int_t ipt = lowpt_init; ipt < highpt_init; ipt++) {
      //// from relative error to absolute error
			tmpsys[iy][ipt] = h1D_SysErr[ipt]->GetBinContent(iy+1)*h1D_CorrY[ipt]->GetBinContent(iy+1);
      //cout << iy << "th iy, "<<ipt<<"th ipt h1D_CorrY = " << h1D_CorrY[ipt]->GetBinContent(iy+1) << endl;
      //cout << iy << "th iy, "<<ipt<<"th ipt h1D_SysErr = " << h1D_SysErr[ipt]->GetBinContent(iy+1) << endl;
      //cout << iy << "th iy, "<<ipt<<"th ipt tmpsys = " << tmpsys[iy][ipt] << endl;
      ////eysys_lowpt[iy] += tmpsys[iy][ipt]*tmpsys[iy][ipt];  // same as stat.
      eysys_lowpt[iy] += TMath::Abs(tmpsys[iy][ipt]);  // linear sum (Emilien)
      
		}
    ////eysys_lowpt[iy] = TMath::Sqrt(eysys_lowpt[iy]);
    cout << "eysys_lowpt[iy] = " << eysys_lowpt[iy] << endl;
  }	
  
  for (Int_t iy=0; iy<nRap; iy++){
		eysys_highpt[iy]= 0;
		for (Int_t ipt = highpt_init; ipt < nPt; ipt++) {
      //// from relative error to absolute error
			tmpsys[iy][ipt] = h1D_SysErr[ipt]->GetBinContent(iy+1)*h1D_CorrY[ipt]->GetBinContent(iy+1);
      //cout << iy << "th iy, "<<ipt<<"th ipt h1D_CorrY = " << h1D_CorrY[ipt]->GetBinContent(iy+1) << endl;
      //cout << iy << "th iy, "<<ipt<<"th ipt h1D_SysErr = " << h1D_SysErr[ipt]->GetBinContent(iy+1) << endl;
      //cout << iy << "th iy, "<<ipt<<"th ipt tmpsys = " << tmpsys[iy][ipt] << endl;
      ////eysys_highpt[iy] += tmpsys[iy][ipt]*tmpsys[iy][ipt]; // same as stat. 
      eysys_highpt[iy] += TMath::Abs(tmpsys[iy][ipt]);  // linear sum (Emilien)
		}
    //eysys_highpt[iy] = TMath::Sqrt(eysys_highpt[iy]);
    //cout << "eysys_highpt[iy] = " << eysys_highpt[iy] << endl;
  }	

  //// pt bin merging
	cout << "1) low pT bin starts from : " << ptArr[lowpt_init].Data() << endl;
	for (Int_t ipt = lowpt_init+1; ipt < highpt_init; ipt++) {
		h1D_CorrY[lowpt_init]->Add(h1D_CorrY[ipt]);
    cout << ", merging : " << ptArr[ipt].Data() << endl; 
	}
	cout << "2) high pT bin starts from : " << ptArr[highpt_init].Data() << endl;
	for (Int_t ipt = highpt_init+1; ipt < nPt; ipt++) {
		h1D_CorrY[highpt_init]->Add(h1D_CorrY[ipt]);
		cout << ", merging : " << ptArr[ipt].Data() << endl; 
	}

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////// calcualte cross-section = corrY/(dY*lumi) ///////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	
  TH1D* h1D_cross[nPt]; 
	for (Int_t ipt = 0; ipt < nPt; ipt++) {
    h1D_cross[ipt] = (TH1D*)h1D_CorrY[ipt]->Clone(Form("h1D_cross_%d",ipt));
		//// value normalization (no pT bin)
    h1D_cross[ipt]->Scale(1,"width"); // rapbin
		h1D_cross[ipt]->Scale(1./lumi_mub); // lumi
		//h1D_cross[ipt]->Scale(1./br); //br
    h1D_cross[ipt]->Scale(pileReg); //pileup correction	
	  //// scaling for drawing
	  h1D_cross[ipt]->Scale(scaleF_low);
	  h1D_cross[ipt]->Scale(scaleF_high);
	}
  
  //// syst. nomalization
  for (Int_t iy=0; iy<nRap; iy++){
    eysys_lowpt[iy] /= rapBinW[iy]; //rapbin
    eysys_lowpt[iy] /= lumi_mub; //lumi
    eysys_lowpt[iy] *= pileReg; //pileup correction 
    eysys_lowpt[iy] *= scaleF_low; //scale for drawing
    eysys_highpt[iy] /= rapBinW[iy]; //rapbin
    eysys_highpt[iy] /= lumi_mub; //lumi
    eysys_highpt[iy] *= pileReg; //pileup correction 
    eysys_highpt[iy] *= scaleF_high; //scale for drawing
  }
  	
	//////////////////////////////////////////////////////////////////////
	
  TString ptArr_low;
	TString ptArr_high;
	formPtArr(ptArrNum[lowpt_init], ptArrNum[highpt_init], &ptArr_low);
	formPtArr(ptArrNum[highpt_init], ptArrNum[nPt], &ptArr_high);

	TLegend *legUL = new TLegend(0.17, 0.77, 0.56, 0.88);
	SetLegendStyle(legUL);
	//legUL->SetTextSize(0.037);
	legUL->SetTextSize(0.040);

	TLatex* globtex = new TLatex();
	globtex->SetNDC();
  //globtex->SetTextAlign(12); //1:left, 2:vertical center
	globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);
	
	//////////////////////////////////////////////////////////////////////
	//// convert to TGraphErrors 	
	
  TCanvas* c1 = new TCanvas("c1","c1",200,10,600,600);
	c1->cd();
	
	//// 1) cross_lowpt
	TGraphAsymmErrors* g_cross_lowpt = new TGraphAsymmErrors(h1D_cross[lowpt_init]);
	g_cross_lowpt->SetName("g_cross_lowpt");
	for (Int_t iy=0; iy<nRap; iy++){
		g_cross_lowpt->SetPointEXlow(iy,ex[iy]);
		g_cross_lowpt->SetPointEXhigh(iy,ex[iy]);
		g_cross_lowpt->GetPoint(iy, pxtmp_lowpt[iy], pytmp_lowpt[iy]);
		eytmp_lowpt[iy] = g_cross_lowpt-> GetErrorY(iy);
	}
	//// 2) sys_lowpt
	TGraphAsymmErrors* g_cross_sys_lowpt = new TGraphAsymmErrors(h1D_cross[lowpt_init]);
	g_cross_sys_lowpt->SetName("g_cross_sys_lowpt");
	for (Int_t iy=0; iy<nRap; iy++){
		//// absolute error calculation
		//eysys_lowpt[iy]=eysysrel_lowpt[iy]*pytmp_lowpt[iy];
		//g_cross_sys_lowpt->SetPointError(iy, exsys[iy], exsys[iy], scaleF_low*eysys_lowpt[iy], scaleF_low*eysys_lowpt[iy]);
		g_cross_sys_lowpt->SetPointError(iy, exlow[iy], exhigh[iy], scaleF_low*eysys_lowpt[iy], scaleF_low*eysys_lowpt[iy]);
		//cout << "" << endl;
		//cout << "cross_lowpt["<<iy<<"] = " << pytmp_lowpt[iy]<<endl;
		//cout << "stat._lowpt["<<iy<<"] = " << eytmp_lowpt[iy]<<endl;
		//cout << "syst._lowpt["<<iy<<"] = " << eysys_lowpt[iy]<<endl;
	}
	
	//// 3) cross_highpt
	TGraphAsymmErrors* g_cross_highpt = new TGraphAsymmErrors(h1D_cross[highpt_init]);
	g_cross_highpt->SetName("g_cross_highpt");
	for (Int_t iy=0; iy<nRap; iy++){
		g_cross_highpt->SetPointEXlow(iy,ex[iy]);
		g_cross_highpt->SetPointEXhigh(iy,ex[iy]);
		g_cross_highpt->GetPoint(iy, pxtmp_highpt[iy], pytmp_highpt[iy]);
		eytmp_highpt[iy] = g_cross_highpt-> GetErrorY(iy);
	}
	
	//// 4) sys_highpt
	TGraphAsymmErrors* g_cross_sys_highpt = new TGraphAsymmErrors(h1D_cross[highpt_init]);
	g_cross_sys_highpt->SetName("g_cross_sys_highpt");
	for (Int_t iy=0; iy<nRap; iy++){
		//// absolute error calculation
		//eysys_highpt[iy]=eysysrel_highpt[iy]*pytmp_highpt[iy];
		//g_cross_sys_highpt->SetPointError(iy, exsys[iy], exsys[iy], scaleF_high*eysys_highpt[iy], scaleF_high*eysys_highpt[iy]);
		g_cross_sys_highpt->SetPointError(iy, exlow[iy], exhigh[iy], scaleF_high*eysys_highpt[iy], scaleF_high*eysys_highpt[iy]);
    //cout << "" << endl;
		//cout << "cross_highpt["<<iy<<"] = " << pytmp_highpt[iy]<<endl;
		//cout << "stat._highpt["<<iy<<"] = " << eytmp_highpt[iy]<<endl;
		//cout << "syst._highpt["<<iy<<"] = " << eysys_highpt[iy]<<endl;
	}
  
  cout << "::: for excel ::: " << endl;
	for (Int_t iy=0; iy<nRap; iy++){
    cout << pytmp_lowpt[nRap-1-iy] <<"\t"<<eytmp_lowpt[nRap-1-iy] << "\t "<<eysys_lowpt[nRap-1-iy]<<endl;
    cout << pytmp_highpt[nRap-1-iy] <<"\t"<<eytmp_highpt[nRap-1-iy] << "\t "<<eysys_highpt[nRap-1-iy]<<endl;
	}

	
	g_cross_sys_lowpt->GetXaxis()->SetTitle("y_{CM}");	
	g_cross_sys_lowpt->GetXaxis()->CenterTitle();	
	g_cross_sys_lowpt->GetYaxis()->SetTitle("B d#sigma/dy (#mub)");	
	g_cross_sys_lowpt->GetYaxis()->CenterTitle();	
  //if (isPA==0) { g_cross_sys_lowpt->GetXaxis()->SetLimits(-2.4,2.4);}
  //else { g_cross_sys_lowpt->GetXaxis()->SetLimits(-2.87,1.93);}	
  if (isPA==0) { g_cross_sys_lowpt->GetXaxis()->SetLimits(-2.5,2.5);}
  else { g_cross_sys_lowpt->GetXaxis()->SetLimits(-3.0,2.1);}	
  g_cross_sys_lowpt->SetMinimum(0.0);	
	if (isPA==0) {
    if (isPrompt) g_cross_sys_lowpt->SetMaximum(0.02);	
    else g_cross_sys_lowpt->SetMaximum(0.006);	
  } else {
    if (isPrompt) g_cross_sys_lowpt->SetMaximum(4.);	
		else g_cross_sys_lowpt->SetMaximum(1.2);	
	}
	g_cross_sys_lowpt->SetFillColorAlpha(kRed-10,0.5);	
	g_cross_sys_lowpt->SetLineColor(kPink-6);	
	g_cross_sys_lowpt->Draw("A5");
	g_cross_sys_highpt->SetFillColorAlpha(kBlue-10,0.5);	
	g_cross_sys_highpt->SetLineColor(kBlue-2);	
	g_cross_sys_highpt->Draw("5");
	SetGraphStyleFinal(g_cross_lowpt,1,0);
	g_cross_lowpt->SetMarkerSize(1.4);
	g_cross_lowpt->Draw("P");
	SetGraphStyleFinal(g_cross_highpt,2,3);
	//g_cross_highpt->SetMarkerSize(2.1);
	g_cross_highpt->SetMarkerSize(1.4);
	g_cross_highpt->Draw("P");

	if (isScale && scaleF_low != 1.0) legUL -> AddEntry(g_cross_lowpt,Form("6.5 < p_{T} < 10 GeV/c [x%1.f]",scaleF_low), "lp");
	else legUL -> AddEntry(g_cross_lowpt,"6.5 < p_{T} < 10 GeV/c","lp");
	if (isScale && scaleF_high != 1.0) legUL -> AddEntry(g_cross_highpt,Form("10 < p_{T} < 30 GeV/c [x%.1f]",scaleF_high),"lp");
	else legUL -> AddEntry(g_cross_highpt,"10 < p_{T} < 30 GeV/c","lp");
	legUL->Draw();
	globtex->SetTextSize(0.048);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.92, 0.77, "Prompt J/#psi");
  else globtex->DrawLatex(0.92, 0.77, "Nonprompt J/#psi");
	//globtex->SetTextSize(0.035);
	//globtex->SetTextFont(42);
  //if (isPA==0) globtex->DrawLatex(0.93, 0.79, "Global uncertainty : 4 \%");
	//else globtex->DrawLatex(0.93, 0.79, "Global uncertainty : 3.5 \%");
	CMS_lumi( c1, isPA, iPos );
	
  dashedLine (0.,0.,0.,g_cross_sys_lowpt->GetMaximum()*0.75,1,1);
  c1->Update();
  if (isPA==0){
    if (noPtWeight) {
      c1->SaveAs(Form("plot_cross/pp_cross_rap_isPrompt%d_isScale%d_noPtWeight.pdf",(int)isPrompt,(int)isScale));
      c1->SaveAs(Form("plot_cross/pp_cross_rap_isPrompt%d_isScale%d_noPtWeight.png",(int)isPrompt,(int)isScale));
    } else {
      c1->SaveAs(Form("plot_cross/pp_cross_rap_isPrompt%d_isScale%d.pdf",(int)isPrompt,(int)isScale));
      c1->SaveAs(Form("plot_cross/pp_cross_rap_isPrompt%d_isScale%d.png",(int)isPrompt,(int)isScale));
    }
  } else {
    if (noPtWeight) {
      c1->SaveAs(Form("plot_cross/pA_cross_rap_isPrompt%d_isScale%d_noPtWeight.pdf",(int)isPrompt,(int)isScale));
      c1->SaveAs(Form("plot_cross/pA_cross_rap_isPrompt%d_isScale%d_noPtWeight.png",(int)isPrompt,(int)isScale));
    } else {
      c1->SaveAs(Form("plot_cross/pA_cross_rap_isPrompt%d_isScale%d.pdf",(int)isPrompt,(int)isScale));
      c1->SaveAs(Form("plot_cross/pA_cross_rap_isPrompt%d_isScale%d.png",(int)isPrompt,(int)isScale));
    }
  }
	
  ///////////////////////////////////////////////////////////////////
	//// save as a root file
	TFile *outFile;
  if (isPA==0) {
    if (noPtWeight) {
      outFile = new TFile(Form("plot_cross/pp_cross_rap_isPrompt%d_isScale%d_noPtWeight.root",(int)isPrompt,(int)isScale),"RECREATE");
    } else {
      outFile = new TFile(Form("plot_cross/pp_cross_rap_isPrompt%d_isScale%d.root",(int)isPrompt,(int)isScale),"RECREATE");
    }
  }else {
    if (noPtWeight) {
      outFile = new TFile(Form("plot_cross/pA_cross_rap_isPrompt%d_isScale%d_noPtWeight.root",(int)isPrompt,(int)isScale),"RECREATE");
    } else {
      outFile = new TFile(Form("plot_cross/pA_cross_rap_isPrompt%d_isScale%d.root",(int)isPrompt,(int)isScale),"RECREATE");
    }
  }

	outFile->cd();
	g_cross_sys_lowpt->Write();
	g_cross_lowpt->Write();
	g_cross_sys_highpt->Write();
	g_cross_highpt->Write();
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

