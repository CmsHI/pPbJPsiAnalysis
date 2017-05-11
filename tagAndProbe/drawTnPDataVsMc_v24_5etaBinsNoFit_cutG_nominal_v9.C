//Headers{{{
#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TDirectory.h>
#include <TCollection.h>
#include <TPostScript.h>

#include <TH1F.h>
#include <TH2D.h>
#include <TGraphAsymmErrors.h>
#include <TLine.h>
#include <TLatex.h>

#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TRandom.h>
#include <TCut.h>
#include <TRandom3.h>

#include <RooFit.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooArgSet.h>

#include <iostream>
#include "../../commonUtility.h"

using namespace RooFit;
using namespace std;
//}}}

TGraphAsymmErrors *plotEffPt(RooDataSet *a, int aa);
TGraphAsymmErrors *plotEffEta(RooDataSet *a, int aa);
void drawTnPDataVsMc_v24_5etaBinsNoFit_cutG_nominal_v9()
{
	gStyle->SetOptStat(0000);

//Define file and graph{{{
	// data
	TFile *f1; 
	RooDataSet *daTrkEta1[20];
	TGraphAsymmErrors *eff_eta1[20];//graph obtained from input
	TGraphAsymmErrors *eff_eta1_1;//refilled graph
	// mc
	TFile *f2; 
	RooDataSet *daTrkEta2[20];
	TGraphAsymmErrors *eff_eta2[20];//graph obtained from input
	TGraphAsymmErrors *eff_eta2_1;//refilled graph

	TGraphAsymmErrors *eff_rate;//efficiency ratio draw to canvas
	//TF1* func[20];
//}}}

	TString label = "";
	int NOetaB = 0;
	Double_t etaBinsArr[] = {-2.4, -2.2, -2.1, -1.8, -1.6, -1.3, -1.2, -0.9, -0.6, 0.0, 0.6, 0.9, 1.2, 1.3, 1.6, 1.8, 2.1, 2.2, 2.4};
	NOetaB = sizeof(etaBinsArr)/sizeof(Double_t) -1;

//Get graph{{{
	f1 = new TFile("/home/kisoo/work/pPb_Jpsi/tagAndProbe/DataFile/outputEveryCut_data_allDirection_v24_nominalCond_5etaBins_cutG_all_nominal_v9.root");
	f2 = new TFile("/home/kisoo/work/pPb_Jpsi/tagAndProbe/DataFile/outputEveryCut_mc_allDirection_v24_nominalCond_5etaBins_cutG_all_nominal_v9.root");
	for(Int_t ieta = 0; ieta < NOetaB; ieta++)
	{
		daTrkEta1[ieta] = (RooDataSet*)f1->Get(Form("tpTree/etaBin%d/fit_eff",ieta));
		daTrkEta2[ieta] = (RooDataSet*)f2->Get(Form("tpTree/etaBin%d/fit_eff",ieta));
	}
	label = "MuIdAndTrig";
//}}}

	eff_eta1_1 = new TGraphAsymmErrors();
	eff_eta2_1 = new TGraphAsymmErrors();
	eff_rate = new TGraphAsymmErrors();
	eff_rate->SetName(Form("eff_ratio_%s",label.Data()));

	for(Int_t ieta = 0; ieta < NOetaB; ieta++)
	{

//Define graph{{{
		eff_eta1[ieta] = plotEffEta(daTrkEta1[ieta], 1);
		eff_eta1[ieta]->SetName(Form("eff_data_%s_etaBin%d",label.Data(),ieta));
		eff_eta2[ieta] = plotEffEta(daTrkEta2[ieta], 1);
		eff_eta2[ieta]->SetName(Form("eff_mc_%s_etaBin%d",label.Data(),ieta));
//}}}

//Fill in graph{{{
		double x1, x2, y1, y2;
		eff_eta1[ieta]->GetPoint(0, x1, y1);
		eff_eta2[ieta]->GetPoint(0, x2, y2);

		double yerr1 = eff_eta1[ieta]->GetErrorY(0);
		double yerr2 = eff_eta2[ieta]->GetErrorY(0);

		cout << "etabin: " << ieta << ", RD: (" << x1 << ", " << y1 << ", " << yerr1 << "), MC: (" << x2 << ", " << y2 << ", " << yerr2 << ")" << endl;

		double error = y1/y2 * sqrt( (yerr1*yerr1)/(y1*y1) + (yerr2*yerr2)/(y2*y2) );
		//double error = sqrt( (yerr1*yerr1)/(y1*y1) + (yerr2*yerr2)/(y2*y2) );
		eff_rate->SetPoint(ieta, x1, y1/y2);
		eff_rate->SetPointError(ieta, 0, 0, error,error);

		eff_eta1_1->SetPoint(ieta, x1, y1);
		eff_eta2_1->SetPoint(ieta, x2, y2);
		eff_eta1_1->SetPointError(ieta, 0., 0., yerr1, yerr1);
		eff_eta2_1->SetPointError(ieta, 0., 0., yerr2, yerr2);
//}}}

	}
/*
	if (ieta == 1) func[ieta] = new TF1(Form("func_%d", ieta), "[0]/(TMath::Exp((-x+[1])/[2]+1)+[3])+[4]", ptMin[0], 20);
	func[ieta]->SetLineColor(kRed);
	func[ieta]->SetLineWidth(2.5);
	func[ieta]->SetParameters(-4.795, 1.653, 0.923, 3.018, 2.369);
*/

//Draw{{{
	TH1F* tempTH1f = new TH1F("Graph_hxy_fit_eff1",";#eta;Efficiency",20,-2.4,2.4);
	handsomeTH1(tempTH1f,1);
	TCanvas*c1 = new TCanvas("c1","",300,600);
	c1->Divide(1,2,0.0,0.0);

//efficiency{{{
	c1->cd(1);
	tempTH1f->SetAxisRange(0.0,1.2,"Y");
	tempTH1f->DrawCopy();
	handsomeTGraph(eff_eta1_1,2);
	handsomeTGraph(eff_eta2_1,1);
	eff_eta1_1->SetMarkerStyle(20);
	eff_eta2_1->SetMarkerStyle(20);
	eff_eta1_1->SetMarkerSize(1.2);
	eff_eta2_1->SetMarkerSize(1.2);
	eff_eta1_1->Draw("p");
	eff_eta2_1->Draw("p");

	drawText(  "     tag p_{T}>5.0",0.5,0.15);
	TLegend *leg = new TLegend(0.5562018,0.25,0.9975074,0.4,NULL,"brNDC");
	leg->AddEntry(eff_eta1_1, "Data","p");
	leg->AddEntry(eff_eta2_1, "MC","p");
	leg->Draw();
	jumSun(-2.4,1,2.4,1);
	//onSun(ptMin[ieta-1],0.,ptMin[ieta-1],1.2, 4,0.8);
//}}}

//ratio{{{
	c1->cd(2);
	tempTH1f->SetAxisRange(0.6,2.0,"Y");
	tempTH1f->SetYTitle("Data/MC Ratio");
	tempTH1f->SetMarkerColor(0);
	tempTH1f->SetLineColor(0);
	tempTH1f->DrawCopy(); 

	eff_rate->SetMarkerStyle(20);
	eff_rate->SetMarkerSize(1.2);
	eff_rate->Draw("p");
	jumSun(-2.4,1,2.4,1);
	//onSun(ptMin[ieta-1],0.6,ptMin[ieta-1],2.0, 4,0.8);
//}}}

//}}}

//save{{{
   c1->Update();
   c1->SaveAs("tnpSummaryPlot_nominpt_v24_5eta_cutG_all_nominal_v9.pdf");
	TFile * output = new TFile("output_v24_5eta_cutG_all_nominal_v9.root","recreate");
	eff_eta1_1->Write();
	eff_eta2_1->Write();
	eff_rate->Write();
	//func->Write();

	output->Close();
//}}}
}

//external functions{{{
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
//cout << txhi[i] << " , " << txlo[i] << " , " << tyhi[i] << " , " << tylo[i] << " , " << endl;
  }

  //cout<<"NBins : "<<nbins<<endl;

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

TGraphAsymmErrors *plotEffEta(RooDataSet *a, int aa){
    const RooArgSet *set = a->get();
    RooRealVar *xAx = (RooRealVar*)set->find("eta");
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
    b->GetXaxis()->SetTitle("#eta");
    b->GetYaxis()->SetTitle("Efficiency");
    b->GetXaxis()->CenterTitle();
    //b->Draw("apz");

    for (int i=0; i<nbins; i++) {
        cout << x[i] << " " << y[i] << " " << yhi[i] << " " << ylo[i] << endl;
    }

    return b;
}

//}}}
