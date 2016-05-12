#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);

void draw_DataMCRecoPt_pA(bool isPrompt=false, bool isLog=false)
{
	gROOT->Macro("../Style.C");

	const int nRap = 8;
	const int nPt = 9;
	const int nRapTmp = nRap+1;
	const int nPtTmp = nPt+1;
  double ratiomin = 0.2;
  double ratiomax = 1.8;
  //double ratiomin = 0.5;
  //double ratiomax = 1.5;

	Double_t pxtmp[nRap][nPt]; //x point to fill temp
	Double_t pytmp[nRap][nPt]; //y point to fill temp
	/*
	Double_t px[nRap][nPt] = {	
		{2.49530, 3.48529, 4.47275, 5.68792, 6.9644, 7.96271, 9.1754, 11.5315, 17.7588}, 
		{0.00000, 3.53123, 4.5027, 5.71709, 6.96523, 7.9693, 9.17314, 11.4952, 17.6927}, 
		{0.00000, 0.00000, 0.00000, 0.00000, 7.01977, 7.99712, 9.19936, 11.5743, 17.7732},
		{0.00000, 0.00000, 0.00000, 0.00000, 7.12292, 8.01305, 9.22816, 11.6279, 17.8879}, 
		{0.00000, 0.00000, 0.00000, 0.00000, 7.05476, 8.00208, 9.21589, 11.5645, 17.7176}, 
		{0.00000, 0.00000, 0.00000, 5.82095, 6.97886, 7.96732, 9.18979, 11.5158, 17.4116}, 
		{0.00000, 3.52893, 4.48328, 5.69351, 6.96188, 7.95707, 9.14886, 11.4747, 17.231},
		{2.49481, 3.47853, 4.46938, 5.6761, 6.96419, 7.97702, 9.16158, 11.5077, 17.3061}
	}; // x point
	*/
	// new mean pT with BkgSub, with new sgl mu acc cut
	Double_t px[nRap][nPt] = {	
		{2.51443, 3.50757, 4.47953, 5.70624, 6.96258, 7.96788, 9.17032, 11.5446, 17.8838}, 
		{0.00000, 3.56222, 4.52616, 5.74214, 6.97103, 7.97166, 9.18028, 11.4711, 17.7127}, 
		{0.00000, 0.00000, 0.00000, 0.00000, 7.01909, 7.99970, 9.19743, 11.5722, 17.7907}, 
		{0.00000, 0.00000, 0.00000, 0.00000, 7.12323, 8.01502, 9.23109, 11.6318, 17.8046},  
		{0.00000, 0.00000, 0.00000, 0.00000, 7.04981, 8.00419, 9.21309, 11.5536, 17.6251}, 
		{0.00000, 0.00000, 0.00000, 5.85766, 6.98218, 7.97548, 9.19420, 11.5218, 17.4151},
		{0.00000, 3.57190, 4.51540, 5.72587, 6.97029, 7.95706, 9.14684, 11.4619, 17.1339}, 
		{2.51848, 3.49477, 4.47722, 5.69022, 6.96768, 7.98316, 9.17100, 11.4908, 17.1958}
	}; // x point
	Double_t ex[nPt] = {0,0,0,0,0,0,0,0,0}; // x stat error

	////// y_CM array 
	//// for_pA
	Double_t rapArrNumFB[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
	//// for_pp
  //Double_t rapArrNumFB[nRapTmp] = {2.4, 1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4};// for pt dist.
	Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1]; 
		//rapBinW[iy] = rapArrNumBF[iy+1]-rapArrNumBF[iy]; 
	}
	//////// pt array
	Double_t ptArrNum[nPtTmp] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};//8rap9pt
	Double_t ptBinW[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
	}

	// array string
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
	TFile * f2D = new TFile("../FittingResult/totalHist_pA_8rap9pt_newcut_Zvtx1_SF1_noPtWeight.root","READ");

	// --- read-in 2D hist for data reco dist
	TH2D* h2D_Fit_pA;
	if (isPrompt) { h2D_Fit_pA = (TH2D*)f2D->Get("h2D_Fit_PR_pA");}
	else { h2D_Fit_pA = (TH2D*)f2D->Get("h2D_Fit_NP_pA"); }
	h2D_Fit_pA->SetName("h2D_Fit_pA");
	cout << "h2D_Fit_pA = " << h2D_Fit_pA << endl;
	// --- read-in 2D hist for MC reco dist.
	 TH2D* h2D_Eff_Num_pA;
	if (isPrompt) { h2D_Eff_Num_pA = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_PR_pA"); }
	else { h2D_Eff_Num_pA = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_NP_pA"); }
	h2D_Eff_Num_pA->SetName("h2D_Eff_Num_pA");
	cout << "h2D_Eff_Num_pA = " << h2D_Eff_Num_pA << endl;

	const int nbinsX = h2D_Fit_pA->GetNbinsX();
	const int nbinsY = h2D_Fit_pA->GetNbinsY();
	cout << "nbinsX = " << nbinsX << ", nbinsY = " << nbinsY << endl;
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
	
	// ---  projection to 1D hist
	TH1D* h1D_data_pA[nbinsX]; 
	TH1D* h1D_MC_pA[nbinsX]; 
	// iy=0 refers to forwards !!! (ordering here)
	for (Int_t iy = 0; iy < nbinsX; iy++) {
	  //// for_pA
		h1D_data_pA[iy] = h2D_Fit_pA->ProjectionY(Form("h1D_data_pA_%d",iy),iy+1,iy+1);
		h1D_MC_pA[iy] = h2D_Eff_Num_pA->ProjectionY(Form("h1D_MC_pA_%d",iy),iy+1,iy+1);
	  //// for_pp
		//h1D_data_pp[iy] = h2D_Fit_pp->ProjectionY(Form("h1D_data_pp_%d",iy),nbinsX-iy,nbinsX-iy);
		//h1D_MC_pp[iy] = h2D_Eff_Num_pp->ProjectionY(Form("h1D_MC_pp_%d",iy),nbinsX-iy,nbinsX-iy);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	
	// set values as zero for unused bins
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		if (iy>=1 && iy<=6) {
			h1D_data_pA[iy]->SetBinContent(1,0);
			h1D_data_pA[iy]->SetBinError(1,0);
			h1D_MC_pA[iy]->SetBinContent(1,0);
			h1D_MC_pA[iy]->SetBinError(1,0);
			h1D_data_pA[iy]->SetBinContent(2,0);
			h1D_data_pA[iy]->SetBinError(2,0);
			h1D_MC_pA[iy]->SetBinContent(2,0);
			h1D_MC_pA[iy]->SetBinError(2,0);
		}
		if (iy>=2 && iy<=5) {
			h1D_data_pA[iy]->SetBinContent(3,0);
			h1D_data_pA[iy]->SetBinError(3,0);
			h1D_MC_pA[iy]->SetBinContent(3,0);
			h1D_MC_pA[iy]->SetBinError(3,0);
		}
		if (iy>=2 && iy<=4) { //for_pA
		//if (iy>=2 && iy<=5) { //for_pp
			h1D_data_pA[iy]->SetBinContent(4,0);
			h1D_data_pA[iy]->SetBinError(4,0);
			h1D_MC_pA[iy]->SetBinContent(4,0);
			h1D_MC_pA[iy]->SetBinError(4,0);
		}
	}
	////// after zero-bin setting, normalize!
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_data_pA[iy]->Scale(1./h1D_data_pA[iy]->Integral());
		h1D_MC_pA[iy]->Scale(1./h1D_MC_pA[iy]->Integral());
		h1D_data_pA[iy]->Scale(1,"width");
		h1D_MC_pA[iy]->Scale(1,"width");
	}
	
  //// calculate ratio
  TH1D* hRatio_pA[nRap];
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		hRatio_pA[iy]=(TH1D*)h1D_data_pA[iy]->Clone(Form("hRatio_pA_%d",iy));
		hRatio_pA[iy]->Divide(h1D_MC_pA[iy]);
	}
	
  //////////////////////////////////////////////////////////////////
	//// --- Draw histograms
	TLegend *legUR = new TLegend(0.45, 0.75, 0.86, 0.92);
	SetLegendStyle(legUR);
	 	
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);
	
	TCanvas* c_pA = new TCanvas("c_pA","c_pA",200,10,1600,800);
	c_pA->Divide(4,2);
	
  TGraphAsymmErrors* g_data_pA[nbinsX];
  TGraphAsymmErrors* g_MC_pA[nbinsX];
	double maxVal=0.;
	
  for (Int_t iy = 0; iy < nbinsX; iy++) {
		c_pA->cd(iy+1);
	  if (isLog) gPad->SetLogy(1);
	  else gPad->SetLogy(0);
		g_data_pA[iy]=new TGraphAsymmErrors(h1D_data_pA[iy]);
		g_data_pA[iy]->SetName(Form("g_data_pA_%d",iy));
		g_MC_pA[iy]=new TGraphAsymmErrors(h1D_MC_pA[iy]);
		g_MC_pA[iy]->SetName(Form("g_MC_pA_%d",iy));
		SetGraphStyle(g_data_pA[iy],1,0);
		SetGraphStyle(g_MC_pA[iy],2,10);
		g_data_pA[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		g_data_pA[iy]->GetXaxis()->CenterTitle();
		g_data_pA[iy]->GetXaxis()->SetLimits(0.,30.);
		g_data_pA[iy]->GetYaxis()->SetTitle("");
		if (!isLog){
			maxVal = g_data_pA[iy]->GetMaximum();
			g_data_pA[iy]->SetMinimum(0.);
//			g_data_pA[iy]->SetMaximum(maxVal*1.3);
		}
		g_data_pA[iy]->Draw("ap");
		g_MC_pA[iy]->Draw("p");
		if (iy==0){
			if (isPrompt) legUR -> SetHeader("pA Prompt J/#psi");
			else legUR -> SetHeader("pA Non-prompt J/#psi");
			legUR -> AddEntry(g_data_pA[iy],"data RECO","lp");
			legUR -> AddEntry(g_MC_pA[iy],"MC RECO","lp");
			//legUR->Draw();
			latex->DrawLatex(0.56,0.68,Form("%s",rapArr[iy].c_str()));
		}
		else latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
	}
	legUR->Clear();
	
	
  //////////////////////////////////////////////////////////////////
	//// hRatio, and convert to gRatio (TGraphAsymErrors)
	//////////////////////////////////////////////////////////////////
	
  TCanvas* c_pA_2 = new TCanvas("c_pA_2","c_pA_2",200,10,1600,800);
	c_pA_2->Divide(4,2);	
	
	TGraphAsymmErrors* gRatio_pA[nbinsX];
	
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		gRatio_pA[iy]=new TGraphAsymmErrors(hRatio_pA[iy]);
		gRatio_pA[iy]->SetName(Form("gRatio_pA_%d",iy));
	  SetGraphStyle(gRatio_pA[iy],4,0);
  	for (Int_t ipt=0; ipt<nbinsY; ipt++ ){
			gRatio_pA[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);	
			gRatio_pA[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gRatio_pA[iy]->SetPointEXlow(ipt, ex[ipt]);
			gRatio_pA[iy]->SetPointEXhigh(ipt, ex[ipt]);
		}
		c_pA_2->cd(iy+1);
		gRatio_pA[iy]->GetXaxis()->SetLimits(0.0,25.);
		gRatio_pA[iy]->GetXaxis()->CenterTitle();
		gRatio_pA[iy]->SetMinimum(ratiomin);
		gRatio_pA[iy]->SetMaximum(ratiomax);
		gRatio_pA[iy]->GetYaxis()->CenterTitle();
		gRatio_pA[iy]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
		gRatio_pA[iy]->GetYaxis()->SetTitle("Data/MC");
		gRatio_pA[iy]->Draw("AP");
		if (iy==0){
			if (isPrompt) latex->DrawLatex(0.23,0.23,"pA Prompt J/#psi");
			else latex->DrawLatex(0.23, 0.23,"pA Non-prompt J/#psi");
		}
		latex->DrawLatex(0.56,0.88,Form("%s",rapArr[iy].c_str()));
		dashedLine(0.,1.,25.,1.,1,1);
	}	
	c_pA_2->SaveAs(Form("dir_DataMcRecoPt/pA_DataMcRecoPtRatio_isPrompt%d.pdf",(int)isPrompt));

	//////////////////////////////////////////////////////////////////
	// root file
	TFile *fOut = new TFile(Form("dir_DataMcRecoPt/pA_DataMcRecoPt_isPrompt%d.root",(int)isPrompt),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nbinsX; iy++) {
		h1D_data_pA[iy]->Write();
		h1D_MC_pA[iy]->Write();
		hRatio_pA[iy]->Write();
		g_data_pA[iy]->Write();
		g_MC_pA[iy]->Write();
		gRatio_pA[iy]->Write();
	}
	fOut->Close();
	cout << "pA_DataMcRecoPt_isPrompt" <<(int)isPrompt<<".root has been created :)" <<endl; 
	
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


