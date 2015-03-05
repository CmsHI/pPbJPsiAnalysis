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
#include <TLatex.h>
#include <TPaveText.h>
#include <TRandom.h>
#include "TFitResult.h"
#include <ctime>
#include <string>
#include <sstream>
//#include "../HiForestAnalysis/hiForest.h"
//#include "../gammaJetAnalysis/CutAndBinCollection2012.h"

Double_t ptArrNum[] = {0.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
const Int_t nPt = sizeof(ptArrNum)/sizeof(Double_t)-1;

Double_t ptFineArrNum[] = {0.0, 0.6, 1.2, 1.8, 2.4, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.3, 5.6, 5.9, 6.2, 6.5, 6.7, 6.9, 7.1, 7.3, 7.5, 7.7, 7.9, 8.1, 8.3, 8.5, 8.8, 9.1, 9.4, 9.7, 10., 10.8, 11.6, 12.4, 13.2, 14., 17.2, 20.4, 23.6, 26.8, 30.};
const Int_t nPtFine = sizeof(ptFineArrNum)/sizeof(Double_t)-1;

Double_t yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // 8rap9pt
//Double_t rapArrNumFB[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
//Double_t rapArrNumBF[] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
const Int_t nRap = sizeof(yBinsArr)/sizeof(Double_t)-1;

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);
double fitHevi(double *x, double *par);
double fitExp(double *x, double *par);

