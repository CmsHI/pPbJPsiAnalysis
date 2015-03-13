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
#include "TString.h"

#include <TMath.h>
#include <math.h>

#include <sstream>
#include <string>

#include "KYOcommonOpt.h"


void check_fraction(char *stringA = "6rap3pt")
{

	gROOT->Macro("./JpsiStyle.C");

	TFile * refFile;
	//refFile = new TFile(Form("ethf_data_%s.root",stringA));
	refFile = new TFile(Form("ethf_MB_%s.root",stringA));
	string vartype = "Et";
	int nBin;

	TH1D* hist = (TH1D*)refFile->Get(Form("h%s_0",vartype.c_str()));
	nBin = hist->GetSize()-2;
	cout << endl;
	cout << " ***** " << vartype <<" hist : " << hist <<", nBin = "<< nBin << endl;

	cout << "hist integ = " << hist->Integral() << endl;
	hist->Scale(1./hist->Integral());
	cout << "hist norm integ = " << hist->Integral()*100 << " (\%)" <<endl;

	/////// calculation ////////
	
	double counter =0;
	const int binning = 20;
	cout << "binning = " << binning << endl;
	double fracVal[binning]={5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
	double fracRealVal[binning]={123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123 };
	double binVal[binning];


	for (Int_t i=0; i<nBin; i++){
		counter += 100*(hist->GetBinContent(i+1));
		if (counter>100) break;	
		if (counter >=fracVal[0] && counter< fracRealVal[0]) { 
			cout << fracVal[0]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[0]= counter;
			binVal[0]=i;
		}
		if (counter >=fracVal[1] && counter< fracRealVal[1]) { 
			cout << fracVal[1]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[1]= counter;
			binVal[1]=i;
		}
		if (counter >=fracVal[2] && counter< fracRealVal[2]) { 
			cout << fracVal[2]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[2]= counter;
			binVal[2]=i;
		}
		if (counter >=fracVal[3] && counter< fracRealVal[3]) { 
			cout << fracVal[3]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[3]= counter;
			binVal[3]=i;
		}
		if (counter >=fracVal[4] && counter< fracRealVal[4]) { 
			cout << fracVal[4]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[4]= counter;
			binVal[4]=i;
		}
		if (counter >=fracVal[5] && counter< fracRealVal[5]) { 
			cout << fracVal[5]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[5]= counter;
			binVal[5]=i;
		}
		if (counter >=fracVal[6] && counter< fracRealVal[6]) { 
			cout << fracVal[6]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[6]= counter;
			binVal[6]=i;
		}
		if (counter >=fracVal[7] && counter< fracRealVal[7]) { 
			cout << fracVal[7]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[7]= counter;
			binVal[7]=i;
		}
		if (counter >=fracVal[8] && counter< fracRealVal[8]) { 
			cout << fracVal[8]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[8]= counter;
			binVal[8]=i;
		}
		if (counter >=fracVal[9] && counter< fracRealVal[9]) { 
			cout << fracVal[9]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[9]= counter;
			binVal[9]=i;
		}
		if (counter >=fracVal[10] && counter< fracRealVal[10]) { 
			cout << fracVal[10]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[10]= counter;
			binVal[10]=i;
		}
		if (counter >=fracVal[11] && counter< fracRealVal[11]) { 
			cout << fracVal[11]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[11]= counter;
			binVal[11]=i;
		}
		if (counter >=fracVal[12] && counter< fracRealVal[12]) { 
			cout << fracVal[12]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[12]= counter;
			binVal[12]=i;
		}
		if (counter >=fracVal[13] && counter< fracRealVal[13]) { 
			cout << fracVal[13]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[13]= counter;
			binVal[13]=i;
		}
		if (counter >=fracVal[14] && counter< fracRealVal[14]) { 
			cout << fracVal[14]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[14]= counter;
			binVal[14]=i;
		}
		if (counter >=fracVal[15] && counter< fracRealVal[15]) { 
			cout << fracVal[15]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[15]= counter;
			binVal[15]=i;
		}
		if (counter >=fracVal[16] && counter< fracRealVal[16]) { 
			cout << fracVal[16]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[16]= counter;
			binVal[16]=i;
		}
		if (counter >=fracVal[17] && counter< fracRealVal[17]) { 
			cout << fracVal[17]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[17]= counter;
			binVal[17]=i;
		}
		if (counter >=fracVal[18] && counter< fracRealVal[18]) { 
			cout << fracVal[18]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[18]= counter;
			binVal[18]=i;
		}
		if (counter >=fracVal[19] && counter< fracRealVal[19]) { 
			cout << fracVal[19]<<"(\%) bin is = "<< i <<", real fraction = " << counter << endl;
			fracRealVal[19]= counter;
			binVal[19]=i;
		}
	}

	cout << "last counter should be 100 (\%) = " << counter << endl;
	cout << endl;

	// check the value
	for (Int_t i=0; i< binning-1; i++){
		//cout << fracVal[i] << "\t" << binVal[i]<<"\t" <<fracRealVal[i] << endl;
		cout << binVal[i]<<"\t" <<fracRealVal[i] << endl;
	}


/*
	//latex box for beam, rapidity, pT info
	TLatex* tex = new TLatex();
	tex->SetNDC();
	tex->SetTextAlign(12);
	tex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms
	TCanvas* c1  = new TCanvas("c1","c1",600,600);
	
	////// 01 acceptance	
	c1->cd();
	//c1->SetLeftMargin(0.14);
  c1->SetTopMargin(0.015);
  c1->SetLeftMargin(0.09);
  //c1->SetBottomMargin(0.01);
	//gStyle->SetPadTopMargin(0.03);
	
	h2D_Acc->GetXaxis()->SetTitle("y_{lab}");
	h2D_Acc->GetXaxis()->CenterTitle();
	h2D_Acc->GetXaxis()->SetTitleOffset(1.3);
	h2D_Acc->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_Acc->GetYaxis()->CenterTitle();
	//h2D_Acc->GetYaxis()->SetTitleOffset(1.1);
	h2D_Acc->SetMinimum(0);
	h2D_Acc->SetMaximum(1);
	//gStyle->SetPaintTextFormat(".3f");
	//h2D_Acc->SetMarkerSize(1.2);
	h2D_Acc->SetMarkerSize(0.8);
	h2D_Acc->Draw("e text colz");
	if (isCoarser==0) c1->SaveAs(Form("h2D_Acc_%s_double.pdf",stringA));
	else if (isCoarser==1) c1->SaveAs(Form("h2D_Acc_%s_double_coarser.pdf",stringA));
	c1->Clear();

	////// 02 efficiency PR
	c1->cd();
	//c1->SetLeftMargin(0.14);
	h2D_EffPR->GetXaxis()->SetTitle("y_{lab}");
	h2D_EffPR->GetXaxis()->CenterTitle();
	h2D_EffPR->GetXaxis()->SetTitleOffset(1.3);
	h2D_EffPR->GetYaxis()->SetTitle("p_{T} (GeV/c)");
	h2D_EffPR->GetYaxis()->CenterTitle();
	//h2D_EffPR->GetYaxis()->SetTitleOffset(1.1);
	h2D_EffPR->SetMinimum(0);
	h2D_EffPR->SetMaximum(1);
	//gStyle->SetPaintTextFormat(".3f");
	//h2D_EffPR->SetMarkerSize(1.2);
	h2D_EffPR->SetMarkerSize(0.8);
	h2D_EffPR->Draw("e text colz");
	if (isCoarser==0) c1->SaveAs(Form("h2D_EffPR_%s_double.pdf",stringA));
	else if (isCoarser==1) c1->SaveAs(Form("h2D_EffPR_%s_double_coarser.pdf",stringA));
	c1->Clear();

*/


	return;

}

