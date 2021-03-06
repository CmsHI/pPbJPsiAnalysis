#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

/// maxrap = 300 (-1.5 < y < 1.5) in fwrap, (do NOT use bwrap) [used for ATLAS R_pPb]
/// maxrap = 386 (-1.93 < y < 1.93) in fwrap, (do NOT use bwrap) [used for B R_pPb]
void draw_cross_pt_integ_middle(bool sysByHand=false, bool noPtWeight=false, bool isScale=false, bool isLog=false, int isPA = 1, bool isPrompt=true, double maxrap=300)
{
	gROOT->Macro("./tdrstyle_kyo.C");
  gStyle->SetTitleYOffset(1.38); //KYO

//	int iPos=33;//right corner
//	int iPos=11;//left corner
	int iPos=0.;//outOfFrame

	// pileup rejection!!
	Double_t pileReg;
  if (isPA==0) pileReg = 1;
  else pileReg = 128234./123240.;
	//const Double_t pileRegRelErr = 0.23;

	//// zvtx correction!!
	//const Double_t zvtxCor = 1.064; // not used anymore!!

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
    cout << "select among isPA = 0 or 1"<< endl; return ;
  }
	cout << "isPA = " << isPA << ", and lumi_mub = " << lumi_mub <<"+-" <<lumi_mub_err <<  endl;
	cout << " *** pileReg = " << pileReg << endl;

  //double A_pb =208;

	/////////////////////////////////////////////////////////////////////////
	//// bin center & systematic uncertainties by hand  
	const int nRap = 8;	
	const int nPt = 9;
	const int nRapTmp = nRap + 1;
	const int nPtTmp = nPt + 1;
	const int nRapRFB = 3;	
	const int nPtRFB = 3;
	
	Double_t pxtmp[nRap][nPt]; //x point to fill remporarily
	Double_t pytmp[nRap][nPt]; //y point to fill remporarily
	Double_t eytmp[nRap][nPt]; //y point error to fill remporarily

	Double_t px[nRap][nPt];
	/*
  Double_t px_pp[nRap][nPt] = { //x point (mean pT) by JB
    {2.54567, 3.50886, 4.48508, 5.69331, 6.97532, 7.97107, 9.17601, 11.5322, 17.4867},
    {0, 0, 4.54938, 5.75633, 6.9727, 7.97359, 9.17558, 11.4729, 17.4391},
    {0, 0, 0, 0, 7.0061, 7.97991, 9.19355, 11.5729, 17.6818},
    {0, 0, 0, 0, 7.08557, 8.01392, 9.2137, 11.6042, 17.9741},
    {0, 0, 0, 0, 7.08944, 8.01343, 9.21616, 11.6091, 17.7608},
    {0, 0, 0, 0, 7.00408, 7.98632, 9.19122, 11.5535, 17.7004},
    {0, 0, 4.54198, 5.76465, 6.97492, 7.96787, 9.18318, 11.5223, 17.4279},
    {2.54164, 3.5085, 4.48298, 5.69705, 6.97263, 7.97372, 9.17313, 11.5032, 17.3023}
	};
	Double_t px_pA[nRap][nPt] = { //x point (mean pT) by JB
    {2.525, 3.51255, 4.4772, 5.70327, 6.96635, 7.96061, 9.17243, 11.5938, 18.0681},
    {0, 0, 4.52793, 5.74033, 6.97622, 7.98335, 9.19458, 11.4927, 17.6693},
    {0, 0, 0, 0, 7.018, 8.00224, 9.19714, 11.5483, 17.6577},
    {0, 0, 0, 0, 7.11111, 8.02103, 9.24485, 11.6204, 17.8454},
    {0, 0, 0, 0, 7.05329, 8.00998, 9.20583, 11.5222, 17.4633},
    {0, 0, 0, 5.84477, 6.98466, 7.97917, 9.17551, 11.5322, 17.34},
    {0, 0, 4.52204, 5.72881, 6.97074, 7.95284, 9.14157, 11.4976, 17.3058},
    {2.51699, 3.4959, 4.47636, 5.68624, 6.97338, 7.97824, 9.1805, 11.4841, 16.8762}
	};
*/	
	//// for middle
  Double_t px_pp[nRap][nPt] = {	
		{2.5, 3.5, 4.5, 5.75, 7, 8, 9.25, 12., 22}, 
		{0., 0., 4.5, 5.75, 7, 8, 9.25, 12., 22},
		{0., 0., 0., 0., 7, 8, 9.25, 12., 22},
		{0., 0., 0., 0., 7, 8, 9.25, 12., 22}, 
		{0., 0., 0., 0., 7, 8, 9.25, 12., 22}, 
		{0., 0., 0., 0., 7, 8, 9.25, 12., 22},
		{0., 0., 4.5, 5.75, 7, 8, 9.25, 12., 22}, 
		{2.5, 3.5, 4.5, 5.75, 7, 8, 9.25, 12., 22}
	};
	Double_t px_pA[nRap][nPt] = {	
		{2.5, 3.5, 4.5, 5.75, 7, 8, 9.25, 12., 22}, 
		{0., 0., 4.5, 5.75, 7, 8, 9.25, 12., 22},
		{0., 0., 0., 0., 7, 8, 9.25, 12., 22},
		{0., 0., 0., 0., 7, 8, 9.25, 12., 22}, 
		{0., 0., 0., 0., 7, 8, 9.25, 12., 22}, 
		{0., 0., 0., 5.75, 7, 8, 9.25, 12., 22},
		{0., 0., 4.5, 5.75, 7, 8, 9.25, 12., 22}, 
		{2.5, 3.5, 4.5, 5.75, 7, 8, 9.25, 12., 22}
	};
  Double_t ex[nPt] = {0,0,0,0,0,0,0,0,0}; // x stat error
	Double_t exlow[nRap][nPt]; // x binWidth
  Double_t exhigh[nRap][nPt]; // x binWidth
  Double_t exsys[nPt] = {0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4}; // x sys error
	//Double_t eysysrel[nRap][nPt]; //relative y sys error
	Double_t eysys[nRap][nPt]; //absolute y sys error
	for (Int_t iy=0; iy<nRap; iy++) {
	  for (Int_t ipt=0; ipt<nPt; ipt++) {
      if (isPA==0) { px[iy][ipt] = px_pp[iy][ipt]; }
      else { px[iy][ipt] = px_pA[iy][ipt]; }
      //cout << "px["<<iy<<"]["<<ipt<<"] = " << px[iy][ipt] << endl;
    }
  }

	//// scaling for drawing
  double scaleF[nRap];
	double scaleF_pp[nRap] = {1000., 100., 10., 1., 1., 10., 100., 1000.};
	double scaleF_pA[nRap] = {100., 10., 1., 1., 10., 100., 1000., 10000.};
  for (int iy=0; iy<nRap; iy++){
    if (isPA==0) {scaleF[iy] = scaleF_pp[iy]; }
    else {scaleF[iy] = scaleF_pA[iy]; }
	  if (!isScale) { scaleF[iy] = 1.; };
    cout << "scaleF["<<iy<<"] = " << scaleF[iy] << endl;	
	}
  
  //// 1) y_CM array (from forward to backward)
	Double_t rapArrNumFB_pA[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
	//Double_t rapArrNumBF_pA[nRapTmp] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
	Double_t rapArrNumFB_pp[nRapTmp] = {2.4, 1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4};// for pt dist.
	//Double_t rapArrNumBF_pp[nRapTmp] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4};// for rap dist.
	Double_t rapArrNumFB[nRapTmp];
	for (Int_t iy=0; iy<nRapTmp; iy++) {
		if (isPA==0) { rapArrNumFB[iy] = rapArrNumFB_pp[iy]; }
		else { rapArrNumFB[iy] = rapArrNumFB_pA[iy]; }
	}
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
    rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1];
    //rapBinW[iy] = rapArrNumBF[iy+1]-rapArrNumBF[iy];
    //cout << iy <<"th rapBinW = " << rapBinW[iy] <<endl;
	}
	//// 2) pt array
	Double_t ptArrNum[nPtTmp] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
	Double_t ptBinW[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
		//cout << ipt <<"th ptBinW = " << ptBinW[ipt] <<endl;
	}
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
  
  //// ex calculation
  for (Int_t iy=0; iy<nRap; iy++) {
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      exlow[iy][ipt] = px[iy][ipt]-ptArrNum[ipt];
      exhigh[iy][ipt] = ptArrNum[ipt+1]-px[iy][ipt];
    }
  }

	//////////////////////////////////////////////////////////////	
	//// read-in sys. file 
	TFile * fSys;
  if (isPA==0) fSys = new TFile("../TotalSys/TotSys_8rap9pt_pp_etOpt0.root");
  else fSys = new TFile("../TotalSys/TotSys_8rap9pt_pA_etOpt0.root");
	TH2D* h2D_SysErr;
  if (isPrompt) h2D_SysErr = (TH2D*)fSys->Get("hTotalPR");
	else h2D_SysErr = (TH2D*)fSys->Get("hTotalNP");