void S1_GenerateToyGaussian(bool isPrompt=true, bool isPbp=true){
    gRandom->SetSeed(time(0));
    TFile* fin = new TFile(Form("../004_closure/DataMcReco_8rap9pt/DataMcRecoPt_isPropmt%d.root",(int)isPrompt));//in KNU 
    string runstring;
    if(isPbp) runstring="Pbp";
    else runstring="pPb";

    string rapArr[nRap];
    for (Int_t iy=0; iy<nRap; iy++) {
        formRapArr(yBinsArr[iy], yBinsArr[iy+1], &rapArr[iy]);
        cout << iy <<"th rapArr = " << rapArr[iy] << endl;
    }
    string ptArr[nPt];
    for (Int_t ipt=0; ipt<nPt; ipt++) {
        formPtArr(ptArrNum[ipt], ptArrNum[ipt+1], &ptArr[ipt]);
        cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
    }

    char* sampleName;
    if(isPrompt) sampleName = "Prompt";
    else sampleName = "Non-prompt";
    TH1D* hRatio[nRap];
    TH1D* hRatio_tot[nRap];
    for(int iy=0;iy<nRap;iy++){
        hRatio[iy]=(TH1D*)fin->Get(Form("hRatio_%s_%d",runstring.c_str(),iy));
        hRatio_tot[iy]=(TH1D*)fin->Get(Form("hRatio_tot_%d",iy));
    }

    TFile* fout= new TFile(Form("ToyGaussian_isPrompt%d_%s_andTot.root",(int)isPrompt,runstring.c_str()),"recreate");

    ////////////////////////////////////////////////////////////
    // Output gaussian parameter histograms

    TH1D* hWeight[nRap];
    TH1D* hWeight_tot[nRap];
    // TH1D* hWeightSigma[nRap];
    for(int iy=0;iy<nRap;iy++){
        hWeight[iy]=new TH1D(Form("hWeight_%s_%d",runstring.c_str(),iy), Form("hWeight_%s_%d",runstring.c_str(),iy), nPtFine, ptFineArrNum);
        hWeight_tot[iy]=new TH1D(Form("hWeight_tot_%s_%d",runstring.c_str(),iy), Form("hWeight_tot_%s_%d",runstring.c_str(),iy), nPtFine, ptFineArrNum);
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
    TF1* func_tot[nRap];

    TCanvas* c1 = new TCanvas("c1", "c1", 1000,600);
    c1 -> Divide(4,2);

    TCanvas* c1_tot = new TCanvas("c1_tot", "c1_tot", 1000,600);
    c1_tot -> Divide(4,2);

    TLatex* latex = new TLatex();
    latex->SetNDC();
    latex->SetTextAlign(12);
    latex->SetTextSize(0.045);

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

            }
        } else if(isPrompt==1 && isPbp==0 && iy==4){
            func[4] = new TF1(Form("func_%d",iy), fitHevi,0.0,30.0,5); 
            func[4]->SetParameters(-3.40747e+02,3.32373e+00,6.51294e-01,2.98861e+00,1.14947e+02);
        } else{
            func[iy] = new TF1(Form("func_%d",iy), fitExp,0.0,30.0,3);
            if(isPrompt==0 && isPbp==1 && (iy==2 || iy==3 || iy==4 || iy==5) ) func[iy]->SetParameters(-1.01556e+01,1.51712e-01,2.80282e+01);
            if(isPrompt==0 && isPbp==0 && (iy==2 || iy==3 || iy==4) ) func[iy]->SetParameters(-1.01556e+01,1.51712e-01,2.80282e+01);
            if(isPrompt==1 && isPbp==0 && (iy==3) ) func[iy]->SetParameters(-1.01556e+01,1.51712e-01,2.80282e+01);
            if(isPrompt==1 && isPbp==1 && (iy==3) ) func[iy]->SetParameters(-1.01556e+01,1.51712e-01,2.80282e+01);
        }

        if(iy==4) {
            func_tot[iy] = new TF1(Form("func_tot_%d",iy), fitExp,0.0,30.0,3);
            func_tot[iy]->SetParameters(-1.01556e+01,1.51712e-01,2.80282e+01);
        } else {
            func_tot[iy] = new TF1(Form("func_tot_%d",iy), fitHevi,0.0,30.0,5);
            func_tot[iy]->SetParameters(-4.795, 1.653, 0.923, 3.018, 2.369);
            func_tot[iy]->FixParameter(0,-5.17167e+01 );
            func_tot[iy]->FixParameter(1,-6.94876e-01 );
            //func[iy]->FixParameter(2,4.91544e-01 );
            //            func_tot[iy]->FixParameter(2,9.91544e-01 );
        }

        hRatio[iy]->Fit(Form("func_%d",iy));
        hRatio[iy]->SetMarkerStyle(20);
        hRatio[iy]->SetMarkerSize(1.0);
        func[iy]->Draw("same");
        if(iy==0) latex->DrawLatex(0.50,0.75,sampleName);
        latex->DrawLatex(0.50,0.70,Form("%s",rapArr[iy].c_str()));
        //latex->DrawLatex(0.56,0.70,Form("%s",ptArr[iy].c_str()));

        c1_tot->cd(iy+1);
        hRatio_tot[iy]->Fit(Form("func_tot_%d",iy));
        func_tot[iy]->Draw("same");
        for(int ifpt=0;ifpt<nPtFine;ifpt++){
            int hBin = hRatio[iy]->FindBin(ptFineArrNum[ifpt+1]-0.1);
            double mean = func[iy]->Eval(ptFineArrNum[ifpt+1]);
            double sigma = hRatio[iy]->GetBinError(hBin) / mean;
            hWeight[iy]->SetBinContent(ifpt+1, mean);
            hWeight[iy]->SetBinError(ifpt+1, sigma);
            //cout <<"x : "<<ptFineArrNum[ifpt+1]-0.01<< ", hRatio bin : " <<hBin << ", Ratio error value : " <<  hRatio[iy]->GetBinError(hBin) << endl;
            //cout << iy+1 << "th rapidity, "<< ifpt<<"th fine ptbin, mean = "<< mean <<", sigma = "<<sigma<<endl;

            int hBin_tot = hRatio_tot[iy]->FindBin(ptFineArrNum[ifpt+1]-0.1);
            double mean_tot = func_tot[iy]->Eval(ptFineArrNum[ifpt+1]);
            double sigma_tot = hRatio_tot[iy]->GetBinError(hBin_tot) / mean_tot;
            hWeight_tot[iy]->SetBinContent(ifpt+1, mean_tot);
            hWeight_tot[iy]->SetBinError(ifpt+1, sigma_tot);
        }
    }
    c1 -> Update();
    c1_tot -> Update();

    c1 -> SaveAs(Form("pdf/Ratio_fitting_%s.pdf",sampleName));
    fout->cd();	
    for(int iy=0;iy<nRap;iy++){
        hWeight[iy]->Write();
        hWeight_tot[iy]->Write();
        func[iy]->Write();
        func_tot[iy]->Write();
    }
    c1->Write();
    c1_tot->Write();
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
        *arr = Form("%.0f < y_{LAB} < %.0f", binmin, binmax);
    } else if ( fracMin != 0 && fracMax == 0 ) { 
        *arr = Form("%.2f < y_{LAB} < %.0f", binmin, binmax);
    } else if ( fracMin == 0 && fracMax != 0 ) { 
        *arr = Form("%.0f < y_{LAB} < %.2f", binmin, binmax);
    } else {
        *arr = Form("%.2f < y_{LAB} < %.2f", binmin, binmax);
    }   
}

void formPtArr(Double_t binmin, Double_t binmax, string* arr) {
    Double_t intMin, intMax;
    Double_t fracMin = modf(binmin, &intMin);
    Double_t fracMax = modf(binmax, &intMax);
    if ( fracMin == 0 && fracMax == 0 ) { 
        *arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
    } else if ( fracMin != 0 && fracMax == 0 ) { 
        *arr = Form("%.1f < p_{T} < %.0f GeV/c", binmin, binmax);
    } else if ( fracMin == 0 && fracMax != 0 ) { 
        *arr = Form("%.0f < p_{T} < %.1f GeV/c", binmin, binmax);
    } else {
        *arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
    }   
}
