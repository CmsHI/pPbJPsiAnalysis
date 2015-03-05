// Author : Yeonju
// to draw Acceptance distributions
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>
#include <string>
#include <math.h>

#include <TROOT.h>
#include "TSystem.h"
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TMath.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "TClonesArray.h"
#include <TAxis.h>
#include <cmath>
#include <TLorentzRotation.h>

#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TGraphErrors.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLine.h>
#include <TAxis.h>

#include <TRandom.h>
#include <TStopwatch.h>
#include <ctime>

const double shiftvar = -0.47; // conversion constant y=0(collision)==y=-0.47(LAB frame)  

void formRapArr(Double_t binmin, Double_t binmax, string* arr);
void formPtArr(Double_t binmin, Double_t binmax, string* arr);
void GetMinMaxMean(TH1D* hs,double stdVal);
void jumSun(Double_t x1=0,Double_t y1=0,Double_t x2=1,Double_t y2=1,Int_t color=1, Double_t width=1);
void onSun(Double_t x1=0,Double_t y1=0,Double_t x2=1,Double_t y2=1,Int_t color=1, Double_t width=1);

void draw_AccDist(bool isPrompt=true, bool isPbp=true){
	using namespace std;

	// Definition of bin
	// --- pt Bin
	Double_t ptBinsArr[] = {0.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap9pt
	//Double_t ptBinsArr[] = {5.0, 6.5, 10.0, 30.0}; // 6rap3pt
	const Int_t nPtBins = sizeof(ptBinsArr)/sizeof(double)-1;
	cout << "nPtBins=" << nPtBins << endl;

	Double_t AccCentArr[] = {0.087, 0.089, 0.131, 0.196, 0.288, 0.370, 0.447, 0.539, 0.690}; // Acceptance central value

    // --- y Bin //set to 1st run (For 2nd run, will be automatically changed later)
    Double_t yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // 8rap9pt
    //Double_t yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46}; // 6rap3pt
    const Int_t nYBins = sizeof(yBinsArr)/sizeof(double)-1;
    cout << "nYBins=" << nYBins << endl;

	// for 2nd run
	Double_t yBinsArr2nd[nYBins+1] = {};
	for (Int_t i=0; i<nYBins+1; i++) {
		 yBinsArr2nd[i] = -1*yBinsArr[nYBins-i];
		cout <<"yBinsArr["<<i<<"] = " <<yBinsArr[i]<<endl;
		cout <<"yBinsArr2nd["<<i<<"] = " <<yBinsArr2nd[i]<<endl;
	}
    const Int_t nYBins2nd = sizeof(yBinsArr2nd)/sizeof(double)-1;

    string rapArr[nYBins];
    for (Int_t iy=0; iy<nYBins; iy++) {
        formRapArr(yBinsArr[iy], yBinsArr[iy+1], &rapArr[iy]);
        cout << iy <<"th rapArr = " << rapArr[iy] << endl;
    }
    string ptArr[nPtBins];
    for (Int_t ipt=0; ipt<nPtBins; ipt++) {
        formPtArr(ptBinsArr[ipt], ptBinsArr[ipt+1], &ptArr[ipt]);
        cout << ipt <<"th ptArr = " << ptArr[ipt] << endl;
    }

    char* sampleName;

    ////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
    // Get the files and define histogrames
    const int nFile = 5;
    TFile* fin[nFile];
    if(isPrompt==0 && isPbp==0){
        sampleName = "Non-prompt pPb";
        fin[0]=new TFile("isPrompt0isPbp0/AccDist_isPrompt0_pPb_date1425037626.root");
        fin[1]=new TFile("isPrompt0isPbp0/AccDist_isPrompt0_pPb_date1425037616.root");
        fin[2]=new TFile("isPrompt0isPbp0/AccDist_isPrompt0_pPb_date1425037596.root");
        fin[3]=new TFile("isPrompt0isPbp0/AccDist_isPrompt0_pPb_date1425037635.root");
        fin[4]=new TFile("isPrompt0isPbp0/AccDist_isPrompt0_pPb_date1425037607.root");
    } else if(isPrompt==0 && isPbp==1){
        sampleName = "Non-prompt";
        fin[0]=new TFile("isPrompt0isPbp1/AccDist_isPrompt0_Pbp_date1425037960.root");
        fin[1]=new TFile("isPrompt0isPbp1/AccDist_isPrompt0_Pbp_date1425037950.root");
        fin[2]=new TFile("isPrompt0isPbp1/AccDist_isPrompt0_Pbp_date1425037969.root");
        fin[3]=new TFile("isPrompt0isPbp1/AccDist_isPrompt0_Pbp_date1425037937.root");
        fin[4]=new TFile("isPrompt0isPbp1/AccDist_isPrompt0_Pbp_date1425037978.root");
    } else if(isPrompt==1 && isPbp==0){
        sampleName = "Prompt pPb";
        fin[0]=new TFile("isPrompt1isPbp0/AccDist_isPrompt1_pPb_date1425038166.root");
        fin[1]=new TFile("isPrompt1isPbp0/AccDist_isPrompt1_pPb_date1425038156.root");
        fin[2]=new TFile("isPrompt1isPbp0/AccDist_isPrompt1_pPb_date1425038143.root");
        fin[3]=new TFile("isPrompt1isPbp0/AccDist_isPrompt1_pPb_date1425038179.root");
        fin[4]=new TFile("isPrompt1isPbp0/AccDist_isPrompt1_pPb_date1425038192.root");
    } else if(isPrompt==1 && isPbp==1){
        sampleName = "Prompt";
        fin[0]=new TFile("isPrompt1isPbp1/AccDist_isPrompt1_Pbp_date1425038470.root");
        fin[1]=new TFile("isPrompt1isPbp1/AccDist_isPrompt1_Pbp_date1425038384.root");
        fin[2]=new TFile("isPrompt1isPbp1/AccDist_isPrompt1_Pbp_date1425038367.root");
        fin[3]=new TFile("isPrompt1isPbp1/AccDist_isPrompt1_Pbp_date1425038402.root");
        fin[4]=new TFile("isPrompt1isPbp1/AccDist_isPrompt1_Pbp_date1425038431.root");
    }

    TCanvas* can[nFile];
    for(int ifile=0;ifile<nFile;ifile++){
        can[ifile] = new TCanvas(Form("can_%d",ifile),Form("can_%d",ifile),1000,600);
        can[ifile]->Divide(8,9);//8rap9pt
    }

    TH1D *hAccCompBin[nYBins][nPtBins][nFile];
    TH1D *hAccCompBin_tot[nYBins][nPtBins];
    for(int iy=0;iy<nYBins;iy++){
        for(int ipt=0;ipt<nPtBins;ipt++){
            cout << "PAD number : " << 9*iy+ipt+1 << endl;
            for(int ifile=0;ifile<nFile;ifile++){
                //can[ifile]->cd(8*iy+ipt+1); 
                can[ifile]->cd(72-(8*ipt)-(7-iy));
                hAccCompBin[iy][ipt][ifile]=(TH1D*)fin[ifile]->Get(Form("hAccCompBin_y%d_pt%d",iy,ipt));
                hAccCompBin[iy][ipt][ifile]->SetName(Form("hAccCompBin_y%d_pt%d_ifile%d",iy,ipt,ifile));
                hAccCompBin[iy][ipt][ifile]->SetTitle(Form("hAccCompBin_y%d_pt%d_ifile%d",iy,ipt,ifile));
                hAccCompBin[iy][ipt][ifile]->Draw("hist");
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
    // Add histogrames
 
    for(int iy=0;iy<nYBins;iy++){
        for(int ipt=0;ipt<nPtBins;ipt++){
            hAccCompBin_tot[iy][ipt] = (TH1D*)hAccCompBin[iy][ipt][0]->Clone(Form("hAccCompBin_tot_y%d_pt%d",iy,ipt));
            for(int ifile=1;ifile<nFile;ifile++){
                hAccCompBin_tot[iy][ipt]->Add(hAccCompBin[iy][ipt][ifile],+1);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
    // Set the standard(?) value
 
    TFile *accCentFile_PR = new TFile("../001_Acceptance/AccAna_8rap9pt_PRMC_boosted.root");
    TFile *accCentFile_NP = new TFile("../001_Acceptance/AccAna_8rap9pt_NPMC_boosted.root");
    TH1D* hAccCent;
    if(isPrompt==0 && isPbp==0){
        hAccCent = (TH1D*) accCentFile_NP->Get("h2D_Acc_pt_y_pPb");
    }else if(isPrompt==0 && isPbp==1){
        hAccCent = (TH1D*) accCentFile_NP->Get("h2D_Acc_pt_y_Pbp");
    }else if(isPrompt==1 && isPbp==0){
        hAccCent = (TH1D*) accCentFile_PR->Get("h2D_Acc_pt_y_pPb");
    }else if(isPrompt==1 && isPbp==1){
        hAccCent = (TH1D*) accCentFile_PR->Get("h2D_Acc_pt_y_Pbp");
    }   
    double AccCent[nYBins][nPtBins];

    TLatex* latex = new TLatex();
    latex->SetNDC();
    latex->SetTextAlign(12);
    latex->SetTextSize(0.04);

    TCanvas* can_tot = new TCanvas("can_tot","can_tot",1000,600);
    can_tot->Divide(8,9);
    for(int iy=0;iy<nYBins;iy++){
        for(int ipt=0;ipt<nPtBins;ipt++){
            can_tot->cd(72-(8*ipt)-(7-iy));
            hAccCompBin_tot[iy][ipt]->SetLabelSize(0.04,"X");
            hAccCompBin_tot[iy][ipt]->SetLabelOffset(0.03,"X");
            hAccCompBin_tot[iy][ipt]->SetTitle("");
            hAccCompBin_tot[iy][ipt]->Draw("hist");
            AccCent[iy][ipt] = hAccCent->GetBinContent(iy+1,ipt+1);
            jumSun(AccCent[iy][ipt],0,AccCent[iy][ipt],1000,4);
        }
    }

    TCanvas* can_tot_iy[nYBins];
    for(int iy=0;iy<nYBins;iy++){
        can_tot_iy[iy]= new TCanvas(Form("can_tot_iy%d",iy),Form("can_tot_iy%d",iy),2000,1000);
        can_tot_iy[iy]->Divide(4,2);
        for(int ipt=0;ipt<nPtBins;ipt++){
            can_tot_iy[iy]->cd(ipt+1);
            hAccCompBin_tot[iy][ipt]->Draw("hist");
            hAccCompBin_tot[iy][ipt]->GetXaxis()->SetRangeUser(AccCent[iy][ipt]-0.03,AccCent[iy][ipt]+0.03);
//            hAccCompBin_tot[iy][ipt]->SetRangeUser(AccCent[iy][ipt]-0.04,AccCent[iy][ipt]+0.04);
            jumSun(AccCent[iy][ipt],0,AccCent[iy][ipt],18000,4);
            if(ipt==0){
                latex->DrawLatex(0.56,0.29,sampleName);
                latex->DrawLatex(0.56,0.35,Form("%s",rapArr[iy].c_str()));
            }
            latex->DrawLatex(0.56,0.70,Form("%s",ptArr[ipt].c_str()));
            //latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
            cout << "iy : " << iy << ", ipt : " << ipt << " >>> " <<endl;
            GetMinMaxMean(hAccCompBin_tot[iy][ipt],AccCent[iy][ipt]);
        }
    }      
    ////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
    // Save the data!
 
    for(int ifile=0;ifile<nFile;ifile++){
        can[ifile]->SaveAs(Form("pdf/AccDist_isPrompt%d_isPbp%d_ifile%d.pdf",(int)isPrompt,(int)isPbp,ifile));
    }
    can_tot->SaveAs(Form("pdf/AccDist_isPrompt%d_isPbp%d_tot.pdf",(int)isPrompt,(int)isPbp));
    for(int iy=0;iy<nYBins;iy++){
        can_tot_iy[iy]->SaveAs(Form("pdf/AccDist_isPrompt%d_isPbp%d_tot_rapidity%d.pdf",(int)isPrompt,(int)isPbp,iy));
    } 

    TFile* outf = new TFile(Form("draw_AccDist_isPrompt%d_isPbp%d.root",(int)isPrompt,(int)isPbp),"recreate");
    outf->cd();
    for(int iy=0;iy<nYBins;iy++){
        can_tot_iy[iy]->Write();
        for(int ipt=0;ipt<nPtBins;ipt++){
            hAccCompBin_tot[iy][ipt]->Write();
        }
    }
    for(int ifile=0;ifile<nFile;ifile++){
        can[ifile]->Write();
    }
    can_tot->Write();
    outf->Close();
}

void jumSun(Double_t x1,Double_t y1,Double_t x2,Double_t y2,Int_t color, Double_t width)
{
    TLine* t1 = new TLine(x1,y1,x2,y2);
    t1->SetLineWidth(width);
    t1->SetLineStyle(7);
    t1->SetLineColor(color);
    t1->Draw();
}


void onSun(Double_t x1,Double_t y1,Double_t x2,Double_t y2,Int_t color, Double_t width)
{
    TLine* t1 = new TLine(x1,y1,x2,y2);
    t1->SetLineWidth(width);
    t1->SetLineStyle(1);
    t1->SetLineColor(color);
    t1->Draw();
}

void GetMinMaxMean(TH1D* hs, double stdVal){
    int hsbin=hs->GetNbinsX();
    bool isFillMin=false;
    bool isFillMax=false;
    double MinVal,MaxVal,Err,largestVal;
    Err = 0;
//    MinVal=hs->GetBinLowEdge(k);
    for (int k=1;k<hsbin+1;k++){
        if (!isFillMin && (hs->GetBinContent(k))!=0) {isFillMin=true;MinVal=hs->GetXaxis()->GetBinLowEdge(k);}
        if (k<hsbin && hs->GetBinContent(k)!=0 && (hs->GetBinContent(k+1)==0 || isFillMax)) {isFillMax=true;MaxVal=hs->GetXaxis()->GetBinLowEdge(k)+hs->GetBinWidth(k);}
    }

    if( abs(stdVal-MinVal) >= abs(stdVal-MaxVal) ) { Err = abs(stdVal-MinVal)/stdVal; largestVal = MinVal; }
    else { Err = abs(stdVal-MaxVal)/stdVal;  largestVal = MaxVal; }
    //std::cout << "B4 Weigt : "<< stdVal << ", MinVal : " << MinVal << " , Mean : " << hs->GetMean() << " , MaxVal : " << MaxVal << ", Error : " << Err << std::endl;
    cout << stdVal << "\t" << largestVal << "\t" << Err << std::endl;
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