//	cout << " *** h2D_SysErr = " <<  h2D_SysErr << endl;	

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
	//// read-in 2D hist for corrected yield
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

	////  projection to 1D hist : iy=0 refers to forwards !!! (ordering here)
	TH1D* h1D_CorrY[nRap]; 
	TH1D* h1D_SysErr[nRap];
	for (Int_t iy = 0; iy < nRap; iy++) {
		if ( isPA==0) {
      h1D_CorrY[iy] = h2D_CorrY->ProjectionY(Form("h1D_CorrY_%d",iy),nRap-iy,nRap-iy);
		  h1D_SysErr[iy] = h2D_SysErr->ProjectionY(Form("h1D_SysErr_%d",iy),nRap-iy,nRap-iy);
		} else {
      h1D_CorrY[iy] = h2D_CorrY->ProjectionY(Form("h1D_CorrY_%d",iy),iy+1,iy+1);
		  h1D_SysErr[iy] = h2D_SysErr->ProjectionY(Form("h1D_SysErr_%d",iy),iy+1,iy+1);
		}
	}
  //// read sys values from hist	
	//for (Int_t iy = 0; iy < nRap; iy++) {
	//	for (int ipt=0; ipt <nPt; ipt ++ ){ 
	//		eysysrel[iy][ipt] = h1D_SysErr[iy]->GetBinContent(ipt+1);
	//	}
	//}
	
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	
  //// integrate the rapidity for ATLAS!!!
  int fwrap_init, bwrap_init, bwrap_final;
  if (maxrap==193) {
    if (isPA==0) {
      fwrap_init = 1; // y_CM = 1.5-1.93
      bwrap_init = 4; // y_CM = -0.9-0
      bwrap_final = 7; // y_CM = -1.93--1.5
    } else {
      fwrap_init = 0; // y_CM = 1.5-1.93
      bwrap_init = 3; // y_CM = -0.9-0
      bwrap_final = 6; // y_CM = -1.93--1.5
    }
  } else if (maxrap==150) {
    if (isPA==0) {
      fwrap_init = 2; // y_CM = 0.9-1.5
      bwrap_init = 4; // y_CM = -0.9-0
      bwrap_final = 6; // y_CM = -1.5--0.9
    } else {
      fwrap_init = 1; // y_CM = 0.9-1.5
      bwrap_init = 3; // y_CM = -0.9-0
      bwrap_final = 5; // y_CM = -1.5--0.9
    }
  } else if (maxrap==300){
    if (isPA==0) {
      fwrap_init = 2; // y_CM = 0.9-1.5
      bwrap_init = 6; // y_CM = -1.5--0.9
      bwrap_final = nRap; // arbitrary
    } else {
      fwrap_init = 1; // y_CM = 0.9-1.5
      bwrap_init = 5; // y_CM = -1.5-0.9
      bwrap_final = nRap; // arbitrary
    }
  } else if (maxrap==386){ 
    if (isPA==0) {
      fwrap_init = 1; // y_CM = 1.5-1.93
      bwrap_init = 7; // y_CM = -1.93--1.5
      bwrap_final = nRap; // arbitrary
    } else {
      fwrap_init = 0; // y_CM = 1.5-1.93
      bwrap_init = 6; // y_CM = -1.93--1.5
      bwrap_final = nRap; // arbitrary 
    }
  
  } else {
    cout << " *** ERROR : select maxrap among 300 or 386 " << endl; return;
  }
  cout << "max rap = " << (maxrap/100.) << endl;

	//////////////////////////////////////////////////////////////////
  //// systematcis
