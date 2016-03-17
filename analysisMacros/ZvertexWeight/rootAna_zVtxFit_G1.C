#include "SONGKYO.h"
#include <TChain.h>
#include <TF1.h>

double tfGaus(double *x, double *par);
double tf2Gaus(double *x, double *par);
double tfDivideGaus(double *x, double *par);
double tfLine(double *x, double *par);

int rootAna_zVtxFit_G1(char *stringA = "G1", int isPA = 0, bool isPrompt = true, bool isNewAccCut = true, bool isOfficial = false){

	gROOT->Macro("./Style.C");
	double nbin = 60;
	double binmax = 30;
	double fitmax1 = 8;
	double fitmax2 = 15;

	char* szPA;
	if (isPA==0) szPA = "pp";
	else if (isPA==1) szPA = "Pbp";
	else if (isPA==2) szPA = "pPb";
	else { cout << "select isPA among 0, 1 and 2 " << endl; return 0; }

	////// read-in root file
	TChain *tree01;
	TChain *tree02;
	if (isPA==0) tree01 = new TChain("hionia/myTree");
	else tree01 = new TChain("myTree");
	if (isPA==0) tree02 = new TChain("hionia/myTree");
	else tree02 = new TChain("myTree");
	
	if (isPA==0) { //pp
		tree01->Add("/home/songkyo/kyo/ppDataSample/Data/OniaTree_DoubleMu_Run2015E-PromptReco-v1_Run_262157_262328_noCUT_TRKMU.root");
		if (isPrompt) {
			if (isOfficial) { 
				tree02->Add("");
			}
			else {
				tree02->Add("/home/songkyo/kyo/ppDataSample/EfficiencySamplePrivate/OniaTree_JpsiMM_pp5p02TeV_TuneCUETP8M1_Trk_noCUT.root");
			}
		}
		else {
			if (isOfficial) { 
					tree02->Add("");
			}
			else {
				tree02->Add("/home/songkyo/kyo/ppDataSample/EfficiencySamplePrivate/OniaTree_BJpsiMM_pp5p02TeV_TuneCUETP8M1_Trk_noCUT.root");
			}
		}
	}
	else if (isPA==1) { //Pbp 1st run
		if (!isNewAccCut) {
			tree01->Add("/home/songkyo/kyo/pPbDataSample/Data_old/merged_pPbData_1st_ntuple_PromptReco-v1_GR_P_V43D_pileupRej_muID_tot.root");
			tree01->Add("/home/songkyo/kyo/pPbDataSample/Data_old/merged_pPbData_1st_ntuple_ReprocessedReco-v1_GR_P_V43F_pileupRej_muID_tot.root");
		}
		else {
			tree01->Add("/home/songkyo/kyo/pPbDataSample/Data/pPbData_1st_ntuple_PromptReco-v1_GR_P_V43D_pileupRej_newAccCut_tot.root");
			tree01->Add("/home/songkyo/kyo/pPbDataSample/Data/pPbData_1st_ntuple_PromptReco-v1_GR_P_V43F_pileupRej_newAccCut_tot.root");
		}
		if (isPrompt) {
			if (isOfficial) { 
				tree02->Add("/home/songkyo/kyo/pPbDataSample/EfficiencySample/tot_JPsiWithFSR_pa_1st_run_STARTHI53_V27-v1_noAccCut_sglTrig_genMatch-v2.root");
			}
			else {
				tree02->Add("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/tot_PromptJpsi_PYTHIAboosted_1st_STARTHI53_V27_noAccCut_sglTrig_genMatch-v2.root");
			}
		}
		else {
			if (isOfficial) { 
					tree02->Add("/home/songkyo/kyo/pPbDataSample/EfficiencySample/tot_inclBtoJPsiMuMu_pa_1st_run_STARTHI53_V27-v1_noAccCut_sglTrig_genMatch-v2.root");
			}
			else {
				tree02->Add("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/tot_B2Jpsi_PYTHIAboosted_1st_STARTHI53_V27_noAccCut_sglTrig_genMatch-v2.root");
			}
		}
	}
	else if (isPA==2) { //pPb 2nd run
		if (!isNewAccCut) {
			tree01->Add("/home/songkyo/kyo/pPbDataSample/Data_old/merged_pPbData_2nd_ntuple_PromptReco-v1_GR_P_V43D_pileupRej_muID_tot.root");
		}
		else {
			tree01->Add("/home/songkyo/kyo/pPbDataSample/Data/pPbData_2nd_PromptReco-v1_GR_P_V43D_pileupRej_newAccCut_tot.root");
		}
		if (isPrompt) {
			if (isOfficial) { 
				tree02->Add("/home/songkyo/kyo/pPbDataSample/EfficiencySample/tot_JPsiWithFSR_pa_2nd_run_STARTHI53_V27-v1_noAccCut_sglTrig_genMatch-v1.root");
			}
			else {
				tree02->Add("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/tot_PromptJpsi_PYTHIAboosted_2nd_STARTHI53_V27_noAccCut_sglTrig_genMatch-v2.root");
			}
		}
		else {
			if (isOfficial) { 
				tree02->Add("/home/songkyo/kyo/pPbDataSample/EfficiencySample/tot_inclBtoJPsiMuMu_pa_2nd_run_STARTHI53_V27-v1_noAccCut_sglTrig_genMatch-v1.root");
			}
			else {
				tree02->Add("/home/songkyo/kyo/pPbDataSample/EfficiencySamplePrivate/tot_B2Jpsi_PYTHIAboosted_2nd_STARTHI53_V27_noAccCut_sglTrig_genMatch-v2.root");
			}
		}
	}
	
	//TTree* tree01;
	//tree01 = (TTree*)f01->Get("hionia/myTree");
	//if ( tree01 == 0 ){ tree01 = (TTree*)f01->Get("myTree");}
	//TTree* tree02;
	//tree02 = (TTree*)f02->Get("hionia/myTree");
	//if ( tree02 == 0 ){ tree02 = (TTree*)f02->Get("myTree");}
	
	TCanvas* c_tmp = new TCanvas("c_tmp","",300,300); c_tmp->SetGridx(); c_tmp->SetGridy();
	TCanvas* c01 = new TCanvas("c01","distributions",600,600); c01->SetGridx(); c01->SetGridy();
	TCanvas* c02 = new TCanvas("c02","ratios",600,600); c02->SetGridx(); c02->SetGridy();

	cout << "nbin = " << nbin << endl;
	TH1D* h1D_01 = new TH1D("h1D_01","zVtx;Z_{vtx} [cm];events;",nbin,-binmax,binmax);
	TH1D* h1D_02 = new TH1D("h1D_02","zVtx;Z_{vtx} [cm];events;",nbin,-binmax,binmax);
	h1D_01->Sumw2();
	h1D_02->Sumw2();
	
	TCut trigCut = "((Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )";
	TCut recoCut = "Reco_QQ_size>=1 && Reco_QQ_sign==0";
	TCut massNarrow = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
	TCut massWide = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
	TCut rooDataCut = "Reco_QQ_ctau > -3.0 && Reco_QQ_ctau < 5.0 && Reco_QQ_ctauErr > 0. && Reco_QQ_ctauErr < 1."; // + sign, massWide, pT, y
	TCut rapRange = "Reco_QQ_4mom.Rapidity() > -2.4 && Reco_QQ_4mom.Rapidity() < 2.4";
	//TCut rapRange = "Reco_QQ_4mom.Rapidity() > -2.4 && Reco_QQ_4mom.Rapidity() < -1.97";
	//TCut rapRange = "Reco_QQ_4mom.Rapidity() > 1.03 && Reco_QQ_4mom.Rapidity() < 1.46";
	//TCut ptRange = "Reco_QQ_4mom.Pt() > 0.0 && Reco_QQ_4mom.Pt() < 30.0";
	TCut ptRange = "Reco_QQ_4mom.Pt() > 0.0 && Reco_QQ_4mom.Pt() < 100.0";
	//TCut ptRange = "Reco_QQ_4mom.Pt() > 5.0 && Reco_QQ_4mom.Pt() < 30.0";
	//TCut ptRange = "Reco_QQ_4mom.Pt() > 5.0 && Reco_QQ_4mom.Pt() < 6.5";

	//// old cut
 TCut accOldRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 1.3 && Reco_QQ_mupl_4mom.Pt() >=3.3) || (1.3 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.2 && Reco_QQ_mupl_4mom.P() >=2.9) || (2.2 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt() >= 0.8)))";
