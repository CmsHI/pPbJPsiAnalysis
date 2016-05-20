#include "../SONGKYO.h"

void formRapStr(Double_t min, Double_t max, string* arr);
void formStr(Double_t min, Double_t max, string* arr);

/////// main func. ///////
int make_fitResHist(int MrapNpt=89, int isPA =3, int accCutType=1, TString szSys="nominal"){
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
  //std::ifstream fparam(Form("./fitRes_%s/summary_%s/fit_parameters",szBinning,szPA),std::ios::in);
  //if(!fparam.is_open()) { cout << "Warning : can NOT open the fit_parameters file!"<<endl; }

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

  ////// read ctauErr txt
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
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////
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

