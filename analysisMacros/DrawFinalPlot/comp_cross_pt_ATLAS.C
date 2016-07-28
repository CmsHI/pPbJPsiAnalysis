#include "../SONGKYO.h"
#include "CMS_lumi.h"

const int nAtlasBin = 5;

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);
void getMeanPtBins( Double_t* binArr, TF1* func);
void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void comp_cross_pt_ATLAS(bool shifted=false, bool isPrompt = false, bool isLog=true)
{
	gROOT->Macro("./tdrstyle_kyo.C");
	int isPA = 1;
	int iPos=0;
  
  //// BR and lumi info.
	const Double_t br = 0.0593 ;
	const Double_t brErr = 0.0006;
	const Double_t pp_lumi_pb =26.3; // 26.3/pb
	const Double_t pp_lumi_pb_err = 2.63; // tmp
	const Double_t pPb_lumi_nb = 34.622; // 34.6/nb
	const Double_t pPb_lumi_nb_err = 1.2;
	Double_t lumi_mub;
	Double_t lumi_mub_err;
  if (isPA==0) {
    lumi_mub = pp_lumi_pb *1000*1000;
    lumi_mub_err = pp_lumi_pb_err *1000*1000;
  }
  else if (isPA==1) {
    lumi_mub = pPb_lumi_nb * 1000;
    lumi_mub_err = pPb_lumi_nb_err * 1000;
  } else {
    cout << "select among isPA = 0 or 1"<< endl; return ;
  }
	cout << "isPA = " << isPA << ", and lumi_mub = " << lumi_mub <<"+-" <<lumi_mub_err <<  endl;

	//// read our result
	TFile *inFile;
  if (isPrompt) inFile = new TFile("plot_cross/pA_cross_pt_integ_middle_isPrompt1_isLog0_isScale0.root");
  else inFile= new TFile("plot_cross/pA_cross_pt_integ_middle_isPrompt0_isLog0_isScale0.root");
	TGraphAsymmErrors* g_cross_fwrap = (TGraphAsymmErrors*)inFile->Get("g_cross_fwrap"); 
	TGraphAsymmErrors* g_cross_bwrap = (TGraphAsymmErrors*)inFile->Get("g_cross_bwrap"); 
	TGraphAsymmErrors* g_cross_sys_fwrap = (TGraphAsymmErrors*)inFile->Get("g_cross_sys_fwrap"); 
	TGraphAsymmErrors* g_cross_sys_bwrap = (TGraphAsymmErrors*)inFile->Get("g_cross_sys_bwrap"); 

	//// put xErr (bin Width)
	const int nPt = 9;
	Double_t ex[nPt]= {0.5, 0.5, 0.5, 0.75, 0.5, 0.5, 0.75, 2., 8.};
	for (int ipt=0; ipt<nPt; ipt++) {
			g_cross_fwrap->SetPointEXlow(ipt, ex[ipt]);
			g_cross_fwrap->SetPointEXhigh(ipt, ex[ipt]);
			g_cross_sys_fwrap->SetPointEXlow(ipt, 0.4);
			g_cross_sys_fwrap->SetPointEXhigh(ipt, 0.4);
			g_cross_bwrap->SetPointEXlow(ipt, ex[ipt]);
			g_cross_bwrap->SetPointEXhigh(ipt, ex[ipt]);
			g_cross_sys_bwrap->SetPointEXlow(ipt, 0.4);
			g_cross_sys_bwrap->SetPointEXhigh(ipt, 0.4);
	}

	///////////////////////////////////////////////////
	///////////////////// ATLAS ////////////////////////
	///////////////////////////////////////////////////

	//cross section ds -1.94<y<0
	//vs pT
	// 414, 173, 58.2, 11.8, 1.41
	//stat 12, 4, 1.4, 0.4, 0.08
	//syst 39, 16, 4.3, 0.8, 0.10

	//cross section ds 0<y<1.94
	//408, 159, 55.5, 11.9, 1.13
	//stat 12, 4, 1.5, 0.3, 0.08
	//syst 50. 15, 5.7, 0.9, 0.07

	/// scaling nb -> mb
	Double_t scaleF = 1./1000.; 
	const int nPtATLAS = 5;
	Double_t atlas_px[nPtATLAS] = {8.75, 10.5, 12.75, 17.0, 25.0};
	//Double_t atlas_px_binW[] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 3.5};
	//Double_t atlas_ex[] = {0., 0., 0., 0., 0., 0., 0., 0.};
	Double_t atlas_ex[nPtATLAS] = {0.75, 0.5, 1.25, 3., 5.};
	Double_t atlas_py_pr_fw[nPtATLAS] = {408*scaleF, 159*scaleF, 55.5*scaleF, 11.9*scaleF, 1.13*scaleF};
	Double_t atlas_py_pr_bw[nPtATLAS] = {414*scaleF, 173*scaleF, 58.2*scaleF, 11.8*scaleF, 1.41*scaleF};
	Double_t atlas_py_np_fw[nPtATLAS] = {136*scaleF, 69.9*scaleF, 29.2*scaleF, 9.06*scaleF, 1.48*scaleF };
	Double_t atlas_py_np_bw[nPtATLAS] = { 167*scaleF, 69.1*scaleF, 32.3*scaleF, 9.28*scaleF, 1.43*scaleF};

	Double_t atlas_ey_pr_fw[nPtATLAS] = { 12*scaleF, 4*scaleF, 1.5*scaleF, 0.3*scaleF, 0.08*scaleF };
	Double_t atlas_ey_pr_bw[nPtATLAS] = { 12*scaleF, 4*scaleF, 1.4*scaleF, 0.4*scaleF, 0.08*scaleF };
	Double_t atlas_ey_np_fw[nPtATLAS] = { 8*scaleF, 2.8*scaleF, 1.3*scaleF, 0.33*scaleF, 0.9*scaleF };
	Double_t atlas_ey_np_bw[nPtATLAS] = {9*scaleF, 2.6*scaleF, 1.2*scaleF, 0.33*scaleF, 0.08*scaleF};

	Double_t atlas_exsys[nPtATLAS] = {0.3, 0.3, 0.3, 0.3, 0.3};
	Double_t atlas_eysys01_pr_fw[nPtATLAS] = {50.0*scaleF, 15.0 *scaleF, 5.7*scaleF, 0.9*scaleF, 0.07*scaleF};
//	Double_t atlas_eysys02_pr_fw[] = { 11*scaleF, 4*scaleF, 1.5*scaleF, 0.3*scaleF, 0.03 *scaleF};
	Double_t atlas_eysys01_pr_bw[nPtATLAS] = { 39*scaleF, 16*scaleF, 4.3*scaleF, 0.8*scaleF, 0.10*scaleF};
//	Double_t atlas_eysys02_pr_bw[] = { 11*scaleF, 5*scaleF, 1.6*scaleF, 0.3*scaleF, 0.04*scaleF};

	Double_t atlas_eysys01_np_fw[nPtATLAS] = { 17*scaleF, 6.6*scaleF, 3.0*scaleF, 0.7*scaleF, 0.09*scaleF};
//	Double_t atlas_eysys02_np_fw[] = { 4*scaleF, 1.9*scaleF, 0.8*scaleF, 0.24*scaleF, 0.04*scaleF};
	Double_t atlas_eysys01_np_bw[nPtATLAS] = {16*scaleF, 6.3*scaleF, 2.4*scaleF, 0.63*scaleF, 0.10 *scaleF};
//	Double_t atlas_eysys02_np_bw[] = {5*scaleF, 1.9*scaleF, 0.9*scaleF, 0.25*scaleF, 0.04 *scaleF};
	
	////////////////////
	////// Draw Plots
	////////////////////

	TLegend *legUL01 = new TLegend(0.165,0.80,0.55,0.92); //upper left
	SetLegendStyle(legUL01);
	TLegend *legBL01 = new TLegend(0.165,0.175,0.55,0.34); //upper left
	SetLegendStyle(legBL01);
	TLegend *legUC01 = new TLegend(0.30,0.67,0.58,0.83); //upper left
	SetLegendStyle(legUC01);
	TLegend *legUC02 = new TLegend(0.62,0.67,0.89,0.83); //upper left
	SetLegendStyle(legUC02);
  
	legUL01->SetTextSize(0.037);
	legBL01->SetTextSize(0.037);
	legUC01->SetTextSize(0.037);
	legUC02->SetTextSize(0.037);
  legUL01->SetTextFont(42);
  legBL01->SetTextFont(42);
  legUC01->SetTextFont(42);
  legUC02->SetTextFont(42);

	TLatex* latex = new TLatex();
	latex->SetNDC();
	if (isLog) latex->SetTextAlign(12);
	else latex->SetTextAlign(32);
	latex->SetTextSize(0.04);
	
	TBox * globalbox = new TBox(0.5, 6.4, 1.5, 13.6);
	globalbox->SetFillColor(kYellow);
	
  TLatex* globtex = new TLatex();
	globtex->SetNDC();
	//globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);

	//// graph for ATLAS /////
	TGraphAsymmErrors * g_cross_atlas_fwrap;
	TGraphAsymmErrors * g_cross_atlas_bwrap;
	TGraphAsymmErrors * g_cross_atlas_sys_fwrap;
	TGraphAsymmErrors * g_cross_atlas_sys_bwrap;

	TCanvas *c1 = new TCanvas("c1","", 200, 10, 600, 600);
	c1->cd();
	if (isLog) gPad->SetLogy(1);
	else gPad->SetLogy(0);

	TH1D *dummy1 = new TH1D("","",50,0.,400.);
	dummy1->GetXaxis()->CenterTitle();
	dummy1->GetXaxis()->SetLimits(5.0, 30.0);
//	dummy1->SetYTitle("d^{2}#sigma/dp_{T}dy x BR (J/#psi #rightarrow #mu #mu) [ #mub/(GeV/c)]");
	dummy1->SetYTitle("B x d^{2}#sigma/dp_{T}dy [ #mub/(GeV/c)]");
	dummy1->SetXTitle("p_{T} [GeV/c]");

 	if (isLog){
    if (isPrompt) {
  	  dummy1->SetMinimum(0.0002);
  	  dummy1->SetMaximum(20);
 	  } else {
  	  dummy1->SetMinimum(0.0004);
  	  dummy1->SetMaximum(2.0);
    }
 	}
 	else {
    if (isPrompt) {
	    dummy1->SetMinimum(-0.05);
	    dummy1->SetMaximum(1.5);
 	  } else {
  	  dummy1->SetMinimum(-0.01);
  	  dummy1->SetMaximum(0.4);
    }
	}
	dummy1->SetLineColor(kWhite); //KYO
	dummy1->Draw(); //KYO

	if (!isLog) dashedLine(5.0,0.0,30.0,0.0,1,1);

	if (isPrompt) g_cross_atlas_sys_fwrap = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_pr_fw, atlas_exsys, atlas_exsys, atlas_eysys01_pr_fw, atlas_eysys01_pr_fw);	
	else g_cross_atlas_sys_fwrap = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_np_fw, atlas_exsys, atlas_exsys, atlas_eysys01_np_fw, atlas_eysys01_np_fw);	
	g_cross_atlas_sys_fwrap->SetFillColor(kGray+1);
	//g_cross_atlas_sys_fwrap->SetFillStyle(3001);
	
	if (isPrompt) g_cross_atlas_fwrap = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_pr_fw, atlas_ex, atlas_ex, atlas_ey_pr_fw, atlas_ey_pr_fw);	
	else g_cross_atlas_fwrap = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_np_fw, atlas_ex, atlas_ex, atlas_ey_np_fw, atlas_ey_np_fw);	
	SetGraphStyleFinal(g_cross_atlas_fwrap,9,10);
	TF1 *expoFunc_fw = new TF1("expoFunc_fw","[0]*exp(-[1]*x)", 8, 30);
	expoFunc_fw->SetParameter(0,10);
	expoFunc_fw->SetParameter(1,1.5);
	expoFunc_fw->SetLineColor(6);
	g_cross_atlas_fwrap->Fit("expoFunc_fw","R LL M N");

	double atlas_px_fwShift[nAtlasBin];
	getMeanPtBins( atlas_px_fwShift, expoFunc_fw);
	TGraphAsymmErrors* g_cross_atlas_fwrap_shift;
  if (isPrompt) g_cross_atlas_fwrap_shift =  new TGraphAsymmErrors(nPtATLAS, atlas_px_fwShift, atlas_py_pr_fw, atlas_ex, atlas_ex, atlas_ey_pr_fw, atlas_ey_pr_fw);
  else g_cross_atlas_fwrap_shift =  new TGraphAsymmErrors(nPtATLAS, atlas_px_fwShift, atlas_py_np_fw, atlas_ex, atlas_ex, atlas_ey_np_fw, atlas_ey_np_fw);
   SetGraphStyleFinal(g_cross_atlas_fwrap_shift,9,10);
	
	TGraphAsymmErrors* g_cross_atlas_sys_fwrap_shift;
  if (isPrompt) g_cross_atlas_sys_fwrap_shift = new TGraphAsymmErrors(nPtATLAS, atlas_px_fwShift, atlas_py_pr_fw, atlas_exsys, atlas_exsys, atlas_eysys01_pr_fw, atlas_eysys01_pr_fw);
  else g_cross_atlas_sys_fwrap_shift = new TGraphAsymmErrors(nPtATLAS, atlas_px_fwShift, atlas_py_pr_fw, atlas_exsys, atlas_exsys, atlas_eysys01_pr_fw, atlas_eysys01_pr_fw);
	g_cross_atlas_sys_fwrap_shift->SetFillColor(kGray+1);
	
	if ( shifted)  { 
	  g_cross_atlas_sys_fwrap_shift->Draw("2");
	  g_cross_atlas_fwrap_shift->Draw("p");
	}
	else {
	  g_cross_atlas_sys_fwrap->Draw("2");
	  g_cross_atlas_fwrap->Draw("P");
	}

	if (isPrompt) g_cross_atlas_sys_bwrap = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_pr_bw, atlas_exsys, atlas_exsys, atlas_eysys01_pr_bw, atlas_eysys01_pr_bw);	
	else g_cross_atlas_sys_bwrap = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_np_bw, atlas_exsys, atlas_exsys, atlas_eysys01_np_bw, atlas_eysys01_np_bw);	
	g_cross_atlas_sys_bwrap->SetFillColor(kGray);

	SetGraphStyleFinal(g_cross_fwrap,1,3);

	g_cross_fwrap->Draw("P");

	if (isPrompt) g_cross_atlas_bwrap = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_pr_bw, atlas_ex, atlas_ex, atlas_ey_pr_bw, atlas_ey_pr_bw);	
	else g_cross_atlas_bwrap = new TGraphAsymmErrors(nPtATLAS, atlas_px, atlas_py_np_bw, atlas_ex, atlas_ex, atlas_ey_np_bw, atlas_ey_np_bw);	
	SetGraphStyleFinal(g_cross_atlas_bwrap,9,11);
	TF1 *expoFunc_bw = new TF1("expoFunc_bw","[0]*exp(-[1]*x)", 8, 30);
	expoFunc_bw->SetParameter(0,10000);
	expoFunc_bw->SetParameter(1,1.5);
	expoFunc_bw->SetLineColor(6);
	expoFunc_bw->SetLineStyle(2);
	g_cross_atlas_bwrap->Fit("expoFunc_bw","R LL M N");

	double atlas_px_bwShift[nAtlasBin];
        getMeanPtBins( atlas_px_bwShift, expoFunc_bw);
        TGraphAsymmErrors* g_cross_atlas_bwrap_shift;
        if (isPrompt) g_cross_atlas_bwrap_shift  =  new TGraphAsymmErrors(nPtATLAS, atlas_px_bwShift, atlas_py_pr_bw, atlas_ex, atlas_ex, atlas_ey_pr_bw, atlas_ey_pr_bw);
        else g_cross_atlas_bwrap_shift  =  new TGraphAsymmErrors(nPtATLAS, atlas_px_bwShift, atlas_py_np_bw, atlas_ex, atlas_ex, atlas_ey_np_bw, atlas_ey_np_bw);
				SetGraphStyleFinal(g_cross_atlas_bwrap_shift,9,11);

        TGraphAsymmErrors* g_cross_atlas_sys_bwrap_shift;
        if (isPrompt) g_cross_atlas_sys_bwrap_shift = new TGraphAsymmErrors(nPtATLAS, atlas_px_bwShift, atlas_py_pr_bw, atlas_exsys, atlas_exsys, atlas_eysys01_pr_bw, atlas_eysys01_pr_bw);
        else g_cross_atlas_sys_bwrap_shift = new TGraphAsymmErrors(nPtATLAS, atlas_px_bwShift, atlas_py_np_bw, atlas_exsys, atlas_exsys, atlas_eysys01_np_bw, atlas_eysys01_np_bw);
        g_cross_atlas_sys_bwrap_shift->SetFillColor(kGray);
       
	if ( shifted)  { 
	  g_cross_atlas_sys_bwrap_shift->Draw("2");
	  g_cross_atlas_bwrap_shift->Draw("p");
	}
	else {
	  g_cross_atlas_sys_bwrap->Draw("2");
	  g_cross_atlas_bwrap->Draw("P");
	}
	
	g_cross_sys_fwrap->SetFillColor(kRed-9);
	g_cross_sys_fwrap->Draw("2");
	g_cross_sys_bwrap->SetFillColor(kRed-10);
	g_cross_sys_bwrap->Draw("2");

	SetGraphStyleFinal(g_cross_fwrap,1,3);
	SetGraphStyleFinal(g_cross_bwrap,1,13);
	g_cross_fwrap->Draw("P");
	g_cross_bwrap->Draw("P");

	//// re-draw
	if ( shifted)  { 
	 g_cross_atlas_fwrap_shift->Draw("p");
	 g_cross_atlas_bwrap_shift->Draw("p");
	}
	else {
	 g_cross_atlas_fwrap->Draw("p");
	 g_cross_atlas_bwrap->Draw("p");
	}
	g_cross_fwrap->Draw("P");
	g_cross_bwrap->Draw("P");

	if (isLog){
//	  legUL01 -> AddEntry(g_cross_fwrap,"CMS Preliminary"," ");
	  legUL01 -> AddEntry(g_cross_fwrap,"0 < y_{CM} < 1.93","lp");
	  legUL01 -> AddEntry(g_cross_bwrap,"-1.93 < y_{CM} < 0","lp");
	  legUL01-> Draw();
	  
		legBL01 -> SetHeader("ATLAS");
	  legBL01 -> AddEntry(g_cross_atlas_fwrap,"0 < y_{CM} < 1.94","lp");
	  legBL01 -> AddEntry(g_cross_atlas_bwrap,"-1.94 < y_{CM} < 0","lp");
	  legBL01-> Draw();
	}

       else{
	legUC01 -> AddEntry(g_cross_fwrap,"CMS"," ");
	legUC01 -> AddEntry(g_cross_fwrap,"0 < y_{CM} < 1.93","lp");
 	legUC01 -> AddEntry(g_cross_bwrap,"-1.93 < y_{CM} < 0","lp");
 	legUC01-> Draw();
	legUC02 -> AddEntry(g_cross_fwrap,"ATLAS"," ");
	legUC02 -> AddEntry(g_cross_atlas_fwrap,"0 < y_{CM} < 1.94","lp");
	legUC02 -> AddEntry(g_cross_atlas_bwrap,"-1.94 < y_{CM} < 0","lp");
 	legUC02-> Draw();
       }
	
	globtex->SetTextSize(0.045);
	globtex->SetTextFont(22);
	if (isPrompt) globtex->DrawLatex(0.88, 0.88, "Prompt J/#psi");
	else globtex->DrawLatex(0.88, 0.88, "Non-prompt J/#psi");
	