TCut accOldRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 1.3 && Reco_QQ_mumi_4mom.Pt() >=3.3) || (1.3 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.2 && Reco_QQ_mumi_4mom.P() >=2.9) || (2.2 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt() >= 0.8)))";
	//// new cut
	TCut accNewRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 1.2 && Reco_QQ_mupl_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.1 && Reco_QQ_mupl_4mom.Pt() >= -(1.0/0.9)*TMath::Abs(Reco_QQ_mupl_4mom.Eta())+(1.2*(1.0/0.9)+2.6)) || (2.1 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt() >= 1.3)))";
	TCut accNewRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 1.2 && Reco_QQ_mumi_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.1 && Reco_QQ_mumi_4mom.Pt() >= -(1.0/0.9)*TMath::Abs(Reco_QQ_mumi_4mom.Eta())+(1.2*(1.0/0.9)+2.6)) || (2.1 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt() >= 1.3)))";

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//// no cut at all
	TCut totalCut = "";
	//// no acc cut
	//totalCut = trigCut;
	//totalCut = trigCut && recoCut && massNarrow && rapRange && ptRange;
	//if (!isNewAccCut) { totalCut = trigCut && recoCut && massWide && rapRange && ptRange && accOldRecoPlus && accOldRecoMinus; }
	//else { totalCut = trigCut && recoCut && massWide && rapRange && ptRange && accNewRecoPlus && accNewRecoMinus; }
	if (!isNewAccCut) { totalCut = trigCut && recoCut && massNarrow && rooDataCut && rapRange && ptRange && accOldRecoPlus && accOldRecoMinus; }
	else { totalCut = trigCut && recoCut && massNarrow && rooDataCut && rapRange && ptRange && accNewRecoPlus && accNewRecoMinus; }
	cout << "totalCut = " << totalCut << endl;
	////////////////////////////////////////////////////////////////////////////////////////////////////

	c_tmp->cd();	
	const int nEntry = 20000;
