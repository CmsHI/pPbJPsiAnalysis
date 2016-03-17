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

int rootAna_MCvsMC_noMuID_gen(bool isSmall=false)
{
	// # of event range
	const int nEntry = 20000;
		
	gROOT->Macro("./JpsiStyle.C");

	TFile *fPrivate;
	TFile *fOfficial;
	
	// read-in root file
	fPrivate = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/tot_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_noMuID_sglTrig_genMatch_20150205.root");
	fOfficial = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/tot_JPsiWithFSR_pa_1st_run_STARTHI53_V27-v1_noAccCut_noMuID_sglTrig_genMatch-v1.root");

	TTree *tree01 = (TTree*)fPrivate->Get("myTree");
	TTree *tree02 = (TTree*)fOfficial->Get("myTree");

	// cut definitions
	TCut trigCut = "((Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )";
	TCut recoCut = "Reco_QQ_size>=1 && Reco_QQ_sign==0";	
	TCut genCut = "Gen_QQ_size>=1";	
	TCut massWide = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
	TCut massNarrow = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
	TCut rapRange = "Reco_QQ_4mom.Rapidity() > -2.4 && Reco_QQ_4mom.Rapidity() < 2.4";
	TCut ptRange = "Reco_QQ_4mom.Pt() > 0.0 && Reco_QQ_4mom.Pt() < 30.0";
	TCut accRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta())<2.4) && ( (TMath::Abs(Reco_QQ_mupl_4mom.Eta())<1.3 && Reco_QQ_mupl_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.2 && sqrt(Reco_QQ_mupl_4mom.Px()*Reco_QQ_mupl_4mom.Px()+ Reco_QQ_mupl_4mom.Py()*Reco_QQ_mupl_4mom.Py() + Reco_QQ_mupl_4mom.Pz()*Reco_QQ_mupl_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt()>=0.8) )";
	TCut accRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta())<2.4) && ( (TMath::Abs(Reco_QQ_mumi_4mom.Eta())<1.3 && Reco_QQ_mumi_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.2 && sqrt(Reco_QQ_mumi_4mom.Px()*Reco_QQ_mumi_4mom.Px()+ Reco_QQ_mumi_4mom.Py()*Reco_QQ_mumi_4mom.Py() + Reco_QQ_mumi_4mom.Pz()*Reco_QQ_mumi_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt()>=0.8) )";
	TCut accReco = accRecoPlus && accRecoMinus;
	TCut accGenPlus = "(TMath::Abs(Gen_QQ_mupl_4mom.Eta())<2.4) && ( (TMath::Abs(Gen_QQ_mupl_4mom.Eta())<1.3 && Gen_QQ_mupl_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Gen_QQ_mupl_4mom.Eta()) && TMath::Abs(Gen_QQ_mupl_4mom.Eta()) < 2.2 && sqrt(Gen_QQ_mupl_4mom.Px()*Gen_QQ_mupl_4mom.Px()+ Gen_QQ_mupl_4mom.Py()*Gen_QQ_mupl_4mom.Py() + Gen_QQ_mupl_4mom.Pz()*Gen_QQ_mupl_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Gen_QQ_mupl_4mom.Eta()) && Gen_QQ_mupl_4mom.Pt()>=0.8) )";
	TCut accGenMinus = "(TMath::Abs(Gen_QQ_mumi_4mom.Eta())<2.4) && ( (TMath::Abs(Gen_QQ_mumi_4mom.Eta())<1.3 && Gen_QQ_mumi_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Gen_QQ_mumi_4mom.Eta()) && TMath::Abs(Gen_QQ_mumi_4mom.Eta()) < 2.2 && sqrt(Gen_QQ_mumi_4mom.Px()*Gen_QQ_mumi_4mom.Px()+ Gen_QQ_mumi_4mom.Py()*Gen_QQ_mumi_4mom.Py() + Gen_QQ_mumi_4mom.Pz()*Gen_QQ_mumi_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Gen_QQ_mumi_4mom.Eta()) && Gen_QQ_mumi_4mom.Pt()>=0.8) )";
	TCut accGen = accGenPlus && accGenMinus;
	TCut runCut = "runNb >=210498 && runNb <= 210658"; //1st reprocessedReco
	
//	TCut totalCutMC = trigCut && recoCut && rapRange && ptRange && accReco;
//	TCut totalCutMC = accReco;
	TCut rapRangeGen = "Gen_QQ_4mom.Rapidity() > -2.4 && Gen_QQ_4mom.Rapidity() < 2.4";
	TCut ptRangeGen = "Gen_QQ_4mom.Pt() > 0.0 && Gen_QQ_4mom.Pt() < 30.0";
	TCut totalCutMC = accGen &&rapRangeGen && ptRangeGen;

	// define 1D hist
	//const int nbin[] = {80, 80, 80, 50};
	const int nbin[] = {40, 60, 60, 60};
	const double minbin[] = {2.6, 0.0, -2.5, -3.2};
	const double maxbin[] = {3.5, 30.0, 2.5, 3.2};
//	TString histName[] = {"M", "Pt", "Rapidity","Phi"};
	TString histName[] = {"M", "Pt", "Eta","Phi"};
	TString histTitle[] = {"mass (GeV/c^{2})", "p_{T}^{#mu} (GeV/c)", "#eta_{lab}", "#phi (rad)"};
	
	const int nHist = sizeof(nbin)/sizeof(int);
	cout << "nHist = "<<nHist<<endl;

	TH1D *hPrivate[nHist];
	TH1D *hOfficial[nHist];
	TH1D *hRatio[nHist];
	
	TCanvas* c1[nHist];
	TCanvas* c2[nHist];
	TLegend *legUR = new TLegend(0.58,0.68,0.90,0.90,NULL,"brNDC");
	TLegend *legUM = new TLegend(0.30,0.68,0.65,0.90,NULL,"brNDC");
	TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
	TLegend *legBM = new TLegend(0.30,0.20,0.65,0.42,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legUM);
	SetLegendStyle(legUL);
	SetLegendStyle(legBM);

	for (int i=0; i<nHist; i++) {
		if (i==0) continue;
		hPrivate[i] = new TH1D(Form("hPrivate_%s",histName[i].Data()),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		hOfficial[i] = new TH1D(Form("hOfficial_%s",histName[i].Data()),Form(";%s;",histTitle[i].Data()),nbin[i],minbin[i],maxbin[i]);
		hPrivate[i]->Sumw2();
		hOfficial[i]->Sumw2();
		std::cout << i <<"th hist name = " << histName[i].Data() << std::endl;
		if (isSmall) {
			tree01->Draw(Form("Gen_QQ_mupl_4mom.%s()>>%s",histName[i].Data(),hPrivate[i]->GetName()),totalCutMC && massNarrow,"",nEntry);
			tree01->Draw(Form("Gen_QQ_mumi_4mom.%s()>>+%s",histName[i].Data(),hPrivate[i]->GetName()),totalCutMC && massNarrow,"",nEntry);
			tree02->Draw(Form("Gen_QQ_mupl_4mom.%s()>>%s",histName[i].Data(),hOfficial[i]->GetName()),totalCutMC && massNarrow,"",nEntry);
			tree02->Draw(Form("Gen_QQ_mumi_4mom.%s()>>+%s",histName[i].Data(),hOfficial[i]->GetName()),totalCutMC && massNarrow,"",nEntry);
		}
		else {
			tree01->Draw(Form("Gen_QQ_mupl_4mom.%s()>>%s",histName[i].Data(),hPrivate[i]->GetName()),totalCutMC && massNarrow,"");
			tree01->Draw(Form("Gen_QQ_mumi_4mom.%s()>>+%s",histName[i].Data(),hPrivate[i]->GetName()),totalCutMC && massNarrow,"");
			tree02->Draw(Form("Gen_QQ_mupl_4mom.%s()>>%s",histName[i].Data(),hOfficial[i]->GetName()),totalCutMC && massNarrow,"");
			tree02->Draw(Form("Gen_QQ_mumi_4mom.%s()>>+%s",histName[i].Data(),hOfficial[i]->GetName()),totalCutMC && massNarrow,"");
		}
		std::cout << histName[i].Data() <<" : Private integral = " << hPrivate[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : Official integral = " << hOfficial[i]->Integral() << std::endl;
		hPrivate[i]->Scale(1/hPrivate[i]->Integral());
		hOfficial[i]->Scale(1/hOfficial[i]->Integral());
	
		c1[i] = new TCanvas(Form("c1_%s",histName[i].Data()),"",600,600);
		c1[i]->cd();
		//if (i==0 || i==1 ) gPad->SetLogy(1);
		//else gPad->SetLogy(0);
		SetHistStyle(hPrivate[i],3,0);
		SetHistStyle(hOfficial[i],4,10);
		hPrivate[i]->SetFillColor(kOrange+7);
		hPrivate[i]->SetFillStyle(3003);
		hOfficial[i]->SetFillColor(kGreen+3);
		hOfficial[i]->SetFillStyle(3005);
		double tmpmax = hPrivate[i]->GetMaximum();
		hPrivate[i]->SetAxisRange(0., tmpmax*1.3," Y");
		hPrivate[i]->Draw("pe");
		hOfficial[i]->Draw("pe same");

			legUR->AddEntry(hPrivate[i],"privatae MC","l");
			legUR->AddEntry(hOfficial[i],"official MC","l");
			legUR->Draw();

		c1[i]->SaveAs(Form("MCvsMC_noMuID/sglvariables_gen_%s.pdf",histName[i].Data()));
//		c1[i]->SaveAs(Form("SglVariables/sglvariables_%s_%s.png",histName[i].Data(),stringA));
		//c1[i]->Clear();
		legUR->Clear();
		legUM->Clear();
		legUL->Clear();
		legBM->Clear();
	
		c2[i] = new TCanvas(Form("c2_%s",histName[i].Data()),"",600,600);
		c2[i]->cd();
		hRatio[i] = (TH1D*)hPrivate[i]->Clone(Form("hRatio_%d",i));
		hRatio[i]->Divide(hOfficial[i]);
		hRatio[i]->SetAxisRange(0.7,1.3, "Y");
		SetHistStyle(hRatio[i],0,0);
		hRatio[i]->Draw("pe");
		c2[i]->SaveAs(Form("MCvsMC_noMuID/sglratio_gen_%s.pdf",histName[i].Data()));
	}
/*
	//Save as a root file
	TFile* outFile = new TFile("SglVariables/sglvariables.root","RECREATE");
	outFile->cd();
	for (int i=0; i<nHist; i++) {
		if (i==0) continue;
		hPrivate[i]->Write();
		hOfficial[i]->Write();
	}	
	outFile->Close();
*/
	return 0;	

}


