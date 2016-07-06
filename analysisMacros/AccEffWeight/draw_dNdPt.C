#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void draw_dNdPt(int isPA=1, bool isPrompt=true, bool noPtWeight=true, bool isLog=false)
{
	gROOT->Macro("../Style.C");

	const int nRap = 8;
	const int nPt = 9;
	const int nRapTmp = nRap+1;
	const int nPtTmp = nPt+1;
  //double ratiomin = 0.2;
  //double ratiomax = 1.8;
  double ratiomin = 0.0;
  double ratiomax = 2.0;
  if (!(isPA==0 || isPA==1)) {cout << "select among isPA = 0 or 1"<< endl; return ;}

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
	
  //// new mean pT with BkgSub, with new sgl mu acc cut
	Double_t px[nRap][nPt];
  Double_t px_pp[nRap][nPt] = {	 // tmp
		{2.51443, 3.50757, 4.47953, 5.70624, 6.96258, 7.96788, 9.17032, 11.5446, 17.8838}, 
		{0.00000, 0.00000, 4.52616, 5.74214, 6.97103, 7.97166, 9.18028, 11.4711, 17.7127}, 
		{0.00000, 0.00000, 0.00000, 0.00000, 7.01909, 7.99970, 9.19743, 11.5722, 17.7907}, 
		{0.00000, 0.00000, 0.00000, 0.00000, 7.12323, 8.01502, 9.23109, 11.6318, 17.8046},  
		{0.00000, 0.00000, 0.00000, 0.00000, 7.04981, 8.00419, 9.21309, 11.5536, 17.6251}, 
		{0.00000, 0.00000, 0.00000, 0.00000, 6.98218, 7.97548, 9.19420, 11.5218, 17.4151},
		{0.00000, 0.00000, 4.51540, 5.72587, 6.97029, 7.95706, 9.14684, 11.4619, 17.1339}, 
		{2.51848, 3.49477, 4.47722, 5.69022, 6.96768, 7.98316, 9.17100, 11.4908, 17.1958}
	}; // x point
	Double_t px_pA[nRap][nPt] = {	
		{2.51443, 3.50757, 4.47953, 5.70624, 6.96258, 7.96788, 9.17032, 11.5446, 17.8838}, 
		{0.00000, 0.00000, 4.52616, 5.74214, 6.97103, 7.97166, 9.18028, 11.4711, 17.7127}, 
		{0.00000, 0.00000, 0.00000, 0.00000, 7.01909, 7.99970, 9.19743, 11.5722, 17.7907}, 
		{0.00000, 0.00000, 0.00000, 0.00000, 7.12323, 8.01502, 9.23109, 11.6318, 17.8046},  
		{0.00000, 0.00000, 0.00000, 0.00000, 7.04981, 8.00419, 9.21309, 11.5536, 17.6251}, 
		{0.00000, 0.00000, 0.00000, 5.85766, 6.98218, 7.97548, 9.19420, 11.5218, 17.4151},
		{0.00000, 0.00000, 4.51540, 5.72587, 6.97029, 7.95706, 9.14684, 11.4619, 17.1339}, 
		{2.51848, 3.49477, 4.47722, 5.69022, 6.96768, 7.98316, 9.17100, 11.4908, 17.1958}
	}; // x point
	Double_t ex[nPt] = {0,0,0,0,0,0,0,0,0}; // x stat error
  for (Int_t iy=0; iy<nRap; iy++) {
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      if (isPA==0) { px[iy][ipt] = px_pp[iy][ipt]; }
      else { px[iy][ipt] = px_pA[iy][ipt]; }
    }
  }

	//// 1) y_CM array  (from forward to backward)
	Double_t rapArrNumFB_pA[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
  Double_t rapArrNumFB_pp[nRapTmp] = {2.4, 1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4};// for pt dist.
  Double_t rapArrNumFB[nRapTmp];
  for (Int_t iy=0; iy<nRapTmp; iy++) {
    if (isPA==0) { rapArrNumFB[iy] = rapArrNumFB_pp[iy]; }
    else { rapArrNumFB[iy] = rapArrNumFB_pA[iy]; }
	}
  Double_t rapBinW[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1]; 
		//rapBinW[iy] = rapArrNumBF[iy+1]-rapArrNumBF[iy]; 
	}
	//// 2) pt array
	Double_t ptArrNum[nPtTmp] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};//8rap9pt
	Double_t ptBinW[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
	}
	//// array string
	TString rapArr[nRap];
	for (Int_t iy=0; iy<nRap; iy++) {
		formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
		cout << iy <<"th rapArr = " << rapArr[iy] << endl;
	}
	TString ptArr[nPt];
	for (Int_t ipt=0; ipt<nPt; ipt++) {
		formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
		cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
	}
	
  //////////////////////////////////////////////////////////////	
	//// read-in corr-yield file
	TFile * f2D;
  if (isPA==0) {
    if (noPtWeight) f2D = new TFile("../FittingResult/totalHist_pp_8rap9pt_newcut_nominal_Zvtx0_SF1_noPtWeight.root");
//    if (noPtWeight) f2D = new TFile("../FittingResult/totalHist_pp_8rap9pt_newcut_nominal_Zvtx0_SF0_noPtWeight.root");
    else f2D = new TFile("../FittingResult/totalHist_pp_8rap9pt_newcut_nominal_Zvtx0_SF1.root");
  } else {
    if (noPtWeight) f2D = new TFile("../FittingResult/totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF1_noPtWeight.root");
    else f2D = new TFile("../FittingResult/totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF1.root");

  }
	//// read-in 2D hist for corrected yield
	TH2D* h2D_Data;
	TH2D* h2D_MC;
	if (isPA==0) {
	  if (isPrompt) { 
      h2D_Data = (TH2D*)f2D->Get("h2D_Fit_PR_pp");
      h2D_MC = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_PR_pp");
    }
	  else { 
      h2D_Data = (TH2D*)f2D->Get("h2D_Fit_NP_pp");
      h2D_MC = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_NP_pp");
    }
  } else {
	  if (isPrompt) { 
      h2D_Data = (TH2D*)f2D->Get("h2D_Fit_PR_pA");
      h2D_MC = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_PR_pA");
    }
	  else { 
      h2D_Data = (TH2D*)f2D->Get("h2D_Fit_NP_pA");
      h2D_MC = (TH2D*)f2D->Get("otherMCInfo/h2D_Eff_Num_NP_pA");
    }
  }
	h2D_Data->SetName("h2D_Data");
	h2D_MC->SetName("h2D_MC");
	const int nbinsX = h2D_Data->GetNbinsX();
	const int nbinsY = h2D_Data->GetNbinsY();
	if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
	if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };

	////  projection to 1D hist : iy=0 refers to forwards !!! (ordering here)
	TH1D* h1D_Data[nbinsX]; 
	TH1D* h1D_MC[nbinsX]; 
	for (Int_t iy = 0; iy < nRap; iy++) {
		if ( isPA==0) {
	    h1D_Data[iy] = h2D_Data->ProjectionY(Form("h1D_Data_%d",iy),nRap-iy,nRap-iy);
		  h1D_MC[iy] = h2D_MC->ProjectionY(Form("h1D_MC_%d",iy),nRap-iy,nRap-iy);
		} else {
	    h1D_Data[iy] = h2D_Data->ProjectionY(Form("h1D_Data_%d",iy),iy+1,iy+1);
		  h1D_MC[iy] = h2D_MC->ProjectionY(Form("h1D_MC_%d",iy),iy+1,iy+1);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	
  //// set values as zero for unused bins
	for (Int_t iy = 0; iy < nRap; iy++) {
		if (iy>=1 && iy<=6) {
			h1D_Data[iy]->SetBinContent(1,0);
			h1D_Data[iy]->SetBinError(1,0);
			h1D_Data[iy]->SetBinContent(2,0);
			h1D_Data[iy]->SetBinError(2,0);
			h1D_MC[iy]->SetBinContent(1,0);
			h1D_MC[iy]->SetBinError(1,0);
			h1D_MC[iy]->SetBinContent(2,0);
			h1D_MC[iy]->SetBinError(2,0);
		}
		if (iy>=2 && iy<=5) {
			h1D_Data[iy]->SetBinContent(3,0);
			h1D_Data[iy]->SetBinError(3,0);
			h1D_MC[iy]->SetBinContent(3,0);
			h1D_MC[iy]->SetBinError(3,0);
		}
		if (isPA==0) {
		  if (iy>=2 && iy<=5) { //for_pp
			  h1D_Data[iy]->SetBinContent(4,0);
			  h1D_Data[iy]->SetBinError(4,0);
			  h1D_MC[iy]->SetBinContent(4,0);
			  h1D_MC[iy]->SetBinError(4,0);
		  }
    } else {
      if (iy>=2 && iy<=4) { //for_pA
  			h1D_Data[iy]->SetBinContent(4,0);
  			h1D_Data[iy]->SetBinError(4,0);
  			h1D_MC[iy]->SetBinContent(4,0);
  			h1D_MC[iy]->SetBinError(4,0);
      }
		}
	}
	
  //// after zero-bin setting, normalize!
	for (Int_t iy = 0; iy < nRap; iy++) {
		h1D_Data[iy]->Scale(1./h1D_Data[iy]->Integral());
		h1D_MC[iy]->Scale(1./h1D_MC[iy]->Integral());
		h1D_Data[iy]->Scale(1,"width");
		h1D_MC[iy]->Scale(1,"width");
	}
  
  //// calculate ratio
  TH1D* hRatio[nRap];
	for (Int_t iy = 0; iy < nRap; iy++) {
		hRatio[iy]=(TH1D*)h1D_Data[iy]->Clone(Form("hRatio_%d",iy));
		hRatio[iy]->Divide(h1D_MC[iy]);
	}
	
  //////////////////////////////////////////////////////////////////
	//// Draw histograms
	TLegend *legUR = new TLegend(0.65, 0.75, 0.86, 0.83);
	SetLegendStyle(legUR);
	 	
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);
	
	TCanvas* c1 = new TCanvas("c1","c1",200,10,1600,800);
	c1->Divide(4,2);
	
  TGraphAsymmErrors* g_Data[nRap];
  TGraphAsymmErrors* g_MC[nRap];
	double maxVal=0.;
	
  for (Int_t iy = 0; iy < nRap; iy++) {
		c1->cd(iy+1);
	  if (isLog) gPad->SetLogy(1);
	  else gPad->SetLogy(0);
		g_Data[iy]=new TGraphAsymmErrors(h1D_Data[iy]);
		g_Data[iy]->SetName(Form("g_Data_%d",iy));
		g_MC[iy]=new TGraphAsymmErrors(h1D_MC[iy]);
		g_MC[iy]->SetName(Form("g_MC_%d",iy));
		SetGraphStyle(g_Data[iy],1,0);
		SetGraphStyle(g_MC[iy],2,10);
  	for (Int_t ipt=0; ipt<nPt; ipt++ ){
			g_Data[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);	
			g_Data[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			g_Data[iy]->SetPointEXlow(ipt, ex[ipt]);
			g_Data[iy]->SetPointEXhigh(ipt, ex[ipt]);
			g_MC[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);	
			g_MC[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			g_MC[iy]->SetPointEXlow(ipt, ex[ipt]);
			g_MC[iy]->SetPointEXhigh(ipt, ex[ipt]);
		}
		g_Data[iy]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		g_Data[iy]->GetXaxis()->CenterTitle();
		g_Data[iy]->GetXaxis()->SetLimits(0.,30.);
		g_Data[iy]->GetYaxis()->SetTitle("dN/p_{T}");
		g_Data[iy]->GetYaxis()->CenterTitle();
		if (!isLog){
			maxVal = g_Data[iy]->GetMaximum();
			g_Data[iy]->SetMinimum(0.);
			//g_Data[iy]->SetMaximum(maxVal*1.000000000001);
			g_Data[iy]->SetMaximum(0.28);
		}
		g_Data[iy]->Draw("ap");
		g_MC[iy]->Draw("p");
		
		if (iy==0){
      if (isPA==0) {
			  if (isPrompt) latex->DrawLatex(0.19,0.88,"pp Prompt J/#psi");
			  else latex->DrawLatex(0.19, 0.88,"pp Non-prompt J/#psi");
      } else {
			  if (isPrompt) latex->DrawLatex(0.19,0.88,"pPb Prompt J/#psi");
			  else latex->DrawLatex(0.19, 0.88,"pPb Non-prompt J/#psi");
      }
		  legUR -> AddEntry(g_Data[iy],"Data","lp");
		  legUR -> AddEntry(g_MC[iy],"MC","lp");
		  legUR->Draw();
		}
    latex->DrawLatex(0.57,0.88,Form("%s",rapArr[iy].Data()));
	}
	if (isPA==0) c1->SaveAs(Form("dir_dNdPt/pp_dNdPt_isPrompt%d.pdf",(int)isPrompt));
	else c1->SaveAs(Form("dir_dNdPt/pA_dNdPt_isPrompt%d.pdf",(int)isPrompt));
	//legUR->Clear();
	
	
  //////////////////////////////////////////////////////////////////
	//// hRatio, and convert to gRatio (TGraphAsymErrors)
	//////////////////////////////////////////////////////////////////
	
  TCanvas* c2 = new TCanvas("c2","c2",200,10,1600,800);
	c2->Divide(4,2);	
	
	TGraphAsymmErrors* gRatio[nRap];
	
	for (Int_t iy = 0; iy < nRap; iy++) {
		gRatio[iy]=new TGraphAsymmErrors(hRatio[iy]);
		gRatio[iy]->SetName(Form("gRatio_%d",iy));
	  SetGraphStyle(gRatio[iy],4,0);
  	for (Int_t ipt=0; ipt<nPt; ipt++ ){
			gRatio[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);	
			gRatio[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
			gRatio[iy]->SetPointEXlow(ipt, ex[ipt]);
			gRatio[iy]->SetPointEXhigh(ipt, ex[ipt]);
		}
		c2->cd(iy+1);
		gRatio[iy]->GetXaxis()->SetLimits(0.0,25.);
		gRatio[iy]->GetXaxis()->CenterTitle();
		gRatio[iy]->SetMinimum(ratiomin);
		gRatio[iy]->SetMaximum(ratiomax);
		gRatio[iy]->GetYaxis()->CenterTitle();
		gRatio[iy]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
		gRatio[iy]->GetYaxis()->SetTitle("Data/MC");
		gRatio[iy]->Draw("AP");
		if (iy==0){
      if (isPA==0) {
			  if (isPrompt) latex->DrawLatex(0.19,0.23,"pp Prompt J/#psi");
			  else latex->DrawLatex(0.19, 0.23,"pp Non-prompt J/#psi");
      } else {
			  if (isPrompt) latex->DrawLatex(0.19,0.23,"pPb Prompt J/#psi");
			  else latex->DrawLatex(0.19, 0.23,"pPb Non-prompt J/#psi");
      }
		}
		if (isPrompt) latex->DrawLatex(0.57,0.88,Form("%s",rapArr[iy].Data()));
		else latex->DrawLatex(0.57,0.22,Form("%s",rapArr[iy].Data()));
		dashedLine(0.,1.,25.,1.,1,1);
	}	
	if (isPA==0) c2->SaveAs(Form("dir_dNdPt/pp_dNdPtRatio_isPrompt%d.pdf",(int)isPrompt));
	else c2->SaveAs(Form("dir_dNdPt/pA_dNdPtRatio_isPrompt%d.pdf",(int)isPrompt));

	//////////////////////////////////////////////////////////////////
	// root file
	TFile *fOut;
  if (isPA==0) fOut= new TFile(Form("dir_dNdPt/pp_dNdPt_isPrompt%d.root",(int)isPrompt),"RECREATE");
  else fOut= new TFile(Form("dir_dNdPt/pA_dNdPt_isPrompt%d.root",(int)isPrompt),"RECREATE");
	fOut->cd();
	for (Int_t iy = 0; iy < nRap; iy++) {
		h1D_Data[iy]->Write();
		h1D_MC[iy]->Write();
		hRatio[iy]->Write();
		g_Data[iy]->Write();
		g_MC[iy]->Write();
		gRatio[iy]->Write();
	}
	fOut->Close();
	if (isPA==0) cout << "pp_dNdPt_isPrompt" <<(int)isPrompt<<".root has been created :)" <<endl; 
	else cout << "pA_dNdPt_isPrompt" <<(int)isPrompt<<".root has been created :)" <<endl; 
	
	return;

} // end of main func.

void formRapArr(Double_t binmin, Double_t binmax, TString* arr) {
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

void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr) {
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

void formPtArr(Double_t binmin, Double_t binmax, TString* arr) {
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


