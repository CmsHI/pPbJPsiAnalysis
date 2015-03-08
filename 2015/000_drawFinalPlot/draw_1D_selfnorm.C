#include <Riostream.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
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

#include "KYOcommonOptFinal.h"

void draw_1D_selfnorm(char* dirName = "9et", int runCode=0)
{
	gROOT->Macro("./JpsiStyleForFinalResult.C");

	// set info.
	const Double_t br = 0.0593 ;
	const Double_t brErr = 0.0006;
	Double_t lumi_nb;
	Double_t lumi_nb_err;
	Double_t lumi_mub;
	Double_t lumi_mub_err;
	string runstring;
	string lumistring;
	string cmsstring = "CMS preliminary";
	string beamstring = "pPb #sqrt{s_{NN}} = 5.02 TeV";
	if (runCode ==0) { runstring = "All"; lumi_nb =34.622; lumi_nb_err=1.2; }
	else if (runCode == 1) { runstring = "Pbp"; lumi_nb =20.7; lumi_nb_err=0.7; }//1stRun
	else if (runCode == 2) { runstring = "pPb"; lumi_nb = 14.0; lumi_nb_err=0.5; }//2ndRun
	else { cout << " *** Error!!! choose runCode 0, 1, or 2 " << endl; return ; }
	lumistring = Form("L_{int} = %.1f nb^{  -1}", lumi_nb);
	lumi_mub = lumi_nb * 1000; // (nb)^{-1} -> {#mub}^{-1}
	lumi_mub_err = lumi_nb_err * 1000; // (nb)^{-1} -> {#mub}^{-1}

	/////////////////////////////////////////////////////////////////////////
	/// systematic uncertainties by hand KYO
	const int ntmp=9;
	Double_t px[ntmp]; //x point
	Double_t py_pr[ntmp]; //y point
	Double_t py_np[ntmp]; //y point
	Double_t ex[ntmp]; //x error (0)
	Double_t ey_pr[ntmp]; //y error
	Double_t ey_np[ntmp]; //y error
	Double_t exsys[ntmp]; //sys x error
	Double_t eysys_pr[ntmp]; //sys y error
	Double_t eysys_np[ntmp]; //sys y error
	
	px = {0.352171566,
	0.845911229,
	1.183528621,
	1.521050753,
	1.858525256,
	2.203144164,
	2.53410297,
	2.983022035,
	3.778776135};
	py_pr = {0.253684822,
	0.801659126,
	1.19682879,
	1.644191172,
	2.052496459,
	2.410399435,
	2.695106885,
	3.190048306,
	3.945568047};
	py_np = {0.277754618,
	0.859648482,
	1.226160295,
	1.622593968,
	1.936588072,
	2.286604052,
	2.681356162,
	2.918925613,
	3.439841682};
	ex = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, };
	ey_pr = {0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, };
	ey_np = {0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, };
	exsys = {0.08,0.08,0.08,0.08,0.08,0.08,0.08,0.08,0.08};
	eysys_pr = {0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09};
	eysys_np = {0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09};
	
	//rap array in yCM (from forward to backward)
	string rapArr = "-2.87 < y_{CM} < 1.93";
	string ptArr = "6.5 < p_{T} < 30 (GeV/c)";

	//////////////////////////////////////////////////////////////////
	TCanvas* c1 = new TCanvas("c1","c1",600,600);
	c1->cd();
	
	//TLegend *legUL = new TLegend(0.17, 0.55, 0.51, 0.92); //upper left
	TLegend *legUL = new TLegend(0.17, 0.78, 0.51, 0.92); //upper left
	TLegend *legBR = new TLegend(0.52, 0.15, 0.86, 0.39); //bottom left
	SetLegendStyle(legUL);	
	SetLegendStyle(legBR);	
	
	//latex box for beam, rapidity, pT info
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	TGraphAsymmErrors* gself_pr_sys = new TGraphAsymmErrors(ntmp, px, py_pr, exsys, exsys, eysys_pr, eysys_pr);
	TGraphAsymmErrors* gself_pr = new TGraphAsymmErrors(ntmp, px, py_pr, ex, ex, ey_pr, ey_pr);
	TGraphAsymmErrors* gself_np_sys = new TGraphAsymmErrors(ntmp, px, py_np, exsys, exsys, eysys_np, eysys_np);
	TGraphAsymmErrors* gself_np = new TGraphAsymmErrors(ntmp, px, py_np, ex, ex, ey_np, ey_np);
	//sys
	gself_pr_sys->GetXaxis()->SetTitle("E_{T}^{HF |#eta|>4}/<E_{T}^{HF |#eta|>4}>");	
	gself_pr_sys->GetXaxis()->CenterTitle();	
	gself_pr_sys->GetYaxis()->SetTitle("N(J/#psi)/<N(J/#psi)>");	
	gself_pr_sys->GetXaxis()->SetLimits(0.,4.8);	
	gself_pr_sys->SetMinimum(0.0);	
	gself_pr_sys->SetMaximum(4.8);	
	gself_pr_sys->SetFillColor(kRed-9);	
	gself_pr_sys->Draw("A2");
	gself_np_sys->SetFillColor(kTeal+7);
	gself_np_sys->Draw("2");
	//value
	SetGraphStyle(gself_pr, 1, 3);
	gself_pr->SetMarkerSize(1.2);
	gself_pr->Draw("P");	
	SetGraphStyle(gself_np, 0, 5);
	gself_np->SetMarkerSize(1.9);
	gself_np->Draw("P");	

	dashedLine(0.,0.,4.8,4.8,1,1.5);
	//legUL->SetTextSize(0.037);
	legUL -> AddEntry(gself_pr, "Prompt J/#psi","lp");
	legUL -> AddEntry(gself_np, "Non-prompt J/#psi","lp");
	legUL->Draw();
	
	latex->SetTextSize(0.05);
	latex->DrawLatex(0.54, 0.35, cmsstring.c_str());	
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.56, 0.27, beamstring.c_str());
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.56, 0.20, lumistring.c_str());
	
	latex->SetTextSize(0.037);
	latex->DrawLatex(0.20, 0.74, rapArr.c_str());
	latex->SetTextSize(0.037);
	latex->DrawLatex(0.20, 0.68, ptArr.c_str());

	c1->SaveAs(Form("selfnorm_%s/selfnorm.pdf",dirName));
	


	return;

}


