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

int rootAna_2Dsglmupt(){

	// # of event range
	const int nEntry = 5000;
		
	gROOT->Macro("./JpsiStyle.C");

	TFile *fPRMCPbp;
//	TFile *fPRMCpPb;
	TFile *fNPMCPbp;
//	TFile *fNPMCpPb;
	
	// read-in root file
	// nominal
	PRMCPbp = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/merged_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_1Mevt.root");
//	fPRMCpPb = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/merged_PromptJpsi_PYTHIAboosted_2nd_STARTHI53_V27_1Mevt.root");
	fNPMCPbp = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/merged_B2Jpsi_PYTHIAboosted_1st_STARTHI53_V27_1Mevt.root");
//	fNPMCpPb = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/merged_B2Jpsi_PYTHIAboosted_2nd_STARTHI53_V27_1Mevt.root");

	TTree *treePRMCPbp = (TTree*)fPRMCPbp->Get("myTree");
//	TTree *treePRMCpPb = (TTree*)fPRMCpPb->Get("myTree");
	TTree *treeNPMCPbp = (TTree*)fNPMCPbp->Get("myTree");
//	TTree *treeNPMCpPb = (TTree*)fNPMCpPb->Get("myTree");

	// cut definitions
	TCut trigCut = "((Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )";
	TCut recoCut = "Reco_QQ_size>=1 && Reco_QQ_sign==0";	
	TCut genCut = "Gen_QQ_size>=1";	
	TCut massRange01 = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
	TCut massRange02 = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
	//TCut rapRange = "Reco_QQ_4mom.Rapidity() > -2.4 && Reco_QQ_4mom.Rapidity() < 2.4";
	//TCut ptRange = "Reco_QQ_4mom.Pt() > 0.0 && Reco_QQ_4mom.Pt() < 30.0";
	TCut rapRange = "Reco_QQ_4mom.Rapidity() > -2.4 && Reco_QQ_4mom.Rapidity() < -1.97";
	//TCut ptRange = "Reco_QQ_4mom.Pt() > 0.0 && Reco_QQ_4mom.Pt() < 3.0";
	TCut ptRange = "Reco_QQ_4mom.Pt() > 2.0 && Reco_QQ_4mom.Pt() < 3.0";
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
	const int nbin[] = {100};
	//const int nbin[] = {5000};
	//const int nbin[] = {60, 60, 40, 30};
	const double minbin[] = {0.0};
//	const double maxbin[] = {30.0};
	const double maxbin[] = {5.0};
//	TString histName[] = {"M", "Pt", "Rapidity","Phi"};
	TString histName[] = {"Pt"};
	TString histTitle[] = {"p_{T}"};
	
	const int nHist = sizeof(nbin)/sizeof(int);
	cout << "nHist = "<<nHist<<endl;

	TH2D *hDataPbp[nHist];
	TH2D *hSigPbp[nHist];
	TH2D *hBkgPbp[nHist];
	TH2D *hPRMCPbp[nHist];
	TH2D *hNPMCPbp[nHist];
	
	TCanvas* c_Data[nHist];
	TCanvas* c_Sig[nHist];
	TCanvas* c_Bkg[nHist];
	TCanvas* c_PRMC[nHist];
	TCanvas* c_NPMC[nHist];
	TLegend *legUR = new TLegend(0.58,0.68,0.90,0.90,NULL,"brNDC");
	TLegend *legUM = new TLegend(0.30,0.68,0.65,0.90,NULL,"brNDC");
	TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
	TLegend *legBM = new TLegend(0.30,0.20,0.65,0.42,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legUM);
	SetLegendStyle(legUL);
	SetLegendStyle(legBM);

	for (int i=0; i<nHist; i++) {
		hDataPbp[i] = new TH2D(Form("hDataPbp_%s",histName[i].Data()),Form(";%s^{#mu+} (GeV/c); %s^{#mu-} (GeV/c)",histTitle[i].Data(), histTitle[i].Data()),nbin[i],minbin[i],maxbin[i], nbin[i],minbin[i],maxbin[i]);
		hSigPbp[i] = new TH2D(Form("hSigPbp_%s",histName[i].Data()),Form(";%s^{#mu+} (GeV/c); %s^{#mu-} (GeV/c)",histTitle[i].Data(), histTitle[i].Data()),nbin[i],minbin[i],maxbin[i],nbin[i],minbin[i],maxbin[i]);
		hBkgPbp[i] = new TH2D(Form("hBkgPbp_%s",histName[i].Data()),Form(";%s^{#mu+} (GeV/c); %s^{#mu-} (GeV/c)",histTitle[i].Data(), histTitle[i].Data()),nbin[i],minbin[i],maxbin[i],nbin[i],minbin[i],maxbin[i]);
		hPRMCPbp[i] = new TH2D(Form("hPRMCPbp_%s",histName[i].Data()),Form(";%s^{#mu+} (GeV/c); %s^{#mu-} (GeV/c)",histTitle[i].Data(), histTitle[i].Data()),nbin[i],minbin[i],maxbin[i],nbin[i],minbin[i],maxbin[i]);
		hNPMCPbp[i] = new TH2D(Form("hNPMCPbp_%s",histName[i].Data()),Form(";%s^{#mu+} (GeV/c); %s^{#mu-} (GeV/c)",histTitle[i].Data(), histTitle[i].Data()),nbin[i],minbin[i],maxbin[i],nbin[i],minbin[i],maxbin[i]);
		hDataPbp[i]->Sumw2();
		hSigPbp[i]->Sumw2();
		hBkgPbp[i]->Sumw2();
		hPRMCPbp[i]->Sumw2();
		hNPMCPbp[i]->Sumw2();
		std::cout << i <<"th hist name = " << histName[i].Data() << std::endl;
			treeDataPbp1->Draw(Form("Reco_QQ_mumi_4mom.%s():Reco_QQ_mupl_4mom.%s()>>%s",histName[i].Data(),histName[i].Data(),hDataPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treeDataPbp1->Draw(Form("Reco_QQ_mumi_4mom.%s():Reco_QQ_mupl_4mom.%s()>>%s",histName[i].Data(),histName[i].Data(),hSigPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange02,"");
			treeDataPbp1->Draw(Form("Reco_QQ_mumi_4mom.%s():Reco_QQ_mupl_4mom.%s()>>%s",histName[i].Data(),histName[i].Data(),hBkgPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01 && (!massRange02),"");
			treeDataPbp2->Draw(Form("Reco_QQ_mumi_4mom.%s():Reco_QQ_mupl_4mom.%s()>>+%s",histName[i].Data(),histName[i].Data(),hDataPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01,"");
			treeDataPbp2->Draw(Form("Reco_QQ_mumi_4mom.%s():Reco_QQ_mupl_4mom.%s()>>+%s",histName[i].Data(),histName[i].Data(),hSigPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange02,"");
			treeDataPbp2->Draw(Form("Reco_QQ_mumi_4mom.%s():Reco_QQ_mupl_4mom.%s()>>+%s",histName[i].Data(),histName[i].Data(),hBkgPbp[i]->GetName()),(totalCut&&(!runCut)) && massRange01 && (!massRange02),"");
			treePRMCPbp->Draw(Form("Reco_QQ_mumi_4mom.%s():Reco_QQ_mupl_4mom.%s()>>%s",histName[i].Data(),histName[i].Data(),hPRMCPbp[i]->GetName()),totalCutMC && massRange02,"");
			treeNPMCPbp->Draw(Form("Reco_QQ_mumi_4mom.%s():Reco_QQ_mupl_4mom.%s()>>%s",histName[i].Data(),histName[i].Data(),hNPMCPbp[i]->GetName()),totalCutMC && massRange02,"");
		std::cout << histName[i].Data() <<" : DataPbp Integral = " << hDataPbp[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : SigPbp Integral = " << hSigPbp[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : BkgPbp Integral = " << hBkgPbp[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : PRMC Integral = " << hPRMCPbp[i]->Integral() << std::endl;
		std::cout << histName[i].Data() <<" : NPMC Integral = " << hNPMCPbp[i]->Integral() << std::endl;
//		hDataPbp[i]->Scale(1/hDataPbp[i]->Integral());
//		hSigPbp[i]->Scale(1/hSigPbp[i]->Integral());
//		hBkgPbp[i]->Scale(1/hBkgPbp[i]->Integral());
//		hPRMCPbp[i]->Scale(1/hPRMCPbp[i]->Integral());
//		hNPMCPbp[i]->Scale(1/hNPMCPbp[i]->Integral());
		
		c_Data[i] = new TCanvas(Form("c_Data_%s",histName[i].Data()),"",600,600);
		c_Data[i]->cd();
		hDataPbp[i]->Draw("colz");
		
		c_Sig[i] = new TCanvas(Form("c_Sig_%s",histName[i].Data()),"",600,600);
		c_Sig[i]->cd();
		hSigPbp[i]->Draw("colz");
		
		c_Bkg[i] = new TCanvas(Form("c_Bkg_%s",histName[i].Data()),"",600,600);
		c_Bkg[i]->cd();
		hBkgPbp[i]->Draw("colz");
		
		c_PRMC[i] = new TCanvas(Form("c_PRMC_%s",histName[i].Data()),"",600,600);
		c_PRMC[i]->cd();
		hPRMCPbp[i]->Draw("colz");

		c_NPMC[i] = new TCanvas(Form("c_NPMC_%s",histName[i].Data()),"",600,600);
		c_NPMC[i]->cd();
		hNPMCPbp[i]->Draw("colz");

		c_Data[i]->SaveAs("2DVar/2Dsglmupt_Data.pdf");
		c_Sig[i]->SaveAs("2DVar/2Dsglmupt_Sig.pdf");
		c_Bkg[i]->SaveAs("2DVar/2Dsglmupt_Bkg.pdf");
		c_PRMC[i]->SaveAs("2DVar/2Dsglmupt_PRMC.pdf");
		c_Bkg[i]->SaveAs("2DVar/2Dsglmupt_Bkg.pdf");
	}

	//Save as a root file
	TFile* outFile = new TFile("2DVar/2Dsglmupt.root","RECREATE");
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


