// latest version : 2015/02/20
// Yeonju Go 
// The code is based on FitAndMakeRatioToy_pt.C by Yeonju.
// The code generate gaussian mean & sigma of fine bins for the each rapidity bins
// 2D differential ratio(DATA/MC)

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

//#include "../HiForestAnalysis/hiForest.h"
//#include "../gammaJetAnalysis/CutAndBinCollection2012.h"

//Double_t ptArrNum[] = {0.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
Double_t ptArrNum[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
const Int_t nPt = sizeof(ptArrNum)/sizeof(Double_t)-1;

Double_t ptFineArrNum[] = {0.0, 0.6, 1.2, 1.8, 2.4, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.3, 5.6, 5.9, 6.2, 6.5, 6.7, 6.9, 7.1, 7.3, 7.5, 7.7, 7.9, 8.1, 8.3, 8.5, 8.8, 9.1, 9.4, 9.7, 10., 10.8, 11.6, 12.4, 13.2, 14., 17.2, 20.4, 23.6, 26.8, 30.};
const Int_t nPtFine = sizeof(ptFineArrNum)/sizeof(Double_t)-1;

Double_t rapArrNumFB[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
//Double_t rapArrNumBF[] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
const Int_t nRap = sizeof(rapArrNumFB)/sizeof(Double_t)-1;

double fitHevi(double *x, double *par);
double fitExp(double *x, double *par);
void formRapArr(Double_t binmin, Double_t binmax, string* arr);

void fitRatio_kyo01(bool isPrompt=false, bool isPbp=true, char* dirName="8rap9pt2gev"){
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


//    TFile* fin = new TFile(Form("../pPbJPsiAnalysis/2015/004_closure/DataMcReco_8rap9pt/DataMcRecoPt_isPropmt%d.root",(int)isPrompt));//in KNU 
    TFile* fin = new TFile(Form("../004_closure/DataMcReco_%s/DataMcRecoPt_isPropmt%d.root",dirName,(int)isPrompt));//in KNU  //KYO
    string runstring;
    string runstringOverlay;
    if(isPbp) {runstring="Pbp"; runstringOverlay="pPb";}
    else {runstring="pPb"; runstringOverlay="Pbp"; }

    TGraphAsymmErrors* gRatio[nRap];
    TGraphAsymmErrors* gRatioOverlay[nRap];
    for(int iy=0;iy<nRap;iy++){
        gRatio[iy]=(TGraphAsymmErrors*)fin->Get(Form("gRatio_%s_%d",runstring.c_str(),iy));
        gRatioOverlay[iy]=(TGraphAsymmErrors*)fin->Get(Form("gRatio_%s_%d",runstringOverlay.c_str(),iy));
				gRatio[iy]->SetName(Form("gRatio_%d",iy));
				gRatioOverlay[iy]->SetName(Form("gRatioOverlay_%d",iy));
    }

    //TFile* fout= new TFile(Form("ToyGaussian_isPrompt%d_%s.root",(int)isPrompt,runstring.c_str()),"recreate");
    //TFile* fout= new TFile(Form("ToyGaussian_isPrompt%d_%s_kyo01.root",(int)isPrompt,runstring.c_str()),"recreate");//KYO
    TFile* fout= new TFile(Form("fitRatio_isPrompt%d_%s_kyo01.root",(int)isPrompt,runstring.c_str()),"recreate");//KYO

    ////////////////////////////////////////////////////////////
    // Output gaussian parameter histograms

    TH1D* hWeight[nRap];
    // TH1D* hWeightSigma[nRap];
    for(int iy=0;iy<nRap;iy++){
//				if (iy!=0) continue ; //KYO
        hWeight[iy]=new TH1D(Form("hWeight_%s_%d",runstring.c_str(),iy), Form("hWeight_%s_%d",runstring.c_str(),iy), nPtFine, ptFineArrNum);
    //    hWeightSigma[iy]=new TH1D(Form("hWeightSigma_%s_%d",runstring.c_str(),iy), Form("hWeightSigma_%s_%d",runstring.c_str(),iy), nPtFine, ptFineArrNum);
    }
#if 0
    int rapiditybin[nRap],nfpt[nRap];
    double mean[nRap][nPtFine],sigma[nRap][nPtFine];

    TTree* toyGaus[nRap];
    for(int iy=0;iy<nRap;iy++){
        toyGaus[iy]=new TTree(Form("toyGaus%d",iy),Form("rapidity bin %dth",iy));
        toyGaus[iy]->Branch("rapiditybin", &rapiditybin[iy]);
        toyGaus[iy]->Branch("nfpt", &nfpt[iy]);
        toyGaus[iy]->Branch("mean", mean[iy],"mean[nfpt]/D");
        toyGaus[iy]->Branch("sigma", sigma[iy],"sigma[nfpt]/D");
    }
#endif

    ////////////////////////////////////////////////////////////
    // Fitting

//    TF1* fhevi= new TF1("fhevi","[0]/(exp((-x+[1])/[2] +1)+[3])+[4]",0,30);
//    fhevi->SetParameters(-4.795, 1.653, 0.923, 3.018, 2.369);
//    TF1* fexp= new TF1("fexp","[0]/(1+exp([1]*x))+[2]/x",0.0,30.0);//Prompt
    TF1* func[nRap];
   
//    TCanvas* c1 = new TCanvas("c1", "", 1200,1500);
//    c1 -> Divide(3,3);
    TCanvas* c1 = new TCanvas("c1", "", 1600,800); //KYO
    c1 -> Divide(4,2); // KYO
    for(int iy=0;iy<nRap;iy++){
        c1->cd(iy+1);
        if(iy==0 || iy==nRap-1){
            func[iy] = new TF1(Form("func_%d",iy), fitHevi,0.0,30.0,5);
            func[iy]->SetParameters(-4.795, 1.653, 0.923, 3.018, 2.369);
            if(isPrompt==1 && isPbp==0 && iy==0) { 
                //      func[iy]->SetParameters(-51.7167, -0.694876, 0.491544,4.55470,12.1672);
#if 1
                func[iy]->FixParameter(0,-5.17167e+01 );
                func[iy]->FixParameter(1,-6.94876e-01 );
                //func[iy]->FixParameter(2,4.91544e-01 );
                func[iy]->FixParameter(2,9.91544e-01 );
                //func[iy]->FixParameter(3,4.55470e+00 );
                //func[iy]->FixParameter(3,3.55470e+00 );
                //func[iy]->FixParameter(4,1.11672e+01 );
#endif
            } else if (isPrompt==0 && isPbp==1 && iy==7) {
                func[iy]->FixParameter(0, -3.40711e+02);
                func[iy]->FixParameter(1, 3.20176e+00); 
                func[iy]->FixParameter(2, 1.09760e+00);
            }


        }  
        //else if(isPrompt==1 && isPbp==0 && iy==4){
        //  func[iy] = new TF1(Form("func_%d",iy), fitHevi,0.0,30.0,5); 
        //  func[iy]->SetParameters(-3.40747e+02,3.32373e+00,6.51294e-01,2.98861e+00,1.14947e+02);
        //} 
        else{
            func[iy] = new TF1(Form("func_%d",iy), fitHevi,0.0,30.0,5); 
            //func[iy]->SetParameters(-3.40711e+02,-2.02319e+00,1.42753e+00,2.98897e+00,1.14958e+02); //yeonju 0509
            func[iy]->SetParameters(-3.40711e+02,-5.20176e+00,1.79760e+00,2.98896e+00,1.14958e+02); //yeonju 0509
            //func[iy]->SetParameters(-3.40747e+02,3.32373e+00,6.51294e-01,2.98861e+00,1.14947e+02); //yeonju
            //yeonju
            if (isPrompt==0 && iy==2) {
                func[iy]->FixParameter(0, -3.40711e+02);
                func[iy]->FixParameter(1, -5.20176e+00); 
                func[iy]->FixParameter(2, 1.79760e+00);
                func[iy]->FixParameter(3, 2.98896);

            } else if (isPrompt==0 && iy==6) {
                func[iy]->FixParameter(0, -3.40711e+02);
                func[iy]->FixParameter(1, 3.20176e+00); 
                func[iy]->FixParameter(2, 1.79760e+00);

            }        
            //KYO
            if (isPrompt && iy==1 || iy==6) func[iy]->SetParameters(-4.795, 1.653, 0.923, 3.018, 2.369);
            else if (isPrompt && iy==2) {
                func[iy]->FixParameter(0,-3.40743e+02);
                func[iy]->FixParameter(1,-1.37032e-01);
                func[iy]->FixParameter(2,1.19902);
            }									 
            /*
               func[iy] = new TF1(Form("func_%d",iy), fitExp,0.0,30.0,3);
               if(isPrompt==0 && isPbp==1 && (iy==2 || iy==3 || iy==4 || iy==5) ) func[iy]->SetParameters(-1.01556e+01,1.51712e-01,2.80282e+01);
               if(isPrompt==0 && isPbp==0 && (iy==2 || iy==3 || iy==4) ) func[iy]->SetParameters(-1.01556e+01,1.51712e-01,2.80282e+01);
               if(isPrompt==1 && isPbp==0 && (iy==3) ) func[iy]->SetParameters(-1.01556e+01,1.51712e-01,2.80282e+01);
               if(isPrompt==1 && isPbp==1 && (iy==3) ) func[iy]->SetParameters(-1.01556e+01,1.51712e-01,2.80282e+01);
               */
        }
        //KYO for TGraphAsymmErrors only
        if (!isPrompt && iy==0){
            func[iy]->SetParameters(-1.04829e+01,-5.38813e+00,2.10443e+00,1.74460e+00,6.89848e+02);
            func[iy]->FixParameter(0,-1.04829e+01);
            func[iy]->FixParameter(1,-5.38813e+00);
            func[iy]->FixParameter(2,2.10443e+00);
        }
        SetGraphStyle(gRatio[iy],2,0);
        SetGraphStyle(gRatioOverlay[iy],4,10);
        func[iy]->SetLineColor(kRed);
        gRatio[iy]->Fit(Form("func_%d",iy));
        gRatio[iy]->Draw("AP");
        gRatioOverlay[iy]->Draw("P");
        //func[iy]->Draw("same");
        if (iy==0){
            if (isPrompt) latex->DrawLatex(0.23,0.23,"Prompt J/#psi Pbp");
            else latex->DrawLatex(0.23, 0.23,"Non-prompt J/#psi Pbp");
        }
        latex->DrawLatex(0.56,0.80,Form("%s",rapArr[iy].c_str()));
        dashedLine(0.,1.,25.,1.,1,1);

        /*
				for(int ifpt=0;ifpt<nPtFine;ifpt++){
            int hBin = gRatio[iy]->FindBin(ptFineArrNum[ifpt+1]-0.1);
            double mean = func[iy]->Eval(ptFineArrNum[ifpt+1]);
            double sigma = gRatio[iy]->GetBinError(hBin) / mean;
            hWeight[iy]->SetBinContent(ifpt+1, mean);
            hWeight[iy]->SetBinError(ifpt+1, sigma);
            //cout <<"x : "<<ptFineArrNum[ifpt+1]-0.01<< ", gRatio bin : " <<hBin << ", Ratio error value : " <<  gRatio[iy]->GetBinError(hBin) << endl;
            //cout << iy+1 << "th rapidity, "<< ifpt<<"th fine ptbin, mean = "<< mean <<", sigma = "<<sigma<<endl;
        }
				*/
    }
    c1 -> Update();

    fout->cd();	
    for(int iy=0;iy<nRap;iy++){
        //hWeight[iy]->Write();
        gRatio[iy]->Write();
        func[iy]->Write();
    }
    c1->Write();
    fout->Close();

}//end of main func.

double fitHevi(double *x, double *par) {
    return par[0]/(exp((-x[0]+par[1])/par[2] +1)+par[3])+par[4];
}
double fitExp(double *x, double *par) {
    return par[0]/(1+exp(par[1]*x[0]))+par[2]/x[0];
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

