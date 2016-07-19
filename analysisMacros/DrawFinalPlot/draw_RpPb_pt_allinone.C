#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void draw_RpPb_pt(bool sysByHand=true, bool noPtWeight=true, bool isPrompt=true)
{
  gROOT->Macro("./tdrstyle_kyo.C");
  //cmsTextFont   = 42;  // for b.hong
  //extraTextFont = 42;  // for b.hong
  int isPA = 10;  // 0:pp, 1:pPb, 10 : pp & pPb together for RpPb plot
	int iPos=33;//right corner
//	int iPos=11;//left corner
//	int iPos=0.;//outOfFrame
  
  //// BR and lumi info.
  const Double_t br = 0.0593 ;
  const Double_t brErr = 0.0006;
  const Double_t pp_lumi_pb =26.3; // 26.3/pb
  const Double_t pp_lumi_pb_err = 2.63; // tmp
  const Double_t pPb_lumi_nb = 34.622; // 34.6/nb
  const Double_t pPb_lumi_nb_err = 1.2;
  const Double_t pp_lumi_mub = pp_lumi_pb *1000*1000;
  const Double_t pPb_lumi_mub_err = pp_lumi_pb_err *1000*1000;
 
  double A_pb =208;

  /////////////////////////////////////////////////////////////////////////
  // bin center & systematic uncertainties by hand
  // for 1.5 - 1.93 
  const int nRap = 8; 
  const int nPt = 9;
  const int nRapTmp = nRap + 1;
  const int nPtTmp = nPt + 1;
  const int nRapRpPb = 7;  

  Double_t dummy1, dummy2, dummy3, dummy4;
    
  Double_t pxtmp[nRap][nPt]; //x point to fill remporarily
  Double_t pytmp[nRap][nPt]; //y point to fill remporarily
  Double_t eytmp[nRap][nPt]; //y point error to fill remporarily
  Double_t eysys[nRap][nPt]; //absolute y sys error
  
  Double_t pxtmp_pA[nRap][nPt]; //x point to fill remporarily
  Double_t pytmp_pA[nRap][nPt]; //y point to fill remporarily
  Double_t eytmp_pA[nRap][nPt]; //y point error to fill remporarily
  Double_t eysys_pA[nRap][nPt]; //absolute y sys error
  
  Double_t pxtmp_pp[nRap][nPt]; //x point to fill remporarily
  Double_t pytmp_pp[nRap][nPt]; //y point to fill remporarily
  Double_t eytmp_pp[nRap][nPt]; //y point error to fill remporarily
  Double_t eysys_pp[nRap][nPt]; //absolute y sys error
  
  Double_t px_pA[nRap][nPt] = { //x point (mean pT)
    {2.49530, 3.48529, 4.47275, 5.68792, 6.9644, 7.96271, 9.1754, 11.5315, 17.7588}, 
    {0.00000, 3.53123, 4.5027, 5.71709, 6.96523, 7.9693, 9.17314, 11.4952, 17.6927}, 
    {0.00000, 0.00000, 0.00000, 0.00000, 7.01977, 7.99712, 9.19936, 11.5743, 17.7732}, 
    {0.00000, 0.00000, 0.00000, 0.00000, 7.12292, 8.01305, 9.22816, 11.6279, 17.8879}, 
    {0.00000, 0.00000, 0.00000, 0.00000, 7.05476, 8.00208, 9.21589, 11.5645, 17.7176}, 
    {0.00000, 0.00000, 0.00000, 5.82095, 6.97886, 7.96732, 9.18979, 11.5158, 17.4116}, 
    {0.00000, 3.52893, 4.48328, 5.69351, 6.96188, 7.95707, 9.14886, 11.4747, 17.231}, 
    {2.49481, 3.47853, 4.46938, 5.6761, 6.96419, 7.97702, 9.16158, 11.5077, 17.3061} 
  };
	Double_t ex[nPt] = {0,0,0,0,0,0,0,0,0}; // x stat error
  Double_t exsys[nPt] = {0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4}; // x sys error
  
  //// 1) y_CM array (from forward to backward)
  Double_t rapArrNumFB[nRapTmp] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
  //Double_t rapArrNumBF[nRapTmp] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
  Double_t rapBinW[nRap];
  for (Int_t iy=0; iy<nRap; iy++) {
    rapBinW[iy] = rapArrNumFB[iy]-rapArrNumFB[iy+1];
    //cout << iy <<"th rapBinW = " << rapBinW[iy] <<endl;
  }
  //// 2) pt array
  Double_t ptArrNum[nPtTmp] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
  Double_t ptBinW[nPt];
  for (Int_t ipt=0; ipt<nPt; ipt++) {
    ptBinW[ipt] = ptArrNum[ipt+1]-ptArrNum[ipt]; 
    //cout << ipt <<"th ptBinW = " << ptBinW[ipt] <<endl;
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

  //// read our result
  TFile *inFile_pA, *inFile_pp;
  if (noPtWeight) {
    if (isPrompt) {
      inFile_pA= new TFile("plot_cross/pA_cross_pt_isPrompt1_isLog0_isScale0_noPtWeight.root");
      inFile_pp= new TFile("plot_cross/pp_cross_pt_isPrompt1_isLog0_isScale0_noPtWeight.root");
    } else {
      inFile_pA= new TFile("plot_cross/pA_cross_pt_isPrompt0_isLog0_isScale0_noPtWeight.root");
      inFile_pp= new TFile("plot_cross/pp_cross_pt_isPrompt0_isLog0_isScale0_noPtWeight.root");
    }
  } else {
    if (isPrompt) {
      inFile_pA= new TFile("plot_cross/pA_cross_pt_isPrompt1_isLog0_isScale0.root");
      inFile_pp= new TFile("plot_cross/pp_cross_pt_isPrompt1_isLog0_isScale0.root");
    } else {
      inFile_pA= new TFile("plot_cross/pA_cross_pt_isPrompt0_isLog0_isScale0.root");
      inFile_pp= new TFile("plot_cross/pp_cross_pt_isPrompt0_isLog0_isScale0.root");
    }
  }
  TGraphAsymmErrors* g_cross_sys_pA[nRap];
  TGraphAsymmErrors* g_cross_pA[nRap];
  TGraphAsymmErrors* g_cross_sys_pp[nRap];
  TGraphAsymmErrors* g_cross_pp[nRap];
  for (int iy =0; iy <  nRap; iy++ ) {
    g_cross_sys_pA[iy] = (TGraphAsymmErrors*)inFile_pA->Get(Form("g_cross_sys_%d",iy));   
    g_cross_pA[iy] = (TGraphAsymmErrors*)inFile_pA->Get(Form("g_cross_%d",iy));   
    g_cross_sys_pp[iy] = (TGraphAsymmErrors*)inFile_pp->Get(Form("g_cross_sys_%d",iy));   
    g_cross_pp[iy] = (TGraphAsymmErrors*)inFile_pp->Get(Form("g_cross_%d",iy));   
  }
  //cout << "GetN() : " <<g_cross_sys_pA[0]->GetN() << endl;
  
  //////////////////////////////////////////////////////////////////
  //// Calcaulate RpPb

  //// read out py, ey values
  for (Int_t iy = 0; iy < nRap; iy++) { 
    for (Int_t ipt = 0; ipt < nPt; ipt++) { 
      g_cross_pA[iy]->GetPoint(ipt, pxtmp_pA[iy][ipt], pytmp_pA[iy][ipt]);
      eytmp_pA[iy][ipt] = g_cross_pA[iy]-> GetErrorY(ipt);
      eysys_pA[iy][ipt] = g_cross_sys_pA[iy]-> GetErrorY(ipt);
      g_cross_pp[iy]->GetPoint(ipt, pxtmp_pp[iy][ipt], pytmp_pp[iy][ipt]);
      eytmp_pp[iy][ipt] = g_cross_pp[iy]-> GetErrorY(ipt);
      eysys_pp[iy][ipt] = g_cross_sys_pp[iy]-> GetErrorY(ipt);
    }
  } 
  //// NOTE :: Divide different rapidity ranges in pPb and pp!! (due to y shift)  
  for (Int_t iy = 0; iy < nRapRpPb; iy++) { 
    for (Int_t ipt = 0; ipt < nPt; ipt++) { 
      DivideValue(pytmp_pA[iy][ipt], eytmp_pA[iy][ipt], pytmp_pp[iy+1][ipt], eytmp_pp[iy+1][ipt], &pytmp[iy][ipt], &eytmp[iy][ipt]); //actual values
      DivideValue(pytmp_pA[iy][ipt], eysys_pA[iy][ipt], pytmp_pp[iy+1][ipt], eysys_pp[iy+1][ipt], &dummy1, &eysys[iy][ipt]); // syst.
      //// nomarlize by A_pb = 208.
      pytmp[iy][ipt] = pytmp[iy][ipt]/A_pb;
      eytmp[iy][ipt] = eytmp[iy][ipt]/A_pb;
      eysys[iy][ipt] = eysys[iy][ipt]/A_pb;
      cout << "R_pPb["<<iy<<"]["<<ipt<<"] = "<< pytmp[iy][ipt] << endl;; 
      cout << "stat.["<<iy<<"]["<<ipt<<"] = " << eytmp[iy][ipt]<<endl;
      cout << "sys.["<<iy<<"]["<<ipt<<"] = " << eysys[iy][ipt]<<endl;
    }
  } 
 
  TGraphAsymmErrors* g_RpPb_sys[nRapRpPb];
  TGraphAsymmErrors* g_RpPb[nRapRpPb];
   
  for (Int_t iy = 0; iy < nRapRpPb; iy++) { 
  //for (Int_t iy = 0; iy < 1; iy++) { 
    g_RpPb_sys[iy] = new TGraphAsymmErrors(nPt, px_pA[iy], pytmp[iy], exsys, exsys, eysys[iy], eysys[iy]);
    g_RpPb[iy] = new TGraphAsymmErrors(nPt, px_pA[iy], pytmp[iy], ex, ex, eytmp[iy], eytmp[iy]);
    g_RpPb_sys[iy]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    g_RpPb_sys[iy]->GetXaxis()->CenterTitle();
    g_RpPb_sys[iy]->GetYaxis()->SetTitle("R_{pPb}");
    g_RpPb_sys[iy]->GetYaxis()->CenterTitle();
    g_RpPb_sys[iy]->GetXaxis()->SetLimits(0.,20.0);
    g_RpPb_sys[iy]->SetMinimum(0.5);
    //g_RpPb_sys[iy]->SetMaximum(1.15);
    g_RpPb_sys[iy]->SetMaximum(1.5);
  } 
  g_RpPb_sys[0]->SetFillColor(kTeal-9);
  g_RpPb_sys[1]->SetFillColor(kRed-9);
  g_RpPb_sys[2]->SetFillColor(kAzure-9);
  g_RpPb_sys[3]->SetFillColor(kAzure-9);
  g_RpPb_sys[4]->SetFillColor(kRed-9);
  g_RpPb_sys[5]->SetFillColor(kTeal-9);
  g_RpPb_sys[6]->SetFillColor(kViolet-9);
  SetGraphStyleFinal(g_RpPb[0],  0,5);
  g_RpPb[0]->SetMarkerSize(2.5);
  SetGraphStyleFinal(g_RpPb[1],  1,3);
  g_RpPb[1]->SetMarkerSize(1.7);
  SetGraphStyleFinal(g_RpPb[2],  2,0);
  g_RpPb[2]->SetMarkerSize(1.7);
  SetGraphStyleFinal(g_RpPb[3],  2,0);
  g_RpPb[3]->SetMarkerSize(1.7);
  SetGraphStyleFinal(g_RpPb[4],  1,3);
  g_RpPb[4]->SetMarkerSize(1.7);
  SetGraphStyleFinal(g_RpPb[5],  0,5);
  g_RpPb[5]->SetMarkerSize(2.5);
  SetGraphStyleFinal(g_RpPb[6],  8,2);
  g_RpPb[6]->SetMarkerSize(2.1);
  
  //////////////////////////////////////////////////////////////////
  //// draw

	TLegend *legBRFW = new TLegend(0.50, 0.17, 0.70, 0.38);
	SetLegendStyle(legBRFW);
	legBRFW->SetTextSize(0.05);
	TLegend *legBRBW = new TLegend(0.50, 0.17, 0.70, 0.45);
	SetLegendStyle(legBRBW);
	legBRBW->SetTextSize(0.05);
	 	
	TLatex* globtex = new TLatex();
	globtex->SetNDC();
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
	globtex->SetTextSize(0.04);

 
  //// forward 
  TCanvas* c_fw = new TCanvas("c_fw","c_fw",600,600);
  c_fw->cd();
  
  g_RpPb_sys[0]->Draw("A2");
  g_RpPb_sys[1]->Draw("2");
  g_RpPb_sys[2]->Draw("2");
  g_RpPb[0]->Draw("P");
  g_RpPb[1]->Draw("P");
  g_RpPb[2]->Draw("P");
  dashedLine(0.,1.,20.,1.,1,1);
  
	TLegendEntry *leFW1=legBRFW->AddEntry("leFW1",Form("  %s", rapArr[0].Data()),"lpf");
	leFW1->SetFillColor(kTeal-9);
	leFW1->SetFillStyle(1001);
	leFW1->SetLineColor(kGreen+3);
	leFW1->SetMarkerStyle(kFullDiamond);
	leFW1->SetMarkerColor(kGreen+3);
	leFW1->SetMarkerSize(3.3);
	TLegendEntry *leFW2=legBRFW->AddEntry("leFW2",Form("  %s", rapArr[1].Data()),"lpf");
	leFW2->SetFillColor(kRed-10);
	leFW2->SetFillStyle(1001);
	leFW2->SetLineColor(kPink-6);
	leFW2->SetMarkerStyle(kFullSquare);
	leFW2->SetMarkerColor(kPink-6);
	leFW2->SetMarkerSize(2.1);
	TLegendEntry *leFW3=legBRFW->AddEntry("leFW3",Form("  %s", rapArr[2].Data()),"lpf");
	leFW3->SetFillColor(kBlue-10);
	leFW3->SetFillStyle(1001);
	leFW3->SetLineColor(kBlue-3);
	leFW3->SetMarkerStyle(kFullCircle);
	leFW3->SetMarkerColor(kBlue-3);
	leFW3->SetMarkerSize(2.1);

	legBRFW->Draw();
	
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");
	
  CMS_lumi( c_fw, isPA, iPos );

  //// backward
  TCanvas* c_bw = new TCanvas("c_bw","c_bw",600,600);
  c_bw->cd();
  
  g_RpPb_sys[3]->Draw("A2");
  g_RpPb_sys[4]->Draw("2");
  g_RpPb_sys[5]->Draw("2");
  g_RpPb_sys[6]->Draw("2");
  g_RpPb[3]->Draw("P");
  g_RpPb[4]->Draw("P");
  g_RpPb[5]->Draw("P");
  g_RpPb[6]->Draw("P");
  dashedLine(0.,1.,20.,1.,1,1);
	
	TLegendEntry *leBW1=legBRBW->AddEntry("leBW1",Form("  %s", rapArr[3].Data()),"lpf");
	leBW1->SetFillColor(kBlue-10);
	leBW1->SetFillStyle(1001);
	leBW1->SetLineColor(kBlue-3);
	leBW1->SetMarkerStyle(kFullCircle);
	leBW1->SetMarkerColor(kBlue-3);
	leBW1->SetMarkerSize(2.1);
	TLegendEntry *leBW2=legBRBW->AddEntry("leBW2",Form("  %s", rapArr[4].Data()),"lpf");
	leBW2->SetFillColor(kRed-10);
	leBW2->SetFillStyle(1001);
	leBW2->SetLineColor(kPink-6);
	leBW2->SetMarkerStyle(kFullSquare);
	leBW2->SetMarkerColor(kPink-6);
	leBW2->SetMarkerSize(2.1);
	TLegendEntry *leBW3=legBRBW->AddEntry("leBW3",Form("  %s", rapArr[5].Data()),"lpf");
	leBW3->SetFillColor(kTeal-9);
	leBW3->SetFillStyle(1001);
	leBW3->SetLineColor(kGreen+3);
	leBW3->SetMarkerStyle(kFullDiamond);
	leBW3->SetMarkerColor(kGreen+3);
	leBW3->SetMarkerSize(3.3);
	TLegendEntry *leBW4=legBRBW->AddEntry("leBW4",Form("  %s", rapArr[6].Data()),"lpf");
	leBW4->SetFillColor(kViolet-9);
	leBW4->SetFillStyle(1001);
	leBW4->SetLineColor(kViolet-6);
	leBW4->SetMarkerStyle(kFullCircle);
	leBW4->SetMarkerColor(kViolet-6);
	leBW4->SetMarkerSize(2.1);

	legBRBW->Draw();
	
  globtex->SetTextSize(0.055);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
	else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");
  
  CMS_lumi( c_bw, isPA, iPos );

  if (noPtWeight) { 
    c_fw->SaveAs(Form("plot_RpPb/fw_RpPb_pt_isPrompt%d_noPtWeight.pdf",(int)isPrompt));
    c_fw->SaveAs(Form("plot_RpPb/fw_RpPb_pt_isPrompt%d_noPtWeight.png",(int)isPrompt));
    c_bw->SaveAs(Form("plot_RpPb/bw_RpPb_pt_isPrompt%d_noPtWeight.pdf",(int)isPrompt));
    c_bw->SaveAs(Form("plot_RpPb/bw_RpPb_pt_isPrompt%d_noPtWeight.png",(int)isPrompt));
  } else {
    c_fw->SaveAs(Form("plot_RpPb/fw_RpPb_pt_isPrompt%d.pdf",(int)isPrompt));
    c_fw->SaveAs(Form("plot_RpPb/fw_RpPb_pt_isPrompt%d.png",(int)isPrompt));
    c_bw->SaveAs(Form("plot_RpPb/bw_RpPb_pt_isPrompt%d.pdf",(int)isPrompt));
    c_bw->SaveAs(Form("plot_RpPb/bw_RpPb_pt_isPrompt%d.png",(int)isPrompt));
  }
  
  ///////////////////////////////////////////////////////////////////
  //// save as a root file
  TFile *outFile;
  if (noPtWeight) outFile = new TFile(Form("plot_RpPb/RpPb_pt_isPrompt%d_noPtWeight.root",(int)isPrompt),"RECREATE");
  else outFile = new TFile(Form("plot_RpPb/RpPb_pt_isPrompt%d.root",(int)isPrompt),"RECREATE");
  outFile->cd();
  for (int iy=0; iy<nRapRpPb; iy++){
    g_RpPb_sys[iy]->Write();  
    g_RpPb[iy]->Write();  
  }
  outFile->Close();
  
  return;
} // end of main func.

void formRapArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( binmin == 1.93 || binmin == -1.93 || binmin == -2.87) {
		*arr = Form("%.2f < y_{CM} < %.1f", binmin, binmax);
  }
  else if ( binmax == 1.93 || binmax ==  -1.93 || binmax == -2.87 ) {
		*arr = Form("%.1f < y_{CM} < %.2f", binmin, binmax);
  }
  else if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < y_{CM} < %.0f", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.1f < y_{CM} < %.0f", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < y_{CM} < %.1f", binmin, binmax);
  } else {
    *arr = Form("%.1f < y_{CM} < %.1f", binmin, binmax);
  }
}

