//void nc_totalComp()
{
	gStyle->SetOptStat(0);
	gStyle->SetPaintTextFormat(".3f");
	//gStyle->SetPaintTextFormat(".5f");
	
	//TFile* f1 = TFile::Open("AccAna_8rap9pt_PRMC_boosted.root");
	//TFile* f2 = TFile::Open("AccAna_8rap9pt_PRMC_boosted_PAS.root");
	TFile* f1 = TFile::Open("AccAna_8rap9pt_pp_PR.root");
	TFile* f2 = TFile::Open("rootNonOnia/AccAna_8rap9pt_PRMC_boosted.root");
	
	TCanvas* c1 = new TCanvas("c1","",600,600);
	//TH2D* h1 = (TH2D*)f1->Get("h2D_Den_Jpsi_pt_y");
	TH2D* h1 = (TH2D*)f1->Get("h2D_Acc_pt_y");
	h1->SetName("h1");

	//TH2D* h2 = (TH2D*)f1->Get("h2D_Den_pt_y");
	TH2D* h2 = (TH2D*)f2->Get("h2D_Acc_pt_y_Pbp");
	h2->SetName("h2");
	h1->Divide(h2);
	h1->SetMaximum(1.3);	
	h1->SetMaximum(1.7);	
	h1->Draw("text e colz");
}
