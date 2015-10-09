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

int rootAna_variables(char *stringA = "20140301"){

	// # of event range
//	const int nEntry = 5000;
		
	gROOT->Macro("./JpsiStyle.C");
	//gStyle->SetCanvasDefW(800);

	TFile *fDataPbp1; // 210498<= runNb <= 210658 should be rejected
	TFile *fDataPbp2; // only 210498<= runNb <= 210658 will be used : 1st 7 run
	TFile *fDatapPb;
	TFile *fPRMCPbp;
	TFile *fPRMCpPb;
	TFile *fNPMCPbp;
	TFile *fNPMCpPb;
	
	// read-in root file
	fDataPbp1 = new TFile("/home/songkyo/kyo/pPbDataSample/merged_pPbData_1st_ntuple_PromptReco-v1_GR_P_V43D_pileupRej_noMuID_tot.root");
	fDataPbp2 = new TFile("/home/songkyo/kyo/pPbDataSample/merged_pPbData_1st_ntuple_ReprocessedReco-v1_GR_P_V43F_pileupRej_muID_tot.root");
	fDatapPb = new TFile("/home/songkyo/kyo/pPbDataSample/merged_pPbData_2nd_ntuple_PromptReco-v1_GR_P_V43D_pileupRej_muID_tot.root");
	fPRMCPbp = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_1Mevt.root");
	fPRMCpPb = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_PromptJpsi_PYTHIAboosted_2nd_STARTHI53_V27_1Mevt.root");
	fNPMCPbp = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_B2Jpsi_PYTHIAboosted_1st_STARTHI53_V27_1Mevt.root");
	fNPMCpPb = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_B2Jpsi_PYTHIAboosted_2nd_STARTHI53_V27_1Mevt.root");

	TTree *treeDataPbp1 = (TTree*)fDataPbp1->Get("myTree");
	TTree *treeDataPbp2 = (TTree*)fDataPbp2->Get("myTree");
	TTree *treeDatapPb = (TTree*)fDatapPb->Get("myTree");
	TTree *treePRMCPbp = (TTree*)fPRMCPbp->Get("myTree");
	TTree *treePRMCpPb = (TTree*)fPRMCpPb->Get("myTree");
	TTree *treeNPMCPbp = (TTree*)fNPMCPbp->Get("myTree");
	TTree *treeNPMCpPb = (TTree*)fNPMCpPb->Get("myTree");

	// cut definitions
	TCut trigCut = "((Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )";
	TCut recoCut = "Reco_QQ_size>=1 && Reco_QQ_sign==0";	
	TCut genCut = "Gen_QQ_size>=1";	
	TCut massRange01 = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
	TCut massRange02 = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
	TCut rapRange = "Reco_QQ_4mom.Rapidity() > -2.4 && Reco_QQ_4mom.Rapidity() < 2.4";
	TCut ptRange = "Reco_QQ_4mom.Pt() > 0.0 && Reco_QQ_4mom.Pt() < 30.0";
	TCut accRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta())<2.4) && ( (TMath::Abs(Reco_QQ_mupl_4mom.Eta())<1.3 && Reco_QQ_mupl_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.2 && sqrt(Reco_QQ_mupl_4mom.Px()*Reco_QQ_mupl_4mom.Px()+ Reco_QQ_mupl_4mom.Py()*Reco_QQ_mupl_4mom.Py() + Reco_QQ_mupl_4mom.Pz()*Reco_QQ_mupl_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt()>=0.8) )";
	TCut accRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta())<2.4) && ( (TMath::Abs(Reco_QQ_mumi_4mom.Eta())<1.3 && Reco_QQ_mumi_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.2 && sqrt(Reco_QQ_mumi_4mom.Px()*Reco_QQ_mumi_4mom.Px()+ Reco_QQ_mumi_4mom.Py()*Reco_QQ_mumi_4mom.Py() + Reco_QQ_mumi_4mom.Pz()*Reco_QQ_mumi_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt()>=0.8) )";
	TCut accReco = accRecoPlus && accRecoMinus;
	TCut accGenPlus = "(TMath::Abs(Gen_QQ_mupl_4mom.Eta())<2.4) && ( (TMath::Abs(Gen_QQ_mupl_4mom.Eta())<1.3 && Gen_QQ_mupl_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Gen_QQ_mupl_4mom.Eta()) && TMath::Abs(Gen_QQ_mupl_4mom.Eta()) < 2.2 && sqrt(Gen_QQ_mupl_4mom.Px()*Gen_QQ_mupl_4mom.Px()+ Gen_QQ_mupl_4mom.Py()*Gen_QQ_mupl_4mom.Py() + Gen_QQ_mupl_4mom.Pz()*Gen_QQ_mupl_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Gen_QQ_mupl_4mom.Eta()) && Gen_QQ_mupl_4mom.Pt()>=0.8) )";
	TCut accGenMinus = "(TMath::Abs(Gen_QQ_mumi_4mom.Eta())<2.4) && ( (TMath::Abs(Gen_QQ_mumi_4mom.Eta())<1.3 && Gen_QQ_mumi_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Gen_QQ_mumi_4mom.Eta()) && TMath::Abs(Gen_QQ_mumi_4mom.Eta()) < 2.2 && sqrt(Gen_QQ_mumi_4mom.Px()*Gen_QQ_mumi_4mom.Px()+ Gen_QQ_mumi_4mom.Py()*Gen_QQ_mumi_4mom.Py() + Gen_QQ_mumi_4mom.Pz()*Gen_QQ_mumi_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Gen_QQ_mumi_4mom.Eta()) && Gen_QQ_mumi_4mom.Pt()>=0.8) )";
	TCut accGen = accGenPlus && accGenMinus;
	TCut runCut = "runNb >=210498 && runNb <= 210658"; //1st reprocessedReco
	
	TCut totalCut = trigCut && recoCut && rapRange && ptRange && accReco;
	TCut totalCutMC = trigCut && recoCut && rapRange &&  ptRange && accReco && accGen && genCut;

	// define 1D hist
	const int nbin[] = {80, 80, 80, 50};
	//const int nbin[] = {60, 60, 40, 30};
	const double minbin[] = {2.6, 0.0, -3.0, -3.14};
	const double maxbin[] = {3.5, 35.0, 2.0, 3.14};
	TString histName[] = {"M", "Pt", "Rapidity","Phi"};
	TString histTitle[] = {"mass (GeV/c^{2})", "p_{T} (GeV/c)", "y_{CM}", "#phi (rad)"};
	
	const int nHist = sizeof(nbin)/sizeof(int);
	cout << "nHist = "<<nHist<<endl;

	TH1D *hDataPbp[nHist];
	TH1D *hSigPbp[nHist];
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
//		if (i!=1) continue;
		hDataPbp[i] = new TH1D(Form("hDataPbp_%s",histName[i].Data()),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		hSigPbp[i] = new TH1D(Form("hSigPbp_%s",histName[i].Data()),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		hBkgPbp[i] = new TH1D(Form("hBkgPbp_%s",histName[i].Data()),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		hPRMCPbp[i] = new TH1D(Form("hPRMCPbp_%s",histName[i].Data()),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		hNPMCPbp[i] = new TH1D(Form("hNPMCPbp_%s",histName[i].Data()),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		hDataPbp[i]->Sumw2();
		hSigPbp[i]->Sumw2();
		hBkgPbp[i]->Sumw2();
		hPRMCPbp[i]->Sumw2();
		hNPMCPbp[i]->Sumw2();
		std::cout << i <<"th hist name = " << histName[i].Data() << std::endl;
		if (i==0) { // mass :: 2.6 - 3.5
			treeDataPbp1->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hDataPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treeDataPbp1->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hSigPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treeDataPbp1->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hBkgPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treeDataPbp2->Draw(Form("Reco_QQ_4mom.%s()>>+%s",histName[i].Data(),hDataPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treeDataPbp2->Draw(Form("Reco_QQ_4mom.%s()>>+%s",histName[i].Data(),hSigPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treeDataPbp2->Draw(Form("Reco_QQ_4mom.%s()>>+%s",histName[i].Data(),hBkgPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treePRMCPbp->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hPRMCPbp[i]->GetName()),totalCutMC && massRange01 ,"");
			treeNPMCPbp->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hNPMCPbp[i]->GetName()),totalCutMC && massRange01,"");
		}		
		else if (i==2) { // rapidity :: convert y_lab^1st to y_CM
			treeDataPbp1->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hDataPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treeDataPbp1->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hSigPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange02,"");
			treeDataPbp1->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hBkgPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01 && (!massRange02),"");
			treeDataPbp2->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>+%s",histName[i].Data(),hDataPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treeDataPbp2->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>+%s",histName[i].Data(),hSigPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange02,"");
			treeDataPbp2->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>+%s",histName[i].Data(),hBkgPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01 && (!massRange02),"");
			treePRMCPbp->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hPRMCPbp[i]->GetName()),totalCutMC && massRange02,"");
			treeNPMCPbp->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hNPMCPbp[i]->GetName()),totalCutMC && massRange02,"");
		}
		else {
			treeDataPbp1->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hDataPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treeDataPbp1->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hSigPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange02,"");
			treeDataPbp1->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hBkgPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01 && (!massRange02),"");
			treeDataPbp2->Draw(Form("Reco_QQ_4mom.%s()>>+%s",histName[i].Data(),hDataPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treeDataPbp2->Draw(Form("Reco_QQ_4mom.%s()>>+%s",histName[i].Data(),hSigPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange02,"");
			treeDataPbp2->Draw(Form("Reco_QQ_4mom.%s()>>+%s",histName[i].Data(),hBkgPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01 && (!massRange02),"");
			treePRMCPbp->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hPRMCPbp[i]->GetName()),totalCutMC && massRange02,"");
			treeNPMCPbp->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[i].Data(),hNPMCPbp[i]->GetName()),totalCutMC && massRange02,"");
		}
		std::cout << histName[i].Data() <<" : DataPbp integral = " << hDataPbp[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : SigPbp integral = " << hSigPbp[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : BkgPbp integral = " << hBkgPbp[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : PRMC integral = " << hPRMCPbp[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : NPMC integral = " << hNPMCPbp[i]->Integral() << std::endl;
//		hDataPbp[i]->Scale(1/hDataPbp[i]->Integral());
//		hSigPbp[i]->Scale(1/hSigPbp[i]->Integral());
//		hBkgPbp[i]->Scale(1/hBkgPbp[i]->Integral());
//		hPRMCPbp[i]->Scale(1/hPRMCPbp[i]->Integral());
//		hNPMCPbp[i]->Scale(1/hNPMCPbp[i]->Integral());
	
		c1[i] = new TCanvas(Form("c1_%s",histName[i].Data()),"",600,600);
		c1[i]->cd();
		if (i==0 || i==1 ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		SetHistStyle(hDataPbp[i],1,0);
		SetHistStyle(hSigPbp[i],2,1);
		SetHistStyle(hBkgPbp[i],0,3);
		SetHistStyle(hPRMCPbp[i],3,0);
		SetHistStyle(hNPMCPbp[i],4,0);
		hPRMCPbp[i]->SetFillColor(kOrange+7);
		hPRMCPbp[i]->SetFillStyle(3003);
		hNPMCPbp[i]->SetFillColor(kGreen+3);
		hNPMCPbp[i]->SetFillStyle(3005);
		if (i==0) hDataPbp[i]->SetAxisRange(0.0001,1.,"Y");
		/////else if (i==1) hDataPbp[i]->SetAxisRange(0.001,1,"Y");
		else if (i==2) hDataPbp[i]->SetAxisRange(0.,0.1,"Y"); //00300
		//else if (i==2) hDataPbp[i]->SetAxisRange(0.,0.07,"Y"); //65300
		else if (i==3) hDataPbp[i]->SetAxisRange(0.0,0.06,"Y");
		hDataPbp[i]->Draw("pe");
		if (i!=0) hSigPbp[i]->Draw("pe same");
		if (i!=0) hBkgPbp[i]->Draw("pe same");
		hPRMCPbp[i]->Draw("hist same");
		hNPMCPbp[i]->Draw("hist same");

		if (i==0) {
			legUL->AddEntry(hDataPbp[i],"Data","lp");
			legUL->AddEntry(hPRMCPbp[i],"prompt MC","l");
			legUL->AddEntry(hNPMCPbp[i],"non-prompt MC","l");
			legUL->Draw();
		}
		else if (i==2) {
			legUM->AddEntry(hDataPbp[i],"Data","lp");
			legUM->AddEntry(hSigPbp[i],"Signal","lp");
			legUM->AddEntry(hBkgPbp[i],"Background","lp");
			legUM->AddEntry(hPRMCPbp[i],"prompt MC","l");
			legUM->AddEntry(hNPMCPbp[i],"non-prompt MC","l");
			legUM->Draw();
		}
		else {
			legUR->AddEntry(hDataPbp[i],"Data","lp");
			legUR->AddEntry(hSigPbp[i],"Signal","lp");
			legUR->AddEntry(hBkgPbp[i],"Background","lp");
			legUR->AddEntry(hPRMCPbp[i],"prompt MC","l");
			legUR->AddEntry(hNPMCPbp[i],"non-prompt MC","l");
			legUR->Draw();
		}

		c1[i]->SaveAs(Form("Variables/variables_%s_%s.pdf",histName[i].Data(),stringA));
		c1[i]->SaveAs(Form("Variables/variables_%s_%s.png",histName[i].Data(),stringA));
		//c1[i]->Clear();
		legUR->Clear();
		legUM->Clear();
		legUL->Clear();
		legBM->Clear();
	}

	//Save as a root file
	TFile* outFile = new TFile("Variables/variables.root","RECREATE");
	outFile->cd();
	for (int i=0; i<nHist; i++) {
		hDataPbp[i]->Write();
		hSigPbp[i]->Write();
		hBkgPbp[i]->Write();
		hPRMCPbp[i]->Write();
		hNPMCPbp[i]->Write();
	}	
	outFile->Close();

	return 0;	

}


