#include "CMS_lumi.h"
#include "../SONGKYO.h"

void formRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString* arr);
void formPtArr(Double_t binmin, Double_t binmax, TString* arr);

void CMS_lumi( TPad* pad, int iPeriod, int iPosX );

void draw_RFB_pt_mergedBin(bool sysByHand=false, bool noPtWeight=false, bool isPrompt=false)
{
  gROOT->Macro("./tdrstyle_kyo.C");
  //cmsTextFont   = 42;  // for b.hong
  //extraTextFont = 42;  // for b.hong
  int isPA = 1;  // 0:pp, 1:pPb
	//int iPos=0; //outside topleft
	int iPos=33; //right corner

  //// BR and lumi info.
  const Double_t br = 0.0593 ;
  const Double_t brErr = 0.0006;
  const Double_t pPb_lumi_nb = 34.622; // 34.6/nb
  const Double_t pPb_lumi_nb_err = 1.2; // 3.5 %
  const Double_t pPb_lumi_mub = pPb_lumi_nb * 1000; // (nb)^{-1} -> {#mub}^{-1}
  const Double_t pPb_lumi_mub_err = pPb_lumi_nb_err * 1000; // (nb)^{-1} -> {#mub}^{-1}
  
  /////////////////////////////////////////////////////////////////////////
  // bin center & systematic uncertainties by hand
  // for 1.5 - 1.93 
  const int nRap = 8; 
  const int nPt = 9;
  const int nRapTmp = nRap + 1;
  const int nPtTmp = nPt + 1;
  const int nRapRFB = 3;  
  const int nPtRFB = 3;
  
  Double_t pxtmp[nRapRFB][nPtRFB]; //x point to fill remporarily
  Double_t pytmp[nRapRFB][nPtRFB]; //y point to fill remporarily
  Double_t eytmp[nRapRFB][nPtRFB]; //y point error to fill remporarily
  /// 2015 PAS
  Double_t px[nRapRFB][nPtRFB] = { // x point (mean pT)
    {5.75945, 7.90077, 13.2826}, //1.5-1.93
    {-531, 7.98106, 13.2588}, //0.9-1.5
    {-531, 8.2616, 13.5598} //0.0-0.9
  };
  Double_t ex[nPtRFB] = {0.,0.,0.}; // x stat error (0)
  Double_t exlow[nRapRFB][nPtRFB];
  Double_t exhigh[nRapRFB][nPtRFB];
  //Double_t exsys[nPtRFB] = {0.25,0.25,0.25}; // x sys err (box width)
  Double_t exsys[nPtRFB] = {0.3,0.3,0.3}; // x sys err (box width)
  Double_t eysys[nRapRFB][nPtRFB]; //absolute y sys error
  //Double_t eysysrel[nRapRFB][nPtRFB]; //relative y sys error
  /*
  Double_t eysysrelPR[nRapRFB][nPtRFB] = {
    {0.04529, 0.04795, 0.06486}, //1.5-1.03
    {0.00, 0.04245, 0.03360}, //0.9-1.5
    {0.00, 0.04694, 0.03705} //0.0-0.9
  };
  Double_t eysysrelNP[nRapRFB][nPtRFB] = {
    {0.06133, 0.10487,  0.09280},
    {0.00, 0.06288, 0.05592},
    {0.00, 0.07412, 0.04483}
  };
  for (int iy=0; iy<nRapRFB; iy++){
    for (int ipt=0; ipt<nPtRFB; ipt++){
      if(isPrompt) eysysrel[iy][ipt] = eysysrelPR[iy][ipt];
      else eysysrel[iy][ipt] = eysysrelNP[iy][ipt];
    }
  }
*/
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

  //////////////////////////////////////////////////////////////  
  //// read-in sys. file 
  TFile * fSys;
  if (isPA==0) fSys = new TFile("../TotalSys/TotSys_8rap9pt_pp_etOpt0.root");
  else fSys = new TFile("../TotalSys/TotSys_8rap9pt_pA_etOpt0.root");
  TH2D* h2D_SysErr;
  if (isPrompt) h2D_SysErr = (TH2D*)fSys->Get("hTotalPR");
  else h2D_SysErr = (TH2D*)fSys->Get("hTotalNP");
//  cout << " *** h2D_SysErr = " <<  h2D_SysErr << endl;  

  /////////////////////////////////////////////////////////////////////////
  //// read-in file
  TFile * f2D;
  if (noPtWeight) f2D = new TFile("../FittingResult/totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF1_etOpt0_noPtWeight.root");
  else f2D = new TFile("../FittingResult/totalHist_pA_8rap9pt_newcut_nominal_Zvtx1_SF1_etOpt0.root");
  //// read-in 2D hist
  TH2D* h2D_CorrY;
  if (isPrompt) h2D_CorrY = (TH2D*)f2D->Get("h2D_CorrY_PR_pA");
  else h2D_CorrY = (TH2D*)f2D->Get("h2D_CorrY_NP_pA");
  h2D_CorrY->SetName("h2D_CorrY");  
  const int nbinsX = h2D_CorrY->GetNbinsX();
  const int nbinsY = h2D_CorrY->GetNbinsY();
  if (nbinsX != nRap) { cout << " *** Error!!! nbinsX != nRap"; return; };
  if (nbinsY != nPt) { cout << " *** Error!!! nbinsY != nPt"; return; };
  
  //// projection to 1D hist : iy=0 refers to forwards !!! (ordering here)
  TH1D* h1D_CorrY[nRap]; 
  TH1D* h1D_SysErr[nRap];
  for (Int_t iy = 0; iy < nRap; iy++) {
    h1D_CorrY[iy] = h2D_CorrY->ProjectionY(Form("h1D_CorrY_%d",iy),iy+1,iy+1);
    h1D_SysErr[iy] = h2D_SysErr->ProjectionY(Form("h1D_SysErr_%d",iy),iy+1,iy+1);
  }
  
  //////////////////////////////////////////////////////////////////
  //// calculate RFB

  TH1D* h1D_RFB_tmp[nRap]; // CorrYield with merged pT    
  TH1D* h1D_RFB[nRapRFB]; // actual RFB   
  TH1D* h1D_RFBSys_tmp[nRap]; // Sys with merged pT   
  TH1D* h1D_RFBSys[nRapRFB]; // Sys F/B   
  
  //// RFB bin setting
  TString rapAbsArr[nRap];
  for (Int_t iy=0; iy<nRapRFB; iy++) {
    formAbsRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapAbsArr[iy]);
    cout << iy <<"th rapAbsArr = " << rapAbsArr[iy] << endl;
  }
  Double_t ptArrRFBNum[nPtRFB+1] = {5.0, 6.5, 10., 30.};
  
  //// ex calculation
  for (Int_t iy=0; iy<nRapRFB; iy++) {
    for (Int_t ipt=0; ipt<nPtRFB; ipt++) {
      exlow[iy][ipt] = px[iy][ipt]-ptArrRFBNum[ipt]; 
      exhigh[iy][ipt] = ptArrRFBNum[ipt+1]-px[iy][ipt]; 
      //cout << iy<<"th y, "<<ipt<<"th pt"<<endl;
      //cout << "exlow = "<< exlow[iy][ipt] << endl;
      //cout << "exhigh = " <<exhigh[iy][ipt] << endl;
    }
  }
  
  //// take proper error propagataion for sys (merge pT bins (KYO - byHand))
  double tmpsys01, tmpsys02, tmpsys03;
  double actsys01, actsys02, actsys03;
  for (int iy=0; iy<nRapRFB*2; iy++){
    h1D_RFBSys_tmp[iy]= new TH1D(Form("h1D_RFBSys_tmp_%d",iy),Form("h1D_RFBSys_tmp_%d",iy),nPtRFB,ptArrRFBNum);
    h1D_RFBSys_tmp[iy]->Sumw2();
    actsys01=0; actsys02=0; actsys03=0;
    //// 1) pT 5-6.5 GeV
    tmpsys01=0; tmpsys02=0;tmpsys03=0;
    if (iy==0 || iy==nRapRFB*2-1){
      //// from relative error to absolute error
      //actsys01=h1D_SysErr[iy]->GetBinContent(4);
      actsys01=h1D_SysErr[iy]->GetBinContent(4)*h1D_CorrY[iy]->GetBinContent(4);  
      h1D_RFBSys_tmp[iy]->SetBinContent(1,actsys01);
    } else {
      h1D_RFBSys_tmp[iy]->SetBinContent(1,0.);
    }
    //// 2) pT 6.5-10. GeV
    tmpsys01=0; tmpsys02=0;tmpsys03=0;
    //// from relative error to absolute error
    tmpsys01=h1D_SysErr[iy]->GetBinContent(5)*h1D_CorrY[iy]->GetBinContent(5);  
    tmpsys02=h1D_SysErr[iy]->GetBinContent(6)*h1D_CorrY[iy]->GetBinContent(6);  
    tmpsys03=h1D_SysErr[iy]->GetBinContent(7)*h1D_CorrY[iy]->GetBinContent(7);  
    actsys02=TMath::Sqrt( TMath::Power(tmpsys01,2) + TMath::Power(tmpsys02,2) +TMath::Power(tmpsys03,2) );
    h1D_RFBSys_tmp[iy]->SetBinContent(2,actsys02);
    //// 3) pT 10.-30. GeV
    tmpsys01=0; tmpsys02=0;tmpsys03=0;
    //// from relative error to absolute error
    tmpsys01=h1D_SysErr[iy]->GetBinContent(8)*h1D_CorrY[iy]->GetBinContent(8);  
    tmpsys02=h1D_SysErr[iy]->GetBinContent(9)*h1D_CorrY[iy]->GetBinContent(9);  
    actsys03=TMath::Sqrt( TMath::Power(tmpsys01,2) + TMath::Power(tmpsys02,2) );
    h1D_RFBSys_tmp[iy]->SetBinContent(3,actsys03);
  }
  //TCanvas *ctmp = new TCanvas("ctmp","",1200,800); ctmp->Divide(3,2);
  //for (int iy=0; iy<nRapRFB*2; iy++){
  //  ctmp->cd(iy+1);
  //  h1D_RFBSys_tmp[iy]->Draw("");
  //}
  
  //// merge pT bins (KYO - byHand)
  double tmpPRval01, tmpPRerr01, tmpPRval02, tmpPRerr02, tmpPRval03, tmpPRerr03;
  double actPRval01, actPRerr01, actPRval02, actPRerr02, actPRval03, actPRerr03;
  for (int iy=0; iy<nRapRFB*2; iy++){
    h1D_RFB_tmp[iy]= new TH1D(Form("h1D_RFB_tmp_%d",iy),Form("h1D_RFB_tmp_%d",iy),nPtRFB,ptArrRFBNum);
    h1D_RFB_tmp[iy]->Sumw2();
    actPRval01=0; actPRval02=0; actPRval03=0; actPRerr01=0; actPRerr02=0; actPRerr03=0;
    //// 1) pT 5-6.5 GeV
    tmpPRval01=0; tmpPRval02=0;tmpPRval03=0; tmpPRerr01=0; tmpPRerr02=0;tmpPRerr03=0;
    if (iy==0 || iy==nRapRFB*2-1){
      actPRval01=h1D_CorrY[iy]->GetBinContent(4); 
      actPRerr01=h1D_CorrY[iy]->GetBinError(4); 
      h1D_RFB_tmp[iy]->SetBinContent(1,actPRval01);
      h1D_RFB_tmp[iy]->SetBinError(1,actPRerr01);
    } else {
      h1D_RFB_tmp[iy]->SetBinContent(1,0.);
      h1D_RFB_tmp[iy]->SetBinError(1,0.);
    }
    //// 2) pT 6.5-10. GeV
    tmpPRval01=0; tmpPRval02=0;tmpPRval03=0; tmpPRerr01=0; tmpPRerr02=0;tmpPRerr03=0;
    tmpPRval01=h1D_CorrY[iy]->GetBinContent(5); 
    tmpPRerr01=h1D_CorrY[iy]->GetBinError(5); 
    tmpPRval02=h1D_CorrY[iy]->GetBinContent(6); 
    tmpPRerr02=h1D_CorrY[iy]->GetBinError(6); 
    tmpPRval03=h1D_CorrY[iy]->GetBinContent(7); 
    tmpPRerr03=h1D_CorrY[iy]->GetBinError(7); 
    actPRval02=tmpPRval01+tmpPRval02+tmpPRval03;
    actPRerr02=TMath::Sqrt( TMath::Power(tmpPRerr01,2) + TMath::Power(tmpPRerr02,2) +TMath::Power(tmpPRerr03,2) );
    h1D_RFB_tmp[iy]->SetBinContent(2,actPRval02);
    h1D_RFB_tmp[iy]->SetBinError(2,actPRerr02);
    //// 3) pT 10.-30. GeV
    tmpPRval01=0; tmpPRval02=0;tmpPRval03=0; tmpPRerr01=0; tmpPRerr02=0;tmpPRerr03=0;
    tmpPRval01=h1D_CorrY[iy]->GetBinContent(8); 
    tmpPRerr01=h1D_CorrY[iy]->GetBinError(8); 
    tmpPRval02=h1D_CorrY[iy]->GetBinContent(9); 
    tmpPRerr02=h1D_CorrY[iy]->GetBinError(9); 
    actPRval03=tmpPRval01+tmpPRval02;
    actPRerr03=TMath::Sqrt( TMath::Power(tmpPRerr01,2) + TMath::Power(tmpPRerr02,2) );
    h1D_RFB_tmp[iy]->SetBinContent(3,actPRval03);
    h1D_RFB_tmp[iy]->SetBinError(3,actPRerr03);
    //for (int ipt=0; ipt<nPtRFB; ipt++){
    //  cout << iy<<"th rap, pT merged PR CorrYield = " << h1D_RFB_tmp[iy]->GetBinContent(ipt+1) <<endl;
    //}
  }
  //////////////////////////////////////////////////////////////////
  //// actual RFB calculation 
  for (int iy=0; iy<nRapRFB; iy++){
    h1D_RFB[iy] = (TH1D*)h1D_RFB_tmp[iy]->Clone(Form("h1D_RFB_%d",iy));
    h1D_RFB[iy]->Divide(h1D_RFB_tmp[2*nRapRFB-iy-1]); 
  }
  //// sys F/B calculation
  Double_t dummy;
  for (int iy=0; iy<nRapRFB; iy++){
    for (int ipt=0; ipt<nPtRFB; ipt++){
      //eysysrel[iy][ipt] = TMath::Sqrt(h1D_RFBSys_tmp[iy]->GetBinContent(ipt+1)*h1D_RFBSys_tmp[iy]->GetBinContent(ipt+1)+ h1D_RFBSys_tmp[2*nRapRFB-iy-1]->GetBinContent(ipt+1)*h1D_RFBSys_tmp[2*nRapRFB-iy-1]->GetBinContent(ipt+1)); 
      DivideValue(h1D_RFB_tmp[iy]->GetBinContent(ipt+1),h1D_RFBSys_tmp[iy]->GetBinContent(ipt+1),h1D_RFB_tmp[2*nRapRFB-iy-1]->GetBinContent(ipt+1),h1D_RFBSys_tmp[2*nRapRFB-iy-1]->GetBinContent(ipt+1),&dummy,&eysys[iy][ipt]);
      //cout << "FW bin = " << h1D_RFBSys_tmp[iy]->GetBinContent(ipt+1) << endl;
      //cout << "BW bin = " << h1D_RFBSys_tmp[2*nRapRFB-iy-1]->GetBinContent(ipt+1) << endl;
      //cout << "dummy = " << dummy << endl;
      //cout << "eysys["<<iy<<"]["<<ipt<<"] = " << eysys[iy][ipt] << endl;
    }
  }
  //////////////////////////////////////////////////////////////////

  TLegend *legBR = new TLegend(0.50, 0.18, 0.70, 0.39);
  SetLegendStyle(legBR);
  legBR->SetTextSize(0.05);
    
  TLatex* globtex = new TLatex();
  globtex->SetNDC();
  globtex->SetTextAlign(12); //1:left, 2:vertical center
  //globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
  globtex->SetTextSize(0.04);

  //TCanvas* c1 = new TCanvas("c1","c1",600,600);
  //c1->cd();
  
  //////////////////////////////////////////////////////////////////
  //// convert to TGraphAsymErrors
  const int fw_init = 0;
  
  //// 1) sys graph
  TGraphAsymmErrors*gRFB_sys[nRapRFB]; 
  for (int iy=0; iy<nRapRFB; iy++){
    gRFB_sys[iy] = new TGraphAsymmErrors(h1D_RFB[iy]);
    gRFB_sys[iy] -> SetName(Form("gRFB_sys_%d",iy));
    for (int ipt=0; ipt<nPtRFB; ipt++){
      gRFB_sys[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
      gRFB_sys[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
      //// absolute err calculation
      //eysys[iy][ipt]=eysysrel[iy][ipt]*pytmp[iy][ipt];
      //gRFB_sys[iy]->SetPointError(ipt, exsys[ipt], exsys[ipt], eysys[iy][ipt], eysys[iy][ipt]);
      gRFB_sys[iy]->SetPointError(ipt, exlow[iy][ipt], exhigh[iy][ipt], eysys[iy][ipt], eysys[iy][ipt]);
    }
    gRFB_sys[iy]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    gRFB_sys[iy]->GetXaxis()->CenterTitle();
    gRFB_sys[iy]->GetYaxis()->SetTitle("R_{FB}");
    gRFB_sys[iy]->GetYaxis()->CenterTitle();
    gRFB_sys[iy]->GetXaxis()->SetLimits(0.,32.0);
    gRFB_sys[iy]->SetMinimum(0.0);
    gRFB_sys[iy]->SetMaximum(1.8);
  }
  
  gRFB_sys[0]->SetFillColor(kGreen-10);
  gRFB_sys[1]->SetFillColor(kRed-10);
  gRFB_sys[2]->SetFillColor(kBlue-10);
  gRFB_sys[0]->SetLineColor(kGreen+3);
  gRFB_sys[1]->SetLineColor(kPink-6);
  gRFB_sys[2]->SetLineColor(kBlue-3);

  //// 2) RFB graph
  TGraphAsymmErrors*gRFB[nRapRFB]; 
  for (int iy=0; iy<nRapRFB; iy++){
    gRFB[iy] = new TGraphAsymmErrors(h1D_RFB[iy]);
    gRFB[iy] -> SetName(Form("gRFB_%d",iy));
    cout << "::: for excel ::: iy= " << iy << endl;
    for (int ipt=0; ipt<nPtRFB; ipt++){
      gRFB[iy]->GetPoint(ipt, pxtmp[iy][ipt], pytmp[iy][ipt]);
      eytmp[iy][ipt] = gRFB[iy]-> GetErrorY(ipt);
      gRFB[iy]->SetPoint(ipt, px[iy][ipt], pytmp[iy][ipt]);
      gRFB[iy]->SetPointEXlow(ipt, ex[iy]);
      gRFB[iy]->SetPointEXhigh(ipt, ex[iy]);
      cout << pytmp[iy][ipt] <<"\t"<<eytmp[iy][ipt] << "\t "<<eysys[iy][ipt]<<endl;
    }
  }
  SetGraphStyleFinal(gRFB[0], 0, 5); //1.5-1.93
  SetGraphStyleFinal(gRFB[1], 1, 3); //0.9-1.5
  SetGraphStyleFinal(gRFB[2], 2, 0); //0-0.9
  gRFB[0]->SetMarkerSize(2.6);
  gRFB[1]->SetMarkerSize(1.4);
  gRFB[2]->SetMarkerSize(1.4);

  ///////////////// CANVAS 1  
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  c1->cd(); 
  gRFB_sys[0]->Draw("A5");
  gRFB[0]->Draw("P");
  dashedLine(0.,1.,32.,1.,1,1);
  
  TLegendEntry *le1=legBR->AddEntry("le1",Form("  %s", rapAbsArr[2].Data()),"lpf");
  le1->SetFillColor(kBlue-10);
  le1->SetFillStyle(1001);
  le1->SetLineColor(kBlue-3);
  le1->SetMarkerStyle(kFullCircle);
  le1->SetMarkerColor(kBlue-3);
  le1->SetMarkerSize(2.1);
  TLegendEntry *le2=legBR->AddEntry("le2",Form("  %s", rapAbsArr[1].Data()),"lpf");
  le2->SetFillColor(kRed-10);
  le2->SetFillStyle(1001);
  le2->SetLineColor(kPink-6);
  le2->SetMarkerStyle(kFullSquare);
  le2->SetMarkerColor(kPink-6);
  le2->SetMarkerSize(2.1);
  TLegendEntry *le3=legBR->AddEntry("le3",Form("  %s", rapAbsArr[0].Data()),"lpf");
  le3->SetFillColor(kGreen-10);
  le3->SetFillStyle(1001);
  le3->SetLineColor(kGreen+3);
  le3->SetMarkerStyle(kFullDiamond);
  le3->SetMarkerColor(kGreen+3);
  le3->SetMarkerSize(3.3);

  //legBR->Draw();

  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
  globtex->SetTextSize(0.05);
  globtex->DrawLatex(0.9, 0.23, rapAbsArr[0].Data());
  
  globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextSize(0.055);
  globtex->SetTextFont(42);
  if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
  else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");

  CMS_lumi( c1, isPA, iPos );
  c1->Update();
  
  if (noPtWeight) { 
    c1->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_noPtWeight_rap1.pdf",(int)isPrompt));
    c1->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_noPtWeight_rap1.png",(int)isPrompt));
  } else {
    c1->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_rap1.pdf",(int)isPrompt));
    c1->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_rap1.png",(int)isPrompt));
  }

  ///////////////// CANVAS 2  
  
  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  c2->cd();
  gRFB_sys[1]->Draw("A5");
  gRFB[1]->Draw("P");
  dashedLine(0.,1.,32.,1.,1,1);
  //legBR->Draw();
  
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
  globtex->SetTextSize(0.05);
  globtex->DrawLatex(0.9, 0.23, rapAbsArr[1].Data());
  
  globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextSize(0.055);
  globtex->SetTextFont(42);
  if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
  else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");
  
  CMS_lumi( c2, isPA, iPos );
  c2->Update();
 
  if (noPtWeight) { 
    c2->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_noPtWeight_rap2.pdf",(int)isPrompt));
    c2->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_noPtWeight_rap2.png",(int)isPrompt));
  } else {
    c2->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_rap2.pdf",(int)isPrompt));
    c2->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_rap2.png",(int)isPrompt));
  }

  
  ///////////////// CANVAS 3  
  
  TCanvas* c3 = new TCanvas("c3","c3",600,600);
  c3->cd();
  gRFB_sys[2]->Draw("A5");
  gRFB[2]->Draw("P");
  dashedLine(0.,1.,32.,1.,1,1);
  ///legBR->Draw();
  
  globtex->SetTextAlign(32); //3:right 2:vertical center
  globtex->SetTextFont(42);
  globtex->SetTextSize(0.05);
  globtex->DrawLatex(0.9, 0.23, rapAbsArr[2].Data());

  globtex->SetTextAlign(12); //1:left, 2:vertical center
  globtex->SetTextSize(0.055);
  globtex->SetTextFont(42);
  if (isPrompt) globtex->DrawLatex(0.21, 0.84, "Prompt J/#psi");
  else globtex->DrawLatex(0.21, 0.84, "Non-prompt J/#psi");

  CMS_lumi( c3, isPA, iPos );
  c3->Update();

  if (noPtWeight) { 
    c3->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_noPtWeight_rap3.pdf",(int)isPrompt));
    c3->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_noPtWeight_rap3.png",(int)isPrompt));
  } else {
    c3->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_rap3.pdf",(int)isPrompt));
    c3->SaveAs(Form("plot_RFB/RFB_pt_mergedBin_isPrompt%d_rap3.png",(int)isPrompt));
  }
  
  ///////////////////////////////////////////////////////////////////
  //// save as a root file
  TFile *outFile;
  if (noPtWeight) outFile = new TFile(Form("plot_RFB/RFB_pt_isPrompt%d_noPtWeight.root",(int)isPrompt),"RECREATE");
  else outFile = new TFile(Form("plot_RFB/RFB_pt_isPrompt%d.root",(int)isPrompt),"RECREATE");
  outFile->cd();
  for (int iy=0; iy<nRapRFB; iy++){
    gRFB_sys[iy]->Write();  
    gRFB[iy]->Write();  
  }
  outFile->Close();
  
  return;

} // end of main func.

void formRapArr(Double_t binmin, Double_t binmax, TString* arr) {
  Double_t intMin, intMax; 
  Double_t fracMin = modf(binmin, &intMin);
  Double_t fracMax = modf(binmax, &intMax);
  if ( binmin == 1.93 || binmin == -1.93 || binmin == -2.40 || binmin == -2.87) {
    *arr = Form("%.2f < y_{CM} < %.1f", binmin, binmax);
  }
  else if ( binmax == 1.93 || binmax ==  -1.93 || binmax == 2.40 || binmax == -2.87 ) {
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
  if ( binmin == 1.93 || binmin == -1.93 || binmin == -2.40 || binmin == -2.87) {
    *arr = Form("%.2f < |y_{CM}| < %.1f", binmin, binmax);
  }
  else if ( binmax == 1.93 || binmax ==  -1.93 || binmax == 2.40 || binmax == -2.87 ) {
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
