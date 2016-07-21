#include "../SONGKYO.h"
#include <TF1.h>

//bool kineCut(double muPt, double muEta, double muP);
bool kineCut(const TLorentzVector* Mu, int accCutType);
bool massCutWide(double lv_dimu_mass);
bool massCutNarrow(double lv_dimu_mass);
void formRapStr(Double_t min, Double_t max, string* arr);
void formStr(Double_t min, Double_t max, string* arr);
float getSF(float mupt1=0, float mueta1=0, float mupt2=0, float mueta2=0, int ispa=0, int acctype=2); 

//read TNP plots for useSF

//// pPb, old acc cut (PAS)
TFile* fTnp_pa_old = new TFile("../Efficiency/tagAndProbe/tnpRate_nominal_fitRatio_5etaBin.root"); //priv (eta4-5 tagpt5)
TF1* hTnp_pa_old_eta1 = (TF1*)fTnp_pa_old->Get("ferrScale_ieta1");
TF1* hTnp_pa_old_eta2 = (TF1*)fTnp_pa_old->Get("ferrScale_ieta2");
TF1* hTnp_pa_old_eta3 = (TF1*)fTnp_pa_old->Get("ferrScale_ieta3");
TF1* hTnp_pa_old_eta4 = (TF1*)fTnp_pa_old->Get("ferrScale_ieta4");
TF1* hTnp_pa_old_eta5 = (TF1*)fTnp_pa_old->Get("ferrScale_ieta5");

//// pPb, new acc cut
//TFile* fTnp_pa_new = new TFile("../Efficiency/tagAndProbe/output_official_5eta_cutG_all_nominal_v1_20160516.root"); // 20160516 (off8M tagpt3)
//TFile* fTnp_pa_new = new TFile("../Efficiency/tagAndProbe/output_official_5eta_cutG_all_nominal_v2_20160621.root"); // 20160621 (off8M, eta4-5 tagpt5)
TFile* fTnp_pa_new = new TFile("../Efficiency/tagAndProbe/output_official_5eta_cutG_all_nominal_v3.root"); // 20160626 (off8M, tagpt5)
TF1* hTnp_pa_new_eta1 = (TF1*)fTnp_pa_new->Get("func_1");
TF1* hTnp_pa_new_eta2 = (TF1*)fTnp_pa_new->Get("func_2");
TF1* hTnp_pa_new_eta3 = (TF1*)fTnp_pa_new->Get("func_3");
TF1* hTnp_pa_new_eta4 = (TF1*)fTnp_pa_new->Get("func_4");
TF1* hTnp_pa_new_eta5 = (TF1*)fTnp_pa_new->Get("func_5");

//// pp new TNP
//TFile* fTnp_pp_new = new TFile("../Efficiency/tagAndProbe/output_pp_5eta_cutG_all_nominal_v5.root"); //20160620 (off tagpt3)
//TFile* fTnp_pp_new = new TFile("../Efficiency/tagAndProbe/output_pp_5eta_cutG_all_nominal_v6.root"); //20160621 (off, eta4-5 tagpt5)
TFile* fTnp_pp_new = new TFile("../Efficiency/tagAndProbe/output_pp_5eta_cutG_all_nominal_v7.root"); //20160626 (off, tagpt5)
TF1* hTnp_pp_new_eta1 = (TF1*)fTnp_pp_new->Get("func_1");
TF1* hTnp_pp_new_eta2 = (TF1*)fTnp_pp_new->Get("func_2");
TF1* hTnp_pp_new_eta3 = (TF1*)fTnp_pp_new->Get("func_3");
TF1* hTnp_pp_new_eta4 = (TF1*)fTnp_pp_new->Get("func_4");
TF1* hTnp_pp_new_eta5 = (TF1*)fTnp_pp_new->Get("func_5");

////////////////////////////////////////////////////////////////////////////////  
////// MrapNpt (M rapidity and N pt bins) : (MN) = (89, 83, 63, 62)
////// isPA : 0=pp, 1=Pbp, 2=pPb
////// accCutType : 1=oldcut, 2=newcut
////// isPrompt : true=PR, false=NP
////// useZvtxWeight : true=ZvteWeight, false=no
////// useSF : true=useTNPSF, false=no
//////////////////////////////////////////////////////////////////////////////////  