void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( binmin == 1.93 || binmin == -1.93 || binmin == -2.87) {
    *arr = Form("%.2f < |y_{CM}| < %.1f", binmin, binmax);
  }
  else if ( binmax == 1.93 || binmax ==  -1.93 || binmax == -2.87 ) {
    *arr = Form("%.1f < |y_{CM}| < %.2f", binmin, binmax);
  }
  else if ( fracMin == 0 && fracMax == 0 ) {
    *arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
  } else if ( fracMin != 0 && fracMax == 0 ) {
    *arr = Form("%.1f < |y_{CM}| < %.0f", binmin, binmax);
  } else if ( fracMin == 0 && fracMax != 0 ) {
    *arr = Form("%.0f < |y_{CM}| < %.1f", binmin, binmax);
  } else {
    *arr = Form("%.1f < |y_{CM}| < %.1f", binmin, binmax);
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
  } else if( iPeriod==10){
    lumiText += "pPb ";
    lumiText += lumi_pPb502TeV;
    lumiText += ", pp ";
    lumiText += lumi_pp502TeV;
    lumiText += " (5.02 TeV)";
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
      //cout << "posX_ = " << posX_ << ", posY_ = " << posY_ << endl;
      if (iPosX==33) {posX_ -= 0.03; posY_-=0.03; } // KYO
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
