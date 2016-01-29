
//void nc_totalComp()
{
	//gStyle->SetPaintTextFormat(".3f");
	gStyle->SetPaintTextFormat(".5f");
	
	//TFile* f1 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_tag5_v1.root");
	//TFile* f2 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_check5_oldacc_oldeta_private_lowerptlimit.root");
	//TFile* f2 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_CtErr1_SF0_Zvtx1.root");
	
	//TFile* f1 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_oldacccut_check5.root");
	//TFile* f2 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_CtErr1_SF0_Zvtx1_oldacccut.root");
	
	//TFile* f1 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_CtErr1_SF0_Zvtx1.root");
	//TFile* f1 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_tag5_v1_eta0912.root");
	//TFile* f2 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_CtErr1_SF0_Zvtx1_eta0912.root");
	
	//TFile* f1 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_CtErr1_SF0_Zvtx1_oldacccut.root");
	//TFile* f1 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_oldacccut_allTag5_eta0912.root");
	//TFile* f2 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_CtErr1_SF0_Zvtx1_oldacccut_eta0912.root");
	
	TFile* f1 = TFile::Open("total2Dhist_8rap3pt_noPtWeight_tag5_v1_bfracControl.root");
	//TFile* f2 = TFile::Open("total2Dhist_8rap3pt_isStiff1.root");
	TFile* f2 = TFile::Open("total2Dhist_8rap3pt_isStiff0.root");
	
	TCanvas* c1 = new TCanvas("c1","",600,600);
	//TH2D* h1 = (TH2D*)f1->Get("h2D_Acc_pt_y_PR_Pbp");
	//TH2D* h1 = (TH2D*)f1->Get("h2D_Eff_pt_y_PR_Pbp");
	//TH2D* h1 = (TH2D*)f1->Get("h2D_fit_pt_y_PR_Pbp");
	TH2D* h1 = (TH2D*)f1->Get("h2D_corrY_PR_Pbp");
	h1->SetName("h1");

	//TH2D* h2 = (TH2D*)f2->Get("h2D_Acc_pt_y_PR_Pbp");
	//TH2D* h2 = (TH2D*)f2->Get("h2D_Eff_pt_y_PR_Pbp");
	//TH2D* h2 = (TH2D*)f2->Get("h2D_fit_pt_y_PR_Pbp");
	TH2D* h2 = (TH2D*)f2->Get("h2D_corrY_PR_Pbp");
	h2->SetName("h2");
	h1->Divide(h2);
	h1->Draw("text e colz");
}
