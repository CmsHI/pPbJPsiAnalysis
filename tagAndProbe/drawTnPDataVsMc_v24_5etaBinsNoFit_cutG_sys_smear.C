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
#include <fstream>
#include "../commonUtility.h"

using namespace RooFit;
using namespace std;
//}}}

TGraphAsymmErrors *plotEffPt(RooDataSet *a, int aa);
void drawTnPDataVsMc_v24_5etaBinsNoFit_cutG_sys_smear()
{
	gStyle->SetOptStat(0000);

//Define file and graph{{{
	// data
	TFile *f1; 
	TFile *fRDvar[5]; 
	RooDataSet *daTrkPt1[20];
	RooDataSet *daPtRD[5][20];
	TGraphAsymmErrors *eff_pt1[20];
	TGraphAsymmErrors *eff_pt1_1[20];
	TGraphAsymmErrors *eff_ptRD[5][20];
	// mc
	TFile *f2; 
	TFile *fMCvar[5]; 
	RooDataSet *daTrkPt2[20];
	RooDataSet *daPtMC[5][20];
	TGraphAsymmErrors *eff_pt2[20];
	TGraphAsymmErrors *eff_pt2_1[20];
	TGraphAsymmErrors *eff_ptMC[5][20];

	TGraphAsymmErrors *eff_rate[20];
	TF1 *func[20];
//}}}

	TString label = "";
	int nEtaBin = 5;  

	TString varname[5] = {"narrow", "2G", "pol1", "Multi", "PAMu7"};
	FILE* ftxt;
	ftxt = fopen("SingleMuon_error_v24_smear_v3.txt", "w");
	fprintf(ftxt, Form("pt range \t \t %s \t \t %s \t \t \t %s \t \t %s \t \t %s \t \t stat. \t \t total \n", varname[0].Data(), varname[1].Data(), varname[2].Data(), varname[3].Data(), varname[4].Data()));

//Get graph{{{

//nominal{{{
	f1 = new TFile("/home/kisoo/work/pPb_Jpsi/tagAndProbe/DataFile/outputEveryCut_data_allDirection_v24_nominalCond_5etaBins_cutG_all_nominal_v3.root");
	for ( int ieta = 1; ieta<=nEtaBin ; ieta++)
	{
		daTrkPt1[ieta] = (RooDataSet*)f1->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
	}
	f2 = new TFile("/home/kisoo/work/pPb_Jpsi/tagAndProbe/DataFile/outputEveryCut_mc_allDirection_v24_nominalCond_5etaBins_cutG_all_nominal_v3.root");
	for ( int ieta = 1; ieta<=nEtaBin ; ieta++)
	{
		daTrkPt2[ieta] = (RooDataSet*)f2->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
	}
	label = "MuIdAndTrig";
//}}}

//variation{{{
	for(Int_t ivar = 0; ivar < 5; ivar++)
	{
		fRDvar[ivar] = new TFile(Form("/home/kisoo/work/pPb_Jpsi/tagAndProbe/DataFile/outputEveryCut_data_allDirection_v24_nominalCond_5etaBins_cutG_all_%s_v3.root", varname[ivar].Data()));
		for ( int ieta = 1; ieta<=nEtaBin ; ieta++)
		{
			daPtRD[ivar][ieta] = (RooDataSet*)fRDvar[ivar]->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
		}
		fMCvar[ivar] = new TFile(Form("/home/kisoo/work/pPb_Jpsi/tagAndProbe/DataFile/outputEveryCut_mc_allDirection_v24_nominalCond_5etaBins_cutG_all_%s_v3.root", varname[ivar].Data()));
		for ( int ieta = 1; ieta<=nEtaBin ; ieta++)
		{
			daPtMC[ivar][ieta] = (RooDataSet*)fMCvar[ivar]->Get(Form("tpTree/ptBin_eta%d/fit_eff",ieta));
		}
	}
//}}}

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
//eta1{{{
		{
			nPtBins = sizeof(ptBinsArr1)/sizeof(double)-1;
			for(Int_t i = 0; i < nPtBins+1; i++)
			{
				ptBinsArr[i] = ptBinsArr1[i];
			}
		}
//}}}
		else if(ieta == 2)
//eta2{{{
		{
			nPtBins = sizeof(ptBinsArr2)/sizeof(double)-1;
			for(Int_t i = 0; i < nPtBins+1; i++)
			{
				ptBinsArr[i] = ptBinsArr2[i];
			}
		}
//}}}
		else if (ieta == 3) 
//eta3{{{
		{
			nPtBins = sizeof(ptBinsArr3)/sizeof(double)-1;
			for(Int_t i = 0; i < nPtBins+1; i++)
			{
				ptBinsArr[i] = ptBinsArr3[i];
			}
		}
//}}}
		else if (ieta == 4) 
//eta4{{{
		{
			nPtBins = sizeof(ptBinsArr4)/sizeof(double)-1;
			for(Int_t i = 0; i < nPtBins+1; i++)
			{
				ptBinsArr[i] = ptBinsArr4[i];
			}
		}
//}}}
		else if (ieta == 5) 
//eta5{{{
		{
			nPtBins = sizeof(ptBinsArr5)/sizeof(double)-1;
			for(Int_t i = 0; i < nPtBins+1; i++)
			{
				ptBinsArr[i] = ptBinsArr5[i];
			}
		}
//}}}
//}}}

//Get & define graph{{{

//nominal{{{
		eff_pt1[ieta] = plotEffPt(daTrkPt1[ieta], 1);
		eff_pt1[ieta]->SetName(Form("eff_data_%s_etaBin%d",label.Data(),ieta));
		eff_pt2[ieta] = plotEffPt(daTrkPt2[ieta], 1);
		eff_pt2[ieta]->SetName(Form("eff_mc_%s_etaBin%d",label.Data(),ieta));
		eff_pt1_1[ieta] =  new TGraphAsymmErrors();
		eff_pt2_1[ieta] =  new TGraphAsymmErrors();
		eff_rate[ieta] =  new TGraphAsymmErrors();
		eff_rate[ieta]->SetName(Form("eff_ratio_%s_etaBin%d",label.Data(),ieta));
//}}}

//variation{{{
		for(Int_t ivar = 0; ivar < 5; ivar++)
		{
			eff_ptRD[ivar][ieta] = plotEffPt(daPtRD[ivar][ieta], 1);
			eff_ptRD[ivar][ieta]->SetName(Form("eff_data_%s_etaBin%d",label.Data(),ieta));

			eff_ptMC[ivar][ieta] = plotEffPt(daPtMC[ivar][ieta], 1);
			eff_ptMC[ivar][ieta]->SetName(Form("eff_mc_%s_etaBin%d",label.Data(),ieta));
		}
//}}}

//}}}

		fprintf(ftxt, Form("etabin%d \n", ieta));

		for ( int ip = 0 ; ip< eff_pt1[ieta]->GetN(); ip++)
		{
//Get value{{{
			double xRD, yRD, xRD1, xRD2, xRD3, xRD4, xRD5, yRD1, yRD2, yRD3, yRD4, yRD5;
			double xMC, yMC, xMC1, xMC2, xMC3, xMC4, xMC5, yMC1, yMC2, yMC3, yMC4, yMC5;
			eff_pt1[ieta]->GetPoint(ip, xRD, yRD);
			eff_pt2[ieta]->GetPoint(ip, xMC, yMC);
			eff_ptRD[0][ieta]->GetPoint(ip, xRD1, yRD1);
			eff_ptMC[0][ieta]->GetPoint(ip, xMC1, yMC1);
			eff_ptRD[1][ieta]->GetPoint(ip, xRD2, yRD2);
			eff_ptMC[1][ieta]->GetPoint(ip, xMC2, yMC2);
			eff_ptRD[2][ieta]->GetPoint(ip, xRD3, yRD3);
			eff_ptMC[2][ieta]->GetPoint(ip, xMC3, yMC3);
			eff_ptRD[3][ieta]->GetPoint(ip, xRD4, yRD4);
			eff_ptMC[3][ieta]->GetPoint(ip, xMC4, yMC4);
			eff_ptRD[4][ieta]->GetPoint(ip, xRD5, yRD5);
			eff_ptMC[4][ieta]->GetPoint(ip, xMC5, yMC5);

//			cout << "etabin: " << ieta << ", ip: " << ip << ", RD: (" << xRD << ", " << yRD << "), MC: (" << xMC << ", " << yMC << ")" << ", RD1: (" << xRD1 << ", " << yRD1 << "), MC1: (" << xMC1 << ", " << yMC1 << "), RD2: (" << xRD2 << ", " << yRD2 << "), MC2: (" << xMC2 << ", " << yMC2 << "), RD3: (" << xRD3 << ", " << yRD3 << "), MC3: (" << xMC3 << ", " << yMC3 << "), RD: (" << xRD4 << ", " << yRD4 << "), MC4: (" << xMC4 << ", " << yMC4 << ")" << endl;
//}}}

//Get error{{{ 
			double yerr1  = eff_pt1[ieta]->GetErrorY(ip);
			double yerr2  = eff_pt2[ieta]->GetErrorY(ip);

			cout << "etabin: " << ieta << ", ip: " << ip << ", RD: (" << xRD << ", " << yRD << ", " << yerr1 << "), MC: (" << xMC << ", " << yMC << ", " << yerr2 << ")" << endl;

			if(ieta == 2 && ip == 8) yerr2 = yerr2/4;
			if(ieta == 3)
			{
				if(ip == 2) yerr1 = yerr1/6;
				if(ip == 5) yerr1 = yerr1/1.1;
				if(ip == 6) yerr1 = yerr1/4;
				if(ip == 9) yerr1 = yerr1/1.4;
			}
			if(ieta == 4)
			{
				if(ip == 4)
				{
					yerr1 = yerr1/10;
					yerr2 = yerr2/100;
				}
				if(ip == 5) yerr1 = yerr1/2;
				if(ip == 7) yerr2 = yerr1/10;
			}
			if(ieta == 5)
			{
				if(ip == 0) yerr1 = yerr1/1.1;
				if(ip == 1) yerr1 = yerr1/1.5;
				if(ip == 5) yerr2 = yerr2/2;
			}

			double error = yRD/yMC * sqrt( (yerr1*yerr1)/(yRD*yRD) + (yerr2*yerr2)/( yMC*yMC) );

			double diff1 = (yRD/yMC-yRD1/yMC1);
			double diff2 = (yRD/yMC-yRD2/yMC2);
			double diff3 = (yRD/yMC-yRD3/yMC3);
			double diff4 = (yRD/yMC-yRD4/yMC4);
			double diff5 = (yRD/yMC-yRD5/yMC5);

			double new_error = sqrt(error*error+diff1*diff1+diff2*diff2+diff3*diff3+diff4*diff4+diff5*diff5);

			fprintf(ftxt, Form("& %.1f & %.1f \t & %.4f \t & %.4f \t & %.4f \t & %.4f \t & %.4f \t & %.4f \t & %.4f \n", ptBinsArr[ip], ptBinsArr[ip+1], abs(diff1), abs(diff2), abs(diff3), abs(diff4), abs(diff5), error, new_error));
//}}}

//Fill value{{{
			eff_rate[ieta]->SetPoint(ip, xRD, yRD/yMC);
			eff_rate[ieta]->SetPointError(ip, 0, 0, new_error,new_error);

			double err_pt1 = eff_pt1[ieta]->GetErrorY(ip);
			double err_pt2 = eff_pt2[ieta]->GetErrorY(ip);
			eff_pt1_1[ieta]->SetPoint(ip, xRD, yRD);
			eff_pt2_1[ieta]->SetPoint(ip, xMC, yMC);
			eff_pt1_1[ieta]->SetPointError(ip, 0., 0., err_pt1, err_pt1);
			eff_pt2_1[ieta]->SetPointError(ip, 0., 0., err_pt2, err_pt2);
//}}}
		}

//Define fit function{{{
		func[ieta] = new TF1(Form("func_%d", ieta), "[0]/(TMath::Exp((-x+[1])/[2]+1)+[3])+[4]", 0, 20);
		func[ieta]->SetLineColor(kRed);
		func[ieta]->SetLineWidth(2.5);
		func[ieta]->SetParameters(-4.795, 1.653, 0.923, 3.018, 2.369);
//}}}
	}