/*
	/// find the MAX of sys among rap bins
	for (int ipt=0; ipt < nPt; ipt ++ ){ 
		for (Int_t iy = fwrap_init+1; iy < bwrap_init; iy++) {
			if(eysysrel[fwrap_init][ipt]<eysysrel[iy][ipt]) eysysrel[fwrap_init][ipt] = eysysrel[iy][ipt];
		}
		for (Int_t iy = bwrap_init+1; iy < bwrap_final; iy++) {
			if(eysysrel[bwrap_init][ipt]<eysysrel[iy][ipt]) eysysrel[bwrap_init][ipt] = eysysrel[iy][ipt];
		}
		//cout << ipt << "th pt, eysysrel[fwrap_init][ipt] =  " << eysysrel[fwrap_init][ipt] << endl;
		//cout << ipt << "th pt, eysysrel[bwrap_init][ipt] =  " << eysysrel[bwrap_init][ipt] << endl;
	}	
*/
  //// take proper error propagation for sys
  double tmpsys[nRap][nPt];
	for (int ipt=0; ipt < nPt; ipt ++ ){ 
    eysys[fwrap_init][ipt] = 0;
		for (Int_t iy = fwrap_init; iy < bwrap_init; iy++) {
      //// from relative error to absolute error
			tmpsys[iy][ipt] = h1D_SysErr[iy]->GetBinContent(ipt+1)*h1D_CorrY[iy]->GetBinContent(ipt+1);
		  //cout << "fw : " << iy << "th iy, "<<ipt<<"th ipt tmpssys = " << tmpsys[iy][ipt] << endl;
      //eysys[fwrap_init][ipt] += tmpsys[iy][ipt]*tmpsys[iy][ipt]; //same as stat.
      eysys[fwrap_init][ipt] += TMath::Abs(tmpsys[iy][ipt]); //linear sum (Emilien)
    }
    //eysys[fwrap_init][ipt] = TMath::Sqrt(eysys[fwrap_init][ipt]);
    //cout << "fw : eysys[fwrap_init]["<<ipt<<"] = " << eysys[fwrap_init][ipt] << endl;
    
    eysys[bwrap_init][ipt] = 0;
		for (Int_t iy = bwrap_init; iy < bwrap_final; iy++) {
			tmpsys[iy][ipt] = h1D_SysErr[iy]->GetBinContent(ipt+1)*h1D_CorrY[iy]->GetBinContent(ipt+1);
		  //cout << "bw : " << iy << "th iy, "<<ipt<<"th ipt tmpssys = " << tmpsys[iy][ipt] << endl;
      //eysys[bwrap_init][ipt] += tmpsys[iy][ipt]*tmpsys[iy][ipt]; //same as stat.
      eysys[bwrap_init][ipt] += TMath::Abs(tmpsys[iy][ipt]); //linear sum (Emilien)
		}
    //eysys[bwrap_init][ipt] = TMath::Sqrt(eysys[bwrap_init][ipt]);
    //cout << "bw : eysys[bwrap_init]["<<ipt<<"] = " << eysys[bwrap_init][ipt] << endl;
  } 
  //// rap bin merging
	cout << "1) fw rap bin starts from : " << rapArr[fwrap_init].Data() << endl;
	for (Int_t iy = fwrap_init+1; iy < bwrap_init; iy++) {
		h1D_CorrY[fwrap_init]->Add(h1D_CorrY[iy]);
		cout << ", merging : " << rapArr[iy].Data() << endl; 
	}
	cout << "2) bw rap bin starts from : " << rapArr[bwrap_init].Data() << endl;
	for (Int_t iy = bwrap_init+1; iy < bwrap_final; iy++) {
		h1D_CorrY[bwrap_init]->Add(h1D_CorrY[iy]);
		cout << ", merging : " << rapArr[iy].Data() << endl; 
	}
  
  //////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	//// calcualte cross-section = corrY/(dPt*dY*lumi)
	TH1D* h1D_cross[nRap]; 
	for (Int_t iy = 0; iy < nRap; iy++) {
		h1D_cross[iy] = (TH1D*)h1D_CorrY[iy]->Clone(Form("h1D_cross_%d",iy));
		//// normalization
		h1D_cross[iy]->Scale(1,"width"); //pT bin 
		//h1D_cross[iy]->Scale(1./rapBinW[iy]); //rap bin
		h1D_cross[iy]->Scale(1./(maxrap/100.)); //for ATLAS !!
		h1D_cross[iy]->Scale(1./lumi_mub); // lumi
		// h1D_cross[iy]->Scale(1./br); //br
		// h1D_cross[iy]->Scale(zvtxCor); // z vertex correction	
    h1D_cross[iy]->Scale(pileReg);	// pileup correction
		h1D_cross[iy]->Scale(scaleF[iy]); // scaling for drawing
	}
	
  //// syst. nomalization
  for (Int_t iy=0; iy<nRap; iy++){
	  for (int ipt=0; ipt < nPt; ipt ++ ){ 
      eysys[iy][ipt] /= ptBinW[ipt]; //pT bin 
      //eysys[iy][ipt] /= rapBinW[iy]; //rapbin
      eysys[iy][ipt] /= (maxrap/100.); //rapbin
      eysys[iy][ipt] /= lumi_mub; //lumi
      eysys[iy][ipt] *= pileReg; //pileup correction 
      eysys[iy][ipt] *= scaleF[iy]; // scaling for drawing
    } 
  } 
  	
	//// set values as zero for unused bins
  //// for ATLAS
  const int tmpPtBin = 4; // < 6.5 GeV
  for (int iy = 0; iy < nRap; iy++) {
    for (int ipt=0; ipt < tmpPtBin; ipt ++ ){ 
      h1D_cross[iy]->SetBinContent(ipt+1,-532);
      h1D_cross[iy]->SetBinError(ipt+1,0);
    }
  }





  //////////////////////////////////////////////////////////////////

