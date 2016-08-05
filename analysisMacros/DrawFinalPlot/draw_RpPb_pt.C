#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void draw_RpPb_pt(bool sysByHand=false, bool noPtWeight=false, bool isPrompt=true)
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
	const Double_t pp_lumi_pb =28.538; // 28.5/pb
	const Double_t pp_lumi_pb_err = 1.1; // 4 %
	const Double_t pPb_lumi_nb = 34.622; // 34.6/nb
	const Double_t pPb_lumi_nb_err = 1.2; // 3.5 %
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
    {2.525, 3.51255, 4.4772, 5.70327, 6.96635, 7.96061, 9.17243, 11.5938, 18.0681},
    {0, 0, 4.52793, 5.74033, 6.97622, 7.98335, 9.19458, 11.4927, 17.6693},
    {0, 0, 0, 0, 7.018, 8.00224, 9.19714, 11.5483, 17.6577},
    {0, 0, 0, 0, 7.11111, 8.02103, 9.24485, 11.6204, 17.8454},
    {0, 0, 0, 0, 7.05329, 8.00998, 9.20583, 11.5222, 17.4633},
    {0, 0, 0, 5.84477, 6.98466, 7.97917, 9.17551, 11.5322, 17.34},
    {0, 0, 4.52204, 5.72881, 6.97074, 7.95284, 9.14157, 11.4976, 17.3058},
    {2.51699, 3.4959, 4.47636, 5.68624, 6.97338, 7.97824, 9.1805, 11.4841, 16.8762}
  };
	Double_t ex[nPt] = {0,0,0,0,0,0,0,0,0}; // x stat error
	Double_t exlow[nRap][nPt];
	Double_t exhigh[nRap][nPt];
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
  
  //// ex calculation
  for (Int_t iy=0; iy<nRap; iy++) {
    for (Int_t ipt=0; ipt<nPt; ipt++) {
      exlow[iy][ipt] = px_pA[iy][ipt]-ptArrNum[ipt]; 
      exhigh[iy][ipt] = ptArrNum[ipt+1]-px_pA[iy][ipt]; 
      cout << iy<<"th y, "<<ipt<<"th pt"<<endl;
      cout << "exlow = "<< exlow[iy][ipt] << endl;
      cout << "exhigh = " <<exhigh[iy][ipt] << endl;
    }
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
      cout << "" << endl;
      cout << "R_pPb["<<iy<<"]["<<ipt<<"] = "<< pytmp[iy][ipt] << endl;; 
      cout << "stat.["<<iy<<"]["<<ipt<<"] = " << eytmp[iy][ipt]<<endl;
      cout << "sys.["<<iy<<"]["<<ipt<<"] = " << eysys[iy][ipt]<<endl;
    }
  } 

	//// set values as zero for unused bins
	for (Int_t iy = 0; iy < nRapRpPb; iy++) {
		if (iy>=1 && iy<=6) {
			pytmp[iy][0]=-532;
	    eytmp[iy][0]=0; eysys[iy][0]=0;
      exlow[iy][0]=0; exhigh[iy][0]=0;
			pytmp[iy][1]=-532;
			eytmp[iy][1]=0; eysys[iy][1]=0;
      exlow[iy][1]=0; exhigh[iy][1]=0;
		}
		if (iy>=2 && iy<=5) {
			pytmp[iy][2]=-532;
	    eytmp[iy][2]=0; eysys[iy][2]=0;
      exlow[iy][2]=0; exhigh[iy][2]=0;
		}
  	if (iy>=2 && iy<=4) {
			pytmp[iy][3]=-532;
	    eytmp[iy][3]=0; eysys[iy][3]=0;
      exlow[iy][3]=0; exhigh[iy][3]=0;
		}
	}

  ////////////////////////////////////////////////////////////////
 
  TGraphAsymmErrors* g_RpPb_sys[nRapRpPb];
  TGraphAsymmErrors* g_RpPb[nRapRpPb];
   
  for (Int_t iy = 0; iy < nRapRpPb; iy++) { 
  //for (Int_t iy = 0; iy < 1; iy++) { 
    //g_RpPb_sys[iy] = new TGraphAsymmErrors(nPt, px_pA[iy], pytmp[iy], exsys, exsys, eysys[iy], eysys[iy]);
    g_RpPb_sys[iy] = new TGraphAsymmErrors(nPt, px_pA[iy], pytmp[iy], exlow[iy], exhigh[iy], eysys[iy], eysys[iy]);
    g_RpPb[iy] = new TGraphAsymmErrors(nPt, px_pA[iy], pytmp[iy], ex, ex, eytmp[iy], eytmp[iy]);
    //g_RpPb[iy] = new TGraphAsymmErrors(nPt, px_pA[iy], pytmp[iy], exlow[iy], exhigh[iy], eytmp[iy], eytmp[iy]);
		g_RpPb_sys[iy] -> SetName(Form("g_RpPb_sys_%d",iy));
		g_RpPb[iy] -> SetName(Form("g_RpPb_%d",iy));
    g_RpPb_sys[iy]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    g_RpPb_sys[iy]->GetXaxis()->SetTitleOffset(0.9);
    if (iy==0 || iy==3) {
      g_RpPb_sys[iy]->GetXaxis()->SetTitleSize(0.067);
      g_RpPb_sys[iy]->GetXaxis()->SetTitleOffset(1.0);
      g_RpPb_sys[iy]->GetXaxis()->SetLabelSize(0.05);
      g_RpPb_sys[iy]->GetXaxis()->SetLabelOffset(0.003);
    } else {
      g_RpPb_sys[iy]->GetXaxis()->SetTitleSize(0.075);
      g_RpPb_sys[iy]->GetXaxis()->SetLabelSize(0.06);
      g_RpPb_sys[iy]->GetXaxis()->SetLabelOffset(0.004);
    }
    g_RpPb_sys[iy]->GetXaxis()->CenterTitle();
    g_RpPb_sys[iy]->GetYaxis()->SetTitle("R_{pPb}");
    g_RpPb_sys[iy]->GetYaxis()->SetTitleOffset(1.1);
    g_RpPb_sys[iy]->GetYaxis()->SetTitleSize(0.075);
    g_RpPb_sys[iy]->GetYaxis()->SetLabelSize(0.055);
    g_RpPb_sys[iy]->GetYaxis()->CenterTitle();
    g_RpPb_sys[iy]->GetXaxis()->SetLimits(0.,31.0);
    g_RpPb_sys[iy]->SetMinimum(0.0);
    g_RpPb_sys[iy]->SetMaximum(1.8);
//    g_RpPb_sys[iy]->SetMinimum(0.4);
//    g_RpPb_sys[iy]->SetMaximum(1.6);
  } 

  g_RpPb_sys[0]->SetFillColor(kGreen-10);
  g_RpPb_sys[1]->SetFillColor(kRed-10);
  g_RpPb_sys[2]->SetFillColor(kBlue-10);
  g_RpPb_sys[3]->SetFillColor(kBlue-10);
  g_RpPb_sys[4]->SetFillColor(kRed-10);
  g_RpPb_sys[5]->SetFillColor(kGreen-10);
  g_RpPb_sys[6]->SetFillColor(kMagenta-10);
 
  g_RpPb_sys[0]->SetLineColor(kGreen+3);
  g_RpPb_sys[1]->SetLineColor(kPink-6);
  g_RpPb_sys[2]->SetLineColor(kBlue-3);
  g_RpPb_sys[3]->SetLineColor(kBlue-3);
  g_RpPb_sys[4]->SetLineColor(kPink-6);
  g_RpPb_sys[5]->SetLineColor(kGreen+3);
  g_RpPb_sys[6]->SetLineColor(kViolet-6);
 /* 
  g_RpPb_sys[0]->SetFillColor(kWhite);
  g_RpPb_sys[1]->SetFillColor(kWhite);
  g_RpPb_sys[2]->SetFillColor(kWhite);
  g_RpPb_sys[3]->SetFillColor(kWhite);
  g_RpPb_sys[4]->SetFillColor(kWhite);
  g_RpPb_sys[5]->SetFillColor(kWhite);
  g_RpPb_sys[6]->SetFillColor(kWhite);
  */
  SetGraphStyleFinal(g_RpPb[0],  0,5);
  g_RpPb[0]->SetMarkerSize(1.8);
  SetGraphStyleFinal(g_RpPb[1],  1,3);
  g_RpPb[1]->SetMarkerSize(1.1);
  SetGraphStyleFinal(g_RpPb[2],  2,0);
  g_RpPb[2]->SetMarkerSize(1.1);
  SetGraphStyleFinal(g_RpPb[3],  2,0);
  g_RpPb[3]->SetMarkerSize(1.1);
  SetGraphStyleFinal(g_RpPb[4],  1,3);
  g_RpPb[4]->SetMarkerSize(1.1);
  SetGraphStyleFinal(g_RpPb[5],  0,5);
  g_RpPb[5]->SetMarkerSize(1.8);
  SetGraphStyleFinal(g_RpPb[6],  8,6);
  g_RpPb[6]->SetMarkerSize(1.3);
  
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
	globtex->SetTextSize(0.075);

  //// Draw 
	TBox* emptybox = new TBox(0.0, 0.0, 0.97, 0.86);
	emptybox->SetFillColor(kWhite);
	emptybox->SetFillStyle(4000);
	emptybox->SetLineColor(kBlack);
	emptybox->SetLineWidth(1);
  
  //TCanvas* c_all = new TCanvas("c_all","c_all",1050,600);
  TCanvas* c_all = new TCanvas("c_all","c_all",1200,700);
  c_all->Divide(4,2);
  TVirtualPad* pad_all[nRap]; // 8 pads
  pad_all[0] = new TPad("pad_all_0", "",0, 0.506, 0.29, 1);
  pad_all[1] = new TPad("pad_all_1", "",0.29, 0.506, 0.52, 1);
  pad_all[2] = new TPad("pad_all_2", "",0.52, 0.506, 0.75, 1);
  pad_all[3] = new TPad("pad_all_3", "",0.75, 0.506, 0.99, 1);
  pad_all[4] = new TPad("pad_all_4", "",0, 0.0, 0.29, 0.506);
  pad_all[5] = new TPad("pad_all_5", "",0.29, 0.0, 0.52, 0.506);
  pad_all[6] = new TPad("pad_all_6", "",0.52, 0.0, 0.75, 0.506);
  pad_all[7] = new TPad("pad_all_7", "",0.75, 0.0, 0.99, 0.506);
  for (Int_t iy = 0; iy < nRap; iy++) { 
    pad_all[iy]->Draw();
    if (iy<=3) { 
      pad_all[iy]->SetTopMargin(0.14); 
      pad_all[iy]->SetBottomMargin(0.0); 
    }
    else { 
      pad_all[iy]->SetTopMargin(0.0); 
      pad_all[iy]->SetBottomMargin(0.161); 
    }
    if (iy==0 || iy==4) { pad_all[iy]->SetLeftMargin(0.20);}
    else {pad_all[iy]->SetLeftMargin(0.0);}
    if (!(iy==3 || iy==7)) { pad_all[iy]->SetRightMargin(0.0);}
  }