int rootAna_efficiency_ptWeight(int MrapNpt = 89, int isPA =1, int accCutType = 2, bool isPrompt = true, bool useZvtxWeight =true, bool useSF=true)
{
  using namespace std;
  int initev =0;
  int nevt;
  if (!isPrompt) nevt = 8900000; 
  else nevt = -1; //all
  //int nevt = 100000;
  //int nevt = 1000;
  
  TString szBinning;
  if (MrapNpt==89)  {szBinning = "8rap9pt"; }
  else if (MrapNpt==83) { szBinning = "8rap3pt"; }
  else if (MrapNpt==63) { szBinning = "6rap3pt"; }
  else if (MrapNpt==62) { szBinning = "6rap2pt"; }
  else {cout << "select among MrapNpt = 89, 83, 63, or 62"<< endl; return 0; }
  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="Pbp";
  else if (isPA==2) szPA="pPb";
  else {cout << "select among isPA = 0, 1, or 2"<< endl; return 0; }
  TString szAccCut;
  if (accCutType==1) szAccCut="oldcut";
  else if (accCutType==2) szAccCut="newcut";
  else {cout << "select among accCutType = 0 or 1"<< endl; return 0; }
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP";
  const TString szFinal = Form("%s_%s_%s_%s",szBinning.Data(),szPA.Data(),szPrompt.Data(),szAccCut.Data());
  std::cout << "szFinal: " << szFinal << std::endl;

  if (isPA==0 && accCutType==1 && useSF) {cout <<"No oldcut TNP for pp"<<endl; return 0; }

  //////// read zVtx functions for weight : ratio = (Data)/(pythia)
  TFile* fZvtx;
  if (accCutType==1) { fZvtx = new TFile(Form("../Efficiency/zVtx201604/zVtx_G1_%s_isPrompt%d_isNewAccCut0_isOfficial1.root",szPA.Data(), (int)isPrompt));
  } else { fZvtx = new TFile(Form("../Efficiency/zVtx201604/zVtx_G1_%s_isPrompt%d_isNewAccCut1_isOfficial1.root",szPA.Data(), (int)isPrompt)); }
  cout << "fZvtx = " << fZvtx << endl;
  TF1* gRatio = (TF1*)fZvtx->Get("gRatio");
  
  TFile *f1;
  ////// read-in HiOniaTrees
  if (isPA==0){
    if (isPrompt) {
      f1 = new TFile("/home/storage/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root"); //official
      //f1 = new TFile("/home/songkyo/kyo/ppDataSample/EfficiencySamplePrivate/OniaTree_JpsiMM_pp5p02TeV_TuneCUETP8M1_Trk_noCUT.root"); //private
    }
    else {
      f1 = new TFile("/home/storage/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_BJpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root"); //official
      //f1 = new TFile("/home/songkyo/kyo/ppDataSample/EfficiencySamplePrivate/OniaTree_BJpsiMM_pp5p02TeV_TuneCUETP8M1_Trk_noCUT.root"); //private
    }
  }
  else if (isPA==1){
    if (isPrompt) {
      f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCJPsiWithFSR_pa_1st_run_STARTHI53_V27_ext1_nocut.root"); // official 8M
      //f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/tot_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_noAccCut_sglTrig_genMatch-v2.root"); // private
    }
    else {
      f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCinclBtoJPsiMuMu_pa_1st_run_STARTHI53_V27_ext1_nocut.root"); // official 8M
      //f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/tot_B2Jpsi_PYTHIAboosted_1st_STARTHI53_V27_noAccCut_sglTrig_genMatch-v2.root"); // private
    }
  }
  else {
    if (isPrompt) {
      f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCJPsiWithFSR_pa_2nd_run_STARTHI53_V27_ext1_nocut.root"); // official 8M
      //f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/tot_PromptJpsi_PYTHIAboosted_2nd_STARTHI53_V27_noAccCut_sglTrig_genMatch-v2.root"); // private
    }
    else {
      f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCinclBtoJPsiMuMu_pa_2nd_run_STARTHI53_V27_ext1_nocut.root"); // official 8M
      //f1 = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/tot_B2Jpsi_PYTHIAboosted_2nd_STARTHI53_V27_noAccCut_sglTrig_genMatch-v2.root"); // private
    }
  }
  cout << "f1 = " << f1->GetName() <<endl;

  #if 0
  ///// read in Ctau error range file
  TString szDirName = Form("fitRes_%s",szBinning.Data());
  std::ifstream fctau(Form("../Efficiency/%s/summary_%s/fit_ctauErrorRange",szDirName,szPA.Data()),std::ios::in);
  if(!fctau.is_open()) { cout << "Warning : can NOT open the fit_ctauErrorRange file!"<<endl; }
  #endif

  ///////////////////////////////////////////////////
  //////// Definition of binning
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
  Double_t rapArrTmp[nRapTmp];
  Double_t rapArr8rapPA[9] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4};
  Double_t rapArr6rapPA[7] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46};
  Double_t rapArr8rapPP[9] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4};
  Double_t rapArr6rapPP[7] = {-1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};
  for (int iy=0; iy<nRapTmp; iy++){
    if (isPA==0) {
      if (MrapNpt==89 || MrapNpt==83) {rapArrTmp[iy]=rapArr8rapPP[iy];}
      else {rapArrTmp[iy]=rapArr6rapPP[iy];}
    } else {
      if (MrapNpt==89 || MrapNpt==83) {rapArrTmp[iy]=rapArr8rapPA[iy];}
      else {rapArrTmp[iy]=rapArr6rapPA[iy];}
    }
  } 

  //Double_t rapArr[nRapTmp] = {};
  Double_t rapArr[nRapTmp];
  if (isPA==2){
    //// change "y_lab" from Pbp to pPb
    for (Int_t i=0; i<nRapTmp; i++) {
      rapArr[i] = -1*rapArrTmp[nRapTmp-1-i]; 
      cout <<"rapArr["<<i<<"] = " <<rapArr[i]<<endl;
    }
  }
  else {
    for (Int_t i=0; i<nRapTmp; i++) {
      rapArr[i] = rapArrTmp[i]; 
      cout <<"rapArr["<<i<<"] = " <<rapArr[i]<<endl;
    }
  }

  // --- Et bin
  Double_t etBinsArr[] = {0.0, 120.0};
  const Int_t nEt = sizeof(etBinsArr)/sizeof(double)-1;
  cout << "nEt=" << nEt << endl;
  
  if (nRap==6 && isPA==0) { cout << " *** Error!! nRap==6 is not used for pp"<<endl; return 0; };

  ////////////////////////////////////////////////////////////////////////////////  
  //// ptWeight input file (Data/MC dNdPt)
  //Double_t lowPt_8rap_pA[] = {2.0, 4.0, 6.5, 6.5, 6.5, 5.0, 4.0, 2.0};
  //Double_t lowPt_8rap_pp[] = {2.0, 4.0, 6.5, 6.5, 6.5, 6.5, 4.0, 2.0};
  //Double_t lowPt_6rap[] = {5.0, 6.5, 6.5, 6.5, 6.5, 5.0};
  double ptWeight;
  TFile* inFile;
  if (isPA==0) { inFile= new TFile(Form("dir_fitRatio/%s_fitRatio_%s.root",szPA.Data(),szPrompt.Data()));}
  else { inFile= new TFile(Form("dir_fitRatio/pA_fitRatio_%s.root",szPrompt.Data()));}
  TF1* funct[nRap];
  for(int iy=0;iy<nRap;iy++){
    if (isPA==0) funct[iy]=(TF1*) inFile->Get(Form("funct_%s_%s_%d",szPrompt.Data(),szPA.Data(),iy));
    else funct[iy]=(TF1*) inFile->Get(Form("funct_%s_pA_%d",szPrompt.Data(),iy));
  }


  #if 0
  // setting for string bin array for checking ctau error range
  const int nrap = nRap;
  const int npt = nPt;
  string yrange[nrap]; //1st run string
  string ptrange[npt];
  string etrange[nEt];
  for (Int_t i=0; i<nrap; i++){ 
    formRapStr(rapArr[i], rapArr[i+1], &yrange[i]);
    cout << "yrange["<<i<<"] = "<< yrange[i].c_str() << endl;
  }
  for (Int_t i=0; i<npt; i++){  
    formStr(ptArr[i], ptArr[i+1], &ptrange[i]);
    cout << "ptrange["<<i<<"] = "<< ptrange[i].c_str() << endl;
  }
  for (Int_t i=0; i<nEt; i++){  
    formStr(etBinsArr[i], etBinsArr[i+1], &etrange[i]);
    cout << "etrange["<<i<<"] = "<< etrange[i].c_str() << endl;
  }
  #endif

  ///////////////////////////////////////////////////////////////////////////////////////////////
  /////// read-in tree
  TTree * myTree;
  if (isPA==0) { myTree = (TTree*)f1->Get("hionia/myTree"); }
  else { myTree = (TTree*)f1->Get("myTree"); }
  
  Float_t         zVtx;
  ULong64_t       HLTriggers_PP; //jolla zzajeong.. : different type for_pp and for_pa
  Int_t           HLTriggers_PA;
  Int_t           Reco_QQ_size;
  Int_t           Reco_QQ_sign[210];   //[Reco_QQ_size] : 0=+,- / 1=+/+, 2=-/-
  ULong64_t       Reco_QQ_trig_PP[210];   //[Reco_QQ_size] : different type for_pp and for_pa
  Int_t           Reco_QQ_trig_PA[210];   //[Reco_QQ_size] 
  Float_t         Reco_QQ_VtxProb[210];   //[Reco_QQ_size]