//	tree01->Draw("zVtx>>h1D_01",totalCut,"colz",nEntry);
//	tree02->Draw("zVtx>>h1D_02",totalCut,"colz",nEntry);
	tree01->Draw("zVtx>>h1D_01",totalCut,"colz");
	tree02->Draw("zVtx>>h1D_02",totalCut,"colz");

	h1D_01->Scale(1,"width");	
	h1D_02->Scale(1,"width");	
	h1D_01->Scale(1./h1D_01->Integral());	
	h1D_02->Scale(1./h1D_02->Integral());	
	
	//define fitting function
//	TF1* g1 = new TF1("g1",tfGaus,-binmax, binmax,3);
//	TF1* g2 = new TF1("g2",tfGaus,-binmax, binmax,3);
//	TF1* g3 = new TF1("g3",tfGaus,-binmax, binmax,3);
	TF1* g1 = new TF1("g1","[0]*TMath::Exp(-0.5*(((x-([1]))*(x-([1])))/(([2])*([2]))))",-binmax, binmax);
	TF1* g2 = new TF1("g2","[0]*TMath::Exp(-0.5*(((x-([1]))*(x-([1])))/(([2])*([2]))))",-binmax, binmax);
	TF1* g3 = new TF1("g3","[0]*TMath::Exp(-0.5*(((x-([1]))*(x-([1])))/(([2])*([2]))))",-binmax, binmax);
	Double_t par01[3]; //for h01
	Double_t par02[3]; //for h02
	//Double_t par03[3]; //for Ratio (not used)
	
	TLegend *legU1 = new TLegend(0.17,0.79,0.50,0.93,NULL,"brNDC");
	SetLegendStyle(legU1);
	//legU-> SetNColumns(2);
	TLegend *legU2 = new TLegend(0.63,0.79,0.90,0.93,NULL,"brNDC");
	SetLegendStyle(legU2);
	
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//fit
	g1->SetParameters(0.05,0.,5.);
	g1->SetParLimits(0, 0.001, 0.1);
	g1->SetParLimits(1, -2.,2.);
	g1->SetParLimits(2, 0.,10);
	//g1->SetParLimits(1, -10.,10.);
	//g1->SetParLimits(2, 0.,200);
	//h1D_01->Fit("g1", "N");
	h1D_01->Fit("g1", "N","",-fitmax1,fitmax1);
	g2->SetParameters(0.001,0,10.);
	g2->SetParLimits(0, 0.01, 0.1);
	g2->SetParLimits(1, -3.,3.);
	g2->SetParLimits(2, 0.,15.);
	//g2->SetParLimits(1, -10.,10.);
	//g2->SetParLimits(2, 0.,200.);
	//h1D_02->Fit("g2", "N");
	h1D_02->Fit("g2", "N","",-fitmax2,fitmax2);

	//Draw and Fit
	c01->cd();
	gPad->SetLogy(0);
	SetHistStyle(h1D_01,1,10);
	SetHistStyle(h1D_02,2,13);
	h1D_01->SetFillColor(kRed+2);
//	h1D_01->SetFillStyle(3004);
	h1D_02->SetFillColor(kBlue+1);
