#include "../../SONGKYO.h"

double totFunc(double *x, double *par);
using namespace std;

void checkSFRatio(){

	gROOT->Macro("../../Style.C");

	const int nEta = 5;
	
  TFile* f1 = new TFile("output_pp_5eta_cutG_all_nominal_v10.root","READ"); //20160816 (L2Mu2 || L3Mu3)
	TFile *f2 = new TFile("output_pp_5eta_cutG_all_L2Single_v8.root","READ"); //20160803 (wrong : L2Mu3 || L2Mu5 || LuMu7)

	TString sz1 = "L2Mu2_L3Mu3";
	TString sz2 = "L2Mu3_L2Mu5_L2Mu7";

	double sfmin = 0.6;
	double sfmax = 2.0;
	double ratiomin = 0.5;
	double ratiomax = 1.5;

	double etabin_new[] = {0.0, 0.9, 1.2, 1.6, 2.1, 2.4};
	double ptmin_newcut_neweta[] = {3.3, 3.3, 2.16, 1.60, 1.3};
	double ptmax = 15.;

	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);
	TLegend *legUR = new TLegend(0.47, 0.70, 0.86, 0.85);
	SetLegendStyle(legUR);

	TCanvas*c1 = new TCanvas("c1","",1500,600);
	c1->Divide(5,2);
	TH1D* h_dummy = new TH1D("h_dummy",";p_{T}^{#mu} (GeV/c);Scale Factor",100,0,ptmax);
	h_dummy->GetXaxis()->SetRangeUser(0., ptmax);
	h_dummy->GetYaxis()->SetRangeUser(sfmin, sfmax);
	TGraphAsymmErrors* tg1[nEta];
	TGraphAsymmErrors* tg2[nEta];
	TF1* tf1[nEta];
	TF1* tf2[nEta];
	string sz_tf1[nEta];
	string sz_tf2[nEta];
	for (Int_t ir=0; ir<nEta; ir++){
		c1->cd(ir+1);
		h_dummy->Draw();
		//// 1) TGraphAsymmErrors : points
		tg1[ir] = (TGraphAsymmErrors*)f1->Get(Form("eff_ratio_MuIdAndTrig_etaBin%d",ir+1));
		tg2[ir] = (TGraphAsymmErrors*)f2->Get(Form("eff_ratio_MuIdAndTrig_etaBin%d",ir+1));
		tg1[ir]->GetListOfFunctions()->Delete(Form("func_%d",ir));
		tg2[ir]->GetListOfFunctions()->Delete(Form("func_%d",ir));
		SetGraphStyle(tg1[ir],1,10);
		SetGraphStyle(tg2[ir],2,10);
		//tg1[ir]->Draw("P"); //withPnt
		//tg2[ir]->Draw("P same"); //withPnt
		
    //// 2) TF1 : fit functions
		tf1[ir] = (TF1*)f1->Get(Form("func_%d",ir+1));
		tf2[ir] = (TF1*)f2->Get(Form("func_%d",ir+1));
		// option "p" to get the express with fixed parameters
		sz_tf1[ir] = tf1[ir]->GetExpFormula("p");
		sz_tf2[ir] = tf2[ir]->GetExpFormula("p");
		cout << ir<<"th tf1 GetExpFormula : " << sz_tf1[ir]<< endl;
		cout << ir<<"th tf2 GetExpFormula : " << sz_tf2[ir]<< endl;
		tf1[ir]->SetRange(ptmin_newcut_neweta[ir],ptmax);
		tf2[ir]->SetRange(ptmin_newcut_neweta[ir],ptmax);
		tf1[ir]->SetLineColor(kRed);
		tf1[ir]->SetLineWidth(1);
		tf2[ir]->SetLineColor(kBlue);
		tf2[ir]->SetLineWidth(1);
		tf1[ir]->Draw("same");
		tf2[ir]->Draw("same");
		if (ir==0){
			legUR->AddEntry(tf1[ir],sz1.Data(),"l");
			legUR->AddEntry(tf2[ir],sz2.Data(),"l");
			legUR->Draw();
		}
		latex->DrawLatex(0.54, 0.55, Form("%.1f < |#eta^{#mu}| < %.1f",etabin_new[ir],etabin_new[ir+1]));
		dashedLine(0,1,ptmax,1);
		dashedLine(ptmin_newcut_neweta[ir],sfmin,ptmin_newcut_neweta[ir],sfmax,1,0.5);
	}
	
	//// ratio
	TH1D* h_dummy_2 = new TH1D("h_dummy_2",";p_{T}^{#mu} (GeV/c);Ratio of SFs",100,0,ptmax);
	h_dummy_2->GetXaxis()->SetRangeUser(0., ptmax);
	TF1* tfratio[nEta];
	for (Int_t ir=0; ir<nEta; ir++){
		c1->cd(ir+6);
		h_dummy_2->GetYaxis()->SetRangeUser(ratiomin, ratiomax);
		h_dummy_2->Draw();
		tfratio[ir] = new TF1(Form("tfratio_%d",ir+1),Form("(%s)/(%s)",sz_tf1[ir].c_str(),sz_tf2[ir].c_str()),ptmin_newcut_neweta[ir],ptmax);
		tfratio[ir]->SetLineColor(kRed+1);
		tfratio[ir]->SetLineWidth(1);
		tfratio[ir]->Draw("same");
		dashedLine(0,1,ptmax,1);
		dashedLine(ptmin_newcut_neweta[ir],ratiomin,ptmin_newcut_neweta[ir],ratiomax,1,0.5);
	}

	c1->SaveAs(Form("SFRatio_%s_VS_%s.pdf",sz1.Data(),sz2.Data()));
	return;
}

double totFunc(double *x, double *par){
	return 1;	
}
