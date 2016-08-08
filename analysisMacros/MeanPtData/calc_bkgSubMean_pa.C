#include "../SONGKYO.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>
#include <string>

#include <TROOT.h>
#include "TSystem.h"
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TDirectory.h>
#include <TNtuple.h>
#include <TMath.h>
#include <math.h>
#include <cmath>
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
#include <TLorentzRotation.h>
#include <TCut.h>

#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveStats.h>

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);
void formEtArr(Double_t binmin, Double_t binmax, string* arr);

int calc_bkgSubMean_pa(){

	// # of event range
	const double nSigPbp = 123144.;
	const double nBkgPbpWide = 92145.; // in 2.6-3.5
	const double nBkgPbp = nBkgPbpWide*(0.4/0.9); // in 2.9-3.3
	const double nBkgToSigPbp = nBkgPbp/nSigPbp;
	cout << "nBkgToSigPbp = " << nBkgToSigPbp << endl;
	const double nSigpPb = 84014.3;
	const double nBkgpPbWide =  61795.; // in 2.6-3.5
	const double nBkgpPb =  nBkgpPbWide*(0.4/0.9); // in 2.6-3.5
	const double nBkgToSigpPb = nBkgpPb/nSigpPb;
	cout << "nBkgToSigpPb = " << nBkgToSigpPb << endl;
		
	gROOT->Macro("../Style.C");

  int Nfile = 8;
	// read-in root file
	//TFile *fIn = new TFile("./dir_meanPt_n100_PAS/meanPt_8rap9pt2gev_wBkg_untilPAS.root"); //oldres && nbin=100
	TFile *fIn[Nfile];
  for(int ifile = 0; ifile<Nfile; ifile++)
  {
    fIn[ifile]  = new TFile(Form("./dir_meanPt_n20_9pt/pa_meanPt_9pt_wBkg_%d.root",ifile)); //newres && nbin=20
  }
//	TFile *fIn = new TFile("./dir_meanPt_cndr_n100_8rap9pt/meanPt_8rap9pt_wBkg.root"); //newres && nbin=100

	/////////////////////////////////////////////////////////////	
	////////////// binning	
	Double_t ptArr[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap9pt
	//Double_t ptArr[] = {5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap3pt
	const Int_t nPt = sizeof(ptArr)/sizeof(double)-1;
	cout << "nPt=" << nPt << endl;	
	// in Ycm (will be change to 1st lab and 2nd lab later)
	Double_t yArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // 8rap9pt
	//Double_t yArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46}; // 8rap9pt
	const Int_t nRap = sizeof(yArr)/sizeof(double)-1;
	cout << "nRap=" << nRap << endl;
	string rapstrArr[nRap];
	for (int ir=0; ir<nRap; ir++){
		formRapArr(yArr[ir+1], yArr[ir], &rapstrArr[ir]);
		cout << "rap string arr = " << rapstrArr[ir].c_str() << endl;
	}
	Double_t etArr[] = {0.0, 120.0}; // 8rap9pt
	//Double_t etArr[] = {0.0, 20., 30., 120.0}; // ethf
	const Int_t nEt = sizeof(etArr)/sizeof(double)-1;
	cout << "nEt=" << nEt << endl;
	string etstrArr[nEt];
	for (int in=0; in<nEt; in++){
		 formEtArr(etArr[in], etArr[in+1], &etstrArr[in]);
		 cout << "et string arr = " << etstrArr[in].c_str() << endl;
	}
	
	TH1D *hMeanPt[nEt][nRap][nPt];
	TH1D *hMeanPtBkg[nEt][nRap][nPt];
	TH1D *hMeanPtSub[nEt][nRap][nPt];
	
	double nSigCand[nEt][nRap][nPt]; // Entries in 2.9-3.3
	double nBkgSide[nEt][nRap][nPt]; // Entries in 2.6-2.9 && 3.3-3.5
	double nBkg[nEt][nRap][nPt]; // nBkgSide*(0.4/0.5); // bkg in 2.9-3.3
	double nBkgToSig[nEt][nRap][nPt]; // nBkg/nSig ratio to scale Bkg hist.
	
  double MeanVal[nEt][nRap][nPt];

	TCanvas* cmulti[nEt][nRap];
	TLegend *legUR = new TLegend(0.58,0.68,0.90,0.90,NULL,"brNDC");
	TLegend *legUM = new TLegend(0.30,0.68,0.65,0.90,NULL,"brNDC");
	TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
	TLegend *legBM = new TLegend(0.30,0.20,0.65,0.42,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legUM);
	SetLegendStyle(legUL);
	SetLegendStyle(legBM);

	for (int in=0; in<nEt; in++)
  {
		for (int ir=0; ir<nRap; ir++ ) 
    {
			for (int ipt=0; ipt<nPt; ipt++) 
      {
				hMeanPt[in][ir][ipt] = (TH1D*)fIn[ir]->Get(Form("hMeanPt_%d_0_%d",in,ipt));
				hMeanPtBkg[in][ir][ipt] = (TH1D*)fIn[ir]->Get(Form("hMeanPtBkg_%d_0_%d",in,ipt));
				nSigCand[in][ir][ipt] = hMeanPt[in][ir][ipt]->Integral();
				nBkgSide[in][ir][ipt] = hMeanPtBkg[in][ir][ipt]->Integral();
				std::cout << in<<ir<<ipt<<"th hMeanPt integral = " << nSigCand[in][ir][ipt] << std::endl;
				std::cout << in<<ir<<ipt<<"th hMeanPtBkg integral = " << nBkgSide[in][ir][ipt] << std::endl;
		
      	// first normalized by their integral
        
        nBkgSide[in][ir][ipt] *= 0.8;
        
        hMeanPtBkg[in][ir][ipt] -> Scale(0.8);
        hMeanPt[in][ir][ipt] -> Add(hMeanPtBkg[in][ir][ipt],-1);

       // if(nPt==9) MeanVal[in][ir][ipt] = hMeanPt[in][ir][ipt]->GetMean(1);      

      }
    }
  } 

  if(nPt==3 || nPt==9)
  {
    for(int in=0;in<nEt;in++)
    {
      for(int ir=0;ir<3;ir++)
      {
        for(int ipt=0;ipt<nPt;ipt++)
        {
          hMeanPt[in][ir][ipt]->Add(hMeanPt[in][6-1-ir][ipt],1);
          MeanVal[in][ir][ipt] = hMeanPt[in][ir][ipt]->GetMean(1);  
        }
      }
    }
  }    

  int nRapc = nRap;
  if(nPt==3) nRapc=nRap/2;
  if(nPt==9) nRapc=nRap/2;
  nRapc=3;
  for(int in=0;in<nEt;in++)
  {
   for(int ir=0; ir<nRapc; ir++)
   {
      cout << "{";
    for(int ipt=0;ipt<nPt;ipt++)
    {
      if(ipt!=nPt-1) cout << MeanVal[in][ir][ipt] << ", ";
      else if(ipt==nPt-1) cout << MeanVal[in][ir][ipt];
    }
      if(ir!=nRapc-1) cout << "}," << endl;
      else if(ir==nRapc-1) cout << "}" << endl;
   }
  }
  


/*
		c1[i] = new TCanvas(Form("c1_%s",histName[i].Data()),"",600,600);
		c1[i]->cd();
		if (i==0 || i==1 ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		SetHistStyle(hSigSubPbp[i],2,1);
		SetHistStyle(hPRMCPbp[i],3,0);
		SetHistStyle(hNPMCPbp[i],4,0);
		hPRMCPbp[i]->SetFillColor(kOrange+7);
		hPRMCPbp[i]->SetFillStyle(3003);
		hNPMCPbp[i]->SetFillColor(kGreen+3);
		hNPMCPbp[i]->SetFillStyle(3005);
		if (i==0) hSigSubPbp[i]->SetAxisRange(0.0001,1.,"Y");
		/////else if (i==1) hSigSubPbp[i]->SetAxisRange(0.001,1,"Y");
		else if (i==1) hSigSubPbp[i]->SetAxisRange(0.,30.,"X");
		else if (i==2) hSigSubPbp[i]->SetAxisRange(0.,0.07,"Y");
		else if (i==3) hSigSubPbp[i]->SetAxisRange(0.0,0.045,"Y");
		hSigSubPbp[i]->Draw("pe");
		hPRMCPbp[i]->Draw("hist same");
		hNPMCPbp[i]->Draw("hist same");

		if (i==0) {
			legUL->AddEntry(hSigSubPbp[i],"scaled signal","lp");
			legUL->AddEntry(hPRMCPbp[i],"prompt MC","l");
			legUL->AddEntry(hNPMCPbp[i],"non-prompt MC","l");
			legUL->Draw();
		}
		else if (i==2) {
			legUM->AddEntry(hSigSubPbp[i],"scaled signal","lp");
			legUM->AddEntry(hPRMCPbp[i],"prompt MC","l");
			legUM->AddEntry(hNPMCPbp[i],"non-prompt MC","l");
			legUM->Draw();
		}
		else {
			legUR->AddEntry(hSigSubPbp[i],"scaled signal","lp");
			legUR->AddEntry(hPRMCPbp[i],"prompt MC","l");
			legUR->AddEntry(hNPMCPbp[i],"non-prompt MC","l");
			legUR->Draw();
		}

		c1[i]->SaveAs(Form("VariablesSubtract/variables_%s_%s.pdf",histName[i].Data(),stringA));
		c1[i]->SaveAs(Form("VariablesSubtract/variables_%s_%s.png",histName[i].Data(),stringA));
		//c1[i]->Clear();
		legUR->Clear();
		legUM->Clear();
		legUL->Clear();
		legBM->Clear();*/
/*	
	//Save as a root file
	TFile* outFile = new TFile("VariablesSubtract/variables.root","RECREATE");
	outFile->cd();
	for (int i=0; i<nHist; i++) {
		hSigSubPbp[i]->Write();
		hPRMCPbp[i]->Write();
		hNPMCPbp[i]->Write();
	}	
	outFile->Close();
*/
	return 0;	

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

void formRapArr(Double_t binmin, Double_t binmax, string* arr) {
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

void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr) {
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

void formPtArr(Double_t binmin, Double_t binmax, string* arr) {
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

void formEtArr(Double_t binmin, Double_t binmax, string* arr) {
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

