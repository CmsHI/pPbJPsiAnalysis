{
	gStyle->SetPaintTextFormat(".3f");
	//gStyle->SetPaintTextFormat(".5f");
	gStyle->SetOptStat(0);
	
	//TFile* f1 = TFile::Open("zVtx201603/zVtx_G1_pp_isPrompt1_isNewAccCut1_isOfficial0.root"); //Data
	//TFile* f2 = TFile::Open("EffAna_8rap9pt_pp_PR_newcut_Zvtx1_SF0.root"); // MC
	TFile* f1 = TFile::Open("zVtx201603/zVtx_G1_Pbp_isPrompt1_isNewAccCut1_isOfficial1.root"); //Data
	TFile* f2 = TFile::Open("EffAna_8rap9pt_Pbp_PR_newcut_Zvtx1_SF0.root"); // MC
	
	TCanvas* c1 = new TCanvas("c1","",600,600);
	TH1D* h1 = (TH1D*)f1->Get("h1D_01");
	h1->SetName("h1");
	//h1->Draw("pe");
	TH1D* h2 = (TH1D*)f2->Get("h1D_zVtx"); //weighted
	h2->SetName("h2");
	h2->Scale(1,"width");
	h2->Scale(1./h2->Integral());
	h2->SetMarkerColor(kBlue);
	h2->SetMarkerStyle(kFullCircle);
	h2->SetMarkerSize(1.2);
	h2->GetXaxis()->SetTitle("Z_{vtx} [cm]");
	//h2->GetXaxis()->SetRangeUser(-10, 10);
	h2->GetXaxis()->SetRangeUser(-30, 30);
	//h2->GetYaxis()->SetRangeUser(0, 0.12);
	h2->GetYaxis()->SetRangeUser(0, 0.09);
	h2->Draw("pe");
	h1->Draw("pe same");

	TLegend *legUR = new TLegend(0.58, 0.82, 0.89, 0.94);
	legUR->AddEntry(h1,"Data","lp");
	legUR->AddEntry(h2,"MC weighted","lp");
	legUR->Draw();

	TCanvas* c2 = new TCanvas("c2","",600,600);
	c2->cd();
	TH1D* hRatio = (TH1D*)h1->Clone("hRatio");
	hRatio->Divide(h2);
	hRatio->SetMinimum(0.0);
	hRatio->SetMaximum(2.0);
	hRatio->Draw("pe");
}
