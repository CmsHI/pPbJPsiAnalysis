//#include "SONGKYO.h"

//void nc_totalComp()
{
	gStyle->SetOptStat(0);
	gStyle->SetPaintTextFormat(".3f");
	//gStyle->SetPaintTextFormat(".5f");
  gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadRightMargin(0.04); //KYO
  //gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadLeftMargin(0.17);
  gStyle->SetTitleXOffset(1.15);
  gStyle->SetTitleYOffset(1.2);

	double lumi_PA =34.622*1000;
	//double lumi_PA =20.7*1000;
	//double lumi_PA =14.0*1000;
	double lumi_PP =26.3*1000*1000;
	double A_pb =208;

  double ymin=0.5;
  double ymax=1.5;
		
	TFile* f1 = TFile::Open("totalHist_pA_8rap9pt_newcut_Zvtx0_SF0_noPtWeight.root");
	TFile* f2 = TFile::Open("totalHist_pp_8rap9pt_newcut_Zvtx0_SF0_noPtWeight.root");
	
	TH2D* h2D_PA = (TH2D*)f1->Get("h2D_CorrY_PR_pA");
	h2D_PA->SetName("h2D_PA");
	TH2D* h2D_PP = (TH2D*)f2->Get("h2D_CorrY_PR_pp");
	h2D_PP->SetName("h2D_PP");
	
	// ---  projection to 1D hist
	const int nRap = 8;
	const int nPt = 9;
	TH1D* h1D_CorrY_PR_PA[nRap]; 
	TH1D* h1D_CorrY_PR_PP[nRap]; 
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nRap; iy++) {
		//from neg y_lab  (Pbp)
		h1D_CorrY_PR_PA[iy] = h2D_PA->ProjectionY(Form("h1D_CorrY_PR_pA_%d",iy),iy+1,iy+1);
		h1D_CorrY_PR_PP[iy] = h2D_PP->ProjectionY(Form("h1D_CorrY_PR_PP_%d",iy),nRap-iy,nRap-iy);
	}

	//// unused bin	
	for (Int_t iy = 0; iy < nRap; iy++) {
		for (Int_t ipt =0; ipt < nPt; ipt++) {
			if (iy==0) { //4 GeV
				h1D_CorrY_PR_PA[iy]->SetBinContent(1,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinContent(1,0);
				h1D_CorrY_PR_PA[iy]->SetBinContent(2,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinContent(2,0);
				h1D_CorrY_PR_PA[iy]->SetBinError(1,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinError(1,0);
				h1D_CorrY_PR_PA[iy]->SetBinError(2,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinError(2,0);
			}
			else if (iy>=1 && iy<=4) { // 6.5 GeV
				h1D_CorrY_PR_PA[iy]->SetBinContent(1,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinContent(1,0);
				h1D_CorrY_PR_PA[iy]->SetBinContent(2,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinContent(2,0);
				h1D_CorrY_PR_PA[iy]->SetBinContent(3,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinContent(3,0);
				h1D_CorrY_PR_PA[iy]->SetBinContent(4,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinContent(4,0);
				h1D_CorrY_PR_PA[iy]->SetBinError(1,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinError(1,0);
				h1D_CorrY_PR_PA[iy]->SetBinError(2,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinError(2,0);
				h1D_CorrY_PR_PA[iy]->SetBinError(3,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinError(3,0);
				h1D_CorrY_PR_PA[iy]->SetBinError(4,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinError(4,0);
			}
			else if (iy==5) { // 5 GeV
				h1D_CorrY_PR_PA[iy]->SetBinContent(1,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinContent(1,0);
				h1D_CorrY_PR_PA[iy]->SetBinContent(2,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinContent(2,0);
				h1D_CorrY_PR_PA[iy]->SetBinContent(3,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinContent(3,0);
				h1D_CorrY_PR_PA[iy]->SetBinError(1,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinError(1,0);
				h1D_CorrY_PR_PA[iy]->SetBinError(2,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinError(2,0);
				h1D_CorrY_PR_PA[iy]->SetBinError(3,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinError(3,0);
			}
			else if (iy==6) { // 4 GeV
				h1D_CorrY_PR_PA[iy]->SetBinContent(1,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinContent(1,0);
				h1D_CorrY_PR_PA[iy]->SetBinContent(2,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinContent(2,0);
				h1D_CorrY_PR_PA[iy]->SetBinError(1,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinError(1,0);
				h1D_CorrY_PR_PA[iy]->SetBinError(2,0);
				h1D_CorrY_PR_PP[iy+1]->SetBinError(2,0);
			}
		///	cout << iy <<"th, "<<ipt<<"th h1D_CorrY_PR_PA : " << h1D_CorrY_PR_PP[iy]->GetBinContent(ipt+1) << endl;
		}
	}
	
	TCanvas* c1 = new TCanvas("c1","",1600,700);
	gPad->SetLogy(1);
	c1->Divide(4,2);

	for (Int_t iy = 0; iy < nRap-1; iy++) {
		if (iy<3) c1->cd(iy+1);
		else c1->cd(iy+2);
		h1D_CorrY_PR_PP[iy+1]->SetMarkerColor(kBlack);
		h1D_CorrY_PR_PP[iy+1]->SetLineColor(kBlack);
		h1D_CorrY_PR_PP[iy+1]->SetMarkerStyle(kFullCircle);
		h1D_CorrY_PR_PP[iy+1]->Scale(1./lumi_PP);
		h1D_CorrY_PR_PP[iy+1]->Scale(A_pb);
		h1D_CorrY_PR_PP[iy+1]->Draw("pe");
		h1D_CorrY_PR_PA[iy]->SetMarkerColor(kBlue);
		h1D_CorrY_PR_PA[iy]->SetLineColor(kBlue);
		h1D_CorrY_PR_PA[iy]->SetMarkerStyle(kFullCircle);
		h1D_CorrY_PR_PA[iy]->Scale(1./lumi_PA);
		h1D_CorrY_PR_PA[iy]->Draw("pe same");
	}

	TCanvas* c2 = new TCanvas("c2","",1600,700);
	gPad->SetLogy(0);
	c2->Divide(4,2);
	
	TH1D* RpPb[nRap]; 
  TLine* t1 = new TLine(0.,1.,30.,1.);
  t1->SetLineStyle(7);
	for (Int_t iy = 0; iy < nRap-1; iy++) {
		if (iy<3) c2->cd(iy+1);
		else c2->cd(iy+2);
		RpPb[iy] = (TH1D*)h1D_CorrY_PR_PA[iy]->Clone(Form("RpPb_%d",iy));
		RpPb[iy]->GetXaxis()->SetRangeUser(0,30);
		RpPb[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		RpPb[iy]->GetYaxis()->SetTitle("R_{pPb}");
		RpPb[iy]->GetXaxis()->SetTitleSize(0.05);
		RpPb[iy]->GetYaxis()->SetTitleSize(0.05);
		RpPb[iy]->GetXaxis()->CenterTitle(1);
		RpPb[iy]->GetYaxis()->CenterTitle(1);
		RpPb[iy]->GetYaxis()->SetRangeUser(ymin,ymax);
		RpPb[iy]->Divide(h1D_CorrY_PR_PP[iy+1]);
		RpPb[iy]->Draw("pe");
  	t1->Draw();
	}
	//c2->SaveAs("dir_RpPb/RpPb_PR_PA_dropLowPtBin.pdf");

}