//	h1D_02->SetFillStyle(3005);
	//h1D_01->SetAxisRange(0.0,0.08,"Y");
	h1D_01->SetAxisRange(0.0,0.1,"Y");

	h1D_01->Draw("pe");
	g1->SetLineColor(kRed+2);
	g1->Draw("same");
	h1D_02->Draw("pe same");
	g2->SetLineColor(kBlue+1);
	g2->Draw("same");
		
	legU1->AddEntry(h1D_01,Form("%s_Data",szPA),"lp");
	if (isPrompt) legU2->AddEntry(h1D_02,Form("%s_PRMC",szPA),"lp");
	legU1->Draw();
	legU2->Draw();
	
	g1->GetParameters(&par01[0]);
	g2->GetParameters(&par02[0]);
	latex->DrawLatex(0.20, 0.80, Form("mean : %.2f",par01[1]));	
	latex->DrawLatex(0.20, 0.74, Form("sigma : %.2f",par01[2]));
	latex->DrawLatex(0.67, 0.80, Form("mean : %.2f",par02[1]));	
	latex->DrawLatex(0.67, 0.74, Form("sigma : %.2f",par02[2]));
		
	c01->SaveAs(Form("zVtx201603/zVtxFit_%s_%s_isPrompt%d_isNewAccCut%d_isOfficial%d.pdf",stringA,szPA,(int)isPrompt,(int)isNewAccCut,(int)isOfficial));
	//c01->Clear();
	//legU->Clear();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// divide two TF
	c02->cd();
	// option "p" to get the express with fixed parameters
	string sz_tf1;
	string sz_tf2;
	sz_tf1 = g1->GetExpFormula("p");
	sz_tf2 = g2->GetExpFormula("p");
	cout << "g1 GetExpFormula : " << sz_tf1<< endl;
	cout << "g2 GetExpFormula : " << sz_tf2<< endl;
	//TF1* gRatio = new TF1("gRatio",tfDivideGaus,-binmax, binmax,6);
	//gRatio->SetParameters(par01[0],par01[1],par01[2],par02[0],par02[1],par02[2]);
	TF1* gRatio = new TF1("gRatio",Form("(%s)/(%s)",sz_tf1.c_str(),sz_tf2.c_str()),-binmax,binmax);
	gRatio->SetLineColor(kViolet+2);

	//ratio
	gPad->SetLogy(0);
	TH1D* hRatio = (TH1D*)h1D_01->Clone("hRatio");
	hRatio->Divide(h1D_02);
	hRatio->GetYaxis()->SetRangeUser(0.,2.0);
	hRatio->GetYaxis()->SetTitle("Ratio = Data/MC");
	SetHistStyle(hRatio,1,0);
	hRatio->SetMarkerSize(1.4);
	hRatio->SetMarkerColor(kBlack);
	hRatio->SetLineColor(kBlack);
	hRatio->Draw("pe");
/*	
	//just cross check w/ gaussian : fit the ratio itself
	hRatio->Fit("gCommon", "N");
	gCommon->GetParameters(&parC[0]);
	g3->SetParLimits(0, parC[0]-0.01,parC[0]+0.01);
	g3->SetParLimits(1, parC[1]-0.01,parC[1]+0.01);
	g3->SetParLimits(2, parC[2]-0.01,parC[2]+0.01);
	g3->SetLineColor(kBlack);
	hRatio->Fit("g3","N");
	g3->GetParameters(&par03[0]);
	//latex->DrawLatex(0.20, 0.77, Form("mean : %.2f",par03[1]));	
	//latex->DrawLatex(0.20, 0.71, Form("sigma : %.2f",par03[2]));
	g3->Draw("same");
*/	
	gRatio->Draw("same");
	
	dashedLine(-binmax,1.,binmax,1.,1,1);
	
	c02->SaveAs(Form("zVtx201603/zVtxFitRatio_%s_%s_isPrompt%d_isNewAccCut%d_isOfficial%d.pdf",stringA,szPA,(int)isPrompt,(int)isNewAccCut,(int)isOfficial));

	//Save as a root file
	//TFile *outFile = new TFile(Form("zVtxFit_%s.root",stringA),"RECREATE");		
	TFile* outFile = new TFile(Form("zVtx201603/zVtx_%s_%s_isPrompt%d_isNewAccCut%d_isOfficial%d.root",stringA,szPA,(int)isPrompt,(int)isNewAccCut,(int)isOfficial),"RECREATE");
	outFile->cd();
	h1D_01->Write();
	h1D_02->Write();
	hRatio->Write();
	g1->Write();
	g2->Write();
	gRatio->Write();
	outFile->Close();

	return 0;	

}

double tfGaus(double *x, double *par) {
	return par[0]*exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2]) ) );
}
double tf2Gaus(double *x, double *par) {
	return par[0]*exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2])) ) + par[3]*exp( -0.5*( ((x[0]-par[4])*(x[0]-par[4]))/(par[5]*par[5])) );
}

double tfDivideGaus(double *x, double *par) {
	return (par[0]*exp( -0.5*( ((x[0]-par[1])*(x[0]-par[1]))/(par[2]*par[2]) ) )) / (par[3]*exp( -0.5*( ((x[0]-par[4])*(x[0]-par[4]))/(par[5]*par[5]) )) );
}


double tfLine(double *x, double *par) {
	return par[0]*x[0]+par[1];
}
