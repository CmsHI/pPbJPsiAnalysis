#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include <TLegend.h>

void SFOverlay()
{
	TFile* fnominal = new TFile("/home/kisoo/work/pPb_Jpsi/tagAndProbe/pp/v8/output_pp_5eta_cutG_all_nominal_v8.root", "READ");
	TFile* fmax = new TFile("/home/kisoo/work/pPb_Jpsi/tagAndProbe/pp/SysStudy/pp_Merged_error_ratio_max_v8.root", "READ");
	TFile* fmin = new TFile("/home/kisoo/work/pPb_Jpsi/tagAndProbe/pp/SysStudy/pp_Merged_error_ratio_min_v8.root", "READ");
	TFile* fstat[100];

	TCanvas* c1 = new TCanvas("c1", "", 0, 0, 1500, 400);
	c1->Divide(5,0, 0.0, 0.0);
	TH1F* h1[5];
	Double_t limit[5] = {3.3, 3.3, 2.16, 1.6, 1.3};

	gStyle->SetOptStat(0000);
	for(Int_t i = 0; i < 5; i++)
	{
		c1->cd(i+1);
		h1[i] = new TH1F(Form("h1_%d", i), "", 1, 0, 20);
		h1[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h1[i]->GetYaxis()->SetTitle("SF");
		h1[i]->SetAxisRange(0.6,2.0,"Y");
		h1[i]->Draw();
		TLegend* leg = new TLegend(0.58, 0.7, 0.9, 0.9);
		leg->SetFillStyle(0);
		leg->SetFillColor(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.04);
		for(Int_t j = 0; j < 100; j++)
		{
			fstat[i] = new TFile(Form("/home/kisoo/work/pPb_Jpsi/tagAndProbe/pp/SysStudy/TnPToyMC5etaStat/tnpRate_pp_eff_fit_Calo_ToyMC_v8_%d.root", j+1), "READ");
			TF1* funcstat = (TF1*) fstat[i]->Get(Form("func_%d", i));
			funcstat->SetLineColor(1);
			funcstat->SetRange(limit[i], 20);
			funcstat->Draw("same");
			if(j == 0) leg->AddEntry(funcstat, "stat (100 toys)", "l");
		}
		TF1* funcnominal = (TF1*) fnominal->Get(Form("func_%d", i+1));
		funcnominal->SetLineColor(2);
		funcnominal->SetRange(limit[i], 20);
		funcnominal->Draw("same");
		leg->AddEntry(funcnominal, "Nominal", "l");
		TF1* funcmax = (TF1*) fmax->Get(Form("func_%d", i+1));
		funcmax->SetLineColor(7);
		funcmax->SetRange(limit[i], 20);
		funcmax->Draw("same");
		leg->AddEntry(funcmax, "syst (+/- 1#sigma)", "l");
		TF1* funcmin = (TF1*) fmin->Get(Form("func_%d", i+1));
		funcmin->SetLineColor(7);
		funcmin->SetRange(limit[i], 20);
		funcmin->Draw("same");
		leg->Draw("same");
	}
	c1->SaveAs("SF_overlay_pp_v8.pdf");
}
