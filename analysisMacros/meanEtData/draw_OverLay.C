#include "../SONGKYO.h"

void draw_OverLay(bool isScale=1)
{
	gROOT->Macro("../Style.C");

	TFile * fMB = new TFile("ethf_MB_6rap3pt.root");
	TFile * fDimu = new TFile("ethf_data_6rap3pt.root");
	TH1D* h1D_et_min = (TH1D*)fMB->Get("hEt_0");
	TH1D* h1D_et_dimu = (TH1D*)fDimu->Get("hEt_0");

	//// check
	cout <<"h1D_et_min =" <<h1D_et_min<<endl;
	cout <<"h1D_et_dimu =" <<h1D_et_dimu<<endl;

	//// latex box 
	//TLatex* latex = new TLatex();
	//latex->SetNDC();
	//latex->SetTextAlign(12);
	//latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	//// Draw histograms
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
	TLegend *legBM = new TLegend(0.20, 0.19, 0.45, 0.35);
	SetLegendStyle(legBM);

	//////////////////////////////////////////////////////////////////
  c1->cd();
	SetHistStyle(h1D_et_min,3,0);	 // orange
	SetHistStyle(h1D_et_dimu,5,0); // azure
	gPad->SetLogy(1);
	h1D_et_min->GetXaxis()->SetTitle("E_{T}^{HF |#eta|>4} (GeV)");
	h1D_et_min->GetYaxis()->SetTitle("Events/(GeV)");
	//	h1D_et_min->GetYaxis()->SetTitleOffset(1.8);
	//	h1D_et_min->SetMaximum(22000.);
	h1D_et_min->GetXaxis()->CenterTitle();
	//h1D_et_min->GetXaxis()->SetTitleOffset(1.3);
		
	h1D_et_min->Scale(1,"width");
	h1D_et_dimu->Scale(1,"width");
	if (isScale){
	  h1D_et_min->Scale(1/h1D_et_min->Integral());
	  h1D_et_dimu->Scale(1/h1D_et_dimu->Integral());
	}
	h1D_et_min->SetLineWidth(3);
	h1D_et_dimu->SetLineWidth(3);
	h1D_et_min->Draw("hist");
	h1D_et_dimu->Draw("hist same");
	legBM -> AddEntry(h1D_et_min,"Minimum bias sample","l");
	legBM -> AddEntry(h1D_et_dimu,"Muon triggered sample","l");
	legBM -> Draw();
	c1->SaveAs(Form("et_dimu_min_isScale%d.pdf",(int)isScale));
	c1->Clear();
	legBM -> Clear();

	return;

}

