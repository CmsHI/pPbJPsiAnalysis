#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TMatrixD.h>
#include <TDecompLU.h>
#include <TDecompSVD.h>
#include <iostream>
#include <TLegend.h>
#include <TLine.h>
#include <TString.h>

void formPtArr(Double_t binmin, Double_t binmax, string* arr);

void Decomposition()
{
	gStyle->SetOptStat(0000);
	gStyle->SetPaintTextFormat(".2f");

	bool isPrompt = false;
	bool is1st = false;

	const char *samplename;

	if(isPrompt == 1 && is1st == 1)
	{
		samplename = "PRMCboosted_Pbp";
	}
	else if(isPrompt == 1 && is1st != 1)
	{
		samplename = "PRMCboosted_pPb";
	}
	else if(isPrompt != 1 && is1st == 1)
	{
		samplename = "NPMCboosted_Pbp";
	}
	else if(isPrompt != 1 && is1st != 1)
	{
		samplename = "NPMCboosted_pPb";
	}

	const char *strBinning = "8rap18ptMatchdR";
	TString datastring = Form("%s_%s", strBinning, samplename);

	TFile *fin = new TFile(Form("recoPt_genPt_%s_test.root", datastring.Data()), "READ");
	TFile *fout = new TFile(Form("Deconvolution_%s.root", datastring.Data()), "RECREATE");

	//
	// define binning
	//
	Double_t ptBinsArrfine[] = {0.0,1.5,3.0,3.5,4.0,4.5,5.0,5.8,6.5,7.0,7.5,8.0,8.5,9.0,10.0,12.5,14.0,22.0,30.0,35.0};
	Double_t ptBinsArrcoarse[] = {0.0,3.0,4.0,5.0,6.5,7.5,8.5,10.0,14.0,30.0,35.0};

	Double_t etaBinsArrPbp[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4};//1st run
	Double_t etaBinsArrpPb[] = {-2.4, -1.93, -1.46, -1.03, -0.43, 0.47, 1.37, 1.97, 2.4};//2nd run

	const Int_t nPtBinsfine = sizeof(ptBinsArrfine)/sizeof(double)-1;
	const Int_t nPtBinscoarse = sizeof(ptBinsArrcoarse)/sizeof(double)-1;
	const Int_t totnBins = nPtBinsfine*nPtBinsfine;
	const Int_t nEtaBins = sizeof(etaBinsArrPbp)/sizeof(double)-1;

	string ptArr[nPtBinsfine];
	string ptArrcoarse[nPtBinsfine];
	Double_t pt_val_gen[nEtaBins][nPtBinsfine];
	Double_t pt_val_reco[nEtaBins][nPtBinsfine];
	Double_t pt_val_gen_coarse[nEtaBins][nPtBinscoarse];
	Double_t pt_val_inverse_reco[nEtaBins][nPtBinsfine];
	Double_t pt_val_inverse_reco_coarse[nEtaBins][nPtBinscoarse];
	Double_t BinWidth[nEtaBins][nPtBinscoarse];

	string etaArr[nEtaBins];

	for(Int_t i = 0; i < nPtBinsfine; i++)
	{
		formPtArr(ptBinsArrfine[i], ptBinsArrfine[i+1], &ptArr[i]);
		formPtArr(ptBinsArrcoarse[i], ptBinsArrcoarse[i+1], &ptArrcoarse[i]);
	}

	for(Int_t i = 0; i < nEtaBins; i++)
	{
		if(is1st == 1)
		{
			formPtArr(etaBinsArrPbp[i], etaBinsArrPbp[i+1], &etaArr[i]);
		}
		else
		{
			formPtArr(etaBinsArrpPb[i], etaBinsArrpPb[i+1], &etaArr[i]);
		}
	}

	//
	// define canvas and histogram
	//
	TCanvas *c_hist[nEtaBins];
	TCanvas *c_Matrix[nEtaBins];
	TCanvas *c_Invert[nEtaBins];
	TCanvas *c_Decon[nEtaBins];
	TCanvas *c_GEN_num[nEtaBins];
	TCanvas *c_RECO_num[nEtaBins];
	TCanvas *c_GEN_num_Decon[nEtaBins];
	TCanvas *c_GEN_comp[nEtaBins];
	TCanvas *c_GEN_ratio[nEtaBins];

	TH2D *h_Reco_Gen_Matrix[nEtaBins];
	TH2D *h_Decon[nEtaBins];
	TH1D *h_GEN_num[nEtaBins];
	TH1D *h_RECO_num[nEtaBins];
	TH1D *h_GEN_num_Decon[nEtaBins];
	TH1D *h1_gen[nEtaBins];
	TH1D *h1_reco[nEtaBins];
	TH1D *h1_inverse_reco[nEtaBins];

	TLegend *leg_Comp[nEtaBins];
	TLegend *leg_hist[nEtaBins];
	TLegend *leg_ratio[nEtaBins];
	TLine* t1[nEtaBins];

	for(Int_t i = 0; i < nEtaBins; i ++)
	{
		c_hist[i] = new TCanvas(Form("c_hist_%d", i), "", 0, 10, 600, 600);
		c_Matrix[i] = new TCanvas(Form("c_Matrix_%d", i), "", 0, 10, 1200, 1200);
		c_Invert[i] = new TCanvas(Form("c_Invert_%d", i), "", 0, 10, 1200, 1200);
		c_Decon[i] = new TCanvas(Form("c_Decon_%d", i), "", 0, 20, 1200, 1200);
		c_GEN_num[i] = new TCanvas(Form("c_GEN_num_%d", i), "", 0, 30, 1200, 1200);
		c_RECO_num[i] = new TCanvas(Form("c_RECO_num_%d", i), "", 0, 40, 1200, 1200);
		c_GEN_num_Decon[i] = new TCanvas(Form("c_GEN_num_Decon_%d", i), "", 0, 50, 1200, 1200);
		c_GEN_comp[i] = new TCanvas(Form("c_GEN_comp_%d", i), "", 0, 50, 600, 600);
		c_GEN_ratio[i] = new TCanvas(Form("c_GEN_ratio_%d", i), "", 0, 50, 600, 600);

		h_Reco_Gen_Matrix[i] = new TH2D(Form("h_Reco_Gen_Matrix_%d", i), "", nPtBinsfine, ptBinsArrfine, nPtBinsfine, ptBinsArrfine);
		h_Decon[i] = new TH2D(Form("h_Decon_%d", i), "", nPtBinsfine, ptBinsArrfine, nPtBinsfine, ptBinsArrfine);
		h_GEN_num_Decon[i] = new TH1D(Form("h_GEN_num_Decon_%d", i), "", nPtBinsfine, ptBinsArrfine);
		h1_gen[i] = new TH1D(Form("h1_gen_%d", i), "", nPtBinscoarse, 0, nPtBinscoarse);
		h1_reco[i] = new TH1D(Form("h1_reco_%d", i), "", nPtBinscoarse, 0, nPtBinscoarse);
		h1_inverse_reco[i] = new TH1D(Form("h1_inverse_reco_%d", i), "", nPtBinscoarse, 0, nPtBinscoarse);

		h_Reco_Gen_Matrix[i]->Sumw2();
		h_Decon[i]->Sumw2();
		h_GEN_num_Decon[i]->Sumw2();
		h1_gen[i]->Sumw2();
		h1_reco[i]->Sumw2();
		h1_inverse_reco[i]->Sumw2();

		leg_Comp[i] = new TLegend(0.7, 0.7, 0.8, 0.8);
		leg_Comp[i]->SetFillColor(0);
		leg_Comp[i]->SetBorderSize(0);
		leg_Comp[i]->SetTextSize(0.045);
		leg_Comp[i]->SetTextAlign(12);

		leg_hist[i] = new TLegend(0.7, 0.7, 0.8, 0.8);
		leg_hist[i]->SetFillColor(0);
		leg_hist[i]->SetBorderSize(0);
		leg_hist[i]->SetTextSize(0.045);
		leg_hist[i]->SetTextAlign(12);

		leg_ratio[i] = new TLegend(0.2, 0.2, 0.3, 0.3);
		leg_ratio[i]->SetFillColor(0);
		leg_ratio[i]->SetBorderSize(0);
		leg_ratio[i]->SetTextSize(0.045);
		leg_ratio[i]->SetTextAlign(12);

		t1[i] = new TLine(0, 1, 10, 1);
		t1[i]->SetLineWidth(3);
		t1[i]->SetLineStyle(1);
		t1[i]->SetLineColor(1);

		TMatrixD M_Matrix(nPtBinsfine, nPtBinsfine);
		TMatrixD M_Gen(nPtBinsfine,1);
		TMatrixD M_Reco(nPtBinsfine,1);
		TMatrixD M_Reco_Decon(nPtBinsfine,1);

		h_Reco_Gen_Matrix[i] = (TH2D*) fin->Get(Form("h2D_fixedMatrix_%s_%d", strBinning, i));
		h_GEN_num[i] = (TH1D*) fin->Get(Form("h1D_Gen_Matrix_%s_%d", strBinning, i));
		h_RECO_num[i] = (TH1D*) fin->Get(Form("h1D_Reco_Matrix_%s_%d", strBinning, i));
		h_GEN_num[i]->Sumw2();
		h_RECO_num[i]->Sumw2();

		TArrayD data_Reco(nPtBinsfine);
		TArrayD data_Gen(nPtBinsfine);
		TArrayD data_Matrix(totnBins);

		if(isPrompt == 1 && is1st == 1)
		{
			for(Int_t j = 0; j < nPtBinsfine; j++)
			{
				M_Reco(j,0) = h_RECO_num[i]->GetBinContent(j+1);
				M_Gen(j,0) = h_GEN_num[i]->GetBinContent(j+1);
				for(Int_t k = 0; k < nPtBinsfine; k++)
				{
					M_Matrix(k, j) = h_Reco_Gen_Matrix[i]->GetBinContent(j+1, k+1);
					if(i == 2)
					{
						if((k < 3 || j < 3) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 3 || j < 3) && j != k) M_Matrix(k, j) = 0;
					}
					else if(i == 3)
					{
						if((k < 7 || j < 7) && j == k) M_Matrix(k, j) = 1;
						else if((k < 7 || j < 7) && j != k) M_Matrix(k, j) = 0;
					}
					else if(i == 4)
					{
						if((k < 6 || j < 6) && j == k) M_Matrix(k, j) = 1;
						else if((k < 6 || j < 6) && j != k) M_Matrix(k, j) = 0;
					}
					else if(i == 5)
					{
						if((k < 1 || j < 1) && j == k) M_Matrix(k, j) = 1;
						else if((k < 1 || j < 1) && j != k) M_Matrix(k, j) = 0;
					}
				}
			}
		}
		else if(isPrompt == 1 && is1st != 1)
		{
			for(Int_t j = 0; j < nPtBinsfine; j++)
			{
				M_Reco(j,0) = h_RECO_num[i]->GetBinContent(j+1);
				M_Gen(j,0) = h_GEN_num[i]->GetBinContent(j+1);

				for(Int_t k = 0; k < nPtBinsfine; k++)
				{
					M_Matrix(k, j) = h_Reco_Gen_Matrix[i]->GetBinContent(j+1, k+1);
					if(i == 1)
					{
						if((k < 1 || j < 1) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 1 || j < 1) && j != k) M_Matrix(k, j) = 0;
					}
					if(i == 2)
					{
						if((k < 1 || j < 1) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 1 || j < 1) && j != k) M_Matrix(k, j) = 0;
					}
					if(i == 3)
					{
						if((k < 5 || j < 5) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 5 || j < 5) && j != k) M_Matrix(k, j) = 0;
					}
					if(i == 4)
					{
						if((k < 7 || j < 7) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 7 || j < 7) && j != k) M_Matrix(k, j) = 0;
					}
					if(i == 5)
					{
						if((k < 4 || j < 4) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 4 || j < 4) && j != k) M_Matrix(k, j) = 0;
					}
				}
			}
		}
		else if(isPrompt != 1 && is1st == 1)
		{
			for(Int_t j = 0; j < nPtBinsfine; j++)
			{
				M_Reco(j,0) = h_RECO_num[i]->GetBinContent(j+1);
				M_Gen(j,0) = h_GEN_num[i]->GetBinContent(j+1);

				for(Int_t k = 0; k < nPtBinsfine; k++)
				{
					M_Matrix(k, j) = h_Reco_Gen_Matrix[i]->GetBinContent(j+1, k+1);
					if(i == 1)
					{
						if((k < 1 || j < 1) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 1 || j < 1) && j != k) M_Matrix(k, j) = 0;
					}
					if(i == 2)
					{
						if((k < 4 || j < 4) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 4 || j < 4) && j != k) M_Matrix(k, j) = 0;
					}
					if(i == 3)
					{
						if((k < 7 || j < 7) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 7 || j < 7) && j != k) M_Matrix(k, j) = 0;
					}
					if(i == 4)
					{
						if((k < 6 || j < 6) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 6 || j < 6) && j != k) M_Matrix(k, j) = 0;
					}
					if(i == 5)
					{
						if((k < 3 || j < 3) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 3 || j < 3) && j != k) M_Matrix(k, j) = 0;
					}
				}
			}
		}
		else
		{
			for(Int_t j = 0; j < nPtBinsfine; j++)
			{
				M_Reco(j,0) = h_RECO_num[i]->GetBinContent(j+1);
				M_Gen(j,0) = h_GEN_num[i]->GetBinContent(j+1);

				for(Int_t k = 0; k < nPtBinsfine; k++)
				{
					M_Matrix(k, j) = h_Reco_Gen_Matrix[i]->GetBinContent(j+1, k+1);
					if(i == 2)
					{
						if((k < 3 || j < 3) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 3 || j < 3) && j != k) M_Matrix(k, j) = 0;
					}
					if(i == 3)
					{
						if((k < 5 || j < 5) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 5 || j < 5) && j != k) M_Matrix(k, j) = 0;
					}
					if(i == 4)
					{
						if((k < 7 || j < 7) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 7 || j < 7) && j != k) M_Matrix(k, j) = 0;
					}
					if(i == 5)
					{
						if((k < 3 || j < 4) && j == k) M_Matrix(k, j) = 1;
						else if( (k < 3 || j < 4) && j != k) M_Matrix(k, j) = 0;
					}
				}
			}
		}

		// comp hist
		c_hist[i]->cd();
		h_GEN_num[i]->SetTitle(Form("bin_%d", i));
		h_GEN_num[i]->GetXaxis()->SetTitle("p_{T} (Gev/c)");
		h_GEN_num[i]->GetYaxis()->SetTitle("Counts");
		h_GEN_num[i]->SetMarkerStyle(21);
		h_GEN_num[i]->SetMarkerColor(2);
		leg_hist[i]->AddEntry(h_GEN_num[i], "GEN");
		h_GEN_num[i]->Draw();
		h_RECO_num[i]->SetMarkerStyle(29);
		h_RECO_num[i]->SetMarkerColor(3);
		leg_hist[i]->AddEntry(h_RECO_num[i], "RECO");
		h_RECO_num[i]->Draw("same");
		leg_hist[i]->Draw();
		c_hist[i]->SaveAs(Form("Hist_GEN_RECO_%s_%d_test.pdf", datastring.Data(), i));
		c_hist[i]->SaveAs(Form("Hist_GEN_RECO_%s_%d_test.png", datastring.Data(), i));

		// draw the matrix
		c_Matrix[i]->cd();
		M_Matrix.Draw("text colz");
		c_Matrix[i]->SaveAs(Form("Matrix_MuID_%s_%d_test.pdf", datastring.Data(), i));
		c_Matrix[i]->SaveAs(Form("Matrix_MuID_%s_%d_test.png", datastring.Data(), i));
		// invert matrix and draw
		M_Matrix.Invert();

		c_Invert[i]->cd();
		M_Matrix.Draw("text colz");
		c_Invert[i]->SaveAs(Form("Invert_MuID_%s_%d_test.pdf", datastring.Data(), i));
		c_Invert[i]->SaveAs(Form("Invert_MuID_%s_%d_test.png", datastring.Data(), i));

		// calculate GEN from RECO and compare it with original GEN
		M_Reco_Decon.Mult(M_Matrix, M_Reco);

		c_GEN_num_Decon[i]->Divide(2,1);
		c_GEN_num_Decon[i]->cd(1);
		M_Gen.Draw("text");
		c_GEN_num_Decon[i]->cd(2);
		M_Reco_Decon.Draw("text");
