#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formRapLabArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void ctauerr_differential(TString szPA = "pa", bool isSmall =true, bool isAddNP=false, double rapmin=-1.47, double rapmax=-0.47, double ptmin=6.5, double ptmax=7.5)
{
  gROOT->Macro("../Style.C");
	//gStyle->SetPaintTextFormat(".3f");
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

  const int nEntry = 300000; //for isSmall 
  //const int nEntry = 5000; //for isSmall 

	//double nbin = 120;
	double nbin = 100;
  double binmax = 1.;
	//double binmax = 0.2;
  if (ptmin==0.0) { binmax=0.5; } //integ
  else if (ptmin==2.0 || ptmin==3.0 || ptmin==4.0 || ptmin==5.0) { binmax=0.5; }
  else { binmax=0.2; }
//  else if (ptmin==2.0 || ptmin==3.0 || ptmin==4.0) { binmax=0.5; }
//  else if (ptmin==5.0 || ptmin==6.5 || ptmin==7.5) { binmax=0.3; }
//  else if (ptmin==8.5 || ptmin==10.0 || ptmin==14.0) { binmax=0.1; }
  //binmax = 0.12; // tmp
	
  //// read-in root file
  TFile *fData;
  TFile *fPRMC;
  TFile *fNPMC;
  TTree *treeData;
  TTree *treePRMC;
  TTree *treeNPMC;
  if (strcmp(szPA,"pp")==0) {
    fData = new TFile("/storage/OniaTree/Onia5TeV/ppData/OniaTree_DoubleMu_Run2015E-PromptReco-v1_Run_262157_262328_noCUT_TRKMU.root");
    fPRMC = new TFile("/storage/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");
    fNPMC = new TFile("/storage/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_BJpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");
    treeData = (TTree*)fData->Get("hionia/myTree");
    treePRMC = (TTree*)fPRMC->Get("hionia/myTree");
    treeNPMC = (TTree*)fNPMC->Get("hionia/myTree");
  } else if (strcmp(szPA,"pa")==0) {
    fData = new TFile("/home/songkyo/kyo/pPbDataSample/Data/RD2013_pa_1st_run_210676-211256_GR_P_V43D_nocut.root");
    fPRMC = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCJPsiWithFSR_pa_1st_run_STARTHI53_V27_ext1_nocut.root");
    fNPMC = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCinclBtoJPsiMuMu_pa_1st_run_STARTHI53_V27_ext1_nocut.root");
    treeData = (TTree*)fData->Get("myTree");
    treePRMC = (TTree*)fPRMC->Get("myTree");
    treeNPMC = (TTree*)fNPMC->Get("myTree");
  }
  else { cout << "WARNING :: Select among szPA = \"pp\" or \"pa\" "; return; }
  cout << "szPA : " << szPA <<endl;

/*
  ////rap array in yCM (from forward to backward)
  const Int_t nRap = 8; 
  const Int_t nPt = 9; 
  const int nRapTmp = nRap +1; 
  const int nPtTmp = nPt +1; 
  Double_t rapArrLab[nRapTmp];
  Double_t rapArrLabPA[nRapTmp] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4};
  Double_t rapArrLabPP[nRapTmp] = {-2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93, 2.4};
  for (int iy=0; iy<nRapTmp; iy++){
    if (strcmp(szPA,"pp")==0) { rapArrLab[iy]=rapArrLabPP[iy];}
    else { rapArrLab[iy]=rapArrLabPA[iy];}
  } 
  ////pt array
  Double_t ptArrNum[10] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
  //// array string
  TString rapArr[nRap];
  for (Int_t iy=0; iy<nRap; iy++) {
    formRapLabArr(rapArrLab[iy], rapArrLab[iy+1], &rapArr[iy]);
    cout << iy <<"th rapArr = " << rapArr[iy] << endl;
  }
  TString ptArr[nPt];
  for (Int_t ipt=0; ipt<nPt; ipt++) {
    formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
    cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
  }
*/
  TString rapArr;
  formRapLabArr(rapmin,rapmax,&rapArr);
  cout << "rapArr : " << rapArr << endl; 
  TString ptArr;
  formPtArr(ptmin,ptmax,&ptArr);
  cout << "ptArr : " << ptArr << endl; 

  ///////////////////////////////////////////////////////////////////
  //// cut definitions
  TCut trigCut = "((Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )";
  TCut recoPairCut = "Reco_QQ_sign==0 && Reco_QQ_VtxProb > 0.01";  
  TCut massNarrow = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
  TCut massWide = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
  TCut ctauCut = "-1.5 < Reco_QQ_ctau && Reco_QQ_ctau < 3.0 && 0 < Reco_QQ_ctauErr && Reco_QQ_ctauErr < 1";
  TCut rapRange = Form("Reco_QQ_4mom.Rapidity() > %f && Reco_QQ_4mom.Rapidity() < %f",rapmin,rapmax);
  TCut ptRange = Form("Reco_QQ_4mom.Pt() > %f && Reco_QQ_4mom.Pt() < %f",ptmin,ptmax);
	
	//// new cut
 TCut accNewRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 1.2 && Reco_QQ_mupl_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.1 && Reco_QQ_mupl_4mom.Pt() >= 3.93-1.11*TMath::Abs(Reco_QQ_mupl_4mom.Eta())) || (2.1 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt() >= 1.3)))";
  TCut accNewRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 1.2 && Reco_QQ_mumi_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.1 && Reco_QQ_mumi_4mom.Pt() >= 3.93-1.11*TMath::Abs(Reco_QQ_mumi_4mom.Eta())) || (2.1 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt() >= 1.3)))";   
  
  //// muID
  TCut muIDPlus;
  TCut muIDMinus;
  if(strcmp(szPA,"pp")==0) {
    muIDPlus = "Reco_QQ_mupl_highPurity && Reco_QQ_mupl_TrkMuArb && Reco_QQ_mupl_TMOneStaTight && Reco_QQ_mupl_nTrkWMea > 5 && Reco_QQ_mupl_nPixWMea > 0 &&  TMath::Abs(Reco_QQ_mupl_dxy) < 0.3 && TMath::Abs(Reco_QQ_mupl_dz) < 20.";
    muIDMinus = "Reco_QQ_mumi_highPurity && Reco_QQ_mumi_TrkMuArb && Reco_QQ_mumi_TMOneStaTight && Reco_QQ_mumi_nTrkWMea > 5 && Reco_QQ_mumi_nPixWMea > 0 && TMath::Abs(Reco_QQ_mumi_dxy) < 0.3 && TMath::Abs(Reco_QQ_mumi_dz) < 20.";
  }
  else if (strcmp(szPA,"pa")==0) {
    muIDPlus = "Reco_QQ_mupl_isHighPurity && Reco_QQ_mupl_TrkMuArb && Reco_QQ_mupl_TMOneStaTight && Reco_QQ_mupl_nTrkWMea > 5 && Reco_QQ_mupl_nPixWMea > 0 &&  TMath::Abs(Reco_QQ_mupl_dxy) < 0.3 && TMath::Abs(Reco_QQ_mupl_dz) < 20.";
    muIDMinus = "Reco_QQ_mumi_isHighPurity && Reco_QQ_mumi_TrkMuArb && Reco_QQ_mumi_TMOneStaTight && Reco_QQ_mumi_nTrkWMea > 5 && Reco_QQ_mumi_nPixWMea > 0 && TMath::Abs(Reco_QQ_mumi_dxy) < 0.3 && TMath::Abs(Reco_QQ_mumi_dz) < 20.";
  }
	////////////////////////////////////////////////////////////////////////////////////////////////////
	TCut totalCut = "";
  totalCut = trigCut && recoPairCut && massNarrow && ctauCut && rapRange && ptRange && accNewRecoPlus && accNewRecoMinus && muIDPlus && muIDMinus;
	cout << "total cut : " << totalCut << endl;
  ////////////////////////////////////////////////////////////////////////////////////////////////////
	
  TCanvas* c_tmp = new TCanvas("c_tmp","",300,300); c_tmp->SetGridx(); c_tmp->SetGridy();
	TCanvas* c01 = new TCanvas("c01","",600,600); c01->SetGridx(); c01->SetGridy();
	TCanvas* c02 = new TCanvas("c02","",600,600); c02->SetGridx(); c02->SetGridy();

	TH1D* h1D_01 = new TH1D("h1D_01","ctau error;J/#psi c#tau error [mm];events;",nbin,0,binmax);
	TH1D* h1D_02 = new TH1D("h1D_02","ctau error;J/#psi c#tau error [mm];events;",nbin,0,binmax);
	TH1D* h1D_03 = new TH1D("h1D_03","ctau error;J/#psi c#tau error [mm];events;",nbin,0,binmax);
	h1D_01->Sumw2();
	h1D_02->Sumw2();
	h1D_03->Sumw2();

	c_tmp->cd();	
  if (isSmall) {
  	treeData->Draw("Reco_QQ_ctauErr>>h1D_01",totalCut,"colz",nEntry);
  	treePRMC->Draw("Reco_QQ_ctauErr>>h1D_02",totalCut,"colz",nEntry);
  	if (isAddNP) {treeNPMC->Draw("Reco_QQ_ctauErr>>h1D_03",totalCut,"colz",nEntry); }
  }
  else {
  	treeData->Draw("Reco_QQ_ctauErr>>h1D_01",totalCut,"colz");
  	treePRMC->Draw("Reco_QQ_ctauErr>>h1D_02",totalCut,"colz");
  	if (isAddNP) {treeNPMC->Draw("Reco_QQ_ctauErr>>h1D_03",totalCut,"colz"); }
  }
	
	h1D_01->Scale(1,"width");	
	h1D_02->Scale(1,"width");	
	if (isAddNP) h1D_03->Scale(1,"width");	
	h1D_01->Scale(1./h1D_01->Integral());	
	h1D_02->Scale(1./h1D_02->Integral());	
	if (isAddNP) h1D_03->Scale(1./h1D_03->Integral());	
	
  SetHistStyle(h1D_01,1,0);
  SetHistStyle(h1D_02,4,0);
  SetHistStyle(h1D_03,3,0);
  h1D_02->SetFillColor(kGreen+3);
  h1D_02->SetFillStyle(3004);
  h1D_03->SetFillColor(kOrange+1);
  h1D_03->SetFillStyle(3005);
	
	TLegend *legUR = new TLegend(0.53, 0.80, 0.89, 0.95);
  SetLegendStyle(legUR);
	legUR->AddEntry(h1D_01,Form("%s Data",szPA.Data()),"pl");
	legUR->AddEntry(h1D_02,Form("%s MC prompt",szPA.Data()),"lf");
	if (isAddNP) legUR->AddEntry(h1D_03,Form("%s MC non-prompt",szPA.Data()),"lf");
	//legUR->AddEntry(h1D_01,"pPb Data","pl");
	//legUR->AddEntry(h1D_02,"pPb MC","lf");
  
  //latex box for beam, rapidity, pT info
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);
	
	c01->cd();
	gPad->SetLogy(0);
	//h1D_01->SetMinimum(0);
	//h1D_01->SetMaximum(1);
  h1D_01->Draw("pe");
	h1D_02->Draw("hist same");
	if (isAddNP) h1D_03->Draw("hist same");
	h1D_01->Draw("pe same");
 	legUR->Draw();
  latex->DrawLatex(0.55,0.75,Form("%s",rapArr.Data()));
  latex->DrawLatex(0.55,0.70,Form("%s",ptArr.Data()));
	c01->SaveAs(Form("dir_ctauerr/%s_ctauerr_addNP%d_rap%.2f-%.2f_pt%.2f-%.2f.pdf",szPA.Data(),(int)isAddNP,rapmin,rapmax,ptmin,ptmax));
	
	c02->cd();
	gPad->SetLogy(1);
	//h1D_01->SetMinimum(0.0003);
	//h1D_01->SetMaximum(0.3);
	h1D_01->Draw("pe");
	h1D_02->Draw("hist same");
	if (isAddNP) h1D_03->Draw("hist same");
	h1D_01->Draw("pe same");
  latex->DrawLatex(0.55,0.75,Form("%s",rapArr.Data()));
  latex->DrawLatex(0.55,0.70,Form("%s",ptArr.Data()));
 	legUR->Draw();
	c02->SaveAs(Form("dir_ctauerr/%s_ctauerr_log_addNP%d_rap%.2f-%.2f_pt%.2f-%.2f.pdf",szPA.Data(),(int)isAddNP,rapmin,rapmax,ptmin,ptmax));

