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
#include "../commonUtility.h"

using namespace RooFit;
using namespace std;
//}}}

TGraphAsymmErrors *plotEffPt(RooDataSet *a, int aa);
void drawTnPDataVsMc_v24_5etaBinsNoFit_cutG_Multi()
{
	gStyle->SetOptStat(0000);

//Define file and graph{{{
	// data
	TFile *f1; 
	RooDataSet *daTrkPt1[20];
	TGraphAsymmErrors *eff_pt1[20];
	TGraphAsymmErrors *eff_pt1_1[20];
	// mc
	TFile *f2; 
	RooDataSet *daTrkPt2[20];
	TGraphAsymmErrors *eff_pt2[20];
	TGraphAsymmErrors *eff_pt2_1[20];

	TGraphAsymmErrors *eff_rate[20];
	TF1* func[20];
//}}}

	TString label = "";
	int nEtaBin = 5;  
	double ptMin[5] = {3.3, 3.3, 2.16, 1.6, 1.3};

//Get graph{{{
	f1 = new TFile("/home/kisoo/work/pPb_Jpsi/tagAndProbe/DataFile/outputEveryCut_data_allDirection_v24_nominalCond_5etaBins_cutG_all_Multi_v3.root");
	for ( int ieta = 1; ieta<=nEtaBin ; ieta++)
	{
		daTrkPt1[ieta] = (RooDataSet*)f1->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
	}
	f2 = new TFile("/home/kisoo/work/pPb_Jpsi/tagAndProbe/DataFile/outputEveryCut_mc_allDirection_v24_nominalCond_5etaBins_cutG_all_Multi_v3.root");
  for ( int ieta = 1; ieta<=nEtaBin ; ieta++)
	{
		daTrkPt2[ieta] = (RooDataSet*)f2->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
	}
	label = "MuIdAndTrig";
//}}}

//Define binning{{{
	Double_t ptBinsArr1[10] = {3.3, 3.8, 4.3, 5, 6, 7, 8, 10, 13, 30};
	Double_t ptBinsArr2[11] = {3.3, 3.5, 3.9, 4.3, 5, 6, 7, 8, 10, 13, 30};
	Double_t ptBinsArr3[11] = {2.15, 3.0, 3.6, 4.3, 5, 6, 7, 8, 10, 13, 30};
	Double_t ptBinsArr4[11] = {1.6, 2.2, 2.5, 2.8, 3.5, 4.5, 5, 6, 10, 12, 30};
	Double_t ptBinsArr5[8] = {1.3, 1.6, 2.2, 3.4, 4.8, 6, 9, 30};
	Double_t ptBinsArr[100] = {};
	Int_t nPtBins;
//}}}

	for ( int ieta = 1; ieta<=nEtaBin ; ieta++)
	{
//Set binning{{{
		if(ieta == 1)
		{
			nPtBins = sizeof(ptBinsArr1)/sizeof(double)-1;
			for(Int_t i = 0; i < nPtBins+1; i++)
			{
				ptBinsArr[i] = ptBinsArr1[i];
			}
		}
		else if(ieta == 2)
		{
			nPtBins = sizeof(ptBinsArr2)/sizeof(double)-1;
			for(Int_t i = 0; i < nPtBins+1; i++)
			{
				ptBinsArr[i] = ptBinsArr2[i];
			}
		}
		else if (ieta == 3) 
		{
			nPtBins = sizeof(ptBinsArr3)/sizeof(double)-1;
			for(Int_t i = 0; i < nPtBins+1; i++)
			{
				ptBinsArr[i] = ptBinsArr3[i];
			}
		}
		else if (ieta == 4) 
		{
			nPtBins = sizeof(ptBinsArr4)/sizeof(double)-1;
			for(Int_t i = 0; i < nPtBins+1; i++)
			{
				ptBinsArr[i] = ptBinsArr4[i];
			}
		}
		else if (ieta == 5) 
		{
			nPtBins = sizeof(ptBinsArr5)/sizeof(double)-1;
			for(Int_t i = 0; i < nPtBins+1; i++)
			{
				ptBinsArr[i] = ptBinsArr5[i];
			}
		}
//}}}

//Define graph{{{
		eff_pt1[ieta] = plotEffPt(daTrkPt1[ieta], 1);
		eff_pt1[ieta]->SetName(Form("eff_data_%s_etaBin%d",label.Data(),ieta));
		eff_pt2[ieta] = plotEffPt(daTrkPt2[ieta], 1);
		eff_pt2[ieta]->SetName(Form("eff_mc_%s_etaBin%d",label.Data(),ieta));
		eff_pt1_1[ieta] =  new TGraphAsymmErrors();
		eff_pt2_1[ieta] =  new TGraphAsymmErrors();
		eff_rate[ieta] =  new TGraphAsymmErrors();
		eff_rate[ieta]->SetName(Form("eff_ratio_%s_etaBin%d",label.Data(),ieta));
//}}}

//Fill in graph{{{
		for ( int ip = 0 ; ip< eff_pt1[ieta]->GetN(); ip++)
		{
			double x1, x2, y1, y2;
			eff_pt1[ieta]->GetPoint(ip, x1, y1);
			eff_pt2[ieta]->GetPoint(ip, x2, y2);
 
			double yerr1  = eff_pt1[ieta]->GetErrorY(ip);
			double yerr2  = eff_pt2[ieta]->GetErrorY(ip);
/*
			if(ieta == 3 && ip == 7) yerr2 = yerr2/2;
			if(ieta == 4 && ip == 6) yerr2 = yerr2/2;
			if(ieta == 5 && ip == 1) yerr1 = yerr1/2;
			if(ieta == 5 && ip == 5) yerr2 = yerr2/10;
*/
			cout << "etabin: " << ieta << ", ip: " << ip << ", RD: (" << x1 << ", " << y1 << ", " << yerr1 << "), MC: (" << x2 << ", " << y2 << ", " << yerr2 << ")" << endl;

			double error = y1/y2 * sqrt( (yerr1*yerr1)/(y1*y1) + (yerr2*yerr2)/(y2*y2) );
			eff_rate[ieta]->SetPoint(ip, x1, y1/y2);
			eff_rate[ieta]->SetPointError(ip, 0, 0, error,error);

			eff_pt1_1[ieta]->SetPoint(ip, x1, y1);
			eff_pt2_1[ieta]->SetPoint(ip, x2, y2);
			eff_pt1_1[ieta]->SetPointError(ip, 0., 0., yerr1, yerr1);
			eff_pt2_1[ieta]->SetPointError(ip, 0., 0., yerr2, yerr2);
		}
//}}}

		func[ieta] = new TF1(Form("func_%d", ieta), "[0]/(TMath::Exp((-x+[1])/[2]+1)+[3])+[4]", 0, 20);
		func[ieta]->SetLineColor(kRed);
		func[ieta]->SetLineWidth(2.5);
		func[ieta]->SetParameters(-4.795, 1.653, 0.923, 3.018, 2.369);
	}

//Draw{{{
	TH1F* tempTH1f = new TH1F("Graph_hxy_fit_eff1",";p_{T} (GeV/c);Efficiency",100,0,12);
	handsomeTH1(tempTH1f,1);
	TCanvas*c1 = new TCanvas("c1","",300*nEtaBin,700);
	c1->Divide(nEtaBin,2,0.0,0.0);

	for ( int ieta = 1; ieta<=nEtaBin ; ieta++)
	{
//efficiency{{{
		c1->cd(ieta);
		tempTH1f->SetAxisRange(0.0,1.2,"Y");
		tempTH1f->DrawCopy();
		handsomeTGraph(eff_pt1_1[ieta],2);
		handsomeTGraph(eff_pt2_1[ieta],1);
		eff_pt1_1[ieta]->SetMarkerStyle(20);
		eff_pt2_1[ieta]->SetMarkerStyle(20);
		eff_pt1_1[ieta]->SetMarkerSize(1.2);
		eff_pt2_1[ieta]->SetMarkerSize(1.2);
		eff_pt1_1[ieta]->Draw("p");
		eff_pt2_1[ieta]->Draw("p");
	    
		if(ieta == 1)    drawText(  "     |#eta|<0.9",0.5,0.3);
		else if (ieta == 2)  drawText("0.9 <|#eta|<1.2",0.5,0.3);
		else if (ieta == 3)  drawText("1.2 <|#eta|<1.6",0.5,0.3);
		else if (ieta == 4)  drawText("1.6 <|#eta|<2.1",0.5,0.3);
		else if (ieta == 5)  drawText("2.1 <|#eta|<2.4",0.5,0.3);
	    
		if (ieta == 1)
		{
			TLegend *leg = new TLegend(0.5562018,0.3933851,0.9975074,0.6073602,NULL,"brNDC");
			leg->AddEntry(eff_pt1_1[ieta], "Data","p");
			leg->AddEntry(eff_pt2_1[ieta], "MC","p");
			leg->Draw();
		}
		jumSun(0,1,20,1);
		//onSun(ptMin[ieta-1],0.,ptMin[ieta-1],1.2, 4,0.8);
//}}}

//ratio{{{
		c1->cd(ieta+nEtaBin);
		tempTH1f->SetAxisRange(0.6,2.0,"Y");
		tempTH1f->SetYTitle("Data/MC Ratio");
		tempTH1f->SetMarkerColor(0);
		tempTH1f->SetLineColor(0);
		tempTH1f->DrawCopy(); 
	    
		eff_rate[ieta]->SetMarkerStyle(20);
		eff_rate[ieta]->SetMarkerSize(1.2);
		eff_rate[ieta]->Draw("p");
		if(ieta == 4)
		{
			func[ieta]->SetParLimits(0, -5, -4);
			func[ieta]->SetParLimits(1, 1, 2);
			func[ieta]->SetParLimits(2, 0, 0.5);
			func[ieta]->SetParLimits(3, 2.5, 3.5);
			func[ieta]->SetParLimits(4, 2, 3);
		}
		eff_rate[ieta]->Fit(Form("func_%d", ieta));
		jumSun(0,1,20,1);
/*
		if(ieta == 1) cout << "lowest pt value eta" << ieta << ": " << func[ieta]->Eval(3.3) << endl;
		if(ieta == 2) cout << "lowest pt value eta" << ieta << ": " << func[ieta]->Eval(3.3) << endl;
		if(ieta == 3) cout << "lowest pt value eta" << ieta << ": " << func[ieta]->Eval(2.16) << endl;
		if(ieta == 4) cout << "lowest pt value eta" << ieta << ": " << func[ieta]->Eval(1.6) << endl;
		if(ieta == 5) cout << "lowest pt value eta" << ieta << ": " << func[ieta]->Eval(1.3) << endl;
*/
		onSun(ptMin[ieta-1],0.6,ptMin[ieta-1],2.0, 4,0.8);
//}}}
	}
//}}}

//save{{{
   c1->Update();
   c1->SaveAs("tnpSummaryPlot_nominpt_v24_5eta_cutG_all_Multi_v3_noErrorfix.pdf");
	TFile * output = new TFile("output_v24_5eta_cutG_all_Multi_v3_noErrorfix.root","recreate");
	for ( int ieta = 1; ieta<=nEtaBin ; ieta++)
	{
		eff_pt1[ieta]->Write();
		eff_pt2[ieta]->Write();
		eff_rate[ieta]->Write();
		func[ieta]->Write();
	}

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
cout << txhi[i] << " , " << txlo[i] << " , " << tyhi[i] << " , " << tylo[i] << " , " << endl;
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
//}}}
