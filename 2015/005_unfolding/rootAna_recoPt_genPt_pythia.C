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

bool kineCut(double muPt, double muEta, double muP);
bool massCut1(double lv_dimu_mass);
bool massCut2(double lv_dimu_mass);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

struct Condition {
	double theCentrality;
	int theType, theSign, Reco_QQ_trig, HLTriggers;
	double theMass, theRapidity, thePt, thePhi;
	double mupl_p, mumi_p, mupl_pt, mumi_pt, mupl_eta, mumi_eta;
	float theCtErr;
};

void rootAna_recoPt_genPt_pythia(char *strBinning = "8rap9ptMatchdR", bool isPrompt = true){

	using namespace std;
	
	// # of event range
	int initev =0;
	int nevt = -1; //all
	//int nevt = 10000;
		
	char* samplename;
	double minylab=-2.4;
	double maxylab=2.4;
	double minpt=0.0;
	double maxpt=30.0;

// read-in root file
	TFile *f1;
	if (isPrompt==1) {
		//f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_1Mevt.root");
		f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/tot_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_noMuID_sglTrig_genMatch_20150205.root");
		samplename="PRMCboosted_Pbp"; 
	} else  {
		f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/merged_B2Jpsi_PYTHIAboosted_1st_STARTHI53_V27_1Mevt.root");
		samplename="NPMCboosted_Pbp";
	} 

	const char* datestring = Form("%s_%s",strBinning,samplename);
	std::cout << "datestring: " << datestring << std::endl;
	
	///////////////////////////////////////////////////
	// Definition of binning
	// --- pt Bin
	//	Double_t ptBinsArr[] = {0.0,1.0,2.0,3.0,4.0,5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0, 35.0}; //20141228Finer
	Double_t ptBinsArr[] = {0.0,3.0,4.0,5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; //20141228
	const Int_t nPtBins = sizeof(ptBinsArr)/sizeof(double)-1;
	cout << "nPtBins=" << nPtBins << endl;

	// form array string
	string ptArr[nPtBins];
	for (Int_t i=0; i<nPtBins; i++) {
		formPtArr(ptBinsArr[i], ptBinsArr[i+1], &ptArr[i]);
		cout << i <<"th ptArr = " << ptArr[i] << endl;
	}

	///////////////////////////////////////////////////
	//read-in tree
	TTree *mytree = (TTree*)f1->Get("myTree");

  UInt_t          eventNb;
  Int_t           Centrality;
  Int_t           HLTriggers;
	Float_t         zVtx;
	Int_t           Reco_QQ_size;
  Int_t           Reco_QQ_type[20];   //[Reco_QQ_size] : for Data GG, GT, TT?
  Int_t           Reco_QQ_sign[20];   //[Reco_QQ_size] : 0=+,- / 1=+/+, 2=-/-
  Int_t           Reco_QQ_trig[20];   //[Reco_QQ_size] : DoubleMuOpen ==1
	Float_t         Reco_QQ_ctau[20];
	Float_t         Reco_QQ_ctauErr[20];
  TClonesArray    *Reco_QQ_4mom;
  TClonesArray    *Reco_QQ_mupl_4mom;
  TClonesArray    *Reco_QQ_mumi_4mom;
  TClonesArray    *Reco_QQ_mupl_matchedGen_4mom;
  TClonesArray    *Reco_QQ_mumi_matchedGen_4mom;
	float						Reco_QQ_mupl_matchedGen_dR[20];
	float						Reco_QQ_mumi_matchedGen_dR[20];
//  Int_t           Gen_QQ_size;
//  Int_t           Gen_QQ_type[20];	//[Gen_QQ_size] : for MC PR=0 / NP=1
//  TClonesArray    *Gen_QQ_4mom;
//  TClonesArray    *Gen_QQ_mupl_4mom;
//  TClonesArray    *Gen_QQ_mumi_4mom;

  TBranch        *b_eventNb;
  TBranch        *b_Centrality;   //!
	TBranch        *b_HLTriggers;
	TBranch        *b_zVtx;
  TBranch        *b_Reco_QQ_size;   //!
  TBranch        *b_Reco_QQ_type;   //!
  TBranch        *b_Reco_QQ_sign;   //!
  TBranch        *b_Reco_QQ_trig;   //!
	TBranch        *b_Reco_QQ_ctau; 
  TBranch        *b_Reco_QQ_ctauErr;
	TBranch        *b_Reco_QQ_4mom;   //!
  TBranch        *b_Reco_QQ_mupl_4mom;   //!
  TBranch        *b_Reco_QQ_mumi_4mom;   //!
  TBranch        *b_Reco_QQ_mupl_matchedGen_4mom;   //!
  TBranch        *b_Reco_QQ_mumi_matchedGen_4mom;   //!
  TBranch        *b_Reco_QQ_mupl_matchedGen_dR;   //!
  TBranch        *b_Reco_QQ_mumi_matchedGen_dR;   //!
//  TBranch        *b_Gen_QQ_size;   //!
//  TBranch        *b_Gen_QQ_type;
//  TBranch        *b_Gen_QQ_4mom;   //!
//  TBranch        *b_Gen_QQ_mupl_4mom;   //!
//  TBranch        *b_Gen_QQ_mumi_4mom;   //!

  TLorentzVector* JP_Reco = new TLorentzVector;
  TLorentzVector* m1P_Reco = new TLorentzVector;
  TLorentzVector* m2P_Reco = new TLorentzVector;
//  TLorentzVector* JP_Gen_tmp = new TLorentzVector;
//  TLorentzVector* JP_Gen_tmp_qq = new TLorentzVector;
  TLorentzVector* JP_Gen = new TLorentzVector;
  TLorentzVector* m1P_Gen = new TLorentzVector;
  TLorentzVector* m2P_Gen = new TLorentzVector;
		
	Reco_QQ_4mom =0;
	Reco_QQ_mupl_4mom =0;
	Reco_QQ_mumi_4mom =0;
	Reco_QQ_mupl_matchedGen_4mom =0;
	Reco_QQ_mumi_matchedGen_4mom =0;
//	Gen_QQ_4mom =0;
//	Gen_QQ_mupl_4mom =0;
//	Gen_QQ_mumi_4mom =0;

	// read-in branches
	mytree->SetBranchAddress("eventNb", &eventNb, &b_eventNb);
	mytree->SetBranchAddress("Centrality", &Centrality, &b_Centrality);
  mytree->SetBranchAddress("HLTriggers", &HLTriggers, &b_HLTriggers);
	mytree->SetBranchAddress("zVtx", &zVtx, &b_zVtx);
	mytree->SetBranchAddress("Reco_QQ_size", &Reco_QQ_size, &b_Reco_QQ_size);
  mytree->SetBranchAddress("Reco_QQ_type", Reco_QQ_type, &b_Reco_QQ_type);
  mytree->SetBranchAddress("Reco_QQ_sign", Reco_QQ_sign, &b_Reco_QQ_sign);
  mytree->SetBranchAddress("Reco_QQ_trig", Reco_QQ_trig, &b_Reco_QQ_trig);
	mytree->SetBranchAddress("Reco_QQ_ctau", Reco_QQ_ctau, &b_Reco_QQ_ctau);
  mytree->SetBranchAddress("Reco_QQ_ctauErr", Reco_QQ_ctauErr, &b_Reco_QQ_ctauErr);
	mytree->SetBranchAddress("Reco_QQ_4mom", &Reco_QQ_4mom, &b_Reco_QQ_4mom);
  mytree->SetBranchAddress("Reco_QQ_mupl_4mom", &Reco_QQ_mupl_4mom, &b_Reco_QQ_mupl_4mom);
  mytree->SetBranchAddress("Reco_QQ_mumi_4mom", &Reco_QQ_mumi_4mom, &b_Reco_QQ_mumi_4mom);
  mytree->SetBranchAddress("Reco_QQ_mupl_matchedGen_4mom", &Reco_QQ_mupl_matchedGen_4mom, &b_Reco_QQ_mupl_matchedGen_4mom);
  mytree->SetBranchAddress("Reco_QQ_mumi_matchedGen_4mom", &Reco_QQ_mumi_matchedGen_4mom, &b_Reco_QQ_mumi_matchedGen_4mom);
  mytree->SetBranchAddress("Reco_QQ_mupl_matchedGen_dR", &Reco_QQ_mupl_matchedGen_dR, &b_Reco_QQ_mupl_matchedGen_dR);
  mytree->SetBranchAddress("Reco_QQ_mumi_matchedGen_dR", &Reco_QQ_mumi_matchedGen_dR, &b_Reco_QQ_mumi_matchedGen_dR);
//	mytree->SetBranchAddress("Gen_QQ_size", &Gen_QQ_size, &b_Gen_QQ_size);
//	mytree->SetBranchAddress("Gen_QQ_type", Gen_QQ_type, &b_Gen_QQ_type);
//  mytree->SetBranchAddress("Gen_QQ_4mom", &Gen_QQ_4mom, &b_Gen_QQ_4mom);
//  mytree->SetBranchAddress("Gen_QQ_mupl_4mom", &Gen_QQ_mupl_4mom, &b_Gen_QQ_mupl_4mom);
//  mytree->SetBranchAddress("Gen_QQ_mumi_4mom", &Gen_QQ_mumi_4mom, &b_Gen_QQ_mumi_4mom);

	////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	// define 2D hist
	// fine binning
	TH2D *h2D_recoY_genY = new TH2D(Form("h2D_recoY_genY_%s",strBinning),"",1000,-2.5,2.5,1000,-2.5,2.5);
	TH2D *h2D_recoPt_genPt = new TH2D(Form("h2D_recoPt_genPt_%s",strBinning),"",1000,0.,30,1000,0.,30);
	TH2D *h2D_rawMatrix = new TH2D(Form("h2D_rawMatrix_%s",strBinning),"",nPtBins,ptBinsArr,nPtBins,ptBinsArr);
	TH2D *h2D_fixedMatrix = new TH2D(Form("h2D_fixedMatrix_%s",strBinning),"",nPtBins,0,nPtBins,nPtBins,0,nPtBins);

	h2D_recoY_genY->Sumw2();
	h2D_recoPt_genPt->Sumw2();
	h2D_rawMatrix->Sumw2();
	h2D_fixedMatrix->Sumw2();

	////////////////////////////////////////////////////////////////////////////////////////////////

	// event loop start
	if(nevt == -1) nevt = mytree->GetEntries();
	for(int iev=initev; iev<nevt; ++iev){
		if(iev%100000==0) cout << ">>>>> EVENT " << iev << " / " << mytree->GetEntries() <<  endl;

		//bool yn_reco = false;
		//bool yn_gen = false;
		struct Condition Jpsi_Reco; 
		struct Condition Jpsi_Gen; 

		mytree->GetEntry(iev);
		
		//////  GEN-RECO matching temporary
		//if (! (Gen_QQ_size ==1 && Reco_QQ_size==1) ) continue;

		// Reco_QQ_size loop
		for (Int_t irqq=0; irqq<Reco_QQ_size; ++irqq) {
			JP_Reco = (TLorentzVector*) Reco_QQ_4mom->At(irqq);
			m1P_Reco = (TLorentzVector*) Reco_QQ_mupl_4mom->At(irqq);
			m2P_Reco = (TLorentzVector*) Reco_QQ_mumi_4mom->At(irqq);

			Jpsi_Reco.theCentrality = 97.5; // for pp!
			Jpsi_Reco.HLTriggers = HLTriggers;
			Jpsi_Reco.theType = Reco_QQ_type[irqq]; // GG, GT, TT
			Jpsi_Reco.theSign = Reco_QQ_sign[irqq];
			Jpsi_Reco.Reco_QQ_trig  = Reco_QQ_trig[irqq];
			Jpsi_Reco.theCtErr = Reco_QQ_ctauErr[irqq];

			Jpsi_Reco.theMass = JP_Reco->M();
			Jpsi_Reco.theRapidity = JP_Reco->Rapidity(); // y_{lab}
			Jpsi_Reco.thePt = JP_Reco->Pt();
			Jpsi_Reco.thePhi = JP_Reco->Phi();

			Jpsi_Reco.mupl_p = sqrt( (m1P_Reco->Px())*(m1P_Reco->Px()) + (m1P_Reco->Py())*(m1P_Reco->Py()) + (m1P_Reco->Pz())*(m1P_Reco->Pz()) );
			Jpsi_Reco.mumi_p = sqrt( (m2P_Reco->Px())*(m2P_Reco->Px()) + (m2P_Reco->Py())*(m2P_Reco->Py()) + (m2P_Reco->Pz())*(m2P_Reco->Pz()) );
			Jpsi_Reco.mupl_pt = m1P_Reco->Pt();
			Jpsi_Reco.mumi_pt = m2P_Reco->Pt();
			Jpsi_Reco.mupl_eta = m1P_Reco->Eta();
			Jpsi_Reco.mumi_eta = m2P_Reco->Eta();
	
			bool yn_reco = false;
			bool yn_gen = false;
			if ( Jpsi_Reco.theSign ==0 
				//&& massCut1(Jpsi_Reco.theMass)
				&& massCut2(Jpsi_Reco.theMass)
				&& ( (Jpsi_Reco.Reco_QQ_trig&1)==1 && (Jpsi_Reco.HLTriggers&1)==1 )
				&& kineCut(Jpsi_Reco.mupl_pt, Jpsi_Reco.mupl_eta, Jpsi_Reco.mupl_p) 
				&& kineCut(Jpsi_Reco.mumi_pt, Jpsi_Reco.mumi_eta, Jpsi_Reco.mumi_p) 
				&& minpt<=Jpsi_Reco.thePt && Jpsi_Reco.thePt < maxpt
				&& minylab<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity < maxylab) {
					yn_reco = true;
			}

			// Gen matching with new root file !!
			if (yn_reco) {
				m1P_Gen = (TLorentzVector*) Reco_QQ_mupl_matchedGen_4mom->At(irqq);
				m2P_Gen = (TLorentzVector*) Reco_QQ_mumi_matchedGen_4mom->At(irqq);

				// variables only used for Reco
				Jpsi_Gen.HLTriggers = 0;
				Jpsi_Gen.Reco_QQ_trig  = 0;
				Jpsi_Gen.theSign = 0; //already +- pair
			
				Jpsi_Gen.theCentrality = 97.5; // for pp!
				//Jpsi_Gen.theType = Gen_QQ_type[igqq]; // PR or NP

				Jpsi_Gen.mupl_p = sqrt( (m1P_Gen->Px())*(m1P_Gen->Px()) + (m1P_Gen->Py())*(m1P_Gen->Py()) + (m1P_Gen->Pz())*(m1P_Gen->Pz()) );
				Jpsi_Gen.mumi_p = sqrt( (m2P_Gen->Px())*(m2P_Gen->Px()) + (m2P_Gen->Py())*(m2P_Gen->Py()) + (m2P_Gen->Pz())*(m2P_Gen->Pz()) );
				Jpsi_Gen.mupl_pt = m1P_Gen->Pt();
				Jpsi_Gen.mumi_pt = m2P_Gen->Pt();
				Jpsi_Gen.mupl_eta = m1P_Gen->Eta();
				Jpsi_Gen.mumi_eta = m2P_Gen->Eta();
				if ( //massCut1(Jpsi_Gen.theMass)
				kineCut(Jpsi_Gen.mupl_pt, Jpsi_Gen.mupl_eta, Jpsi_Gen.mupl_p) 
				&& kineCut(Jpsi_Gen.mumi_pt, Jpsi_Gen.mumi_eta, Jpsi_Gen.mumi_p)
				&& Reco_QQ_mupl_matchedGen_dR[irqq] < 0.2 // KYO dR cut
				&& Reco_QQ_mumi_matchedGen_dR[irqq] < 0.2 ) {
					*JP_Gen = *m1P_Gen +  *m2P_Gen; // instead of Gen_QQ
					Jpsi_Gen.theMass = JP_Gen->M();
					Jpsi_Gen.theRapidity = JP_Gen->Rapidity();	
					Jpsi_Gen.thePt = JP_Gen->Pt();
					Jpsi_Gen.thePhi = JP_Gen->Phi();
					if ( massCut1(Jpsi_Gen.theMass)  
							&& minpt<=Jpsi_Gen.thePt && Jpsi_Gen.thePt <maxpt 
							&& minylab<=Jpsi_Gen.theRapidity && Jpsi_Gen.theRapidity <maxylab) {
								yn_gen = true;
					}
				} //yn_gen
			} //yn_reco
				
			// fill the histogram
			if (yn_reco == true &&yn_gen == true){
				h2D_recoY_genY->Fill(Jpsi_Gen.theRapidity,Jpsi_Reco.theRapidity);
				h2D_recoPt_genPt->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
				h2D_rawMatrix->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
				//h2D_fixedMatrix->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
			}
		} // end of Reco_QQ_size loop

/*	
		////// Gen_QQ_size loop
		for (Int_t igqq=0; igqq<Gen_QQ_size; ++igqq) {
			//struct Condition Jpsi_Gen; 
			m1P_Gen = (TLorentzVector*) Gen_QQ_mupl_4mom->At(igqq);
			m2P_Gen = (TLorentzVector*) Gen_QQ_mumi_4mom->At(igqq);
			JP_Gen_tmp_qq = (TLorentzVector*) Gen_QQ_4mom->At(igqq); // Gen Jpsi (for filling NoCut)
			*JP_Gen_tmp = *m1P_Gen +  *m2P_Gen; // Gen dimuon pairs (for filling NoCut)

			// variables only used for Reco
			Jpsi_Gen.HLTriggers = 0;
			Jpsi_Gen.Reco_QQ_trig  = 0;
			Jpsi_Gen.theSign = 0; //already +- pair
			
			Jpsi_Gen.theCentrality = 97.5; // for pp!
			Jpsi_Gen.theType = Gen_QQ_type[igqq]; // PR or NP

			Jpsi_Gen.mupl_p = sqrt( (m1P_Gen->Px())*(m1P_Gen->Px()) + (m1P_Gen->Py())*(m1P_Gen->Py()) + (m1P_Gen->Pz())*(m1P_Gen->Pz()) );
			Jpsi_Gen.mumi_p = sqrt( (m2P_Gen->Px())*(m2P_Gen->Px()) + (m2P_Gen->Py())*(m2P_Gen->Py()) + (m2P_Gen->Pz())*(m2P_Gen->Pz()) );
			Jpsi_Gen.mupl_pt = m1P_Gen->Pt();
			Jpsi_Gen.mumi_pt = m2P_Gen->Pt();
			Jpsi_Gen.mupl_eta = m1P_Gen->Eta();
			Jpsi_Gen.mumi_eta = m2P_Gen->Eta();
	
			// --- cut02 : GEN for denominator
			//bool yn_gen = false;
			if ( //massCut1(Jpsi_Gen.theMass)
			kineCut(Jpsi_Gen.mupl_pt, Jpsi_Gen.mupl_eta, Jpsi_Gen.mupl_p) 
			&& kineCut(Jpsi_Gen.mumi_pt, Jpsi_Gen.mumi_eta, Jpsi_Gen.mumi_p)) {
				*JP_Gen = *m1P_Gen +  *m2P_Gen; // instead of Gen_QQ
				Jpsi_Gen.theMass = JP_Gen->M();
				Jpsi_Gen.theRapidity = JP_Gen->Rapidity();	
				Jpsi_Gen.thePt = JP_Gen->Pt();
				Jpsi_Gen.thePhi = JP_Gen->Phi();
				if ( massCut1(Jpsi_Gen.theMass)  
						&& minpt<=Jpsi_Gen.thePt && Jpsi_Gen.thePt <maxpt 
						&& minylab<=Jpsi_Gen.theRapidity && Jpsi_Gen.theRapidity <maxylab) {
							yn_gen = true;
				}
			}
		} //end of Gen_QQ_size loop
*/
/*
		// fill the histogram
		if (yn_reco == true &&yn_gen == true){
			h2D_recoY_genY->Fill(Jpsi_Gen.theRapidity,Jpsi_Reco.theRapidity);
			h2D_recoPt_genPt->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
			h2D_rawMatrix->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
			//h2D_fixedMatrix->Fill(Jpsi_Gen.thePt,Jpsi_Reco.thePt);
		}
*/
	} //end of event loop

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// calculate for fixedMatrix
	double fix_entries[nPtBins][nPtBins];
	double fixden_entries[nPtBins];
	for (Int_t igenpt=0; igenpt< nPtBins; igenpt++){	 
		fixden_entries[igenpt] =0;
		//cout << "***** igenpt= " <<igenpt << " ***********" << endl;
		for (Int_t irecopt=0; irecopt<nPtBins; irecopt++) {
			fix_entries[igenpt][irecopt] = (double)h2D_rawMatrix->GetBinContent(igenpt+1, irecopt+1);
			fixden_entries[igenpt] += (double)h2D_rawMatrix->GetBinContent(igenpt+1, irecopt+1);
			//cout << "* irecopt= "<<irecopt<< endl;
			//cout << "fix_entries = " <<fix_entries[igenpt][irecopt]<<endl;
			//cout << "fixden_entries = " <<fixden_entries[igenpt]<<endl;
		}
	}

	for (Int_t igenpt=0; igenpt< nPtBins; igenpt++){	 
		for (Int_t irecopt=0; irecopt<nPtBins; irecopt++) {
			cout << "***** igenpt= " <<igenpt <<", irecopt= "<<irecopt<< endl;
			cout << "fix_entries = " <<fix_entries[igenpt][irecopt]<<endl;
			cout << "fixden_entries = " <<fixden_entries[igenpt]<<endl;
			h2D_fixedMatrix->SetBinContent(igenpt+1, irecopt+1, fix_entries[igenpt][irecopt]/fixden_entries[igenpt]);	
		}
	}

	// axis
	for (Int_t igenpt=0; igenpt< nPtBins; igenpt++){	 
		h2D_fixedMatrix->GetXaxis()->SetBinLabel(igenpt+1,ptArr[igenpt].c_str());
	}
	for (Int_t irecopt=0; irecopt< nPtBins; irecopt++){	 
		h2D_fixedMatrix->GetYaxis()->SetBinLabel(irecopt+1,ptArr[irecopt].c_str());
	}
	
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Save the data!

	// --- save03 : save as a root file
	TFile *outFile = new TFile(Form("recoPt_genPt_%s.root",datestring),"RECREATE");
	std::cout << "datestring: " << datestring << std::endl;
	outFile->cd();
	h2D_recoY_genY->Write();
	h2D_recoPt_genPt->Write();
	h2D_rawMatrix->Write();
	h2D_fixedMatrix->Write();
	outFile->Close();

} // end of main func

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

bool kineCut(double muPt, double muEta, double muP) {
	return ( TMath::Abs(muEta) < 2.4 &&
				((TMath::Abs(muEta) < 1.3 && muPt >=3.3) ||
				(1.3 <= TMath::Abs(muEta) && TMath::Abs(muEta) < 2.2 && muP >=2.9) ||
				(2.2 <= TMath::Abs(muEta) && muPt >= 0.8)));
}

bool massCut1(double lv_dimu_mass) {
	if(lv_dimu_mass < 2.6 || lv_dimu_mass >= 3.5) {return false;}
	else {return true;}
}

bool massCut2(double lv_dimu_mass) {
	//if(lv_dimu_mass < 2.95 || lv_dimu_mass >= 3.25) {return false;}
	if(lv_dimu_mass < 2.9 || lv_dimu_mass >= 3.3) {return false;}
	else {return true;}
}

void formPtArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
//		*arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
		*arr = Form("%.0f-%.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f-%.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f-%.1f", binmin, binmax);
	} else {
		*arr = Form("%.1f-%.1f", binmin, binmax);
	}
}
