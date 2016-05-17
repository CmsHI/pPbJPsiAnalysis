#include "KYOcommonOpt.h"

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

int draw_figure_pt3gev(char *stringA = "20140301"){

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
		
	gROOT->Macro("./JpsiStyle.C");

	// read-in root file
	TFile *fIn = new TFile(" Variables/variables.root");

	const int nbin[] = {80, 80, 80, 50};
	TString histName[] = {"M", "Pt", "Rapidity","Phi"};
	TString histTitle[] = {"mass (GeV/c^{2})", "p_{T} (GeV/c)", "y_{CM}", "#phi (rad)"};
	const int nHist = sizeof(nbin)/sizeof(int);
	cout << "nHist = "<<nHist<<endl;
	
	TH1D *hSigPbp[nHist];
	TH1D *hSigSubPbp[nHist];
	TH1D *hBkgPbp[nHist];
	TH1D *hPRMCPbp[nHist];
	TH1D *hNPMCPbp[nHist];
	
	TCanvas* c1[nHist];
	TLegend *legUR = new TLegend(0.58,0.68,0.90,0.90,NULL,"brNDC");
	TLegend *legUM = new TLegend(0.30,0.68,0.65,0.90,NULL,"brNDC");
	TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
	TLegend *legBM = new TLegend(0.30,0.20,0.65,0.42,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legUM);
	SetLegendStyle(legUL);
	SetLegendStyle(legBM);

	for (int i=0; i<nHist; i++) {
		//if (i==0) continue;
		if (i!=1) continue;
		hSigPbp[i] = (TH1D*)fIn->Get(Form("hSigPbp_%s",histName[i].Data()));
		hBkgPbp[i] = (TH1D*)fIn->Get(Form("hBkgPbp_%s",histName[i].Data()));
		hPRMCPbp[i] = (TH1D*)fIn->Get(Form("hPRMCPbp_%s",histName[i].Data()));
		hNPMCPbp[i] = (TH1D*)fIn->Get(Form("hNPMCPbp_%s",histName[i].Data()));
		std::cout << i <<"th hist name = " << histName[i].Data() << std::endl;
		std::cout << histName[i].Data() <<" : SigPbp integral = " << hSigPbp[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : BkgPbp integral = " << hBkgPbp[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : PRMC integral = " << hPRMCPbp[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : NPMC integral = " << hNPMCPbp[i]->Integral() << std::endl;

		////// background subtraction
		hSigSubPbp[i] = (TH1D*)hSigPbp[i]->Clone(Form("hSigSubPbp_%s",histName[i].Data()));

		// first normalized by their integral
		hSigPbp[i]->Scale(1/hSigPbp[i]->Integral());
		hSigSubPbp[i]->Scale(1/hSigSubPbp[i]->Integral());
		hBkgPbp[i]->Scale(1/hBkgPbp[i]->Integral());

		// Bkg normalized to nBkgToSigPbp and subtract from the signal
		hBkgPbp[i] ->Scale(nBkgToSigPbp);
		hSigSubPbp[i]->Add(hBkgPbp[i],-1);
		
		// pt 3 GeV cut before normalization
		hSigSubPbp[i]->GetXaxis()->SetRangeUser(3.0,30.0);
		hPRMCPbp[i]->GetXaxis()->SetRangeUser(3.0,30.0);
		hNPMCPbp[i]->GetXaxis()->SetRangeUser(3.0,30.0);
		
		//normalize them again by integral
		hSigPbp[i]->Scale(1/hSigPbp[i]->Integral());
		hSigSubPbp[i]->Scale(1/hSigSubPbp[i]->Integral());
		hBkgPbp[i]->Scale(1/hBkgPbp[i]->Integral());
		hPRMCPbp[i]->Scale(1/hPRMCPbp[i]->Integral());
		hNPMCPbp[i]->Scale(1/hNPMCPbp[i]->Integral());
		
		SetHistStyle(hSigSubPbp[i],2,10);
		hSigSubPbp[i]->Draw("pe");
		
		SetHistStyle(hPRMCPbp[i],1,0);
		hPRMCPbp[i]->Draw("hist same");
		SetHistStyle(hNPMCPbp[i],0,3);
		hNPMCPbp[i]->Draw("hist same");

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
		//else if (i==1) hSigSubPbp[i]->SetAxisRange(0.,30.,"X");
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

		c1[i]->SaveAs(Form("VariablesSubtract/variables_pt3gev_%s_%s.pdf",histName[i].Data(),stringA));
		c1[i]->SaveAs(Form("VariablesSubtract/variables_pt3gev_%s_%s.png",histName[i].Data(),stringA));
		

		gPad->SetLogy(0);
		c1[i]->SaveAs(Form("VariablesSubtract/variables_log0_pt3gev_%s_%s.pdf",histName[i].Data(),stringA));
		c1[i]->SaveAs(Form("VariablesSubtract/variables_log0_pt3gev_%s_%s.png",histName[i].Data(),stringA));
		//c1[i]->Clear();
		legUR->Clear();
		legUM->Clear();
		legUL->Clear();
		legBM->Clear();
	}
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