//  Float_t         Reco_QQ_ctau[210];
//  Float_t         Reco_QQ_ctauErr[210];
 
  Bool_t          Reco_QQ_mupl_TrkMuArb[210]; 
  Bool_t          Reco_QQ_mumi_TrkMuArb[210];  
  Bool_t          Reco_QQ_mupl_TMOneStaTight[210];  
  Bool_t          Reco_QQ_mumi_TMOneStaTight[210]; 
  Bool_t          Reco_QQ_mupl_highPurity[210];
  Bool_t          Reco_QQ_mumi_highPurity[210]; 
  Int_t           Reco_QQ_mupl_nTrkWMea[210];  
  Int_t           Reco_QQ_mumi_nTrkWMea[210];  
  Int_t           Reco_QQ_mupl_nPixWMea[210];  
  Int_t           Reco_QQ_mumi_nPixWMea[210]; 
  Float_t         Reco_QQ_mupl_dxy[210]; 
  Float_t         Reco_QQ_mumi_dxy[210];
  Float_t         Reco_QQ_mupl_dz[210]; 
  Float_t         Reco_QQ_mumi_dz[210];  
  
  TClonesArray    *Reco_QQ_4mom;
  TClonesArray    *Reco_QQ_mupl_4mom;
  TClonesArray    *Reco_QQ_mumi_4mom;
  
  Int_t           Gen_QQ_size;
  TClonesArray    *Gen_QQ_4mom;
  TClonesArray    *Gen_QQ_mupl_4mom;
  TClonesArray    *Gen_QQ_mumi_4mom;

  TBranch        *b_zVtx;
  TBranch        *b_HLTriggers_PP;
  TBranch        *b_HLTriggers_PA;
  TBranch        *b_Reco_QQ_size;   //!
  TBranch        *b_Reco_QQ_sign;   //!
  TBranch        *b_Reco_QQ_trig_PP;   //!
  TBranch        *b_Reco_QQ_trig_PA;   //!
  TBranch        *b_Reco_QQ_VtxProb;   //!
 // TBranch        *b_Reco_QQ_ctau;   //!
 // TBranch        *b_Reco_QQ_ctauErr;   //!

  TBranch        *b_Reco_QQ_mupl_TrkMuArb;   //!
  TBranch        *b_Reco_QQ_mumi_TrkMuArb;   //!
  TBranch        *b_Reco_QQ_mupl_TMOneStaTight;   //!
  TBranch        *b_Reco_QQ_mumi_TMOneStaTight;   //!
  TBranch        *b_Reco_QQ_mupl_highPurity;   //!
  TBranch        *b_Reco_QQ_mumi_highPurity;   //!
  TBranch        *b_Reco_QQ_mupl_nTrkWMea;   //!
  TBranch        *b_Reco_QQ_mumi_nTrkWMea;   //!
  TBranch        *b_Reco_QQ_mumi_nPixWMea;   //!
  TBranch        *b_Reco_QQ_mupl_nPixWMea;   //!
  TBranch        *b_Reco_QQ_mupl_dxy;   //!
  TBranch        *b_Reco_QQ_mumi_dxy;   //!
  TBranch        *b_Reco_QQ_mupl_dz;   //!
  TBranch        *b_Reco_QQ_mumi_dz;   //!

  TBranch        *b_Reco_QQ_4mom;   //!
  TBranch        *b_Reco_QQ_mupl_4mom;   //!
  TBranch        *b_Reco_QQ_mumi_4mom;   //!

  TBranch        *b_Gen_QQ_size;   //!
  TBranch        *b_Gen_QQ_4mom;   //!
  TBranch        *b_Gen_QQ_mupl_4mom;   //!
  TBranch        *b_Gen_QQ_mumi_4mom;   //!

  Reco_QQ_4mom =0;
  Reco_QQ_mupl_4mom =0;
  Reco_QQ_mumi_4mom =0;
  Gen_QQ_4mom =0;
  Gen_QQ_mupl_4mom =0;
  Gen_QQ_mumi_4mom =0;

  // read-in branches
  myTree->SetBranchAddress("zVtx", &zVtx, &b_zVtx);
  if (isPA==0) myTree->SetBranchAddress("HLTriggers", &HLTriggers_PP, &b_HLTriggers_PP);
  else myTree->SetBranchAddress("HLTriggers", &HLTriggers_PA, &b_HLTriggers_PA);
  myTree->SetBranchAddress("Reco_QQ_size", &Reco_QQ_size, &b_Reco_QQ_size);
  myTree->SetBranchAddress("Reco_QQ_sign", Reco_QQ_sign, &b_Reco_QQ_sign);
  myTree->SetBranchAddress("Reco_QQ_VtxProb", Reco_QQ_VtxProb, &b_Reco_QQ_VtxProb);
  if (isPA==0) myTree->SetBranchAddress("Reco_QQ_trig", Reco_QQ_trig_PP, &b_Reco_QQ_trig_PP);
  else myTree->SetBranchAddress("Reco_QQ_trig", Reco_QQ_trig_PA, &b_Reco_QQ_trig_PA);