//	TLegend *legBLFW = new TLegend(0.15, 0.165, 0.43, 0.315);
//	TLegend *legBL = new TLegend(0.15, 0.165, 0.43, 0.415);
	TLegend *legBLFW; 
	TLegend *legBLBW;
  if (isPA==0) {
    legBLFW = new TLegend(0.19, 0.165, 0.46, 0.365);
    legBLBW = new TLegend(0.19, 0.165, 0.46, 0.365);
  } else {
    legBLFW = new TLegend(0.19, 0.165, 0.46, 0.315);
    legBLBW = new TLegend(0.19, 0.165, 0.46, 0.415);
  }
	SetLegendStyle(legBLFW);
	SetLegendStyle(legBLBW);
	legBLFW->SetTextSize(0.037); 
	legBLBW->SetTextSize(0.037); 
	 	
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	//globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);

	//// global uncertainty from lumi
	//TBox * globalbox = new TBox(0.5, 6.4, 1.5, 13.6);
	//globalbox->SetFillColor(kYellow);
	
	//////////////////////////////////////////////////////////////////////////////////////
	//// convert to TGraphAsymErrors
	TGraphAsymmErrors* g_cross_sys[nRap];	
	TGraphAsymmErrors* g_cross[nRap];	
	for (Int_t iy = 0; iy < nRap; iy++) {
		g_cross_sys[iy] = new TGraphAsymmErrors(h1D_cross[iy]);
		g_cross[iy] = new TGraphAsymmErrors(h1D_cross[iy]);
		g_cross_sys[iy]->SetName(Form("g_cross_sys_%d",iy));
		g_cross[iy]->SetName(Form("g_cross_%d",iy));
    if (iy==fwrap_init) cout << "::: for excel ::: fwrap_init = " << iy << endl;
    if (iy==bwrap_init) cout << "::: for excel ::: bwrap_init = " << iy << endl;
		for (Int_t ipt=0; ipt<nPt; ipt++ ){
			g_cross_sys[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
			g_cross_sys[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			//// absolute error calculation 
			//eysys[iy][ipt]=eysysrel[iy][ipt]*pytmp[iy][ipt];
			//g_cross_sys[iy]->SetPointError(ipt, exsys[ipt], exsys[ipt], eysys[iy][ipt], eysys[iy][ipt]);
			g_cross_sys[iy]->SetPointError(ipt, exlow[iy][ipt], exhigh[iy][ipt], eysys[iy][ipt], eysys[iy][ipt]);
			g_cross[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
			eytmp[iy][ipt] = g_cross[iy]-> GetErrorY(ipt);
			g_cross[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			g_cross[iy]->SetPointEXlow(ipt, ex[ipt]);
			g_cross[iy]->SetPointEXhigh(ipt, ex[ipt]);
			//cout << "" << endl;
      //cout << "cross["<<iy<<"]["<<ipt<<"] = " << pytmp[iy][ipt]<<endl;
			//cout << "stat.["<<iy<<"]["<<ipt<<"] = " << eytmp[iy][ipt]<<endl;
			//cout << "sys.["<<iy<<"]["<<ipt<<"] = " << eysys[iy][ipt]<<endl;
		  if (iy==fwrap_init) {
        cout << pytmp[iy][ipt] <<"\t"<<eytmp[iy][ipt] << "\t "<<eysys[iy][ipt]<<endl;
      }
      if (iy==bwrap_init){
        cout << pytmp[iy][ipt] <<"\t"<<eytmp[iy][ipt] << "\t "<<eysys[iy][ipt]<<endl;
      }
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	//// Draw
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	
  g_cross_sys[fwrap_init]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	g_cross_sys[fwrap_init]->GetXaxis()->CenterTitle("");
	g_cross_sys[fwrap_init]->GetYaxis()->SetTitle("B d^{2}#sigma/dp_{T}dy (#mub/ GeV/c)");
	g_cross_sys[fwrap_init]->GetYaxis()->CenterTitle("");
	if (isLog) {
		if (isPrompt) {
      g_cross_sys[fwrap_init]->SetMinimum(0.0002);
      g_cross_sys[fwrap_init]->SetMaximum(20.);
    } else {
      g_cross_sys[fwrap_init]->SetMinimum(0.0004);
      g_cross_sys[fwrap_init]->SetMaximum(2.);
    }
  }
  else {
    if (isPrompt) {
      g_cross_sys[fwrap_init]->SetMinimum(-0.05);
      g_cross_sys[fwrap_init]->SetMaximum(1.5);
    } else {
      g_cross_sys[fwrap_init]->SetMinimum(-0.01);
      g_cross_sys[fwrap_init]->SetMaximum(0.4);
	  }
	}
  g_cross_sys[fwrap_init]->GetXaxis()->SetLimits(0.0, 30.);
	g_cross_sys[fwrap_init]->SetFillColorAlpha(kMagenta-10,0.5);
	SetGraphStyleFinal(g_cross[fwrap_init],	8,2);
	g_cross[fwrap_init]->SetMarkerSize(2.1);
	
	g_cross_sys[bwrap_init]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	g_cross_sys[bwrap_init]->GetXaxis()->CenterTitle("");
	g_cross_sys[bwrap_init]->GetYaxis()->SetTitle("B x d^{2}#sigma/dp_{T}dy [#mub/(GeV/c)]");
	g_cross_sys[bwrap_init]->GetYaxis()->CenterTitle("");
	if (isLog) {
		if (isPrompt) {
      g_cross_sys[bwrap_init]->SetMinimum(0.0002);
      g_cross_sys[bwrap_init]->SetMaximum(20.);
    } else {
      g_cross_sys[bwrap_init]->SetMinimum(0.0004);
      g_cross_sys[bwrap_init]->SetMaximum(2.);
    }
  }
  else {
    if (isPrompt) {
      g_cross_sys[bwrap_init]->SetMinimum(-0.05);
      g_cross_sys[bwrap_init]->SetMaximum(1.5);
    } else {
      g_cross_sys[bwrap_init]->SetMinimum(-0.01);
      g_cross_sys[bwrap_init]->SetMaximum(0.4);
	  }
	}
	g_cross_sys[bwrap_init]->GetXaxis()->SetLimits(0.0, 30.);
	g_cross_sys[bwrap_init]->SetFillColorAlpha(kMagenta-10,0.5);
	SetGraphStyleFinal(g_cross[bwrap_init],	8,2);
	g_cross[bwrap_init]->SetMarkerSize(2.1);

  ////////  Forward
	TCanvas* c_fw = new TCanvas("c_fw","c_fw",200,10,600,600);
	c_fw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
  g_cross_sys[fwrap_init]->Draw("A2");
	g_cross[fwrap_init]->Draw("P");
	
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.91, 0.86, "Prompt J/#psi");
	else globtex->DrawLatex(0.91, 0.86, "Nonprompt J/#psi");
	globtex->SetTextSize(0.035);
	globtex->SetTextFont(42);
	if (isPA==0) globtex->DrawLatex(0.91, 0.80, "Global uncertainty : 4 \%");
	else globtex->DrawLatex(0.91, 0.80, "Global uncertainty : 3.5 \%");
	CMS_lumi( c_fw, isPA, iPos );
	c_fw->Update();
/*
  if (isPA==0){
    if (noPtWeight) {
    	c_fw->SaveAs(Form("plot_cross/pp_fw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f_noPtWeight.pdf",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    	c_fw->SaveAs(Form("plot_cross/pp_fw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f_noPtWeight.png",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
	  } else {
    	c_fw->SaveAs(Form("plot_cross/pp_fw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f.pdf",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    	c_fw->SaveAs(Form("plot_cross/pp_fw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f.png",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    }
  } else {
    if (noPtWeight) {
    	c_fw->SaveAs(Form("plot_cross/pA_fw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f_noPtWeight.pdf",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    	c_fw->SaveAs(Form("plot_cross/pA_fw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f_noPtWeight.png",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
	  } else {
    	c_fw->SaveAs(Form("plot_cross/pA_fw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f.pdf",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    	c_fw->SaveAs(Form("plot_cross/pA_fw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f.png",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    }
  }
*/
  legBLFW->Clear();
	
  ////////  Backward
	TCanvas* c_bw = new TCanvas("c_bw","c_bw",200,10,600,600);
	c_bw->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);
  g_cross_sys[bwrap_init]->Draw("A2");
	g_cross[bwrap_init]->Draw("P");
  
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.91, 0.86, "Prompt J/#psi");
	else globtex->DrawLatex(0.91, 0.86, "Nonprompt J/#psi");
	globtex->SetTextSize(0.035);
	globtex->SetTextFont(42);
	if (isPA==0) globtex->DrawLatex(0.91, 0.80, "Global uncertainty : 4 \%");
	else globtex->DrawLatex(0.91, 0.80, "Global uncertainty : 3.5 \%");
	CMS_lumi( c_bw, isPA, iPos );
	c_bw->Update();
/*
  if (isPA==0){
    if (noPtWeight) {
    	c_bw->SaveAs(Form("plot_cross/pp_bw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f_noPtWeight.pdf",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    	c_bw->SaveAs(Form("plot_cross/pp_bw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f_noPtWeight.png",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
	  } else {
    	c_bw->SaveAs(Form("plot_cross/pp_bw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f.pdf",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    	c_bw->SaveAs(Form("plot_cross/pp_bw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f.png",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    }
  } else {
    if (noPtWeight) {
    	c_bw->SaveAs(Form("plot_cross/pA_bw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f_noPtWeight.pdf",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    	c_bw->SaveAs(Form("plot_cross/pA_bw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f_noPtWeight.png",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
	  } else {
    	c_bw->SaveAs(Form("plot_cross/pA_bw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f.pdf",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    	c_bw->SaveAs(Form("plot_cross/pA_bw_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f.png",(int)isPrompt,(int)isLog,(int)isScale,maxrap));
    }
  }
*/
	legBLBW->Clear();
  	
	///////////////////////////////////////////////////////////////////
	//// save as a root file
	TFile *outFile;
  if (isPA==0) {
    if (noPtWeight) {
      outFile = new TFile(Form("plot_cross/pp_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f_noPtWeight.root",(int)isPrompt,(int)isLog,(int)isScale,maxrap),"RECREATE");
    } else {
      outFile = new TFile(Form("plot_cross/pp_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f.root",(int)isPrompt,(int)isLog,(int)isScale,maxrap),"RECREATE");
    }
  }else {
    if (noPtWeight) {
      outFile = new TFile(Form("plot_cross/pA_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f_noPtWeight.root",(int)isPrompt,(int)isLog,(int)isScale,maxrap),"RECREATE");
    } else {
      outFile = new TFile(Form("plot_cross/pA_cross_pt_integ_middle_isPrompt%d_isLog%d_isScale%d_maxrap%.0f.root",(int)isPrompt,(int)isLog,(int)isScale,maxrap),"RECREATE");
    }
  }

	outFile->cd();
	g_cross_sys[fwrap_init]->SetName("g_cross_sys_fwrap");
  g_cross_sys[fwrap_init]->Write(); 
	g_cross[fwrap_init]->SetName("g_cross_fwrap");
  g_cross[fwrap_init]->Write(); 
	g_cross_sys[bwrap_init]->SetName("g_cross_sys_bwrap");
  g_cross_sys[bwrap_init]->Write(); 
	g_cross[bwrap_init]->SetName("g_cross_bwrap");
  g_cross[bwrap_init]->Write(); 
	outFile->Close();
	
  return;

} // end of main func.

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
