#include <TStyle.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TCut.h>
#include <TClonesArray.h>
#include <TRandom3.h>

#include <RooFit.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooArgSet.h>

#include <TStyle.h>
#include <TLatex.h>
#include <TDirectory.h>
#include <TCollection.h>
#include <TPostScript.h>

#include "commonUtility.h"

using namespace RooFit;
using namespace std;

TGraphAsymmErrors *plotEffPt(RooDataSet *a, int aa);
void drawTnPDataVsMc(int opt=4)  // 1 tracking,  2 muonId,  3 trigger
{
  // data
  TFile *f1; 
  RooDataSet *daTrkPt1[10];
  TGraphAsymmErrors *eff_pt1[10];
  // mc
  TFile *f2; 
  RooDataSet *daTrkPt2[10];
  TGraphAsymmErrors *eff_pt2[10];

  TGraphAsymmErrors *eff_rate[10];

  
  TString label = "";
  int nEtaBin = 3;  
  if ( opt == 1 )  { 
    f1 = new TFile("FitRootFiles/tracking/outputTrackCuts_binned_data_allDirection_v8.root");
    for ( int ieta = 1; ieta<=nEtaBin ; ieta++)   
      daTrkPt1[ieta] = (RooDataSet*)f1->Get(Form("tpTreeSta/ptBin_eta%d/fit_eff",ieta));
    f2 = new TFile("FitRootFiles/tracking/outputTrackCuts_binned_mc_allDirection_v8.root");
    for ( int ieta = 1; ieta<=nEtaBin ; ieta++)   
      daTrkPt2[ieta] = (RooDataSet*)f2->Get(Form("tpTreeSta/ptBin_eta%d/fit_eff",ieta));
    label = "tracking";
  }
  else if ( opt == 4 )  { 
    f1 = new TFile("FitRootFiles/muTriger/outputTriggerMuId_data_allDirection_v5.root");
    for ( int ieta = 1; ieta<=nEtaBin ; ieta++)   
      daTrkPt1[ieta] = (RooDataSet*)f1->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
    f2 = new TFile("FitRootFiles/muTriger/outputTriggerMuId_mc_allDirection_v5.root");
    for ( int ieta = 1; ieta<=nEtaBin ; ieta++)   
      daTrkPt2[ieta] = (RooDataSet*)f2->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
    label = "MuIdAndTrig";
  }
  
  else if ( opt == 2 )  { 
    f1 = new TFile("FitRootFiles/muonId_v2/outputMuonId_data_allDirection_v2.root");
    for ( int ieta = 1; ieta<=nEtaBin ; ieta++)   
      daTrkPt1[ieta] = (RooDataSet*)f1->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
    f2 = new TFile("FitRootFiles/muonId_v2/outputMuonId_mc_allDirection_v2.root");
    for ( int ieta = 1; ieta<=nEtaBin ; ieta++)   
      daTrkPt2[ieta] = (RooDataSet*)f2->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
    label = "muonId";
  }
  else if ( opt == 3 )  { 
    f1 = new TFile("FitRootFiles/trigger_v2/outputTrigger_data_allDirection_v2.root");
    for ( int ieta = 1; ieta<=nEtaBin ; ieta++)   
      daTrkPt1[ieta] = (RooDataSet*)f1->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
    f2 = new TFile("FitRootFiles/trigger_v2/outputTrigger_mc_allDirection_v2.root");
    for ( int ieta = 1; ieta<=nEtaBin ; ieta++)   
      daTrkPt2[ieta] = (RooDataSet*)f2->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
    label = "trigger";
  }
  
  for ( int ieta = 1; ieta<=nEtaBin ; ieta++) {
    eff_pt1[ieta] = plotEffPt(daTrkPt1[ieta], 1);
    eff_pt1[ieta]->SetName(Form("eff_data_%s_etaBin%d",label.Data(),ieta));
    eff_pt2[ieta] = plotEffPt(daTrkPt2[ieta], 1);
    eff_pt2[ieta]->SetName(Form("eff_mc_%s_etaBin%d",label.Data(),ieta));
    eff_rate[ieta] =  new TGraphAsymmErrors();
    eff_rate[ieta]->SetName(Form("eff_ratio_%s_etaBin%d",label.Data(),ieta));
    for ( int ip = 0 ; ip< eff_pt1[ieta]->GetN() ; ip++) { 
      double x1, x2, y1, y2;
      eff_pt1[ieta]->GetPoint(ip, x1, y1);
      eff_pt2[ieta]->GetPoint(ip, x2, y2);
      double error = sqrt( eff_pt1[ieta]->GetErrorY(ip)*eff_pt1[ieta]->GetErrorY(ip) + eff_pt2[ieta]->GetErrorY(ip)*eff_pt2[ieta]->GetErrorY(ip));

      eff_rate[ieta]->SetPoint(ip, x1, y1/y2);
      eff_rate[ieta]->SetPointError(ip, 0, 0, error,error);
      
    }
  }
  
  TH1F* tempTH1f = new TH1F("Graph_hxy_fit_eff1",";p_{T} (GeV/c);Efficiency",100,0,20);
  handsomeTH1(tempTH1f,1);
  TCanvas*c1 = new TCanvas("c1","",300*nEtaBin,700);
  c1->Divide(nEtaBin,2,0.0,0.0);
  for ( int ieta = 1; ieta<=nEtaBin ; ieta++) {
    c1->cd(ieta);
    tempTH1f->SetAxisRange(0.0,1.2,"Y");
    tempTH1f->DrawCopy();
    handsomeTGraph(eff_pt1[ieta],2);
    handsomeTGraph(eff_pt2[ieta],1);
    eff_pt1[ieta]->SetMarkerStyle(20);
    eff_pt2[ieta]->SetMarkerStyle(20);

    eff_pt1[ieta]->Draw("p");
    eff_pt2[ieta]->Draw("p");
    // Fitting 
    TF1 *ferr1 = new TF1(Form("ferr1_ieta%d",ieta),"[0]*TMath::Erf((x-[1])/[2])",0.0,30);
    TF1 *ferr2 = new TF1(Form("ferr2_ieta%d",ieta),"[0]*TMath::Erf((x-[1])/[2])",0.0,30);
    TF1 *ferrScale = new TF1(Form("ferrScale_ieta%d",ieta),"[0]*TMath::Erf((x-[1])/[2]) / ([3]*TMath::Erf((x-[4])/[5]))",0.0,30);
    //hsigmapt_staerr->Fit("fitft_opt1","M","",0.0,120.0);
    ferr1->SetParameter(0,1);
    ferr1->SetParameter(1,4);
    ferr1->SetParameter(2,1);
    ferr2->SetParameter(0,1);
    ferr2->SetParameter(1,4);
    ferr2->SetParameter(2,1);
    float lowx = 1;
    if ( ieta == 1 )  lowx = 3;
    else if ( ieta == 2 )  lowx = 2;
    else if ( ieta == 3 )  lowx = 1;

    eff_pt1[ieta]->Fit(ferr1->GetName(),"LM","",lowx,15);
    ferr1->SetLineColor(2);

    eff_pt2[ieta]->Fit(ferr2->GetName(),"LM","",lowx,15);
    ferr2->SetLineColor(1);
    

    ferr1->Draw("same");
    ferr2->Draw("same");
    
    if  (ieta==1 )    drawText(  "     |#eta|<1.2",0.5,0.3);
    else if ( ieta==2)  drawText("1.2 <|#eta|<1.6",0.5,0.3);
    else if ( ieta==3)  drawText("1.6 <|#eta|<2.4",0.5,0.3);
    
    if ( ieta == 1)  {
      TLegend *leg = new TLegend(0.5562018,0.3933851,0.9975074,0.6073602,NULL,"brNDC");
      if ( opt == 1 )  easyLeg(leg,"Tracking Eff.");
      else if ( opt == 2 )  easyLeg(leg,"Muon ID Eff.");
      else if ( opt == 3 )  easyLeg(leg,"Trg^{L1DoubleMuOpen} Eff.");
      leg->AddEntry(eff_pt1[ieta], "Data","p");
      leg->AddEntry(eff_pt2[ieta], "MC","p");
      leg->Draw();
    }
    jumSun(0,1,20,1);

    
    
    c1->cd(ieta+nEtaBin);
    tempTH1f->SetAxisRange(0.8,1.2,"Y");
    tempTH1f->SetYTitle("Data/MC Ratio");
    tempTH1f->DrawCopy(); 
    
    ferrScale->SetParameter(0, ferr1->GetParameter(0));
    ferrScale->SetParameter(1, ferr1->GetParameter(1));
    ferrScale->SetParameter(2, ferr1->GetParameter(2));
    ferrScale->SetParameter(3, ferr2->GetParameter(0));
    ferrScale->SetParameter(4, ferr2->GetParameter(1));
    ferrScale->SetParameter(5, ferr2->GetParameter(2));
    ferrScale->Draw("same"); //feff_rate[ieta]->Draw("p");
    jumSun(0,1,20,1);




  }
  c1->SaveAs(Form("tnpSummaryPlot_%s.pdf",label.Data()));


 







    
  TFile* fout = new TFile("tnpRate.root","recreate");
  for ( int ieta = 1; ieta<=nEtaBin ; ieta++) {
    eff_pt1[ieta]->Write();
    eff_pt2[ieta]->Write();
    eff_rate[ieta]->Write();
  } 
  fout->Close();

}
    


