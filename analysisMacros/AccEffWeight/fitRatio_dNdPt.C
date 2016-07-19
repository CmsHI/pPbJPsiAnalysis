#include "../SONGKYO.h"

//const int nRap = 8;
//const int nPt = 9;

Double_t ptArrNum[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10., 14., 30.};
const Int_t nPt = sizeof(ptArrNum)/sizeof(Double_t)-1;

Double_t rapArrNumFB_pA[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87};// for pt dist.
Double_t rapArrNumFB_pp[] = {2.4, 1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4};
//Double_t rapArrNumBF[] = {-2.87, -2.4, -1.93, -1.5, -0.9, 0., 0.9, 1.5, 1.93};// for rap dist.
const Int_t nRap = sizeof(rapArrNumFB_pA)/sizeof(Double_t)-1;

Double_t lowPt_pA[] = {2.0, 4.0, 6.5, 6.5, 6.5, 5.0, 4.0, 2.0};
Double_t lowPt_pp[] = {2.0, 4.0, 6.5, 6.5, 6.5, 6.5, 4.0, 2.0};

double fitHevi(double *x, double *par);
double fitExp(double *x, double *par);
double fitStraight(double *x, double *par);
void formRapArr(Double_t binmin, Double_t binmax, TString* arr);

void fitRatio_dNdPt(int isPA=1, bool isPrompt=false){
    gRandom->SetSeed(time(0));
		gROOT->Macro("../Style.C");

		//latex box for beam, rapidity, pT info
		TLatex* latex = new TLatex();
		latex->SetNDC();
		latex->SetTextAlign(12);
		latex->SetTextSize(0.04);

    TString szPA;
    if (isPA==0) szPA="pp";
    else if (isPA==1) szPA="pA";
    else {cout << "select among isPA = 0, or 1"<< endl; return ; }
    TString szPrompt;
		if (isPrompt) szPrompt = "PR";
    else szPrompt = "NP";
	
    if (nRap != sizeof(rapArrNumFB_pp)/sizeof(Double_t)-1) { cout << "check nRap!!!" <<endl; return; }
    TString rapArr[nRap];
		for (Int_t iy=0; iy<nRap; iy++) {
			if (isPA==0) { formRapArr(rapArrNumFB_pp[iy+1], rapArrNumFB_pp[iy], &rapArr[iy]); }
			else { formRapArr(rapArrNumFB_pA[iy+1], rapArrNumFB_pA[iy], &rapArr[iy]); }
			cout << iy <<"th rapArr = " << rapArr[iy] << endl;
		}


    //////////////////////////
    TFile* fin = new TFile(Form("./dir_dNdPt/%s_dNdPt_isPrompt%d.root",szPA.Data(),(int)isPrompt));

    TGraphAsymmErrors* gRatio[nRap];
    for(int iy=0;iy<nRap;iy++){
        gRatio[iy]=(TGraphAsymmErrors*)fin->Get(Form("gRatio_%d",iy));
    }

    ////////////////////////////////////////////////////////////
    //// Fitting
    TF1* funct[nRap];
    TCanvas* c1 = new TCanvas("c1", "", 1600,800);
    c1 -> Divide(4,2);
    for(int iy=0;iy<nRap;iy++){
		  //if (iy!=0) continue;
      c1->cd(iy+1);
      if (isPA==0) {
        if (isPrompt && (iy==0 || iy==2 || iy==3 || iy==4 || iy==7) ) {
          //funct[iy] = new TF1(Form("funct_%s_%s_%d",szPrompt.Data(),szPA.Data(),iy), fitHevi,0.0,30.0,5);
          funct[iy] = new TF1(Form("funct_%s_%s_%d",szPrompt.Data(),szPA.Data(),iy), fitHevi,lowPt_pp[iy],30.0,5);
          funct[iy]->SetParameters(-4.795, 1.653, 0.923, 3.018, 2.369);
        } else {
          //funct[iy] = new TF1(Form("funct_%s_%s_%d",szPrompt.Data(),szPA.Data(),iy), fitStraight,0.0,30.0,2); 
          funct[iy] = new TF1(Form("funct_%s_%s_%d",szPrompt.Data(),szPA.Data(),iy), fitStraight,lowPt_pp[iy],30.0,2); 
          funct[iy]->SetParameters(0.,0.); 
        }
      } else {
        if ( (isPrompt && iy!=1) || (!isPrompt && (iy==3 || iy==5)) ) {
          //funct[iy] = new TF1(Form("funct_%s_%s_%d",szPrompt.Data(),szPA.Data(),iy), fitHevi,0.0,30.0,5);
          funct[iy] = new TF1(Form("funct_%s_%s_%d",szPrompt.Data(),szPA.Data(),iy), fitHevi,lowPt_pA[iy],30.0,5);
          funct[iy]->SetParameters(-4.795, 1.653, 0.923, 3.018, 2.369);
        } else {
          //funct[iy] = new TF1(Form("funct_%s_%s_%d",szPrompt.Data(),szPA.Data(),iy), fitStraight,0.0,30.0,2); 
          funct[iy] = new TF1(Form("funct_%s_%s_%d",szPrompt.Data(),szPA.Data(),iy), fitStraight,lowPt_pA[iy],30.0,2); 
          funct[iy]->SetParameters(0.,0.); 
        }
      }
      //// bin by bin tunning
      if (isPA==0) {
        if (isPrompt) {
          if (iy==0) {
            funct[iy]->SetParameter(0, -5.6);
            funct[iy]->SetParLimits(0,-5.8,-4.0);
            funct[iy]->SetParameter(1, -1.2);
            funct[iy]->SetParLimits(1,-1.4, 0.5); // the larger, the stiffer at low pT
          }
          else if (iy==2) {
            //funct[iy]->SetParameter(0, -4.6);
            //funct[iy]->SetParLimits(0,-4.8,-4.4);
            //funct[iy]->SetParameter(1, -3.0);
            //funct[iy]->SetParLimits(1,-3.2, 3.6);
            funct[iy]->SetParameter(0, -4.6);
            funct[iy]->SetParLimits(0,-10.8,-4.4);
            funct[iy]->SetParameter(1, 3.0);
            funct[iy]->SetParLimits(1,0.2, 3.6);
          }
          else if (iy==7) {
            funct[iy]->SetParameter(0, -4.6);
            funct[iy]->SetParLimits(0,-4.8,-4.4);
            funct[iy]->SetParameter(1, -3.0);
            funct[iy]->SetParLimits(1,-3.2, 0.3);
          }
        }
      } else { // pA
        if (isPrompt) {
          if (iy==0) {
            funct[iy]->SetParameter(0, -5.6);
            //funct[iy]->SetParLimits(0,-5.8,-5.4);
            funct[iy]->SetParLimits(0,-5.8,-4.4);
            funct[iy]->SetParameter(1, -1.2);
            //funct[iy]->SetParLimits(1,-1.4, -1.0);
            funct[iy]->SetParLimits(1,-1.4, 0.5); // the larger, the stiffer at low pT
          }
          //if (iy==7) {
            //funct[iy]->SetParameter(0, -4.6);
            //funct[iy]->SetParLimits(0,-4.8,-4.4);
            //funct[iy]->SetParameter(1, -3.0);
           // funct[iy]->SetParLimits(1,-3.2, -2.6);
          //}
        } else {
          if (iy==3) {
            //funct[iy]->FixParameter(0,-4.13184);
            funct[iy]->SetParameter(0, -4.6);
            funct[iy]->SetParLimits(0,-6.8,-3.8);
            funct[iy]->SetParameter(1, -3.0);
            funct[iy]->SetParLimits(1,-3.8, 8.0);
          }
          else if (iy==5) {
            //funct[iy]->FixParameter(0,-4.13184);
            funct[iy]->SetParameter(0, -4.6);
            funct[iy]->SetParLimits(0,-6.8,-3.8);
            funct[iy]->SetParameter(1, -3.0);
            funct[iy]->SetParLimits(1,-3.8, -1.0);
          }
        }
      }
      SetGraphStyle(gRatio[iy],0,0);
      //gRatio[iy]->SetMarkerSize(1.6);
      gRatio[iy]->SetMarkerSize(1.3);
      funct[iy]->SetLineColor(kRed);
      funct[iy]->SetLineWidth(1);
      gRatio[iy]->Fit(Form("funct_%s_%s_%d",szPrompt.Data(),szPA.Data(),iy),"R");
      gRatio[iy]->Draw("AP");
      if (iy==0){
        if (isPrompt) latex->DrawLatex(0.19,0.23,Form("%s Prompt J/#psi",szPA.Data()));
        else latex->DrawLatex(0.19, 0.23,Form("%s Non-prompt J/#psi",szPA.Data()));
      }
		  if (isPrompt) latex->DrawLatex(0.57,0.88,Form("%s",rapArr[iy].Data()));
		  else latex->DrawLatex(0.57,0.22,Form("%s",rapArr[iy].Data()));
      dashedLine(0.,1.,25.,1.,1,1);
    }
    c1 -> Update();
		c1-> SaveAs(Form("dir_fitRatio/%s_fitRatio_%s.pdf",szPA.Data(),szPrompt.Data()));	
    
    //// save as a root file
    TFile* fout= new TFile(Form("dir_fitRatio/%s_fitRatio_%s.root",szPA.Data(),szPrompt.Data()),"recreate");
		fout->cd();	
    for(int iy=0;iy<nRap;iy++){
      gRatio[iy]->Write();
      funct[iy]->Write();
    }
    c1->Write();
    fout->Close();

}//end of main funct.

double fitHevi(double *x, double *par) {
    return par[0]/(exp((-x[0]+par[1])/par[2] +1)+par[3])+par[4];
}
double fitExp(double *x, double *par) {
    return par[0]/(1+exp(par[1]*x[0]))+par[2]/x[0];
}
double fitStraight(double *x, double *par) {
    return par[0]*x[0]+par[1];
}

void formRapArr(Double_t binmin, Double_t binmax, TString* arr) {
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

