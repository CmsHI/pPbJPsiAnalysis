
//void nc_totalComp()
{
	gStyle->SetPaintTextFormat(".5f");

	//TFile *f1 = TFile::Open("AccAna_8rap9pt_pp_PR_newcut_ptWeight.root");
	//TFile *f2 = TFile::Open("AccAna_8rap9pt_pp_PR_newcut_ptWeight_0722.root");
	
  TFile *f1 = TFile::Open("EffAna_8rap9pt_pp_PR_newcut_Zvtx0_SF1_ptWeight.root");
  TFile *f2 = TFile::Open("EffAna_8rap9pt_pp_PR_newcut_Zvtx0_SF1_ptWeight_0722.root");

	TCanvas* c1 = new TCanvas("c1","",600,600);

	//TH2D* h1 = (TH2D*)f1->Get("h2D_Acc_pt_y");
	//TH2D* h1 = (TH2D*)f1->Get("h2D_Acc_pt_y_Pbp");
	TH2D* h1 = (TH2D*)f1->Get("h2D_Eff_pt_y");
	h1->SetName("h1");

	//TH2D* h2 = (TH2D*)f2->Get("h2D_Acc_pt_y");
	TH2D* h2 = (TH2D*)f2->Get("h2D_Eff_pt_y");
	//TH2D* h2 = (TH2D*)f1->Get("h2D_Acc_pt_y_noPtWeight");
	//TH2D* h2 = (TH2D*)f1->Get("h2D_Acc_pt_y_noPtWeight_Pbp");
	//TH2D* h2 = (TH2D*)f1->Get("h2D_Eff_pt_y_noPtWeight");
	h2->SetName("h2");
	h1->Divide(h2);
	h1->Draw("text e colz");
}