//	if (isLog) {
//		latex->DrawLatex(0.17, 0.28, Form("%s",beamstring.c_str()));
//	}
//	else {
//		latex->DrawLatex(0.89, 0.28, Form("%s",beamstring.c_str()));
//	}	

	CMS_lumi( c1, isPA, iPos );
	c1->Update();
	//c1->SaveAs(Form("compOtherExp/comp_crossSection_pt_ATLAS_pr_isLog%d_%d_shifted_%d_middle_xErr.pdf",(int)isLog,(int)isPtCut,(int)shifted));
	//c1->SaveAs(Form("compOtherExp/comp_crossSection_pt_ATLAS_pr_isLog%d_%d_shifted_%d_middle_xErr.png",(int)isLog,(int)isPtCut,(int)shifted));
	//c1->Clear();
	legUL01->Clear();
	legBL01->Clear();
	legUC01->Clear();
	legUC02->Clear();

	return;
}

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
		*arr = Form("%.0f < p_{T} < %.0f [GeV/c]", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < p_{T} < %.0f [GeV/c]", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < p_{T} < %.1f [GeV/c]", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f [GeV/c]", binmin, binmax);
	}
}



void getMeanPtBins( Double_t * binArr, TF1* func) {
  Double_t atlas_Bin[nAtlasBin+1] = {8, 9.5, 11.5, 14, 20, 30};
  
  TH1D* firhist = (TH1D*)func->GetHistogram();
  for ( int i=1 ; i<= nAtlasBin ; i++) {
    float lowPtCut = atlas_Bin[i-1];
    float highPtCut = atlas_Bin[i];
    
    TH1D* tempHist = (TH1D*)firhist->Clone("tempHist");
    for ( int xbin = 1 ;xbin<=tempHist->GetNbinsX() ; xbin++) {
      if ( (tempHist->GetBinCenter(xbin) > highPtCut) || (tempHist->GetBinCenter(xbin) < lowPtCut) \
	   )
      tempHist->SetBinContent(xbin,0);
    }
    float meanPt = tempHist->GetMean();
    cout << " meanPt = " << meanPt << endl;
    binArr[i-1] = meanPt;
    delete tempHist;
    
  }
  
}

