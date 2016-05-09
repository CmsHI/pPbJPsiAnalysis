#include <Riostream.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <vector>
#include <TSystem.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include "TStyle.h"
#include "TSystem.h"
#include "TPaletteAxis.h"

#include <TMath.h>
#include <math.h>

#include <sstream>
#include <string>

#include "KYOcommonOpt.h"


void draw_2D(bool isPrompt = true, bool is8rap9pt2gev = true)
{
  gROOT->Macro("./JpsiStyle2D.C");
  gStyle->SetPaintTextFormat(".3f");

  // --- read-in file
  TFile * f2D;
  char* sampleName;
  char* strPrompt;
  char* strBin;

  if (isPrompt) { strPrompt = "PRMC";}
  else { strPrompt = "NPMC";}
  if (is8rap9pt2gev) { strBin = "8rap9pt2gev";}
  else { strBin = "6rap3pt";}

//  f2D = new TFile(Form("./EffCounting_%s_%spythia_Pbp_useCtErr_1_useDataDriven_1_useZvtxStep1_0_Step2_1.root",strBin,strPrompt));
  f2D = new TFile(Form("./EffCounting_%s_%spythia_Pbp_useCtErr_1_useDataDriven_0_useZvtxStep1_0_Step2_1.root",strBin,strPrompt));
  
  sampleName = Form("%s_%s",strBin,strPrompt);
  cout << "sampleName = " << sampleName << endl;

  // --- read-in 2D hist
  TH2D* h2D_Den = (TH2D*)f2D->Get("h2D_Den_pt_y");
  TH2D* h2D_Num = (TH2D*)f2D->Get("h2D_Num_pt_y");
  TH2D* h2D_Eff = (TH2D*)f2D->Get("h2D_Eff_pt_y");

  //latex box for beam, rapidity, pT info
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  //////////////////////////////////////////////////////////////////
  // --- Draw histograms
  TCanvas* c1 = new TCanvas("c1","c1",700,600);
  TPaletteAxis* pal; 
  c1->cd();
/*
  h2D_Den->GetXaxis()->SetTitle("y_{lab}");
  h2D_Den->GetXaxis()->CenterTitle();
  h2D_Den->GetYaxis()->SetTitle("p_{T} (GeV/c)");
  h2D_Den->Draw("colz");
  c1->Update();
  pal = (TPaletteAxis*)h2D_Den->GetListOfFunctions()->FindObject("palette"); 
  pal->SetX2NDC(0.92);
  c1->Modified();
  c1->Update();
  c1->SaveAs(Form("acc2Dplots/h2D_Den_isPrompt%d.pdf",(int)isPrompt));

  h2D_Num->GetXaxis()->SetTitle("y_{lab}");
  h2D_Num->GetXaxis()->CenterTitle();
  h2D_Num->GetYaxis()->SetTitle("p_{T} (GeV/c)");
  h2D_Num->Draw("colz");
  c1->Update();
  pal = (TPaletteAxis*)h2D_Num->GetListOfFunctions()->FindObject("palette"); 
  pal->SetX2NDC(0.92);
  c1->Modified();
  c1->Update();
  c1->SaveAs(Form("acc2Dplots/h2D_Num_isPrompt%d.pdf",(int)isPrompt));
*/
  h2D_Eff->GetXaxis()->SetTitle("y_{lab}");
  h2D_Eff->GetXaxis()->CenterTitle();
  h2D_Eff->GetYaxis()->SetTitle("p_{T} (GeV/c)");
  h2D_Eff->SetMaximum(1.);
  h2D_Eff->Draw("colz");
  //h2D_Eff->SetMarkerSize(0.8);
  //h2D_Eff->Draw("text e colz");
  c1->Update();
  pal = (TPaletteAxis*)h2D_Eff->GetListOfFunctions()->FindObject("palette"); 
  pal->SetX2NDC(0.92);
  c1->Modified();
  c1->Update();
  if (is8rap9pt2gev) {
    dashedLine(-2.4,2.0,-1.97,2.0,1,4);
    dashedLine(-1.97,2.0,-1.97,3.0,1,4);
    dashedLine(-1.97,3.0,-1.37,3.0,1,4);
    dashedLine(-1.37,3.0,-1.37,6.5,1,4);
    dashedLine(-1.37,6.5,1.03,6.5,1,4);
    dashedLine(1.03,5.0,1.03,6.5,1,4);
    dashedLine(1.03,5.0,1.46,5.0,1,4);
    dashedLine(1.46,3.0,1.46,5.0,1,4);
    dashedLine(1.46,3.0,1.93,3.0,1,4);
    dashedLine(1.93,2.0,1.93,3.0,1,4);
    dashedLine(1.93,2.0,2.4,2.0,1,4);
    dashedLine(2.4,2.0,2.4,30.0,1,4);
    dashedLine(-2.4,30.0,2.4,30.0,1,4);
    dashedLine(-2.4,2.0,-2.4,30.0,1,4);
  }
  else {
    dashedLine(-2.4,5.0,-1.97,5.0,1,4);
    dashedLine(-1.97,5.0,-1.97,6.5,1,4);
    dashedLine(-1.97,6.5,1.03,6.5,1,4);
    dashedLine(1.03,5.0,1.03,6.5,1,4);
    dashedLine(1.03,5.0,1.46,5.0,1,4);
    dashedLine(1.46,5.0,1.46,30.0,1,4);
    dashedLine(-2.4,30.0,1.46,30.0,1,4);
    dashedLine(-2.4,5.0,-2.4,30.0,1,4);
  }
  c1->SaveAs(Form("eff2Dplots/h2D_Eff_%s.pdf",sampleName));

  return;

}

