#ifndef COMMONOPT_Songkyo_H
#define COMMONOPT_Songkyo_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

#include "TFile.h"
#include <TROOT.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TLegend.h>
#include <TMath.h>
#include <TPaveText.h>
#include <TText.h>
#include <TLatex.h>


////// calculation with error propagation

void DivideValue(Double_t num, Double_t numErr, Double_t den, Double_t denErr, Double_t* frac, Double_t* fracErr){
	*frac = num/den;
	*fracErr = (*frac) * TMath::Sqrt( TMath::Power(numErr/num,2) + TMath::Power(denErr/den,2) );
}

void MultiplyValue(Double_t a, Double_t aErr, Double_t b, Double_t bErr, Double_t* res, Double_t* resErr){
	*res = a*b;
	*resErr = (*res) * TMath::Sqrt( TMath::Power(aErr/a,2) + TMath::Power(bErr/b,2) );
}

////// draw lines

void dashedLine(Double_t x1=0,Double_t y1=0,Double_t x2=1,Double_t y2=1,Int_t color=1, Double_t width=1)
{
   TLine* t1 = new TLine(x1,y1,x2,y2);
   t1->SetLineWidth(width);
   t1->SetLineStyle(7);
   t1->SetLineColor(color);
   t1->Draw();
}

void solidLine(Double_t x1=0,Double_t y1=0,Double_t x2=1,Double_t y2=1,Int_t color=1, Double_t width=1)
{
  TLine* t1 = new TLine(x1,y1,x2,y2);
  t1->SetLineWidth(width);
  t1->SetLineStyle(1);
  t1->SetLineColor(color);
  t1->Draw();
}

////// SetStyle

void SetHistStyle(TH1* h, Int_t c, Int_t m) {
	Int_t colorArr[] = { kGray+3, kRed-4, kBlue, kOrange+7, kGreen+3, kAzure+9, kViolet-4, kGreen+1,kBlack };
	Int_t markerFullArr[] = {kFullCircle, kFullTriangleUp, kFullTriangleDown, kFullSquare, kFullStar, kFullDiamond};
	Int_t markerOpenArr[] = {kOpenCircle, kOpenTriangleUp, kOpenTriangleDown, kOpenSquare, kOpenStar, kOpenDiamond};
	h-> SetMarkerColor(colorArr[c]);
	if(m<10) h-> SetMarkerStyle(markerFullArr[m]);
	else h-> SetMarkerStyle(markerOpenArr[m-10]);
	h-> SetMarkerSize(1.2);
	h-> SetLineColor(colorArr[c]);
	h-> SetLineWidth(1.8);
}

void SetGraphStyle(TGraph* gr, Int_t c, Int_t m) {
	Int_t colorArr[] = { kGray+3, kRed-4, kBlue, kOrange+7, kGreen+3, kViolet-4, kAzure+8, kGreen+1, kBlack };
	Int_t markerFullArr[] = {kFullCircle, kFullTriangleUp, kFullTriangleDown, kFullSquare, kFullStar, kFullDiamond};
	Int_t markerOpenArr[] = {kOpenCircle, kOpenTriangleUp, kOpenTriangleDown, kOpenSquare, kOpenStar, kOpenDiamond};

	gr-> SetMarkerColor(colorArr[c]);
	if(m<10) gr-> SetMarkerStyle(markerFullArr[m]);
	else gr-> SetMarkerStyle(markerOpenArr[m-10]);
	gr-> SetMarkerSize(1.2);
	gr-> SetLineColor(colorArr[c]);
	gr-> SetLineWidth(0.8);
}

void SetLegendStyle(TLegend* l) {
	l->SetFillColor(0);
	l->SetFillStyle(4000);
	l->SetBorderSize(0);
	l->SetMargin(0.2);
	l->SetTextSize(0.040);
}

void SetTextStyle(TPaveText* t) {
	t->SetFillColor(0);
	t->SetFillStyle(4000);
	t->SetBorderSize(0);
	t->SetMargin(0.2);
	t->SetTextSize(0.035);
}


#endif
