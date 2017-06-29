#include "../../SONGKYO.h"
#include "TKey.h"

using namespace std;

void drawTNP(int isPA=0){

	gROOT->Macro("../../Style.C");

	const int nEta = 5;
	
  TFile* f1;
  if(isPA==0) f1 = new TFile("output_pp_5eta_cutG_all_nominal_v10.root","READ");
  else f1 = new TFile("output_official_5eta_cutG_all_nominal_v3.root","READ"); 

	TString sz1 = "Data";
	TString sz2 = "MC";

	double effmin = 0.0;
	double effmax = 1.1;
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
	latex->SetTextSize(0.09);
	TLegend *legUR = new TLegend(0.62, 0.44, 0.90, 0.65);
	SetLegendStyle(legUR);
  legUR->SetTextSize(0.085);

	//TCanvas*c1 = new TCanvas("c1","",400,1000);
	TCanvas*c1 = new TCanvas("c1","",600,1500);
	c1->Divide(2,5);
	TH1D* hdummy01 = new TH1D("hdummy01",";p_{T}^{#mu} (GeV/c);Efficiency",100,0,ptmax);
	TH1D* hdummy02 = new TH1D("hdummy02",";p_{T}^{#mu} (GeV/c);Scale Factor",100,0,ptmax);
	SetHistStyle(hdummy01,0,0);
	SetHistStyle(hdummy02,0,0);
  hdummy01->GetXaxis()->SetRangeUser(0., ptmax);
	hdummy01->GetYaxis()->SetRangeUser(effmin, effmax);
  hdummy02->GetXaxis()->SetRangeUser(0., ptmax);
	hdummy02->GetYaxis()->SetRangeUser(sfmin, sfmax);
	TGraphAsymmErrors* t_data[nEta];
	TGraphAsymmErrors* t_mc[nEta];
	TGraphAsymmErrors* t_ratio[nEta];
	TF1* tfunc[nEta];
	string sz_tfunc[nEta];

  //// Read root files using List
  auto list = f1-> GetListOfKeys();
  TIter nextkey(list);
  TKey *key;
  int ik=0;
  while (1) {
    key = (TKey*) nextkey.Next();
    if (key == nullptr) {break;}
    if (ik==0) t_mc[4] = (TGraphAsymmErrors*) key->ReadObj();
    else if (ik==1) t_data[4] = (TGraphAsymmErrors*) key->ReadObj();
    else if (ik==2) t_mc[3] = (TGraphAsymmErrors*) key->ReadObj();
    else if (ik==3) t_data[3] = (TGraphAsymmErrors*) key->ReadObj();
    else if (ik==4) t_mc[2] = (TGraphAsymmErrors*) key->ReadObj();
    else if (ik==5) t_data[2] = (TGraphAsymmErrors*) key->ReadObj();
    else if (ik==6) t_mc[1] = (TGraphAsymmErrors*) key->ReadObj();
    else if (ik==7) t_data[1] = (TGraphAsymmErrors*) key->ReadObj();
    else if (ik==8) t_mc[0] = (TGraphAsymmErrors*) key->ReadObj();
    else if (ik==9) t_data[0] = (TGraphAsymmErrors*) key->ReadObj();
    //auto graph = (TGraphAsymmErrors*) key->ReadObj();
    //graph -> Draw("p");  
    ik++;
    if (ik==10) break;
  }
 
  //// Draw Efficiency 
  for (Int_t ir=0; ir<nEta; ir++){
		c1->cd(ir*2+1);
//		if (ir!=0) {gPad->SetTopMargin(0);}
//		if (ir!=4) {gPad->SetBottomMargin(0);}
    gPad->SetTopMargin(0.01);
    gPad->SetBottomMargin(0.12);
    hdummy01->Draw();
    SetGraphStyle(t_data[ir],1,0);
		SetGraphStyle(t_mc[ir],2,10);
		t_data[ir]->Draw("P"); //withPnt
		t_mc[ir]->Draw("P"); //withPnt
		latex->DrawLatex(0.37, 0.25, Form("%.1f < |#eta^{#mu}| < %.1f",etabin_new[ir],etabin_new[ir+1]));
		dashedLine(0,1,ptmax,1);
		if (ir==0){
			legUR->AddEntry(t_data[ir],sz1.Data(),"lp");
			legUR->AddEntry(t_mc[ir],sz2.Data(),"lp");
			legUR->Draw();
		}
	}

  //// Draw SF
  for (Int_t ir=0; ir<nEta; ir++){
		c1->cd(ir*2+2);
    gPad->SetTopMargin(0.01);
    gPad->SetBottomMargin(0.12);
		hdummy02->Draw();
		t_ratio[ir] = (TGraphAsymmErrors*)f1->Get(Form("eff_ratio_MuIdAndTrig_etaBin%d",ir+1));
		t_ratio[ir]->GetListOfFunctions()->Delete(Form("func_%d",ir));
		SetGraphStyle(t_ratio[ir],4,0);
		t_ratio[ir]->Draw("P"); //withPnt
		
    //// 2) TF1 : fit functions
		tfunc[ir] = (TF1*)f1->Get(Form("func_%d",ir+1));
		// option "p" to get the express with fixed parameters
		sz_tfunc[ir] = tfunc[ir]->GetExpFormula("p");
		cout << ir<<"th tfunc GetExpFormula : " << sz_tfunc[ir]<< endl;
		tfunc[ir]->SetRange(ptmin_newcut_neweta[ir],ptmax);
		tfunc[ir]->SetLineColor(kRed);
		tfunc[ir]->SetLineWidth(2);
		tfunc[ir]->Draw("same");
		//latex->DrawLatex(0.37, 0.25, Form("%.1f < |#eta^{#mu}| < %.1f",etabin_new[ir],etabin_new[ir+1]));
		dashedLine(0,1,ptmax,1);
//		dashedLine(ptmin_newcut_neweta[ir],sfmin,ptmin_newcut_neweta[ir],sfmax,1,1);
		solidLine(ptmin_newcut_neweta[ir],sfmin,ptmin_newcut_neweta[ir],sfmax,4,1);
	}
	c1->SaveAs(Form("TNPSF_isPA%d.pdf",isPA));
	return;
}
