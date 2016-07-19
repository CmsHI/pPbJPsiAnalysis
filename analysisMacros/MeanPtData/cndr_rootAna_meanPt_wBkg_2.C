#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);
void formEtArr(Double_t binmin, Double_t binmax, TString* arr);

int cndr_rootAna_meanPt_wBkg_2(TString szPA="pa", bool isSmall=true){
//int rootAna_muIDvariables(TString szPA = "pa", bool isScaled = true, bool isSmall =true){

	//const int nbin = 100; //arbitrary binning for each pT hist.
	const int nbin = 20; //arbitrary binning for each pT hist.
	const int nEntry = 10000;
		
	gROOT->Macro("../Style.C");

  //// read-in root file
  TFile *fData;
  TFile *fPRMC;
  TTree *treeData;
  TTree *treePRMC;
  if (strcmp(szPA,"pp")==0) {
    fData = new TFile("/storage/OniaTree/Onia5TeV/ppData/OniaTree_DoubleMu_Run2015E-PromptReco-v1_Run_262157_262328_noCUT_TRKMU.root");
    fPRMC = new TFile("/storage/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");
    treeData = (TTree*)fData->Get("hionia/myTree");
    treePRMC = (TTree*)fPRMC->Get("hionia/myTree");
  } else if (strcmp(szPA,"pa")==0) {
    fData = new TFile("/home/songkyo/kyo/pPbDataSample/Data/RD2013_pa_1st_run_210676-211256_GR_P_V43D_nocut.root");
    fPRMC = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCJPsiWithFSR_pa_1st_run_STARTHI53_V27_ext1_nocut.root");
    treeData = (TTree*)fData->Get("myTree");
    treePRMC = (TTree*)fPRMC->Get("myTree");
  }
  else { cout << "WARNING :: Select among szPA = \"pp\" or \"pa\" "; return 0; }

	/////////////////////////////////////////////////////////////	
	////////////// binning
	Double_t ptArr[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap9pt
	const Int_t nPt = sizeof(ptArr)/sizeof(double)-1;
	cout << "nPt=" << nPt << endl;
	//// in Ycm (will be change to 1st lab and 2nd lab later)
	const int nRap = 1;
	const int nRapTmp = nRap+1;
	Double_t yArr[nRapTmp] = {0.9, 0.0};
	//Double_t yArr[] = {2.4, 1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4}; // 8rap9pt pp
	//Double_t yArr[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87}; // 8rap9pt pA
	//const Int_t nRap = sizeof(yArr)/sizeof(double)-1;
	cout << "nRap=" << nRap << endl;

	TString rapstrArr[nRap];
	for (int ir=0; ir<nRap; ir++){
		formRapArr(yArr[ir+1], yArr[ir], &rapstrArr[ir]);
		cout << "rap string arr = " << rapstrArr[ir].Data() << endl;
	}
	Double_t etArr[] = {0.0, 120.0}; // 8rap9pt
	//Double_t etArr[] = {0.0, 20., 30., 120.0}; // ethf
	const Int_t nEt = sizeof(etArr)/sizeof(double)-1;
	cout << "nEt=" << nEt << endl;
  TString etstrArr[nEt];
	for (int in=0; in<nEt; in++){
		 formEtArr(etArr[in], etArr[in+1], &etstrArr[in]);
		 cout << "et string arr = " << etstrArr[in].Data() << endl;
	}

	/////////////////////////////////////////////////////////////	
  //// cut definitions
  TCut trigCut = "((Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )";
  TCut recoPairCut = "Reco_QQ_sign==0 && Reco_QQ_VtxProb > 0.01";  
  TCut massNarrow = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
  TCut massWide = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
  TCut ctauCut = "-1.5 < Reco_QQ_ctau && Reco_QQ_ctau < 3.0 && 0 < Reco_QQ_ctauErr && Reco_QQ_ctauErr < 1";
  
  //// new cut
  TCut accNewRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 1.2 && Reco_QQ_mupl_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.1 && Reco_QQ_mupl_4mom.Pt() >= 3.93-1.11*TMath::Abs(Reco_QQ_mupl_4mom.Eta())) || (2.1 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt() >= 1.3)))";
  TCut accNewRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 1.2 && Reco_QQ_mumi_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.1 && Reco_QQ_mumi_4mom.Pt() >= 3.93-1.11*TMath::Abs(Reco_QQ_mumi_4mom.Eta())) || (2.1 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt() >= 1.3)))";

  //// muID
  TCut muIDPlus;
  TCut muIDMinus;
  if(strcmp(szPA,"pp")==0) {
    muIDPlus = "Reco_QQ_mupl_highPurity && Reco_QQ_mupl_TrkMuArb && Reco_QQ_mupl_TMOneStaTight && Reco_QQ_mupl_nTrkWMea > 5 && Reco_QQ_mupl_nPixWMea > 0 &&  TMath::Abs(Reco_QQ_mupl_dxy) < 0.3 && TMath::Abs(Reco_QQ_mupl_dz) < 20.";
    muIDMinus = "Reco_QQ_mumi_highPurity && Reco_QQ_mumi_TrkMuArb && Reco_QQ_mumi_TMOneStaTight && Reco_QQ_mumi_nTrkWMea > 5 && Reco_QQ_mumi_nPixWMea > 0 && TMath::Abs(Reco_QQ_mumi_dxy) < 0.3 && TMath::Abs(Reco_QQ_mumi_dz) < 20.";
  }
  else if (strcmp(szPA,"pa")==0) {
    muIDPlus = "Reco_QQ_mupl_isHighPurity && Reco_QQ_mupl_TrkMuArb && Reco_QQ_mupl_TMOneStaTight && Reco_QQ_mupl_nTrkWMea > 5 && Reco_QQ_mupl_nPixWMea > 0 &&  TMath::Abs(Reco_QQ_mupl_dxy) < 0.3 && TMath::Abs(Reco_QQ_mupl_dz) < 20.";
    muIDMinus = "Reco_QQ_mumi_isHighPurity && Reco_QQ_mumi_TrkMuArb && Reco_QQ_mumi_TMOneStaTight && Reco_QQ_mumi_nTrkWMea > 5 && Reco_QQ_mumi_nPixWMea > 0 && TMath::Abs(Reco_QQ_mumi_dxy) < 0.3 && TMath::Abs(Reco_QQ_mumi_dz) < 20.";
  }
  //TCut totalCut = recoPairCut && rapRange && ptRange && accNewRecoPlus && accNewRecoMinus;
  //TCut totalCutMC = recoPairCut && rapRange && ptRange && accNewRecoPlus && accNewRecoMinus;
  
	TCut ptRange[nEt][nRap][nPt];
	TCut rapRange[nEt][nRap][nPt];
	TCut totalCut[nEt][nRap][nPt];
	TCut totalCutBkg[nEt][nRap][nPt];

	//// forming TCut for diffential binning	
	for (int in=0; in<nEt; in++){
		for (int ir=0; ir<nRap; ir++ ) {
			for (int ipt=0; ipt<nPt; ipt++) {
				ptRange[in][ir][ipt] = Form("Reco_QQ_4mom.Pt()>%.1f && Reco_QQ_4mom.Pt()<%.1f",ptArr[ipt] , ptArr[ipt+1]);
				//// change y_CM to y_lab
				if(strcmp(szPA,"pp")==0) {
          rapRange[in][ir][ipt] = Form("Reco_QQ_4mom.Rapidity()>%.2f && Reco_QQ_4mom.Rapidity()<%.2f",yArr[ir+1] , yArr[ir]);
        } else {  
          rapRange[in][ir][ipt] = Form("-0.47-1.*Reco_QQ_4mom.Rapidity()>%.2f && -0.47-1.*Reco_QQ_4mom.Rapidity()<%.2f",yArr[ir+1] , yArr[ir]);
				  //rapRangepPb[in][ir][ipt] = Form("-0.47+Reco_QQ_4mom.Rapidity()>%.2f && -0.47+Reco_QQ_4mom.Rapidity()<%.2f",yArr[ir+1] , yArr[ir]);
        } 
				cout << "ptRange = " <<ptRange[in][ir][ipt] << endl;
				cout << "rapRange = " <<rapRange[in][ir][ipt] << endl;
				totalCut[in][ir][ipt] = trigCut && recoPairCut && massNarrow && ctauCut && accNewRecoPlus && accNewRecoMinus && muIDPlus && muIDMinus && rapRange[in][ir][ipt] && ptRange[in][ir][ipt];
				totalCutBkg[in][ir][ipt] = trigCut && recoPairCut && massWide && (!massNarrow) && ctauCut && accNewRecoPlus && accNewRecoMinus && muIDPlus && muIDMinus && rapRange[in][ir][ipt] && ptRange[in][ir][ipt];
			}
		}
	}
	/////////////////////////////////////////////////////////////	
	/////////////////////////////////////////////////////////////	
	//// define 1D hist
	TH1D *hMeanPt[nEt][nRap][nPt];
	TH1D *hMeanPtBkg[nEt][nRap][nPt];
	double meanVal[nEt][nRap][nPt];
	double maxVal[nEt][nRap][nPt];
	double meanValBkg[nEt][nRap][nPt];
	double maxValBkg[nEt][nRap][nPt];

	TCanvas* cmulti[nEt][nRap];
	TLegend *legUR = new TLegend(0.58,0.68,0.90,0.90,NULL,"brNDC");
	TLegend *legUM = new TLegend(0.30,0.68,0.65,0.90,NULL,"brNDC");
	TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
	TLegend *legBM = new TLegend(0.30,0.20,0.65,0.42,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legUM);
	SetLegendStyle(legUL);
	SetLegendStyle(legBM);
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	for (int in=0; in<nEt; in++){
		for (int ir=0; ir<nRap; ir++ ) {
	 		cmulti[in][ir]= new TCanvas(Form("cmulti_%d_%d",in,ir),Form("%s",rapstrArr[ir].Data()),1500,600);
			cmulti[in][ir]->Divide(5,2);
			for (int ipt=0; ipt<nPt; ipt++) {
				hMeanPt[in][ir][ipt] = new TH1D(Form("hMeanPt_%d_%d_%d",in, ir, ipt),";p_{T} (GeV/c);events", nbin, ptArr[ipt], ptArr[ipt+1]);
				hMeanPt[in][ir][ipt] ->Sumw2();
				SetHistStyle(hMeanPt[in][ir][ipt],4,0);
				hMeanPtBkg[in][ir][ipt] = new TH1D(Form("hMeanPtBkg_%d_%d_%d",in, ir, ipt),";p_{T} (GeV/c);events", nbin, ptArr[ipt], ptArr[ipt+1]);
				hMeanPtBkg[in][ir][ipt] ->Sumw2();
				SetHistStyle(hMeanPtBkg[in][ir][ipt],3,10);
				cmulti[in][ir]->cd(ipt+1);
				gPad->SetLogy(1);
        if (isSmall) {
				  treeData->Draw(Form("Reco_QQ_4mom.Pt()>>%s",hMeanPt[in][ir][ipt]->GetName()),totalCut[in][ir][ipt],"",nEntry);
				  treeData->Draw(Form("Reco_QQ_4mom.Pt()>>%s",hMeanPtBkg[in][ir][ipt]->GetName()),totalCutBkg[in][ir][ipt],"pe same",nEntry);
			  } else {
				  treeData->Draw(Form("Reco_QQ_4mom.Pt()>>%s",hMeanPt[in][ir][ipt]->GetName()),totalCut[in][ir][ipt],"");
				  treeData->Draw(Form("Reco_QQ_4mom.Pt()>>%s",hMeanPtBkg[in][ir][ipt]->GetName()),totalCutBkg[in][ir][ipt],"pe same");
				}
				meanVal[in][ir][ipt] =  hMeanPt[in][ir][ipt]->GetMean(1);
				maxVal[in][ir][ipt] = 50*hMeanPt[in][ir][ipt]->GetMaximum();
				meanValBkg[in][ir][ipt] =  hMeanPtBkg[in][ir][ipt]->GetMean(1);
				maxValBkg[in][ir][ipt] = 50*hMeanPtBkg[in][ir][ipt]->GetMaximum();
				hMeanPt[in][ir][ipt]->GetYaxis()->SetRangeUser(0.1,maxVal[in][ir][ipt]);
				hMeanPtBkg[in][ir][ipt]->GetYaxis()->SetRangeUser(0.1,maxValBkg[in][ir][ipt]);
				std::cout << hMeanPt[in][ir][ipt]->GetName() <<" : entries = " << hMeanPt[in][ir][ipt]->GetEntries() << std::endl;
				cout << " Cand <p_{T}> = " << meanVal[in][ir][ipt] << endl;			
				cout << " Bkg <p_{T}> = " << meanValBkg[in][ir][ipt] << endl;			
				latex->DrawLatex(0.54, 0.87, rapstrArr[ir].Data());
				latex->DrawLatex(0.54, 0.80, Form("Cand <p_{T}> = %.2f (GeV/c)",meanVal[in][ir][ipt]));
				latex->DrawLatex(0.54, 0.72, Form("Bkg <p_{T}> = %.2f (GeV/c)",meanValBkg[in][ir][ipt]));
				//c1->Update();
				//c1->SaveAs(Form("%s.png",hMeanPt[in][ir][ipt]->GetName()));
				//c1->Clear();
			}
			cmulti[in][ir]->Update();
			cmulti[in][ir]->SaveAs(Form("dir_meanPt_n%d_%dpt/meanPt_%dpt_%d_%d_wBkg_2.pdf",nbin,nPt,nPt,in,ir));
			//cmulti[in][ir]->Clear();
		}
	}
	
	/// Save as a root file
	TFile *outFile = new TFile(Form("dir_meanPt_n%d_%dpt/meanPt_%dpt_wBkg_2.root",nbin,nPt,nPt),"RECREATE");
	outFile->cd();
	for (int in=0; in<nEt; in++){
		for (int ir=0; ir<nRap; ir++ ) {
			for (int ipt=0; ipt<nPt; ipt++) {
				hMeanPt[in][ir][ipt]->Write();
				hMeanPtBkg[in][ir][ipt]->Write();
			}
		}
	}
	outFile->Close();

	return 0;	

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

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

void formEtArr(Double_t binmin, Double_t binmax, TString* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < E_{T}^{HF} < %.0f GeV", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < E_{T}^{HF} < %.0f GeV", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < E_{T}^{HF} < %.1f GeV", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
	}
}

