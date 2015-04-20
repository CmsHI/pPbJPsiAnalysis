#include <Riostream.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <vector>
#include <TSystem.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include "TStyle.h"
#include "TSystem.h"

#include <TMath.h>
#include <math.h>

#include <sstream>
#include <string>

#include "KYOcommonOpt.h"


void draw_1D_finePt(){
  gROOT->Macro("./JpsiStyle.C");
    // --- read-in file
    TFile * f2D;
    char* strEmbd;
	char* strPrompt;
	char* str1st;

	f2D = new TFile("EffCounting_8rap9pt_PRMCpythia_Pbp_useCtErr_0_useDataDriven_0_useZvtxStep1_0_Step2_1_finePtBin.root");
	//	f2D = new TFile("EffCounting_8rap9pt_PRMCpythia_Pbp_useCtErr_0_useDataDriven_1_useZvtxStep1_0_Step2_1_finePtBin.root");

	// --- read-in 2D hist
	TH2D* h2D_Eff = (TH2D*)f2D->Get("h2D_Eff_pt_y");
	const int nbinsX = h2D_Eff->GetNbinsX();
	const int nbinsY = h2D_Eff->GetNbinsY();
	cout << "h2D_Eff = " << h2D_Eff << endl;
	cout << "nbinsX = " << nbinsX << endl;
	cout << "nbinsY = " << nbinsY << endl;

	// --- read bin edges (rapidity) and fill to Tree
	int nEdge = nbinsX+1; 
	double rapEdge[nEdge];
	for (Int_t ie = 0; ie < nEdge; ie++) {
		rapEdge[ie] = h2D_Eff->GetXaxis()->GetBinLowEdge(ie+1);
		cout << "rapEdge["<<ie<<"] = " << rapEdge[ie] << endl;
	}
	TTree* tRap = new TTree("tRap","");
	tRap->Branch("nEdge",&nEdge,"nEdge/I");
	tRap->Branch("rapEdge",&rapEdge,"rapEdge[nEdge]/D");
	tRap->Fill();
	
	// ---  projection to 1D hist
	TH1D* h1D_EffPt[nbinsX]; 
	for (Int_t iy = 0; iy < nbinsX; iy++) {
	  h1D_EffPt[iy] = h2D_Eff->ProjectionY(Form("h1D_EffPt_%d",iy),iy+1,iy+1);
	  // test
	  cout <<""<<endl;
	  cout << iy <<"th rap " << endl;
	  for (int ipt=0; ipt <nbinsY; ipt ++ ){
	    cout << ipt <<"th pt" << endl;
	    cout << "h1D_EffPt genBinCenter = " << h1D_EffPt[iy]->GetXaxis()->GetBinCenter(ipt+1) << endl;
	    cout << "h1D_EffPt value = " << h1D_EffPt[iy]->GetBinContent(ipt+1) << endl;
	  }
	}
	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms
	for (Int_t iy = 0; iy < nbinsX; iy++) {
	  TCanvas* c1 = new TCanvas(Form("c1_%d",iy),Form("c1_%d",iy),400,400);
	  c1->cd();
	  SetHistStyle(h1D_EffPt[iy],0,0);
	  h1D_EffPt[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
	  h1D_EffPt[iy]->GetYaxis()->SetTitle("Efficiency");
	  h1D_EffPt[iy]->SetMinimum(0.);
	  h1D_EffPt[iy]->SetMaximum(1.);
	  h1D_EffPt[iy]->Draw("pe");
	  latex->DrawLatex(0.50,0.25,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
	  //		c1->SaveAs(Form("%s/%s_EffPt_%d.pdf",dirName,sampleName,iy));
	  //		c1->SaveAs(Form("%s/%s_sf%d_EffPt_%d.pdf",dirName,sampleName,(int)useDataDrivenEff,iy));
	}
	
	TFile* fout=new TFile(Form("h1Eff_%s",f2D->GetName()),"recreate");
	for (Int_t iy = 0; iy < nbinsX; iy++) {
	  h1D_EffPt[iy]->Write();
	}
	fout->Close();
}