//		c_GEN_num_Decon[i]->SaveAs(Form("Deconvolution_comp_MuID_%s_%d_test.pdf", datastring.Data(), i));
//		c_GEN_num_Decon[i]->SaveAs(Form("Deconvolution_comp_MuID_%s_%d_test.png", datastring.Data(), i));

		// draw the previous comparison in the histogram form
		for(Int_t j = 0; j < nPtBinscoarse; j++)
		{
			if(j != nPtBinscoarse-1)
			{
				pt_val_gen[i][j] = M_Gen(2*j,0)+M_Gen(2*j+1, 0);
				pt_val_reco[i][j] = M_Reco(2*j,0)+M_Reco(2*j+1, 0);
				pt_val_inverse_reco[i][j] = M_Reco_Decon(2*j,0)+M_Reco_Decon(2*j+1,0);
			}
			else
			{
				pt_val_gen[i][j] = M_Gen(2*j,0);
				pt_val_reco[i][j] = M_Reco(2*j,0);
				pt_val_inverse_reco[i][j] = M_Reco_Decon(2*j,0);
			}
			BinWidth[i][j] = ptBinsArrcoarse[j+1]-ptBinsArrcoarse[j];

			h1_gen[i]->SetBinContent(j+1, pt_val_gen[i][j]/BinWidth[i][j]);
			h1_reco[i]->SetBinContent(j+1, pt_val_reco[i][j]/BinWidth[i][j]);
			h1_inverse_reco[i]->SetBinContent(j+1, pt_val_inverse_reco[i][j]/BinWidth[i][j]);
			h1_gen[i]->GetXaxis()->SetBinLabel(j+1, ptArrcoarse[j].c_str());
			h1_inverse_reco[i]->GetXaxis()->SetBinLabel(j+1, ptArrcoarse[j].c_str());
		}

		c_GEN_comp[i]->cd();
		h1_gen[i]->SetMarkerStyle(21);
		h1_gen[i]->SetMarkerColor(2);
		h1_gen[i]->SetMarkerSize(3);
		h1_gen[i]->GetXaxis()->SetTitle("p_{T} (GeV)");
		h1_gen[i]->GetYaxis()->SetTitle("dN/dp_{T}");
		leg_Comp[i]->AddEntry(h1_gen[i], "GEN", "ep");
		h1_gen[i]->Draw("e");
		h1_inverse_reco[i]->SetMarkerStyle(29);
		h1_inverse_reco[i]->SetMarkerColor(3);
		h1_inverse_reco[i]->SetMarkerSize(3);
		leg_Comp[i]->AddEntry(h1_inverse_reco[i], "RECO", "ep");
		h1_inverse_reco[i]->Draw("same e");
		leg_Comp[i]->Draw();
