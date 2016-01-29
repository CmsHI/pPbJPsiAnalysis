#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>

#include <TROOT.h>
#include "TSystem.h"
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TMath.h>
#include <math.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "TRandom3.h"
#include "TClonesArray.h"
#include <TAxis.h>
#include <cmath>
#include <TLorentzRotation.h>

#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveStats.h>

/////// main func. ///////
int merge_cndr(char *dirName = "PAS"){

  using namespace std;
	const int nEt = 1;
	const int nRap = 8;
	const int nPt = 9;
	TFile *inFile[nRap];
	TH1D* hMeanPt[nEt][nRap][nPt];	
	TH1D* hMeanPtBkg[nEt][nRap][nPt];	

	for (int ir=0; ir<nRap; ir++){
		inFile[ir]= new TFile(Form("meanPt_%s_wBkg_%d.root",dirName,ir));
		for (int in=0; in<nEt; in++){
			for (int ipt=0; ipt< nPt; ipt++) {
				hMeanPt[in][ir][ipt] = (TH1D*)inFile[ir]->Get(Form("hMeanPt_%d_0_%d",in,ipt));
				hMeanPtBkg[in][ir][ipt] = (TH1D*)inFile[ir]->Get(Form("hMeanPtBkg_%d_0_%d",in,ipt));
				hMeanPt[in][ir][ipt]->SetName(Form("hMeanPt_%d_%d_%d",in,ir,ipt));
				hMeanPtBkg[in][ir][ipt]->SetName(Form("hMeanPtBkg_%d_%d_%d",in,ir,ipt));
				cout << in << ir << ipt << "th  = " << hMeanPt[in][ir][ipt] <<" "<< hMeanPtBkg[in][ir][ipt] << endl;
			}
		}	
	}
	
	////////////////////////////////////////////////
	////// save as a root file
	TFile *outFile = new TFile(Form("meanPt_%s_wBkg.root",dirName),"RECREATE");
	std::cout << "dirName: " << dirName << std::endl;
	cout << "meanPt_"<<dirName<<"_wBkg.root has been created :) " <<endl;	

	outFile->cd();
	for (int ir=0; ir<nRap; ir++){
		for (int in=0; in<nEt; in++){
			for (int ipt=0; ipt< nPt; ipt++) {
				hMeanPt[in][ir][ipt]->Write();
				hMeanPtBkg[in][ir][ipt]->Write();
			}
		}	
	}
		
	outFile->Close();
	
	return 0;

} // end of main func