/*
  for (Int_t iy = 0; iy < nRap; iy++) { 
    pad_all[iy] = c_all->cd(iy+1);
    if (iy<=3) { pad_all[iy]->SetBottomMargin(0.0); }
    else { pad_all[iy]->SetTopMargin(0.0); }
    if (!(iy==0 || iy==4)) { pad_all[iy]->SetLeftMargin(0.0);}
    if (!(iy==3 || iy==7)) { pad_all[iy]->SetRightMargin(0.0);}
  }
*/  
  for (Int_t iy = 0; iy < nRapRpPb; iy++) { 
//    if (iy< 3) pad_all[iy]->cd();
    //// reverse the order for forward
    if (iy==0) pad_all[2]->cd();
    else if (iy==1) pad_all[1]->cd();
    else if (iy==2) pad_all[0]->cd();
    else pad_all[iy+1]->cd();
    //g_RpPb_sys[iy]->Draw("A2");
    g_RpPb_sys[iy]->Draw("A5");
    g_RpPb[iy]->Draw("P");
    dashedLine(0.,1.,31.,1.,1,1);
    globtex->SetTextAlign(32); //3:right 2:vertical center
	  globtex->SetTextFont(42);
	  //if (iy==0 || iy==3) globtex->SetTextSize(0.063);
	  if (iy==2 || iy==3) globtex->SetTextSize(0.063);
	  else globtex->SetTextSize(0.078);
    if (iy<3) globtex->DrawLatex(0.9, 0.09, rapArr[iy].Data());
    else globtex->DrawLatex(0.9, 0.24, rapArr[iy].Data());
  }
  pad_all[3]->cd();
  emptybox->Draw("l");

  
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
	leFW3->SetFillColor(kAzure-9);
	leFW3->SetFillStyle(1001);
	leFW3->SetLineColor(kBlue-3);
	leFW3->SetMarkerStyle(kFullCircle);
	leFW3->SetMarkerColor(kBlue-3);
	leFW3->SetMarkerSize(2.1);
	TLegendEntry *leBW1=legBRBW->AddEntry("leBW1",Form("  %s", rapArr[3].Data()),"lpf");
	leBW1->SetFillColor(kAzure-9);
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

	//legBRFW->Draw();
	//legBRBW->Draw();
	
	globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextSize(0.09);
	globtex->SetTextFont(42);
	if (isPrompt) globtex->DrawLatex(0.08, 0.28, "Prompt J/#psi");
	else globtex->DrawLatex(0.08, 0.28, "Non-prompt J/#psi");
  globtex->SetTextAlign(12); //1:left 2:vertical center
	globtex->SetTextSize(0.07);
	globtex->SetTextFont(42);
  globtex->DrawLatex(0.08, 0.18, "Global uncertainty : 5.3 \%");

