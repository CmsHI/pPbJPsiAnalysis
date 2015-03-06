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

void Decomposition_yeonju(bool isPrompt = true, bool is1st = true)
{
	gStyle->SetOptStat(0000);
	gStyle->SetPaintTextFormat(".2f");
	gStyle->SetTextSize(1.0);

	const char *samplename;
	if(isPrompt == 1 && is1st == 1) samplename = "PRMCboosted_Pbp";
	else if(isPrompt == 1 && is1st != 1) samplename = "PRMCboosted_pPb";
	else if(isPrompt != 1 && is1st == 1) samplename = "NPMCboosted_Pbp";
	else if(isPrompt != 1 && is1st != 1) samplename = "NPMCboosted_pPb";

	const char *strBinning = "8rap9ptMatchdR";
	TString datastring = Form("%s_%s", strBinning, samplename);

	TFile *fin = new TFile(Form("recoPt_genPt_%s_diffRap.root", datastring.Data()), "READ");
	TFile *fout = new TFile(Form("Deconvolution_%s.root", datastring.Data()), "RECREATE");

	// define binning
    
	Double_t ptBinsArrfine[] = {0.0,3.0,4.0,5.0,6.5,7.5,8.5,10.0,14.0,30.0,35.0};
	const Int_t nPtBinsfine = sizeof(ptBinsArrfine)/sizeof(double)-1;
	cout <<"nPtBinsfine = " << nPtBinsfine << endl;

	Double_t etaBinsArrPbp[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4};//1st run
	Double_t etaBinsArrpPb[] = {-2.4, -1.93, -1.46, -1.03, -0.43, 0.47, 1.37, 1.97, 2.4};//2nd run
	const Int_t nEtaBins = sizeof(etaBinsArrPbp)/sizeof(double)-1;

    int nPt_Pbp[nEtaBins]={10,10,9,7,7,9,10,10};
    int nPt_pPb[nEtaBins]={10,10,9,8,7,8,10,10};

//    int nPt_Pbp[nEtaBins]={10,9,6,6,6,7,9,10};
//    int nPt_pPb[nEtaBins]={10,9,7,6,6,6,9,10};

//    cout <<"nPt : " << nPt[0] << endl;
	// define canvas and histogram
	TCanvas *c_Matrix[nEtaBins];
	TCanvas *c_Invert[nEtaBins];
	TH2D *h_Reco_Gen_Matrix[nEtaBins];

	for(Int_t i = 0; i < nEtaBins; i ++)
	{
		c_Matrix[i] = new TCanvas(Form("c_Matrix_%d", i), "", 0, 10, 1200, 1200);
		c_Invert[i] = new TCanvas(Form("c_Invert_%d", i), "", 0, 10, 1200, 1200);

		h_Reco_Gen_Matrix[i] = new TH2D(Form("h_Reco_Gen_Matrix_%d", i), "", nPtBinsfine, ptBinsArrfine, nPtBinsfine, ptBinsArrfine);
		h_Reco_Gen_Matrix[i]->Sumw2();

        TMatrixD M_Matrix(nPtBinsfine, nPtBinsfine);
        if(is1st) M_Matrix.ResizeTo(nPt_Pbp[i], nPt_Pbp[i]);
        else M_Matrix.ResizeTo(nPt_pPb[i], nPt_pPb[i]);

		h_Reco_Gen_Matrix[i] = (TH2D*) fin->Get(Form("h2D_fixedMatrix_%s_%d", strBinning, i));
        cout << "**** str Binning " << i << "  ->" << strBinning << endl;

    if(is1st){	
            for(Int_t j = 0; j < nPt_Pbp[i]; j++){
                for(Int_t k = 0; k < nPt_Pbp[i]; k++){
                    M_Matrix(k, j) = h_Reco_Gen_Matrix[i]->GetBinContent(j+1+(10-nPt_Pbp[i]), k+1+(10-nPt_Pbp[i]));
                }
            }
        } else {
            for(Int_t j = 0; j < nPt_pPb[i]; j++){
                for(Int_t k = 0; k < nPt_pPb[i]; k++){
                    M_Matrix(k, j) = h_Reco_Gen_Matrix[i]->GetBinContent(j+1+(10-nPt_pPb[i]), k+1+(10-nPt_pPb[i]));
                }
            }
        }    

		// draw the matrix
		c_Matrix[i]->cd();
		M_Matrix.Draw("text colz");
		c_Matrix[i]->SaveAs(Form("pdf/Matrix_MuID_%s_%d_yeonju_1st.pdf", datastring.Data(), i));
	//	c_Matrix[i]->SaveAs(Form("pdf/Matrix_MuID_%s_%d_yeonju_1st.png", datastring.Data(), i));
		// invert matrix and draw
		M_Matrix.Invert();

		c_Invert[i]->cd();
		M_Matrix.Draw("text colz");
		c_Invert[i]->SaveAs(Form("pdf/Invert_MuID_%s_%d_yeonju_1st.pdf", datastring.Data(), i));
	//	c_Invert[i]->SaveAs(Form("pdf/Invert_MuID_%s_%d_yeonju_1st.png", datastring.Data(), i));

		}
		return;	
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