////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	TCanvas* c_ratio = new TCanvas("c_ratio","official / private",600,200); 
	c_ratio->SetGridx(); c_ratio->SetGridy();
	c_ratio->cd();
	TH1D* hratio = (TH1D*)h1D_02->Clone("hratio"); 
	hratio->Divide(h1D_01);
	hratio->GetYaxis()->SetRangeUser(0.7,1.3); //nocut
	//hratio->GetYaxis()->SetRangeUser(0.,2.);
	hratio->SetLineColor(kRed);
	hratio->SetLineWidth(1);
	hratio->SetMarkerColor(kRed);
	hratio->SetMarkerStyle(kFullCircle);
	hratio->SetFillColor(kRed);
	hratio->Draw("pe");
	c_ratio->SaveAs(Form("plot_ctauerrcomp_diffSample/%s_ratio.pdf",dirName));
	cout << "middle = " << hratio->GetBinContent(2) << endl;
*/

  return;
}

void formRapArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < y_{CM} < %.0f", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.2f < y_{CM} < %.0f", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < y_{CM} < %.2f", binmin, binmax);
  } else {
    *arr = Form("%.2f < y_{CM} < %.2f", binmin, binmax);
  }
}

void formRapLabArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < y_{lab} < %.0f", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.2f < y_{lab} < %.0f", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < y_{lab} < %.2f", binmin, binmax);
  } else {
    *arr = Form("%.2f < y_{lab} < %.2f", binmin, binmax);
  }
}

void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.2f < |y_{CM}| < %.0f", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < |y_{CM}| < %.2f", binmin, binmax);
  } else {
    *arr = Form("%.2f < |y_{CM}| < %.2f", binmin, binmax);
  }
}

void formPtArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.1f < p_{T} < %.0f GeV/c", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < p_{T} < %.1f GeV/c", binmin, binmax);
  } else {
    *arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
  }
}

