#include "../SONGKYO.h"

int rootAna_dimuVariables(TString szPA = "pa", bool isScaled = true, bool isSmall =true){

  gROOT->Macro("../Style.C");
  
  const int nEntry = 10000; // for isSmall
  double rapmin = -2.4;
  double rapmax = 2.4;
  double ptmin = 0;
  double ptmax = 30;
    
  //// read-in root file
  TFile *fData;
  TFile *fPRMC;
  TFile *fNPMC;
  TTree *treeData;
  TTree *treePRMC;
  TTree *treeNPMC;
  if (strcmp(szPA,"pp")==0) {
    fData = new TFile("/storage/OniaTree/Onia5TeV/ppData/OniaTree_DoubleMu_Run2015E-PromptReco-v1_Run_262157_262328_noCUT_TRKMU.root");
    fPRMC = new TFile("/storage/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");
    fNPMC = new TFile("/storage/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_BJpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");
    treeData = (TTree*)fData->Get("hionia/myTree");
    treePRMC = (TTree*)fPRMC->Get("hionia/myTree");
    treeNPMC = (TTree*)fNPMC->Get("hionia/myTree");
  } else if (strcmp(szPA,"pa")==0) {
    fData = new TFile("/home/songkyo/kyo/pPbDataSample/Data/RD2013_pa_1st_run_210676-211256_GR_P_V43D_nocut.root");
    fPRMC = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCJPsiWithFSR_pa_1st_run_STARTHI53_V27_ext1_nocut.root");
    fNPMC = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCinclBtoJPsiMuMu_pa_1st_run_STARTHI53_V27_ext1_nocut.root");
    treeData = (TTree*)fData->Get("myTree");
    treePRMC = (TTree*)fPRMC->Get("myTree");
    treeNPMC = (TTree*)fNPMC->Get("myTree");
  }
  else { cout << "WARNING :: Select among szPA = \"pp\" or \"pa\" "; return 0; }
  cout << "szPA : " << szPA <<endl;
  
  //// define 1D hist
  //const int nbin[] = {80, 80, 80, 50};
  //const double minbin[] = {2.6, 0.0, -3.0, -3.14};
  //const double maxbin[] = {3.5, 30.0, 2.0, 3.14};
  //TString histName[] = {"M", "Pt", "Rapidity","Phi"};
  //TString histTitle[] = {"mass (GeV/c^{2})", "p_{T} (GeV/c)", "y_{CM}", "#phi (rad)"};
  const int nbin[] = {80, 80, 50};
  const double minbin[] = {0.0, -2.4, -3.14};
  const double maxbin[] = {30.0, 2.4, 3.14};
  TString histName[] = {"Pt", "Rapidity","Phi"};
  TString histTitle[] = {"p_{T} (GeV/c)", "y_{lab}", "#phi (rad)"};
  
  const int nHist = sizeof(nbin)/sizeof(int);
  cout << "nHist = "<<nHist<<endl;

  //mass distibution for yield checks
  TH1D *hMassData = new TH1D("hMassData","dimuon mass (GeV/c^{2})",45, 2.6, 3.5);; 
  TH1D *hMassPRMC = new TH1D("hMassPRMC","dimuon mass (GeV/c^{2})",45, 2.6, 3.5);;
  TH1D *hMassNPMC = new TH1D("hMassNPMC","dimuon mass (GeV/c^{2})",45, 2.6, 3.5);;
  hMassData->Sumw2();
  hMassPRMC->Sumw2();
  hMassNPMC->Sumw2();

  TH1D *hSig[nHist];
  TH1D *hBkg[nHist];
  TH1D *hPRMC[nHist];
  TH1D *hNPMC[nHist];

  ///////////////////////////////////////////////////////////////////
  //// cut definitions
  TCut trigCut = "((Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )";
  TCut recoPairCut = "Reco_QQ_sign==0 && Reco_QQ_VtxProb > 0.01";  
  TCut massNarrow = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
  TCut massWide = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
  TCut ctauCut = "-1.5 < Reco_QQ_ctau && Reco_QQ_ctau < 3.0 && 0 < Reco_QQ_ctauErr && Reco_QQ_ctauErr < 1";
  TCut rapRange = Form("Reco_QQ_4mom.Rapidity() > %f && Reco_QQ_4mom.Rapidity() < %f",rapmin,rapmax);
  TCut ptRange = Form("Reco_QQ_4mom.Pt() > %f && Reco_QQ_4mom.Pt() < %f",ptmin,ptmax);
  
  //// new acc cut
  TCut accNewRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 1.2 && Reco_QQ_mupl_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.1 && Reco_QQ_mupl_4mom.Pt() >= 3.93-1.11*TMath::Abs(Reco_QQ_mupl_4mom.Eta())) || (2.1 <= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt() >= 1.3)))";
  TCut accNewRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.4 && ((TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 1.2 && Reco_QQ_mumi_4mom.Pt() >=3.3) || (1.2 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.1 && Reco_QQ_mumi_4mom.Pt() >= 3.93-1.11*TMath::Abs(Reco_QQ_mumi_4mom.Eta())) || (2.1 <= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt() >= 1.3)))";

  // muID
  TCut muIDPlus;
  TCut muIDMinus;
  if(strcmp(szPA,"pp")==0) {
    muIDPlus = "Reco_QQ_mupl_highPurity && Reco_QQ_mupl_TrkMuArb && Reco_QQ_mupl_TMOneStaTight && Reco_QQ_mupl_nTrkWMea > 5 && Reco_QQ_mupl_nPixWMea > 0 &&  TMath::Abs(Reco_QQ_mupl_dxy) < 0.3 && TMath::Abs(Reco_QQ_mupl_dz) < 20.";
    muIDMinus = "Reco_QQ_mumi_highPurity && Reco_QQ_mumi_TrkMuArb && Reco_QQ_mumi_TMOneStaTight && Reco_QQ_mumi_nTrkWMea > 5 && Reco_QQ_mumi_nPixWMea > 0 && TMath::Abs(Reco_QQ_mumi_dxy) < 0.3 && TMath::Abs(Reco_QQ_mumi_dz) < 20.";
  }
  else if (strcmp(szPA,"pa")==0) {
    muIDPlus = "Reco_QQ_mupl_isHighPurity && Reco_QQ_mupl_TrkMuArb && Reco_QQ_mupl_TMOneStaTight && Reco_QQ_mupl_nTrkWMea > 5 && Reco_QQ_mupl_nPixWMea > 0 &&  TMath::Abs(Reco_QQ_mupl_dxy) < 0.3 && TMath::Abs(Reco_QQ_mupl_dz) < 20.";
    muIDMinus = "Reco_QQ_mumi_isHighPurity && Reco_QQ_mumi_TrkMuArb && Reco_QQ_mumi_TMOneStaTight && Reco_QQ_mumi_nTrkWMea > 5 && Reco_QQ_mumi_nPixWMea > 0 && TMath::Abs(Reco_QQ_mumi_dxy) < 0.3 && TMath::Abs(Reco_QQ_mumi_dz) < 20.";
  }

  TCut totalCut = trigCut && recoPairCut && ctauCut && rapRange && ptRange && accNewRecoPlus && accNewRecoMinus && muIDPlus && muIDMinus; // without Mass

  ///////////////////////////////////////////////////////////////////
  //// Fill hist.
  TCanvas* ctmp = new TCanvas("ctmp","",200,200);
  //// 1) mass distributions
  if (isSmall) { 
    treeData->Draw("Reco_QQ_4mom.M()>>hMassData",totalCut && massWide,"",nEntry);
    treePRMC->Draw("Reco_QQ_4mom.M()>>hMassPRMC",totalCut && massWide,"",nEntry);
    treeNPMC->Draw("Reco_QQ_4mom.M()>>hMassNPMC",totalCut && massWide,"",nEntry);
  }
  else {
    treeData->Draw("Reco_QQ_4mom.M()>>hMassData",totalCut && massWide,"");
    treePRMC->Draw("Reco_QQ_4mom.M()>>hMassPRMC",totalCut && massWide,"");
    treeNPMC->Draw("Reco_QQ_4mom.M()>>hMassNPMC",totalCut && massWide,"");
  }
  if (isScaled){ 
    hMassData->Scale(1/hMassData->Integral());
    hMassPRMC->Scale(1/hMassPRMC->Integral());
    hMassNPMC->Scale(1/hMassNPMC->Integral());
  }

  //// 2) actual variables distributions
  for (int ih=0; ih<nHist; ih++) {
    hSig[ih] = new TH1D(Form("hSig_%s",histName[ih].Data()),Form(";%s;",histTitle[ih].Data()),nbin[ih],minbin[ih],maxbin[ih]);
    hBkg[ih] = new TH1D(Form("hBkg_%s",histName[ih].Data()),Form(";%s;",histTitle[ih].Data()),nbin[ih],minbin[ih],maxbin[ih]);
    hPRMC[ih] = new TH1D(Form("hPRMC_%s",histName[ih].Data()),Form(";%s;",histTitle[ih].Data()),nbin[ih],minbin[ih],maxbin[ih]);
    hNPMC[ih] = new TH1D(Form("hNPMC_%s",histName[ih].Data()),Form(";%s;",histTitle[ih].Data()),nbin[ih],minbin[ih],maxbin[ih]);
    hSig[ih]->Sumw2();
    hBkg[ih]->Sumw2();
    hPRMC[ih]->Sumw2();
    hNPMC[ih]->Sumw2();
    std::cout << ih <<"th hist name = " << histName[ih].Data() << std::endl;
    if (isSmall) {
      /*if (strcmp(histName[ih],"Rapidity")==0) { // rapidity :: convert y_lab^1st to y_CM
        treeData->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hSig[ih]->GetName()),totalCut && massNarrow,"",nEntry);
        treeData->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hBkg[ih]->GetName()),totalCut && massWide && (!massNarrow),"",nEntry);
        treePRMC->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hPRMC[ih]->GetName()),totalCut && massNarrow,"",nEntry);
        treeNPMC->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hNPMC[ih]->GetName()),totalCut && massNarrow,"",nEntry);
      }*/
      //else {
        treeData->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hSig[ih]->GetName()),totalCut && massNarrow,"",nEntry);
        treeData->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hBkg[ih]->GetName()),totalCut && massWide && (!massNarrow),"",nEntry);
        treePRMC->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hPRMC[ih]->GetName()),totalCut && massNarrow,"",nEntry);
        treeNPMC->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hNPMC[ih]->GetName()),totalCut && massNarrow,"",nEntry);
      //}
    }
    else {
      /*if (strcmp(histName[ih],"Rapidity")==0) { // rapidity :: convert y_lab^1st to y_CM
        treeData->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hSig[ih]->GetName()),totalCut && massNarrow,"");
        treeData->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hBkg[ih]->GetName()),totalCut && massWide && (!massNarrow),"");
        treePRMC->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hPRMC[ih]->GetName()),totalCut && massNarrow,"");
        treeNPMC->Draw(Form("-0.47-1.*Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hNPMC[ih]->GetName()),totalCut && massNarrow,"");
      }*/
      //else {
        treeData->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hSig[ih]->GetName()),totalCut && massNarrow,"");
        treeData->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hBkg[ih]->GetName()),totalCut && massWide && (!massNarrow),"");
        treePRMC->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hPRMC[ih]->GetName()),totalCut && massNarrow,"");
        treeNPMC->Draw(Form("Reco_QQ_4mom.%s()>>%s",histName[ih].Data(),hNPMC[ih]->GetName()),totalCut && massNarrow,"");
      //}
    }
    
    std::cout << histName[ih].Data() <<" : SigPbp integral = " << hSig[ih]->Integral() << std::endl;
    std::cout << histName[ih].Data() <<" : BkgPbp integral = " << hBkg[ih]->Integral() << std::endl;
    std::cout << histName[ih].Data() <<" : PRMC integral = " << hPRMC[ih]->Integral() << std::endl;
    std::cout << histName[ih].Data() <<" : NPMC integral = " << hNPMC[ih]->Integral() << std::endl;
    //// scaled
    if (isScaled){
      hSig[ih]->Scale(1/hSig[ih]->Integral());
      hBkg[ih]->Scale(1/hBkg[ih]->Integral());
      hPRMC[ih]->Scale(1/hPRMC[ih]->Integral());
      hNPMC[ih]->Scale(1/hNPMC[ih]->Integral());
    } 
  }    

  ////////////////////////////////////////////////////////////////////// 
  //// Draw hist.
  TCanvas* c1[nHist];
  TLegend *legUR = new TLegend(0.58,0.68,0.90,0.90,NULL,"brNDC");
  TLegend *legUM = new TLegend(0.33,0.68,0.68,0.90,NULL,"brNDC");
  TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
  TLegend *legBM = new TLegend(0.30,0.20,0.65,0.42,NULL,"brNDC");
  SetLegendStyle(legUR);
  SetLegendStyle(legUM);
  SetLegendStyle(legUL);
  SetLegendStyle(legBM);

  for (int ih=0; ih<nHist; ih++) {
    c1[ih] = new TCanvas(Form("c1_%s",histName[ih].Data()),"",600,600);
    c1[ih]->cd();
    if (strcmp(histName[ih],"Pt")==0) {gPad->SetLogy(1);}
    else { gPad->SetLogy(0); }
    SetHistStyle(hSig[ih],1,0);
    SetHistStyle(hBkg[ih],2,10);
    SetHistStyle(hPRMC[ih],4,0);
    SetHistStyle(hNPMC[ih],3,0);
    hPRMC[ih]->SetFillColor(kGreen+9);
    hPRMC[ih]->SetFillStyle(3003);
    hNPMC[ih]->SetFillColor(kOrange+1);
    hNPMC[ih]->SetFillStyle(3005);
    hPRMC[ih]->Draw("hist");
    hNPMC[ih]->Draw("hist same");
    hSig[ih]->Draw("pe same");
    hBkg[ih]->Draw("pe same");
    if (ih==nHist-1) {
      legUR->AddEntry(hSig[ih],"RD: mass region","lp");
      legUR->AddEntry(hBkg[ih],"RD : sideband","lp");
      legUR->AddEntry(hPRMC[ih],"prompt MC","l");
      legUR->AddEntry(hNPMC[ih],"non-prompt MC","l");
      legUR->Draw();
    }
    c1[ih]->SaveAs(Form("dir_dimu/%s_dimuVariables_%s_isScaled%d.pdf",szPA.Data(),histName[ih].Data(),(int)isScaled));
  }

  //Save as a root file
  TFile* outFile = new TFile(Form("dir_dimu/%s_dimuVariables_isScaled%d.root",szPA.Data(),(int)isScaled),"RECREATE");
  outFile->cd();
  hMassData->Write();
  hMassPRMC->Write();
  hMassNPMC->Write();
  for (int ih=0; ih<nHist; ih++) {
    hSig[ih]->Write();
    hBkg[ih]->Write();
    hPRMC[ih]->Write();
    hNPMC[ih]->Write();
  } 
  outFile->Close();

  return 0; 

}

