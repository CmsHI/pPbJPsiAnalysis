#include "../SONGKYO.h"

double calculInteg( TH1 *a, double minX, double maxX);

//szPA = "pp" or "pa"
int rootAna_muIDvariables(TString szPA = "pa", bool isScaled = true, bool isSmall =true){

  using namespace std;
  //// # of event range ( for isSmall)
  //const int nEntry = 100000;
  //const int nEntry = 5000;
  const int nEntry = 100000;
  gROOT->Macro("../Style.C");

  //// read-in root file
  TFile *fData;
  TFile *fPRMC;
  TTree *treeData;
  TTree *treePRMC;
  if (strcmp(szPA,"pp")==0) {
    fData = new TFile("/storage/OniaTree/Onia5TeV/ppData/OniaTree_DoubleMu_Run2015E-PromptReco-v1_Run_262157_262328_noCUT_TRKMU.root");
    fPRMC = new TFile("/storage/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");
    treeData = (TTree*)fData->Get("hionia/myTree");
    treePRMC = (TTree*)fPRMC->Get("hionia/myTree");
  } else if (strcmp(szPA,"pa")==0) {
    fData = new TFile("/home/songkyo/kyo/pPbDataSample/Data/RD2013_pa_1st_run_210676-211256_GR_P_V43D_nocut.root");
    fPRMC = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCJPsiWithFSR_pa_1st_run_STARTHI53_V27_ext1_nocut.root");
    treeData = (TTree*)fData->Get("myTree");
    treePRMC = (TTree*)fPRMC->Get("myTree");
  }
  else { cout << "WARNING :: Select among szPA = \"pp\" or \"pa\" "; return 0; }

  cout << "szPA : " << szPA <<endl;
  double rapmin = -2.4;
  double rapmax = 2.4;
  double ptmin = 0;
  double ptmax = 30;
  //if (strcmp (szPA,"ptJpsi36")==0) {ptmin = 3.; ptmax = 6.;}
  
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

  // for yields in each mass range
  //TH1D *hData_M = new TH1D("hData_M","dimuon mass (GeV/c^{2})",45,2.6,3.5); //whole
  //TH1D *hSig_M = new TH1D("hSig_M","dimuon mass (GeV/c^{2})",45,2.6,3.5);; // in 2.9 - 3.3 GeV
  //TH1D *hBkg_M = new TH1D("hBkg_M","dimuon mass (GeV/c^{2})",45,2.6,3.5);; // out of 2.9 - 3.3 GeV
  //TH1D *hPRMC_M = new TH1D("hPRMC_M","dimuon mass (GeV/c^{2})",45,2.6,3.5);; // in 2.9 - 3.3 GeV

  TH1D *hMass[nHist]; //mass distibution for yield checks
  TH1D *hSig[nHist]; // in 2.9 - 3.3 GeV
  TH1D *hBkg[nHist]; // out of 2.9 - 3.3 GeV
  TH1D *hPRMC[nHist]; // in 2.9 - 3.3 GeV

  ////////// 1) general cut definitions
  //TCut trigCut = "((Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )"; //L1DoubleMu0
  TCut recoPairCut = "Reco_QQ_sign==0 && Reco_QQ_VtxProb > 0.01";
  TCut massNarrow = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
  TCut massWide = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
  //TCut ctauCut = "-1.5 < Reco_QQ_ctau && Reco_QQ_ctau < 3.0 && 0 < Reco_QQ_ctauErr && Reco_QQ_ctauErr < 1";
  TCut rapRange = Form("Reco_QQ_4mom.Rapidity() > %f && Reco_QQ_4mom.Rapidity() < %f",rapmin,rapmax);
  TCut ptRange = Form("Reco_QQ_4mom.Pt() > %f && Reco_QQ_4mom.Pt() < %f",ptmin,ptmax);
  
  //// new cut
  TCut accNewRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 1.2 && Reco_QQ_mupl_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.1 && Reco_QQ_mupl_4mom.Pt() >= 3.93-1.11*TMath::Abs(Reco_QQ_mupl_4mom.Eta())) || (2.1 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt() >= 1.3)))";
  TCut accNewRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 1.2 && Reco_QQ_mumi_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.1 && Reco_QQ_mumi_4mom.Pt() >= 3.93-1.11*TMath::Abs(Reco_QQ_mumi_4mom.Eta())) || (2.1 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt() >= 1.3)))";

  TCut totalCut = recoPairCut && rapRange && ptRange && accNewRecoPlus && accNewRecoMinus;
  TCut totalCutMC = recoPairCut && rapRange && ptRange && accNewRecoPlus && accNewRecoMinus;

  ////////// 2) muID cut definition
  //// 2-1) mupl : each component
  TCut c_mupl[nHist];
  if(strcmp(szPA,"pp")==0) {c_mupl[0] = "Reco_QQ_mupl_highPurity==1";}
  else if (strcmp(szPA,"pa")==0) { c_mupl[0] = "Reco_QQ_mupl_isHighPurity==1";}
  c_mupl[1] = "Reco_QQ_mupl_TrkMuArb == 1";
  c_mupl[2] = "Reco_QQ_mupl_TMOneStaTight == 1";
  c_mupl[3] = "Reco_QQ_mupl_nTrkWMea > 5";
  c_mupl[4] = "Reco_QQ_mupl_nPixWMea > 0";
  c_mupl[5] = "TMath::Abs(Reco_QQ_mupl_dxy) < 0.3";
  c_mupl[6] = "TMath::Abs(Reco_QQ_mupl_dz) < 20.";
  //// 2-2) mupl : compose total cuts (except for the one to be drawn) 
  TCut c_mupl_tot[nHist];
  for (int ih=0; ih<nHist; ih++) {
    for (int jh=0; jh<5; jh++) {
      if (ih!=jh) c_mupl_tot[ih] += c_mupl[jh];
    }
    cout << ih << "th c_mupl_tot = " << c_mupl_tot[ih] << endl;
  }
  //// 2-3) mumi : each component
  TCut c_mumi[nHist];
  if(strcmp(szPA,"pp")==0) {c_mumi[0] = "Reco_QQ_mumi_highPurity==1";}
  else if (strcmp(szPA,"pa")==0) { c_mumi[0] = "Reco_QQ_mumi_isHighPurity==1";}
  c_mumi[1] = "Reco_QQ_mumi_TrkMuArb == 1";
  c_mumi[2] = "Reco_QQ_mumi_TMOneStaTight == 1";
  c_mumi[3] = "Reco_QQ_mumi_nTrkWMea > 5";
  c_mumi[4] = "Reco_QQ_mumi_nPixWMea > 0";
  c_mumi[5] = "TMath::Abs(Reco_QQ_mumi_dxy) < 0.3";
  c_mumi[6] = "TMath::Abs(Reco_QQ_mumi_dz) < 20.";
  //// 2-4) mumi : compose total cuts (except for the one to be drawn) 
  TCut c_mumi_tot[nHist];
  for (int ih=0; ih<nHist; ih++) {
    for (int jh=0; jh<5; jh++) {
      if (ih!=jh) c_mumi_tot[ih] += c_mumi[jh];
    }
    cout << ih << "th c_mumi_tot = " << c_mumi_tot[ih] << endl;
  }
  
  /////////////////////////////////////////////////////////////////////////
  //// fill mass histograms
  //// fill muID histograms
  TCanvas* ctmp = new TCanvas("ctmp","",200,200);
  for (int ih=0; ih<nHist; ih++) {
    hMass[ih] = new TH1D(Form("hMass_%s",histName[ih].Data()),"dimuon mass (GeV/c^{2})",45, 2.6, 3.5);
    hSig[ih] = new TH1D(Form("hSig_%s",histName[ih].Data()),Form(";%s;",histTitle[ih].Data()),nbin[ih],minbin[ih],maxbin[ih]);
    hBkg[ih] = new TH1D(Form("hBkg_%s",histName[ih].Data()),Form(";%s;",histTitle[ih].Data()),nbin[ih],minbin[ih],maxbin[ih]);
    hPRMC[ih] = new TH1D(Form("hPRMC_%s",histName[ih].Data()),Form(";%s;",histTitle[ih].Data()),nbin[ih],minbin[ih],maxbin[ih]);
    hMass[ih]->Sumw2();
    hSig[ih]->Sumw2();
    hBkg[ih]->Sumw2();
    hPRMC[ih]->Sumw2();
    std::cout << ih <<"th hist name = " << histName[ih].Data() << std::endl;
    if (isSmall) {
      //treeData->Draw(Form("Reco_QQ_4mom.M()>>%s",hMass[ih]->GetName()),totalCut && massWide && c_mupl_tot[ih],"",nEntry);
      //treeData->Draw(Form("Reco_QQ_4mom.M()>>+%s",hMass[ih]->GetName()),totalCut && massWide && c_mumi_tot[ih],"",nEntry);
      treeData->Draw(Form("Reco_QQ_4mom.M()>>%s",hMass[ih]->GetName()),totalCut && massWide && c_mupl_tot[ih] && c_mumi_tot[ih],"",nEntry);
      treeData->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[ih].Data(),hSig[ih]->GetName()),totalCut && massNarrow && c_mupl_tot[ih],"",nEntry);
      treeData->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[ih].Data(),hBkg[ih]->GetName()),totalCut && massWide && (!massNarrow) && c_mupl_tot[ih],"",nEntry);
      treePRMC->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[ih].Data(),hPRMC[ih]->GetName()),totalCut && massNarrow && c_mupl_tot[ih],"",nEntry);
      treeData->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[ih].Data(),hSig[ih]->GetName()),totalCut && massNarrow && c_mumi_tot[ih],"",nEntry);
      treeData->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[ih].Data(),hBkg[ih]->GetName()),totalCut && massWide && (!massNarrow) && c_mumi_tot[ih],"",nEntry);
      treePRMC->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[ih].Data(),hPRMC[ih]->GetName()),totalCut && massNarrow && c_mumi_tot[ih],"",nEntry);
    }
    else {
      //treeData->Draw(Form("Reco_QQ_4mom.M()>>%s",hMass[ih]->GetName()),totalCut && massWide && c_mupl_tot[ih],"");
      //treeData->Draw(Form("Reco_QQ_4mom.M()>>+%s",hMass[ih]->GetName()),totalCut && massWide && c_mumi_tot[ih],"");
      treeData->Draw(Form("Reco_QQ_4mom.M()>>%s",hMass[ih]->GetName()),totalCut && massWide && c_mupl_tot[ih] && c_mumi_tot[ih],"");
      treeData->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[ih].Data(),hSig[ih]->GetName()),totalCut && massNarrow && c_mupl_tot[ih],"");
      treeData->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[ih].Data(),hBkg[ih]->GetName()),totalCut && massWide && (!massNarrow) && c_mupl_tot[ih],"");
      treePRMC->Draw(Form("Reco_QQ_mupl_%s>>%s",histName[ih].Data(),hPRMC[ih]->GetName()),totalCut && massNarrow && c_mupl_tot[ih],"");
      treeData->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[ih].Data(),hSig[ih]->GetName()),totalCut && massNarrow && c_mumi_tot[ih],"");
      treeData->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[ih].Data(),hBkg[ih]->GetName()),totalCut && massWide && (!massNarrow) && c_mumi_tot[ih],"");
      treePRMC->Draw(Form("Reco_QQ_mumi_%s>>+%s",histName[ih].Data(),hPRMC[ih]->GetName()),totalCut && massNarrow && c_mumi_tot[ih],"");
    }
    std::cout << histName[ih] <<" : Mass integral = " << hMass[ih]->Integral() << std::endl;
    std::cout << histName[ih] <<" : Sig integral = " << hSig[ih]->Integral() << std::endl;
    std::cout << histName[ih] <<" : Bkg integral = " << hBkg[ih]->Integral() << std::endl;
    std::cout << histName[ih] <<" : PRMC integral = " << hPRMC[ih]->Integral() << std::endl;
    //// scaled
    if (isScaled){
      //hMass[ih]->Scale(1/hMass[ih]->Integral());
      hSig[ih]->Scale(1/hSig[ih]->Integral());
      hBkg[ih]->Scale(1/hBkg[ih]->Integral());
      hPRMC[ih]->Scale(1/hPRMC[ih]->Integral());
    }
  }

  //////////////////////////////////////////////////////////////////////
  //// Draw  histograms
  TCanvas* c1[nHist];
  //TLegend *legUR = new TLegend(0.58,0.68,0.90,0.90,NULL,"brNDC");
  //TLegend *legUR = new TLegend(0.58,0.72,0.90,0.90,NULL,"brNDC");
	TLegend *legUR = new TLegend(0.58,0.75,0.88,0.90,NULL,"brNDC");
  SetLegendStyle(legUR);
  legUR->SetTextSize(0.031);
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextFont(42);
  latex->SetTextSize(0.04);

  for (int ih=0; ih<nHist; ih++) { 
    c1[ih] = new TCanvas(Form("c1_%s",histName[ih].Data()),"",600,600);
    c1[ih]->cd();
    if (strcmp(histName[ih],"dxy")==0 || strcmp(histName[ih],"dz")==0) {gPad->SetLogy(1);}
    else { gPad->SetLogy(0);}
    SetHistStyle(hSig[ih],1,0);
    SetHistStyle(hBkg[ih],2,10);
    SetHistStyle(hPRMC[ih],4,0);
    hPRMC[ih]->SetFillColor(kGreen+9);
    hPRMC[ih]->SetFillStyle(3003);
    if (isScaled) hPRMC[ih]->SetAxisRange(ymin[ih],ymax[ih],"Y");
    hPRMC[ih]->Draw("hist");
    hSig[ih]->Draw("pe same");
    hBkg[ih]->Draw("pe same");
    if (ih==nHist-1){
      legUR->AddEntry(hSig[ih],"RD: mass region","lp");
      legUR->AddEntry(hBkg[ih],"RD: sideband","lp");
      legUR->AddEntry(hPRMC[ih],"MC: mass region","f");
      legUR->Draw();
    }
    //// line for actual cut regions
    dashedLine(xmin[ih],ymin[ih],xmin[ih],ymax[ih]);
    dashedLine(xmax[ih],ymin[ih],xmax[ih],ymax[ih]);
    //// check integral over cut regions
    double nsig=-999, nmc=-999, nratio=-999;
    nsig = calculInteg(hSig[ih],xmin[ih],xmax[ih]);
    nmc = calculInteg(hPRMC[ih],xmin[ih],xmax[ih]);
    //nratio = nsig/nmc;
    nratio = (1.-nsig)/(1.-nmc);
    cout << "nsig = " << nsig << ", nmc = " << nmc << ", nratio = " << nratio << endl;
    latex->DrawLatex(0.16,0.80,"#int_{out}Data/#int_{out}MC");    
    latex->DrawLatex(0.16,0.70,Form("= %.3f / %.3f = %.2f",1.-nsig,1.-nmc,nratio));    
    c1[ih]->SaveAs(Form("dir_muID/%s_muIDvariables_%s_isScaled%d.pdf",szPA.Data(),histName[ih].Data(),(int)isScaled));
  }

  //Save as a root file
  TFile* outFile = new TFile(Form("dir_muID/%s_muIDvariables_isScaled%d.root",szPA.Data(),(int)isScaled),"RECREATE");
  outFile->cd();
  for (int ih=0; ih<nHist; ih++) {
    hMass[ih]->Write();
    hSig[ih]->Write();
    hBkg[ih]->Write();
    hPRMC[ih]->Write();
  } 
  outFile->Close();

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

/*
void scaleInt( TH1 *a=0, double normFac=1., double minX=-999.21231, double maxX=-999.21231)
{
  float fac=0;
  int lowBin=1;
  int highBin=a->GetNbinsX();
  if ( minX != -999.21231)
    lowBin = a->FindBin(minX);
  if ( maxX != -999.21231)
    highBin=a->FindBin(maxX);

  fac =  a->Integral( lowBin, highBin);
  if ( fac>0) a->Scale(normFac/fac);
}
*/