void CMS_lumi( TPad* pad, int iPeriod, int iPosX )
{            
  bool outOfFrame    = false;
  if( iPosX/10==0 ) { outOfFrame = true; }
  int alignY_=3;
  int alignX_=2;
  if( iPosX/10==0 ) alignX_=1;
  if( iPosX==0    ) alignX_=1;
  if( iPosX==0    ) alignY_=1;
  if( iPosX/10==1 ) alignX_=1;
  if( iPosX/10==2 ) alignX_=2;
  if( iPosX/10==3 ) alignX_=3;
  //if( iPosX == 0  ) relPosX = 0.12;
  if( iPosX == 0  ) relPosX = 0.15; // KYO
  int align_ = 10*alignX_ + alignY_;

  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  //  float e = 0.025;

  pad->cd();

  TString lumiText;
  if( iPeriod==0 ) {
    lumiText += lumi_pp502TeV;
    lumiText += " (pp 5.02 TeV)";
  } else if( iPeriod==1 || iPeriod==2 || iPeriod==3){
    lumiText += lumi_pPb502TeV;
    lumiText += " (pPb 5.02 TeV)";
  } else {
    lumiText += "LumiText Not Selected";
  }
   
  cout << lumiText << endl;

  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);    

  //float extraTextSize = extraOverCmsTextSize*cmsTextSize;
  float extraTextSize = extraOverCmsTextSize*cmsTextSize*1.1;
  
  latex.SetTextFont(42);
  latex.SetTextAlign(31); 
  latex.SetTextSize(lumiTextSize*t);    
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);
  if( outOfFrame ) {
    latex.SetTextFont(cmsTextFont);
    latex.SetTextAlign(11); 
    latex.SetTextSize(cmsTextSize*t);    
    latex.DrawLatex(l,1-t+lumiTextOffset*t,cmsText);
  }
  
  pad->cd();

  float posX_=0;
  if( iPosX%10<=1 ) { posX_ =   l + relPosX*(1-l-r); }
  else if( iPosX%10==2 ) { posX_ =  l + 0.5*(1-l-r); }
  else if( iPosX%10==3 ) { posX_ =  1-r - relPosX*(1-l-r); }
  float posY_ = 1-t - relPosY*(1-t-b);
  if( !outOfFrame ) {
    if( drawLogo ) {
	    posX_ =   l + 0.045*(1-l-r)*W/H;
	    posY_ = 1-t - 0.045*(1-t-b);
	    float xl_0 = posX_;
	    float yl_0 = posY_ - 0.15;
	    float xl_1 = posX_ + 0.15*H/W;
	    float yl_1 = posY_;
	    //TASImage* CMS_logo = new TASImage("CMS-BW-label.png");
	    TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
	    pad_logo->Draw();
	    pad_logo->cd();
	    //CMS_logo->Draw("X");
	    pad_logo->Modified();
	    pad->cd();
	  } else {
	    latex.SetTextFont(cmsTextFont);
	    latex.SetTextSize(cmsTextSize*t);
	    latex.SetTextAlign(align_);
	    latex.DrawLatex(posX_, posY_, cmsText);
	    if( writeExtraText ) {
	      latex.SetTextFont(extraTextFont);
	      latex.SetTextAlign(align_);
	      latex.SetTextSize(extraTextSize*t);
	      latex.DrawLatex(posX_, posY_- relExtraDY*cmsTextSize*t, extraText);
	    }
	  }
  } else if( writeExtraText ) {
    if( iPosX==0) {
  	  posX_ =   l +  relPosX*(1-l-r);
  	  posY_ =   1-t+lumiTextOffset*t;
  	}
    latex.SetTextFont(extraTextFont);
    latex.SetTextSize(extraTextSize*t);
    latex.SetTextAlign(align_);
    latex.DrawLatex(posX_, posY_, extraText);      
  }
  return;
}