//		c_GEN_comp[i]->SaveAs(Form("Inverse_Comp_MuID_%s_%d_test.pdf", datastring.Data(), i));
//		c_GEN_comp[i]->SaveAs(Form("Inverse_Comp_MuID_%s_%d_test.png", datastring.Data(), i));

		// draw the ratio of RECO/GEN, GEN_matrix/GEN
		c_GEN_ratio[i]->cd();
		h1_inverse_reco[i]->Divide(h1_gen[i]);
		h1_inverse_reco[i]->SetMarkerStyle(21);
		h1_inverse_reco[i]->SetMarkerColor(2);
		h1_inverse_reco[i]->SetMarkerSize(2);
		h1_inverse_reco[i]->GetXaxis()->SetTitle("p_{T} (GeV)");
		h1_inverse_reco[i]->GetYaxis()->SetTitle("ratio");
		h1_inverse_reco[i]->SetMinimum(0.0);
		leg_ratio[i]->AddEntry(h1_inverse_reco[i], "GEN_{matrix}/GEN", "pe");
		h1_inverse_reco[i]->Draw("pe");
		h1_reco[i]->Divide(h1_gen[i]);
		h1_reco[i]->SetMarkerStyle(29);
		h1_reco[i]->SetMarkerColor(3);
		h1_reco[i]->SetMarkerSize(2);
		h1_reco[i]->Draw("same pe");
		leg_ratio[i]->AddEntry(h1_reco[i], "RECO/GEN", "pe");
		leg_ratio[i]->Draw();
		t1[i]->Draw();
		c_GEN_ratio[i]->SaveAs(Form("GEN_ratio_MuID_%s_%d_test.pdf", datastring.Data(), i));
		c_GEN_ratio[i]->SaveAs(Form("GEN_ratio_MuID_%s_%d_test.png", datastring.Data(), i));
	}
}

void formPtArr(Double_t binmin, Double_t binmax, string*arr)
{
	Double_t intMin, intMax;
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 )
	{
		*arr = Form("%.0f-%.0f", binmin, binmax);
	}
	else if ( fracMin != 0 && fracMax == 0 )
	{
		*arr = Form("%.1f-%.0f", binmin, binmax);
	}
	else if ( fracMin == 0 && fracMax != 0 )
	{
		*arr = Form("%.0f-%.1f", binmin, binmax);
	}
	else
	{
		*arr = Form("%.1f-%.1f", binmin, binmax);
	}
}
