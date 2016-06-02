#include "../SONGKYO.h"

void formRapStr(Double_t min, Double_t max, string* arr);
void formStr(Double_t min, Double_t max, string* arr);

/////// main func. ///////
int make_fitResHist(int MrapNpt=89, int isPA =0, int accCutType=2, TString szSys="nominal"){
//  char *dirName = "fitRes_8rap9pt", bool is1st = true){

  using namespace std;

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
  else if (isPA==3) szPA="pA";
  else {cout << "select among isPA = 0, 1, 2, or 3"<< endl; return 0; }
  TString szAccCut;
  if (accCutType==1) szAccCut="oldcut";
  else if (accCutType==2) szAccCut="newcut";
  else {cout << "select among accCutType = 1 or 2"<< endl; return 0; }  

  const TString szFinal = Form("%s_%s_%s_%s",szBinning.Data(),szPA.Data(),szAccCut.Data(),szSys.Data());
  std::cout << "szFinal: " << szFinal << std::endl;

  //// read-in 3 txt files
  std::ifstream fctau(Form("./fitRes/summary_%s_%s_%s_%s/fit_ctauErrorRange",szPA.Data(),szBinning.Data(),szAccCut.Data(),szSys.Data()),std::ios::in);
  if(!fctau.is_open()) { cout << "Warning : can NOT open the fit_ctauErrorRange file!"<<endl; return 0; }
  std::ifstream ftable(Form("./fitRes/summary_%s_%s_%s_%s/fit_table",szPA.Data(),szBinning.Data(),szAccCut.Data(),szSys.Data()),std::ios::in);
  if(!ftable.is_open()) { cout << "Warning : can NOT open the fit_table file!"<<endl; return 0; }
  std::ifstream fparam(Form("./fitRes/summary_%s_%s_%s_%s/fit_parameters",szPA.Data(),szBinning.Data(),szAccCut.Data(),szSys.Data()),std::ios::in);
  if(!fparam.is_open()) { cout << "Warning : can NOT open the fit_table file!"<<endl; return 0; }

  ///////////////////////////////////////////////////
  //////// Definition of binning
  int nRapInt, nPtInt;
  if (MrapNpt==89) { nRapInt = 8; nPtInt = 9; }
  else if (MrapNpt==83) { nRapInt = 8; nPtInt = 3; }
  else if (MrapNpt==63) { nRapInt = 6; nPtInt = 3;}
  else { nRapInt = 6; nPtInt = 2;}
  const int nRapTmp = nRapInt +1;
  const int nPtTmp = nPtInt +1; 
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
  
  Double_t rapArr[nRapTmp];
  if (isPA==2){
    // change "y_lab" from Pbp to pPb
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
  const Int_t nRap = sizeof(rapArr)/sizeof(double)-1;
  const Int_t nPt = sizeof(ptArr)/sizeof(double)-1;
  
  // --- ntrk bin
  Double_t ntrArr[] = {0.0, 350.0};
  const Int_t nNtr = sizeof(ntrArr)/sizeof(double)-1;
  // --- Et bin
  Double_t etArr[] = {0.0, 120.0};
  const Int_t nEt = sizeof(etArr)/sizeof(double)-1;

  cout << " *** nBins *** " << endl;  
  cout << "nRap=" << nRap << endl;
  cout << "nPt=" << nPt << endl;
  cout << "nNtr=" << nNtr << endl;
  cout << "nEt=" << nEt << endl;
  
  // setting for string bin array for checking summary & files
  string* yrange = new string[nRap];
  string* ptrange = new string[nPt];
  string* ntrrange = new string[nNtr];
  string* etrange = new string[nEt];
  for (Int_t i=0; i<nRap; i++){ 
    formRapStr(rapArr[i], rapArr[i+1], &yrange[i]);
    cout << "yrange["<<i<<"] = "<< yrange[i] << endl;
  }
  for (Int_t i=0; i<nPt; i++){  
    formStr(ptArr[i], ptArr[i+1], &ptrange[i]);
    cout << "ptrange["<<i<<"] = "<< ptrange[i] << endl;
  }
  for (Int_t i=0; i<nNtr; i++){ 
    formStr(ntrArr[i], ntrArr[i+1], &ntrrange[i]);
    cout << "ntrrange["<<i<<"] = "<< ntrrange[i] << endl;
  }
  for (Int_t i=0; i<nEt; i++){  
    formStr(etArr[i], etArr[i+1], &etrange[i]);
    cout << "etrange["<<i<<"] = "<< etrange[i] << endl;
  }

  //////////////////////////////////////////////////////
  ////// 1) read fit_ctauErrorRange
  float ctErrmin[nRap][nPt];
  float ctErrmax[nRap][nPt];
  int NoCutEntry[nRap][nPt];
  int CutEntry[nRap][nPt];
  float CutRatio[nRap][nPt];
  TH2D *h2D_ctErrmin = new TH2D("h2D_ctErrmin","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_ctErrmax = new TH2D("h2D_ctErrmax","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_NoCutEntry = new TH2D("h2D_NoCutEntry","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_CutEntry = new TH2D("h2D_CutEntry","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_CutRatio = new TH2D("h2D_CutRatio","",nRap,rapArr,nPt,ptArr);

  string headers;
  getline(fctau, headers); // remove prefix
  getline(fctau, headers); // remove column names
  string rapdum, ptdum, ntrdum, etdum, errmin, errmax, nocutentry, cutentry;

  while(!fctau.eof()) {
    fctau >> rapdum >> ptdum >> ntrdum >> etdum >> errmin >> errmax >> nocutentry >> cutentry;
    //cout << rapdum << ptdum << ntrdum << etdum << errmin << errmax << nocutentry << cutentry << endl;
    for (Int_t iy=0; iy<nRap; iy++){
      for (Int_t ipt=0; ipt<nPt; ipt++) {
        if (!yrange[iy].compare(rapdum) && !ptrange[ipt].compare(ptdum) && !ntrrange[0].compare(ntrdum) && !etrange[0].compare(etdum)) { // no loop for ntrrange and etrange
          ctErrmin[iy][ipt] = atof(errmin.c_str());
          ctErrmax[iy][ipt] = atof(errmax.c_str());
          NoCutEntry[iy][ipt] = atoi(nocutentry.c_str());
          CutEntry[iy][ipt] = atoi(cutentry.c_str());
          CutRatio[iy][ipt] = (double)NoCutEntry[iy][ipt]/(double)CutEntry[iy][ipt];
        }
      } 
    }
  } //end of while file open
  
  // put the values into hist
  int tmpbin=0;
  for (Int_t iy=0; iy<nRap; iy++){
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      tmpbin = h2D_ctErrmin->FindBin((rapArr[iy]+rapArr[iy+1])/2, (ptArr[ipt]+ptArr[ipt+1])/2); 
      h2D_ctErrmin->SetBinContent(tmpbin, ctErrmin[iy][ipt]);
      h2D_ctErrmax->SetBinContent(tmpbin, ctErrmax[iy][ipt]);
      h2D_NoCutEntry->SetBinContent(tmpbin, NoCutEntry[iy][ipt]);
      h2D_CutEntry->SetBinContent(tmpbin, CutEntry[iy][ipt]);
      h2D_CutRatio->SetBinContent(tmpbin, CutRatio[iy][ipt]);
    }
  }
  
  //////////////////////////////////////////////////////
  ////// 2) read fit_table

  float nSig[nRap][nPt];
  float nBkg[nRap][nPt];
  float nPrompt[nRap][nPt];
  float nNonprompt[nRap][nPt];
  float bFraction[nRap][nPt];
  
  float nSigErr[nRap][nPt];
  float nBkgErr[nRap][nPt];
  float nPromptErr[nRap][nPt];
  float nNonpromptErr[nRap][nPt];
  float bFractionErr[nRap][nPt];
  
  TH2D *h2D_nSig_Raw = new TH2D("h2D_nSig_Raw","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nBkg_Raw = new TH2D("h2D_nBkg_Raw","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nPrompt_Raw = new TH2D("h2D_nPrompt_Raw","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nNonPrompt_Raw = new TH2D("h2D_nNonPrompt_Raw","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_bFraction = new TH2D("h2D_bFraction","",nRap,rapArr,nPt,ptArr);
  
  headers;
  getline(ftable, headers); // remove prefix
  getline(ftable, headers); // remove column names
  string sigdum, sigerrdum, bkgdum, bkgerrdum, prdum, prerrdum, npdum, nperrdum, bfrdum, bfrerrdum;

  while(!ftable.eof()) {
    ftable >> rapdum >> ptdum >> ntrdum >> etdum >> sigdum >> sigerrdum >> bkgdum >> bkgerrdum >> prdum >> prerrdum >> npdum >> nperrdum >> bfrdum >> bfrerrdum;
    //cout <<  rapdum << ptdum << << ntrdum << etdum << sigdum << sigerrdum << bkgdum << bkgerrdum << prdum << prerrdum << npdum << nperrdum << bfrdum << bfrerrdum << endl;
    for (Int_t iy=0; iy<nRap; iy++){
      for (Int_t ipt=0; ipt<nPt; ipt++) {
        if (!yrange[iy].compare(rapdum) && !ptrange[ipt].compare(ptdum) && !ntrrange[0].compare(ntrdum) && !etrange[0].compare(etdum)) { // no loop for ntrrange and etrange
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
  for (Int_t iy=0; iy<nRap; iy++){
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      tmpbin = h2D_nSig_Raw->FindBin((rapArr[iy]+rapArr[iy+1])/2, (ptArr[ipt]+ptArr[ipt+1])/2); 
      h2D_nSig_Raw->SetBinContent(tmpbin, nSig[iy][ipt]);
      h2D_nSig_Raw->SetBinError(tmpbin, nSigErr[iy][ipt]);
      h2D_nBkg_Raw->SetBinContent(tmpbin, nBkg[iy][ipt]);
      h2D_nBkg_Raw->SetBinError(tmpbin, nBkgErr[iy][ipt]);
      h2D_nPrompt_Raw->SetBinContent(tmpbin, nPrompt[iy][ipt]);
      h2D_nPrompt_Raw->SetBinError(tmpbin, nPromptErr[iy][ipt]);
      h2D_nNonPrompt_Raw->SetBinContent(tmpbin, nNonprompt[iy][ipt]);
      h2D_nNonPrompt_Raw->SetBinError(tmpbin, nNonpromptErr[iy][ipt]);
      h2D_bFraction->SetBinContent(tmpbin, bFraction[iy][ipt]);
      h2D_bFraction->SetBinError(tmpbin, bFractionErr[iy][ipt]);
    }
  }
  
  //// weight by CutRatio
  TH2D *h2D_nSig = new TH2D("h2D_nSig","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nBkg = new TH2D("h2D_nBkg","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nPrompt = new TH2D("h2D_nPrompt","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nNonPrompt = new TH2D("h2D_nNonPrompt","",nRap,rapArr,nPt,ptArr);
  tmpbin=0;
  for (Int_t iy=0; iy<nRap; iy++){
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      tmpbin = h2D_nSig->FindBin((rapArr[iy]+rapArr[iy+1])/2, (ptArr[ipt]+ptArr[ipt+1])/2); 
      h2D_nSig->SetBinContent(tmpbin, nSig[iy][ipt]*CutRatio[iy][ipt]);
      h2D_nSig->SetBinError(tmpbin, nSigErr[iy][ipt]*CutRatio[iy][ipt]);
      h2D_nBkg->SetBinContent(tmpbin, nBkg[iy][ipt]*CutRatio[iy][ipt]);
      h2D_nBkg->SetBinError(tmpbin, nBkgErr[iy][ipt]*CutRatio[iy][ipt]);
      h2D_nPrompt->SetBinContent(tmpbin, nPrompt[iy][ipt]*CutRatio[iy][ipt]);
      h2D_nPrompt->SetBinError(tmpbin, nPromptErr[iy][ipt]*CutRatio[iy][ipt]);
      h2D_nNonPrompt->SetBinContent(tmpbin, nNonprompt[iy][ipt]*CutRatio[iy][ipt]);
      h2D_nNonPrompt->SetBinError(tmpbin, nNonpromptErr[iy][ipt]*CutRatio[iy][ipt]);
    }
  }

  //////////////////////////////////////////////////////
  ////// 3) read fit_parameters

  float coefExp[nRap][nPt]; float coefExpErr[nRap][nPt];
  float coefPol[nRap][nPt]; float coefPolErr[nRap][nPt];
  float fracG1[nRap][nPt]; float fracG1Err[nRap][nPt];
  float meanSig[nRap][nPt]; float meanSigErr[nRap][nPt];
  float sigmaSig1[nRap][nPt]; float sigmaSig1Err[nRap][nPt];
  float sigmaSig2[nRap][nPt]; float sigmaSig2Err[nRap][nPt];
  float alpha[nRap][nPt]; float alphaErr[nRap][nPt];
  float enne[nRap][nPt]; float enneErr[nRap][nPt];
  float sigWidth[nRap][nPt]; float sigWidthErr[nRap][nPt];
  float sigmaNPTrue[nRap][nPt]; float sigmaNPTrueErr[nRap][nPt];
  float coefExpNPTrue[nRap][nPt]; float coefExpNPTrueErr[nRap][nPt];
  float fracRes[nRap][nPt]; float fracResErr[nRap][nPt];
  float meanPRResW[nRap][nPt]; float meanPRResWErr[nRap][nPt];
  float meanPRResN[nRap][nPt]; float meanPRResNErr[nRap][nPt];
  float sigmaPRResW[nRap][nPt]; float sigmaPRResWErr[nRap][nPt];
  float sigmaPRResN[nRap][nPt]; float sigmaPRResNErr[nRap][nPt];
  float fracCtBkg1[nRap][nPt]; float fracCtBkg1Err[nRap][nPt];
  float fracCtBkg2[nRap][nPt]; float fracCtBkg2Err[nRap][nPt];
  float fracCtBkg3[nRap][nPt]; float fracCtBkg3Err[nRap][nPt];
  float lambdam[nRap][nPt]; float lambdamErr[nRap][nPt];
  float lambdap[nRap][nPt]; float lambdapErr[nRap][nPt];
  float lambdasym[nRap][nPt]; float lambdasymErr[nRap][nPt];
  
  float NLL[nRap][nPt];
  float Resolution[nRap][nPt]; float ResolutionErr[nRap][nPt];
  
  float UnNormChi2_mass[nRap][nPt];
  float nFitParam_mass[nRap][nPt];
  float nFullBinsPull_mass[nRap][nPt];
  float Dof_mass[nRap][nPt];
  float Chi2_mass[nRap][nPt];
  float theChi2Prob_mass[nRap][nPt];
  float UnNormChi2_time[nRap][nPt];
  float nFitParam_time[nRap][nPt];
  float nFullBinsPull_time[nRap][nPt];
  float Dof_time[nRap][nPt];
  float Chi2_time[nRap][nPt];
  float theChi2Prob_time[nRap][nPt];
  float UnNormChi2_side[nRap][nPt];
  float nFitParam_side[nRap][nPt];
  float nFullBinsPull_side[nRap][nPt];
  float Dof_side[nRap][nPt];
  float Chi2_side[nRap][nPt];
  float theChi2Prob_side[nRap][nPt];

  TH2D *h2D_coefExp = new TH2D("h2D_coefExp","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_coefPol = new TH2D("h2D_coefPol","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_fracG1 = new TH2D("h2D_fracG1","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_meanSig = new TH2D("h2D_meanSig","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_sigmaSig1 = new TH2D("h2D_sigmaSig1","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_sigmaSig2 = new TH2D("h2D_sigmaSig2","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_alpha = new TH2D("h2D_alpha","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_enne = new TH2D("h2D_enne","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_sigWidth = new TH2D("h2D_sigWidth","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_sigmaNPTrue = new TH2D("h2D_sigmaNPTrue","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_coefExpNPTrue = new TH2D("h2D_coefExpNPTrue","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_fracRes = new TH2D("h2D_fracRes","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_meanPRResW = new TH2D("h2D_meanPRResW","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_meanPRResN = new TH2D("h2D_meanPRResN","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_sigmaPRResW = new TH2D("h2D_sigmaPRResW","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_sigmaPRResN = new TH2D("h2D_sigmaPRResN","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_fracCtBkg1 = new TH2D("h2D_fracCtBkg1","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_fracCtBkg2 = new TH2D("h2D_fracCtBkg2","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_fracCtBkg3 = new TH2D("h2D_fracCtBkg3","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_lambdam = new TH2D("h2D_lambdam","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_lambdap = new TH2D("h2D_lambdap","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_lambdasym = new TH2D("h2D_lambdasym","",nRap,rapArr,nPt,ptArr);
  
  TH2D *h2D_NLL = new TH2D("h2D_NLL","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Resolution = new TH2D("h2D_Resolution","",nRap,rapArr,nPt,ptArr);
  
  TH2D *h2D_UnNormChi2_mass = new TH2D("h2D_UnNormChi2_mass","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nFitParam_mass = new TH2D("h2D_nFitParam_mass","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nFullBinsPull_mass = new TH2D("h2D_nFullBinsPull_mass","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Dof_mass = new TH2D("h2D_Dof_mass","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Chi2_mass = new TH2D("h2D_Chi2_mass","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_theChi2Prob_mass = new TH2D("h2D_theChi2Prob_mass","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_UnNormChi2_time = new TH2D("h2D_UnNormChi2_time","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nFitParam_time = new TH2D("h2D_nFitParam_time","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nFullBinsPull_time = new TH2D("h2D_nFullBinsPull_time","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Dof_time = new TH2D("h2D_Dof_time","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Chi2_time = new TH2D("h2D_Chi2_time","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_theChi2Prob_time = new TH2D("h2D_theChi2Prob_time","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_UnNormChi2_side = new TH2D("h2D_UnNormChi2_side","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nFitParam_side = new TH2D("h2D_nFitParam_side","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_nFullBinsPull_side = new TH2D("h2D_nFullBinsPull_side","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Dof_side = new TH2D("h2D_Dof_side","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_Chi2_side = new TH2D("h2D_Chi2_side","",nRap,rapArr,nPt,ptArr);
  TH2D *h2D_theChi2Prob_side = new TH2D("h2D_theChi2Prob_side","",nRap,rapArr,nPt,ptArr);

  headers;
  getline(fparam, headers); // remove prefix
  getline(fparam, headers); // remove column names
  string coefexp, coefexperr, coefpol, coefpolerr, fracg1, fracg1err, meansig, meansigerr, sigmasig1, sigmasig1err, sigmasig2, sigmasig2err, alphadum, alphadumerr, ennedum, ennedumerr, sigwidth, sigwidtherr;
  string signptr, signptrerr, coefexpnptr, coefexpnptrerr, fracres, fracreserr, meanprresw, meanprreswerr, meanprresn, meanprresnerr, sigprresw, sigprreswerr, sigprresn, sigprresnerr;
  string fracbkg1, fracbkg1err, fracbkg2, fracbkg2err, fracbkg3, fracbkg3err, lambm, lambmerr, lambp, lambperr, lambsym, lambsymerr;
  string nll, resol, resolerr;
  string unnormchi2_m, fitpar_m, nbins_m, dof_m, chi2_m, prob_m;
  string unnormchi2_t, fitpar_t, nbins_t, dof_t, chi2_t, prob_t;
  string unnormchi2_s, fitpar_s, nbins_s, dof_s, chi2_s, prob_s;

  while(!fparam.eof()) {
    fparam >> rapdum >> ptdum >> ntrdum >> etdum >> errmin >> errmax >> nocutentry >> cutentry >> sigdum >> sigerrdum >> bkgdum >> bkgerrdum >> coefexp>> coefexperr>> coefpol>> coefpolerr>> fracg1>> fracg1err>> meansig>> meansigerr>> sigmasig1>> sigmasig1err>> sigmasig2>> sigmasig2err>> alphadum>> alphadumerr>> ennedum>> ennedumerr>> sigwidth>> sigwidtherr >> signptr>> signptrerr>> coefexpnptr>> coefexpnptrerr>> fracres>> fracreserr>> meanprresw>> meanprreswerr>> meanprresn>> meanprresnerr>> sigprresw>> sigprreswerr>> sigprresn>> sigprresnerr >> fracbkg1>> fracbkg1err>> fracbkg2>> fracbkg2err>> fracbkg3>> fracbkg3err>> lambm>> lambmerr>> lambp>> lambperr >> lambsym >> lambsymerr >> nll>> resol>> resolerr >> prdum >> prerrdum >> npdum >> nperrdum >> bfrdum >> bfrerrdum >> unnormchi2_m>> fitpar_m>> nbins_m>> dof_m>> chi2_m>> prob_m >> unnormchi2_t>> fitpar_t>> nbins_t>> dof_t>> chi2_t>> prob_t >> unnormchi2_s>> fitpar_s>> nbins_s>> dof_s>> chi2_s>> prob_s;
    //cout << rapdum <<", "<< ptdum <<", "<< ntrdum <<", "<< etdum <<", "<< errmin <<", "<< errmax <<", "<< nocutentry <<", "<< cutentry <<", "<< sigdum <<", "<< sigerrdum <<", "<< bkgdum <<", "<< bkgerrdum <<", "<< coefexp<<", "<< coefexperr<<", "<< coefpol<<", "<< coefpolerr<<", "<< fracg1<<", "<< fracg1err<<", "<< meansig<<", "<< meansigerr<<", "<< sigmasig1<<", "<< sigmasig1err<<", "<< sigmasig2<<", "<< sigmasig2err<<", "<< alphadum<<", "<< alphadumerr<<", "<< ennedum<<", "<< ennedumerr<<", "<< sigwidth<<", "<< sigwidtherr <<", "<< signptr<<", "<< signptrerr<<", "<< coefexpnptr<<", "<< coefexpnptrerr<<", "<< fracres<<", "<< fracreserr<<", "<< meanprresw<<", "<< meanprreswerr<<", "<< meanprresn<<", "<< meanprresnerr<<", "<< sigprresw<<", "<< sigprreswerr<<", "<< sigprresn<<", "<< sigprresnerr <<", "<< fracbkg1<<", "<< fracbkg1err<<", "<< fracbkg2<<", "<< fracbkg2err<<", "<< fracbkg3<<", "<< fracbkg3err<<", "<< lambm<<", "<< lambmerr<<", "<< lambp<<", "<< lambperr <<", "<< lambsym <<", "<< lambsymerr <<", "<< nll<<", "<< resol<<", "<< resolerr <<", "<< prdum <<", "<< prerrdum <<", "<< npdum <<", "<< nperrdum <<", "<< bfrdum <<", "<< bfrerrdum <<", "<< unnormchi2_m<<", "<< fitpar_m<<", "<< nbins_m<<", "<< dof_m<<", "<< chi2_m<<", "<< prob_m <<", "<< unnormchi2_t<<", "<< fitpar_t<<", "<< nbins_t<<", "<< dof_t<<", "<< chi2_t<<", "<< prob_t <<", "<< unnormchi2_s<<", "<< fitpar_s<<", "<< nbins_s<<", "<< dof_s<<", "<< chi2_s<<", "<< prob_s << endl;
    for (Int_t iy=0; iy<nRap; iy++){
      for (Int_t ipt=0; ipt<nPt; ipt++) {
        if (!yrange[iy].compare(rapdum) && !ptrange[ipt].compare(ptdum) && !ntrrange[0].compare(ntrdum) && !etrange[0].compare(etdum)) { // no loop for ntrrange and etrange
          coefExp[iy][ipt] = atof(coefexp.c_str());
          coefExpErr[iy][ipt] = atof(coefexperr.c_str());
          coefPol[iy][ipt] = atof(coefpol.c_str());
          coefPolErr[iy][ipt] = atof(coefpolerr.c_str());
          fracG1[iy][ipt] = atof(fracg1.c_str());
          fracG1Err[iy][ipt] = atof(fracg1err.c_str());
          meanSig[iy][ipt] = atof(meansig.c_str());
          meanSigErr[iy][ipt] = atof(meansigerr.c_str());
          sigmaSig1[iy][ipt] = atof(sigmasig1.c_str());
          sigmaSig1Err[iy][ipt] = atof(sigmasig1err.c_str());
          sigmaSig2[iy][ipt] = atof(sigmasig2.c_str());
          sigmaSig2Err[iy][ipt] = atof(sigmasig2err.c_str());
          alpha[iy][ipt] = atof(alphadum.c_str());
          alphaErr[iy][ipt] = atof(alphadumerr.c_str());
          enne[iy][ipt] = atof(ennedum.c_str());
          enneErr[iy][ipt] = atof(ennedumerr.c_str());
          sigWidth[iy][ipt] = atof(sigwidth.c_str());
          sigWidthErr[iy][ipt] = atof(sigwidtherr.c_str());
          sigmaNPTrue[iy][ipt] = atof(signptr.c_str());
          sigmaNPTrueErr[iy][ipt] = atof(signptrerr.c_str());
          coefExpNPTrue[iy][ipt] = atof(coefexpnptr.c_str());
          coefExpNPTrueErr[iy][ipt] = atof(coefexpnptrerr.c_str());
          fracRes[iy][ipt] = atof(fracres.c_str());
          fracResErr[iy][ipt] = atof(fracreserr.c_str());
          meanPRResW[iy][ipt] = atof(meanprresw.c_str());
          meanPRResWErr[iy][ipt] = atof(meanprreswerr.c_str());
          meanPRResN[iy][ipt] = atof(meanprresn.c_str());
          meanPRResNErr[iy][ipt] = atof(meanprresnerr.c_str());
          sigmaPRResW[iy][ipt] = atof(sigprresw.c_str());
          sigmaPRResWErr[iy][ipt] = atof(sigprreswerr.c_str());
          sigmaPRResN[iy][ipt] = atof(sigprresn.c_str());
          sigmaPRResNErr[iy][ipt] = atof(sigprresnerr.c_str());
          fracCtBkg1[iy][ipt] = atof(fracbkg1.c_str());
          fracCtBkg1Err[iy][ipt] = atof(fracbkg1err.c_str());
          fracCtBkg2[iy][ipt] = atof(fracbkg2.c_str());
          fracCtBkg2Err[iy][ipt] = atof(fracbkg2err.c_str());
          fracCtBkg3[iy][ipt] = atof(fracbkg3.c_str());
          fracCtBkg3Err[iy][ipt] = atof(fracbkg3err.c_str());
          lambdam[iy][ipt] = atof(lambm.c_str());
          lambdamErr[iy][ipt] = atof(lambmerr.c_str());
          lambdap[iy][ipt] = atof(lambp.c_str());
          lambdapErr[iy][ipt] = atof(lambperr.c_str());
          lambdasym[iy][ipt] = atof(lambsym.c_str());
          lambdasymErr[iy][ipt] = atof(lambsymerr.c_str());
          NLL[iy][ipt] = atof(nll.c_str());
          Resolution[iy][ipt] = atof(resol.c_str());
          ResolutionErr[iy][ipt] = atof(resolerr.c_str());
          
          UnNormChi2_mass[iy][ipt] = atof(unnormchi2_m.c_str());
          nFitParam_mass[iy][ipt] = atof(fitpar_m.c_str());
          nFullBinsPull_mass[iy][ipt] = atof(nbins_m.c_str());
          Dof_mass[iy][ipt] = atof(dof_m.c_str());
          Chi2_mass[iy][ipt] = atof(chi2_m.c_str());
          theChi2Prob_mass[iy][ipt] = atof(prob_m.c_str());
          UnNormChi2_time[iy][ipt] = atof(unnormchi2_t.c_str());
          nFitParam_time[iy][ipt] = atof(fitpar_t.c_str());
          nFullBinsPull_time[iy][ipt] = atof(nbins_t.c_str());
          Dof_time[iy][ipt] = atof(dof_t.c_str());
          Chi2_time[iy][ipt] = atof(chi2_t.c_str());
          theChi2Prob_time[iy][ipt] = atof(prob_t.c_str());
          UnNormChi2_side[iy][ipt] = atof(unnormchi2_s.c_str());
          nFitParam_side[iy][ipt] = atof(fitpar_s.c_str());
          nFullBinsPull_side[iy][ipt] = atof(nbins_s.c_str());
          Dof_side[iy][ipt] = atof(dof_s.c_str());
          Chi2_side[iy][ipt] = atof(chi2_s.c_str());
          theChi2Prob_side[iy][ipt] = atof(prob_s.c_str());
        }
      } 
    }
  } //end of while file open
  
  // put the values into hist
  tmpbin=0;
  for (Int_t iy=0; iy<nRap; iy++){
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      tmpbin = h2D_nSig_Raw->FindBin((rapArr[iy]+rapArr[iy+1])/2, (ptArr[ipt]+ptArr[ipt+1])/2); 
      h2D_coefExp->SetBinContent(tmpbin, coefExp[iy][ipt]);
      h2D_coefExp->SetBinError(tmpbin, coefExpErr[iy][ipt]);
      h2D_coefPol->SetBinContent(tmpbin, coefPol[iy][ipt]);
      h2D_coefPol->SetBinError(tmpbin, coefPolErr[iy][ipt]);
      h2D_fracG1->SetBinContent(tmpbin, fracG1[iy][ipt]);
      h2D_fracG1->SetBinError(tmpbin, fracG1Err[iy][ipt]);
      h2D_meanSig->SetBinContent(tmpbin, meanSig[iy][ipt]);
      h2D_meanSig->SetBinError(tmpbin, meanSigErr[iy][ipt]);
      h2D_sigmaSig1->SetBinContent(tmpbin, sigmaSig1[iy][ipt]);
      h2D_sigmaSig1->SetBinError(tmpbin, sigmaSig1Err[iy][ipt]);
      h2D_sigmaSig2->SetBinContent(tmpbin, sigmaSig2[iy][ipt]);
      h2D_sigmaSig2->SetBinError(tmpbin, sigmaSig2Err[iy][ipt]);
      h2D_alpha->SetBinContent(tmpbin, alpha[iy][ipt]);
      h2D_alpha->SetBinError(tmpbin, alphaErr[iy][ipt]);
      h2D_enne->SetBinContent(tmpbin, enne[iy][ipt]);
      h2D_enne->SetBinError(tmpbin, enneErr[iy][ipt]);
      h2D_sigWidth->SetBinContent(tmpbin, sigWidth[iy][ipt]);
      h2D_sigWidth->SetBinError(tmpbin, sigWidthErr[iy][ipt]);
      h2D_sigmaNPTrue->SetBinContent(tmpbin, sigmaNPTrue[iy][ipt]);
      h2D_sigmaNPTrue->SetBinError(tmpbin, sigmaNPTrueErr[iy][ipt]);
      h2D_coefExpNPTrue->SetBinContent(tmpbin, coefExpNPTrue[iy][ipt]);
      h2D_coefExpNPTrue->SetBinError(tmpbin, coefExpNPTrueErr[iy][ipt]);
      h2D_fracRes->SetBinContent(tmpbin, fracRes[iy][ipt]);
      h2D_fracRes->SetBinError(tmpbin, fracResErr[iy][ipt]);
      h2D_meanPRResW->SetBinContent(tmpbin, meanPRResW[iy][ipt]);
      h2D_meanPRResW->SetBinError(tmpbin, meanPRResWErr[iy][ipt]);
      h2D_meanPRResN->SetBinContent(tmpbin, meanPRResN[iy][ipt]);
      h2D_meanPRResN->SetBinError(tmpbin, meanPRResNErr[iy][ipt]);
      h2D_sigmaPRResW->SetBinContent(tmpbin, sigmaPRResW[iy][ipt]);
      h2D_sigmaPRResN->SetBinError(tmpbin, sigmaPRResNErr[iy][ipt]);
      h2D_fracCtBkg1->SetBinContent(tmpbin, fracCtBkg1[iy][ipt]);
      h2D_fracCtBkg1->SetBinError(tmpbin, fracCtBkg1Err[iy][ipt]);
      h2D_fracCtBkg2->SetBinContent(tmpbin, fracCtBkg2[iy][ipt]);
      h2D_fracCtBkg2->SetBinError(tmpbin, fracCtBkg2Err[iy][ipt]);
      h2D_fracCtBkg3->SetBinContent(tmpbin, fracCtBkg3[iy][ipt]);
      h2D_fracCtBkg3->SetBinError(tmpbin, fracCtBkg3Err[iy][ipt]);
      h2D_lambdam->SetBinContent(tmpbin, lambdam[iy][ipt]);
      h2D_lambdam->SetBinError(tmpbin, lambdamErr[iy][ipt]);
      h2D_lambdap->SetBinContent(tmpbin, lambdap[iy][ipt]);
      h2D_lambdap->SetBinError(tmpbin, lambdapErr[iy][ipt]);
      h2D_lambdasym->SetBinContent(tmpbin, lambdasym[iy][ipt]);
      h2D_lambdasym->SetBinError(tmpbin, lambdasymErr[iy][ipt]);
      h2D_NLL->SetBinContent(tmpbin, NLL[iy][ipt]);
      h2D_Resolution->SetBinContent(tmpbin, Resolution[iy][ipt]);
      h2D_Resolution->SetBinError(tmpbin, ResolutionErr[iy][ipt]);
      
      h2D_UnNormChi2_mass->SetBinContent(tmpbin, UnNormChi2_mass[iy][ipt]);
      h2D_nFitParam_mass->SetBinContent(tmpbin, nFitParam_mass[iy][ipt]);
      h2D_nFullBinsPull_mass->SetBinContent(tmpbin, nFullBinsPull_mass[iy][ipt]);
      h2D_Dof_mass->SetBinContent(tmpbin, Dof_mass[iy][ipt]);
      h2D_Chi2_mass->SetBinContent(tmpbin, Chi2_mass[iy][ipt]);
      h2D_theChi2Prob_mass->SetBinContent(tmpbin, theChi2Prob_mass[iy][ipt]);
      h2D_UnNormChi2_time->SetBinContent(tmpbin, UnNormChi2_time[iy][ipt]);
      h2D_nFitParam_time->SetBinContent(tmpbin, nFitParam_time[iy][ipt]);
      h2D_nFullBinsPull_time->SetBinContent(tmpbin, nFullBinsPull_time[iy][ipt]);
      h2D_Dof_time->SetBinContent(tmpbin, Dof_time[iy][ipt]);
      h2D_Chi2_time->SetBinContent(tmpbin, Chi2_time[iy][ipt]);
      h2D_theChi2Prob_time->SetBinContent(tmpbin, theChi2Prob_time[iy][ipt]);
      h2D_UnNormChi2_side->SetBinContent(tmpbin, UnNormChi2_side[iy][ipt]);
      h2D_nFitParam_side->SetBinContent(tmpbin, nFitParam_side[iy][ipt]);
      h2D_nFullBinsPull_side->SetBinContent(tmpbin, nFullBinsPull_side[iy][ipt]);
      h2D_Dof_side->SetBinContent(tmpbin, Dof_side[iy][ipt]);
      h2D_Chi2_side->SetBinContent(tmpbin, Chi2_side[iy][ipt]);
      h2D_theChi2Prob_side->SetBinContent(tmpbin, theChi2Prob_side[iy][ipt]);
    }
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////
  //// Save as a root file
  TFile *outFile = new TFile(Form("fitResHist_%s.root",szFinal.Data()),"RECREATE");
  std::cout << "szFinal: " << szFinal << std::endl;
  
  outFile->cd();
  h2D_ctErrmin->Write();
  h2D_ctErrmax->Write();
  h2D_NoCutEntry->Write();
  h2D_CutEntry->Write();
  h2D_CutRatio->Write();
  h2D_ctErrmax->Write();
  h2D_nSig_Raw->Write();
  h2D_nBkg_Raw->Write();
  h2D_nPrompt_Raw->Write();
  h2D_nNonPrompt_Raw->Write();
  h2D_nSig->Write();
  h2D_nBkg->Write();
  h2D_nPrompt->Write();
  h2D_nNonPrompt->Write();
  h2D_bFraction->Write();
  
  TDirectoryFile *otherParam = new TDirectoryFile("otherParam","otherParam");
  otherParam->cd();
  h2D_coefExp->Write();
  h2D_coefPol->Write();
  h2D_fracG1->Write();
  h2D_meanSig->Write();
  h2D_sigmaSig1->Write();
  h2D_sigmaSig2->Write();
  h2D_alpha->Write();
  h2D_enne->Write();
  h2D_sigWidth->Write();
  h2D_sigmaNPTrue->Write();
  h2D_coefExpNPTrue->Write();
  h2D_fracRes->Write();
  h2D_meanPRResW->Write();
  h2D_meanPRResN->Write();
  h2D_sigmaPRResW->Write();
  h2D_sigmaPRResN->Write();
  h2D_fracCtBkg1->Write();
  h2D_fracCtBkg2->Write();
  h2D_fracCtBkg3->Write();
  h2D_lambdam->Write();
  h2D_lambdap->Write();
  h2D_lambdasym->Write();
  h2D_NLL->Write();
  h2D_Resolution->Write();
  otherParam->Write();
  
  outFile->cd();
  TDirectoryFile *chi2Prob = new TDirectoryFile("chi2Prob","chi2Prob");
  chi2Prob->cd();
  h2D_UnNormChi2_mass->Write();
  h2D_nFitParam_mass->Write();
  h2D_nFullBinsPull_mass->Write();
  h2D_Dof_mass->Write();
  h2D_Chi2_mass->Write();
  h2D_theChi2Prob_mass->Write();
  h2D_UnNormChi2_time->Write();
  h2D_nFitParam_time->Write();
  h2D_nFullBinsPull_time->Write();
  h2D_Dof_time->Write();
  h2D_Chi2_time->Write();
  h2D_theChi2Prob_time->Write();
  h2D_UnNormChi2_side->Write();
  h2D_nFitParam_side->Write();
  h2D_nFullBinsPull_side->Write();
  h2D_Dof_side->Write();
  h2D_Chi2_side->Write();
  h2D_theChi2Prob_side->Write();
  chi2Prob->Write();
 
  outFile->Close();
  
  return 0;

} // end of main func

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

void formRapStr(Double_t min, Double_t max, string* arr) { //byHand KYO
  //if (min==-2.4 || min==2.4) *arr = Form("%.1f-%.2f",min,max);
  //else if (max==-2.4 || max==2.4) *arr = Form("%.2f-%.1f",min,max);
  *arr = Form("%.2f-%.2f",min,max);
}
void formStr(Double_t min, Double_t max, string* arr) {
  *arr = Form("%.1f-%.1f",min,max);
}

