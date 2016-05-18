#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

void draw_1D_eff_Zvtx1_Zvtx0_pA(TString szBinning="8rap9pt", bool isPrompt=false, bool isLog=false, bool isNoErr=true)
{
	gROOT->Macro("../Style.C");

  double ratiomin = 0.9;
  double ratiomax = 1.1;
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  	
	////rap array in yCM (from forward to backward)
	Double_t rapArrNumFB[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
	//Double_t rapArrNumBF[] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
	const Int_t nRap = sizeof(rapArrNumFB)/sizeof(Double_t)-1;
	cout << "nRap = " << nRap << endl;
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1]; 
	}
	////pt array
	Double_t ptArrNum[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.}; //8rap9pt
	//Double_t ptArrNum[] = {5.0, 6.5, 10., 30.}; //8rap3pt
	const Int_t nPt = sizeof(ptArrNum)/sizeof(Double_t)-1;
	cout << "nPt = " << nPt << endl;
	Double_t ptBinW[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
	}
	//// array string
	string rapArr[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
		cout << iy <<"th rapArr = " << rapArr[iy] << endl;
	}
	string ptArr[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
		cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
	}
	
	// --- read-in file
	TFile * f2D_pA01 = new TFile(Form("../FittingResult/totalHist_pA_%s_newcut_Zvtx1_SF0_noPtWeight.root",szBinning.Data()));
	TFile * f2D_pA02 = new TFile(Form("../FittingResult/totalHist_pA_%s_newcut_Zvtx0_SF0_noPtWeight.root",szBinning.Data()));

	// --- read-in 2D hist for data reco dist
	TH2D* h2D_pA01 = (TH2D*)f2D_pA01->Get(Form("h2D_Eff_%s_pA",szPrompt.Data()));
	TH2D* h2D_pA02 = (TH2D*)f2D_pA02->Get(Form("h2D_Eff_%s_pA",szPrompt.Data()));
	cout << "h2D_pA01 = " << h2D_pA01 << endl;
	cout << "h2D_pA02 = " << h2D_pA02 << endl;

	const int nbinsX = h2D_pA01->GetNbinsX();
	const int nbinsY = h2D_pA01->GetNbinsY();
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
	
	// ---  projection to 1D hist
	TH1D* h1D_pA01[nbinsX]; 
	TH1D* h1D_pA02[nbinsX]; 
	//// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_pA01[iy] = h2D_pA01->ProjectionY(Form("h1D_pA01_%d",iy),iy+1,iy+1);
		h1D_pA02[iy] = h2D_pA02->ProjectionY(Form("h1D_pA02_%d",iy),iy+1,iy+1);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	//// --- set values as zero for unused bins
	//// 8rap9pt pA
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		if (iy>=1 && iy<=6) {
			h1D_pA01[iy]->SetBinContent(1,-532);
			h1D_pA01[iy]->SetBinError(1,0);
			h1D_pA02[iy]->SetBinContent(1,+532);
			h1D_pA02[iy]->SetBinError(1,0);
			h1D_pA01[iy]->SetBinContent(2,-532);
			h1D_pA01[iy]->SetBinError(2,0);
			h1D_pA02[iy]->SetBinContent(2,+532);
			h1D_pA02[iy]->SetBinError(2,0);
		}
		if (iy>=2 && iy<=5) {
			h1D_pA01[iy]->SetBinContent(3,-532);
			h1D_pA01[iy]->SetBinError(3,0);
			h1D_pA02[iy]->SetBinContent(3,+532);
			h1D_pA02[iy]->SetBinError(3,0);
		}
		if (iy>=2 && iy<=4) {
			h1D_pA01[iy]->SetBinContent(4,-532);
			h1D_pA01[iy]->SetBinError(4,0);
			h1D_pA02[iy]->SetBinContent(4,+532);
			h1D_pA02[iy]->SetBinError(4,0);
		}
	}