/*
  // lumiText here
    lumiTextTmp = "pPb ";
    lumiTextTmp += lumi_pPb502TeV;
    globtex->DrawLatex(0.41, 0.44, lumiTextTmp);
    lumiTextTmp = ", pp ";
    lumiTextTmp += lumi_pp502TeV;
    globtex->DrawLatex(0.41, 0.34, lumiTextTmp);
    lumiText = " (5.02 TeV)";
    globtex->DrawLatex(0.41, 0.24, lumiTextTmp);
*/	

  CMS_lumi( c_all, isPA, iPos );
	c_all->Update();
	c_all->Modified();

  if (noPtWeight) { 
    c_all->SaveAs(Form("plot_RpPb/all_RpPb_pt_isPrompt%d_noPtWeight.pdf",(int)isPrompt));
    c_all->SaveAs(Form("plot_RpPb/all_RpPb_pt_isPrompt%d_noPtWeight.png",(int)isPrompt));
  } else {
    c_all->SaveAs(Form("plot_RpPb/all_RpPb_pt_isPrompt%d.pdf",(int)isPrompt));
    c_all->SaveAs(Form("plot_RpPb/all_RpPb_pt_isPrompt%d.png",(int)isPrompt));
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
  //latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);
  latex.DrawLatex(1-r+0.01,1-t+lumiTextOffset*t+0.01,lumiText);//KYO
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
      //if (iPosX==33) {posX_ -= 0.03; posY_-=0.03; } // KYO
      if (iPosX==33) {posX_ += 0.03; posY_-=0.01; } // KYO
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
