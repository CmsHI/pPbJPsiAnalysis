// rootAna_efficiency.cc
// Author: Yongsun Kim, Songkyo Lee, Dongho Moon
// Final editor : Yongsun Kim ( kimy@cern.ch)

// Calculates di-muon reconstruction efficiency in rapidity and pT bins using MC generated particle information 

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

// Basically our basic frame is collision frame, in this code all value means in collision frame.


bool kineCut(double muPt, double muEta, double muP);
bool massCut1(double lv_dimu_mass);
bool massCut2(double lv_dimu_mass);

struct Condition {
	double theCentrality; 
  int theType, theSign, Reco_QQ_trig;
  double theMass, theRapidity, thePt, thePhi;
  double mupl_p, mumi_p, mupl_pt, mumi_pt, mupl_eta, mumi_eta;
} ;

void rootAna_efficiency( int fileCode = 0, double rapbinmin=-2.4, double rapbinmax=1.46, double ptbinmin=6.5, double ptbinmax=30.0){

  TH1::SetDefaultSumw2();
  cout << " Warning1 : This macro is valid only for the kinematic variablese in LAB frame" << endl;
  
  const int kFW = 0;
  const int kBW = 1;
  
  
  // # of event range
  int initev =0;
  int nevt = -1; //all
  //int nevt = 5000;
  
  gStyle->SetCanvasDefW(800);
  
  TFile *f1;
  char* samplename;
  if (fileCode==0) {f1 = new TFile("/home/samba/songkyo/efficiency/PRMC_eff_rootFile/merged_PRMC_eff_Histos.root"); samplename="PRMC_boosted"; }
  else if (fileCode==1) {f1 = new TFile("/home/samba/songkyo/efficiency/NPMC_eff_rootFile/merged_NPMC_eff_Histos.root"); samplename="NPMC_boosted"; }
  
  
  const char* datestring = Form("%s",samplename);
  std::cout << "datestring: " << datestring << std::endl;
  
  //read-in tree
  TTree *mytree = (TTree*)f1->Get("myTree");
  
  UInt_t          eventNb;
  Int_t           Centrality;
  Int_t           Reco_QQ_size;
  Int_t           Reco_QQ_type[20];   //[Reco_QQ_size] : for Data GG, GT, TT?
  Int_t           Reco_QQ_sign[20];   //[Reco_QQ_size] : 0=+,- / 1=+/+, 2=-/-
  Int_t           Reco_QQ_trig[20];   //[Reco_QQ_size] : DoubleMuOpen ==1
  TClonesArray    *Reco_QQ_4mom;
  TClonesArray    *Reco_QQ_mupl_4mom;
  TClonesArray    *Reco_QQ_mumi_4mom;
  Int_t           Gen_QQ_size;
  Int_t           Gen_QQ_type[20];	//[Gen_QQ_size] : for MC PR=0 / NP=1
  TClonesArray    *Gen_QQ_4mom;
  TClonesArray    *Gen_QQ_mupl_4mom;
  TClonesArray    *Gen_QQ_mumi_4mom;
  
  TBranch        *b_eventNb;
  TBranch        *b_Centrality;   //!
  TBranch        *b_Reco_QQ_size;   //!
  TBranch        *b_Reco_QQ_type;   //!
  TBranch        *b_Reco_QQ_sign;   //!
  TBranch        *b_Reco_QQ_trig;   //!
  TBranch        *b_Reco_QQ_4mom;   //!
  TBranch        *b_Reco_QQ_mupl_4mom;   //!
  TBranch        *b_Reco_QQ_mumi_4mom;   //!
  TBranch        *b_Gen_QQ_size;   //!
  TBranch        *b_Gen_QQ_type;
  TBranch        *b_Gen_QQ_4mom;   //!
  TBranch        *b_Gen_QQ_mupl_4mom;   //!
  TBranch        *b_Gen_QQ_mumi_4mom;   //!
  
  TLorentzVector* JP_Reco = new TLorentzVector;
  TLorentzVector* m1P_Reco = new TLorentzVector;
  TLorentzVector* m2P_Reco = new TLorentzVector;
  TLorentzVector* JP_Gen = new TLorentzVector;
  TLorentzVector* m1P_Gen = new TLorentzVector;
  TLorentzVector* m2P_Gen = new TLorentzVector;
  
  Reco_QQ_4mom =0;
  Reco_QQ_mupl_4mom =0;
  Reco_QQ_mumi_4mom =0;
  Gen_QQ_4mom =0;
  Gen_QQ_mupl_4mom =0;
  Gen_QQ_mumi_4mom =0;
  
  // read-in branches
  mytree->SetBranchAddress("eventNb", &eventNb, &b_eventNb);
  mytree->SetBranchAddress("Centrality", &Centrality, &b_Centrality);
  mytree->SetBranchAddress("Reco_QQ_size", &Reco_QQ_size, &b_Reco_QQ_size);
  mytree->SetBranchAddress("Reco_QQ_type", Reco_QQ_type, &b_Reco_QQ_type);
  mytree->SetBranchAddress("Reco_QQ_sign", Reco_QQ_sign, &b_Reco_QQ_sign);
  mytree->SetBranchAddress("Reco_QQ_trig", Reco_QQ_trig, &b_Reco_QQ_trig);
  mytree->SetBranchAddress("Reco_QQ_4mom", &Reco_QQ_4mom, &b_Reco_QQ_4mom);
  mytree->SetBranchAddress("Reco_QQ_mupl_4mom", &Reco_QQ_mupl_4mom, &b_Reco_QQ_mupl_4mom);
  mytree->SetBranchAddress("Reco_QQ_mumi_4mom", &Reco_QQ_mumi_4mom, &b_Reco_QQ_mumi_4mom);
  mytree->SetBranchAddress("Gen_QQ_size", &Gen_QQ_size, &b_Gen_QQ_size);
  mytree->SetBranchAddress("Gen_QQ_type", Gen_QQ_type, &b_Gen_QQ_type);
  mytree->SetBranchAddress("Gen_QQ_4mom", &Gen_QQ_4mom, &b_Gen_QQ_4mom);
  mytree->SetBranchAddress("Gen_QQ_mupl_4mom", &Gen_QQ_mupl_4mom, &b_Gen_QQ_mupl_4mom);
  mytree->SetBranchAddress("Gen_QQ_mumi_4mom", &Gen_QQ_mumi_4mom, &b_Gen_QQ_mumi_4mom);
  
	
  using namespace std;
  
  // Definition of binning
  // binCode ==1
  // --- pt Bin
  Double_t ptBinsArr[] = {6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 13.0, 16.0, 20.0, 30.0};
  const Int_t nPtBins = sizeof(ptBinsArr)/sizeof(double)-1;
  cout << "nPtBins=" << nPtBins << endl;
  
  // --- y Bin
  Double_t yBinsArr[] = {-2.4, -1.97, -1.72, -1.47, -1.22, -0.97, -0.72, -0.47, -0.22, 0.03, 0.28, 0.53, 0.78, 1.03, 1.46}; //14bin
  const Int_t nYBins = sizeof(yBinsArr)/sizeof(double)-1;
  cout << "nYBins=" << nYBins << endl;
  
  std::cout << "datestring: " << datestring << std::endl;
  
  // with defined binning 1 (y dependence)
  //  TH1D *h1D_NoCut_Reco_y_bin = new TH1D(Form("h1D_NoCut_Reco_y_bin"),"",nYBins,yBinsArr);
  //  TH1D *h1D_NoCut_Gen_y_bin = new TH1D(Form("h1D_NoCut_Gen_y_bin"),"",nYBins,yBinsArr);
  TH1D *h1D_All_y_bin = new TH1D(Form("h1D_All_y_bin"),"",nYBins,yBinsArr);  // all jpsis
  TH1D *h1D_Den_y_bin = new TH1D(Form("h1D_Den_y_bin"),"",nYBins,yBinsArr);  // accepted jpsi
  TH1D *h1D_Acc_y_bin = new TH1D(Form("h1D_Acc_y_bin"),"",nYBins,yBinsArr);  // acceptance 

  TH1D *h1D_RecoDimu_y_bin[10];
  TH1D *h1D_EffDimu_y_bin[10];

  h1D_RecoDimu_y_bin[0] = new TH1D(Form("h1D_RecoDimu_y_bin"),"",nYBins,yBinsArr);
  h1D_EffDimu_y_bin[0]  = new TH1D(Form("h1D_EffDimu_y_bin"),"",nYBins,yBinsArr);
  h1D_RecoDimu_y_bin[1] = new TH1D(Form("h1D_RecoDimu_y_bin_trig"),"",nYBins,yBinsArr);
  h1D_EffDimu_y_bin[1]  = new TH1D(Form("h1D_EffDimu_y_bin_trig"),"",nYBins,yBinsArr);
  h1D_RecoDimu_y_bin[2] = new TH1D(Form("h1D_RecoDimu_y_bin_mupl"),"",nYBins,yBinsArr);
  h1D_EffDimu_y_bin[2]  = new TH1D(Form("h1D_EffDimu_y_bin_mupl"),"",nYBins,yBinsArr);
  h1D_RecoDimu_y_bin[3] = new TH1D(Form("h1D_RecoDimu_y_bin_mumi"),"",nYBins,yBinsArr);
  h1D_EffDimu_y_bin[3]  = new TH1D(Form("h1D_EffDimu_y_bin_mumi"),"",nYBins,yBinsArr);
  h1D_RecoDimu_y_bin[4] = new TH1D(Form("h1D_RecoDimu_y_bin_jpsi"),"",nYBins,yBinsArr);
  h1D_EffDimu_y_bin[4]  = new TH1D(Form("h1D_EffDimu_y_bin_jpsi"),"",nYBins,yBinsArr);
  h1D_RecoDimu_y_bin[5] = new TH1D(Form("h1D_RecoDimu_y_bin_mass"),"",nYBins,yBinsArr);
  h1D_EffDimu_y_bin[5]  = new TH1D(Form("h1D_EffDimu_y_bin_mass"),"",nYBins,yBinsArr);

  // with defined binning 2 (pt dependence - FW)
  TH1D *h1D_NoCut_Reco_pt_bin[2];
  TH1D *h1D_NoCut_Gen_pt_bin[2];
  TH1D *h1D_All_pt_bin[2];   // All Gen jpsis..
  TH1D *h1D_Den_pt_bin[2];   // accepted jpsis..
  TH1D *h1D_Acc_pt_bin[2];   // acceptance 

  TH1D *h1D_RecoDimu_pt_bin[10][2];
  TH1D *h1D_EffDimu_pt_bin[10][2];
  for ( int i=0; i<=1 ; i++) { 
    TString fwbw="BW";    
    if ( i==kFW)  fwbw = "FW";  	  else fwbw = "BW";
    h1D_NoCut_Reco_pt_bin[i] = new TH1D(Form("h1D_NoCut_Reco_pt_bin_%s",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_NoCut_Gen_pt_bin[i] = new TH1D(Form("h1D_NoCut_Gen_pt_bin_%s",fwbw.Data()),"",nPtBins,ptBinsArr);

    h1D_All_pt_bin[i] = new TH1D(Form("h1D_All_pt_bin_%s",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_Acc_pt_bin[i] = new TH1D(Form("h1D_Acc_pt_bin_%s",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_Den_pt_bin[i] = new TH1D(Form("h1D_Den_pt_bin_%s",fwbw.Data()),"",nPtBins,ptBinsArr);

    h1D_RecoDimu_pt_bin[0][i] = new TH1D(Form("h1D_RecoDimu_pt_bin_%s",fwbw.Data()),"",nPtBins,ptBinsArr); // 0 = final
    h1D_EffDimu_pt_bin[0][i] = new TH1D(Form("h1D_EffDimu_pt_bin_%s",fwbw.Data()),"",nPtBins,ptBinsArr);

    h1D_RecoDimu_pt_bin[1][i] = new TH1D(Form("h1D_RecoDimu_pt_bin_%s_trig",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_EffDimu_pt_bin[1][i] = new TH1D(Form("h1D_EffDimu_pt_bin_%s_trig",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_RecoDimu_pt_bin[2][i] = new TH1D(Form("h1D_RecoDimu_pt_bin_%s_mupl",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_EffDimu_pt_bin[2][i] = new TH1D(Form("h1D_EffDimu_pt_bin_%s_mupl",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_RecoDimu_pt_bin[3][i] = new TH1D(Form("h1D_RecoDimu_pt_bin_%s_mumi",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_EffDimu_pt_bin[3][i] = new TH1D(Form("h1D_EffDimu_pt_bin_%s_mumi",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_RecoDimu_pt_bin[4][i] = new TH1D(Form("h1D_RecoDimu_pt_bin_%s_jpsi",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_EffDimu_pt_bin[4][i] = new TH1D(Form("h1D_EffDimu_pt_bin_%s_jpsi",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_RecoDimu_pt_bin[5][i] = new TH1D(Form("h1D_RecoDimu_pt_bin_%s_mass",fwbw.Data()),"",nPtBins,ptBinsArr);
    h1D_EffDimu_pt_bin[5][i] = new TH1D(Form("h1D_EffDimu_pt_bin_%s_mass",fwbw.Data()),"",nPtBins,ptBinsArr);
  }
  
  std::cout << "TH1, TH2 definition, datestring: " << datestring << std::endl;
  
  // temp - mass check
  TH1D *h1D_Reco_M = new TH1D("h1D_Reco_M","h1D_Reco_M",100,2.6,3.5);
  TH1D *h1D_Num_M = new TH1D("h1D_Num_M","h1D_Num_M",100,2.6,3.5);
  TH1D *h1D_Gen_M = new TH1D("h1D_Gen_M","h1D_Gen_M",100,2.6,3.5);
  TH1D *h1D_Den_M = new TH1D("h1D_Den_M","h1D_Den_M",100,2.6,3.5);
  TH2D *hdPtEta =   new TH2D("hdPtEta",";dpt;deta",30,-15,15,10,-5,5);
  float genpt(-1), recopt(-1), geny(-1), recoy(-1);
  ////////////////////////////////////////////////////////////////////////////////////////////////
  
  std::cout << "before check eff. datestring: " << datestring << std::endl;
  
  // event loop start
  if(nevt == -1) nevt = mytree->GetEntries();
  for(int iev=initev; iev<nevt; ++iev){
    if(iev%100000==0) cout << ">>>>> EVENT " << iev << " / " << mytree->GetEntries() <<  endl;
    
    mytree->GetEntry(iev);

    // Gen_QQ_size loop
    for (Int_t igqq=0; igqq<Gen_QQ_size; ++igqq) {
      struct Condition Jpsi_Gen; 
      m1P_Gen = (TLorentzVector*) Gen_QQ_mupl_4mom->At(igqq);
      m2P_Gen = (TLorentzVector*) Gen_QQ_mumi_4mom->At(igqq);
      *JP_Gen = *(m1P_Gen) + *(m2P_Gen) ;      //      JP_Gen = (TLorentzVector*) Gen_QQ_4mom->At(igqq);

      /*      cout << " m1P_Gen.E() = " << m1P_Gen->E() << ", ";   // Check this function
      cout << " m2P_Gen.E() = " << m2P_Gen->E() << endl;
      cout << " JP_Gen.E() = " << JP_Gen->E() << endl;*/

      // variables only used for Reco
      Jpsi_Gen.theMass = JP_Gen->M();
      Jpsi_Gen.Reco_QQ_trig  = 0;
      Jpsi_Gen.theSign = 0;
      
      Jpsi_Gen.theCentrality = 97.5; // for pp!
      Jpsi_Gen.theType = Gen_QQ_type[igqq]; // PR or NP
      
      Jpsi_Gen.theRapidity = JP_Gen->Rapidity(); // boosted so that -2.4 < y_{lab} < 1.46	
      Jpsi_Gen.thePt = JP_Gen->Pt();
      Jpsi_Gen.thePhi = JP_Gen->Phi();
      
      Jpsi_Gen.mupl_p = sqrt( (m1P_Gen->Px())*(m1P_Gen->Px()) + (m1P_Gen->Py())*(m1P_Gen->Py()) + (m1P_Gen->Pz())*(m1P_Gen->Pz()) );
      Jpsi_Gen.mumi_p = sqrt( (m2P_Gen->Px())*(m2P_Gen->Px()) + (m2P_Gen->Py())*(m2P_Gen->Py()) + (m2P_Gen->Pz())*(m2P_Gen->Pz()) );
      Jpsi_Gen.mupl_pt = m1P_Gen->Pt();
      Jpsi_Gen.mumi_pt = m2P_Gen->Pt();
      Jpsi_Gen.mupl_eta = m1P_Gen->Eta();
      Jpsi_Gen.mumi_eta = m2P_Gen->Eta();
      
      // --- cut01 : GEN - No cut

      if ( !( (ptbinmin<=Jpsi_Gen.thePt) && (Jpsi_Gen.thePt <ptbinmax)	&& (rapbinmin<=Jpsi_Gen.theRapidity) && (Jpsi_Gen.theRapidity <rapbinmax)) )
	continue;
      
      h1D_Gen_M->Fill(Jpsi_Gen.theMass);
      h1D_All_y_bin->Fill(Jpsi_Gen.theRapidity);
      if (-2.4<=Jpsi_Gen.theRapidity && Jpsi_Gen.theRapidity<-0.47) {
        h1D_All_pt_bin[kFW]->Fill(Jpsi_Gen.thePt);
      }
      else if (-0.47<=Jpsi_Gen.theRapidity && Jpsi_Gen.theRapidity<1.46) {
        h1D_All_pt_bin[kBW]->Fill(Jpsi_Gen.thePt);
      }


      // --- cut02 : GEN for denominator
      if (! ( (massCut1(Jpsi_Gen.theMass)) && kineCut( Jpsi_Gen.mupl_pt, Jpsi_Gen.mupl_eta, Jpsi_Gen.mupl_p)
	      && kineCut( Jpsi_Gen.mumi_pt, Jpsi_Gen.mumi_eta, Jpsi_Gen.mumi_p) ) )
	continue;
      // Let's pick up Well reconstructed J/psi
      
      h1D_Den_M->Fill(Jpsi_Gen.theMass);
      
      h1D_Den_y_bin->Fill(Jpsi_Gen.theRapidity);
      if (-2.4<=Jpsi_Gen.theRapidity && Jpsi_Gen.theRapidity<-0.47) {
	h1D_Den_pt_bin[kFW]->Fill(Jpsi_Gen.thePt);
      }
      else if (-0.47<=Jpsi_Gen.theRapidity && Jpsi_Gen.theRapidity<1.46) {
	h1D_Den_pt_bin[kBW]->Fill(Jpsi_Gen.thePt);
      }
      
      if ( Gen_QQ_size == 1 ) {
	geny = Jpsi_Gen.theRapidity; 
	genpt  = Jpsi_Gen.thePt;
      }
    } //end of Gen_QQ_size loop
    
    // Reco_QQ_size loop
    for (Int_t irqq=0; irqq<Reco_QQ_size; ++irqq) {
      struct Condition Jpsi_Reco; 
      JP_Reco = (TLorentzVector*) Reco_QQ_4mom->At(irqq);
      m1P_Reco = (TLorentzVector*) Reco_QQ_mupl_4mom->At(irqq);
      m2P_Reco = (TLorentzVector*) Reco_QQ_mumi_4mom->At(irqq);
      
      Jpsi_Reco.theCentrality = 97.5; // for pp!
      Jpsi_Reco.theType = Reco_QQ_type[irqq]; // GG, GT, TT
      Jpsi_Reco.theSign = Reco_QQ_sign[irqq];
      Jpsi_Reco.Reco_QQ_trig  = Reco_QQ_trig[irqq];
      
      Jpsi_Reco.theMass = JP_Reco->M();
      Jpsi_Reco.theRapidity = JP_Reco->Rapidity(); // boosted so that -2.4 < y_{lab} < 1.46	
      Jpsi_Reco.thePt = JP_Reco->Pt();
      Jpsi_Reco.thePhi = JP_Reco->Phi();
      
      Jpsi_Reco.mupl_p = sqrt( (m1P_Reco->Px())*(m1P_Reco->Px()) + (m1P_Reco->Py())*(m1P_Reco->Py()) + (m1P_Reco->Pz())*(m1P_Reco->Pz()) );
      Jpsi_Reco.mumi_p = sqrt( (m2P_Reco->Px())*(m2P_Reco->Px()) + (m2P_Reco->Py())*(m2P_Reco->Py()) + (m2P_Reco->Pz())*(m2P_Reco->Pz()) );
      Jpsi_Reco.mupl_pt = m1P_Reco->Pt();
      Jpsi_Reco.mumi_pt = m2P_Reco->Pt();
      Jpsi_Reco.mupl_eta = m1P_Reco->Eta();
      Jpsi_Reco.mumi_eta = m2P_Reco->Eta();
      
      // trigger check for - DoubleMuOpen_v1
      if ( ! (Jpsi_Reco.Reco_QQ_trig&1)==1 ) continue; // Trigger efficiency
      
      // Triggered events 
      h1D_RecoDimu_y_bin[1]->Fill(Jpsi_Reco.theRapidity);
      if (-2.4<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<-0.47) {
	h1D_RecoDimu_pt_bin[1][kFW]->Fill(Jpsi_Reco.thePt);
      }
      if (-0.47<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<1.46) {
	h1D_RecoDimu_pt_bin[1][kBW]->Fill(Jpsi_Reco.thePt);
      }
      
      h1D_Reco_M->Fill(Jpsi_Reco.theMass);

      // After trigger
      if ( Jpsi_Reco.theSign !=0 )  
	continue;
      if ( !kineCut(Jpsi_Reco.mupl_pt, Jpsi_Reco.mupl_eta, Jpsi_Reco.mupl_p))
	continue;

      h1D_RecoDimu_y_bin[2]->Fill(Jpsi_Reco.theRapidity);
      if (-2.4<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<-0.47) {
	h1D_RecoDimu_pt_bin[2][kFW]->Fill(Jpsi_Reco.thePt);
      }
      if (-0.47<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<1.46) {
	h1D_RecoDimu_pt_bin[2][kBW]->Fill(Jpsi_Reco.thePt);
      }

      
      if (!kineCut(Jpsi_Reco.mumi_pt, Jpsi_Reco.mumi_eta, Jpsi_Reco.mumi_p))
	continue;

      h1D_RecoDimu_y_bin[3]->Fill(Jpsi_Reco.theRapidity);
      if (-2.4<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<-0.47) {
	h1D_RecoDimu_pt_bin[3][kFW]->Fill(Jpsi_Reco.thePt);
      }
      if (-0.47<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<1.46) {
	h1D_RecoDimu_pt_bin[3][kBW]->Fill(Jpsi_Reco.thePt);
      }


      if ( !( (ptbinmin<=Jpsi_Reco.thePt) && (Jpsi_Reco.thePt <ptbinmax)) )
	continue;
      if ( !( (rapbinmin<=Jpsi_Reco.theRapidity) && (Jpsi_Reco.theRapidity <rapbinmax) ) )
	continue;
      
      h1D_RecoDimu_y_bin[4]->Fill(Jpsi_Reco.theRapidity);
      if (-2.4<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<-0.47) {
	h1D_RecoDimu_pt_bin[4][kFW]->Fill(Jpsi_Reco.thePt);
      }
      if (-0.47<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<1.46) {
	h1D_RecoDimu_pt_bin[4][kBW]->Fill(Jpsi_Reco.thePt);
      }

      h1D_Num_M->Fill(Jpsi_Reco.theMass);
      if (!(massCut2(Jpsi_Reco.theMass)))
	continue;
      

      h1D_RecoDimu_y_bin[5]->Fill(Jpsi_Reco.theRapidity);
      if (-2.4<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<-0.47) {
	h1D_RecoDimu_pt_bin[5][kFW]->Fill(Jpsi_Reco.thePt);
      }
      if (-0.47<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<1.46) {
	h1D_RecoDimu_pt_bin[5][kBW]->Fill(Jpsi_Reco.thePt);
      }
      
      h1D_RecoDimu_y_bin[0]->Fill(Jpsi_Reco.theRapidity);
      
      if (-2.4<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<-0.47) {
	h1D_RecoDimu_pt_bin[0][kFW]->Fill(Jpsi_Reco.thePt);
      }
      if (-0.47<=Jpsi_Reco.theRapidity && Jpsi_Reco.theRapidity<1.46) {
	h1D_RecoDimu_pt_bin[0][kBW]->Fill(Jpsi_Reco.thePt);
      }
      
      if ( Reco_QQ_size == 1 ) {
	recoy = Jpsi_Reco.theRapidity; 
	recopt  = Jpsi_Reco.thePt;
      }
      
    } //end of Reco_QQ_size loop
    
    if ( (recopt > 0)  && (genpt > 0) )
      hdPtEta->Fill(  recopt-genpt, recoy-geny);
    
  } //end of event loop
  
  // (Num/Den) to get efficiency (B : binomial error)
  
  h1D_Acc_y_bin->Divide(h1D_Den_y_bin,h1D_All_y_bin,1,1,"B");
  h1D_Acc_pt_bin[kFW]->Divide(h1D_Den_pt_bin[kFW],h1D_All_pt_bin[kFW],1,1,"B");
  h1D_Acc_pt_bin[kBW]->Divide(h1D_Den_pt_bin[kBW],h1D_All_pt_bin[kBW],1,1,"B");

  for ( int ij=0 ; ij<=5 ; ij++) { 
    h1D_EffDimu_y_bin[ij]->Divide(h1D_RecoDimu_y_bin[ij],h1D_Den_y_bin,1,1,"B");
    for ( int ifw = 0; ifw<=1 ;ifw++) { 
      h1D_EffDimu_pt_bin[ij][ifw]->Divide(h1D_RecoDimu_pt_bin[ij][ifw],h1D_Den_pt_bin[ifw],1,1,"B");
      
    }
  }
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  // Save the data!
  
  
  // binning 1 y dependence
  //	streamsize prec = cout.precision(); // for setprecision (like %.3f in C)
  std::ofstream outFile_yDep(Form("Eff_yDep_%s.out",datestring),std::ios::app);
  Double_t eff_yDep=0;
  Double_t effErr_yDep=0;
  //	Int_t effBin_yDep = h1D_EffDimu_y_bin[0]->GetNbinsX();
  cout << "*** Get Efficiency value (yDep) ***" << endl;
  //	cout << "nYBins = " << nYBins << ", effBin_yDep = "<< effBin_yDep <<" should be same!"<< endl;
  for (Int_t i=0; i< nYBins; i++) {
    cout << "from y=" << yBinsArr[i] << ", to y="<< yBinsArr[i+1] <<endl;
    //		cout << "from pt=" << ptBinsArr1[0] <<", to pt="<< ptBinsArr1[1]<<endl;
    eff_yDep = h1D_EffDimu_y_bin[0]->GetBinContent(i+1);
    effErr_yDep = h1D_EffDimu_y_bin[0]->GetBinError(i+1);
    cout << std::fixed; //// fix flating point notation for setprecision (like %.3f in C)
    cout << "eff= " << setprecision(3) <<eff_yDep << ", effErr = "<< setprecision(3)<<effErr_yDep<<endl;
    outFile_yDep << setprecision(3) <<  eff_yDep << "\t" << setprecision(3) << effErr_yDep << endl;
  }
  outFile_yDep.close();
  
  // binning 2 pt dependence
  //	streamsize prec = cout.precision(); // for setprecision (like %.3f in C)
  std::ofstream outFile_ptDep(Form("Eff_ptDep_%s.out",datestring),std::ios::app);
  Double_t eff_ptDep=0;
  Double_t effErr_ptDep=0;
  cout << "*** Get Efficiency value (ptDep) ***" << endl;
  // FW
  for (Int_t i=0; i< nPtBins; i++) {
    cout << "from pt=" << ptBinsArr[i] <<", to pt="<< ptBinsArr[i+1]<<endl;
    cout << "from y=" << yBinsArr[0] << ", to y="<< yBinsArr[1] <<endl;
    eff_ptDep = h1D_EffDimu_pt_bin[0][kFW]->GetBinContent(i+1);
    effErr_ptDep = h1D_EffDimu_pt_bin[0][kFW]->GetBinError(i+1);
    cout << std::fixed; //// fix flating point notation for setprecision (like %.3f in C)
    cout << "eff= " << setprecision(3) <<eff_ptDep << ", effErr = "<< setprecision(3)<<effErr_ptDep<<endl;
    outFile_ptDep << setprecision(3) <<  eff_ptDep << "\t" << setprecision(3) << effErr_ptDep << endl;
  }
  // BW
  for (Int_t i=0; i< nPtBins; i++) {
    cout << "from pt=" << ptBinsArr[i] <<", to pt="<< ptBinsArr[i+1]<<endl;
    cout << "from y=" << yBinsArr[1] << ", to y="<< yBinsArr[2] <<endl;
    eff_ptDep = h1D_EffDimu_pt_bin[0][kBW]->GetBinContent(i+1);
    effErr_ptDep = h1D_EffDimu_pt_bin[0][kBW]->GetBinError(i+1);
    cout << std::fixed; //// fix flating point notation for setprecision (like %.3f in C)
    cout << "eff= " << setprecision(3) <<eff_ptDep << ", effErr = "<< setprecision(3)<<effErr_ptDep<<endl;
    outFile_ptDep << setprecision(3) << eff_ptDep << "\t" << setprecision(3) << effErr_ptDep << endl;
  }
  outFile_ptDep.close();
  
  // --- save03 : save as a root file
  
  TFile *outFile = new TFile(Form("EffAna_%s.root",datestring),"RECREATE");
  std::cout << "datestring: " << datestring << std::endl;
	outFile->cd();

	
	// Acceptance 
	h1D_All_y_bin->Write();
	h1D_Den_y_bin->Write();
	h1D_All_pt_bin[kFW]->Write();
	h1D_All_pt_bin[kBW]->Write();
	h1D_Den_pt_bin[kFW]->Write();
	h1D_Den_pt_bin[kBW]->Write();
	h1D_Acc_y_bin->Write();
	h1D_Acc_pt_bin[kFW]->Write();
	h1D_Acc_pt_bin[kBW]->Write();

	// effiiciency
	for ( int ij=0; ij<=5 ; ij++) { 
	  h1D_RecoDimu_y_bin[ij]->Write();
	  h1D_EffDimu_y_bin[ij]->Write();
	  h1D_RecoDimu_pt_bin[ij][kFW]->Write();
	  h1D_EffDimu_pt_bin[ij][kFW]->Write();
	  h1D_RecoDimu_pt_bin[ij][kBW]->Write();
	  h1D_EffDimu_pt_bin[ij][kBW]->Write();
	}
	// mass
	h1D_Reco_M->Write();
	h1D_Num_M->Write();
	h1D_Gen_M->Write();
	h1D_Den_M->Write();

	hdPtEta->Write();

	outFile->Close();

} // end of main func

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

bool kineCut(double muPt, double muEta, double muP) { 

	if(muEta<-2.4 || muEta>=2.4)
	{return false;}
	else if(TMath::Abs(muEta)>=2.2 && TMath::Abs(muEta)<2.4 && muPt<=0.8)
	{return false;}
	else if(TMath::Abs(muEta)>=1.3 && TMath::Abs(muEta)<2.2 && muP<=2.9)
	{return false;}
	else if(TMath::Abs(muEta)<1.3 && muPt<=3.3)
	{return false;}

	else {return true;}

}

bool massCut1(double lv_dimu_mass) {
	if(lv_dimu_mass < 2.6 || lv_dimu_mass >= 3.5)
	{return false;}
	else {return true;}
}

bool massCut2(double lv_dimu_mass) {
	if(lv_dimu_mass < 2.95 || lv_dimu_mass >= 3.25)
	{return false;}
	else {return true;}
}

