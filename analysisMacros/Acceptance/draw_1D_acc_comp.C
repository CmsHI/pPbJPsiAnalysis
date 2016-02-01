#include "SONGKYO.h"

// no ordering in rap (just y_lab)
void draw_1D_acc_comp(bool isPrompt = false, char* dirName = "dir_acc_comp_NEW_PAS", bool isNoError=true)
{
	gROOT->Macro("./Style.C");

	// --- read-in file
	// hRatio = f01 / f02
	TFile * f01;
	TFile * f02;
	char* f01name="NEW";
	char* f02name="PAS";
	
	//double ratiomin=0.85; 
	//double ratiomax=1.45; 
	double ratiomin=0.; 
	double ratiomax=1.2; 

	char* strPrompt;
	char* str1st;
	if (isPrompt) strPrompt = "PRMC";
	else strPrompt = "NPMC";
	str1st = "Pbp";
	//if (isPbp) str1st = "Pbp"; 
	//else str1st = "pPb"; //not used for acc

	const char* sampleName = Form("%spythia_%s",strPrompt,str1st);
	cout << "sampleName = " << sampleName << endl;

	f01 = new TFile(Form("./dir_acc/8rap9pt_%s_AccPt.root",strPrompt));
	f02 = new TFile(Form("./dir_acc_PAS/8rap9pt_%s_AccPt.root",strPrompt));

	cout << "sampleName = " << sampleName << endl;

	// --- read-in tree
	TTree *tr = (TTree*)f01->Get("tRap");
	int nEdge;
	double rapEdge[531];
	TBranch *b_nEdge;
	TBranch *b_rapEdge;
	tr->SetBranchAddress("nEdge",&nEdge,&b_nEdge);
	tr->SetBranchAddress("rapEdge",&rapEdge,&b_rapEdge);
	cout << "tr entries = " << tr->GetEntries() << endl;
	for (int ev=0; ev <tr->GetEntries(); ev++){
		tr->GetEntry(ev);
	}
	cout << "nEdge = " << nEdge << endl;
	for (Int_t i=0; i< nEdge; i++) {
		cout << "rapEdge["<<i<<"] = " << rapEdge[i] << endl;
	}

	// --- read-in eff hist
	const int nRap = nEdge -1 ; 
	cout << "nRap = " << nRap << endl;
	TH1D* h1D_AccPt01[nRap]; 
	TH1D* h1D_AccPt02[nRap]; 
	TH1D *hRatio[nRap]; //f01/f02
	for (int iy=0; iy < nRap; iy ++){
		h1D_AccPt01[iy] = (TH1D*)f01->Get(Form("h1D_AccPt_%d",iy));
		h1D_AccPt02[iy] = (TH1D*)f02->Get(Form("h1D_AccPt_%d",iy));
	cout << "h1D_AccPt01["<<iy<<"] = " << h1D_AccPt01[iy] << endl;
	cout << "h1D_AccPt02["<<iy<<"] = " << h1D_AccPt02[iy] << endl;
		hRatio[iy]=(TH1D*)h1D_AccPt01[iy]->Clone(Form("hRatio_%d",iy));
		hRatio[iy]->Divide(h1D_AccPt02[iy]);
	}
	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	//////////////////////////////////////////////////////////////////
	// --- Draw histograms

	TCanvas* c1 = new TCanvas("c1","c1",1600,800);
	c1->Divide(4,2);
	TCanvas* c2 = new TCanvas("c2","c2",1600,800);
	c2->Divide(4,2);
	TLegend *legBR = new TLegend(0.50,0.30,0.80,0.41,NULL,"brNDC");
	SetLegendStyle(legBR);

	for (Int_t iy = 0; iy < nRap; iy++) {
		//// 1)  distributions
		c1->cd(iy+1);
		SetHistStyle(h1D_AccPt01[iy],3,0);
		SetHistStyle(h1D_AccPt02[iy],4,10);
		h1D_AccPt01[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		h1D_AccPt01[iy]->GetYaxis()->SetTitle("Acceptance");
		h1D_AccPt01[iy]->SetMinimum(0.);
		h1D_AccPt01[iy]->SetMaximum(1.);
		h1D_AccPt01[iy]->GetXaxis()->SetRangeUser(0., 30.);
		h1D_AccPt01[iy]->Draw("pe");
		h1D_AccPt02[iy]->Draw("pe same");
		//// ----- for 1st run
		if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,.8);
		else if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,2,.8);
		else if (iy==5)  dashedLine (5.,0.,5.,1.,2,.8);
		else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,0.,6.5,1.,2,.8);
		/* 
		//// ----- for 2nd run
		if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,.8);
		else if (iy==1 || iy==6) dashedLine (3.,0.,3.,1.,2,.8);
		else if (iy==2)  dashedLine (5.,0.,5.,1.,2,.8);
		else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,.8);
		*/
		latex->DrawLatex(0.53,0.25,sampleName);
		latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
		if (iy==0) {
			legBR->AddEntry(h1D_AccPt01[iy],f01name,"lp");
			legBR->AddEntry(h1D_AccPt02[iy],f02name,"lp");
			legBR->Draw();
		}
		
		//// 2) ratio f01/f02
		c2->cd(iy+1);
		SetHistStyle(hRatio[iy],5,0);
		hRatio[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
		hRatio[iy]->GetYaxis()->SetTitle(Form("[ %s ]/[ %s ]",f01name,f02name));
		hRatio[iy]->SetMinimum(ratiomin);
		hRatio[iy]->SetMaximum(ratiomax);
		hRatio[iy]->GetXaxis()->SetRangeUser(0.,30.);
		if (isNoError) {
			for (int ipt=0; ipt<9; ipt++) {
				hRatio[iy]->SetBinError(ipt+1,0.);
			}
		}
		hRatio[iy]->Draw("pe");
		dashedLine(0.,1.,30.,1.,1,.8);
		//// ----- for 1st run
		if (iy==0 || iy==7) dashedLine (2.,ratiomin,2.,ratiomax,2,.8);
		else if (iy==1 || iy==6) dashedLine (3.,ratiomin,3.,ratiomax,2,.8);
		else if (iy==5)  dashedLine (5.,ratiomin,5.,ratiomax,2,.8);
		else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,ratiomin,6.5,ratiomax,2,.8);
		/* 
		//// ----- for 2nd run
		if (iy==0 || iy==7) dashedLine (2.,ratiomin,2.,ratiomax,2,.8);
		else if (iy==1 || iy==6) dashedLine (3.,ratiomin,3.,ratiomax,2,.8);
		else if (iy==2)  dashedLine (5.,ratiomin,5.,ratiomax,2,.8);
		else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,ratiomin,6.5,ratiomax,2,.8);
		*/
		latex->DrawLatex(0.53,0.25,sampleName);
		latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));

	}

	c1->SaveAs(Form("%s/%s_comp_acc.pdf",dirName,strPrompt));
	c2->SaveAs(Form("%s/%s_compRatio_acc.pdf",dirName,strPrompt));

	return;

}

