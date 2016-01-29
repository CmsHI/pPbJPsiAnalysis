
//void nc_totalComp()
{
	gStyle->SetPaintTextFormat(".3f");

	//TFile *f1 = TFile::Open("AccAna_8rap3pt_PRMC_boosted_ptWeight_isStiff1.root");	
	//TFile *f2 = TFile::Open("AccAna_8rap3pt_PRMC_boosted_ptWeight_isStiff0.root");	
	TFile *f1 = TFile::Open("EffAna_8rap3pt_PRMCpythia_pPb_CtErr1_SF1_Zvtx1_ptWeight_isStiff1.root");

	TCanvas* c1 = new TCanvas("c1","",600,600);

	//TH2D* h1 = (TH2D*)f1->Get("h2D_Acc_noPtWeight_pt_y_Pbp");
	//TH2D* h1 = (TH2D*)f1->Get("h2D_Acc_pt_y_Pbp");
	TH2D* h1 = (TH2D*)f1->Get("h2D_Eff_pt_y");
	h1->SetName("h1");

	//TH2D* h2 = (TH2D*)f2->Get("h2D_Acc_noPtWeight_pt_y_Pbp");
	//TH2D* h2 = (TH2D*)f2->Get("h2D_Acc_pt_y_Pbp");
	TH2D* h2 = (TH2D*)f1->Get("h2D_Eff_noPtWeight_pt_y");
	h2->SetName("h2");
	h1->Divide(h2);
	h1->Draw("text e colz");
}