//Draw{{{
	TH1F* tempTH1f = new TH1F("Graph_hxy_fit_eff1",";p_{T} (GeV/c);Efficiency",100,0,12);
	handsomeTH1(tempTH1f,1);
	TCanvas*c1 = new TCanvas("c1","",300*nEtaBin,700);
	c1->Divide(nEtaBin,2,0.0,0.0);
	
	for ( int ieta = 1; ieta<=nEtaBin ; ieta++)
	{
//Draw RD & MC{{{
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
	    
	   if(ieta == 1)    drawText(  "     |#eta|<0.6",0.5,0.3);
	   else if (ieta == 2)  drawText("0.6 <|#eta|<1.3",0.5,0.3);
	   else if (ieta == 3)  drawText("1.3 <|#eta|<1.8",0.5,0.3);
	   else if (ieta == 4)  drawText("1.8 <|#eta|<2.2",0.5,0.3);
		else if (ieta == 5)  drawText("2.2 <|#eta|<2.4",0.5,0.3);
	    
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

//Draw ratio{{{
	   c1->cd(ieta+nEtaBin);
	   tempTH1f->SetAxisRange(0.6,2.0,"Y");
	   tempTH1f->SetYTitle("Data/MC Ratio");
	   tempTH1f->SetMarkerColor(0);
	   tempTH1f->SetLineColor(0);
	   tempTH1f->DrawCopy(); 
	    
	   eff_rate[ieta]->SetMarkerStyle(20);
	   eff_rate[ieta]->SetMarkerSize(1.2);
	   eff_rate[ieta]->Draw("p");
	   eff_rate[ieta]->Fit(Form("func_%d", ieta));
	   jumSun(0,1,20,1);
		//onSun(ptMin[ieta-1],0.6,ptMin[ieta-1],2.0, 4,0.8);
//}}}
	}
//}}}

//Save{{{
	c1->Update();
	c1->SaveAs("tnpSummaryPlot_v24_5variation_smeared_v3.pdf");
	TFile * output = new TFile("Merged_error_v24_ratio_v3.root","recreate");
   for ( int ieta = 1; ieta<=nEtaBin ; ieta++)
	{
		eff_pt1[ieta]->Write();
      eff_pt2[ieta]->Write();
	   eff_rate[ieta]->Write();
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