/*	
  ////// after zero-bin setting, normalize!
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_pA01[iy]->Scale(1./h1D_pA01[iy]->Integral());
		h1D_pA01[iy]->Scale(1,"width");
		h1D_pA02[iy]->Scale(1./h1D_pA02[iy]->Integral());
		h1D_pA02[iy]->Scale(1,"width");
	}
*/
  
  //// calculate ratio
  TH1D* hRatio_pA[nRap];
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		hRatio_pA[iy]=(TH1D*)h1D_pA01[iy]->Clone(Form("hRatio_pA_%d",iy));
		hRatio_pA[iy]->Divide(h1D_pA02[iy]);
    if (isNoErr) {
      for (int ipt=0; ipt<nbinsY; ipt++) {
        hRatio_pA[iy]->SetBinError(ipt+1,0.);
        cout << iy <<"th rap, "<<ipt<<"th pT, ratio = "<<hRatio_pA[iy]->GetBinContent(ipt+1)<<endl;
      }
    }
  }
	
	//////////////////////////////////////////////////////////////////
	//// Draw plots
	//////////////////////////////////////////////////////////////////
  TGraphAsymmErrors* g_pA01[nbinsX];
  TGraphAsymmErrors* g_pA02[nbinsX];

	TLegend *legBR = new TLegend(0.55, 0.28, 0.86, 0.40);
	SetLegendStyle(legBR);
	 	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	TCanvas* c01 = new TCanvas("c01","c01",200,10,1600,800);
	c01->Divide(4,2);
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		g_pA01[iy]=new TGraphAsymmErrors(h1D_pA01[iy]);
		g_pA01[iy]->SetName(Form("g_pA01_%d",iy));
		g_pA02[iy]=new TGraphAsymmErrors(h1D_pA02[iy]);
		g_pA02[iy]->SetName(Form("g_pA02_%d",iy));
		
    c01->cd(iy+1);
		if (isLog) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		SetGraphStyle(g_pA01[iy],3,0);
		SetGraphStyle(g_pA02[iy],4,10);
		
		g_pA01[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		g_pA01[iy]->GetXaxis()->CenterTitle();
		g_pA01[iy]->GetXaxis()->SetLimits(0.0,30.0);
		g_pA01[iy]->GetYaxis()->SetTitle("Efficiency");
		g_pA01[iy]->GetYaxis()->SetRangeUser(0,1.0);
		
		g_pA01[iy]->Draw("AP");
		g_pA02[iy]->Draw("P");
		if (iy==0) {
			legBR -> AddEntry(g_pA01[iy],"z vtx weight","lp");
			legBR -> AddEntry(g_pA02[iy],"no weight","lp");
			legBR->Draw();
			if (isPrompt) latex->DrawLatex(0.19,0.23,"pA Prompt J/#psi");
			else latex->DrawLatex(0.19, 0.23,"pA Non-prompt J/#psi");
		}
		latex->DrawLatex(0.55,0.23,Form("%s",rapArr[iy].c_str()));
	}
	c01->Modified();
	c01->Update();
	c01->SaveAs(Form("dir_1D_%s/eff_isPrompt%d_isLog%d_Zvtx1_Zvtx0_pA.pdf",szBinning.Data(),(int)isPrompt,(int)isLog));

  //////////////////////////////////////////////////////////////////
	//// hRatio, and convert to gRatio (TGraphAsymErrors)
	//////////////////////////////////////////////////////////////////
	
  TCanvas* c02 = new TCanvas("c02","c02",200,10,1600,800);
	c02->Divide(4,2);	
	
	TGraphAsymmErrors* gRatio_pA[nbinsX];
	
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		gRatio_pA[iy]=new TGraphAsymmErrors(hRatio_pA[iy]);
		gRatio_pA[iy]->SetName(Form("gRatio_pA_%d",iy));
	  SetGraphStyle(gRatio_pA[iy],1,0);
/*  	for (Int_t ipt=0; ipt<nbinsY; ipt++ ){
			gRatio_pA[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);	
			gRatio_pA[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gRatio_pA[iy]->SetPointEXlow(ipt, ex[ipt]);
			gRatio_pA[iy]->SetPointEXhigh(ipt, ex[ipt]);
		}*/
		c02->cd(iy+1);
		gRatio_pA[iy]->GetXaxis()->SetLimits(0.0,30.0);
		gRatio_pA[iy]->GetXaxis()->CenterTitle();
		gRatio_pA[iy]->SetMinimum(ratiomin);
		gRatio_pA[iy]->SetMaximum(ratiomax);
		gRatio_pA[iy]->GetYaxis()->CenterTitle();
		gRatio_pA[iy]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
		gRatio_pA[iy]->GetYaxis()->SetTitle("[Weight] / [No weight]");
		gRatio_pA[iy]->Draw("AP");
		if (iy==0){
			if (isPrompt) latex->DrawLatex(0.19,0.23,"pA Prompt J/#psi");
			else latex->DrawLatex(0.19, 0.23,"pA Non-prompt J/#psi");
		}
		latex->DrawLatex(0.55,0.23,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,30.,1.,1,1);
	}	
	c02->SaveAs(Form("dir_1D_%s/effRatio_isPrompt%d_isLog%d_Zvtx1_Zvtx0_pA.pdf",szBinning.Data(),(int)isPrompt,(int)isLog));





	return;

} // end of main func.

void formRapArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < y_{CM} < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < y_{CM} < %.2f", binmin, binmax);
	}
}

void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < |y_{CM}| < %.2f", binmin, binmax);
	}
}

void formPtArr(Double_t binmin, Double_t binmax, string* arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < p_{T} < %.1f GeV/c", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
	}
}


