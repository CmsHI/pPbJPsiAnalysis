
//void nc_totalComp()
{
	//gStyle->SetPaintTextFormat(".3f");
	gStyle->SetPaintTextFormat(".5f");
	
	TFile* f1 = TFile::Open("AccAna_8rap9pt_PRMC_boosted.root");
	TFile* f2 = TFile::Open("AccAna_8rap9pt_PRMC_boosted_PAS.root");
	
	TCanvas* c1 = new TCanvas("c1","",600,600);
	TH2D* h1 = (TH2D*)f1->Get("h2D_Acc_pt_y_Pbp");
	//TH2D* h1 = (TH2D*)f1->Get("h2D_Eff_pt_y_PR_Pbp");
	//TH2D* h1 = (TH2D*)f1->Get("h2D_fit_pt_y_PR_Pbp");
	//TH2D* h1 = (TH2D*)f1->Get("h2D_corrY_PR_Pbp");
	h1->SetName("h1");

	TH2D* h2 = (TH2D*)f2->Get("h2D_Acc_pt_y_Pbp");
	//TH2D* h2 = (TH2D*)f2->Get("h2D_Eff_pt_y_PR_Pbp");
	//TH2D* h2 = (TH2D*)f2->Get("h2D_fit_pt_y_PR_Pbp");
	//TH2D* h2 = (TH2D*)f2->Get("h2D_corrY_PR_Pbp");
	h2->SetName("h2");
	h1->Divide(h2);
	h1->Draw("text e colz");
}
