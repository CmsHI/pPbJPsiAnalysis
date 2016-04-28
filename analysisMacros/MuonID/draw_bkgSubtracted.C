#include "SONGKYO.h"

double calculInteg( TH1 *a, double minX, double maxX);

//szPA = "pp" or "pa"
int draw_bkgSubtracted(TString szPA = "pp")
{
	gROOT->Macro("./Style.C");
	
	// read-in root file
	TFile *fIn = new TFile(Form("dir_muID/%s_muIDvariables_isScaled0.root",szPA.Data()));

  //// define 1D hist
  const int nbin[] = {2, 2, 2, 20, 6, 90, 88};
  const double minbin[] = {0, 0., 0., 0, 0, -4.5, -22.};
  const double maxbin[] = {2, 2., 2., 20, 6, 4.5, 22.};
  //// for dashedLine (valid for isScaled)
  double xmin[] = {1., 1., 1., 6., 1., -0.3, -20.};
  double xmax[] = {531., 531., 531., 531., 531., 0.3, 20.};
  double ymin[] = {0., 0., 0., 0., 0., 0.00001, 0.00001};
  double ymax[] = {1.1, 1.1, 1.1, 0.25, 0.8, 1, 1};
  TString histName[] = {"highPurity", "TrkMuArb", "TMOneStaTight", "nTrkWMea", "nPixWMea", "dxy", "dz"};
  if (strcmp(szPA,"pa")==0) {histName[0]="isHighPurity";}
  TString histTitle[] = {"high purity flag", "tracker muon arbitrated flag", "TMOneStationTight flag", "# of trk layers w/ measurement", "# of pixel layers w/ measurement", "dxy (cm)", "dz (cm)"};
  const int nHist = sizeof(nbin)/sizeof(int);
  cout << "nHist = "<<nHist<<endl;

  
  TH1D *hMass[nHist]; //mass distibution for yield checks
  TH1D *hSig[nHist]; // in 2.9 - 3.3 GeV
	TH1D *hSigSub[nHist]; // Sig - scaledBkg
  TH1D *hBkg[nHist]; // out of 2.9 - 3.3 GeV
  TH1D *hPRMC[nHist]; // in 2.9 - 3.3 GeV

	TCanvas* c1[nHist];
	//TLegend *legUR = new TLegend(0.58,0.75,0.88,0.90,NULL,"brNDC");
	TLegend *legUR = new TLegend(0.58,0.70,0.88,0.90,NULL,"brNDC");
	SetLegendStyle(legUR);
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextFont(42);
	latex->SetTextSize(0.04);

	for (int ih=0; ih<nHist; ih++) {
		hMass[ih] = (TH1D*)fIn->Get(Form("hMass_%s",histName[ih].Data()));
		hSig[ih] = (TH1D*)fIn->Get(Form("hSig_%s",histName[ih].Data()));
		hBkg[ih] = (TH1D*)fIn->Get(Form("hBkg_%s",histName[ih].Data()));
		hPRMC[ih] = (TH1D*)fIn->Get(Form("hPRMC_%s",histName[ih].Data()));
    //// get yields from mass distributions  for scaling
    double nSig = calculInteg(hMass[ih],2.9,3.3);
    double nBkg = calculInteg(hMass[ih],2.6,2.9);
	  cout << "1) nBkg = " << nBkg << endl;
    nBkg += calculInteg(hMass[ih],3.3,3.5);
	  cout << "2 nBkg = " << nBkg << endl;
    nBkg *= (0.4/0.5); // in 2.9-3.3
	  double nBkgToSig = nBkg/nSig;
		cout << ih <<"th hist name = " << histName[ih].Data() << endl;
	  cout << "nSig = " << nSig << endl;
	  cout << "nBkg = " << nBkg << endl;
	  cout << "nBkgToSig = " << nBkgToSig << endl;
		
		////// background subtraction
		/// 1) subtracted signal hist.	
		hSigSub[ih] = (TH1D*)hSig[ih]->Clone(Form("hSigSub_%s",histName[ih].Data()));
		/// 2) First, normalized by their integral
		hSig[ih]->Scale(1/hSig[ih]->Integral());
		hSigSub[ih]->Scale(1/hSigSub[ih]->Integral());
		hBkg[ih]->Scale(1/hBkg[ih]->Integral());
		/// 3) Bkg normalized to nBkgToSig and subtract from the signal
		hBkg[ih] ->Scale(nBkgToSig);
		//hBkg[ih] ->Scale(0.4/0.5);
		hSigSub[ih]->Add(hBkg[ih],-1);
		/// 4) set zero if BinContent is negative 
    double tmpbin;
    for (int ibin=0; ibin < hSigSub[ih]->GetNbinsX(); ibin++) {
      tmpbin = hSigSub[ih]->GetBinContent(ibin+1);
      if (tmpbin < 0.) {cout << " **** "<<ibin<<"th tmpbin = " << tmpbin<< endl; hSigSub[ih]->SetBinContent(ibin+1,0.);}
    } 
    /// 5) normalize again by integral
		hSig[ih]->Scale(1/hSig[ih]->Integral());
		hSigSub[ih]->Scale(1/hSigSub[ih]->Integral());
		hBkg[ih]->Scale(1/hBkg[ih]->Integral());
		hPRMC[ih]->Scale(1/hPRMC[ih]->Integral());
		
		c1[ih] = new TCanvas(Form("c1_%s",histName[ih].Data()),"",600,600);
		c1[ih]->cd();
    if (strcmp(histName[ih],"dxy")==0 || strcmp(histName[ih],"dz")==0) {gPad->SetLogy(1);}
    else { gPad->SetLogy(0);}
    SetHistStyle(hSigSub[ih],1,0);
    SetHistStyle(hBkg[ih],2,10);
    SetHistStyle(hPRMC[ih],4,0);
		hPRMC[ih]->SetFillColor(kGreen+9);
		hPRMC[ih]->SetFillStyle(3003);
		hPRMC[ih]->SetAxisRange(ymin[ih],ymax[ih],"Y");
		hPRMC[ih]->Draw("hist");
		hSigSub[ih]->Draw("pe same");
		hBkg[ih]->Draw("pe same");
    if (ih==nHist-1){
      legUR->AddEntry(hSigSub[ih],"RD: Signal","lp");
      legUR->AddEntry(hSigSub[ih],"(Bkg subtracted)","");
      legUR->AddEntry(hBkg[ih],"RD: Background","lp");
      legUR->AddEntry(hPRMC[ih],"MC","f");
			legUR->Draw();
		}
		//// line for actual cut regions
		dashedLine(xmin[ih],ymin[ih],xmin[ih],ymax[ih]);
		dashedLine(xmax[ih],ymin[ih],xmax[ih],ymax[ih]);
		//// check integral over cut regions
		double nsig=-999, nmc=-999, nratio=-999;
		nsig = calculInteg(hSigSub[ih],xmin[ih],xmax[ih]);
		nmc = calculInteg(hPRMC[ih],xmin[ih],xmax[ih]);
		//nratio = nsig/nmc;
    //nratio = (1.-nsig)/(1.-nmc);
    nratio = (1.-nmc)/(1.-nsig);
    cout << "nsig = " << nsig << ", nmc = " << nmc << ", nratio = " << nratio << endl;
    //latex->DrawLatex(0.16,0.80,"#int_{out}Data/#int_{out}MC");    
    //latex->DrawLatex(0.16,0.70,Form("= %.3f / %.3f = %.2f",1.-nsig,1.-nmc,nratio));    
    latex->DrawLatex(0.16,0.80,"#int_{out}MC/#int_{out}Data");    
    latex->DrawLatex(0.16,0.70,Form("= %.3f / %.3f = %.2f",1.-nmc,1.-nsig,nratio));    
		c1[ih]->SaveAs(Form("dir_muID/%s_muIDvariables_%s_bkgSubtracted.pdf",szPA.Data(),histName[ih].Data()));
	}
	
	return 0;	

}

double calculInteg( TH1 *a=0, double minX=-999.21231, double maxX=-999.21231)
{
  float fac=0;
  int lowBin=1;
  int highBin=a->GetNbinsX();
  if ( minX != -999.21231)
    lowBin = a->FindBin(minX+0.000001);
  if ( maxX != -999.21231)
    highBin=a->FindBin(maxX-0.0000001);

  return fac =  a->Integral( lowBin, highBin);
}

