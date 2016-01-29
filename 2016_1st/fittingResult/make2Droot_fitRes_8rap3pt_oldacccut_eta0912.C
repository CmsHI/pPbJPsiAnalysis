#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>

#include <TROOT.h>
#include "TSystem.h"
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TMath.h>
#include <math.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "TRandom3.h"
#include "TClonesArray.h"
#include <TAxis.h>
#include <cmath>
#include <TLorentzRotation.h>

#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveStats.h>

//const double shiftvar = -0.47; // conversion constant y=0(collision)==y=-0.47(LAB frame)  
void formRapStr(Double_t min, Double_t max, string* arr);
void formStr(Double_t min, Double_t max, string* arr);

/////// main func. ///////

int make2Droot_fitRes_8rap3pt_oldacccut_eta0912(char *dirName = "fitRes_8rap3pt_oldacccut_eta0912", bool is1st = false){

  using namespace std;
	
	char* runName;
	if(is1st) runName = "Pbp";
	else runName = "pPb";

	// read in fit_ctauErrorRange file
	std::ifstream fctau(Form("./%s/summary_%s/fit_ctauErrorRange",dirName,runName),std::ios::in);
	if(!fctau.is_open()) { cout << "Warning : can NOT open the fit_ctauErrorRange file!"<<endl; }
	// read in fit_table file
	std::ifstream ftable(Form("./%s/summary_%s/fit_table",dirName,runName),std::ios::in);
	if(!ftable.is_open()) { cout << "Warning : can NOT open the fit_table file!"<<endl; }

	const char* strName = Form("%s_%s",dirName,runName);
	std::cout << "strName: " << strName << std::endl;

	///////////////////////////////////////////////////
	//////// Definition of binning
	// --- pt Bin
	//Double_t ptBinsArr[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap9pt
	Double_t ptBinsArr[] = {5.0, 6.5, 10.0, 30.0}; // 8rap3pt
	const Int_t nPtBins = sizeof(ptBinsArr)/sizeof(double)-1;
   cout << "nPtBins=" << nPtBins << endl;

	// --- y Bin
  Double_t tmp_yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // KYO! set to 1st run
  //const Int_t tmp_nYBins = sizeof(tmp_yBinsArr)/sizeof(double)-1;
  const Int_t tmp_nYBins = sizeof(tmp_yBinsArr)/sizeof(double);
  cout << "tmp_nYBins=" << tmp_nYBins << endl;
	
	Double_t yBinsArr[tmp_nYBins] = {};
	if (is1st){
		for (Int_t i=0; i<tmp_nYBins; i++) {
			yBinsArr[i] = tmp_yBinsArr[i]; 
			cout <<"yBinsArr["<<i<<"] = " <<yBinsArr[i]<<endl;
		}
	}
	else {
		// change 1st run to 2nd run
		for (Int_t i=0; i<tmp_nYBins; i++) {
			yBinsArr[i] = -1*tmp_yBinsArr[tmp_nYBins-1-i]; 
			cout <<"yBinsArr["<<i<<"] = " <<yBinsArr[i]<<endl;
		}
	}
  const Int_t nYBins = sizeof(yBinsArr)/sizeof(double)-1;
  cout << "nYBins=" << nYBins << endl;

	// --- ntrk bin
	Double_t ntrBinsArr[] = {0.0, 350.0};
	const Int_t nNtrBins = sizeof(ntrBinsArr)/sizeof(double)-1;
	cout << "nNtrBins=" << nNtrBins << endl;
	// --- Et bin
	Double_t etBinsArr[] = {0.0, 120.0};
	const Int_t nEtBins = sizeof(etBinsArr)/sizeof(double)-1;
	cout << "nEtBins=" << nEtBins << endl;

	// setting for string bin array for checking ctau error range
	string yrange[nYBins]; //1st run string
	string ptrange[nPtBins];
	string ntrrange[nNtrBins];
	string etrange[nEtBins];

	for (Int_t i=0; i<nYBins; i++){	
		formRapStr(yBinsArr[i], yBinsArr[i+1], &yrange[i]);
		cout << "yrange["<<i<<"] = "<< yrange[i].c_str() << endl;
	}
	for (Int_t i=0; i<nPtBins; i++){	
		formStr(ptBinsArr[i], ptBinsArr[i+1], &ptrange[i]);
		cout << "ptrange["<<i<<"] = "<< ptrange[i].c_str() << endl;
	}
	for (Int_t i=0; i<nNtrBins; i++){	
		formStr(ntrBinsArr[i], ntrBinsArr[i+1], &ntrrange[i]);
		cout << "ntrrange["<<i<<"] = "<< ntrrange[i].c_str() << endl;
	}
	for (Int_t i=0; i<nEtBins; i++){	
		formStr(etBinsArr[i], etBinsArr[i+1], &etrange[i]);
		cout << "etrange["<<i<<"] = "<< etrange[i].c_str() << endl;
	}

	//////////////////////////////////////////////////////
	////// read ctauErr txt
	float ctErrmin[nYBins][nPtBins];
	float ctErrmax[nYBins][nPtBins];
	TH2D *h2D_ctErrmin = new TH2D("h2D_ctErrmin","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_ctErrmax = new TH2D("h2D_ctErrmax","",nYBins,yBinsArr,nPtBins,ptBinsArr);

	string headers;
	getline(fctau, headers); // remove prefix
	getline(fctau, headers); // remove column names
	string rapdum, ptdum, centdum, dphidum, ntrkdum, etdum, errmin, errmax;

	while(!fctau.eof()) {
		fctau >> rapdum >> ptdum >> centdum >> dphidum >> ntrkdum >> etdum >> errmin >> errmax;
		//cout << rapdum << ptdum << centdum << dphidum << ntrkdum << etdum << errmin << errmax << endl;
	  for (Int_t iy=0; iy<nYBins; iy++){
	    for (Int_t ipt=0; ipt<nPtBins; ipt++) {
		  	if (!yrange[iy].compare(rapdum) && !ptrange[ipt].compare(ptdum) ) {
					ctErrmin[iy][ipt] = atof(errmin.c_str());
					ctErrmax[iy][ipt] = atof(errmax.c_str());
				}
	    } 
	  }
	} //end of while file open
	
	// put the values into hist
	int tmpbin=0;
	for (Int_t iy=0; iy<nYBins; iy++){
		for (Int_t ipt=0; ipt<nPtBins; ipt++) {
			tmpbin = h2D_ctErrmin->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);	
			h2D_ctErrmin->SetBinContent(tmpbin, ctErrmin[iy][ipt]);
			h2D_ctErrmax->SetBinContent(tmpbin, ctErrmax[iy][ipt]);
		}
	}

	//////////////////////////////////////////////////////
	////// read fit_table

	float nSig[nYBins][nPtBins];
	float nBkg[nYBins][nPtBins];
	float nPrompt[nYBins][nPtBins];
	float nNonprompt[nYBins][nPtBins];
	float bFraction[nYBins][nPtBins];
	
	float nSigErr[nYBins][nPtBins];
	float nBkgErr[nYBins][nPtBins];
	float nPromptErr[nYBins][nPtBins];
	float nNonpromptErr[nYBins][nPtBins];
	float bFractionErr[nYBins][nPtBins];
	
	TH2D *h2D_nSig = new TH2D("h2D_nSig","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_nBkg = new TH2D("h2D_nBkg","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_nPrompt = new TH2D("h2D_nPrompt","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_nNonprompt = new TH2D("h2D_nNonprompt","",nYBins,yBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_bFraction = new TH2D("h2D_bFraction","",nYBins,yBinsArr,nPtBins,ptBinsArr);

	//string headers;
	getline(ftable, headers); // remove prefix
	getline(ftable, headers); // remove column names
	//string rapdum, ptdum, centdum, dphidum, ntrkdum, etdum;
	string sigdum, sigerrdum, bkgdum, bkgerrdum, prdum, prerrdum, npdum, nperrdum, bfrdum, bfrerrdum;

	while(!ftable.eof()) {
		ftable >> rapdum >> ptdum >> centdum >> dphidum >> ntrkdum >> etdum >> sigdum >> sigerrdum >> bkgdum >> bkgerrdum >> prdum >> prerrdum >> npdum >> nperrdum >> bfrdum >> bfrerrdum;
		//cout <<  rapdum << ptdum << centdum << dphidum << ntrkdum << etdum << sigdum << sigerrdum << bkgdum << bkgerrdum << prdum << prerrdum << npdum << nperrdum << bfrdum << bfrerrdum << endl;
	  for (Int_t iy=0; iy<nYBins; iy++){
	    for (Int_t ipt=0; ipt<nPtBins; ipt++) {
		  	if (!yrange[iy].compare(rapdum) && !ptrange[ipt].compare(ptdum) ) {
					nSig[iy][ipt] = atof(sigdum.c_str());
					nSigErr[iy][ipt] = atof(sigerrdum.c_str());
					nBkg[iy][ipt] = atof(bkgdum.c_str());
					nBkgErr[iy][ipt] = atof(bkgerrdum.c_str());
					nPrompt[iy][ipt] = atof(prdum.c_str());
					nPromptErr[iy][ipt] = atof(prerrdum.c_str());
					nNonprompt[iy][ipt] = atof(npdum.c_str());
					nNonpromptErr[iy][ipt] = atof(nperrdum.c_str());
					bFraction[iy][ipt] = atof(bfrdum.c_str());
					bFractionErr[iy][ipt] = atof(bfrerrdum.c_str());
				}
	    } 
	  }
	} //end of while file open
	
	// put the values into hist
	tmpbin=0;
	for (Int_t iy=0; iy<nYBins; iy++){
		for (Int_t ipt=0; ipt<nPtBins; ipt++) {
			tmpbin = h2D_nSig->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);	
			h2D_nSig->SetBinContent(tmpbin, nSig[iy][ipt]);
			h2D_nSig->SetBinError(tmpbin, nSigErr[iy][ipt]);
			h2D_nBkg->SetBinContent(tmpbin, nBkg[iy][ipt]);
			h2D_nBkg->SetBinError(tmpbin, nBkgErr[iy][ipt]);
			h2D_nPrompt->SetBinContent(tmpbin, nPrompt[iy][ipt]);
			h2D_nPrompt->SetBinError(tmpbin, nPromptErr[iy][ipt]);
			h2D_nNonprompt->SetBinContent(tmpbin, nNonprompt[iy][ipt]);
			h2D_nNonprompt->SetBinError(tmpbin, nNonpromptErr[iy][ipt]);
			h2D_bFraction->SetBinContent(tmpbin, bFraction[iy][ipt]);
			h2D_bFraction->SetBinError(tmpbin, bFractionErr[iy][ipt]);
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Save as a root file

//	TFile *outFile = new TFile(Form("EffCounting_%s_useCtErr_%d_useDataDriven_%d_useZvtxStep1_%d_Step2_%d.root",strName, (int)useCtErrRangeEff ,(int)useDataDrivenEff, (int)useZvtxWeightStep1, (int)useZvtxWeightStep2),"RECREATE");
	TFile *outFile = new TFile(Form("2Dhist_%s.root",strName),"RECREATE");
	std::cout << "strName: " << strName << std::endl;
	
	outFile->cd();
	h2D_ctErrmin->Write();
	h2D_ctErrmax->Write();
	h2D_nSig->Write();
	h2D_nBkg->Write();
	h2D_nPrompt->Write();
	h2D_nNonprompt->Write();
	h2D_bFraction->Write();
	outFile->Close();

	return 0;

} // end of main func

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

void formRapStr(Double_t min, Double_t max, string* arr) { //byHand KYO
	if (min==-2.4 || min==2.4) *arr = Form("%.1f-%.2f",min,max);
	else if (max==-2.4 || max==2.4) *arr = Form("%.2f-%.1f",min,max);
	else *arr = Form("%.2f-%.2f",min,max);
}
void formStr(Double_t min, Double_t max, string* arr) {
	*arr = Form("%.1f-%.1f",min,max);
}