//  myTree->SetBranchAddress("Reco_QQ_ctau", Reco_QQ_ctau, &b_Reco_QQ_ctau);
//  myTree->SetBranchAddress("Reco_QQ_ctauErr", Reco_QQ_ctauErr, &b_Reco_QQ_ctauErr);

  myTree->SetBranchAddress("Reco_QQ_mupl_TrkMuArb", Reco_QQ_mupl_TrkMuArb, &b_Reco_QQ_mupl_TrkMuArb);
  myTree->SetBranchAddress("Reco_QQ_mumi_TrkMuArb", Reco_QQ_mumi_TrkMuArb, &b_Reco_QQ_mumi_TrkMuArb);
  myTree->SetBranchAddress("Reco_QQ_mupl_TMOneStaTight", Reco_QQ_mupl_TMOneStaTight, &b_Reco_QQ_mupl_TMOneStaTight);
  myTree->SetBranchAddress("Reco_QQ_mumi_TMOneStaTight", Reco_QQ_mumi_TMOneStaTight, &b_Reco_QQ_mumi_TMOneStaTight);
  if (isPA==0) myTree->SetBranchAddress("Reco_QQ_mupl_highPurity", Reco_QQ_mupl_highPurity, &b_Reco_QQ_mupl_highPurity);
  else myTree->SetBranchAddress("Reco_QQ_mupl_isHighPurity", Reco_QQ_mupl_highPurity, &b_Reco_QQ_mupl_highPurity);
  if (isPA==0) myTree->SetBranchAddress("Reco_QQ_mumi_highPurity", Reco_QQ_mumi_highPurity, &b_Reco_QQ_mumi_highPurity);
  else myTree->SetBranchAddress("Reco_QQ_mumi_isHighPurity", Reco_QQ_mumi_highPurity, &b_Reco_QQ_mumi_highPurity);
  myTree->SetBranchAddress("Reco_QQ_mupl_nTrkWMea", Reco_QQ_mupl_nTrkWMea, &b_Reco_QQ_mupl_nTrkWMea);
  myTree->SetBranchAddress("Reco_QQ_mumi_nTrkWMea", Reco_QQ_mumi_nTrkWMea, &b_Reco_QQ_mumi_nTrkWMea);
  myTree->SetBranchAddress("Reco_QQ_mupl_nPixWMea", Reco_QQ_mupl_nPixWMea, &b_Reco_QQ_mupl_nPixWMea);
  myTree->SetBranchAddress("Reco_QQ_mumi_nPixWMea", Reco_QQ_mumi_nPixWMea, &b_Reco_QQ_mumi_nPixWMea);
  myTree->SetBranchAddress("Reco_QQ_mupl_dxy", Reco_QQ_mupl_dxy, &b_Reco_QQ_mupl_dxy);
  myTree->SetBranchAddress("Reco_QQ_mumi_dxy", Reco_QQ_mumi_dxy, &b_Reco_QQ_mumi_dxy);
  myTree->SetBranchAddress("Reco_QQ_mupl_dz", Reco_QQ_mupl_dz, &b_Reco_QQ_mupl_dz);
  myTree->SetBranchAddress("Reco_QQ_mumi_dz", Reco_QQ_mumi_dz, &b_Reco_QQ_mumi_dz);

  myTree->SetBranchAddress("Reco_QQ_4mom", &Reco_QQ_4mom, &b_Reco_QQ_4mom);
  myTree->SetBranchAddress("Reco_QQ_mupl_4mom", &Reco_QQ_mupl_4mom, &b_Reco_QQ_mupl_4mom);
  myTree->SetBranchAddress("Reco_QQ_mumi_4mom", &Reco_QQ_mumi_4mom, &b_Reco_QQ_mumi_4mom);
  myTree->SetBranchAddress("Gen_QQ_size", &Gen_QQ_size, &b_Gen_QQ_size);
  myTree->SetBranchAddress("Gen_QQ_4mom", &Gen_QQ_4mom, &b_Gen_QQ_4mom);
  myTree->SetBranchAddress("Gen_QQ_mupl_4mom", &Gen_QQ_mupl_4mom, &b_Gen_QQ_mupl_4mom);
  myTree->SetBranchAddress("Gen_QQ_mumi_4mom", &Gen_QQ_mumi_4mom, &b_Gen_QQ_mumi_4mom);

  #if 0
  //////////////////////////////////////////////////////
  ////// read ctauErr txt
  float ctErrmin[nrap][npt];
  float ctErrmax[nrap][npt];
  TH2D *h2D_ctErrmin = new TH2D("h2D_ctErrmin","",nrap,rapArr,npt,ptArr);
  TH2D *h2D_ctErrmax = new TH2D("h2D_ctErrmax","",nrap,rapArr,npt,ptArr);

  string headers;
  getline(fctau, headers); // remove prefix
  getline(fctau, headers); // remove column names
  string rapdum, ptdum, centdum, dphidum, ntrkdum, etdum, errmin, errmax;

  while(!fctau.eof()) {
    fctau >> rapdum >> ptdum >> centdum >> dphidum >> ntrkdum >> etdum >> errmin >> errmax;
    //cout << rapdum << ptdum << centdum << dphidum << ntrkdum << etdum << errmin << errmax << endl;
    for (Int_t iy=0; iy<nrap; iy++){
      for (Int_t ipt=0; ipt<npt; ipt++) {
        if (useCtErrCut) {
          if (!yrange[iy].compare(rapdum) && !ptrange[ipt].compare(ptdum) && !etrange[0].compare(etdum)){
            ctErrmin[iy][ipt] = atof(errmin.c_str());
            ctErrmax[iy][ipt] = atof(errmax.c_str());
          }
        } //end of useCtErrCut
        else {
          ctErrmin[iy][ipt]=-532;
          ctErrmax[iy][ipt]=532;
        }
      } 
    }
  } //end of while

  // put ctErrmin & max values into hist
  int tmpminbin, tmpmaxbin;
  for (Int_t iy=0; iy<nrap; iy++){
    for (Int_t ipt=0; ipt<npt; ipt++) {
      tmpminbin = h2D_ctErrmin->FindBin((rapArr[iy]+rapArr[iy+1])/2, (ptArr[ipt]+ptArr[ipt+1])/2);  
      h2D_ctErrmin->SetBinContent(tmpminbin, ctErrmin[iy][ipt]);
      tmpmaxbin = h2D_ctErrmax->FindBin((rapArr[iy]+rapArr[iy+1])/2, (ptArr[ipt]+ptArr[ipt+1])/2);  
      h2D_ctErrmax->SetBinContent(tmpmaxbin, ctErrmax[iy][ipt]);
    }
  }
  #endif

  ////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////
  //// define 2D hist
  //// 1) not weighted
  TH2D *h2D_Den_pt_y_noPtWeight = new TH2D("h2D_Den_pt_y_noPtWeight","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Num_pt_y_noPtWeight = new TH2D("h2D_Num_pt_y_noPtWeight","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Eff_pt_y_noPtWeight = new TH2D("h2D_Eff_pt_y_noPtWeight","",nRap,rapArr,nPt,ptArr);
  h2D_Den_pt_y_noPtWeight->Sumw2();
  h2D_Num_pt_y_noPtWeight->Sumw2();
  h2D_Eff_pt_y_noPtWeight->Sumw2();
  //// 2) weighted
  TH2D *h2D_Den_pt_y = new TH2D("h2D_Den_pt_y","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Num_pt_y = new TH2D("h2D_Num_pt_y","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Eff_pt_y = new TH2D("h2D_Eff_pt_y","",nRap,rapArr,nPt,ptArr);
  h2D_Den_pt_y->Sumw2();
  h2D_Num_pt_y->Sumw2();
  h2D_Eff_pt_y->Sumw2();
  ////fine-grained bins for plots in AN (for 8rap9pt only)
  TH2D *h2D_Den_pt_y_fine = new TH2D("h2D_Den_pt_y_fine","",100,-2.4,2.4,100,0.,30.);
  TH2D *h2D_Num_pt_y_fine = new TH2D("h2D_Num_pt_y_fine","",100,-2.4,2.4,100,0.,30.);
  TH2D *h2D_Eff_pt_y_fine = new TH2D("h2D_Eff_pt_y_fine","",100,-2.4,2.4,100,0.,30.);
  h2D_Den_pt_y_fine->Sumw2();
  h2D_Num_pt_y_fine->Sumw2();
  h2D_Eff_pt_y_fine->Sumw2();
  TH1D* h1D_zVtx = new TH1D("h1D_zVtx","",60,-30,30);
  h1D_zVtx->Sumw2();
  
  TLorentzVector* dimu_RECO = new TLorentzVector;
  TLorentzVector* mupl_RECO = new TLorentzVector;
  TLorentzVector* mumi_RECO = new TLorentzVector;
  TLorentzVector* jpsi_GEN = new TLorentzVector; // gen jpsi
  TLorentzVector* dimu_GEN = new TLorentzVector; // gen dimuon (not used)
  TLorentzVector* mupl_GEN = new TLorentzVector;
  TLorentzVector* mumi_GEN = new TLorentzVector;


  ////////////////////////////////////////////////////////////////////////////////////////////////
  float theZvtx=531;
  float tnpWeight =1;
  float zWeight =1;
  
  cout << "Entries of tree : " << myTree->GetEntries() << endl;
  if(nevt == -1) nevt = myTree->GetEntries();

  //// event loop start!
  for(int iev=initev; iev<nevt; ++iev){
    if(iev%100000==0) cout << ">>>>> EVENT " << iev << " / " << myTree->GetEntries() << " ("<<(int)(100.*iev/myTree->GetEntries()) << "%)" << endl;
    //// **** buffer : pp privatae
    ////if (isPA==0 && isPrompt==true && (iev==882739 || iev ==884521 )) continue; 
    ////else if (isPA==0 && isPrompt==false && (iev == 226946 || iev==36800 || iev == 780741 || iev == 884521)) continue;
    ////gDebug=2;
    //// **** buffer : pp official
    if (isPA==0 && isPrompt==true && (iev==257671 || iev==728369 || iev==1962013)) continue;
    else if (isPA==0 && isPrompt==false && (iev == 12770 || iev==94836 || iev==228145 || iev==499832 || iev==536493 || iev==536492 || iev==582358 || iev==617861 || iev==687792 || iev==981657 || iev==1085955 || iev==1361016 || iev == 1433478 || iev==1520599 || iev==1753490 || iev==1839208 || iev==1973187 || iev==1997056 || iev==2149281|| iev==2205201)) continue;
    myTree->GetEntry(iev);
//    cout << "iev = " << iev <<", buffer : " << myTree->GetEntry(iev) << endl;

    ////zvtx Weighting
    theZvtx = zVtx; 
    if (useZvtxWeight) { zWeight = gRatio -> Eval(theZvtx); }
  
    //if ( Gen_QQ_size ==0 ) continue; //KYO : remove???
    ////// Gen_QQ_size loop
    for (Int_t igqq=0; igqq<Gen_QQ_size; ++igqq) {
      mupl_GEN = (TLorentzVector*) Gen_QQ_mupl_4mom->At(igqq);
      mumi_GEN = (TLorentzVector*) Gen_QQ_mumi_4mom->At(igqq);
      jpsi_GEN = (TLorentzVector*) Gen_QQ_4mom->At(igqq);
      *dimu_GEN = *mupl_GEN +  *mumi_GEN;
      
      //// ptWeight
      ptWeight = 1;
      //cout << "" << endl;
      //cout << "jpsi_GEN->Rapidity() = " << jpsi_GEN->Rapidity() << endl; 
      //cout << "jpsi_GEN->Pt() = " << jpsi_GEN->Pt() << endl; 
      for(int iy=0;iy<nRap;iy++){
			  if (isPA==1) { // iy=0 refers to forwards in "funct", while rapArr is in lab frame from neg to pos.
          if(rapArr[iy]<= jpsi_GEN->Rapidity() && jpsi_GEN->Rapidity()<rapArr[iy+1]) { 
            ptWeight = funct[iy]->Eval(jpsi_GEN->Pt());
            //if (ptWeight !=0) {
              //cout << "iy = " << iy << endl;
              //cout << "ptWeight = " << ptWeight << endl;
            //}
          } 
        } else {
          if(rapArr[nRap-iy-1]<= jpsi_GEN->Rapidity() && jpsi_GEN->Rapidity()<rapArr[nRap-iy]) { 
            ptWeight = funct[iy]->Eval(jpsi_GEN->Pt());
            //if (ptWeight !=0) {
              //cout << "iy = " << iy << endl;
              //cout << "ptWeight = " << ptWeight << endl;
            //}
          } 
        }
      }
      
      if ( 
      massCutWide(jpsi_GEN->M()) &&
      kineCut(mupl_GEN, accCutType) && 
      kineCut(mumi_GEN, accCutType)) {
        h2D_Den_pt_y_noPtWeight->Fill(jpsi_GEN->Rapidity(),jpsi_GEN->Pt(),zWeight);
        h2D_Den_pt_y->Fill(jpsi_GEN->Rapidity(),jpsi_GEN->Pt(),zWeight*ptWeight);
        if (MrapNpt==89) h2D_Den_pt_y_fine->Fill(jpsi_GEN->Rapidity(),jpsi_GEN->Pt(),zWeight*ptWeight);
      } //end of if statement
    } //end of Gen_QQ_size loop
  
    //// Reco_QQ_size loop
    for (Int_t irqq=0; irqq<Reco_QQ_size; ++irqq) {
      dimu_RECO = (TLorentzVector*) Reco_QQ_4mom->At(irqq);
      mupl_RECO = (TLorentzVector*) Reco_QQ_mupl_4mom->At(irqq);
      mumi_RECO = (TLorentzVector*) Reco_QQ_mumi_4mom->At(irqq);
      
      #if 0
      //// ctau error range cut
      int binmin = h2D_ctErrmin->FindBin(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt);
      double emin = h2D_ctErrmin->GetBinContent(binmin);
      int binmax = h2D_ctErrmax->FindBin(Jpsi_Reco.theRapidity,Jpsi_Reco.thePt);
      double emax = h2D_ctErrmax->GetBinContent(binmax);
      #endif

      if ( Reco_QQ_sign[irqq]==0
      //&& TMath::Abs(theZvtx) < zvtxCut
      //&& emin < Jpsi_Reco.theCtErr && Jpsi_Reco.theCtErr < emax
      && massCutNarrow(dimu_RECO->M())
      && ( ( isPA==0 && (Reco_QQ_trig_PP[irqq]&1)==1 && (HLTriggers_PP&1)==1 ) 
          || ( isPA!=0 && (Reco_QQ_trig_PA[irqq]&1)==1 && (HLTriggers_PA&1)==1 ) )
      && kineCut(mupl_RECO, accCutType)
      && kineCut(mumi_RECO, accCutType)
      //// muID
      && Reco_QQ_VtxProb[irqq] > 0.01
      && Reco_QQ_mupl_TrkMuArb[irqq]
      && Reco_QQ_mupl_TMOneStaTight[irqq]
      && Reco_QQ_mupl_highPurity[irqq]
      && Reco_QQ_mupl_nTrkWMea[irqq] > 5
      && Reco_QQ_mupl_nPixWMea[irqq] > 0
      && TMath::Abs(Reco_QQ_mupl_dxy[irqq]) < 0.3
      && TMath::Abs(Reco_QQ_mupl_dz[irqq]) < 20.0
      && Reco_QQ_mumi_TrkMuArb[irqq]
      && Reco_QQ_mumi_TMOneStaTight[irqq]
      && Reco_QQ_mumi_highPurity[irqq]
      && Reco_QQ_mumi_nTrkWMea[irqq] > 5
      && Reco_QQ_mumi_nPixWMea[irqq] > 0
      && TMath::Abs(Reco_QQ_mumi_dxy[irqq]) < 0.3
      && TMath::Abs(Reco_QQ_mumi_dz[irqq]) < 20.0
      ) {
        //// TNP && vZtx weighting  
        tnpWeight = 1;
        if (useSF){ tnpWeight = getSF(mupl_RECO->Pt(),mupl_RECO->Eta(),mumi_RECO->Pt(),mumi_RECO->Eta(),isPA,accCutType);}
        h2D_Num_pt_y_noPtWeight->Fill(dimu_RECO->Rapidity(),dimu_RECO->Pt(), tnpWeight*zWeight);
        h2D_Num_pt_y->Fill(dimu_RECO->Rapidity(),dimu_RECO->Pt(), tnpWeight*zWeight*ptWeight);
        if (MrapNpt==89) h2D_Num_pt_y_fine->Fill(dimu_RECO->Rapidity(),dimu_RECO->Pt(), tnpWeight*zWeight*ptWeight);
        h1D_zVtx->Fill(theZvtx, zWeight);
      } //end of if statement
    } //end of Reco_QQ_size loop
  Reco_QQ_4mom =0;
  Reco_QQ_mupl_4mom =0;
  Reco_QQ_mumi_4mom =0;
  Gen_QQ_4mom =0;
  Gen_QQ_mupl_4mom =0;
  Gen_QQ_mumi_4mom =0;
  
  if(iev == 2346436) cout << "wtf " << endl;
  
  } //end of event loop
  
  cout << "wtf" << endl;

  // (Num/Den) to get efficiency (B : binomial error)
  h2D_Eff_pt_y_noPtWeight->Divide(h2D_Num_pt_y_noPtWeight,h2D_Den_pt_y_noPtWeight,1,1,"B");
  h2D_Eff_pt_y->Divide(h2D_Num_pt_y,h2D_Den_pt_y,1,1,"B");
  if (MrapNpt==89) h2D_Eff_pt_y_fine->Divide(h2D_Num_pt_y_fine,h2D_Den_pt_y_fine,1,1,"B");
  
  ////////////////////////////////////////////////////////////////////////////
  // Save the data!

  TFile *outFile = new TFile(Form("EffAna_%s_Zvtx%d_SF%d_ptWeight.root",szFinal.Data(), (int)useZvtxWeight,(int)useSF),"RECREATE");
  std::cout << "szFinal: " << szFinal << std::endl;
  outFile->cd();
  //h2D_ctErrmin->Write();
  //h2D_ctErrmax->Write();
  h2D_Den_pt_y_noPtWeight->Write();
  h2D_Num_pt_y_noPtWeight->Write();
  h2D_Eff_pt_y_noPtWeight->Write();
  h2D_Den_pt_y->Write();
  h2D_Num_pt_y->Write();
  h2D_Eff_pt_y->Write();
  h1D_zVtx->Write();
  if (MrapNpt==89) {
    h2D_Den_pt_y_fine->Write();
    h2D_Num_pt_y_fine->Write();
    h2D_Eff_pt_y_fine->Write();
  }
  outFile->Close();

  return 0;

} // end of main func

////////////////////////////////////////////////////////////////////////////////////////////////////  
//// sub-routines function 

float getSF(float mupt1, float mueta1, float mupt2, float mueta2, int ispa, int acctype) { 
//  float mup1 = mupt1* cosh(mueta1);
//  float mup2 = mupt2* cosh(mueta2);
//  if ( kineCut( mupt1, mueta1, mup1)==false) return 0;
//  if ( kineCut( mupt2, mueta2, mup2)==false) return 0;

  TF1* hw1;
  TF1* hw2;

  if (ispa==0) { 
    if (  TMath::Abs(mueta1) < 0.9 )      hw1 = hTnp_pp_new_eta1;
    else if ( TMath::Abs(mueta1) < 1.2 )  hw1 = hTnp_pp_new_eta2;
    else if ( TMath::Abs(mueta1) < 1.6 )  hw1 = hTnp_pp_new_eta3;
    else if ( TMath::Abs(mueta1) < 2.1 )  hw1 = hTnp_pp_new_eta4;
    else                                  hw1 = hTnp_pp_new_eta5;
    if (  TMath::Abs(mueta2) < 0.9 )      hw2 = hTnp_pp_new_eta1;
    else if ( TMath::Abs(mueta2) < 1.2 )  hw2 = hTnp_pp_new_eta2;
    else if ( TMath::Abs(mueta2) < 1.6 )  hw2 = hTnp_pp_new_eta3;
    else if ( TMath::Abs(mueta2) < 2.1 )  hw2 = hTnp_pp_new_eta4;
    else                                  hw2 = hTnp_pp_new_eta5;
  }
  else if (ispa==1 || ispa==2) {
    if (acctype==1) {
      if (  TMath::Abs(mueta1) < 0.6 )      hw1 = hTnp_pa_old_eta1;
      else if ( TMath::Abs(mueta1) < 1.3 )  hw1 = hTnp_pa_old_eta2;
      else if ( TMath::Abs(mueta1) < 1.8 )  hw1 = hTnp_pa_old_eta3;
      else if ( TMath::Abs(mueta1) < 2.2 )  hw1 = hTnp_pa_old_eta4;
      else                                  hw1 = hTnp_pa_old_eta5;
      if (  TMath::Abs(mueta2) < 0.6 )      hw2 = hTnp_pa_old_eta1;
      else if ( TMath::Abs(mueta2) < 1.3 )  hw2 = hTnp_pa_old_eta2;
      else if ( TMath::Abs(mueta2) < 1.8 )  hw2 = hTnp_pa_old_eta3;
      else if ( TMath::Abs(mueta2) < 2.2 )  hw2 = hTnp_pa_old_eta4;
      else                                  hw2 = hTnp_pa_old_eta5;
    }
    else if (acctype==2) {
      if (  TMath::Abs(mueta1) < 0.9 )      hw1 = hTnp_pa_new_eta1;
      else if ( TMath::Abs(mueta1) < 1.2 )  hw1 = hTnp_pa_new_eta2;
      else if ( TMath::Abs(mueta1) < 1.6 )  hw1 = hTnp_pa_new_eta3;
      else if ( TMath::Abs(mueta1) < 2.1 )  hw1 = hTnp_pa_new_eta4;
      else                                  hw1 = hTnp_pa_new_eta5;
      if (  TMath::Abs(mueta2) < 0.9 )      hw2 = hTnp_pa_new_eta1;
      else if ( TMath::Abs(mueta2) < 1.2 )  hw2 = hTnp_pa_new_eta2;
      else if ( TMath::Abs(mueta2) < 1.6 )  hw2 = hTnp_pa_new_eta3;
      else if ( TMath::Abs(mueta2) < 2.1 )  hw2 = hTnp_pa_new_eta4;
      else                                  hw2 = hTnp_pa_new_eta5;
    }
  }
  else {return 0;}
    
  float tnpWeight1 = hw1->Eval(mupt1);
  float tnpWeight2 = hw2->Eval(mupt2);

  return tnpWeight1 * tnpWeight2;
}

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
      //(1.2 <= TMath::Abs(Mu->Eta()) && TMath::Abs(Mu->Eta()) < 2.1 && Mu->Pt() >= -(1.0/0.9)*TMath::Abs(Mu->Eta())+(1.2*(1.0/0.9)+2.6)) ||
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

bool massCutWide(double lv_dimu_mass) {
  return ( 2.6 <= lv_dimu_mass && lv_dimu_mass < 3.5 );
}

bool massCutNarrow(double lv_dimu_mass) {
  return ( 2.9 <= lv_dimu_mass && lv_dimu_mass < 3.3 );
}

void formRapStr(Double_t min, Double_t max, string* arr) { //byHand KYO
  if (min==-2.4 || min==2.4) *arr = Form("%.1f-%.2f",min,max);
  else if (max==-2.4 || max==2.4) *arr = Form("%.2f-%.1f",min,max);
  else *arr = Form("%.2f-%.2f",min,max);
}

void formStr(Double_t min, Double_t max, string* arr) {
  *arr = Form("%.1f-%.1f",min,max);
}