TGraphAsymmErrors *plotEffPt(RooDataSet *a, int aa){
  const RooArgSet *set = a->get();
  RooRealVar *xAx = (RooRealVar*)set->find("pt");
  RooRealVar *eff = (RooRealVar*)set->find("efficiency");
  
  const int nbins = xAx->getBinning().numBins();
  double tx[nbins], txhi[nbins], txlo[nbins];
  double ty[nbins], tyhi[nbins], tylo[nbins];
  
  for (int i=0; i<nbins; i++) {
    a->get(i);
    ty[i] = eff->getVal();
    tx[i] = xAx->getVal();
    txhi[i] = fabs(xAx->getErrorHi());
    txlo[i] = fabs(xAx->getErrorLo());
    tyhi[i] = fabs(eff->getErrorHi());
    tylo[i] = fabs(eff->getErrorLo());
  }

  cout<<"NBins : "<<nbins<<endl;

  const double *x = tx;
  const double *xhi = txhi;
  const double *xlo = txlo;
  const double *y = ty;
  const double *yhi = tyhi;
  const double *ylo = tylo;

  TGraphAsymmErrors *b = new TGraphAsymmErrors();
  if(aa == 1) {*b = TGraphAsymmErrors(nbins,x,y,xlo,xhi,ylo,yhi);}
  if(aa == 0) {*b = TGraphAsymmErrors(nbins,x,y,0,0,ylo,yhi);}
  b->SetMaximum(1.1);
  b->SetMinimum(0.0);
  b->SetMarkerStyle(20);
  b->SetMarkerColor(kRed+2);
  b->SetMarkerSize(1.0);
  b->SetTitle("");
  b->GetXaxis()->SetTitleSize(0.05);
  b->GetYaxis()->SetTitleSize(0.05);
  b->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  b->GetYaxis()->SetTitle("Efficiency");
  b->GetXaxis()->CenterTitle();
  //b->Draw("apz");

  for (int i=0; i<nbins; i++) {
    cout << x[i] << " " << y[i] << " " << yhi[i] << " " << ylo[i] << endl;
  }





  return b;




}






