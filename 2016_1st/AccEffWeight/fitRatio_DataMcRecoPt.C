#include <iostream>
#include <iomanip>
#include <TF1.h>
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TLatex.h>
#include <TRandom.h>
#include "TFitResult.h"
#include <ctime>
#include <string>
#include <sstream>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>

//#include "KYOcommonOptMulti.h"
#include "KYOcommonOpt.h"

Double_t ptArrNum[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
const Int_t nPt = sizeof(ptArrNum)/sizeof(Double_t)-1;

Double_t rapArrNumFB[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
//Double_t rapArrNumBF[] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
const Int_t nRap = sizeof(rapArrNumFB)/sizeof(Double_t)-1;

double fitHevi(double *x, double *par);
double fitExp(double *x, double *par);
double fitStraight(double *x, double *par);
void formRapArr(Double_t binmin, Double_t binmax, string* arr);

void fitRatio_DataMcRecoPt(bool isPrompt=true, bool is1st=true, bool isStiff=true,char* dirName="8rap9pt"){
    gRandom->SetSeed(time(0));
		gROOT->Macro("./JpsiStyle.C");

		//latex box for beam, rapidity, pT info
		TLatex* latex = new TLatex();
		latex->SetNDC();
		latex->SetTextAlign(12);
		latex->SetTextSize(0.04);

		string rapArr[nRap];
		for (Int_t iy=0; iy<nRap; iy++) {
			formRapArr(rapArrNumFB[iy+1], rapArrNumFB[iy], &rapArr[iy]);
			cout << iy <<"th rapArr = " << rapArr[iy] << endl;
		}

    TFile* fin = new TFile(Form("./DataMcRecoPt_%s/DataMcRecoPt_isPrompt%d.root",dirName,(int)isPrompt));//in KNU  //KYO
    string strPr;
    string strRun;
    string strRunOverlay;
		if (isPrompt) { strPr = "PR";}
		else {strPr = "NP"; }
    if(is1st) {strRun="Pbp"; strRunOverlay="pPb";}
    else {strRun="pPb"; strRunOverlay="Pbp"; }

    TGraphAsymmErrors* gRatio[nRap];
    TGraphAsymmErrors* gRatioOverlay[nRap];
    for(int iy=0;iy<nRap;iy++){
        gRatio[iy]=(TGraphAsymmErrors*)fin->Get(Form("gRatio_%s_%d",strRun.c_str(),iy));
        gRatioOverlay[iy]=(TGraphAsymmErrors*)fin->Get(Form("gRatio_%s_%d",strRunOverlay.c_str(),iy));
				gRatio[iy]->SetName(Form("gRatio_%d",iy));
				gRatioOverlay[iy]->SetName(Form("gRatioOverlay_%d",iy));
    }

    TFile* fout= new TFile(Form("fitRatio_%s%s_isStiff%d.root",strPr.c_str(),strRun.c_str(),(int)isStiff),"recreate");//KYO

    ////////////////////////////////////////////////////////////
    //// Fitting
    TF1* f_ptRatio[nRap];
    TCanvas* c1 = new TCanvas("c1", "", 1600,800); //KYO
    c1 -> Divide(4,2); // KYO
    for(int iy=0;iy<nRap;iy++){
				//if (iy!=0) continue;
        c1->cd(iy+1);
        // *** KYO *** fit options dedicated to isPrompt=1 && is1st==1 !!!
        if(iy==1 || iy==5){
            f_ptRatio[iy] = new TF1(Form("f_ptRatio_%s%s_%d",strPr.c_str(),strRun.c_str(),iy), fitStraight,0.0,30.0,2); 
            f_ptRatio[iy]->SetParameters(0.,0.); 
        }
				//if(iy==0 || iy==nRap-1){
        else {
            f_ptRatio[iy] = new TF1(Form("f_ptRatio_%s%s_%d",strPr.c_str(),strRun.c_str(),iy), fitHevi,0.0,30.0,5);
            f_ptRatio[iy]->SetParameters(-4.795, 1.653, 0.923, 3.018, 2.369);
            if(isPrompt==1 && is1st==1 && iy==0) { 
                f_ptRatio[iy]->FixParameter(0,-5.17167e+01 );
                f_ptRatio[iy]->FixParameter(1,-6.94876e-01 );
            }
            if(isPrompt==1 && is1st==1 && iy==6) { 
                if (isStiff){
									//from 2nd run result
                	f_ptRatio[iy]->FixParameter(0, -3.40750e+02);
                	f_ptRatio[iy]->FixParameter(1, -3.25937e+00); 
                	f_ptRatio[iy]->FixParameter(2, 1.16504e+00);
                	f_ptRatio[iy]->FixParameter(3, 2.98858e+00);
                	//f_ptRatio[iy]->FixParameter(4, 1.14946e+02);
								}
								else {
									f_ptRatio[iy]->FixParameter(0,-3.40743e+02);
  	              f_ptRatio[iy]->FixParameter(1,-2.56691e+01);
    	            f_ptRatio[iy]->FixParameter(2,5.14736e+00); 
								}
            }
						if (isPrompt==1 && is1st==1 && iy==3 && isStiff){
									//from 2nd run result
									f_ptRatio[iy]->FixParameter(0,-3.40710e+02);
  	              f_ptRatio[iy]->FixParameter(1,2.20286e+00);
    	            f_ptRatio[iy]->FixParameter(2,8.09749e-01); 
						}
						if (isPrompt==1 && is1st==1 && iy==4 && isStiff){
									//from 2nd run result
									f_ptRatio[iy]->FixParameter(0,-3.40734e+02);
  	              f_ptRatio[iy]->FixParameter(1,2.34884e+00);
    	            f_ptRatio[iy]->FixParameter(2,7.84326e-01); 
    	            f_ptRatio[iy]->FixParameter(3,2.98876e+00); 
						}

        }  
        SetGraphStyle(gRatio[iy],0,0);
        SetGraphStyle(gRatioOverlay[iy],2,10);
        f_ptRatio[iy]->SetLineColor(kRed);
        gRatio[iy]->Fit(Form("f_ptRatio_%s%s_%d",strPr.c_str(),strRun.c_str(),iy));
        gRatio[iy]->Draw("AP");
        gRatioOverlay[iy]->Draw("P");
        if (iy==0){
            if (isPrompt) latex->DrawLatex(0.23,0.23,"Prompt J/#psi Pbp");
            else latex->DrawLatex(0.23, 0.23,"Non-prompt J/#psi Pbp");
        }
        latex->DrawLatex(0.56,0.80,Form("%s",rapArr[iy].c_str()));
        dashedLine(0.,1.,25.,1.,1,1);
    }
    c1 -> Update();
		c1-> SaveAs(Form("fitRatio_%s%s_isStiff%d.pdf",strPr.c_str(),strRun.c_str(),(int)isStiff));	
    
		fout->cd();	
    for(int iy=0;iy<nRap;iy++){
        gRatio[iy]->Write();
        f_ptRatio[iy]->Write();
    }
    c1->Write();
    fout->Close();

}//end of main f_ptRatio.

double fitHevi(double *x, double *par) {
    return par[0]/(exp((-x[0]+par[1])/par[2] +1)+par[3])+par[4];
}
double fitExp(double *x, double *par) {
    return par[0]/(1+exp(par[1]*x[0]))+par[2]/x[0];
}
double fitStraight(double *x, double *par) {
    return par[0]*x[0]+par[1];
}

void formRapArr(Double_t binmin, Double_t binmax, string* arr) {
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

