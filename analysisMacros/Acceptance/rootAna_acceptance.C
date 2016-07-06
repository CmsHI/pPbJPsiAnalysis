#include "SONGKYO.h"

bool kineCut(const TLorentzVector* Mu, int accCutType);
bool massCutWide(double t_dimu_M);

////////////////////////////////////////////////////////////////////////////////  
//// MrapNpt (M rapidity and N pt bins) : (MN) = (89, 83, 63, 62)
//// isPA : 0=pp, 1=PA (Pbp and pPb together)
//// accCutType : 1=oldcut, 2=newcut
//// isPrompt : true=PR, false=NP
////////////////////////////////////////////////////////////////////////////////  

int rootAna_acceptance(int MrapNpt=89, int isPA = 0, int accCutType =2, bool isPrompt=true){
  
  using namespace std;
  int initev = 0;
  int nevt = -1; //all
  //int nevt = 5000;
    
  TString szBinning;
  if (MrapNpt==89)  {szBinning = "8rap9pt"; }
  else if (MrapNpt==83) { szBinning = "8rap3pt"; }
  else if (MrapNpt==63) { szBinning = "6rap3pt"; }
  else if (MrapNpt==62) { szBinning = "6rap2pt"; }
  else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return 0; }
  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="pA";
  else {cout << "select among isPA = 0 or 1 only (pA instead of Pbp or pPb) "<< endl; return 0; }
  TString szAccCut;
  if (accCutType==1) szAccCut="oldcut";
  else if (accCutType==2) szAccCut="newcut";
  else {cout << "select among accCutType = 1 or 2"<< endl; return 0; }  
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  const TString szFinal = Form("%s_%s_%s_%s",szBinning.Data(),szPA.Data(),szPrompt.Data(),szAccCut.Data());
  std::cout << "szFinal: " << szFinal << std::endl;

  TFile* f1;
  if (isPA==0) {  // for pp
    if (isPrompt){
      f1 = new TFile("/home/storage/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root");
    } else  { 
      f1 = new TFile("/home/storage/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_BJpsiMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root");
    }
  } else { //for Pbp and pPb
    if (isPrompt){
      f1 = new TFile("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/MCJPsiWithFSR_pa_1st_run_STARTHI53_V27_GENONLY.root");
    } else {
      f1 = new TFile("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/MCinclBtoJPsiMuMu_pa_1st_run_STARTHI53_V27_GENONLY.root");
    }
  }
  TTree * myTree;
  if (isPA==0) { myTree = (TTree*)f1->Get("hionia/myTree"); }
  else { myTree = (TTree*)f1->Get("myTree"); }
  
  ////////////////////////////////////////////////////////////////////////////////  
  //// 1) muons (pdg 13, +-pairs, and status==1 already)
  //// 2) J/psi (pdg 443 already)
  Int_t           Gen_QQ_size;
  TClonesArray    *Gen_QQ_4mom;
  TClonesArray    *Gen_QQ_mupl_4mom;
  TClonesArray    *Gen_QQ_mumi_4mom;

  TBranch        *b_Gen_QQ_size;   //!
  TBranch        *b_Gen_QQ_4mom;   //!
  TBranch        *b_Gen_QQ_mupl_4mom;   //!
  TBranch        *b_Gen_QQ_mumi_4mom;   //!

  //// mandatory
  Gen_QQ_4mom = 0;
  Gen_QQ_mupl_4mom = 0;
  Gen_QQ_mumi_4mom = 0;

  myTree->SetBranchAddress("Gen_QQ_size", &Gen_QQ_size, &b_Gen_QQ_size);
  myTree->SetBranchAddress("Gen_QQ_4mom", &Gen_QQ_4mom, &b_Gen_QQ_4mom);
  myTree->SetBranchAddress("Gen_QQ_mupl_4mom", &Gen_QQ_mupl_4mom, &b_Gen_QQ_mupl_4mom);
  myTree->SetBranchAddress("Gen_QQ_mumi_4mom", &Gen_QQ_mumi_4mom, &b_Gen_QQ_mumi_4mom);

  ////////////////////////////////////////////////////////////////////////////////  
  //// Definition of the binning
  int nRap, nPt;
  if (MrapNpt==89) { nRap = 8; nPt = 9; }
  else if (MrapNpt==83) { nRap = 8; nPt = 3; }
  else if (MrapNpt==63) { nRap = 6; nPt = 3;}
  else { nRap = 6; nPt = 2;}
  const int nRapTmp = nRap +1;
  const int nPtTmp = nPt +1;
  //// pt bins
  Double_t ptArr[nPtTmp];
  Double_t ptArr9pt[10] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0};
  Double_t ptArr3pt[4] = {5.0, 6.5, 10.0, 30.0};
  Double_t ptArr2pt[3] = {5.0, 6.5, 30.0};
  for (int ipt=0; ipt<nPtTmp; ipt++){
    if (MrapNpt==89) {ptArr[ipt]=ptArr9pt[ipt];}
    else if (MrapNpt==83 || MrapNpt==63) {ptArr[ipt]=ptArr3pt[ipt];}
    else {ptArr[ipt]=ptArr2pt[ipt];}
  }
  //// y bins : set to 1st run (For 2nd run, will be automatically changed later)
  Double_t rapArr[nRapTmp];
  Double_t rapArr8rapPA[9] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4};
  Double_t rapArr6rapPA[7] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46};
  Double_t rapArr8rapPP[9] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4};
  Double_t rapArr6rapPP[7] = {-1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};
  for (int iy=0; iy<nRapTmp; iy++){
    if (isPA==0) {
      if (MrapNpt==89 || MrapNpt==83) {rapArr[iy]=rapArr8rapPP[iy];}
      else {rapArr[iy]=rapArr6rapPP[iy];}
    } else {
      if (MrapNpt==89 || MrapNpt==83) {rapArr[iy]=rapArr8rapPA[iy];}
      else {rapArr[iy]=rapArr6rapPA[iy];}
    }
  }
  //// y bins for 2nd run (only for PA)
  Double_t rapArr2nd[nRap+1];
  for (Int_t i=0; i<nRap+1; i++) {
     rapArr2nd[i] = -1*rapArr[nRap-i];
    cout <<"rapArr["<<i<<"] = " <<rapArr[i]<<endl;
    cout <<"rapArr2nd["<<i<<"] = " <<rapArr2nd[i]<<endl;
  }
  const Int_t nRap2nd = sizeof(rapArr2nd)/sizeof(double)-1;
  if (nRap != nRap2nd) { cout << " *** Error!! nRap should be equal to nRap2nd"<<endl; return 0; };

  ////////////////////////////////////////////////////////////////////////////////  
  //// define 2D hist
  TH2D *h2D_Den_pt_y = new TH2D("h2D_Den_pt_y",";y_{lab};p_{T} (GeV);",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Num_pt_y = new TH2D("h2D_Num_pt_y",";y_{lab};p_{T} (GeV);",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Acc_pt_y = new TH2D("h2D_Acc_pt_y",";y_{lab};p_{T} (GeV);",nRap,rapArr,nPt,ptArr);
  h2D_Den_pt_y->Sumw2();
  h2D_Num_pt_y->Sumw2();
  h2D_Acc_pt_y->Sumw2();
  ////fine-grained bins for plots in AN (for 8rap9pt only)
  TH2D *h2D_Den_pt_y_fine = new TH2D("h2D_Den_pt_y_fine",";y_{lab};p_{T} (GeV);",100,-2.4,2.4,100,0.,30.);
  TH2D *h2D_Num_pt_y_fine = new TH2D("h2D_Num_pt_y_fine",";y_{lab};p_{T} (GeV);",100,-2.4,2.4,100,0.,30.);
  TH2D *h2D_Acc_pt_y_fine = new TH2D("h2D_Acc_pt_y_fine",";y_{lab};p_{T} (GeV);",100,-2.4,2.4,100,0.,30.);
  h2D_Den_pt_y_fine->Sumw2();
  h2D_Num_pt_y_fine->Sumw2();
  h2D_Acc_pt_y_fine->Sumw2();

  TLorentzVector* jpsi_GEN = new TLorentzVector; //gen jpsi
  TLorentzVector* dimu_GEN = new TLorentzVector; //gen dimuon (not used)
  TLorentzVector* mupl_GEN = new TLorentzVector;
  TLorentzVector* mumi_GEN = new TLorentzVector;
  
  ////////////////////////////////////////////////////////////////////////////////  
  cout << "Entries of tree : " << myTree->GetEntries() << endl;
  if(nevt == -1) { nevt = myTree->GetEntries(); }
  
  //// event loop start!
  for(int iev=0; iev<nevt; iev++){
    if(iev%100000==0) cout << ">>>>> EVENT " << iev << " / " << myTree->GetEntries() << " ("<<(int)(100.*iev/myTree->GetEntries()) << "%)" << endl;
    myTree->GetEntry(iev);
  
    //// Gen_QQ_size loop
    if (Gen_QQ_size ==0) continue;
    for (Int_t igqq=0; igqq<Gen_QQ_size; ++igqq) {
      
      mupl_GEN = (TLorentzVector*) Gen_QQ_mupl_4mom->At(igqq);
      mumi_GEN = (TLorentzVector*) Gen_QQ_mumi_4mom->At(igqq);
      jpsi_GEN = (TLorentzVector*) Gen_QQ_4mom->At(igqq);
      *dimu_GEN = *mupl_GEN +  *mumi_GEN;

      //// 1) Denominator
      h2D_Den_pt_y->Fill(jpsi_GEN->Rapidity(),jpsi_GEN->Pt());
      if (MrapNpt==89) h2D_Den_pt_y_fine->Fill(jpsi_GEN->Rapidity(),jpsi_GEN->Pt());
      
      //// 2) Numerator
      if ( 
      //massCutWide(dimu_GEN->M()) &&
      massCutWide(jpsi_GEN->M()) &&
      kineCut(mupl_GEN, accCutType) &&
      kineCut(mumi_GEN, accCutType)) {
        h2D_Num_pt_y->Fill(jpsi_GEN->Rapidity(),jpsi_GEN->Pt());
        if (MrapNpt==89) { h2D_Num_pt_y_fine->Fill(jpsi_GEN->Rapidity(),jpsi_GEN->Pt()); }
      }
    } //end of Gen_QQ_size loop
  }// end of event loop

  //// (Num/Den) to get acceptance (B : binomial error)
  h2D_Acc_pt_y->Divide(h2D_Num_pt_y,h2D_Den_pt_y,1,1,"B");
  if (MrapNpt==89) h2D_Acc_pt_y_fine->Divide(h2D_Num_pt_y_fine,h2D_Den_pt_y_fine,1,1,"B");

  ////////////////////////////////////////////////////////////////////////////////////////////////////  
  //// rapidity reverse, for 2nd run hist. (only for pPb)
  TH2D *h2D_Den_pt_y_pPb = new TH2D("h2D_Den_pt_y_pPb","",nRap2nd,rapArr2nd,nPt,ptArr);
  TH2D *h2D_Num_pt_y_pPb = new TH2D("h2D_Num_pt_y_pPb","",nRap2nd,rapArr2nd,nPt,ptArr);
  TH2D *h2D_Acc_pt_y_pPb = new TH2D("h2D_Acc_pt_y_pPb","",nRap2nd,rapArr2nd,nPt,ptArr);
  h2D_Den_pt_y_pPb->Sumw2();
  h2D_Num_pt_y_pPb->Sumw2();
  h2D_Acc_pt_y_pPb->Sumw2();

  int tmpbinDenPbp, tmpbinNumPbp, tmpbinAccPbp;
  int tmpbinDenpPb, tmpbinNumpPb, tmpbinAccpPb;
  float tmpDenVal, tmpDenErr, tmpNumVal, tmpNumErr, tmpAccVal, tmpAccErr;

  if (isPA==1) {  
    for (Int_t iy=0; iy<nRap; iy++){
      for (Int_t ipt=0; ipt<nPt; ipt++) {
        //// Den
        tmpbinDenPbp = h2D_Den_pt_y->FindBin((rapArr[iy]+rapArr[iy+1])/2, (ptArr[ipt]+ptArr[ipt+1])/2);
        tmpDenVal = h2D_Den_pt_y->GetBinContent(tmpbinDenPbp);
        tmpDenErr = h2D_Den_pt_y->GetBinError(tmpbinDenPbp);
        tmpbinDenpPb = h2D_Den_pt_y_pPb->FindBin((rapArr2nd[nRap-iy-1]+rapArr2nd[nRap-iy])/2,(ptArr[ipt]+ptArr[ipt+1])/2);
        h2D_Den_pt_y_pPb->SetBinContent(tmpbinDenpPb,tmpDenVal);
        h2D_Den_pt_y_pPb->SetBinError(tmpbinDenpPb,tmpDenErr);
        //// Num  
        tmpbinNumPbp = h2D_Num_pt_y->FindBin((rapArr[iy]+rapArr[iy+1])/2, (ptArr[ipt]+ptArr[ipt+1])/2);
        tmpNumVal = h2D_Num_pt_y->GetBinContent(tmpbinNumPbp);
        tmpNumErr = h2D_Num_pt_y->GetBinError(tmpbinNumPbp);
        tmpbinNumpPb = h2D_Num_pt_y_pPb->FindBin((rapArr2nd[nRap-iy-1]+rapArr2nd[nRap-iy])/2,(ptArr[ipt]+ptArr[ipt+1])/2);
        h2D_Num_pt_y_pPb->SetBinContent(tmpbinNumpPb,tmpNumVal);
        h2D_Num_pt_y_pPb->SetBinError(tmpbinNumpPb,tmpNumErr);
        //// Acc  
        tmpbinAccPbp = h2D_Acc_pt_y->FindBin((rapArr[iy]+rapArr[iy+1])/2, (ptArr[ipt]+ptArr[ipt+1])/2);
        tmpAccVal = h2D_Acc_pt_y->GetBinContent(tmpbinAccPbp);
        tmpAccErr = h2D_Acc_pt_y->GetBinError(tmpbinAccPbp);
        tmpbinAccpPb = h2D_Acc_pt_y_pPb->FindBin((rapArr2nd[nRap-iy-1]+rapArr2nd[nRap-iy])/2,(ptArr[ipt]+ptArr[ipt+1])/2);
        h2D_Acc_pt_y_pPb->SetBinContent(tmpbinAccpPb,tmpAccVal);
        h2D_Acc_pt_y_pPb->SetBinError(tmpbinAccpPb,tmpAccErr);
      }
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////  
  //// Save the data as an root file
  if (isPA==1) {
    h2D_Den_pt_y->SetName("h2D_Den_pt_y_Pbp");
    h2D_Num_pt_y->SetName("h2D_Num_pt_y_Pbp");
    h2D_Acc_pt_y->SetName("h2D_Acc_pt_y_Pbp");
  }

  TFile *outFile = new TFile(Form("AccAna_%s.root",szFinal.Data()),"RECREATE");
  outFile->cd();
  h2D_Den_pt_y->Write();
  h2D_Num_pt_y->Write();
  h2D_Acc_pt_y->Write();
  if (isPA==1) {
    h2D_Den_pt_y_pPb->Write();
    h2D_Num_pt_y_pPb->Write();
    h2D_Acc_pt_y_pPb->Write();
  }
  if (MrapNpt==89) {
    h2D_Den_pt_y_fine->Write();
    h2D_Num_pt_y_fine->Write();
    h2D_Acc_pt_y_fine->Write();
  }
  outFile->Close();
  std::cout << "szFinal: " << szFinal << std::endl;
  return 0;
} // end of main func

////////////////////////////////////////////////////////////////////////////////////////////////////  
//// sub-routines function 

bool kineCut(const TLorentzVector* Mu, int accCutType){
  if (accCutType == 1) { //old cut
    return (
      (TMath::Abs(Mu->Eta())<1.3 && Mu->Pt()>=3.3) ||
      (1.3<=TMath::Abs(Mu->Eta()) && TMath::Abs(Mu->Eta())<2.2 && Mu->P()>=2.9) ||
      (2.2<=TMath::Abs(Mu->Eta()) && TMath::Abs(Mu->Eta())<2.4 && Mu->Pt()>=0.8)
    );
  }
  else if (accCutType == 2) { // new cut
    return (
      (TMath::Abs(Mu->Eta())<1.2 && Mu->Pt()>=3.3) ||
      (1.2<=TMath::Abs(Mu->Eta()) && TMath::Abs(Mu->Eta())<2.1 && Mu->Pt()>=3.93-1.11*TMath::Abs(Mu->Eta())) ||
      (2.1<=TMath::Abs(Mu->Eta()) && TMath::Abs(Mu->Eta())<2.4 && Mu->Pt()>=1.3)
    );
  }
  else if (accCutType == 3) { //pre-loose cut A on pA data
    return (
      (TMath::Abs(Mu->Eta())<1.2 && Mu->Pt()>=3.3) ||
      (1.2<=TMath::Abs(Mu->Eta()) && TMath::Abs(Mu->Eta())<1.6 && Mu->Pt() >= 6.8-3.5*TMath::Abs(Mu->Eta())) ||
      (1.6<=TMath::Abs(Mu->Eta()) && TMath::Abs(Mu->Eta())<2.1 && Mu->Pt() >= 2.48-0.8*TMath::Abs(Mu->Eta())) ||
      (2.1<=TMath::Abs(Mu->Eta()) && TMath::Abs(Mu->Eta())<2.4 && Mu->Pt() >= 0.8)
    );
  }
  else if (accCutType == 4) { // 2015 PbPb GlbTrk muons
    return (
      (TMath::Abs(Mu->Eta())<1.2 && Mu->Pt() >=3.5) ||
      (1.2<=TMath::Abs(Mu->Eta()) && TMath::Abs(Mu->Eta())<2.1 && Mu->Pt()>=5.77-(1.89)*TMath::Abs(Mu->Eta())) ||
      (2.1<=TMath::Abs(Mu->Eta()) && TMath::Abs(Mu->Eta())<2.4 && Mu->Pt()>=1.8)
    );
  }
  else {
    return false;
  }
}

bool massCutWide(double t_dimu_M) {
  return ( 2.6 <  t_dimu_M && t_dimu_M < 3.5 );
}

