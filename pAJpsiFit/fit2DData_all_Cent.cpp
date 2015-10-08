#include <fit2DData.h>

int main (int argc, char* argv[]) {
  RooMsgService::instance().getStream(0).removeTopic(Plotting);
  RooMsgService::instance().getStream(0).removeTopic(InputArguments);
  RooMsgService::instance().getStream(1).removeTopic(InputArguments);
  RooMsgService::instance().getStream(1).removeTopic(Plotting);
  RooMsgService::instance().getStream(1).removeTopic(NumIntegration);
  RooMsgService::instance().getStream(1).removeTopic(Minimization);
  RooMsgService::instance().getStream(1).removeTopic(Caching);
  
	gROOT->Macro("/afs/cern.ch/work/k/kyolee/private/cms442/src/JpsiStyle.C");
  
  // *** Check options
  parseInputArg(argc, argv, inOpt);

  float pmin=0, pmax=0, ymin=0, ymax=0, lmin=0, lmax=0, cmin=0, cmax=0, psmax=0, psmin=0, errmin=0, errmax=0;
  float etmin=0, etmax=0, ntrmin=0, ntrmax=0; // ETHF & Ntrk
	getOptRange(inOpt.prange,&pmin,&pmax);
  getOptRange(inOpt.lrange,&lmin,&lmax);
  getOptRange(inOpt.errrange,&errmin,&errmax);
  getOptRange(inOpt.crange,&cmin,&cmax);
  getOptRange(inOpt.yrange,&ymin,&ymax);
  getOptRange(inOpt.phirange,&psmin,&psmax);
  getOptRange(inOpt.etrange,&etmin,&etmax);
  getOptRange(inOpt.ntrrange,&ntrmin,&ntrmax);
	

  // *** Strings for plot formatting
  formTitle(inOpt, cmin, cmax);
  formRapidity(inOpt, ymin, ymax);
  formPt(inOpt, pmin, pmax);
	formEt(inOpt, etmin, etmax);
	formNtrk(inOpt, ntrmin, ntrmax);
  formPhi(inOpt, psmin, psmax);


  TLatex *t = new TLatex();  t->SetNDC();  t->SetTextAlign(12);

  // Global objects for drawing
  Double_t fx[2], fy[2], fex[2], fey[2];
  gfake1 = new TGraphErrors(2,fx,fy,fex,fey);
  gfake1->SetMarkerStyle(20); gfake1->SetMarkerSize(1);
  hfake11 = TH1F("hfake11","hfake1",100,200,300);
  hfake11.SetLineColor(kBlue); hfake11.SetLineWidth(4); hfake11.SetLineStyle(7); hfake11.SetFillColor(kAzure-9); hfake11.SetFillStyle(1001);
  hfake21 = TH1F("hfake21","hfake2",100,200,300);
  hfake21.SetLineColor(kBlack); hfake21.SetLineWidth(4); hfake21.SetFillColor(kBlack); hfake21.SetFillStyle(3354);
  hfake31 = TH1F("hfake31","hfake3",100,200,300);
  hfake31.SetLineColor(kRed); hfake31.SetMarkerStyle(kCircle); hfake31.SetLineWidth(4); hfake31.SetMarkerColor(kRed); hfake31.SetLineStyle(9); hfake31.SetFillColor(kRed-7); hfake31.SetFillStyle(3444);
  hfake311 = TH1F("hfake311","hfake311",100,200,300);
  hfake311.SetLineColor(kRed); hfake311.SetMarkerStyle(kCircle); hfake311.SetLineWidth(4); hfake311.SetMarkerColor(kRed); hfake311.SetLineStyle(kDashed); hfake311.SetFillColor(kRed-7); hfake311.SetFillStyle(3444);
  hfake41 = TH1F("hfake41","hfake4",100,200,300);
  hfake41.SetLineColor(kGreen); hfake41.SetMarkerStyle(kCircle); hfake41.SetLineWidth(4); hfake41.SetMarkerColor(kGreen); hfake41.SetLineStyle(kDashDotted); hfake41.SetFillColor(kGreen-7); hfake41.SetFillStyle(3444);

  // *** Read MC and Data files
  TFile fInMC(inOpt.FileNameMC1.c_str());   //Non-prompt J/psi MC
  cout << inOpt.FileNameMC1.c_str() << endl;
  if (fInMC.IsZombie()) { cout << "CANNOT open MC1 root file\n"; return 1; }
  fInMC.cd();
  RooDataSet *dataMC = (RooDataSet*)fInMC.Get("dataJpsi");
  dataMC->SetName("dataMC");

  TFile fInMC2(inOpt.FileNameMC2.c_str());  //Prompt J/psi MC
  cout << inOpt.FileNameMC2.c_str() << endl;
  if (fInMC2.IsZombie()) { cout << "CANNOT open MC2 root file\n"; return 1; }
  fInMC2.cd();
  RooDataSet *dataMC2 = (RooDataSet*)fInMC2.Get("dataJpsi");
  dataMC2->SetName("dataMC2");

  TFile fInData(inOpt.FileName.c_str());
  cout << inOpt.FileName.c_str() << endl;
  if (fInData.IsZombie()) { cout << "CANNOT open data root file\n"; return 1; }
  fInData.cd();
  RooDataSet *data;
  if (inOpt.doWeight == 1) {
    data = (RooDataSet*)fInData.Get("dataJpsiWeight");  //Weighted
    cout << "## WEIGHTED dataset is used\n";
  } else {
    data = (RooDataSet*)fInData.Get("dataJpsi");  //Unweighted
    cout << "## UN-WEIGHTED dataset is used!\n";
  }
  data->SetName("data");

	TFile* fInData2;
	if (inOpt.dataMerge) {
		fInData2 = new TFile(inOpt.FileName2.c_str());
		cout << "## NOTICE : 2nd data to be merged\n";
		cout << inOpt.FileName2.c_str() << endl;
  	if (fInData2->IsZombie()) { cout << "CANNOT open data root file 2\n"; return 1; }
  	fInData2->cd();
  	RooDataSet *data2;
		if (inOpt.doWeight == 1 ) {
			data2 = (RooDataSet*)fInData2->Get("dataJpsiWeight");  //Weighted
	    cout << "## WEIGHTED dataset is used\n";
	 	 } else {
	 	   data2 = (RooDataSet*)fInData2->Get("dataJpsi");  //Unweighted
 		   cout << "## UN-WEIGHTED dataset is used!\n";
 		 }
 	 data2->SetName("data2");
	 data->append(*data2);
	}

  // Create workspace to play with
  RooWorkspace *ws = new RooWorkspace("workspace");

  // Reduce "dataMC" with given ranges/cuts
  char reduceDS[3000], reduceDS2[3000], reduceDSMC[3000], reduceDS2MC[3000];
  if (!inOpt.rpmethod.compare("etHFm")) { // use this option - KYO
    sprintf(reduceDS2,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && Jpsi_Y>=%.2f && Jpsi_Y<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Centrality>=%.1f && Centrality<%.1f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f && Ntracks >=%.1f && Ntracks < %.1f && SumET_HFEta4 >=%.2f && SumET_HFEta4 < %.2f",
        pmin,pmax,ymin,ymax,-lmin,lmax,cmin,cmax,psmin,psmax,ntrmin,ntrmax,etmin,etmax);
    sprintf(reduceDS2MC,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && Jpsi_Y>=%.2f && Jpsi_Y<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f",
        pmin,pmax,ymin,ymax,-lmin,lmax,psmin,psmax);

		if (inOpt.ctErrRange == 0) {
			if (readCtauErrRange(inOpt, &errmin, &errmax)<0) return -1;
		} else if (inOpt.ctErrRange == 2) {
			errmin = 0.008; errmax = 1.0;
		} else {
			getCtauErrRange(data, inOpt, reduceDS2, lmin, lmax, &errmin, &errmax);
		}
 
    sprintf(reduceDS,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && Jpsi_Y>=%.2f && Jpsi_Y<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Jpsi_CtErr>=%.3f && Jpsi_CtErr<%.3f && Centrality>=%.1f && Centrality<%.1f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f && Ntracks >=%.1f && Ntracks < %.1f && SumET_HFEta4 >=%.2f && SumET_HFEta4 < %.2f",
        pmin,pmax,ymin,ymax,-lmin,lmax,errmin,errmax,cmin,cmax,psmin,psmax,ntrmin,ntrmax,etmin,etmax);
    sprintf(reduceDSMC,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && Jpsi_Y>=%.2f && Jpsi_Y<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Jpsi_CtErr>=%.3f && Jpsi_CtErr<%.3f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f",
        pmin,pmax,ymin,ymax,-lmin,lmax,errmin,errmax,psmin,psmax);
  } else if (!inOpt.rpmethod.compare("etHFp")) { //negative rap
    sprintf(reduceDS2,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && Jpsi_Y>=%.2f && Jpsi_Y<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Centrality>=%.1f && Centrality<%.1f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f && Ntracks >=%.1f && Ntracks < %.1f && SumET_HFEta4 >=%.2f && SumET_HFEta4 < %.2f",
        pmin,pmax,-ymax,-ymin,-lmin,lmax,cmin,cmax,psmin,psmax,ntrmin,ntrmax,etmin,etmax);
    sprintf(reduceDS2MC,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && Jpsi_Y>=%.2f && Jpsi_Y<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f",
        pmin,pmax,-ymax,-ymin,-lmin,lmax,psmin,psmax);
 
		if (inOpt.ctErrRange == 0) {
			if (readCtauErrRange(inOpt, &errmin, &errmax)<0) return -1;
		} else if (inOpt.ctErrRange == 2) {
			errmin = 0.008; errmax = 1.0;
		} else {
			getCtauErrRange(data, inOpt, reduceDS2, lmin, lmax, &errmin, &errmax);
		}
 
    sprintf(reduceDS,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && Jpsi_Y>=%.2f && Jpsi_Y<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Jpsi_CtErr>=%.3f && Jpsi_CtErr<%.3f && Centrality>=%.1f && Centrality<%.1f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f && Ntracks >=%.1f && Ntracks < %.1f && SumET_HFEta4 >=%.2f && SumET_HFEta4 < %.2f",
        pmin,pmax,-ymax,-ymin,-lmin,lmax,errmin,errmax,cmin,cmax,psmin,psmax,ntrmin,ntrmax,etmin,etmax);
    sprintf(reduceDSMC,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && Jpsi_Y>=%.2f && Jpsi_Y<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Jpsi_CtErr>=%.3f && Jpsi_CtErr<%.3f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f",
        pmin,pmax,-ymax,-ymin,-lmin,lmax,errmin,errmax,psmin,psmax);
  } else { // absolute rap
    sprintf(reduceDS2,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && TMath::Abs(Jpsi_Y)>=%.2f && TMath::Abs(Jpsi_Y)<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Centrality>=%.1f && Centrality<%.1f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f && Ntracks >=%.1f && Ntracks < %.1f && SumET_HFEta4 >=%.2f && SumET_HFEta4 < %.2f",
        pmin,pmax,ymin,ymax,-lmin,lmax,cmin,cmax,psmin,psmax,ntrmin,ntrmax,etmin,etmax);
    sprintf(reduceDS2MC,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && TMath::Abs(Jpsi_Y)>=%.2f && TMath::Abs(Jpsi_Y)<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f",
        pmin,pmax,ymin,ymax,-lmin,lmax,psmin,psmax);

		if (inOpt.ctErrRange == 0) {
			if (readCtauErrRange(inOpt, &errmin, &errmax)<0) return -1;
		} else if (inOpt.ctErrRange == 2) {
			errmin = 0.008; errmax = 1.0;
		} else {
			getCtauErrRange(data, inOpt, reduceDS2, lmin, lmax, &errmin, &errmax);
		}

    sprintf(reduceDS,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && TMath::Abs(Jpsi_Y)>=%.2f && TMath::Abs(Jpsi_Y)<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Jpsi_CtErr>=%.3f && Jpsi_CtErr<%.3f && Centrality>=%.1f && Centrality<%.1f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f && Ntracks >=%.1f && Ntracks < %.1f && SumET_HFEta4 >=%.2f && SumET_HFEta4 < %.2f",
        pmin,pmax,ymin,ymax,-lmin,lmax,errmin,errmax,cmin,cmax,psmin,psmax,ntrmin,ntrmax,etmin,etmax);
    sprintf(reduceDSMC,
        "Jpsi_Pt>=%.2f && Jpsi_Pt<%.2f && TMath::Abs(Jpsi_Y)>=%.2f && TMath::Abs(Jpsi_Y)<%.2f && Jpsi_Ct>=%.2f && Jpsi_Ct<%.2f && Jpsi_CtErr>=%.3f && Jpsi_CtErr<%.3f && Jpsi_dPhi>=%.3f && Jpsi_dPhi<%.3f",
        pmin,pmax,ymin,ymax,-lmin,lmax,errmin,errmax,psmin,psmax,ntrmin,ntrmax,etmin,etmax);
  }
  
  RooDataSet *redMC, *redMC2, *redData; // with CtErr cut 
  RooDataSet *redMC_2, *redMC2_2, *redData_2; //without CtErr cut

  redMC = (RooDataSet*)dataMC->reduce(reduceDSMC);
  redMC2 = (RooDataSet*)dataMC2->reduce(reduceDSMC);
  redData = (RooDataSet*)data->reduce(reduceDS);
 
  redMC_2 = (RooDataSet*)dataMC->reduce(reduceDS2MC);
  redMC2_2 = (RooDataSet*)dataMC2->reduce(reduceDS2MC);
  redData_2 = (RooDataSet*)data->reduce(reduceDS2);

  if (inOpt.isPEE == 1) {
    cout << "reduceDS: " << reduceDS << endl;
    cout << "reduceDSMC: " << reduceDSMC << endl;
    ws->import(*redMC);
    ws->import(*redMC2);
    ws->import(*redData);
  } else {
    cout << "reduceDS2: " << reduceDS2 << endl;
    cout << "reduceDS2MC: " << reduceDS2MC << endl;
    ws->import(*redMC_2);
    ws->import(*redMC2_2);
    ws->import(*redData_2);
  }

  setWSRange(ws, lmin, lmax, errmin, errmax);

  // Draw data
  ws->var("Jpsi_Mass")->SetTitle("m_{#mu#mu}");
  ws->var("Jpsi_Ct")->SetTitle("#font[12]{l}_{J/#psi}");

  // Test true lifetimes
  RooPlot *trueFrame = ws->var("Jpsi_CtTrue")->frame();
  ws->data("dataMC")->plotOn(trueFrame,DataError(RooAbsData::SumW2),Cut("MCType==MCType::NP"));

  string titlestr;
  TCanvas c0;

  // Define binning for mass
  ws->var("Jpsi_Mass")->setBins(45);
  if (!inOpt.prange.compare("0.0-3.0")){ ws->var("Jpsi_Mass")->setBins(100);} // KYO

  // Define binning for ctau error
  ws->var("Jpsi_CtErr")->setBins(25);
  if (pmin > 40.) ws->var("Jpsi_CtErr")->setBins(8);
//  if (!inOpt.prange.compare("0.0-3.0")){ ws->var("Jpsi_CtErr")->setBins(100);} // KYO

  // define binning for true lifetime
  RooBinning rb(-0.1,4.0);
  rb.addUniform(5,-0.1,0.0);
  rb.addUniform(100,0.0,0.5);
  rb.addUniform(15,0.5,1.0);
  rb.addUniform(20,1.0,2.5);
  rb.addUniform(5,2.5,4.0);
  if (inOpt.analyticBlifetime) {
    ws->var("Jpsi_CtTrue")->setBins(1000);
  } else {
    ws->var("Jpsi_CtTrue")->setBinning(rb);
  }

  // Define binning for lifetime //data
  RooBinning rb2 = setCtBinning(lmin,lmax);
  RooBinning rbCorser(-lmin,lmax);
  rbCorser.addUniform(60,-lmin,lmax);
//  ws->var("Jpsi_Ct")->setBins(90);  //original
  ws->var("Jpsi_Ct")->setBinning(rb2);  //original
//  ws->var("Jpsi_Ct")->setBinning(rbCorser);  //corser
	if (!inOpt.prange.compare("0.0-3.0")){ws->var("Jpsi_Ct")->setBins(100);}  // KYO
  
  // Define ctau binning for plotting (coarser bin)
/*
  RooBinning rb3(-lmin,lmax);
  rb3.addBoundary(-1.0);
  rb3.addBoundary(-0.7);
  rb3.addBoundary(-0.6);
  rb3.addBoundary(-0.5);
  rb3.addUniform(5,-0.5,-0.2);
  rb3.addUniform(15,-0.2,0.2);
  rb3.addUniform(5,0.2,0.5);
//  rb3.addUniform(5,0.5,1.0);
  rb3.addUniform(10,0.5,1.2); //new added
  rb3.addUniform(6,1.2,lmax); //new added
*/
  RooBinning rb3(-lmin,lmax);
  rb3.addUniform(20,-lmin,-0.5);
  rb3.addUniform(5,-0.5,-0.2);
  rb3.addUniform(15,-0.2,0.2);
  rb3.addUniform(5,0.2,0.5);
//  rb3.addUniform(5,0.5,1.0);
  rb3.addUniform(10,0.5,1.1); //new added
  rb3.addUniform(20,1.1,lmax); //new added

  // Additional cuts on data and get sub-datasets/histograms
  RooDataSet *redDataCut;
  string reduceDSstr;
  if (inOpt.isGG == 0) {
    reduceDSstr = "(MCType != MCType::NP || Jpsi_CtTrue>0.0001) &&\
                  (MCType == MCType::PR || MCType == MCType::NP)";
    redDataCut = (RooDataSet*)redData->Clone();
  } else if (inOpt.isGG == 1) {
    reduceDSstr = "(MCType != MCType::NP || Jpsi_CtTrue > 0.0001) &&\
                  (MCType == MCType::PR || MCType == MCType::NP)";
    redDataCut = (RooDataSet*)redData->Clone();
  } else {
    reduceDSstr = "(MCType != MCType::NP || Jpsi_CtTrue>0.0001) &&\
                   (MCType == MCType::PR || MCType == MCType::NP)";
    if (inOpt.isPbPb == 1) {
      redDataCut = (RooDataSet*)redData->reduce("Jpsi_Ct < 600000.");
    } else {
      redDataCut = (RooDataSet*)redData_2->reduce("Jpsi_Ct < 600000.");
    }
  }

  // Test ctau on data and mc with/without ctau error cut
//  ctauErrCutCheck(ws,redData,redData_2,redMC,redMC_2,redMC2,redMC2_2,inOpt);
  RooDataHist *binData = new RooDataHist("binData","binData",RooArgSet( *(ws->var("Jpsi_Mass")),*(ws->var("Jpsi_Ct")),*(ws->var("Jpsi_CtErr")) ), *redDataCut);
  RooDataHist *binDataCtErr = new RooDataHist("binDataCtErr","binDataCtErr",RooArgSet(*(ws->var("Jpsi_CtErr"))),*redDataCut);
  cout << "DATA :: N events to fit: " << binData->sumEntries() << endl;

  // *** Get MC sub-datasets and its histograms corresponds to data
  RooDataSet *redMCCut, *redMCCutNP, *redMCCut2, *redMCCutPR ;
  if (inOpt.isPEE == 1) {
    redMCCut = (RooDataSet*) redMC->reduce(reduceDSstr.c_str());
    redMCCut2 = (RooDataSet*) redMC2->reduce(reduceDSstr.c_str());
  } else {
    redMCCut = (RooDataSet*) redMC_2->reduce(reduceDSstr.c_str());
    redMCCut2 = (RooDataSet*) redMC2_2->reduce(reduceDSstr.c_str());
  }
  redMCCutNP = (RooDataSet*) redMCCut->reduce(RooArgSet(*(ws->var("Jpsi_CtTrue"))));
//  redMCCutNP = (RooDataSet*) redMCCut->reduce(RooArgSet(*(ws->var("Jpsi_CtTrue"))),"MCType == MCType::NP");
  redMCCutPR = (RooDataSet*) redMCCut2->reduce("MCType == MCType::PR");

  // SYSTEMATICS 1 (very sidebands)
  RooDataSet *redDataSB;
  if (inOpt.narrowSideband) redDataSB = (RooDataSet*) redDataCut->reduce("Jpsi_Mass<2.8 || Jpsi_Mass>3.4");
  else redDataSB = (RooDataSet*) redDataCut->reduce("Jpsi_Mass<2.9 || Jpsi_Mass>3.3");

  RooDataSet *redDataSBL, *redDataSBR;
  redDataSBL = (RooDataSet*) redDataCut->reduce("Jpsi_Mass<2.9");
  redDataSBR = (RooDataSet*) redDataCut->reduce("Jpsi_Mass>3.3");

  sidebandLeftRightCheck(ws, redDataSBL, redDataSBR, inOpt);

  RooDataHist *binDataSB = new RooDataHist("binDataSB","Data distribution for background",RooArgSet( *(ws->var("Jpsi_Mass")),*(ws->var("Jpsi_Ct")) , *(ws->var("Jpsi_CtErr")) ),*redDataSB);
  RooDataSet *redDataSIG = (RooDataSet*)redDataCut->reduce("Jpsi_Mass > 2.9 && Jpsi_Mass < 3.3");
  RooDataSet *redDataSIGWide = (RooDataSet*)redDataCut->reduce("Jpsi_Mass > 2.9 && Jpsi_Mass < 3.2");

  RooDataHist *binMCPR = new RooDataHist("binMCPR","MC distribution for PR signal",RooArgSet( *(ws->var("Jpsi_Mass")),*(ws->var("Jpsi_Ct")), *(ws->var("Jpsi_CtErr"))),*redMCCutPR);
  cout << "PRMC :: N events to fit: " << binMCPR->sumEntries() << endl;
  RooDataHist *binMCNP = new RooDataHist("binMCNP","MC distribution for NP signal",RooArgSet(*(ws->var("Jpsi_CtTrue"))),*redMCCutNP);
  cout << "NPMC :: N events to fit: " << binMCNP->sumEntries() << endl;

  RooDataHist *binDataCtErrSB = new RooDataHist("binDataCtErrSB","Data ct error distribution for bkg",RooArgSet(*(ws->var("Jpsi_CtErr"))),*redDataSB);
  RooDataHist *binDataCtErrSIG = new RooDataHist("binDataCtErrSIG","Data ct error distribution for sig",RooArgSet(*(ws->var("Jpsi_CtErr"))),*redDataSIG);

  // *** Define PDFs with parameters (mass and ctau)
  // J/psi mass parameterization
  defineMassBkg(ws);
  defineMassSig(ws, inOpt);
  // J/psi CTau parameterization
  defineCTResol(ws, inOpt);              // R(l) : resolution function
  defineCTBkg(ws, inOpt);                // theta(l') convolution R(l')
  titlestr = inOpt.dirPre + "_rap" + inOpt.yrange + "_pT" + inOpt.prange + "_cent" + inOpt.crange + "_dPhi" + inOpt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_testTrueLifeFit";
  defineCTSig(ws,redMCCutNP,titlestr,lmax,inOpt); // F_B(l) : X_mc(l')

  char funct[100];

  // Set some fitting variables to constant. It depends on the prefitting options.
  RooFitResult *fitM;
  RooFitResult *fit2D;

  if (inOpt.prefitMass) {
    struct PARAM {
      double coeffGaus; double coeffGausErr;
      double meanSig1;  double meanSig1Err;
      double sigmaSig1; double sigmaSig1Err;
      double sigmaSig2; double sigmaSig2Err;
      double sigmaSig3; double sigmaSig3Err;
      double alpha;     double alphaErr;
      double enne;      double enneErr;
      double enneW;     double enneWErr;
    };

    bool centConst = false;  //False: fit w/o any constrained parameters (centrality dep.)
    bool dPhiConst = false;  //False: fit w/o any constrained parameters (dPhi dep.)
    double inputN[2] = {0};  //Number of yield/background in the 0-1.571 rad bin
    if (inOpt.isMB != 0) {
      string inputFN, inputFNcb;
      bool centest = true;  //False: fit w/o any fixed parameters (centrality dep.)
      bool dPhitest = true;  //False: fit w/o any fixed parameters (dPhi dep.)
      if (inOpt.isMB == 1) {
        // Except the *_dPhi0.00-1.57 bins, all other delta Phi bins read/set signal parameters of this bin
        centest = false;
        centConst = false;
        dPhiConst = false;
        if (!inOpt.phirange.compare("0.000-1.571")) {
          dPhitest = false;
        } else {
          dPhitest = true;
          inputFN =  inOpt.dirPre + "_rap" + inOpt.yrange + "_pT" + inOpt.prange + "_cent" + inOpt.crange + "_dPhi0.000-1.571.txt";
        }

      } else if (inOpt.isMB == 2) {
        // Except the *_cent0-100_dPhi0.000-1.571 bin, all other bins read/set signal parameters of this bin
        centest = false;
        centConst = false;
        dPhiConst = false;
        if (!inOpt.crange.compare("0.0-100.0") && !inOpt.phirange.compare("0.000-1.571")) {
          dPhitest = false;
        } else {
          dPhitest = true;
          inputFN =  inOpt.dirPre + "_rap" + inOpt.yrange + "_pT" + inOpt.prange + "_cent0.0-100.0_dPhi0.000-1.571.txt";
        }

      } else if (inOpt.isMB == 3) {
        // Constrain signal parameters that were fixed in the default method.
        // Constrained mean values are coming from *_cent0-100_dPhi0.000-1.571 bin result.
        centest = false;
        dPhitest = false;
        if (!inOpt.crange.compare("0.0-100.0")) {
          centConst = false;
        } else {
          centConst = true;
          inputFNcb =  inOpt.dirPre + "_rap" + inOpt.yrange + "_pT" + inOpt.prange + "_cent0.0-100.0_dPhi0.000-1.571.txt";
        }
        if (!inOpt.phirange.compare("0.000-1.571")) {
          dPhiConst = false;
        } else {
          dPhiConst = true;
          inputFN =  inOpt.dirPre + "_rap" + inOpt.yrange + "_pT" + inOpt.prange + "_cent" + inOpt.crange + "_dPhi0.000-1.571.txt";
        }

      } else if (inOpt.isMB == 4) {
        centConst = false;
        dPhiConst = false;
        if (!inOpt.crange.compare("0.0-100.0")) {
          centest = false;
        } else {
          centest = true;
          inputFNcb =  inOpt.dirPre + "_rap" + inOpt.yrange + "_pT" + inOpt.prange + "_cent0.0-100.0_dPhi0.000-1.571.txt";
        }
        if (!inOpt.phirange.compare("0.000-1.571")) {
          dPhitest = false;
        } else {
          dPhitest = true;
          inputFN =  inOpt.dirPre + "_rap" + inOpt.yrange + "_pT" + inOpt.prange + "_cent" + inOpt.crange + "_dPhi0.000-1.571.txt";
        }
      }

      if (centConst || dPhiConst) {
        ifstream input;
        if (centConst && !dPhiConst) input.open(inputFNcb.c_str());
        else if (dPhiConst) input.open(inputFN.c_str());
        if (!input.good()) { cout << "Failed to open: " <<inputFNcb << endl; return 1; }
        string tmp;
        double inputTmp[2] = {0};
        PARAM inputP;
        while (input.good()) {   //Mass signal parameters
          input >> tmp >> inputTmp[0] >> inputTmp[1];
          cout << tmp << " " << inputTmp[0] << " " << inputTmp[1]<< endl;
          if (!tmp.compare("coeffGaus")) { inputP.coeffGaus = inputTmp[0]; inputP.coeffGausErr = inputTmp[1];
          } else if (!tmp.compare("meanSig1")) { inputP.meanSig1 = inputTmp[0]; inputP.meanSig1Err = inputTmp[1];
          } else if (!tmp.compare("sigmaSig1")) { inputP.sigmaSig1 = inputTmp[0]; inputP.sigmaSig1Err = inputTmp[1];
            if (!inOpt.yrange.compare("0.0-1.2") && inOpt.prange.compare("6.5-8.0")) inputP.sigmaSig1 = inputTmp[0]; inputP.sigmaSig1Err = inputTmp[1]*1.1;
          } else if (!tmp.compare("sigmaSig2")) { inputP.sigmaSig2 = inputTmp[0]; inputP.sigmaSig2Err = inputTmp[1];
          } else if (!tmp.compare("sigmaSig3")) { inputP.sigmaSig3 = inputTmp[0]; inputP.sigmaSig3Err = inputTmp[1];
          } else if (!tmp.compare("alpha")) { inputP.alpha = inputTmp[0]; inputP.alphaErr = inputTmp[1];
          } else if (!tmp.compare("enneW")) { inputP.enneW = inputTmp[0]; inputP.enneWErr = inputTmp[1];
          }
        }

        char confunct[1000]={0};
        if (inOpt.is2Widths == 1) {
          if (dPhiConst) {  //Constrain CB width only for dPhi bins
            sprintf(confunct,"Gaussian::sigmaSig2Con(sigmaSig2,RooConstVar(%f),RooConstVar(%f))",inputP.sigmaSig2,inputP.sigmaSig2Err); ws->factory(confunct);
            sprintf(confunct,"Gaussian::sigmaSig3Con(sigmaSig3,RooConstVar(%f),RooConstVar(%f))",inputP.sigmaSig3,inputP.sigmaSig3Err); ws->factory(confunct);
          }
          sprintf(confunct,"Gaussian::sigmaSig1Con(sigmaSig1,RooConstVar(%f),RooConstVar(%f))",inputP.sigmaSig1,inputP.sigmaSig1Err); ws->factory(confunct);
          sprintf(confunct,"Gaussian::meanSig1Con(meanSig1,RooConstVar(%f),RooConstVar(%f))",inputP.meanSig1,inputP.meanSig1Err); ws->factory(confunct);
          sprintf(confunct,"Gaussian::coeffGausCon(coeffGaus,RooConstVar(%f),RooConstVar(%f))",inputP.coeffGaus,inputP.coeffGausErr); ws->factory(confunct);
          sprintf(confunct,"Gaussian::alphaCon(alpha,RooConstVar(%f),RooConstVar(%f))",inputP.alpha,inputP.alphaErr); ws->factory(confunct);
          sprintf(confunct,"Gaussian::enneWCon(enneW,RooConstVar(%f),RooConstVar(%f))",inputP.enneW,inputP.enneWErr); ws->factory(confunct);
        } else if (inOpt.is2Widths == 0) {
          sprintf(confunct,"Gaussian::sigmaSig1Con(sigmaSig1,RooConstVar(%f),RooConstVar(%f))",inputP.sigmaSig1,inputP.sigmaSig1Err); ws->factory(confunct);
          sprintf(confunct,"Gaussian::meanSig1Con(meanSig1,RooConstVar(%f),RooConstVar(%f))",inputP.meanSig1,inputP.meanSig1Err); ws->factory(confunct);
          sprintf(confunct,"Gaussian::coeffGausCon(coeffGaus,RooConstVar(%f),RooConstVar(%f))",inputP.coeffGaus,inputP.coeffGausErr); ws->factory(confunct);
          sprintf(confunct,"Gaussian::alphaCon(alpha,RooConstVar(%f),RooConstVar(%f))",inputP.alpha,inputP.alphaErr); ws->factory(confunct);
          sprintf(confunct,"Gaussian::enneWCon(enneW,RooConstVar(%f),RooConstVar(%f))",inputP.enneW,inputP.enneWErr); ws->factory(confunct);
        }

        input.close();
      } // end of (centConst || dPhiConst) condition

      if (centest) {
        ifstream input(inputFNcb.c_str());
        if (!input.good()) { cout << "Failed to open: " <<inputFNcb << endl; return 1; }
        string tmp;
        double inputNS[2] = {0};
        double inputTmp[2] = {0};
        PARAM inputP;
        while (input.good()) {   //Mass signal parameters
          input >> tmp >> inputTmp[0] >> inputTmp[1];
          cout << tmp << " " << inputTmp[0] << endl;
          if (!tmp.compare("coeffGaus")) inputP.coeffGaus = inputTmp[0];
          else if (!tmp.compare("meanSig1")) inputP.meanSig1 = inputTmp[0];
          else if (!tmp.compare("sigmaSig1")) inputP.sigmaSig1 = inputTmp[0];
          else if (!tmp.compare("sigmaSig2")) inputP.sigmaSig2 = inputTmp[0];
          else if (!tmp.compare("sigmaSig3")) inputP.sigmaSig3 = inputTmp[0];
          else if (!tmp.compare("alpha")) inputP.alpha = inputTmp[0];
          else if (!tmp.compare("enneW")) inputP.enneW = inputTmp[0];
        }

        ws->var("coeffGaus")->setVal(inputP.coeffGaus);
        ws->var("meanSig1")->setVal(inputP.meanSig1);
        ws->var("sigmaSig1")->setVal(inputP.sigmaSig1);
        ws->var("alpha")->setVal(inputP.alpha);
        ws->var("enneW")->setVal(inputP.enneW);

        ws->var("coeffGaus")->setConstant(kTRUE);
        ws->var("meanSig1")->setConstant(kTRUE);
        ws->var("sigmaSig1")->setConstant(kTRUE);
        ws->var("alpha")->setConstant(kTRUE);
        ws->var("enneW")->setConstant(kTRUE);

        input.close();
      }

      if (dPhitest) {
        ifstream input(inputFN.c_str());
        cout << inputFN << endl;
        if (!input.good()) { cout <<"Failed to open: " <<  inputFN << endl; return 1; }
        string tmp;
        PARAM inputP;
        double inputNS[2] = {0};
        double inputTmp[2]={0};
        while (input.good()) {   //Mass signal parameters
          input >> tmp >> inputTmp[0] >> inputTmp[1];
          cout << tmp << " " << inputTmp[0] << endl;
          if (!tmp.compare("coeffGaus")) inputP.coeffGaus = inputTmp[0];
          else if (!tmp.compare("meanSig1")) inputP.meanSig1 = inputTmp[0];
          else if (!tmp.compare("sigmaSig1")) inputP.sigmaSig1 = inputTmp[0];
          else if (!tmp.compare("sigmaSig2")) inputP.sigmaSig2 = inputTmp[0];
          else if (!tmp.compare("sigmaSig3")) inputP.sigmaSig3 = inputTmp[0];
          else if (!tmp.compare("alpha")) inputP.alpha = inputTmp[0];
          else if (!tmp.compare("enneW")) inputP.enneW = inputTmp[0];
        }

        ws->var("sigmaSig2")->setVal(inputP.sigmaSig2);
        ws->var("sigmaSig3")->setVal(inputP.sigmaSig3);
        ws->var("coeffGaus")->setVal(inputP.coeffGaus);
        ws->var("meanSig1")->setVal(inputP.meanSig1);
        ws->var("sigmaSig1")->setVal(inputP.sigmaSig1);
        ws->var("alpha")->setVal(inputP.alpha);
        ws->var("enneW")->setVal(inputP.enneW);

        ws->var("sigmaSig2")->setConstant(kTRUE);
        ws->var("sigmaSig3")->setConstant(kTRUE);
        ws->var("coeffGaus")->setConstant(kTRUE);
        ws->var("meanSig1")->setConstant(kTRUE);
        ws->var("sigmaSig1")->setConstant(kTRUE);
        ws->var("alpha")->setConstant(kTRUE);
        ws->var("enneW")->setConstant(kTRUE);

        input.close();
      }

    } //End of isMB != 0

//    if (inOpt.isPbPb == 1 || inOpt.isPbPb == 2) {
      double initBkg = redDataSB->sumEntries()*9.0/5.0;
      double initSig = redDataCut->sumEntries() - initBkg;
			initBkg = initBkg+10000; // KYO
      sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
/*    } else {
      // 2010 setting in pp
      sprintf(funct,"SUM::sigMassPDF(NSig[500.0,1.0,1000000.0]*%s,NBkg[2000.,1.,1000000.0]*%s)",inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
    }
*/

			// Special bin variable settings
		// KYO test
    ws->var("alpha")->setVal(1.8);
    ws->var("enneW")->setVal(2.5);
    ws->var("alpha")->setConstant(kTRUE);
    ws->var("enneW")->setConstant(kTRUE);
		sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());

			if (inOpt.isMultiplicity == 0) { // no multiplicity
				if (inOpt.isPbPb == 2) {
					if (!inOpt.yrange.compare("-2.4--1.97")) {
						if (!inOpt.prange.compare("0.0-1.5")){
							ws->var("coefExp")->setRange(-3.,3.5);
							ws->var("coefExp")->setVal(1.9);
							ws->var("lambdap")->setRange(0.05,1.5);
							ws->var("lambdam")->setRange(0.02,1.5);
							ws->var("lambdam")->setVal(0.79);
							ws->var("lambdasym")->setRange(0.02,5.0);
							ws->var("lambdasym")->setVal(0.69);
							//ws->var("fbkgCtTot")->setRange(0.1,0.9);
							ws->var("fbkgCtTot")->setRange(0.4,0.9);
							ws->var("fbkgCtTot")->setVal(0.39);
    	 	     	sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("5.0-6.5")){
							ws->var("sigmaSig1")->setRange(0.01,0.04);
							ws->var("sigmaSig1")->setVal(0.02);
							ws->var("sigmaSig2")->setRange(0.025,0.05);
							ws->var("sigmaSig2")->setVal(0.04);
							ws->var("lambdap")->setRange(0.05,1.5);
							ws->var("lambdam")->setRange(0.02,1.5);
							ws->var("lambdam")->setVal(0.79);
							ws->var("lambdasym")->setRange(0.02,5.0);
							ws->var("lambdasym")->setVal(0.69);
    	 	     	sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("0.0-3.0")){
							//ws->var("fpm")->setRange(0.85,0.95);
							//ws->var("fpm")->setVal(0.9);
							ws->var("lambdap")->setRange(0.01,0.4);
							ws->var("lambdap")->setVal(0.2);
							ws->var("lambdam")->setRange(0.01,0.2);
							ws->var("lambdam")->setVal(0.2);
							ws->var("lambdasym")->setRange(0.001,0.07);
							ws->var("lambdasym")->setVal(0.02);
   	 	     	sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("3.0-6.5")){
							ws->var("coeffGaus")->setRange(0.05,0.93);
							//ws->var("sigmaSig1")->setRange(0.01,0.02);
							ws->var("sigmaSig1")->setRange(0.01,0.05);
//							ws->var("sigmaSig1")->setVal(0.025);
//							ws->var("enneW")->setRange(1.,50.0);
//							ws->var("enneW")->setVal(5.);
//							//ws->var("Gmc")->setRange(0.00001,0.001);
//							//ws->var("Gmc")->setVal(0.0000567607);
//							ws->var("fracRes")->setRange(0.001,0.4);
//							ws->var("meanResSigW")->setRange(-0.02,0.03);
//							ws->var("sigmaResSigW")->setRange(1.3,3.5);
							ws->var("lambdap")->setRange(0.05,1.5);
							ws->var("lambdam")->setRange(0.02,1.5);
							ws->var("lambdam")->setVal(0.79);
							ws->var("lambdasym")->setRange(0.02,5.0);
							ws->var("lambdasym")->setVal(0.69);
    	 	     	sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
//						else if (!inOpt.prange.compare("6.5-7.5")){
//							//ws->var("coefExp")->setRange(-3.,-0.99);
//							ws->var("coefExp")->setRange(-3.,-1.07);
//							ws->var("coefExp")->setVal(-2.5);
//							ws->var("coeffGaus")->setRange(0.3,0.93);
//							ws->var("coeffGaus")->setVal(0.7);
//							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
//						}
//						else if (!inOpt.prange.compare("7.5-8.5")){
//							ws->var("coefExp")->setRange(-3.,-1.44);
//							ws->var("coefExp")->setVal(-2.5);
//							ws->var("coeffGaus")->setRange(0.4,0.93);
//							ws->var("coeffGaus")->setVal(0.7);
//							ws->var("enneW")->setRange(2.5,50.0);
//							ws->var("enneW")->setVal(5.);
//							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
//						}
//						else if (!inOpt.prange.compare("8.5-9.5")){
//							ws->var("coeffGaus")->setRange(0.38,0.91);
//							ws->var("coeffGaus")->setVal(0.7);
//							ws->var("sigmaSig1")->setRange(0.01,0.04);
//							ws->var("sigmaSig1")->setVal(0.02);
//							ws->var("sigmaSig1")->setRange(0.025,0.05);
//							ws->var("sigmaSig1")->setVal(0.04);
//							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
//						}
//						else if (!inOpt.prange.compare("9.5-11.0")){
//							ws->var("coeffGaus")->setRange(0.68,0.93);
//							ws->var("coeffGaus")->setVal(0.8);
//							ws->var("sigmaSig1")->setRange(0.01,0.06);
//							ws->var("sigmaSig1")->setVal(0.02);
//							ws->var("enneW")->setRange(2.5,50.0);
//							ws->var("enneW")->setVal(5.);
//							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
//						}
						else if (!inOpt.prange.compare("11.0-14.0")){
							//ws->var("fpm")->setRange(0.85,0.95);
							//ws->var("fpm")->setVal(0.9);
							ws->var("lambdap")->setRange(0.01,0.4);
							ws->var("lambdap")->setVal(0.2);
							ws->var("lambdam")->setRange(0.01,0.2);
							ws->var("lambdam")->setVal(0.2);
							ws->var("lambdasym")->setRange(0.001,0.07);
							ws->var("lambdasym")->setVal(0.02);
  		    		sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
					}
					else if (!inOpt.yrange.compare("-1.97--1.47")){
						if (!inOpt.prange.compare("6.5-7.5")){
							ws->var("coeffGaus")->setRange(0.05,0.93);
							ws->var("sigmaSig1")->setRange(0.01,0.03);
							ws->var("sigmaSig1")->setVal(0.02);
							ws->var("sigmaSig2")->setRange(0.01,0.05);
							ws->var("sigmaSig2")->setVal(0.02);
							//ws->var("Gmc")->setRange(0.00001,0.001);
							//ws->var("Gmc")->setVal(0.0000101693);
							ws->var("fracRes")->setRange(0.005,0.4);
							ws->var("meanResSigW")->setRange(-0.02,0.03);
							ws->var("sigmaResSigW")->setRange(1.3,3.5);
							ws->var("lambdap")->setRange(0.05,1.5);
							ws->var("lambdam")->setRange(0.02,1.5);
							ws->var("lambdam")->setVal(0.79);
							ws->var("lambdasym")->setRange(0.02,5.0);
							ws->var("lambdasym")->setVal(0.69);
    	 	     	sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("7.5-8.5")){
							ws->var("coeffGaus")->setRange(0.4,0.75);
							ws->var("coeffGaus")->setVal(0.65);
							ws->var("sigmaSig1")->setRange(0.01,0.06);
							ws->var("sigmaSig1")->setVal(0.05);
							ws->var("sigmaSig2")->setRange(0.010,0.05);
							ws->var("sigmaSig2")->setVal(0.03);
          		ws->var("enneW")->setRange(5,50);
          		ws->var("enneW")->setVal(10);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("8.5-9.5")){
							ws->var("coeffGaus")->setRange(0.3,0.93);
							ws->var("coeffGaus")->setVal(0.7);
							ws->var("sigmaSig1")->setRange(0.01,0.03);
							ws->var("sigmaSig1")->setVal(0.02);
							ws->var("sigmaSig2")->setRange(0.01,0.05);
							ws->var("sigmaSig2")->setVal(0.02);
							//ws->var("Gmc")->setRange(0.00001,0.001);
							//ws->var("Gmc")->setVal(0.0000106498);
							ws->var("fracRes")->setRange(0.005,0.4);
							ws->var("meanResSigW")->setRange(-0.02,0.03);
							ws->var("sigmaResSigW")->setRange(1.3,3.5);
							ws->var("lambdap")->setRange(0.05,1.5);
							ws->var("lambdam")->setRange(0.02,1.5);
							ws->var("lambdam")->setVal(0.79);
							ws->var("lambdasym")->setRange(0.02,5.0);
							ws->var("lambdasym")->setVal(0.69);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("9.5-11.0")){
							ws->var("coeffGaus")->setRange(0.75,0.93);
							ws->var("coeffGaus")->setVal(0.88);
							ws->var("sigmaSig1")->setRange(0.04,0.07);
							ws->var("sigmaSig1")->setVal(0.06);
							ws->var("sigmaSig2")->setRange(0.01,0.04);
							ws->var("sigmaSig2")->setVal(0.02);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}


					}
					else if (!inOpt.yrange.compare("-1.47--0.47")){
						if (!inOpt.prange.compare("11.0-14.0")){
							ws->var("fracRes")->setRange(0.005,0.2);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
					}
					else if (!inOpt.yrange.compare("-0.47-0.53")){
						if (!inOpt.prange.compare("6.5-7.5")){
							ws->var("coeffGaus")->setRange(0.45,0.93);
							ws->var("coeffGaus")->setVal(0.8);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("8.5-9.5")){
							ws->var("coeffGaus")->setRange(0.45,0.89);
							ws->var("coeffGaus")->setVal(0.7);
							ws->var("fracRes")->setRange(0.005,0.2);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
					}
					else if (!inOpt.yrange.compare("0.53-1.03")){
						if (!inOpt.prange.compare("7.5-8.5")){
							//ws->var("coefExp")->setRange(-3.,-0.49);
							ws->var("coefExp")->setRange(-3.,-0.4);
							ws->var("coefExp")->setVal(-2.5);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("8.5-9.5")){
							ws->var("coeffGaus")->setRange(0.4,0.8);
							ws->var("coeffGaus")->setVal(0.7);
          		ws->var("enneW")->setRange(2.0,50);
          		ws->var("enneW")->setVal(10);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}

					}
					else if (!inOpt.yrange.compare("1.03-1.93")){
						if (!inOpt.prange.compare("6.5-7.5")){
							ws->var("coeffGaus")->setRange(0.05,0.4);
							ws->var("coeffGaus")->setVal(0.3);
							//ws->var("coefExp")->setRange(-3.,-0.7);
							//ws->var("coefExp")->setVal(-2.);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
								
						}
						else if (!inOpt.prange.compare("7.5-8.5")){
							//ws->var("coeffGaus")->setRange(0.49, 0.93);
							ws->var("coeffGaus")->setRange(0.58, 0.93);
							ws->var("coeffGaus")->setVal(0.7);
          		ws->var("enneW")->setRange(1.8,50);
          		ws->var("enneW")->setVal(10);
							ws->var("sigmaSig1")->setRange(0.01,0.03);
							ws->var("sigmaSig1")->setVal(0.02);
							ws->var("sigmaSig2")->setRange(0.01,0.05);
							ws->var("sigmaSig2")->setVal(0.02);
							ws->var("fracRes")->setRange(0.005,0.4);
							ws->var("meanResSigW")->setRange(-0.02,0.03);
							ws->var("sigmaResSigW")->setRange(1.3,3.5);
							ws->var("lambdap")->setRange(0.05,1.5);
							ws->var("lambdam")->setRange(0.02,1.5);
							ws->var("lambdam")->setVal(0.79);
							ws->var("lambdasym")->setRange(0.02,5.0);
							ws->var("lambdasym")->setVal(0.69);
    	 	     	sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("9.5-11.0")){
          		ws->var("alpha")->setRange(0.6, 3.);
          		ws->var("alpha")->setVal(1.5);
          		ws->var("enneW")->setRange(5,50);
          		ws->var("enneW")->setVal(10);
    	 	     	sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("11.0-14.0")){
							ws->var("sigmaSig1")->setRange(0.035,0.065);
							ws->var("sigmaSig1")->setVal(0.05);
							ws->var("sigmaSig2")->setRange(0.01,0.035);
							ws->var("sigmaSig2")->setVal(0.02);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
					}
				} // isPbPb==2
				else if (inOpt.isPbPb==3) {
					if (!inOpt.yrange.compare("1.97-2.4")) {
//						if (!inOpt.prange.compare("0.0-1.0")){
						if (!inOpt.prange.compare("0.0-1.5")){
							ws->var("coefExp")->setRange(-3.,3.5);
							ws->var("coefExp")->setVal(1.9);
							ws->var("lambdap")->setRange(0.05,1.5);
							ws->var("lambdam")->setRange(0.02,1.5);
							ws->var("lambdam")->setVal(0.79);
							ws->var("lambdasym")->setRange(0.02,5.0);
							ws->var("lambdasym")->setVal(0.69);
							ws->var("fbkgCtTot")->setRange(0.37,0.41);
							ws->var("fbkgCtTot")->setVal(0.39);
    	 	     	sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("0.0-3.0")) {
//							//ws->var("sigmaResSigW")->setRange(1.49,1.55);
//							//ws->var("sigmaResSigW")->setVal(1.52213);
//							ws->var("sigmaResSigW")->setRange(1.49,2.22);
//							ws->var("sigmaResSigW")->setVal(1.52213);
//							ws->var("sigmaResSigN")->setRange(0.82,0.84);
//							ws->var("sigmaResSigN")->setVal(0.834147);
//							ws->var("fpm")->setRange(0.4,0.6);
//							ws->var("fpm")->setVal(0.5);
//							ws->var("fLiving")->setRange(0.1,0.15);
//							ws->var("fLiving")->setVal(0.13);
//							ws->var("fbkgCtTot")->setRange(0.37,0.41);
//							ws->var("fbkgCtTot")->setVal(0.39);
//							ws->var("lambdap")->setRange(0.01,0.21);
//							ws->var("lambdap")->setVal(0.1);
//							ws->var("lambdam")->setRange(0.01,0.09);
//							ws->var("lambdam")->setVal(0.02);
//							ws->var("lambdasym")->setRange(0.001,0.16);
//							ws->var("lambdasym")->setVal(0.14);
  //  	 	     	sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("3.0-6.5")) {
							ws->var("coeffGaus")->setRange(0.20,0.93);
							ws->var("coeffGaus")->setVal(0.5);	
							//ws->var("bTau")->setRange(0.035,0.15);
							//ws->var("bTau")->setVal(0.08);	
							ws->var("lambdap")->setRange(0.05,1.5);
							ws->var("lambdam")->setRange(0.01,1.5);
							ws->var("lambdam")->setVal(0.03);
							ws->var("lambdasym")->setRange(0.02,5.0);
							ws->var("lambdasym")->setVal(0.69);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("8.5-9.5")) {
							initBkg = initBkg+1000; 
							//ws->var("coefExp")->setRange(-3.,-1.17);
							//ws->var("coefExp")->setRange(-3.,-1.23);
							ws->var("coefExp")->setRange(-3.,-1.29);
							ws->var("coefExp")->setVal(-2.5);
							ws->var("coeffGaus")->setRange(0.38,0.86);
							ws->var("coeffGaus")->setVal(0.5);	
          		ws->var("alpha")->setRange(0.6, 3.);
          		ws->var("alpha")->setVal(1.5);
          		ws->var("enneW")->setRange(1.9,50);
          		ws->var("enneW")->setVal(10);
							ws->var("sigmaSig1")->setRange(0.02,0.06);
							ws->var("sigmaSig1")->setVal(0.04);
							ws->var("sigmaSig2")->setRange(0.015,0.038);
							ws->var("sigmaSig2")->setVal(0.02);
							ws->var("lambdap")->setRange(0.05,1.5);
							ws->var("lambdam")->setRange(0.01,1.5);
							ws->var("lambdam")->setVal(0.03);
							ws->var("lambdasym")->setRange(0.02,5.0);
							ws->var("lambdasym")->setVal(0.69);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("9.5-11.0")) {
							ws->var("sigmaSig1")->setRange(0.02,0.06);
							ws->var("sigmaSig1")->setVal(0.04);
							ws->var("sigmaSig2")->setRange(0.015,0.038);
							ws->var("sigmaSig2")->setVal(0.02);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("11.0-14.0")) {
							ws->var("coeffGaus")->setRange(0.39,0.85);
							ws->var("coeffGaus")->setVal(0.6);	
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("14.0-30.0")) {
							//ws->var("sigmaSig1")->setRange(0.01,0.04);
							ws->var("sigmaSig1")->setRange(0.03,0.055);
							ws->var("sigmaSig1")->setVal(0.04);
							ws->var("sigmaSig2")->setRange(0.015,0.045);
							ws->var("sigmaSig2")->setVal(0.02);
          		ws->var("enneW")->setRange(5,50);
          		ws->var("enneW")->setVal(10);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
					}
					else if (!inOpt.yrange.compare("1.47-1.97")) {
						if (!inOpt.prange.compare("8.5-9.5")) {
							ws->var("coefExp")->setRange(-3.0,-1.32);
							ws->var("coefExp")->setVal(-2.);	
							ws->var("coeffGaus")->setRange(0.2,0.8);
							ws->var("coeffGaus")->setVal(0.5);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("9.5-11.0")) {
          		ws->var("enneW")->setRange(2,50);
          		ws->var("enneW")->setVal(10);
							ws->var("coefExp")->setRange(0.02,0.085);
							ws->var("coefExp")->setVal(0.05);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("11.0-14.0")) {
							//ws->var("coefExp")->setRange(-3.0,-0.95);
							ws->var("coefExp")->setRange(-3.0,-0.97);
							ws->var("coefExp")->setVal(-2.);	
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("14.0-30.0")) {
							ws->var("coeffGaus")->setRange(0.70,0.93);
							ws->var("coeffGaus")->setVal(0.8);	
							ws->var("sigmaSig2")->setRange(0.02,0.05);
							ws->var("sigmaSig2")->setVal(0.04);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
					}
					else if (!inOpt.yrange.compare("0.47-1.47")) {
						if (!inOpt.prange.compare("14.0-30.0")) {
							ws->var("coeffGaus")->setRange(0.80,0.93);
							ws->var("coeffGaus")->setVal(0.86);	
							ws->var("sigmaSig2")->setRange(0.02,0.05);
							ws->var("sigmaSig2")->setVal(0.04);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
					}
					else if (!inOpt.yrange.compare("-0.53-0.47")) {
						if (!inOpt.prange.compare("7.5-8.5")) {
							ws->var("sigmaSig1")->setRange(0.01,0.02);
							ws->var("sigmaSig1")->setVal(0.015);
							ws->var("sigmaSig2")->setRange(0.01,0.05);
							ws->var("sigmaSig2")->setVal(0.02);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("8.5-9.5")) {
							ws->var("sigmaSig1")->setRange(0.01,0.02);
							ws->var("sigmaSig1")->setVal(0.015);
							ws->var("sigmaSig2")->setRange(0.01,0.05);
							ws->var("sigmaSig2")->setVal(0.02);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
					}
					else if (!inOpt.yrange.compare("-1.03--0.53")) {
						if (!inOpt.prange.compare("8.5-9.5")) {
							ws->var("coeffGaus")->setRange(0.7,0.93);
							ws->var("coeffGaus")->setVal(0.8);	
							ws->var("sigmaSig1")->setRange(0.027,0.06);
							ws->var("sigmaSig1")->setVal(0.035);
							ws->var("sigmaSig2")->setRange(0.01,0.035);
							ws->var("sigmaSig2")->setVal(0.018);
          		ws->var("enneW")->setRange(2,50);
          		ws->var("enneW")->setVal(10);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("14.0-30.0")) {
							ws->var("sigmaSig1")->setRange(0.01,0.03);
							ws->var("sigmaSig1")->setVal(0.02);
							ws->var("sigmaSig2")->setRange(0.02,0.05);
							ws->var("sigmaSig2")->setVal(0.035);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
					}
					else if (!inOpt.yrange.compare("-1.93--1.03")) {
						if (!inOpt.prange.compare("3.0-6.5")) {
							ws->var("coeffGaus")->setRange(0.12,0.93);
							ws->var("coeffGaus")->setVal(0.5);	
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
						else if (!inOpt.prange.compare("7.5-8.5")) {
							ws->var("coefExp")->setRange(-3.,-1.48);
							ws->var("coefExp")->setVal(-2.);
							//ws->var("coeffGaus")->setRange(0.12,0.93);
							//ws->var("coeffGaus")->setRange(0.4,0.93);
							ws->var("coeffGaus")->setRange(0.53,0.93);
							ws->var("coeffGaus")->setVal(0.7);	
							ws->var("sigmaSig1")->setRange(0.028,0.06);
							ws->var("sigmaSig1")->setVal(0.05);
							ws->var("sigmaSig2")->setRange(0.016,0.05);
							ws->var("sigmaSig2")->setVal(0.035);
							sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*%s,NBkg[%f,1.0,500000.0]*%s)",initSig,initBkg,inOpt.mSigFunct.c_str(),inOpt.mBkgFunct.c_str());
						}
					}
			} //isPbPb==3
		} // isMultiplicity==0

    ws->factory(funct);

//    if (inOpt.isPbPb == 1 || inOpt.isPbPb == 2) {
      if (dPhiConst) { //sigmaSig2 will be constrained too!
        fitM = ws->pdf("sigMassPDF")->fitTo(*redDataCut,ExternalConstraints(RooArgSet(*(ws->pdf("sigmaSig2Con")),*(ws->pdf("sigmaSig1Con")),*(ws->pdf("meanSig1Con")),*(ws->pdf("coeffGausCon")),*(ws->pdf("alphaCon")),*(ws->pdf("enneWCon")))),Extended(1),Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8));
      } else if (centConst && !dPhiConst) { //sigmaSig2 will be NOT constrained!
//        fitM = ws->pdf("sigMassPDF")->fitTo(*redDataCut,ExternalConstraints(RooArgSet(*(ws->pdf("meanSig1Con")),*(ws->pdf("coeffGausCon")),*(ws->pdf("alphaCon")),*(ws->pdf("enneWCon")))),Extended(1),Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8));
        fitM = ws->pdf("sigMassPDF")->fitTo(*redDataCut,ExternalConstraints(RooArgSet(*(ws->pdf("sigmaSig1Con")),*(ws->pdf("meanSig1Con")),*(ws->pdf("coeffGausCon")),*(ws->pdf("alphaCon")),*(ws->pdf("enneWCon")))),Extended(1),Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8));
      } else { // all free fit bin
        fitM = ws->pdf("sigMassPDF")->fitTo(*redDataCut,Extended(1),Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8));
      }
/*    } else if (inOpt.isPbPb == 0) {
      if (dPhiConst || centConst ) { //sigmaSig2 will be constrained too!
        fitM = ws->pdf("sigMassPDF")->fitTo(*redDataCut,ExternalConstraints(RooArgSet(*(ws->pdf("sigmaSig1Con")),*(ws->pdf("meanSig1Con")),*(ws->pdf("coeffGausCon")),*(ws->pdf("alphaCon")),*(ws->pdf("enneWCon")))),Extended(1),Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8));
      } else { // all free fit bin
        fitM = ws->pdf("sigMassPDF")->fitTo(*redDataCut,Extended(1),Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8));
      }
    }*/
    fitM->Print("v");
 
    ws->var("alpha")->setConstant(kTRUE);
    ws->var("enneW")->setConstant(kTRUE);
    ws->var("coeffGaus")->setConstant(kTRUE);
    ws->var("sigmaSig1")->setConstant(kTRUE);
    ws->var("sigmaSig2")->setConstant(kTRUE);
    ws->var("sigmaSig3")->setConstant(kTRUE);
    ws->var("meanSig1")->setConstant(kTRUE);
    ws->var("coefExp")->setConstant(kTRUE);
    ws->var("coefPol1")->setConstant(kTRUE);
    ws->var("NSig")->setConstant(kTRUE);
    ws->var("NBkg")->setConstant(kTRUE);

    // *** Get CB + Gaus combined width
    double mCoeffGaus = 0, mCoeffGausErr = 0;
    double mSigmaGaus = 0, mSigmaGausErr = 0;
    double mSigmaCB = 0,   mSigmaCBErr = 0;

    if (inOpt.mSigFunct == "sigCB2WNG1") {
      mSigmaCB = ws->var("sigmaSig2")->getVal();
      mSigmaGaus = ws->var("sigmaSig1")->getVal();
      mCoeffGaus = ws->var("coeffGaus")->getVal();
      mSigmaCBErr = ws->var("sigmaSig2")->getError();
      mSigmaGausErr = ws->var("sigmaSig1")->getError();
      mCoeffGausErr = ws->var("coeffGaus")->getError();

      inOpt.combinedWidth = sqrt( (1-mCoeffGaus)*pow(mSigmaCB,2) + mCoeffGaus*pow(mSigmaGaus,2) );
      inOpt.combinedWidthErr = (0.5/inOpt.combinedWidth) *
        sqrt ( pow(mCoeffGaus*mSigmaGaus*mSigmaGausErr,2) +
               pow((1-mCoeffGaus)*mSigmaCB*mSigmaCBErr,2) +
               pow(0.5*mCoeffGausErr*( pow(mSigmaGaus,2)-pow(mSigmaCB,2) ),2) );

    } else if (inOpt.mSigFunct == "signalCB3WN") {
      mSigmaCB = ws->var("sigmaSig3")->getVal();
      mSigmaCBErr = ws->var("sigmaSig3")->getError();
      inOpt.combinedWidth = mSigmaCB;
      inOpt.combinedWidthErr = mSigmaCBErr;

    } else if (!inOpt.mSigFunct.compare("sigCBWNG1") || !inOpt.mSigFunct.compare("signalCBWN")) {
      mSigmaCB = ws->var("sigmaSig1")->getVal();
      mSigmaCBErr = ws->var("sigmaSig1")->getError();
      inOpt.combinedWidth = mSigmaCB;
      inOpt.combinedWidthErr = mSigmaCBErr;

    } else {
      cout << "ERROR:: cannot get width of gaussian and crystal ball!!!" << endl;
      assert(0);
    }

    inOpt.PcombinedWidth = inOpt.combinedWidth*1000;
    inOpt.PcombinedWidthErr = inOpt.combinedWidthErr*1000;
    if(inOpt.PcombinedWidthErr < 1) inOpt.PcombinedWidthErr = 1;


    // Draw mass plot before do ctau fit
    if (!inOpt.doBfit) drawMassPlotsWithoutB(ws, redDataCut, fitM, inOpt);

  } else {
    RooRealVar NSig("NSig","dummy total signal events",0.);
    ws->import(NSig);
  }

  Double_t NSig_fin = ws->var("NSig")->getVal();
  Double_t ErrNSig_fin = ws->var("NSig")->getError();
  Double_t NBkg_fin = ws->var("NBkg")->getVal();
  Double_t ErrNBkg_fin = ws->var("NBkg")->getError();

  // *** Get NSig, NBkg, Bfraction and their errors
  Double_t NSigPR_fin, ErrNSigPR_fin;
  Double_t NSigNP_fin, ErrNSigNP_fin;
  Double_t Bfrac_fin, ErrBfrac_fin;
  int nFitPar;
  Double_t theNLL;
  double resol, Errresol;

  RooDataHist *subtrData, *weightedBkg;
  if (inOpt.doBfit) {  // skip ctau fitting
    // scaleF is defined to scale down ct err dist in 2.9-3.3 GeV/c2 signal region
    if (inOpt.prefitMass) {
      if (inOpt.isPEE == 1) {
        float bc;
        if (!inOpt.mBkgFunct.compare("expFunct")) bc = ws->var("coefExp")->getVal();
        else if (!inOpt.mBkgFunct.compare("polFunct")) bc = ws->var("coefPol1")->getVal();
        float scaleF = (exp(2.9*bc)-exp(3.3*bc))/(exp(2.6*bc)-exp(2.9*bc)+exp(3.3*bc)-exp(3.5*bc));
        subtrData = new RooDataHist("subtrData","Subtracted data",RooArgSet(*(ws->var("Jpsi_CtErr")))); 
        weightedBkg = subtractSidebands(ws,subtrData,binDataCtErrSIG,binDataCtErrSB,scaleF,"Jpsi_CtErr");
       
        RooHistPdf errPdfSig("errPdfSig","Error PDF signal",RooArgSet(*(ws->var("Jpsi_CtErr"))),*subtrData);  ws->import(errPdfSig);
        RooHistPdf errPdfBkg("errPdfBkg","Error PDF bkg",RooArgSet(*(ws->var("Jpsi_CtErr"))),*binDataCtErrSB);  ws->import(errPdfBkg);
        RooHistPdf errPdfBkgWeighted("errPdfBkgWeighted","Error PDF bkg weighted",RooArgSet(*(ws->var("Jpsi_CtErr"))),*weightedBkg);  ws->import(errPdfBkgWeighted);

        if (inOpt.ctauBackground == 0) {
         RooFormulaVar fBkg("fBkg","@0/(@0+@1)",RooArgList(*(ws->var("NBkg")),*(ws->var("NSig"))));    ws->import(fBkg);
         RooProdPdf bkgCtauTOT_PEE("bkgCtauTOT_PEE","PDF with PEE", *(ws->pdf("errPdfBkgWeighted")),
                                  Conditional(*(ws->pdf("bkgCtTot")),RooArgList(*(ws->var("Jpsi_Ct"))))
                                 );  ws->import(bkgCtauTOT_PEE);  

        } else if (inOpt.ctauBackground == 1) { //signal region fit
          RooAbsReal *nsig_all = ws->pdf(inOpt.mSigFunct.c_str())->createIntegral(*(ws->var("Jpsi_Mass")),NormSet(*(ws->var("Jpsi_Mass"))),Range(2.6,3.5));
          RooAbsReal *nbkg_all = ws->pdf(inOpt.mBkgFunct.c_str())->createIntegral(*(ws->var("Jpsi_Mass")),NormSet(*(ws->var("Jpsi_Mass"))),Range(2.6,3.5));
          RooAbsReal *nsig_inte = ws->pdf(inOpt.mSigFunct.c_str())->createIntegral(*(ws->var("Jpsi_Mass")),NormSet(*(ws->var("Jpsi_Mass"))),Range(2.9,3.2));
          RooAbsReal *nbkg_inte = ws->pdf(inOpt.mBkgFunct.c_str())->createIntegral(*(ws->var("Jpsi_Mass")),NormSet(*(ws->var("Jpsi_Mass"))),Range(2.9,3.2));
          double nbkg2 = nsig_inte->getVal()/nsig_all->getVal()*ws->var("NBkg")->getVal();
          double nsig2 = nbkg_inte->getVal()/nbkg_all->getVal()*ws->var("NSig")->getVal();
          RooRealVar NBkg2("NBkg2","NBkg2",nbkg2);
          RooRealVar NSig2("NSig2","NSig2",nsig2);
          RooFormulaVar fBkg("fBkg","@0/(@0+@1)",RooArgList(NBkg2,NSig2));    ws->import(fBkg);
          RooProdPdf bkgCtauTOTL_PEE("bkgCtauTOTL_PEE","PDF with PEE", *(ws->pdf("errPdfBkgWeighted")),
                                    Conditional(*(ws->pdf("bkgCtTotL")),RooArgList(*(ws->var("Jpsi_Ct"))))
                                   );  ws->import(bkgCtauTOTL_PEE);
          RooProdPdf bkgCtauTOTR_PEE("bkgCtauTOTR_PEE","PDF with PEE", *(ws->pdf("errPdfBkgWeighted")),
                                    Conditional(*(ws->pdf("bkgCtTotR")),RooArgList(*(ws->var("Jpsi_Ct"))))
                                   );  ws->import(bkgCtauTOTR_PEE);

        } else if (inOpt.ctauBackground == 2) { //step functions
          RooFormulaVar fBkg("fBkg","@0/(@0+@1)",RooArgList(*(ws->var("NBkg")),*(ws->var("NSig"))));    ws->import(fBkg);
          RooProdPdf bkgCtauTOTL_PEE("bkgCtauTOTL_PEE","PDF with PEE", *(ws->pdf("errPdfBkgWeighted")),
                                    Conditional(*(ws->pdf("bkgCtTotL")),RooArgList(*(ws->var("Jpsi_Ct"))))
                                   );  ws->import(bkgCtauTOTL_PEE);
          RooProdPdf bkgCtauTOTR_PEE("bkgCtauTOTR_PEE","PDF with PEE", *(ws->pdf("errPdfBkgWeighted")),
                                    Conditional(*(ws->pdf("bkgCtTotR")),RooArgList(*(ws->var("Jpsi_Ct"))))
                                   );  ws->import(bkgCtauTOTR_PEE);
          RooGenericPdf bkgCtTot("bkgCtTot","bkgCtTot","( @1*(@0<=2.9) + ((1-@3)*@1 +@3*@2)*(@0>2.9 && @0<3.3) + @2*(@0>=3.3) )", RooArgList(*(ws->var("Jpsi_Mass")),bkgCtauTOTL_PEE,bkgCtauTOTR_PEE,*(ws->var("fbkgCtTotR")))); ws->import(bkgCtTot);

        } else {
          cout << "ctau background function type is not defined. exit.\n";
          return -2;
        } // end of ctau background function variation

        sprintf(funct,"PROD::totSIGPR(%s,sigPR)",inOpt.mSigFunct.c_str()); ws->factory(funct);
        sprintf(funct,"PROD::totSIGNP(%s,sigNP)",inOpt.mSigFunct.c_str()); ws->factory(funct);
        sprintf(funct,"PROD::totBKG(%s,bkgCtTot)",inOpt.mBkgFunct.c_str());  ws->factory(funct);
        RooProdPdf totSIGPR_PEE("totSIGPR_PEE","PDF with PEE", *(ws->pdf("errPdfSig")),
                               Conditional( *(ws->pdf("totSIGPR")), RooArgList(*(ws->var("Jpsi_Ct")),*(ws->var("Jpsi_Mass"))) )
                               );  ws->import(totSIGPR_PEE);
        RooProdPdf totSIGNP_PEE("totSIGNP_PEE","PDF with PEE", *(ws->pdf("errPdfSig")),
                               Conditional( *(ws->pdf("totSIGNP")), RooArgList(*(ws->var("Jpsi_Ct")),*(ws->var("Jpsi_Mass"))))
                               );  ws->import(totSIGNP_PEE);    
        RooProdPdf totBKG_PEE("totBKG_PEE","PDF with PEE", *(ws->pdf("errPdfBkgWeighted")),
                             Conditional( *(ws->pdf("totBKG")), RooArgList(*(ws->var("Jpsi_Ct")),*(ws->var("Jpsi_Mass"))) )
                             );  ws->import(totBKG_PEE);
        
        ws->factory("RSUM::totPDF_PEE(fBkg*totBKG_PEE,Bfrac[0.25,0.0,1.]*totSIGNP_PEE,totSIGPR_PEE)");
        //ws->factory("RSUM::totPDF_PEE(fBkg*totBKG_PEE,Bfrac[0.25,0.2,0.8]*totSIGNP_PEE,totSIGPR_PEE)");

        // ** Test Ct error distribution on the sideband region
        ctauErrDistCheck(ws, binDataCtErrSB, binDataCtErrSIG, subtrData, weightedBkg, inOpt);
      
      } else if (inOpt.isPEE == 0) {
        RooFormulaVar fSig("fSig","@0/(@0+@1)",RooArgList( *(ws->var("NSig")),*(ws->var("NBkg")) ));  ws->import(fSig);
        ws->factory("SUM::sigCtPDF(Bfrac[0.1,0.0,1.0]*sigNP,sigPR)");
        //ws->factory("SUM::sigCtPDF(Bfrac[0.25,0.2,0.8]*sigNP,sigPR)");
        sprintf(funct,"PROD::totBKG(%s,bkgCtTot)",inOpt.mBkgFunct.c_str());  ws->factory(funct);
        sprintf(funct,"PROD::totSIG(%s,sigCtPDF)",inOpt.mSigFunct.c_str());  ws->factory(funct);
        ws->factory("SUM::totPDF(fSig*totSIG,totBKG)");   //Final F(l,m)
      }
     
    } else {
      cout << "Should use prefit mass option. exit.\n";
      return -1;
    } //end of prefitmass option

    // *** Start prefit on the signal ctau function
    RooFitResult *fitPR, *fitSB, *fitSBR, *fitSBL;
    double RSS = 0;
    unsigned int nFullBinsResid = 0;
    RooArgSet* newRow;
    RooDataSet* tempJpsi;
    if (inOpt.prefitSignalCTau) {
      RooPlot *tframePR;
      if (inOpt.isPEE == 1) {
        RooProdPdf sigPR_PEE("sigPR_PEE","PDF with PEE", *(ws->pdf("errPdfSig")),
                            Conditional(*(ws->pdf("sigPR")), RooArgList(*(ws->var("Jpsi_Ct"))))
                            );  ws->import(sigPR_PEE);

        fitPR = ws->pdf("sigPR_PEE")->fitTo(*redMCCutPR,Range("promptfit"),SumW2Error(kTRUE),ConditionalObservables(RooArgSet(*(ws->var("Jpsi_CtErr")))),Save(1),NumCPU(8));
        fitPR->Print("v");

        if (ws->var("sigmaResSigW")) ws->var("sigmaResSigW")->setConstant(kTRUE);
        ws->var("meanResSigW")->setConstant(kTRUE);
        if (ws->var("meanResSigN")) ws->var("meanResSigN")->setConstant(kTRUE);
        ws->var("meanResSigN")->setConstant(kTRUE);

        // Check prompt fit is fine with per event error fit. CtWeighted means l/err l
        RooRealVar* CtWeighted = new RooRealVar("CtWeighted","#font[12]{l}_{J/#psi} / #sigma( #font[12]{l}_{J/#psi} )",-5.,5.);
        ws->import(*CtWeighted);
        const RooArgSet* thisRowpr = (RooArgSet*)redMCCutPR->get(0); 
        newRow = new RooArgSet(*CtWeighted);
        tempJpsi = new RooDataSet("tempJpsi","new data",*newRow);
        for (Int_t iSamp = 0; iSamp < redMCCutPR->numEntries(); iSamp++) {
          thisRowpr = (RooArgSet*)redMCCutPR->get(iSamp);
          RooRealVar* myct = (RooRealVar*)thisRowpr->find("Jpsi_Ct");
          RooRealVar* mycterr = (RooRealVar*)thisRowpr->find("Jpsi_CtErr");
          CtWeighted->setVal(myct->getVal()/mycterr->getVal());
          RooArgSet* tempRow = new RooArgSet(*CtWeighted);
          tempJpsi->add(*tempRow);
        }

        if (inOpt.oneGaussianResol) {
          ws->factory("Gaussian::tempsigPR(CtWeighted,meanResSigW,sigmaResSigN)");
        } else {
          ws->factory("Gaussian::tempresGW(CtWeighted,meanResSigW,sigmaResSigW)");
//          ws->factory("Gaussian::tempresGN(CtWeighted,meanResSigW,sigmaResSigN)");
          ws->factory("Gaussian::tempresGN(CtWeighted,meanResSigN,sigmaResSigN)");
          ws->factory("SUM::tempsigPR(fracRes*tempresGW,tempresGN)");
        }  

        tframePR = ws->var("CtWeighted")->frame();
        tempJpsi->plotOn(tframePR,DataError(RooAbsData::SumW2));
        ws->pdf("tempsigPR")->plotOn(tframePR,LineColor(kBlue),Normalization(tempJpsi->sumEntries(),RooAbsReal::NumEvent));

      } else if (inOpt.isPEE == 0) {
        fitPR = ws->pdf("sigPR")->fitTo(*redMCCutPR,SumW2Error(kTRUE),Save(1),NumCPU(8));
        fitPR->Print("v");

        if (ws->var("sigmaResSigO")) ws->var("sigmaResSigO")->setConstant(kTRUE);
        if (ws->var("sigmaResSigM")) ws->var("sigmaResSigM")->setConstant(kTRUE);
        if(inOpt.fracfix){
          if (ws->var("fracRes2")) ws->var("fracRes2")->setConstant(kTRUE);
          if (ws->var("fracRes3")) ws->var("fracRes3")->setConstant(kTRUE);
        }
        ws->var("fracRes")->setConstant(kTRUE);

        tframePR = ws->var("Jpsi_Ct")->frame();
        tframePR->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
        redMCCutPR->plotOn(tframePR,DataError(RooAbsData::SumW2));
        ws->pdf("sigPR")->plotOn(tframePR,LineColor(kBlue),Normalization(redMCCutPR->sumEntries(),RooAbsReal::NumEvent));
      }

      // Plot resolution functions
      ctauResolFitCheck(ws, true, tframePR, inOpt);
    
    } else {
      cout << "Please check running option and turn on prefitSignalCTau\n";
      return -1;
    } //end of (prefitSignalCTau) option

    double bfraction[2] = {0};
    if (inOpt.prefitBkg) {
  //    ws->var("fpm")->setConstant(kTRUE); //mh

      if (inOpt.prefitSignalCTau && inOpt.isPEE == 1) {
        if (ws->var("fracRes")) ws->var("fracRes")->setConstant(kTRUE);   //modified for pPb
        ws->var("meanResSigW")->setConstant(kTRUE);
        if (ws->var("meanResSigN")) ws->var("meanResSigN")->setConstant(kTRUE);
        ws->var("sigmaResSigN")->setConstant(kTRUE);  //modified for pPb
        if (ws->var("sigmaResSigW")) ws->var("sigmaResSigW")->setConstant(kTRUE);//modified
      } else if (inOpt.prefitSignalCTau && inOpt.isPEE == 0) {
        ws->var("sigmaResSigW")->setConstant(kTRUE);
        ws->var("meanResSigW")->setConstant(kTRUE);
        if (ws->var("meanResSigN")) ws->var("meanResSigN")->setConstant(kTRUE);
  //      ws->var("sigmaResSigN")->setConstant(kFALSE);
        ws->var("meanResSigN")->setConstant(kFALSE);
      }

      cout << "DATA :: N events to fit on the sidebands: " << redDataSB->sumEntries() << endl;
      if (inOpt.isPEE == 1) {
        if (inOpt.ctauBackground == 0) {
          fitSB = ws->pdf("bkgCtauTOT_PEE")->fitTo(*redDataSB,SumW2Error(kTRUE),Minos(0),NumCPU(8),Save(1),ConditionalObservables(RooArgSet(*(ws->var("Jpsi_CtErr")))),Optimize(0));
          fitSB->Print("v");
        } else if (inOpt.ctauBackground == 1 || inOpt.ctauBackground == 2) {
          fitSBR = ws->pdf("bkgCtauTOTR_PEE")->fitTo(*redDataSBR,SumW2Error(kTRUE),Minos(0),NumCPU(8),Save(1),ConditionalObservables(RooArgSet(*(ws->var("Jpsi_CtErr")))));
          fitSBR->Print("v");
          fitSBL = ws->pdf("bkgCtauTOTL_PEE")->fitTo(*redDataSBL,SumW2Error(kTRUE),Minos(0),NumCPU(8),Save(1),ConditionalObservables(RooArgSet(*(ws->var("Jpsi_CtErr")))));
          fitSBL->Print("v");
        }
      } else {
        fitSB = ws->pdf("bkgCtTot")->fitTo(*redDataSB,SumW2Error(kTRUE),Save(1),NumCPU(8),Optimize(0));
        fitSB->Print("v");
      }

      if (inOpt.isPEE == 0 || (inOpt.isPEE == 1 && inOpt.ctauBackground == 0)) {
        ws->var("fpm")->setConstant(kTRUE);
        //ws->var("fpm2")->setConstant(kTRUE);
        ws->var("fLiving")->setConstant(kTRUE);
        ws->var("fbkgCtTot")->setConstant(kTRUE);
        ws->var("lambdap")->setConstant(kTRUE);
        //ws->var("lambdap2")->setConstant(kTRUE);
        ws->var("lambdam")->setConstant(kTRUE);
        ws->var("lambdasym")->setConstant(kTRUE);

      } else if ((inOpt.isPEE == 1 && inOpt.ctauBackground == 1) || (inOpt.isPEE == 1 && inOpt.ctauBackground == 2)) {
        ws->var("fpmL")->setConstant(kTRUE);
        ws->var("fLivingL")->setConstant(kTRUE);
        ws->var("fbkgCtTotLeft")->setConstant(kTRUE);
        ws->var("lambdapL")->setConstant(kTRUE);
        ws->var("lambdamL")->setConstant(kTRUE);
        ws->var("lambdasymL")->setConstant(kTRUE);
        
        ws->var("fpmR")->setConstant(kTRUE);
        ws->var("fLivingR")->setConstant(kTRUE);
        ws->var("fbkgCtTotRight")->setConstant(kTRUE);
        ws->var("lambdapR")->setConstant(kTRUE);
        ws->var("lambdamR")->setConstant(kTRUE);
        ws->var("lambdasymR")->setConstant(kTRUE);
      }

      
      if (inOpt.prefitSignalCTau && inOpt.isPEE == 1) {
        if (!inOpt.fixResol2MC) {
          if (ws->var("fracRes")) ws->var("fracRes")->setConstant(kFALSE);
          ws->var("meanResSigW")->setConstant(kFALSE);
          ws->var("meanResSigN")->setConstant(kFALSE);
          ws->var("sigmaResSigN")->setConstant(kFALSE);//modified
          if (ws->var("sigmaResSigW")) ws->var("sigmaResSigW")->setConstant(kFALSE);//modified
        } else {
          if (ws->var("fracRes")) ws->var("fracRes")->setConstant(kTRUE);
          ws->var("meanResSigW")->setConstant(kTRUE);
          ws->var("meanResSigN")->setConstant(kTRUE);
          if (ws->var("meanResSigN")) ws->var("meanResSigN")->setConstant(kTRUE);
          if (ws->var("sigmaResSigW")) ws->var("sigmaResSigW")->setConstant(kFALSE);//modified
          if (ws->var("sigmaResSigN")) ws->var("sigmaResSigN")->setConstant(kTRUE);//modified
        }
      } else if (inOpt.prefitSignalCTau && inOpt.isPEE == 0) {
        ws->var("meanResSigW")->setConstant(kFALSE);
        ws->var("meanResSigN")->setConstant(kFALSE);
        ws->var("sigmaResSigW")->setConstant(kFALSE);
  //      ws->var("sigmaResSigO")->setConstant(kTRUE);
  //      ws->var("sigmaResSigM")->setConstant(kTRUE);
        if(!inOpt.fracfix){
          ws->var("fracRes2")->setConstant(kTRUE);
          ws->var("fracRes3")->setConstant(kTRUE);
        }
      }
      
      drawCtauSBPlots(ws, redDataSB, redDataSBL, redDataSBR, binDataCtErrSB, fitSB, fitSBL, fitSBR, inOpt);
    }

    // Fix below bkg variables in any case
    if (inOpt.ctauBackground == 0) {
      ws->var("fpm")->setConstant(kTRUE);
      ws->var("fLiving")->setConstant(kTRUE);
    } else if (inOpt.ctauBackground == 1 || inOpt.ctauBackground == 2) {
      ws->var("fpmL")->setConstant(kTRUE);
      ws->var("fLivingL")->setConstant(kTRUE);
      ws->var("fpmR")->setConstant(kTRUE);
      ws->var("fLivingR")->setConstant(kTRUE);
    }

		// Special bin setting - KYO 
/*		if (inOpt.isMultiplicity == 0) { // no multiplicity
			if (!inOpt.yrange.compare("-2.4--1.97")) {
				if (!inOpt.prange.compare("0.0-3.0")) {
					cout << "XIA free param fracRes " << endl;
          if (ws->var("fracRes")) ws->var("fracRes")->setConstant(kFALSE);
				}
			}
			else if (!inOpt.yrange.compare("1.97-2.4")) {
				if (!inOpt.prange.compare("0.0-3.0")) {
					cout << "XIA free param fracRes " << endl;
          if (ws->var("fracRes")) ws->var("fracRes")->setConstant(kFALSE);
				}
			}
		}
*/
					//ws->var("bTau")	->setConstant(kTRUE);
					//ws->var("sigmaResSigW")	->setConstant(kTRUE);
    // *** Get NSig, NBkg, Bfraction and their errors
    if (inOpt.prefitMass) {
      if (inOpt.isPEE == 1) {
        if (inOpt.ctauBackground == 0 || inOpt.ctauBackground == 2) {
          fit2D = ws->pdf("totPDF_PEE")->fitTo(*redDataCut,Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8),ConditionalObservables(RooArgSet(*(ws->var("Jpsi_CtErr")))));
        } else if (inOpt.ctauBackground == 1) {
          fit2D = ws->pdf("totPDF_PEE")->fitTo(*redDataSIGWide,Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8),ConditionalObservables(RooArgSet(*(ws->var("Jpsi_CtErr")))));
        }
      } else {
        fit2D = ws->pdf("totPDF")->fitTo(*redDataCut,Save(1),SumW2Error(kTRUE),NumCPU(8));
      }
      fit2D->Print("v");
      nFitPar = fit2D->floatParsFinal().getSize();
      theNLL = fit2D->minNll();
      Bfrac_fin = ws->var("Bfrac")->getVal();
      ErrBfrac_fin = ws->var("Bfrac")->getError();
      if (ws->var("fracRes")) ws->var("fracRes")->setConstant(kTRUE);
      ws->var("meanResSigW")->setConstant(kTRUE);
      ws->var("meanResSigN")->setConstant(kTRUE);
      if (ws->var("meanResSigN")) ws->var("meanResSigN")->setConstant(kTRUE);
      NSigNP_fin = NSig_fin * Bfrac_fin;
      NSigPR_fin = NSig_fin * (1-Bfrac_fin);
      ErrNSigNP_fin = NSigNP_fin * sqrt( pow(ErrNSig_fin/NSig_fin,2)+pow(ErrBfrac_fin/Bfrac_fin,2) );
      ErrNSigPR_fin = NSigPR_fin * sqrt ( pow(ErrNSig_fin/NSig_fin,2)+pow(ErrBfrac_fin/(1.0-Bfrac_fin),2) );
    } else {
      fit2D = ws->pdf("totPDF")->fitTo(*redDataCut,Extended(1),Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8));
      nFitPar = fit2D->floatParsFinal().getSize();
      NSigNP_fin = ws->var("NSigNP")->getVal();
      NSigPR_fin = ws->var("NSigPR")->getVal();
      ErrNSigNP_fin = ws->var("NSigNP")->getError();
      ErrNSigPR_fin = ws->var("NSigPR")->getError();
      Bfrac_fin = NSigNP_fin/(NSigNP_fin+NSigPR_fin);
      ErrBfrac_fin = sqrt( pow(NSigNP_fin*ErrNSigPR_fin,2) + pow(NSigPR_fin*ErrNSigNP_fin,2) ) / pow(NSigNP_fin+NSigPR_fin,2);
    }

    const double sigmaSig2 = ws->var("sigmaResSigN")->getVal();
    const double ErrsigmaSig2 = ws->var("sigmaResSigN")->getError();
    if (inOpt.oneGaussianResol) {
      resol = sigmaSig2;
      Errresol = ErrsigmaSig2;
    } else {
      const double coeffGaus = ws->var("fracRes")->getVal();
      const double ErrcoeffGaus = ws->var("fracRes")->getError();
      const double sigmaSig1 = ws->var("sigmaResSigW")->getVal();
      const double ErrsigmaSig1 = ws->var("sigmaResSigW")->getError();

      resol = sqrt( coeffGaus*pow(sigmaSig1,2) + (1-coeffGaus)*pow(sigmaSig2,2) );
      Errresol = (0.5/resol) *
                    sqrt( pow(sigmaSig1*coeffGaus*ErrsigmaSig1,2) +
                          pow(sigmaSig2*(1-coeffGaus)*ErrsigmaSig2,2) +
                          pow(0.5*(pow(sigmaSig1,2)-pow(sigmaSig2,2))*ErrcoeffGaus,2) );
    }

  } // end of skipping ctau fitting

  // To check values of fit parameters
  cout << endl << "J/psi yields:" << endl;
  cout << "NSig :       Fit :"  << NSig_fin << " +/- " << ErrNSig_fin << endl;
  if (inOpt.doBfit) {
    cout << "PROMPT :     Fit : " << NSigPR_fin << " +/- " << ErrNSigPR_fin << endl;
    cout << "NON-PROMPT : Fit : " << NSigNP_fin << " +/- " << ErrNSigNP_fin << endl;
    cout << "Bfraction : Fit : " << Bfrac_fin << " +/- " << ErrBfrac_fin << endl;
    cout << "Resolution : Fit : " << resol*1000. << " +/- " << Errresol*1000. << " mum" << endl;
  }

  titlestr = inOpt.dirPre + "_rap" + inOpt.yrange + "_pT" + inOpt.prange + "_cent" + inOpt.crange + "_dPhi" + inOpt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + ".txt";

  int cutEntry = redData->sumEntries();
  int noCutEntry = redData_2->sumEntries();

  ofstream outputFile(titlestr.c_str());
  if (!outputFile.good()) {cout << "Fail to open result file." << endl; return 1;}
  outputFile
  << "ctauErrMin "   << errmin                            << " 0 " << "\n"
  << "ctauErrMax "   << errmax                            << " 0 " << "\n"
  << "NoCutEntry "   << noCutEntry                        << " 0 " << "\n"
  << "CutEntry "     << cutEntry                          << " 0 " << "\n"
  << "NSig "         << NSig_fin                          << " " << ErrNSig_fin << "\n"
  << "NBkg "         << NBkg_fin                          << " " << ErrNBkg_fin << "\n"
  << "coefExp "      << ws->var("coefExp")->getVal()      << " " << ws->var("coefExp")->getError() << "\n"
  << "coefPol1 "     << ws->var("coefPol1")->getVal()     << " " << ws->var("coefPol1")->getError() << "\n"
  << "coeffGaus "    << ws->var("coeffGaus")->getVal()    << " " << ws->var("coeffGaus")->getError() << "\n"
  << "meanSig1 "     << ws->var("meanSig1")->getVal()     << " " << ws->var("meanSig1")->getError() << "\n"
  << "sigmaSig1 "    << ws->var("sigmaSig1")->getVal()    << " " << ws->var("sigmaSig1")->getError() << "\n"
  << "sigmaSig2 "    << ws->var("sigmaSig2")->getVal()    << " " << ws->var("sigmaSig2")->getError()<< "\n"
  << "sigmaSig3 "    << ws->var("sigmaSig3")->getVal()    << " " << ws->var("sigmaSig3")->getError()<< "\n"
  << "alpha "        << ws->var("alpha")->getVal()        << " " << ws->var("alpha")->getError() << "\n"
  << "enne "         << ws->var("enne")->getVal()         << " " << ws->var("enne")->getError() << "\n"
  << "enneW "        << ws->var("enneW")->getVal()        << " " << ws->var("enneW")->getError() << "\n"
  << "sigWidth "     << inOpt.combinedWidth               << " " << inOpt.combinedWidthErr << "\n";
  if (inOpt.analyticBlifetime) {
    if (inOpt.isPbPb == 1) {
      outputFile
      << "cutx "        << ws->var("cutx")->getVal()          << " " << ws->var("cutx")->getError() <<  "\n"
      << "Gmc "         << ws->var("Gmc")->getVal()           << " " << ws->var("Gmc")->getError() <<  "\n"
      << "bTau1 "       << ws->var("bTau1")->getVal()         << " " << ws->var("bTau1")->getError() <<  "\n"
      << "bTau2 "       << ws->var("bTau2")->getVal()         << " " << ws->var("bTau2")->getError() <<  "\n";
    } else {
      outputFile
//      << "cutx "        << ws->var("cutx")->getVal()          << " " << ws->var("cutx")->getError() <<  "\n"
//      << "Gmc "         << ws->var("Gmc")->getVal()           << " " << ws->var("Gmc")->getError() <<  "\n"
//      << "bTau1 "       << ws->var("bTau1")->getVal()         << " " << ws->var("bTau1")->getError() <<  "\n"
//      << "bTau2 "       << ws->var("bTau2")->getVal()         << " " << ws->var("bTau2")->getError() <<  "\n";
      << "Gmc "         << ws->var("Gmc")->getVal()           << " " << ws->var("Gmc")->getError() <<  "\n"
      << "bTau "        << ws->var("bTau")->getVal()          << " " << ws->var("bTau")->getError() <<  "\n";
    }
  } else {
    outputFile
    << "cutx "        << "0"                               << " " << "0" <<  "\n"
    << "Gmc "         << "0"                               << " " << "0" <<  "\n"
    << "bTau1 "       << "0"                               << " " << "0" <<  "\n"
    << "bTau2 "       << "0"                               << " " << "0" <<  "\n";
  }
  if (!inOpt.oneGaussianResol && inOpt.isPEE == 1) {
    outputFile
    << "fracRes "      << ws->var("fracRes")->getVal()      << " " << ws->var("fracRes")->getError() <<  "\n";
  }
  if (inOpt.isPEE == 0) {
    outputFile
    << "fracRes "      << ws->var("fracRes")->getVal()      << " " << ws->var("fracRes")->getError() <<  "\n"
    << "fracRes2 "     << ws->var("fracRes2")->getVal()     << " " << ws->var("fracRes2")->getError() << "\n"
    << "fracRes3 "     << ws->var("fracRes3")->getVal()     << " " << ws->var("fracRes3")->getError() << "\n";
  }
  outputFile
  << "meanResSigW "  << ws->var("meanResSigW")->getVal()  << " " << ws->var("meanResSigW")->getError() << "\n";
//  if (ws->var("meanResSigN")) 
  outputFile
  << "meanResSigN "  << ws->var("meanResSigN")->getVal()  << " " << ws->var("meanResSigN")->getError() << "\n";
  if (!inOpt.oneGaussianResol && inOpt.isPEE == 1) {
    outputFile
    << "sigmaResSigW " << ws->var("sigmaResSigW")->getVal() << " " << ws->var("sigmaResSigW")->getError() <<  "\n";
  }
  outputFile
  << "sigmaResSigN " << ws->var("sigmaResSigN")->getVal() << " " << ws->var("sigmaResSigN")->getError() << "\n";
  if (inOpt.isPEE == 0) {
    outputFile
    << "sigmaResSigM " << ws->var("sigmaResSigM")->getVal() << " " << ws->var("sigmaResSigM")->getError() << "\n"
    << "sigmaResSigO " << ws->var("sigmaResSigO")->getVal() << " " << ws->var("sigmaResSigO")->getError() << "\n";
  }
  if (inOpt.ctauBackground == 0) {
    outputFile
    << "fpm "          << ws->var("fpm")->getVal()          << " " << ws->var("fpm")->getError() << "\n"
    //<< "fpm2 "         << ws->var("fpm2")->getVal()         << " " << ws->var("fpm2")->getError() << "\n"
    << "fLiving "      << ws->var("fLiving")->getVal()      << " " << ws->var("fLiving")->getError() << "\n"
    << "fbkgCtTot "    << ws->var("fbkgCtTot")->getVal()    << " " << ws->var("fbkgCtTot")->getError() << "\n"
    << "lambdam "      << ws->var("lambdam")->getVal()      << " " << ws->var("lambdam")->getError() << "\n"
    << "lambdap "      << ws->var("lambdap")->getVal()      << " " << ws->var("lambdap")->getError() << "\n"
    //<< "lambdap2 "     << ws->var("lambdap2")->getVal()     << " " << ws->var("lambdap2")->getError() << "\n"
    << "lambdasym "    << ws->var("lambdasym")->getVal()    << " " << ws->var("lambdasym")->getError() << "\n";
  } else if (inOpt.ctauBackground == 1 || inOpt.ctauBackground == 2) {
    outputFile
    << "sigmaResSigN " << ws->var("sigmaResSigN")->getVal() << " " << ws->var("sigmaResSigN")->getError() << "\n"
    << "fpmL "          << ws->var("fpmL")->getVal()          << " " << ws->var("fpmL")->getError() << "\n"
    << "fLivingL "      << ws->var("fLivingL")->getVal()      << " " << ws->var("fLivingL")->getError() << "\n"
    << "fbkgCtTotLeft "    << ws->var("fbkgCtTotLeft")->getVal()    << " " << ws->var("fbkgCtTotLeft")->getError() << "\n"
    << "lambdamL "      << ws->var("lambdamL")->getVal()      << " " << ws->var("lambdamL")->getError() << "\n"
    << "lambdapL "      << ws->var("lambdapL")->getVal()      << " " << ws->var("lambdapL")->getError() << "\n"
    << "lambdasymL "    << ws->var("lambdasymL")->getVal()    << " " << ws->var("lambdasymL")->getError() << "\n"
    << "fpmR "          << ws->var("fpmR")->getVal()          << " " << ws->var("fpmR")->getError() << "\n"
    << "fLivingR "      << ws->var("fLivingR")->getVal()      << " " << ws->var("fLivingR")->getError() << "\n"
    << "fbkgCtTotRight "    << ws->var("fbkgCtTotRight")->getVal()    << " " << ws->var("fbkgCtTotRight")->getError() << "\n"
    << "lambdamR "      << ws->var("lambdamR")->getVal()      << " " << ws->var("lambdamR")->getError() << "\n"
    << "lambdapR "      << ws->var("lambdapR")->getVal()      << " " << ws->var("lambdapR")->getError() << "\n"
    << "lambdasymR "    << ws->var("lambdasymR")->getVal()    << " " << ws->var("lambdasymR")->getError() << "\n"
    << "fbkgCtTotR "    << ws->var("fbkgCtTotR")->getVal()    << " " << ws->var("fbkgCtTotR")->getError() << "\n";
  }
  if (inOpt.doBfit) {
    outputFile
    << "NLL "          << theNLL                            << endl
    << "PROMPT "       << NSigPR_fin                        << " " << ErrNSigPR_fin << endl
    << "NON-PROMPT "   << NSigNP_fin                        << " " << ErrNSigNP_fin << endl
    << "Bfraction "    << Bfrac_fin                         << " " << ErrBfrac_fin << endl
    << "Resolution "   << resol*1000.                       << " " << Errresol*1000. << endl;
  } else {
    outputFile
    << "PROMPT "       <<"0 0" << endl
    << "NON-PROMPT "   <<"0 0" << endl
    << "Bfraction "    <<"0 0" << endl;
  }

  outputFile.close();

  if (inOpt.doBfit) {  // skip ctau fit plotting
    // Plot various fit results and data points
    drawMassPlotsWithB(ws, redDataCut, NSigNP_fin, NBkg_fin, fitM, inOpt);

    RooPlot *tframePR;
    if (inOpt.isPEE == 1) {
      // Ctau plots with/without pull distribution
      // Check prompt fit is fine with per event error fit. CtWeighted means l/err l
      RooRealVar *CtWeighted = (RooRealVar*)ws->var("CtWeighted");
      const RooArgSet *thisRow = (RooArgSet*)redDataCut->get(0); 
      RooArgSet *newRow = new RooArgSet(*CtWeighted);
      RooDataSet *tempJpsiD = new RooDataSet("tempJpsiD","new data",*newRow);
      for (Int_t iSamp = 0; iSamp < redDataCut->numEntries(); iSamp++) {
        thisRow = (RooArgSet*)redDataCut->get(iSamp);
        RooRealVar *myct = (RooRealVar*)thisRow->find("Jpsi_Ct");
        RooRealVar *mycterr = (RooRealVar*)thisRow->find("Jpsi_CtErr");
        CtWeighted->setVal(myct->getVal()/mycterr->getVal());
        RooArgSet* tempRowD = new RooArgSet(*CtWeighted);
        tempJpsiD->add(*tempRowD);
      }

      tframePR = ws->var("CtWeighted")->frame();
      tempJpsiD->plotOn(tframePR,DataError(RooAbsData::SumW2));
      ws->pdf("tempsigPR")->plotOn(tframePR,LineColor(kBlue),Normalization(tempJpsiD->sumEntries(),RooAbsReal::NumEvent));

    } else if (inOpt.isPEE == 0) {
      tframePR = ws->var("Jpsi_Ct")->frame();
      tframePR->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
      redMCCutPR->plotOn(tframePR,DataError(RooAbsData::SumW2));
      ws->pdf("sigPR")->plotOn(tframePR,LineColor(kBlue),Normalization(redMCCutPR->sumEntries(),RooAbsReal::NumEvent));
    }
    
    // Plot ctau plots with signal only
//    drawCtauFitPlotsSignals(ws, redDataCut, redDataSB, redMCCutNP, NSigPR_fin, NSigNP_fin, inOpt);

    // Plot resolution functions
    ctauResolFitCheck(ws, false, tframePR, inOpt);

    // Plot ctau plots
    drawCtauFitPlots(ws, redDataCut, binDataCtErr, NSigNP_fin, NBkg_fin, fit2D, inOpt);
  
    drawMassCtau2DPlots(ws, inOpt) ;
  } // end of skip ctau fitting

  fInMC.Close();
  fInMC2.Close();
  fInData.Close();

  if (inOpt.dataMerge) {fInData2->Close();}
  return 0;
}

/////////////////////////////////////////////////////////
////////// Sub-routines for plotting ////////////////////
/////////////////////////////////////////////////////////
void parseInputArg(int argc, char* argv[], InputOpt &opt) {

  // Some parameters are set to default values
	opt.dataMerge = 0;
  opt.prefitMass = 1;
  opt.prefitSignalCTau = 1;
  opt.prefitBkg = 1;
  opt.fracfix = 0;    // fracfree will be used
  opt.isGG = 2;   // trk-trk dimuon
  opt.isMB = 0;
  opt.doWeight = 0;   // not use weighting
	opt.isMultiplicity = 0;
 
  opt.analyticBlifetime = 0;
  opt.doBfit = 0;
  opt.narrowSideband = 0;
  opt.oneGaussianResol = 1;
  opt.fixResol2MC = 0;
  opt.usedPhi = 0;
  opt.isPbPb = 2;	//pA
  opt.isPEE = 1;
  opt.is2Widths = 1;
  opt.ctauBackground = 0;

	opt.ctErrRange = 1; //0: ctau error range will be inserted from other file, 1: normal 2: const value
	//opt.ctErrFile = "/afs/cern.ch/work/m/miheejo/private/cms442_Jpsi/src/JpsiRaaRegIt/RegIt/";
	opt.ctErrFile = "/afs/cern.ch/work/k/kyolee/private/cms442/src/pAJpsi_double_Cent/outCtErr/";

  for (int i=1; i<argc; i++) {
    char *tmpargv = argv[i];
    switch (tmpargv[0]) {
      case '-':{
        switch (tmpargv[1]) {
					case 'q':
            if (atoi(argv[i+1]) == 0) {
              opt.dataMerge = false;
              cout << "         only one rooData root file is used" << endl;
              cout << "         option :: -f fileName" << endl;
            } else if (atoi(argv[i+1]) == 1) {
              opt.dataMerge = true;
              cout << "         Two rooData root file are open and will be merged " << endl;
							opt.FileName2 = argv[i+2]; 
							cout << "Data file2: "<< opt.FileName2 << endl;
						}
						break;
          case 'f':
            opt.FileName = argv[i+1];
            cout << "Data file: " << opt.FileName << endl;
            opt.doWeight = atoi(argv[i+2]);
            cout << "Weighting option: " << opt.doWeight << endl;
            break;
          case 'm':
            opt.FileNameMC1 = argv[i+1];
            opt.FileNameMC2 = argv[i+2];
            cout << "MC data file 1: " << opt.FileNameMC1 << endl;
            cout << "MC data file 2: " << opt.FileNameMC2 << endl;
            break;
          case 'v':
            opt.mSigFunct = argv[i+1];
            opt.mBkgFunct = argv[i+2];
            cout << "Mass signal function: " << opt.mSigFunct << endl;
            cout << "Mass background function: " << opt.mBkgFunct << endl;
            break;
          case 'r':
            opt.rpmethod = argv[i+1];
            cout << "Reaction plane: " << opt.rpmethod << endl;
            opt.usedPhi = atoi(argv[i+2]);
            cout << "dPhi is going to be presented on the plots: " << opt.usedPhi << endl;
            break;
          case 'd':
            opt.dirPre = argv[i+1];
            cout << "Prefix for all result files: " << opt.dirPre << endl;
            break;
          case 'u':
            opt.prefitMass = true;
            cout << "Turn On: signal(=data, depends on muon pair type) mass pre-fitting" << endl;
            opt.prefitBkg = true;
            cout << "Turn On: Background(=sideband data) ctau distribution pre-fitting" << endl;
            opt.prefitSignalCTau = true;
            cout << "Turn On: prompt signal ctau distribution pre-fitting" << endl;

            if (atoi(argv[i+1]) == 0) {
              opt.oneGaussianResol = false;
              cout << "         2 Gaussians will be used for resolution function" << endl;
              opt.fixResol2MC = false;
              cout << "         Resolution function will be fixed at FINAL fitting step" << endl;
            } else if (atoi(argv[i+1]) == 1) {
              opt.oneGaussianResol = true;
              cout << "         1 Gaussian will be used for resolution function" << endl;
              opt.fixResol2MC = false;
              cout << "         Resolution function will be fixed at FINAL fitting step" << endl;
            } else if (atoi(argv[i+1]) == 2) {
              opt.oneGaussianResol = false;
              cout << "         2 Gaussians will be used for resolution function" << endl;
              opt.fixResol2MC = true;
              cout << "         Resolution function will be fixed to MC fitting results" << endl;
            } else if (atoi(argv[i+1]) == 3) {
              opt.oneGaussianResol = true;
              cout << "         1 Gaussian will be used for resolution function" << endl;
              opt.fixResol2MC = true;
              cout << "         Resolution function will be fixed to MC fitting results" << endl;
            }
            break;
          case 'p':
            opt.prange = argv[i+1];
            cout << "pT range: " << opt.prange << " GeV/c" << endl;
            break;
          case 'y':
            opt.yrange = argv[i+1];
            cout << "Rapidity range: " << opt.yrange << " rad" << endl;
            break;
          case 'j':
						opt.isMultiplicity = atoi(argv[i+1]);
						cout << "multiplicity shown on plot (0:cent, 1:Ntrk, 2:ET) : " << opt.isMultiplicity << endl;
            break;
					case 't':
            opt.crange = argv[i+1];
            cout << "Centrality range: " << opt.crange << " %" << endl;
            break;
          case 'h':
						opt.etrange = argv[i+1];
						cout << "E_{T}^{HF} range : " << opt.etrange << "GeV" << endl;
            break;
					case 'n':
						opt.ntrrange = argv[i+1];
						cout << "Ntracks range : " << opt.ntrrange << endl;
            break;
					case 'l':
            opt.lrange = argv[i+1];
            cout << "l(J/psi) range: " << opt.lrange << " mm" << endl; 
            break;
          case 'e':
            opt.errrange = argv[i+1];
            cout << "Range for sigma_l(J/psi) is " << opt.errrange << " mm" << endl;
            break;    
          case 's':
            opt.phirange = argv[i+1];
            cout << "dPhi(J/psi) range: " << opt.phirange << " rad" << endl;
            break;
          case 'a':
            if (atoi(argv[i+1]) == 0) {
              opt.analyticBlifetime = false;
              opt.doBfit = true;
              cout << "Turn Off: RooHistPdf from MC template of J/psi Ctau lifetime will be used" << endl;
            } else if (atoi(argv[i+1]) == 1) {
              opt.analyticBlifetime = true;
              opt.doBfit = true;
              cout << "Turn On: Analytical MC J/psi Ctau lifetime PDF will be used" << endl;
            } else if (atoi(argv[i+1]) == 2) {
              opt.analyticBlifetime = false;
              opt.doBfit = false;
              cout << "Turn Off: Only inclusive fitting will be performed" << endl;
            }
            if (atoi(argv[i+2]) == 0) {
              opt.ctauBackground = 0;
              cout << "1 Lifetime background function is fitted over all sideband events" << endl;
            } else if (atoi(argv[i+2]) == 1) {
              opt.ctauBackground = 1;
              cout << "2 Lifetime background functions are fitted over left/right sideband events" << endl;
              cout << "Final fit on signal mass region only" << endl;
            } else if (atoi(argv[i+2]) == 2) {
              opt.ctauBackground = 2;
              cout << "2 Lifetime background functions are fitted over left/right sideband events" << endl;
              cout << "Step function is used for lifetime background functions" << endl;
            }
            break;
          case 'x':
            opt.isMB = atoi(argv[i+1]);
            cout << "Inclusive fit option: " << opt.isMB << endl;
						opt.ctErrRange = atoi(argv[i+2]);
						cout << "CTau error range option: " << opt.ctErrRange << endl;
						cout << " 0 read the range from files " <<  endl;
						cout << " 1 get the range in this macro" <<  endl;
						cout << " 2 set the range const" <<  endl;
						opt.ctErrFile = argv[i+3];
						cout << "CTau error range input file: " << opt.ctErrFile << endl;
            break;
          case 'b':
            opt.isPbPb = atoi(argv[i+1]);
            cout << "PbPb/pp/pA fit option: " << opt.isPbPb << endl;
            opt.isPEE = atoi(argv[i+2]);
            cout << "PEE/notPEE fit option: " << opt.isPEE << endl;
            opt.is2Widths = atoi(argv[i+3]);
            cout << "Mass fit option(Shared 1 width/independent 2 widths): " << opt.is2Widths << endl;
            break;
          case 'z':
            if (0 == atoi(argv[i+1])) {
              opt.fracfix = true;
              cout << "Fix frac2,3 BEFORE ctau bkg fitting" << endl;
            } else if (1 == atoi(argv[i+1])) {
              opt.fracfix = false;
              cout << "Fix frac2,3 AFTER ctau bkg fitting" << endl;
            }
            break;
        }
      }
    }
  }// End check options

} 

void getOptRange(string &ran, float *min, float *max) {
  if (sscanf(ran.c_str(), "%f-%f", min, max) == 0) {
    cout << ran.c_str() << ": not valid!" << endl;
    assert(0);
  }
  return ;
}

void formTitle(InputOpt &opt, float cmin, float cmax) {
  if (opt.isPbPb == 1) {
    // Use for pbpb data set
    sprintf(opt.cmspre,"CMS Preliminary");
    sprintf(opt.beamEn,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
    sprintf(opt.lumi,"L_{int} = 150 #mub^{-1}");
    double intMin, intMax;
    double fracMin = modf(cmin,&intMin);
    double fracMax = modf(cmax,&intMax);
    if (fracMin==0 && fracMax==0) sprintf(opt.centString,"Cent. %.0f-%.0f%%",cmin,cmax);
    else if (fracMin!=0 && fracMax==0) sprintf(opt.centString,"Cent. %.1f-%.0f%%",cmin,cmax);
    else if (fracMin==0 && fracMax!=0) sprintf(opt.centString,"Cent. %.0f-%.1f%%",cmin,cmax);
    else sprintf(opt.centString,"Cent. %.1f-%.1f%%",cmin,cmax);

  } else if (opt.isPbPb == 0) {
    // Use for pp data set
    sprintf(opt.cmspre,"CMS Preliminary");
    sprintf(opt.beamEn,"pp  #sqrt{s} = 2.76 TeV");
    sprintf(opt.lumi,"L_{int} = 5.3 pb^{-1}");

  } else if (opt.isPbPb == 2) {
    // Use for Pbp data set
    sprintf(opt.cmspre,"CMS Preliminary");
    sprintf(opt.beamEn,"pPb  #sqrt{s_{NN}} = 5 TeV");
//    sprintf(opt.lumi,"L_{int} = 18 nb^{-1}");
    sprintf(opt.lumi,"L_{int} = 21 nb^{-1}");
    sprintf(opt.centString,"Cent. %.0f-%.0f%%",cmin,cmax);

  } else if (opt.isPbPb == 3) {
		// Use for pPb data set
		sprintf(opt.cmspre,"CMS Preliminary");
		sprintf(opt.beamEn,"pPb  #sqrt{s_{NN}} = 5 TeV");
//		sprintf(opt.lumi,"L_{int} = 12 nb^{-1}");
		sprintf(opt.lumi,"L_{int} = 14 nb^{-1}");
		sprintf(opt.centString,"Cent. %.0f-%.0f%%",cmin,cmax);

	}
	// is pbpb? is pp? is pA?
}

void formRapidity(InputOpt &opt, float ymin, float ymax) {
  double inteMin, inteMax;
  double fracMin = modf(ymin,&inteMin);
  double fracMax = modf(ymax,&inteMax);

  if (!opt.rpmethod.compare("etHFm")) {
    if (fracMin==0 && fracMax!=0) sprintf(opt.rapString,"%.0f < y_{lab} < %.2f",ymin,ymax);
    else if (fracMin!=0 && fracMax==0) sprintf(opt.rapString,"%.2f < y_{lab} < %.0f",ymin,ymax);
    else if (fracMin==0 && fracMax==0) sprintf(opt.rapString,"%.0f < y_{lab} < %.0f",ymin,ymax);
    else sprintf(opt.rapString,"%.2f < y_{lab} < %.2f",ymin,ymax);
  } else if (!opt.rpmethod.compare("etHFp")) {
    if (fracMin!=0 && fracMax==0) sprintf(opt.rapString,"%.0f < y_{lab} < %.2f",-ymax,-ymin);
    else if (fracMin==0 && fracMax!=0) sprintf(opt.rapString,"%.2f < y_{lab} < %.0f",-ymax,-ymin);
    else if (fracMin==0 && fracMax==0) sprintf(opt.rapString,"%.0f < y_{lab} < %.0f",-ymax,-ymin);
    else sprintf(opt.rapString,"%0.2f < y_{lab} < %.2f",-ymax,-ymin);
  } else {
    if (ymin==0 && fracMax!=0) sprintf(opt.rapString,"|y| < %.2f",ymax);
    else if (fracMin==0 && fracMax!=0) sprintf(opt.rapString,"%.0f < |y_{lab}| < %.2f",ymin,ymax);
    else if (fracMin==0 && fracMax==0) sprintf(opt.rapString,"%.0f < |y_{lab}| < %.0f",ymin,ymax);
    else if (fracMin!=0 && fracMax==0) sprintf(opt.rapString,"%.2f < |y_{lab}| < %.0f",ymin,ymax);
    else sprintf(opt.rapString,"%.2f < |y_{lab}| < %.2f",ymin,ymax);
  }
}

void formPt(InputOpt &opt, float pmin, float pmax) {
  double pminD, pmaxD, pminF, pmaxF;
  pminF = modf(pmin,&pminD);
  pmaxF = modf(pmax,&pmaxD);

  if (pmin == 0) { 
    if (pmaxF == 0) sprintf(opt.ptString,"p_{T} < %.0f GeV/c",pmax);
    else  sprintf(opt.ptString,"p_{T} < %.1f GeV/c",pmax);
  } else {
    if (pminF == 0 && pmaxF == 0) sprintf(opt.ptString,"%.0f < p_{T} < %.0f GeV/c",pmin,pmax);
    else if (pminF == 0 && !pmaxF == 0) sprintf(opt.ptString,"%.0f < p_{T} < %.1f GeV/c",pmin,pmax);
    else if (!pminF == 0 && pmaxF == 0) sprintf(opt.ptString,"%.1f < p_{T} < %.0f GeV/c",pmin,pmax);
    else sprintf(opt.ptString,"%.1f < p_{T} < %.1f GeV/c",pmin,pmax);
  }
}

void formPhi(InputOpt &opt, float psmin, float psmax) {
  string psmin_latex, psmax_latex;
  bool isStr = true;

  if (psmin == 0) psmin_latex="0";
  else if (psmin == 0.393) psmin_latex="#frac{#pi}{8}";
  else if (psmin == 0.785) psmin_latex="#frac{#pi}{4}";
  else if (psmin == 1.178) psmin_latex="#frac{3#pi}{8}";
  else if (psmin == 1.571) psmin_latex="#frac{#pi}{2}";
  else if (psmin == -3.142) psmin_latex="-#pi";
  else if (psmin == 3.142) psmin_latex="#pi";
  else isStr = false;

  if (psmax == 0) psmax_latex="0";
  else if (psmax == 0.393) psmax_latex="#frac{#pi}{8}";
  else if (psmax == 0.785) psmax_latex="#frac{#pi}{4}";
  else if (psmax == 1.178) psmax_latex="#frac{3#pi}{8}";
  else if (psmax == 1.571) psmax_latex="#frac{#pi}{2}";
  else if (psmax == -3.142) psmax_latex="-#pi";
  else if (psmax == 3.142) psmax_latex="#pi";
  else isStr = false;

	cout << "formPhi: " << psmin << " " << psmax << endl;
	cout << "formPhi: " << psmin_latex << " " << psmax_latex << endl;
	if (psmin==0)
		cout << "formPhi: " << psmin << " is psmin" << endl;
	if (psmax==1.571)
		cout << "formPhi: " << psmax << " is psmax" << endl;

  if (opt.usedPhi) {
    if (isStr)
      sprintf(opt.dphiString,"%s < |#phi^{J/#psi}-#Psi_{RP}| < %s",psmin_latex.c_str(),psmax_latex.c_str());
    else
      sprintf(opt.dphiString,"%.3f < |#phi^{J/#psi}-#Psi_{RP}| < %.3f",psmin,psmax);
  } else {
    sprintf(opt.dphiString,"");
  }
}

void formEt(InputOpt &opt, float etmin, float etmax) {
  double etminD, etmaxD, etminF, etmaxF;
  etminF = modf(etmin,&etminD);
  etmaxF = modf(etmax,&etmaxD);

  if (etmin == 0) { 
    if (etmaxF == 0) sprintf(opt.etString,"#Sigma E_{T}^{HF |#eta>4|} < %.0f GeV",etmax);
    else  sprintf(opt.etString,"#Sigma E_{T}^{HF |#eta>4|} < %.1f GeV",etmax);
  } else {
    if (etminF == 0 && etmaxF == 0) sprintf(opt.etString,"%.0f < #Sigma E_{T}^{HF |#eta>4|} < %.0f GeV",etmin,etmax);
    else if (etminF == 0 && !etmaxF == 0) sprintf(opt.etString,"%.0f < #Sigma E_{T}^{HF |#eta>4|} < %.1f GeV",etmin,etmax);
    else if (!etminF == 0 && etmaxF == 0) sprintf(opt.etString,"%.1f < #Sigma E_{T}^{HF |#eta>4|} < %.0f GeV",etmin,etmax);
    else sprintf(opt.etString,"%.1f < #Sigma E_{T}^{HF |#eta>4|} < %.1f GeV",etmin,etmax);
  }
}

void formNtrk(InputOpt &opt, float ntrmin, float ntrmax) {
  double ntrminD, ntrmaxD, ntrminF, ntrmaxF;
  ntrminF = modf(ntrmin,&ntrminD);
  ntrmaxF = modf(ntrmax,&ntrmaxD);

  if (ntrmin == 0) { 
    if (ntrmaxF == 0) sprintf(opt.ntrkString,"N_{tracks}^{|#eta|<2.4} < %.0f ",ntrmax);
    else  sprintf(opt.ntrkString,"N_{tracks}^{|#eta|<2.4} < %.1f ",ntrmax);
  } else {
    if (ntrminF == 0 && ntrmaxF == 0) sprintf(opt.ntrkString,"%.0f < N_{tracks}^{|#eta|<2.4} < %.0f ",ntrmin,ntrmax);
    else if (ntrminF == 0 && !ntrmaxF == 0) sprintf(opt.ntrkString,"%.0f < N_{tracks}^{|#eta|<2.4} < %.1f ",ntrmin,ntrmax);
    else if (!ntrminF == 0 && ntrmaxF == 0) sprintf(opt.ntrkString,"%.1f < N_{tracks}^{|#eta|<2.4} < %.0f ",ntrmin,ntrmax);
    else sprintf(opt.ntrkString,"%.1f < N_{tracks}^{|#eta|<2.4} < %.1f ",ntrmin,ntrmax);
  }
}


int readCtauErrRange(InputOpt &opt, float *errmin, float *errmax) {
	ifstream errinput;
	errinput.open(opt.ctErrFile.c_str(),ifstream::in);
	if (!errinput.good()) {
		cout << "readCtauErrRange:: CANNOT read ctau error list file. Exit." << endl;
		return -1;
	}
	string headers;
	getline(errinput, headers); // prefix
	getline(errinput, headers); // column names

	string rap, pt, cent, dphi, ntrk, et, emin, emax;
	while (!errinput.eof()) {
		errinput >> rap >> pt >> cent >> dphi >> ntrk >> et >> emin >> emax;
		if (!rap.compare(opt.yrange) && !pt.compare(opt.prange) && !cent.compare(opt.crange) && !dphi.compare(opt.phirange) && !ntrk.compare(opt.ntrrange) && !et.compare(opt.etrange)) {
			*errmin = atof(emin.c_str());
			*errmax = atof(emax.c_str());
			return 0;
		} else continue;
	}

	cout << "readCtauErrRange:: CANNOT read ctau error range from a file. Exit." << endl;
	return -2;
}


void getCtauErrRange(RooDataSet *data, InputOpt &opt, const char *reduceDSOrig, float lmin, float lmax, float *errmin, float *errmax) {
  RooWorkspace *ws = new RooWorkspace("ctauerrorcheckWS");
  RooDataSet *redDataCut = (RooDataSet*)data->reduce(reduceDSOrig);
  ws->import(*redDataCut);
  
  ws->var("Jpsi_Mass")->setRange(2.6,3.5);
  ws->var("Jpsi_Mass")->setBins(45);
  ws->var("Jpsi_Ct")->setRange(-lmin,lmax);
  ws->var("Jpsi_CtErr")->setRange(0.0,0.992);
  ws->var("Jpsi_CtErr")->setBins(124);

  RooDataHist *binData = new RooDataHist("binData","binData",RooArgSet( *(ws->var("Jpsi_Mass")),*(ws->var("Jpsi_Ct")),*(ws->var("Jpsi_CtErr")) ), *redDataCut);
  RooDataHist *binDataCtErr = new RooDataHist("binDataCtErr","binDataCtErr",RooArgSet(*(ws->var("Jpsi_CtErr"))),*redDataCut);
  cout << "getCtauErrRange:: DATA :: N events to fit: " << binData->sumEntries() << endl;

  RooDataSet *redDataSB = (RooDataSet*) redDataCut->reduce("Jpsi_Mass < 2.9 || Jpsi_Mass > 3.3");
  RooDataHist *binDataSB = new RooDataHist("binDataSB","Data distribution for background",RooArgSet( *(ws->var("Jpsi_Mass")),*(ws->var("Jpsi_Ct")) ),*redDataSB);
  RooDataSet *redDataSIG = (RooDataSet*)redDataCut->reduce("Jpsi_Mass > 2.9 && Jpsi_Mass < 3.3");

  RooDataHist *binDataCtErrSB = new RooDataHist("binDataCtErrSB","Data ct error distribution for bkg",RooArgSet(*(ws->var("Jpsi_CtErr"))),*redDataSB);
  RooDataHist *binDataCtErrSIG = new RooDataHist("binDataCtErrSIG","Data ct error distribution for sig",RooArgSet(*(ws->var("Jpsi_CtErr"))),*redDataSIG);
  RooHistPdf errPdfBkg("errPdfBkg","Error PDF bkg",RooArgSet(*(ws->var("Jpsi_CtErr"))),*binDataCtErrSB);  ws->import(errPdfBkg);

  // J/psi mass parameterization
  defineMassBkg(ws);
  defineMassSig(ws, opt);

  // Binning for invariant mass distribution
  RooBinning rbm(2.6,3.5);
  rbm.addUniform(45,2.6,3.5);
  
  struct PARAM {
    double coeffGaus; double coeffGausErr;
    double meanSig1;  double meanSig1Err;
    double sigmaSig1; double sigmaSig1Err;
    double sigmaSig2; double sigmaSig2Err;
    double sigmaSig3; double sigmaSig3Err;
    double alpha;     double alphaErr;
    double enne;      double enneErr;
    double enneW;     double enneWErr;
  };
  double cutValue_merged;

  double inputN[2] = {0};  //Number of yield/background in the 0-1.571 rad bin
  string inputFN, inputFNcb;
  bool centest = true;  //False: fit w/o any fixed parameters (centrality dep.)
  bool dPhitest = true;  //False: fit w/o any fixed parameters (dPhi dep.)
  if (!opt.crange.compare("0.0-100.0")) { centest = false; }
  else {
    centest = true;
    inputFNcb = opt.dirPre+ "_rap" + opt.yrange + "_pT" + opt.prange + "_cent0.0-100.0_dPhi0.000-1.571.txt";
  }
  if (!opt.phirange.compare("0.000-1.571")) { dPhitest = false; }
  else {
    dPhitest = true;
    inputFN = opt.dirPre+ "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi0.000-1.571.txt";
  }

  if (centest) {
    ifstream input(inputFNcb.c_str());
    if (!input.good()) { cout << "getCtauErrRange:: Failed to open: " <<inputFNcb << endl; return ; }
    string tmp;
    double inputNS[2] = {0};
    double inputTmp[2] = {0};
    PARAM inputP;
    input >> tmp >> inputNS[0] >> inputNS[1]; //NSig
    input >> tmp >> inputTmp[0] >> inputTmp[1]; //NBkg
    while (input.good()) {   //Mass signal parameters
      input >> tmp >> inputTmp[0] >> inputTmp[1];
      cout << "getCtauErrRange::" << tmp << " " << inputTmp[0] << endl;
      if (!tmp.compare("coeffGaus")) inputP.coeffGaus = inputTmp[0];
      else if (!tmp.compare("meanSig1")) inputP.meanSig1 = inputTmp[0];
      else if (!tmp.compare("sigmaSig1")) inputP.sigmaSig1 = inputTmp[0];
      else if (!tmp.compare("sigmaSig2")) inputP.sigmaSig2 = inputTmp[0];
      else if (!tmp.compare("alpha")) inputP.alpha = inputTmp[0];
      else if (!tmp.compare("enneW")) inputP.enneW = inputTmp[0];
      else if (!tmp.compare("cutValue")) cutValue_merged = inputTmp[0];
    }

    ws->var("sigmaSig1")->setVal(inputP.sigmaSig1);
    ws->var("coeffGaus")->setVal(inputP.coeffGaus);
    ws->var("meanSig1")->setVal(inputP.meanSig1);
    ws->var("alpha")->setVal(inputP.alpha);
    ws->var("enneW")->setVal(inputP.enneW);

    ws->var("sigmaSig1")->setConstant(kTRUE);
    ws->var("coeffGaus")->setConstant(kTRUE);
    ws->var("meanSig1")->setConstant(kTRUE);
    ws->var("alpha")->setConstant(kTRUE);
    ws->var("enneW")->setConstant(kTRUE);

    input.close();
  }

  if (dPhitest) {
    ifstream input(inputFN.c_str());
    if (!input.good()) { cout << "getCtauErrRange:: Failed to open: " <<  inputFN << endl; return ; }
    string tmp;
    PARAM inputP;
    double inputNS[2] = {0};
    double inputTmp[2]={0};
    input >> tmp >> inputNS[0] >> inputNS[1]; //NSig
    input >> tmp >> inputTmp[0] >> inputTmp[1]; //NBkg
    while (input.good()) {   //Mass signal parameters
      input >> tmp >> inputTmp[0] >> inputTmp[1];
      cout << "getCtauErrRange:: " << tmp << " " << inputTmp[0] << endl;
      if (!tmp.compare("coeffGaus")) inputP.coeffGaus = inputTmp[0];
      else if (!tmp.compare("meanSig1")) inputP.meanSig1 = inputTmp[0];
      else if (!tmp.compare("sigmaSig1")) inputP.sigmaSig1 = inputTmp[0];
      else if (!tmp.compare("sigmaSig2")) inputP.sigmaSig2 = inputTmp[0];
      else if (!tmp.compare("alpha")) inputP.alpha = inputTmp[0];
      else if (!tmp.compare("enneW")) inputP.enneW = inputTmp[0];
      else if (!tmp.compare("cutValue")) cutValue_merged = inputTmp[0];
    }

    ws->var("sigmaSig2")->setVal(inputP.sigmaSig2);
    ws->var("sigmaSig1")->setVal(inputP.sigmaSig1);
    ws->var("coeffGaus")->setVal(inputP.coeffGaus);
    ws->var("meanSig1")->setVal(inputP.meanSig1);
    ws->var("alpha")->setVal(inputP.alpha);
    ws->var("enneW")->setVal(inputP.enneW);

    ws->var("sigmaSig2")->setConstant(kTRUE);
    ws->var("sigmaSig1")->setConstant(kTRUE);
    ws->var("coeffGaus")->setConstant(kTRUE);
    ws->var("meanSig1")->setConstant(kTRUE);
    ws->var("alpha")->setConstant(kTRUE);
    ws->var("enneW")->setConstant(kTRUE);

    input.close();
  }

  char funct[100];
  double initBkg = redDataSB->sumEntries()*9.0/5.0;
  double initSig = redDataCut->sumEntries() - initBkg;
  sprintf(funct,"SUM::sigMassPDF(NSig[%f,1.0,50000.0]*sigCB2WNG1,NBkg[%f,1.0,500000.0]*expFunct)",initSig,initBkg);
  ws->factory(funct);
  ws->pdf("sigMassPDF")->fitTo(*redDataCut,Extended(1),Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8));

  // ** Test Ct error distribution on the sideband region
  RooPlot *errframe2 = ws->var("Jpsi_CtErr")->frame();
  binDataCtErrSB->plotOn(errframe2,DataError(RooAbsData::SumW2));
  ws->pdf("errPdfBkg")->plotOn(errframe2,LineColor(kBlue),Normalization(binDataCtErrSB->sumEntries(),RooAbsReal::NumEvent));

  // *** scaleF is defined to scale down ct err dist in 2.9-3.3 GeV/c2 signal region
  float bc = ws->var("coefExp")->getVal();
  float scaleF = (exp(2.9*bc)-exp(3.3*bc))/(exp(2.6*bc)-exp(2.9*bc)+exp(3.3*bc)-exp(3.5*bc));
  RooDataHist* subtrData = new RooDataHist("subtrData","Subtracted data",RooArgSet(*(ws->var("Jpsi_CtErr")))); 
  RooDataHist* scaledBkg = subtractSidebands(ws,subtrData,binDataCtErrSIG,binDataCtErrSB,scaleF,"Jpsi_CtErr");
  subtrData->SetName("subtrData");
  RooHistPdf errPdfSig("errPdfSig","Error PDF signal",RooArgSet(*(ws->var("Jpsi_CtErr"))),*subtrData);  ws->import(errPdfSig);

  // ** Check scaled ct err dist curves and data points in signal region are matched
  errframe2 = ws->var("Jpsi_CtErr")->frame();
  subtrData->plotOn(errframe2,DataError(RooAbsData::SumW2));
  ws->pdf("errPdfSig")->plotOn(errframe2,LineColor(kBlue),Normalization(subtrData->sumEntries(),RooAbsReal::NumEvent));
  binDataCtErrSIG->plotOn(errframe2,DataError(RooAbsData::SumW2),LineColor(kRed),MarkerColor(kRed));  //Not subtracted D_sig
  
  // ** Check the minimum and maximum of the ctau error in signal and background regions
  TH1* hSig = subtrData->createHistogram("hSig",*ws->var("Jpsi_CtErr"));
  TH1* hBkg = scaledBkg->createHistogram("hBkg",*ws->var("Jpsi_CtErr"));
  
  double minSig = 0.5, maxSig = 0.0, minBkg = 0.5, maxBkg = 0.0;
  double cutValue = 0.2;

  int maxBinSig = hSig->GetMaximumBin();
  int maxBinBkg = hBkg->GetMaximumBin();
  
  minSig = hSig->GetBinLowEdge(maxBinSig);
  minBkg = hBkg->GetBinLowEdge(maxBinBkg);
  // pick up lower bound next to other non-zero bins
  for (int xbins = maxBinSig; xbins > 0; xbins--) {
    if (hSig->GetBinContent(xbins) > cutValue) {
      minSig = hSig->GetBinLowEdge(xbins);
//          cout << "getCtauErrRange:: SIG binContent: " << hSig->GetBinContent(xbins) << endl;
//          cout << "getCtauErrRange:: SIG low edge: " << hSig->GetBinLowEdge(xbins) << endl;
    } else break;
  }
  for (int xbins = maxBinBkg; xbins > 0; xbins--) {
    if (hBkg->GetBinContent(xbins) > cutValue) {
      minBkg = hBkg->GetBinLowEdge(xbins);
//          cout << "getCtauErrRange:: BKG binContent: " << hBkg->GetBinContent(xbins) << endl;
//          cout << "getCtauErrRange:: BKG low edge: " << hBkg->GetBinLowEdge(xbins) << endl;
    } else break;
  }

  // pick up upper bound next to other non-zero bins
  maxSig = hSig->GetBinLowEdge(maxBinSig+1);
  maxBkg = hBkg->GetBinLowEdge(maxBinBkg+1);
  for (int xbins = maxBinSig; xbins < hSig->GetNbinsX() ; xbins++) {
    if (hSig->GetBinContent(xbins) > cutValue) {
      maxSig = hSig->GetBinLowEdge(xbins+1);
//          cout << "getCtauErrRange:: SIG binContent: " << hSig->GetBinContent(xbins) << endl;
//          cout << "getCtauErrRange:: SIG upper edge: " << hSig->GetBinLowEdge(xbins+1) << endl;
    } else break;
  }
  for (int xbins = maxBinSig; xbins < hBkg->GetNbinsX() ; xbins++) {
    if (hBkg->GetBinContent(xbins) > cutValue) {
      maxBkg = hBkg->GetBinLowEdge(xbins+1);
//          cout << "getCtauErrRange:: BKG binContent: " << hBkg->GetBinContent(xbins) << endl;
//          cout << "getCtauErrRange:: BKG upper edge: " << hBkg->GetBinLowEdge(xbins+1) << endl;
    } else break;
  }

  // choose the higher lower limit, lower upper limit
  double tmpMin = 0, tmpMax = 0;
  if (minSig > minBkg) tmpMin = minSig; else tmpMin = minBkg;
  if (maxSig < maxBkg) tmpMax = maxSig; else tmpMax = maxBkg;

  // round off lower limit -> allow more entries on the lower limits
  tmpMin = TMath::Floor(tmpMin*1000);
  tmpMin = tmpMin/(double)1000.0;

  // round up upper limit -> allow more entries on the upper limits
  tmpMax = TMath::Ceil(tmpMax*1000);
  tmpMax = tmpMax/(double)1000.0;

  char reduceDS[512];
  sprintf(reduceDS,"Jpsi_CtErr > %.3f && Jpsi_CtErr < %.3f",tmpMin,tmpMax);
  RooDataSet *redDataTmp = (RooDataSet*)redDataCut->reduce(reduceDS);
  if (redDataTmp->sumEntries() < redDataCut->sumEntries()*0.9) { // if ctau error range cuts off >10% events
    delete redDataTmp;
    sprintf(reduceDS,"Jpsi_CtErr > %.3f && Jpsi_CtErr < %.3f",minSig,maxSig);
    redDataTmp = (RooDataSet*)redDataCut->reduce(reduceDS);
    tmpMin = minSig; tmpMax = maxSig; 
  }

  if ((tmpMax - tmpMin) < 0.008) {
      cout << "getCtauErrRange:: Maximum is less than minimum! Possibly there are few events in this bin.\n";
      tmpMax = tmpMin + 0.008;
  }

  // *** Final ctau error plot
  TCanvas c0("ctau_err","ctau_err",500,500);
  c0.Draw(); c0.cd(); c0.SetLogy(1); 
  errframe2 = ws->var("Jpsi_CtErr")->frame();
  binDataCtErrSIG->plotOn(errframe2,DataError(RooAbsData::SumW2),MarkerColor(kRed),LineColor(kRed));
  binDataCtErrSB->plotOn(errframe2,DataError(RooAbsData::SumW2),MarkerColor(kGreen+2),LineColor(kGreen+2),MarkerStyle(24));
  scaledBkg->plotOn(errframe2,DataError(RooAbsData::SumW2),MarkerColor(kBlue),MarkerStyle(24),LineColor(kBlue));
  subtrData->plotOn(errframe2,DataError(RooAbsData::SumW2));
  errframe2->Draw();

  TLatex *t = new TLatex();
  t->SetNDC(); t->SetTextAlign(12);

  t->SetTextSize(0.05);
  t->DrawLatex(0.50,0.90,opt.cmspre);
  t->DrawLatex(0.50,0.82,opt.beamEn);
  t->SetTextSize(0.04);
  t->DrawLatex(0.50,0.75,opt.lumi);
  t->SetTextSize(0.035);
  t->DrawLatex(0.50,0.69,opt.rapString);
  t->DrawLatex(0.50,0.64,opt.ptString);
  if (opt.isPbPb != 0) {
		if (opt.isMultiplicity ==0) t->DrawLatex(0.50,0.60,opt.centString);
		else if (opt.isMultiplicity ==1) t->DrawLatex(0.50,0.60,opt.ntrkString);
		else if (opt.isMultiplicity ==2) t->DrawLatex(0.50,0.60,opt.etString);
	}
  t->DrawLatex(0.50,0.55,opt.dphiString);
  t->SetTextSize(0.04);
  char comment[200];
  sprintf(comment,"Range: %.3f-%.3f mm",tmpMin,tmpMax);
  t->DrawLatex(0.50,0.46,comment);
  string titlestr = opt.dirPre+ "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_testErrPdfSigDetail_Log.pdf";
  c0.SaveAs(titlestr.c_str());

  *errmin = tmpMin; *errmax = tmpMax;
  cout << "getCtauErrRange:: " << reduceDSOrig << " " << lmin << " " << lmax << " " << *errmin << " " << *errmax << endl;
  
  delete ws;
  delete redDataCut;
  delete redDataTmp;
  delete binData;
  delete binDataCtErr;
  delete binDataSB;
  delete binDataCtErrSB;
  delete binDataCtErrSIG;
  delete subtrData;
  delete scaledBkg;
  delete t;
}


void setWSRange(RooWorkspace *ws, float lmin, float lmax, float errmin, float errmax){
  float minRangeForPF = -4*errmax;
  if (minRangeForPF < -lmin) minRangeForPF = -lmin;

  ws->var("Jpsi_CtTrue")->setRange(-0.1,lmax);
  ws->var("Jpsi_Ct")->setRange("promptfit",minRangeForPF,4*errmax);
  ws->var("Jpsi_Ct")->setRange(-lmin,lmax);
  ws->var("Jpsi_CtErr")->setRange(errmin,errmax);

//  ws->cat("Jpsi_Type")->setRange("glbglb","GG");
  ws->cat("MCType")->setRange("prompt","PR");
  ws->cat("MCType")->setRange("nonprompt","NP");

  return;
}

RooBinning setCtBinning(float lmin,float lmax) {
  RooBinning rb2(-lmin,lmax);
  
  if (lmax+lmin>4.9) {
    rb2.addBoundary(-1.5);
    rb2.addBoundary(-1.0);
    rb2.addBoundary(-0.8);
    rb2.addBoundary(-0.6);
    rb2.addBoundary(-0.5);
    rb2.addUniform(6,-0.5,-0.2);
    //rb2.addUniform(18,-0.2,0.2);
    //rb2.addUniform(9,0.2,0.5);
    rb2.addUniform(12,-0.2,0.1);
    rb2.addUniform(8,0.1,0.5);
    rb2.addUniform(5,0.5,1.0);
    rb2.addUniform(15,1.0,lmax);
  } else if (lmax+lmin > 4.4) {
    rb2.addBoundary(-1.5);
    rb2.addBoundary(-1.0);
    rb2.addBoundary(-0.8);
    rb2.addBoundary(-0.6);
    rb2.addBoundary(-0.5);
    rb2.addUniform(9,-0.5,-0.2);
    //rb2.addUniform(36,-0.2,0.2);
    //rb2.addUniform(12,0.2,0.5);
    rb2.addUniform(20,-0.2,0.1);
    rb2.addUniform(11,0.1,0.5);
    rb2.addUniform(5,0.5,1.0);
    rb2.addUniform(5,1.0,lmax);
  } else {
    rb2.addBoundary(-lmin);
    rb2.addBoundary(-0.7);
    rb2.addBoundary(-0.6);
    rb2.addBoundary(-0.5);
    rb2.addUniform(9,-0.5,-0.2);
    //rb2.addUniform(40,-0.2,0.2);
    //rb2.addUniform(12,0.2,0.5);
    rb2.addUniform(28,-0.2,0.1);
    rb2.addUniform(11,0.1,0.5);
    rb2.addUniform(15,0.5,1.2);
    rb2.addUniform(8,1.2,lmax);
    
    // finer binning on Jpsi_Ct >= 0.5 mm, default for HIN-12-014
/*    rb2.addBoundary(-1.0);
    rb2.addBoundary(-0.7);
    rb2.addBoundary(-0.6);
    rb2.addBoundary(-0.5);
    rb2.addUniform(9,-0.5,-0.2);
    rb2.addUniform(40,-0.2,0.2);
    rb2.addUniform(12,0.2,0.5);
    rb2.addUniform(15,0.5,1.2);
    rb2.addUniform(6,1.2,lmax);
*/
    // Same on JpsiRaa50100ppreco/notAnalyticBfits/fit2DData_pbpb.cpp
/*    rb2.addUniform(2,-1.0,-0.5);
    rb2.addUniform(4,-0.5,-0.2);
    rb2.addUniform(8,-0.2,0.2);
    rb2.addUniform(4,0.2,0.6);
    rb2.addUniform(4,0.6,1.0);
    rb2.addUniform(1,1.0,2.0);
*/

/*  original binning (from last year)
    rb2.addBoundary(-1.0);
    rb2.addBoundary(-0.7);
    rb2.addBoundary(-0.6);
    rb2.addBoundary(-0.5);
    rb2.addUniform(9,-0.5,-0.2);
    rb2.addUniform(40,-0.2,0.2);
    rb2.addUniform(12,0.2,0.5);
    rb2.addUniform(10,0.5,1.0);
    rb2.addUniform(4,1.0,lmax);*/

/*  from roberto's code on cvs (J/psi only)
    if (lmin < 1.0) rb2.addBoundary(-1.0);
    if (lmin < 0.7) rb2.addBoundary(-0.7);
    if (lmin < 0.6) rb2.addBoundary(-0.6);
    if (lmin < 0.5) rb2.addBoundary(-0.5);
    if (lmin < 0.5) lmin = 0.5;
    rb2.addUniform(6,-lmin,-0.2);
    rb2.addUniform(50,-0.2,0.2);
    rb2.addUniform(18,0.2,0.5);
    rb2.addUniform(21,0.5,1.2);
    rb2.addUniform(6,1.2,lmax);*/
  }
  return rb2;
}

void ctauErrCutCheck(RooWorkspace *ws, RooDataSet *redData, RooDataSet *redData_2, RooDataSet *redMC, RooDataSet *redMC_2, RooDataSet *redMC2, RooDataSet *redMC2_2, InputOpt &opt) {
  TCanvas c0;
  c0.cd();
  
  RooPlot *massFrame = ws->var("Jpsi_Mass")->frame();
  redMC_2->plotOn(massFrame,DataError(RooAbsData::SumW2));
  redMC->plotOn(massFrame,MarkerColor(kRed),DataError(RooAbsData::SumW2));
  massFrame->Draw();

  char reduceDS[512];
  string titlestr;
  TLatex *t = new TLatex();  t->SetNDC();  t->SetTextAlign(12);
  
  sprintf(reduceDS,"with cut: %.0f",redData_2->sumEntries());
  t->DrawLatex(0.55,0.85,reduceDS);
  sprintf(reduceDS,"without cut: %.0f",redData->sumEntries());
  t->DrawLatex(0.55,0.8,reduceDS);
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_data_mass.pdf";
  c0.SaveAs(titlestr.c_str());
  c0.Clear(); delete massFrame;

  c0.cd();
  RooPlot *ctauFrame = ws->var("Jpsi_Ct")->frame();
  redMC_2->plotOn(ctauFrame,DataError(RooAbsData::SumW2));
  redMC->plotOn(ctauFrame,MarkerColor(kRed),DataError(RooAbsData::SumW2));
  ctauFrame->Draw();
  sprintf(reduceDS,"with cut: %.0f",redMC_2->sumEntries());
  t->DrawLatex(0.55,0.85,reduceDS);
  sprintf(reduceDS,"without cut: %.0f",redMC->sumEntries());
  t->DrawLatex(0.55,0.8,reduceDS);
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_np_ctau_Lin.pdf";
  c0.SaveAs(titlestr.c_str());
  c0.SetLogy(1);
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_np_ctau_Log.pdf";
  c0.SaveAs(titlestr.c_str());
  c0.Clear(); delete ctauFrame;
  
  ctauFrame = ws->var("Jpsi_Ct")->frame();
  redMC2_2->plotOn(ctauFrame,DataError(RooAbsData::SumW2));
  redMC2->plotOn(ctauFrame,MarkerColor(kRed),DataError(RooAbsData::SumW2));
  ctauFrame->Draw();
  c0.SetLogy(0);
  sprintf(reduceDS,"with cut: %.0f",redMC2_2->sumEntries());
  t->DrawLatex(0.55,0.85,reduceDS);
  sprintf(reduceDS,"without cut: %.0f",redMC2->sumEntries());
  t->DrawLatex(0.55,0.8,reduceDS);
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_pr_ctau_Lin.pdf";
  c0.SaveAs(titlestr.c_str());
  c0.SetLogy(1);
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_pr_ctau_Log.pdf";
  c0.SaveAs(titlestr.c_str());
  c0.Clear(); delete ctauFrame;

  ctauFrame = ws->var("Jpsi_Ct")->frame();
  redData_2->plotOn(ctauFrame,DataError(RooAbsData::SumW2));
  redData->plotOn(ctauFrame,MarkerColor(kRed),DataError(RooAbsData::SumW2));
  ctauFrame->Draw();
  c0.SetLogy(0);
  sprintf(reduceDS,"with cut: %.0f",redData_2->sumEntries());
  t->DrawLatex(0.55,0.85,reduceDS);
  sprintf(reduceDS,"without cut: %.0f",redData->sumEntries());
  t->DrawLatex(0.55,0.8,reduceDS);
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_data_ctau_Lin.pdf";
  c0.SaveAs(titlestr.c_str());
  c0.SetLogy(1);
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_data_ctau_Log.pdf";
  c0.SaveAs(titlestr.c_str());
}

void sidebandLeftRightCheck(RooWorkspace *ws, RooDataSet *redDataSBL, RooDataSet *redDataSBR, InputOpt &opt) {
  TH1 *binDataSBL = redDataSBL->createHistogram("sidebandL",*(ws->var("Jpsi_Ct")));
  TH1 *binDataSBR = redDataSBR->createHistogram("sidebandR",*(ws->var("Jpsi_Ct")));
  
  TCanvas c0; c0.SetLogy(0);
  TLatex *t = new TLatex();  t->SetNDC();  t->SetTextAlign(12);
  string titlestr;
  
  binDataSBL->GetXaxis()->CenterTitle(1);
  binDataSBL->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
  binDataSBL->SetMarkerColor(kBlack);
  binDataSBR->SetMarkerColor(kRed);
  binDataSBR->SetLineColor(kRed);
  binDataSBR->Scale(binDataSBL->GetMaximum()/binDataSBR->GetMaximum());
  double originalmax = binDataSBL->GetMaximum();
  binDataSBL->SetMaximum(originalmax*1.9);
  binDataSBL->Draw("p");
  binDataSBR->Draw("p,same");

  t->SetTextSize(0.05);
  t->DrawLatex(0.18,0.90,opt.cmspre);
  t->DrawLatex(0.18,0.82,opt.beamEn);
  t->SetTextSize(0.04);
  t->DrawLatex(0.18,0.75,opt.lumi);
  t->SetTextSize(0.035);
  t->DrawLatex(0.18,0.69,opt.rapString);
  t->DrawLatex(0.18,0.64,opt.ptString);
  if (opt.isPbPb != 0) {
		if (opt.isMultiplicity ==0) t->DrawLatex(0.18,0.60,opt.centString);
		else if (opt.isMultiplicity ==1) t->DrawLatex(0.18,0.60,opt.ntrkString);
		else if (opt.isMultiplicity ==2) t->DrawLatex(0.18,0.60,opt.etString);
	}
  t->DrawLatex(0.18,0.55,opt.dphiString);
  t->SetTextSize(0.04);

  TLegend legsb(0.6,0.6,0.9,0.7,NULL,"brNDC");
  legsb.SetFillStyle(0); legsb.SetBorderSize(0); legsb.SetShadowColor(0); legsb.SetMargin(0.2);
  legsb.AddEntry(binDataSBL,"2.6 < m_{#mu#mu} < 2.9","p");
  legsb.AddEntry(binDataSBR,"3.3 < m_{#mu#mu} < 3.5","p");
  legsb.Draw("same");

  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_testCtauSB_Lin.pdf";
//  c0.SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_testCtauSB_Lin.root";
  //c0.SaveAs(titlestr.c_str());
  c0.SetLogy(1);
  binDataSBL->SetMaximum(originalmax*65);
  c0.Update();
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_testCtauSB_Log.pdf";
//  c0.SaveAs(titlestr.c_str()); 
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_testCtauSB_Log.root";
  //c0.SaveAs(titlestr.c_str()); 
}

void drawMassPlotsWithoutB(RooWorkspace *ws, RooDataSet* redDataCut, RooFitResult *fitM, InputOpt &opt) {

  RooBinning rb(ws->var("Jpsi_Mass")->getBinning().numBins(), ws->var("Jpsi_Mass")->getBinning().array());

  RooPlot *mframe_wob = ws->var("Jpsi_Mass")->frame();
  redDataCut->plotOn(mframe_wob,DataError(RooAbsData::SumW2),XErrorSize(0),MarkerSize(1),Binning(rb));
  double avgBinWidth = rb.averageBinWidth();
  mframe_wob->GetYaxis()->SetTitle(Form("Counts / %.2f (GeV/c^{2})",avgBinWidth));
  mframe_wob->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
  mframe_wob->GetXaxis()->CenterTitle(1);
  const double max = mframe_wob->GetMaximum() * 1.3;
  mframe_wob->SetMaximum(max);
  mframe_wob->SetMinimum(0);
  ws->pdf("sigMassPDF")->plotOn(mframe_wob,DrawOption("F"),FillColor(kBlack),FillStyle(3354),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));

  ws->pdf("sigMassPDF")->plotOn(mframe_wob,Components(opt.mBkgFunct.c_str()),DrawOption("F"),FillColor(kAzure-9),FillStyle(1001),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
  
  ws->pdf("sigMassPDF")->plotOn(mframe_wob,Components(opt.mBkgFunct.c_str()),LineColor(kBlue),LineStyle(7),LineWidth(5),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
  ws->pdf("sigMassPDF")->plotOn(mframe_wob,LineColor(kBlack),LineWidth(2),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
  redDataCut->plotOn(mframe_wob,DataError(RooAbsData::SumW2),XErrorSize(0),MarkerSize(1),Binning(rb));

  TH1 *hdata = redDataCut->createHistogram("hdata",*ws->var("Jpsi_Mass"),Binning(rb));
  // *** Calculate chi2/nDof for mass fitting
  int nBins = hdata->GetNbinsX();
  RooHist *hpullm; hpullm = mframe_wob->pullHist(); hpullm->SetName("hpullM");
  double Chi2 = 0;
  int nFullBinsPull = 0;
  double *ypull = hpullm->GetY();
  for (unsigned int i=0; i < nBins; i++) {
    if (hdata->GetBinContent(i+1) == 0) continue;
    nFullBinsPull++;
    Chi2 = Chi2 + pow(ypull[i],2);
  }
  double UnNormChi2 = Chi2;
  int nFitParam = fitM->floatParsFinal().getSize();
  int Dof = nFullBinsPull - nFitParam;
  Chi2 /= (nFullBinsPull - nFitParam);

  // *** Check in narrower signal region NSig
/*    ws->var("Jpsi_Mass")->setRange("sigpeak",2.9,3.3);
  RooAbsReal *inteAll = ws->pdf(mSigFunct.c_str())->createIntegral(RooArgSet(*ws->var("Jpsi_Mass")),NormSet(RooArgSet(*ws->var("Jpsi_Mass"))));
  RooAbsReal *inteSig = ws->pdf(mSigFunct.c_str())->createIntegral(RooArgSet(*ws->var("Jpsi_Mass")),Range("sigpeak"),NormSet(RooArgSet(*ws->var("Jpsi_Mass"))),Range("sigpeak"));*/


  // *** mass plot without pull
  TCanvas cmwop;  cmwop.Draw();  mframe_wob->Draw();
  TLatex *t = new TLatex();  t->SetNDC();  t->SetTextAlign(12);
  char reduceDS[512];
  string titlestr;
  
  t->SetTextSize(0.05);
  t->DrawLatex(0.18,0.90,opt.cmspre);
  t->DrawLatex(0.18,0.82,opt.beamEn);
  t->SetTextSize(0.04);
  t->DrawLatex(0.18,0.75,opt.lumi);
  t->SetTextSize(0.035);
  t->DrawLatex(0.18,0.69,opt.rapString);
  t->DrawLatex(0.18,0.64,opt.ptString);
  if (opt.isPbPb != 0) {
//		if (opt.isMultiplicity ==0) t->DrawLatex(0.18,0.60,opt.centString);
		if (opt.isMultiplicity ==0) t->DrawLatex(0.18,0.58,opt.centString);
		else if (opt.isMultiplicity ==1) t->DrawLatex(0.18,0.58,opt.ntrkString);
		else if (opt.isMultiplicity ==2) t->DrawLatex(0.18,0.58,opt.etString);
  }
	t->DrawLatex(0.18,0.55,opt.dphiString);
  t->SetTextSize(0.035);
  sprintf(reduceDS,"#chi^{2}/dof = %0.1f/%d",UnNormChi2,Dof);
  t->DrawLatex(0.66,0.85,reduceDS);
  sprintf(reduceDS,"N_{J/#psi}: %0.0f #pm %0.0f",ws->var("NSig")->getVal(),ws->var("NSig")->getError());
  t->DrawLatex(0.66,0.80,reduceDS);
  sprintf(reduceDS,"#sigma = %0.0f #pm %0.0f MeV/c^{2}", opt.PcombinedWidth, opt.PcombinedWidthErr);
  t->DrawLatex(0.66,0.76,reduceDS);

  TLegend leg1(0.65,0.6,0.92,0.73,NULL,"brNDC");
  leg1.SetFillStyle(0); leg1.SetBorderSize(0); leg1.SetShadowColor(0); leg1.SetMargin(0.2);
  leg1.AddEntry(gfake1,"data","p");
  leg1.AddEntry(&hfake21,"total fit","lf");
  leg1.AddEntry(&hfake11,"background","lf");
  leg1.Draw("same");

  titlestr = opt.dirPre + "_rap" + opt.yrange  + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_massfit_wob_wopull.pdf";
  cmwop.SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange  + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_massfit_wob_wopull.root";
  //cmwop.SaveAs(titlestr.c_str());

  // *** mass plot with pull
  TCanvas cm("cm","The mass Canvas",200,10,600,880);
  cm.cd();
  TPad *padm1 = new TPad("padm1","This is pad1",0.05,0.35,0.95,0.97);
  padm1->SetBottomMargin(0.15);
  padm1->Draw();
  TPad *padm2 = new TPad("padm2","This is pad2",0.05,0.03,0.95,0.35);
  padm2->SetTopMargin(0.08);
  padm2->SetBottomMargin(0.24);
  padm2->Draw();

  padm1->cd();  mframe_wob->Draw();
  t->SetTextSize(0.05);
  t->DrawLatex(0.18,0.90,opt.cmspre);
  t->DrawLatex(0.18,0.82,opt.beamEn);
  t->SetTextSize(0.04);
  t->DrawLatex(0.18,0.75,opt.lumi);
  t->SetTextSize(0.035);
  t->DrawLatex(0.18,0.69,opt.rapString);
  t->DrawLatex(0.18,0.64,opt.ptString);
  if (opt.isPbPb != 0) {
//		if (opt.isMultiplicity ==0) t->DrawLatex(0.18,0.60,opt.centString);
		if (opt.isMultiplicity ==0) t->DrawLatex(0.18,0.58,opt.centString);
		else if (opt.isMultiplicity ==1) t->DrawLatex(0.18,0.58,opt.ntrkString);
		else if (opt.isMultiplicity ==2) t->DrawLatex(0.18,0.58,opt.etString);
  }
	t->DrawLatex(0.18,0.55,opt.dphiString);
  t->SetTextSize(0.035);
  sprintf(reduceDS,"N_{J/#psi}: %0.0f #pm %0.0f",ws->var("NSig")->getVal(),ws->var("NSig")->getError());
  t->DrawLatex(0.66,0.80,reduceDS);
  sprintf(reduceDS,"#sigma = %0.0f #pm %0.0f MeV/c^{2}", opt.PcombinedWidth, opt.PcombinedWidthErr);
  t->DrawLatex(0.66,0.76,reduceDS);

  leg1.Draw("same");
  cm.Update();

  RooPlot* mframepull =  ws->var("Jpsi_Mass")->frame(Title("Pull")) ;
  mframepull->GetYaxis()->SetTitle("Pull");
  mframepull->SetLabelSize(0.08,"XYZ");
  mframepull->SetTitleSize(0.1,"XYZ");
  mframepull->SetTitleOffset(0.55,"Y");
  mframepull->addPlotable(hpullm,"PX");
  double mframemax = 0;
  if (mframepull->GetMinimum()*-1 > mframepull->GetMaximum()) mframemax = mframepull->GetMinimum()*-1;
  else mframemax = mframepull->GetMaximum();
  mframepull->SetMaximum(mframemax); 
  mframepull->SetMinimum(-1*mframemax); 
  mframepull->GetXaxis()->SetTitle("m_{#mu#mu} (MeV/c^{2})");
  mframepull->GetXaxis()->CenterTitle(1);

  padm2->cd(); mframepull->Draw();
  t->SetTextSize(0.07);
  sprintf(reduceDS,"#chi^{2}/dof = %.1f/%d",UnNormChi2,Dof);
  t->DrawLatex(0.6,0.83,reduceDS);
  cm.Update();

  titlestr = opt.dirPre + "_rap" + opt.yrange  + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_massfit_wob.pdf";
  cm.SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange  + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_massfit_wob.root";
  //cm.SaveAs(titlestr.c_str());

  delete padm1;
  delete padm2;
  delete hpullm;
}

void ctauErrDistCheck(RooWorkspace *ws, RooDataHist *binDataCtErrSB, RooDataHist *binDataCtErrSIG, RooDataHist *subtrData, RooDataHist *weightedBkg, InputOpt &opt) {
  RooPlot *errframe = ws->var("Jpsi_CtErr")->frame();
  binDataCtErrSB->plotOn(errframe,DataError(RooAbsData::SumW2));
  ws->pdf("errPdfBkgWeighted")->plotOn(errframe,LineColor(kBlue),Normalization(binDataCtErrSB->sumEntries(),RooAbsReal::NumEvent));

/*  c0.Clear(); c0.cd(); c0.SetLogy(0); errframe->Draw();
  titlestr = dirPre + "_rap" + yrange + "_pT" + prange + "_cent" + crange  + "_dPhi" + phirange + "_testErrPdfBkg_Lin.pdf";
  c0.SaveAs(titlestr.c_str());*/
  TCanvas c0;
  string titlestr;
  c0.Clear(); c0.SetLogy(1); errframe->Draw();
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_testErrPdfBkg_Log.pdf";
  c0.SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_testErrPdfBkg_Log.root";
  //c0.SaveAs(titlestr.c_str());
  delete errframe;

  errframe = ws->var("Jpsi_CtErr")->frame();
  subtrData->plotOn(errframe,DataError(RooAbsData::SumW2));
  ws->pdf("errPdfSig")->plotOn(errframe,LineColor(kBlue),Normalization(subtrData->sumEntries(),RooAbsReal::NumEvent));
  binDataCtErrSIG->plotOn(errframe,DataError(RooAbsData::SumW2),LineColor(kRed),MarkerColor(kRed));  //Not subtracted D_sig
   
/*   c0.cd(); c0.Clear(); c0.SetLogy(1); errframe->Draw();
  titlestr = dirPre + "_rap" + yrange + "_pT" + prange + "_cent" + crange + "_dPhi" + phirange + "_testErrPdfSig_Lin.pdf";
  c0.SaveAs(titlestr.c_str());*/
  c0.Clear(); c0.SetLogy(1); errframe->Draw();
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_testErrPdfSig_Log.pdf";
  c0.SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_testErrPdfSig_Log.root";
  //c0.SaveAs(titlestr.c_str());
  delete errframe;

/*    errframe = ws->var("Jpsi_CtErr")->frame();
  binDataCtErrSIG->plotOn(errframe,DataError(RooAbsData::SumW2),MarkerColor(kRed),LineColor(kRed));
  binDataCtErrSB->plotOn(errframe,DataError(RooAbsData::SumW2),MarkerColor(kGreen+2),LineColor(kGreen+2),MarkerStyle(24));
  scaledBkg->plotOn(errframe,DataError(RooAbsData::SumW2),MarkerColor(kBlue),MarkerStyle(24),LineColor(kBlue));
  subtrData->plotOn(errframe,DataError(RooAbsData::SumW2));

  c0.cd(); c0.Clear(); c0.SetLogy(1); errframe->Draw();
  titlestr = dirPre + "_rap" + yrange + "_pT" + prange + "_cent" + crange + "_dPhi" + phirange + "_testErrPdfSigDetail_Log.pdf";
  c0.SaveAs(titlestr.c_str());*/
}

void ctauResolFitCheck(RooWorkspace *ws, bool fitMC, RooPlot *tframePR, InputOpt &opt) {
  TLatex *t = new TLatex();  t->SetNDC();  t->SetTextAlign(12);
  char reduceDS[512];
  string titlestr;

  t->SetTextSize(0.04);
  TCanvas c00; c00.cd(); tframePR->Draw();
  sprintf(reduceDS,"#sigma(G_{N}): %.2f",ws->var("sigmaResSigN")->getVal());
  t->DrawLatex(0.5,0.55,reduceDS);
  double fracGn;
  if ((!opt.oneGaussianResol && opt.isPEE == 1) || opt.isPEE == 0) {
    sprintf(reduceDS,"#sigma(G_{W}): %.2f",ws->var("sigmaResSigW")->getVal());
    t->DrawLatex(0.5,0.5,reduceDS);
    if (opt.isPEE == 1) {
      sprintf(reduceDS,"frac(G_{W}): %.2f",ws->var("fracRes")->getVal());
    } else if (opt.isPEE == 0) {
      fracGn= (1-(ws->var("fracRes")->getVal()+ws->var("fracRes2")->getVal()+ws->var("fracRes3")->getVal()));
    }
    sprintf(reduceDS,"frac(G_{N}): %.2f",fracGn);
    t->DrawLatex(0.5,0.45,reduceDS);
  }
  if (fitMC)
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_resolInit_Lin.pdf";
  else
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_resolFit_Lin.pdf";
  c00.SaveAs(titlestr.c_str());
  if (fitMC)
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_resolInit_Lin.root";
  else
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_resolFit_Lin.root";
  //c00.SaveAs(titlestr.c_str());
  
  c00.Clear(); c00.SetLogy(1); tframePR->Draw();
  sprintf(reduceDS,"#sigma(G_{N}): %.2f",ws->var("sigmaResSigN")->getVal());
  t->DrawLatex(0.5,0.55,reduceDS);
  if ((!opt.oneGaussianResol && opt.isPEE == 1) || opt.isPEE == 0) {
    sprintf(reduceDS,"#sigma(G_{W}): %.2f",ws->var("sigmaResSigW")->getVal());
    t->DrawLatex(0.5,0.5,reduceDS);
    if (opt.isPEE == 1) {
      sprintf(reduceDS,"frac(G_{W}): %.2f",ws->var("fracRes")->getVal());
    } else if (opt.isPEE == 0) {
      double fracGn = (1-(ws->var("fracRes")->getVal()+ws->var("fracRes2")->getVal()+ws->var("fracRes3")->getVal()));
    }
    t->DrawLatex(0.5,0.45,reduceDS);
  }
  if (fitMC)
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_resolInit_Log.pdf";
  else
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_resolFit_Log.pdf";
  c00.SaveAs(titlestr.c_str());
  if (fitMC)
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_resolInit_Log.root";
  else
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_resolFit_Log.root";
  //c00.SaveAs(titlestr.c_str());

  // Get sum of squared residual to fisher's F-test
/*    RooHist *hresid;
    hresid = tframePR->residHist();
    hresid->SetName("residual");
    double *yresid = hresid->GetY();
    unsigned int nBins = ws->var("Jpsi_Ct")->getBinning().numBins();
    for (unsigned int i=0; i < nBins; i++) {
      RSS = RSS + pow(yresid[i],2);
      if (fabs(yresid[i]) > 0.0001) nFullBinsResid++;
    }

    RooPlot* tframeres =  ws->var("Jpsi_Ct")->frame(Title("Residual Distribution")) ;
    tframeres->SetLabelSize(0.08,"XYZ");
    tframeres->SetTitleSize(0.1,"XYZ");
    tframeres->SetTitleOffset(0.55,"Y");
    tframeres->addPlotable(hresid,"P") ;
    tframeres->SetMaximum(-(tframeres->GetMinimum())); 
    tframeres->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi}/#font[12]{#sigma({l}_{J/#psi})} (mm)");
    tframeres->GetXaxis()->CenterTitle(1);

    c00.cd();
    TPad padpr1("padpr1","This is pad1",0.05,0.35,0.95,0.97);
    padpr1.SetBottomMargin(0);
    padpr1.Draw();
    TPad padpr2("padpr2","This is pad2",0.05,0.03,0.95,0.35);
    padpr2.SetTopMargin(0);
    padpr2.SetBottomMargin(0.24);
    padpr2.Draw();

    padpr1.cd(); tframePR->Draw();
    padpr2.cd();
    tframeres->SetMinimum(0.5); 
    tframeres->Draw();

    TLatex tpr;
    tpr.SetNDC(); tpr.SetTextAlign(22);
    tpr.SetTextSize(0.07);
    sprintf(reduceDS,"RSS = %.0f, nFullBinsResid = %d",RSS,nFullBinsResid);
    tpr.DrawLatex(0.7,0.90,reduceDS);

    titlestr = dirPre + "_rap" + yrange + "_pT" + prange + "_cent" + crange + "_dPhi" + phirange + "_2D_" + partFile + "resolfit_Lin.pdf";
    c00.SaveAs(titlestr.c_str());
    
    padpr1.cd(); padpr1.SetLogy(1); tframePR->Draw();
    padpr2.cd(); padpr2.SetLogy(1); tframeres->Draw();
    tpr.DrawLatex(0.7,0.90,reduceDS);
    titlestr = dirPre + "_rap" + yrange + "_pT" + prange + "_cent" + crange + "_dPhi" + phirange + "_2D_" + partFile + "resolfit_Log.pdf";
    c00.SaveAs(titlestr.c_str());
*/
}

void drawCtauSBPlots(RooWorkspace *ws, RooDataSet *redDataSB, RooDataSet *redDataSBL, RooDataSet *redDataSBR, RooDataHist *binDataCtErrSB, RooFitResult *fitSB, RooFitResult *fitSBL, RooFitResult *fitSBR, InputOpt &opt) {
  TLatex *t = new TLatex();  t->SetNDC();  t->SetTextAlign(12);
  char reduceDS[512];
  string titlestr;
  double unNormChi2;
  int dof;

  RooBinning rb(ws->var("Jpsi_Ct")->getBinning().numBins(), ws->var("Jpsi_Ct")->getBinning().array());
  
  TLegend leg11(0.5,0.48,0.9,0.58,NULL,"brNDC");
  leg11.SetFillStyle(0); leg11.SetBorderSize(0); leg11.SetShadowColor(0);
  leg11.SetMargin(0.2);
  leg11.AddEntry(gfake1,"sideband data","p");
  leg11.AddEntry(&hfake11,"background","l");

  if (opt.ctauBackground == 0) {
    RooPlot *tframe1 = ws->var("Jpsi_Ct")->frame();
    double avgBinWidth = rb.averageBinWidth();
    tframe1->GetYaxis()->SetTitle(Form("Counts / %.2f (mm)",avgBinWidth));
    redDataSB->plotOn(tframe1,DataError(RooAbsData::SumW2));
    if (opt.isPEE == 1) {
      ws->pdf("bkgCtauTOT_PEE")->plotOn(tframe1,ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErrSB,kTRUE),NumCPU(8),Normalization(redDataSB->sumEntries(),RooAbsReal::NumEvent),LineStyle(7));
    } else if (opt.isPEE == 0) {
      ws->pdf("bkgCtTot")->plotOn(tframe1,Normalization(redDataSB->sumEntries(),RooAbsReal::NumEvent));
    }
    
    TCanvas *c3 = new TCanvas("c3","The Canvas",200,10,600,880);
    c3->cd();
    TPad *pad1 = new TPad("pad1","This is pad1",0.05,0.35,0.95,0.97);
    pad1->SetBottomMargin(0);  pad1->Draw();
    TPad *pad2 = new TPad("pad2","This is pad2",0.05,0.03,0.95,0.35);
    pad2->SetTopMargin(0);  pad2->SetBottomMargin(0.24);  pad2->Draw();

    pad1->cd(); tframe1->Draw();
    t->SetTextSize(0.05);
    t->DrawLatex(0.5,0.90,opt.cmspre);
    t->DrawLatex(0.5,0.83,opt.beamEn);
    t->SetTextSize(0.04);
    t->DrawLatex(0.5,0.76,opt.lumi);
    t->SetTextSize(0.030);
    t->DrawLatex(0.5,0.70,opt.rapString);
    t->DrawLatex(0.5,0.66,opt.ptString);
    if (opt.isPbPb != 0) {
			if (opt.isMultiplicity ==0) t->DrawLatex(0.5,0.63,opt.centString);
			else if (opt.isMultiplicity ==1) t->DrawLatex(0.5,0.63,opt.ntrkString);
			else if (opt.isMultiplicity ==2) t->DrawLatex(0.5,0.63,opt.etString);
    }
		t->DrawLatex(0.5,0.58,opt.dphiString);

    leg11.Draw("same");

    TH1 *hdatasb = redDataSB->createHistogram("hdatasb",*ws->var("Jpsi_Ct"),Binning(rb));
    RooHist *hpullsb = tframe1->pullHist(); hpullsb->SetName("hpullSB");
    int nFitPar = fitSB->floatParsFinal().getSize();
    double chi2 = 0;
    double *ypullssb = hpullsb->GetY();
    unsigned int nBins = ws->var("Jpsi_Ct")->getBinning().numBins();
    unsigned int nFullBins = 0;
    for (unsigned int i = 0; i < nBins; i++) {
      if (hdatasb->GetBinContent(i+1) == 0) continue;
      chi2 += ypullssb[i]*ypullssb[i];
      nFullBins++;
    }
    unNormChi2 = chi2;
    dof = nFullBins - nFitPar;
    chi2 /= (nFullBins - nFitPar);
    int nDOF = ws->var("Jpsi_Ct")->getBinning().numBins() - nFitPar;

    RooPlot* tframepull =  ws->var("Jpsi_Ct")->frame(Title("Pull")) ;
    tframepull->GetYaxis()->SetTitle("Pull");
    tframepull->SetLabelSize(0.08,"XYZ");
    tframepull->SetTitleSize(0.1,"XYZ");
    tframepull->SetTitleOffset(0.55,"Y");
    tframepull->addPlotable(hpullsb,"PX") ;
    double tframemax = 0;
    if (tframepull->GetMinimum()*-1 > tframepull->GetMaximum()) tframemax = tframepull->GetMinimum()*-1;
    else tframemax = tframepull->GetMaximum();
    tframepull->SetMaximum(tframemax); 
    tframepull->SetMinimum(-1*tframemax); 
    tframepull->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
    tframepull->GetXaxis()->CenterTitle(1);

    pad2->cd(); tframepull->Draw();

    TLatex *t2 = new TLatex();
    t2->SetNDC(); t2->SetTextAlign(22); t2->SetTextSize(0.07);
    sprintf(reduceDS,"#chi^{2}/dof = %.2f/%d",unNormChi2,dof);
    t2->DrawLatex(0.76,0.90,reduceDS);

    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timeside_Lin.pdf";
    c3->SaveAs(titlestr.c_str());
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timeside_Lin.root";
    //c3->SaveAs(titlestr.c_str());

    TCanvas* c3a = new TCanvas("c3a","The Canvas",200,10,600,880);
    c3a->cd();
    TPad *pad1a = new TPad("pad1a","This is pad1",0.05,0.35,0.95,0.97);
    pad1a->SetBottomMargin(0);
    pad1a->Draw();
    TPad *pad2a = new TPad("pad2a","This is pad2",0.05,0.03,0.95,0.35);
    pad2a->SetTopMargin(0);
    pad2a->SetBottomMargin(0.24);
    pad2a->Draw();

    pad1a->cd(); pad1a->SetLogy(1);
    tframe1->SetMaximum(tframe1->GetMaximum()*9); 
    tframe1->SetMinimum(0.5); 
    tframe1->Draw();
   
    t->SetTextSize(0.05);
    t->DrawLatex(0.18,0.90,opt.cmspre);
    t->DrawLatex(0.18,0.85,opt.beamEn);
    t->SetTextSize(0.04);
    t->DrawLatex(0.18,0.79,opt.lumi);
    t->SetTextSize(0.035);
    t->DrawLatex(0.62,0.80,opt.rapString);
    t->DrawLatex(0.62,0.75,opt.ptString);
    if (opt.isPbPb != 0) {
			if (opt.isMultiplicity ==0) t->DrawLatex(0.62,0.71,opt.centString);
			else if (opt.isMultiplicity ==1) t->DrawLatex(0.62,0.71,opt.ntrkString);
			else if (opt.isMultiplicity ==2) t->DrawLatex(0.62,0.71,opt.etString);
    }
		t->DrawLatex(0.62,0.66,opt.dphiString);

    leg11.SetX1NDC(0.62);
    leg11.SetY1NDC(0.55);
    leg11.SetX2NDC(0.92);
    leg11.SetY2NDC(0.65);
    leg11.Draw("same");

    pad2a->cd(); tframepull->Draw();
    sprintf(reduceDS,"#chi^{2}/dof = %.2f/%d",unNormChi2,dof);
    t2->DrawLatex(0.76,0.90,reduceDS);

    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timeside_Log.pdf";
    c3a->SaveAs(titlestr.c_str());
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timeside_Log.root";
    //c3a->SaveAs(titlestr.c_str());

    delete pad1;
    delete pad2;
    delete c3;
    delete pad1a;
    delete pad2a;
    delete c3a;
  
  } else if (opt.ctauBackground == 1 || opt.ctauBackground == 2) {
    // for left side
    RooPlot *tframe1 = ws->var("Jpsi_Ct")->frame();
    double avgBinWidth = rb.averageBinWidth();
    tframe1->GetYaxis()->SetTitle(Form("Counts / %.2f (mm)",avgBinWidth));
    redDataSBL->plotOn(tframe1,DataError(RooAbsData::SumW2),Binning(rb));
    ws->pdf("bkgCtauTOTL_PEE")->plotOn(tframe1,ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErrSB,kTRUE),NumCPU(8),Normalization(redDataSBL->sumEntries(),RooAbsReal::NumEvent),LineStyle(7));

    TCanvas *c3 = new TCanvas("c3","The Canvas",200,10,600,880);
    c3->cd();
    TPad *pad1 = new TPad("pad1","This is pad1",0.05,0.35,0.95,0.97);
    pad1->SetBottomMargin(0);  pad1->Draw();
    TPad *pad2 = new TPad("pad2","This is pad2",0.05,0.03,0.95,0.35);
    pad2->SetTopMargin(0);  pad2->SetBottomMargin(0.24);  pad2->Draw();

    pad1->cd(); tframe1->Draw();
    t->SetTextSize(0.05);
    t->DrawLatex(0.5,0.90,opt.cmspre);
    t->DrawLatex(0.5,0.83,opt.beamEn);
    t->SetTextSize(0.04);
    t->DrawLatex(0.5,0.76,opt.lumi);
    t->SetTextSize(0.030);
    t->DrawLatex(0.5,0.70,opt.rapString);
    t->DrawLatex(0.5,0.66,opt.ptString);
    if (opt.isPbPb != 0) {
			if (opt.isMultiplicity ==0) t->DrawLatex(0.5,0.62,opt.centString);
			else if (opt.isMultiplicity ==1) t->DrawLatex(0.5,0.62,opt.ntrkString);
			else if (opt.isMultiplicity ==2) t->DrawLatex(0.5,0.62,opt.etString);
    }
		t->DrawLatex(0.5,0.57,opt.dphiString);
    t->DrawLatex(0.5,0.53,"2.6 < m_{#mu#mu}< 2.9 GeV/c^{2}");

    TLegend leg11(0.62,0.39,0.92,0.49,NULL,"brNDC");
    leg11.SetFillStyle(0); leg11.SetBorderSize(0); leg11.SetShadowColor(0);
    leg11.SetMargin(0.2);
    leg11.AddEntry(gfake1,"sideband data","p");
    leg11.AddEntry(&hfake11,"background","l");
    leg11.Draw("same");

    TH1 *hdatasb = redDataSBL->createHistogram("hdatasb",*ws->var("Jpsi_Ct"),Binning(rb));
    RooHist *hpullsb = tframe1->pullHist(); hpullsb->SetName("hpullSB");
    int nFitPar = fitSBL->floatParsFinal().getSize();
    double chi2 = 0;
    double *ypullssb = hpullsb->GetY();
    unsigned int nBins = ws->var("Jpsi_Ct")->getBinning().numBins();
    unsigned int nFullBins = 0;
    for (unsigned int i = 0; i < nBins; i++) {
      if (hdatasb->GetBinContent(i+1) == 0) continue;
      chi2 += ypullssb[i]*ypullssb[i];
      nFullBins++;
    }
    unNormChi2 = chi2;
    dof = nFullBins - nFitPar;
    chi2 /= (nFullBins - nFitPar);
    int nDOF = ws->var("Jpsi_Ct")->getBinning().numBins() - nFitPar;

    RooPlot* tframepull = ws->var("Jpsi_Ct")->frame(Title("Pull")) ;
    tframepull->GetYaxis()->SetTitle("Pull");
    tframepull->SetLabelSize(0.08,"XYZ");
    tframepull->SetTitleSize(0.1,"XYZ");
    tframepull->SetTitleOffset(0.55,"Y");
    tframepull->addPlotable(hpullsb,"PX") ;
    double tframemax = 0;
    if (tframepull->GetMinimum()*-1 > tframepull->GetMaximum()) tframemax = tframepull->GetMinimum()*-1;
    else tframemax = tframepull->GetMaximum();
    tframepull->SetMaximum(tframemax); 
    tframepull->SetMinimum(-1*tframemax); 
    tframepull->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
    tframepull->GetXaxis()->CenterTitle(1);

    pad2->cd(); tframepull->Draw();

    TLatex *t2 = new TLatex();
    t2->SetNDC(); t2->SetTextAlign(22); t2->SetTextSize(0.07);
    sprintf(reduceDS,"#chi^{2}/dof = %.2f/%d",unNormChi2,dof);
    t2->DrawLatex(0.76,0.90,reduceDS);

    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timesideL_Lin.pdf";
    c3->SaveAs(titlestr.c_str());
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timesideL_Lin.root";
    //c3->SaveAs(titlestr.c_str());

    TCanvas* c3a = new TCanvas("c3a","The Canvas",200,10,600,880);
    c3a->cd();
    TPad *pad1a = new TPad("pad1a","This is pad1",0.05,0.35,0.95,0.97);
    pad1a->SetBottomMargin(0);
    pad1a->Draw();
    TPad *pad2a = new TPad("pad2a","This is pad2",0.05,0.03,0.95,0.35);
    pad2a->SetTopMargin(0);
    pad2a->SetBottomMargin(0.24);
    pad2a->Draw();

    pad1a->cd(); pad1a->SetLogy(1);
    tframe1->SetMaximum(tframe1->GetMaximum()*9); 
    tframe1->SetMinimum(0.5); 
    tframe1->Draw();
   
    t->SetTextSize(0.05);
    t->DrawLatex(0.18,0.90,opt.cmspre);
    t->DrawLatex(0.18,0.85,opt.beamEn);
    t->SetTextSize(0.04);
    t->DrawLatex(0.18,0.79,opt.lumi);
    t->SetTextSize(0.035);
    t->DrawLatex(0.62,0.80,opt.rapString);
    t->DrawLatex(0.62,0.75,opt.ptString);
    if (opt.isPbPb != 0) {
			if (opt.isMultiplicity ==0) t->DrawLatex(0.62,0.70,opt.centString);
			else if (opt.isMultiplicity ==1) t->DrawLatex(0.62,0.70,opt.ntrkString);
			else if (opt.isMultiplicity ==2) t->DrawLatex(0.62,0.70,opt.etString);
    }
		t->DrawLatex(0.62,0.65,opt.dphiString);
    t->DrawLatex(0.62,0.60,"2.6 < m_{#mu#mu}< 2.9 GeV/c^{2}");

    leg11.SetX1NDC(0.62);
    leg11.SetY1NDC(0.45);
    leg11.SetX2NDC(0.92);
    leg11.SetY2NDC(0.55);
    leg11.Draw("same");

    pad2a->cd(); tframepull->Draw();
    sprintf(reduceDS,"#chi^{2}/dof = %.2f/%d",unNormChi2,dof);
    t2->DrawLatex(0.76,0.90,reduceDS);

    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timesideL_Log.pdf";
    c3a->SaveAs(titlestr.c_str());
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timesideL_Log.root";
    //c3a->SaveAs(titlestr.c_str());

    delete pad1;
    delete pad2;
    delete c3;
    delete pad1a;
    delete pad2a;
    delete c3a;
    delete hdatasb;
    delete hpullsb;
    delete tframe1;

    // right sideband
    tframe1 = ws->var("Jpsi_Ct")->frame();
    redDataSBR->plotOn(tframe1,DataError(RooAbsData::SumW2),Binning(rb));
    ws->pdf("bkgCtauTOTR_PEE")->plotOn(tframe1,ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErrSB,kTRUE),NumCPU(8),Normalization(redDataSBR->sumEntries(),RooAbsReal::NumEvent),LineStyle(7));

    c3 = new TCanvas("c3","The Canvas",200,10,600,880);
    c3->cd();
    pad1 = new TPad("pad1","This is pad1",0.05,0.35,0.95,0.97);
    pad1->SetBottomMargin(0);  pad1->Draw();
    pad2 = new TPad("pad2","This is pad2",0.05,0.03,0.95,0.35);
    pad2->SetTopMargin(0);  pad2->SetBottomMargin(0.24);  pad2->Draw();

    pad1->cd(); tframe1->Draw();
    t->SetTextSize(0.05);
    t->DrawLatex(0.5,0.90,opt.cmspre);
    t->DrawLatex(0.5,0.83,opt.beamEn);
    t->SetTextSize(0.04);
    t->DrawLatex(0.5,0.76,opt.lumi);
    t->SetTextSize(0.030);
    t->DrawLatex(0.5,0.70,opt.rapString);
    t->DrawLatex(0.5,0.66,opt.ptString);
    if (opt.isPbPb != 0) {
			if (opt.isMultiplicity ==0) t->DrawLatex(0.5,0.62,opt.centString);
			else if (opt.isMultiplicity ==1) t->DrawLatex(0.5,0.62,opt.ntrkString);
			else if (opt.isMultiplicity ==2) t->DrawLatex(0.5,0.62,opt.etString);
    }
		t->DrawLatex(0.5,0.57,opt.dphiString);
    t->DrawLatex(0.5,0.54,"3.3 < m_{#mu#mu}< 3.5 GeV/c^{2}");

    leg11.SetX1NDC(0.5);
    leg11.SetY1NDC(0.39);
    leg11.SetX2NDC(0.9);
    leg11.SetY2NDC(0.49);
    leg11.Draw("same");

    hdatasb = redDataSBR->createHistogram("hdatasb",*ws->var("Jpsi_Ct"),Binning(rb));
    hpullsb = tframe1->pullHist(); hpullsb->SetName("hpullSB");
    nFitPar = fitSBR->floatParsFinal().getSize();
    chi2 = 0;
    ypullssb = hpullsb->GetY();
    nBins = ws->var("Jpsi_Ct")->getBinning().numBins();
    nFullBins = 0;
    for (unsigned int i = 0; i < nBins; i++) {
      if (hdatasb->GetBinContent(i+1) == 0) continue;
      chi2 += ypullssb[i]*ypullssb[i];
      nFullBins++;
    }
    unNormChi2 = chi2;
    dof = nFullBins - nFitPar;
    chi2 /= (nFullBins - nFitPar);
    nDOF = ws->var("Jpsi_Ct")->getBinning().numBins() - nFitPar;

    tframepull =  ws->var("Jpsi_Ct")->frame(Title("Pull")) ;
    tframepull->GetYaxis()->SetTitle("Pull");
    tframepull->SetLabelSize(0.08,"XYZ");
    tframepull->SetTitleSize(0.1,"XYZ");
    tframepull->SetTitleOffset(0.55,"Y");
    tframepull->addPlotable(hpullsb,"PX") ;
    tframemax = 0;
    if (tframepull->GetMinimum()*-1 > tframepull->GetMaximum()) tframemax = tframepull->GetMinimum()*-1;
    else tframemax = tframepull->GetMaximum();
    tframepull->SetMaximum(tframemax); 
    tframepull->SetMinimum(-1*tframemax); 
    tframepull->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
    tframepull->GetXaxis()->CenterTitle(1);

    pad2->cd(); tframepull->Draw();

    t2->SetNDC(); t2->SetTextAlign(22); t2->SetTextSize(0.07);
    sprintf(reduceDS,"#chi^{2}/dof = %.2f/%d",unNormChi2,dof);
    t2->DrawLatex(0.76,0.90,reduceDS);

    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timesideR_Lin.pdf";
    c3->SaveAs(titlestr.c_str());
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timesideR_Lin.root";
    //c3->SaveAs(titlestr.c_str());

    c3a = new TCanvas("c3a","The Canvas",200,10,600,880);
    c3a->cd();
    pad1a = new TPad("pad1a","This is pad1",0.05,0.35,0.95,0.97);
    pad1a->SetBottomMargin(0);
    pad1a->Draw();
    pad2a = new TPad("pad2a","This is pad2",0.05,0.03,0.95,0.35);
    pad2a->SetTopMargin(0);
    pad2a->SetBottomMargin(0.24);
    pad2a->Draw();

    pad1a->cd(); pad1a->SetLogy(1);
    tframe1->SetMaximum(tframe1->GetMaximum()*9); 
    tframe1->SetMinimum(0.5); 
    tframe1->Draw();
   
    t->SetTextSize(0.05);
    t->DrawLatex(0.18,0.90,opt.cmspre);
    t->DrawLatex(0.18,0.85,opt.beamEn);
    t->SetTextSize(0.04);
    t->DrawLatex(0.18,0.79,opt.lumi);
    t->SetTextSize(0.035);
    t->DrawLatex(0.62,0.80,opt.rapString);
    t->DrawLatex(0.62,0.75,opt.ptString);
    if (opt.isPbPb != 0) {
			if (opt.isMultiplicity ==0) t->DrawLatex(0.62,0.70,opt.centString);
			else if (opt.isMultiplicity ==1) t->DrawLatex(0.62,0.70,opt.ntrkString);
			else if (opt.isMultiplicity ==2) t->DrawLatex(0.62,0.70,opt.etString);
    }
		t->DrawLatex(0.62,0.65,opt.dphiString);
    t->DrawLatex(0.62,0.60,"3.2 < m_{#mu#mu}< 3.5 GeV/c^{2}");

    leg11.SetX1NDC(0.62);
    leg11.SetY1NDC(0.45);
    leg11.SetX2NDC(0.92);
    leg11.SetY2NDC(0.55);
    leg11.Draw("same");

    pad2a->cd(); tframepull->Draw();
    sprintf(reduceDS,"#chi^{2}/dof = %.2f/%d",unNormChi2,dof);
    t2->DrawLatex(0.76,0.90,reduceDS);

    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timesideR_Log.pdf";
    c3a->SaveAs(titlestr.c_str());
    titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timesideR_Log.root";
    //c3a->SaveAs(titlestr.c_str());

    delete pad1;
    delete pad2;
    delete c3;
    delete pad1a;
    delete pad2a;
    delete c3a;
    delete hdatasb;
    delete hpullsb;
    delete tframe1;
  } // end of sideband ctau plot drawing
}

void drawMassPlotsWithB(RooWorkspace *ws, RooDataSet* redDataCut, float NSigNP_fin, float NBkg_fin, RooFitResult *fitM, InputOpt &opt) {
  // Temporary variables for plotting
  TLatex *t = new TLatex();  t->SetNDC();  t->SetTextAlign(12);
  char reduceDS[512];
  string titlestr;
  
  RooBinning rb(ws->var("Jpsi_Mass")->getBinning().numBins(), ws->var("Jpsi_Mass")->getBinning().array());

  RooRealVar tmpVar1("tmpVar1","tmpVar1",NSigNP_fin);
  RooRealVar tmpVar2("tmpVar2","tmpVar2",NBkg_fin);

  // Mass plot
  RooPlot *mframe = ws->var("Jpsi_Mass")->frame();
  redDataCut->plotOn(mframe,DataError(RooAbsData::SumW2),XErrorSize(0),MarkerSize(1),Binning(rb));
  double avgBinWidth = rb.averageBinWidth();
  mframe->GetYaxis()->SetTitle(Form("Counts / %.2f (GeV/c^{2})",avgBinWidth));
  mframe->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
  mframe->GetXaxis()->CenterTitle(1);
  const double max = mframe->GetMaximum() * 1.3;
  mframe->SetMaximum(max);
  mframe->SetMinimum(0);

  if (opt.isPEE == 1) {
    // Fill color
    ws->pdf("totPDF_PEE")->plotOn(mframe,DrawOption("F"),FillColor(kBlack),FillStyle(3354),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    RooAddPdf tmpPDF("tmpPDF","tmpPDF",RooArgList(*(ws->pdf(opt.mSigFunct.c_str())),*(ws->pdf(opt.mBkgFunct.c_str()))),RooArgList(tmpVar1,tmpVar2));
    tmpPDF.plotOn(mframe,LineColor(kRed),DrawOption("F"),FillColor(kWhite),FillStyle(1001),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent));
    tmpPDF.plotOn(mframe,LineColor(kRed),DrawOption("F"),FillColor(kRed),FillStyle(3444),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent));
    gStyle->SetHatchesLineWidth(2);
    ws->pdf("totPDF_PEE")->plotOn(mframe,Components(opt.mBkgFunct.c_str()),DrawOption("F"),FillColor(kAzure-9),FillStyle(1001),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    //Line color
    ws->pdf("totPDF_PEE")->plotOn(mframe,Components(opt.mBkgFunct.c_str()),LineColor(kBlue),LineStyle(7),LineWidth(5),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    tmpPDF.plotOn(mframe,LineColor(kRed),LineStyle(9),LineWidth(5),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent));
    ws->pdf("totPDF_PEE")->plotOn(mframe,LineColor(kBlack),LineWidth(2),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
  } else {
    // Fill color
    ws->pdf("totPDF")->plotOn(mframe,DrawOption("F"),FillColor(kBlack),FillStyle(3354),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    RooAddPdf tmpPDF("tmpPDF","tmpPDF",RooArgList(*(ws->pdf(opt.mSigFunct.c_str())),*(ws->pdf(opt.mBkgFunct.c_str()))),RooArgList(tmpVar1,tmpVar2));
    tmpPDF.plotOn(mframe,LineColor(kRed),DrawOption("F"),FillColor(kWhite),FillStyle(1001),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent));
    tmpPDF.plotOn(mframe,LineColor(kRed),DrawOption("F"),FillColor(kRed),FillStyle(3444),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent));
    gStyle->SetHatchesLineWidth(2);
    ws->pdf("totPDF")->plotOn(mframe,Components(opt.mBkgFunct.c_str()),DrawOption("F"),FillColor(kAzure-9),FillStyle(1001),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));

    //Line color
    if (!opt.mBkgFunct.compare("expFunct"))
      ws->pdf("totPDF")->plotOn(mframe,Components("expFunct"),LineColor(kBlue),LineStyle(7),LineWidth(5),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    else if (!opt.mBkgFunct.compare("polFunct"))
      ws->pdf("totPDF")->plotOn(mframe,Components("polFunct"),LineColor(kBlue),LineStyle(7),LineWidth(5),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    tmpPDF.plotOn(mframe,LineColor(kRed),LineStyle(9),LineWidth(5),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent));
    ws->pdf("totPDF")->plotOn(mframe,LineColor(kBlack),LineWidth(2),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
  }
  redDataCut->plotOn(mframe,DataError(RooAbsData::SumW2),XErrorSize(0),MarkerSize(1),Binning(rb));

  TH1 *hdata = redDataCut->createHistogram("hdata",*ws->var("Jpsi_Mass"),Binning(rb));
  // *** Calculate chi2/nDof for mass fitting
  int nBins = hdata->GetNbinsX();
  RooHist *hpullm; hpullm = mframe->pullHist(); hpullm->SetName("hpullM");
  double Chi2 = 0;
  int nFullBinsPull = 0;
  double *ypull = hpullm->GetY();
  for (unsigned int i=0; i < nBins; i++) {
    if (hdata->GetBinContent(i+1) == 0) continue;
    nFullBinsPull++;
    Chi2 = Chi2 + pow(ypull[i],2);
  }
  double UnNormChi2 = Chi2;
  int nFitParam = fitM->floatParsFinal().getSize();
  int Dof = nFullBinsPull - nFitParam;
  Chi2 /= (nFullBinsPull - nFitParam);

  TCanvas c1wop; c1wop.Draw();
  mframe->Draw();
  t->SetTextSize(0.05);
  t->DrawLatex(0.18,0.90,opt.cmspre);
  t->DrawLatex(0.18,0.82,opt.beamEn);
  t->SetTextSize(0.04);
  t->DrawLatex(0.18,0.75,opt.lumi);
  t->SetTextSize(0.035);
  t->DrawLatex(0.18,0.69,opt.rapString);
  t->DrawLatex(0.18,0.64,opt.ptString);
  if (opt.isPbPb != 0) {
			if (opt.isMultiplicity ==0) t->DrawLatex(0.18,0.60,opt.centString);
			else if (opt.isMultiplicity ==1) t->DrawLatex(0.18,0.60,opt.ntrkString);
			else if (opt.isMultiplicity ==2) t->DrawLatex(0.18,0.60,opt.etString);
  }
	t->DrawLatex(0.18,0.55,opt.dphiString);
  t->SetTextSize(0.035);
  sprintf(reduceDS,"N_{J/#psi}: %0.0f #pm %0.0f",ws->var("NSig")->getVal(),ws->var("NSig")->getError());
  t->DrawLatex(0.66,0.85,reduceDS);
  sprintf(reduceDS,"#sigma = %0.0f #pm %0.0f MeV/c^{2}", opt.PcombinedWidth, opt.PcombinedWidthErr);
  t->DrawLatex(0.66,0.81,reduceDS);

  TLegend * leg11 = new TLegend(0.63,0.6,0.95,0.78,NULL,"brNDC");
  leg11->SetFillStyle(0); leg11->SetBorderSize(0); leg11->SetShadowColor(0);
  leg11->SetMargin(0.2);
  leg11->AddEntry(gfake1,"data","p");
  leg11->AddEntry(&hfake21,"total fit","lf");
  leg11->AddEntry(&hfake31,"bkgd + non-prompt","lf"); 
  leg11->AddEntry(&hfake11,"background","lf");
  leg11->Draw("same");

  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_massfit_wopull.pdf";
  c1wop.SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_massfit_wopull.root";
  c1wop.SaveAs(titlestr.c_str());

  TCanvas c1("c1","The mass Canvas",200,10,600,880);
  c1.cd();
  TPad *padm1 = new TPad("padm1","This is pad1",0.05,0.35,0.95,0.97);
  padm1->SetBottomMargin(0.15);
  padm1->Draw();
  TPad *padm2 = new TPad("padm2","This is pad2",0.05,0.03,0.95,0.35);
  padm2->SetTopMargin(0.08);
  padm2->SetBottomMargin(0.24);
  padm2->Draw();

  padm1->cd();  mframe->Draw();
  t->SetTextSize(0.05);
  t->DrawLatex(0.18,0.90,opt.cmspre);
  t->DrawLatex(0.18,0.82,opt.beamEn);
  t->SetTextSize(0.04);
  t->DrawLatex(0.18,0.75,opt.lumi);
  t->SetTextSize(0.035);
  t->DrawLatex(0.18,0.69,opt.rapString);
  t->DrawLatex(0.18,0.64,opt.ptString);
  if (opt.isPbPb != 0) {
			if (opt.isMultiplicity ==0) t->DrawLatex(0.18,0.60,opt.centString);
			else if (opt.isMultiplicity ==1) t->DrawLatex(0.18,0.60,opt.ntrkString);
			else if (opt.isMultiplicity ==2) t->DrawLatex(0.18,0.60,opt.etString);
  }
  t->DrawLatex(0.18,0.55,opt.dphiString);
  t->SetTextSize(0.035);
  sprintf(reduceDS,"N_{J/#psi}: %0.0f #pm %0.0f",ws->var("NSig")->getVal(),ws->var("NSig")->getError());
  t->DrawLatex(0.66,0.85,reduceDS);
  sprintf(reduceDS,"#sigma = %0.0f #pm %0.0f MeV/c^{2}", opt.PcombinedWidth, opt.PcombinedWidthErr);
  t->DrawLatex(0.66,0.81,reduceDS);

  leg11->Draw("same");
  c1.Update();

  RooPlot* mframepull =  ws->var("Jpsi_Mass")->frame(Title("Pull")) ;
  mframepull->GetYaxis()->SetTitle("Pull");
  mframepull->SetLabelSize(0.08,"XYZ");
  mframepull->SetTitleSize(0.1,"XYZ");
  mframepull->SetTitleOffset(0.55,"Y");
  mframepull->addPlotable(hpullm,"PX") ;
  double mframemax = 0;
  if (mframepull->GetMinimum()*-1 > mframepull->GetMaximum()) mframemax = mframepull->GetMinimum()*-1;
  else mframemax = mframepull->GetMaximum();
  mframepull->SetMaximum(mframemax); 
  mframepull->SetMinimum(-1*mframemax); 
  mframepull->GetXaxis()->SetTitle("m_{#mu#mu} (MeV/c^{2})");
  mframepull->GetXaxis()->CenterTitle(1);

  padm2->cd(); mframepull->Draw();
  t->SetTextSize(0.07);
  sprintf(reduceDS,"#chi^{2}/dof = %.1f/%d",UnNormChi2,Dof);
  t->DrawLatex(0.6,0.83,reduceDS);
  c1.Update();
  t->SetTextSize(0.04);

  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_massfit.pdf";
  c1.SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_massfit.root";
  //c1.SaveAs(titlestr.c_str());
}

void drawCtauFitPlots(RooWorkspace *ws, RooDataSet *redDataCut, RooDataHist* binDataCtErr, float NSigNP_fin, float NBkg_fin, RooFitResult *fit2D, InputOpt &opt) {
  char reduceDS[512];
  string titlestr;
  
  RooBinning rb(ws->var("Jpsi_Ct")->getBinning().numBins(), ws->var("Jpsi_Ct")->getBinning().array());

  RooRealVar tmpVar1("tmpVar1","tmpVar1",NSigNP_fin);
  RooRealVar tmpVar2("tmpVar2","tmpVar2",NBkg_fin);
  
  RooPlot *tframe = ws->var("Jpsi_Ct")->frame();
  double avgBinWidth = rb.averageBinWidth();
  tframe->GetYaxis()->SetTitle(Form("Counts / %.2f (mm)",avgBinWidth));
  tframe->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
  tframe->GetXaxis()->CenterTitle(1);
//  tframe->GetYaxis()->SetTitle("Events / (0.035 mm)");

  // Ctau total distributions
  RooHist *hpulltot;
  redDataCut->plotOn(tframe,DataError(RooAbsData::SumW2),Binning(rb),MarkerSize(1));

  if (opt.isPEE == 1) {
    ws->pdf("totPDF_PEE")->plotOn(tframe,LineColor(kBlack),LineWidth(2),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    hpulltot = tframe->pullHist(); hpulltot->SetName("hpulltot");
    ws->pdf("totPDF_PEE")->plotOn(tframe,Components("totBKG"),LineColor(kBlue),LineWidth(5),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent),LineStyle(7));
    ws->pdf("totPDF_PEE")->plotOn(tframe,Components("totSIGNP"),LineColor(kRed),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent),LineStyle(kDashed));
    ws->pdf("totPDF_PEE")->plotOn(tframe,Components("totSIGPR"),LineColor(kGreen),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent),LineStyle(kDashDotted));
    ws->pdf("totPDF_PEE")->plotOn(tframe,LineColor(kBlack),LineWidth(2),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
  } else {  //not pee
    ws->pdf("totPDF")->plotOn(tframe,LineColor(kBlack),LineWidth(2),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    hpulltot = tframe->pullHist(); hpulltot->SetName("hpulltot");
    ws->pdf("totPDF")->plotOn(tframe,Components("bkgCtTot"),LineColor(kBlue),LineWidth(5),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent),LineStyle(7));
    ws->pdf("totPDF")->plotOn(tframe,Components("sigNP"),LineColor(kRed),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent),LineStyle(kDashed));
    ws->pdf("totPDF")->plotOn(tframe,Components("sigPR"),LineColor(kGreen),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent),LineStyle(kDashDotted));
    ws->pdf("totPDF")->plotOn(tframe,LineColor(kBlack),LineWidth(2),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
  }

  TH1 *hdatact = redDataCut->createHistogram("hdatact",*ws->var("Jpsi_Ct"),Binning(rb));
  double chi2 = 0, unNormChi2 = 0;
  int dof = 0;
  double *ypulls = hpulltot->GetY();
  unsigned int nBins = ws->var("Jpsi_Ct")->getBinning().numBins();
  unsigned int nFullBins = 0;
  for (unsigned int i = 0; i < nBins; i++) {
    if (hdatact->GetBinContent(i+1) == 0) continue;
    chi2 += ypulls[i]*ypulls[i];
    nFullBins++;
  }
  unNormChi2 = chi2;
  int nFitPar = fit2D->floatParsFinal().getSize();
  dof = nFullBins - nFitPar;
  chi2 /= (nFullBins - nFitPar);

  // WITH RESIDUALS
  TCanvas* c2 = new TCanvas("c2","The Canvas",200,10,600,880);
  c2->cd();
  TPad *pad1 = new TPad("pad1","This is pad1",0.05,0.35,0.95,0.97);
  pad1->SetBottomMargin(0);
  pad1->Draw();
  TPad *pad2 = new TPad("pad2","This is pad2",0.05,0.03,0.95,0.35);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.24);
  pad2->Draw();

  pad1->cd(); tframe->Draw();

  TLatex *t = new TLatex();  t->SetNDC();  t->SetTextAlign(12);
  t->SetTextSize(0.05);
  t->DrawLatex(0.57,0.90,opt.cmspre);
  t->DrawLatex(0.57,0.83,opt.beamEn);
  t->SetTextSize(0.04);
  t->DrawLatex(0.60,0.76,opt.lumi);
  t->SetTextSize(0.030);
  t->DrawLatex(0.57,0.70,opt.rapString);
  t->DrawLatex(0.57,0.66,opt.ptString);
  if (opt.isPbPb != 0) {
//			if (opt.isMultiplicity ==0) t->DrawLatex(0.57,0.63,opt.centString);
			if (opt.isMultiplicity ==0) t->DrawLatex(0.57,0.61,opt.centString);
			else if (opt.isMultiplicity ==1) t->DrawLatex(0.57,0.61,opt.ntrkString);
			else if (opt.isMultiplicity ==2) t->DrawLatex(0.57,0.61,opt.etString);
  }
  t->DrawLatex(0.57,0.57,opt.dphiString);

  TLegend * leg = new TLegend(0.575,0.38,0.85,0.57,NULL,"brNDC");
//  TLegend * leg = new TLegend(0.455,0.35,0.85,0.54,NULL,"brNDC");   //with dPhistring
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetShadowColor(0);
  leg->SetMargin(0.2);
  leg->AddEntry(gfake1,"data","p");
  leg->AddEntry(&hfake21,"total fit","l");
  leg->AddEntry(&hfake41,"prompt","l"); 
  leg->AddEntry(&hfake311,"non-prompt","l"); 
  leg->AddEntry(&hfake11,"background","l");
  leg->Draw("same"); 

  RooPlot* tframepull =  ws->var("Jpsi_Ct")->frame(Title("Pull")) ;
  tframepull->GetYaxis()->SetTitle("Pull");
  tframepull->SetLabelSize(0.08,"XYZ");
  tframepull->SetTitleSize(0.1,"XYZ");
  tframepull->SetTitleOffset(0.55,"Y");
  tframepull->addPlotable(hpulltot,"PX") ;
  double tframemax = 0;
  if (tframepull->GetMinimum()*-1 > tframepull->GetMaximum()) tframemax = tframepull->GetMinimum()*-1;
  else tframemax = tframepull->GetMaximum();
  tframepull->SetMaximum(tframemax); 
  tframepull->SetMinimum(-1*tframemax);
  tframepull->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
  tframepull->GetXaxis()->CenterTitle(1);

  pad2->cd(); tframepull->Draw();

  int nDOF = ws->var("Jpsi_Ct")->getBinning().numBins() - nFitPar;

  TLatex *t2 = new TLatex();
  t2->SetNDC(); t2->SetTextAlign(22); t2->SetTextSize(0.07);
  sprintf(reduceDS,"#chi^{2}/dof = %.2f/%d",unNormChi2,dof);
  t2->DrawLatex(0.76,0.90,reduceDS);
  
  c2->Update();
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timefit_Lin.pdf";
  c2->SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timefit_Lin.root";
  //c2->SaveAs(titlestr.c_str());

  TCanvas* c2a = new TCanvas("c2a","The Canvas",200,10,600,880);
  c2a->cd();
  TPad *pad1a = new TPad("pad1a","This is pad1",0.05,0.35,0.95,0.97);
  pad1a->SetBottomMargin(0);
  pad1a->Draw();
  TPad *pad2a = new TPad("pad2a","This is pad2",0.05,0.03,0.95,0.35);
  pad2a->SetTopMargin(0);
  pad2a->SetBottomMargin(0.24);
  pad2a->Draw();

  pad1a->cd(); pad1a->SetLogy(1);
  tframe->SetMaximum(tframe->GetMaximum()*9); 
  tframe->SetMinimum(0.5); 
  tframe->Draw();
 
  t->SetTextSize(0.05);
  t->DrawLatex(0.20,0.90,opt.cmspre);
  t->DrawLatex(0.20,0.85,opt.beamEn);
  t->SetTextSize(0.04);
  t->DrawLatex(0.20,0.79,opt.lumi);
  t->SetTextSize(0.035);
  t->DrawLatex(0.62,0.79,opt.rapString);
  t->DrawLatex(0.62,0.74,opt.ptString);
  if (opt.isPbPb != 0) {
//			if (opt.isMultiplicity ==0) t->DrawLatex(0.62,0.70,opt.centString);
			if (opt.isMultiplicity ==0) t->DrawLatex(0.62,0.68,opt.centString);
			else if (opt.isMultiplicity ==1) t->DrawLatex(0.62,0.68,opt.ntrkString);
			else if (opt.isMultiplicity ==2) t->DrawLatex(0.62,0.68,opt.etString);
  }
  t->DrawLatex(0.62,0.65,opt.dphiString);

// with dPhiString
//  leg->SetX1NDC(0.62);
//  leg->SetY1NDC(0.37);
//  leg->SetX2NDC(0.93);
//  leg->SetY2NDC(0.57);
  
  leg->SetX1NDC(0.62);
  leg->SetY1NDC(0.44);
  leg->SetX2NDC(0.93);
  leg->SetY2NDC(0.64);
  leg->Draw("same");

  pad2a->cd(); tframepull->Draw();
  sprintf(reduceDS,"#chi^{2}/dof = %.2f/%d",unNormChi2,dof);
  t2->DrawLatex(0.76,0.90,reduceDS);
  
  c2a->Update();
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange +  "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timefit_Log.pdf";
  c2a->SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange +  "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timefit_Log.root";
  //c2a->SaveAs(titlestr.c_str());
  delete c2a;

  TCanvas* c2b = new TCanvas("c2b","The Canvas",200,10,540,546);
  c2b->cd(); c2b->Draw(); c2b->SetLogy(1);

  RooPlot *tframefill = ws->var("Jpsi_Ct")->frame();
  //double avgBinWidth = rb.averageBinWidth();
  tframefill->GetYaxis()->SetTitle(Form("Counts / %.2f (mm)",avgBinWidth));
  redDataCut->plotOn(tframefill,DataError(RooAbsData::SumW2),Binning(rb),MarkerSize(1));
  if (opt.isPEE == 1) {
    ws->pdf("totPDF_PEE")->plotOn(tframefill,DrawOption("F"),FillColor(kBlack),FillStyle(3354),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    ws->pdf("totPDF_PEE")->plotOn(tframefill,LineColor(kBlack),LineWidth(2),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    RooAddPdf tmpPDF2("tmpPDF2","tmpPDF2",RooArgList(*(ws->pdf("totSIGNP")),*(ws->pdf("totBKG"))),RooArgList(tmpVar1,tmpVar2));
    tmpPDF2.plotOn(tframefill,DrawOption("F"),FillColor(kWhite),FillStyle(1001),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent));
    tmpPDF2.plotOn(tframefill,DrawOption("F"),FillColor(kRed),FillStyle(3444),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent));
    gStyle->SetHatchesLineWidth(2);
    ws->pdf("totPDF_PEE")->plotOn(tframefill,Components("totBKG"),DrawOption("F"),FillColor(kAzure-9),FillStyle(1001),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    ws->pdf("totPDF_PEE")->plotOn(tframefill,Components("totBKG"),LineColor(kBlue),LineWidth(5),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent),LineStyle(7));
    tmpPDF2.plotOn(tframefill,LineColor(kRed),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent),LineWidth(5),LineStyle(9));
  } else {  // not pee
    ws->pdf("totPDF")->plotOn(tframefill,DrawOption("F"),FillColor(kBlack),FillStyle(3354),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    RooAddPdf tmpPDF2("tmpPDF2","tmpPDF2",RooArgList(*(ws->pdf("sigNP")),*(ws->pdf("bkgCtTot"))),RooArgList(tmpVar1,tmpVar2));
    tmpPDF2.plotOn(tframefill,DrawOption("F"),FillColor(kWhite),FillStyle(1001),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent));
    tmpPDF2.plotOn(tframefill,DrawOption("F"),FillColor(kRed),FillStyle(3444),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent));
    gStyle->SetHatchesLineWidth(2);
    ws->pdf("totPDF")->plotOn(tframefill,Components("bkgCtTot"),DrawOption("F"),FillColor(kAzure-9),FillStyle(1001),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    ws->pdf("totPDF")->plotOn(tframefill,LineColor(kBlack),LineWidth(2),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent));
    ws->pdf("totPDF")->plotOn(tframefill,Components("bkgCtTot"),LineColor(kBlue),LineWidth(5),Normalization(redDataCut->sumEntries(),RooAbsReal::NumEvent),LineStyle(7));
    tmpPDF2.plotOn(tframefill,LineColor(kRed),Normalization(NSigNP_fin+NBkg_fin,RooAbsReal::NumEvent),LineWidth(5),LineStyle(9));
  }
  redDataCut->plotOn(tframefill,DataError(RooAbsData::SumW2),Binning(rb),MarkerSize(1));

  tframefill->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
  tframefill->GetXaxis()->CenterTitle(1);
  tframefill->GetYaxis()->SetTitle("Events / (0.035 mm)");
  tframefill->SetMaximum(tframefill->GetMaximum()*9); 
  tframefill->SetMinimum(0.5); 
  tframefill->Draw();

  t->SetTextSize(0.05);
  t->DrawLatex(0.19,0.9,opt.cmspre);
  t->DrawLatex(0.19,0.83,opt.beamEn); 
  t->SetTextSize(0.04);
  t->DrawLatex(0.19,0.76,opt.lumi); 
  t->SetTextSize(0.035);
  t->DrawLatex(0.60,0.76,opt.rapString);
  t->DrawLatex(0.60,0.71,opt.ptString);
  //if (opt.isPbPb != 0) t->DrawLatex(0.60,0.67,opt.centString);
  if (opt.isPbPb != 0) {
			if (opt.isMultiplicity ==0) t->DrawLatex(0.60,0.66,opt.centString);
			else if (opt.isMultiplicity ==1) t->DrawLatex(0.60,0.66,opt.ntrkString);
			else if (opt.isMultiplicity ==2) t->DrawLatex(0.60,0.66,opt.etString);
  }
  t->DrawLatex(0.60,0.61,opt.dphiString);

  TLegend * leg11 = new TLegend(0.58,0.5,0.92,0.64,NULL,"brNDC");
  leg11->SetFillStyle(0); leg11->SetBorderSize(0); leg11->SetShadowColor(0);
  leg11->SetMargin(0.2);
  leg11->AddEntry(gfake1,"data","p");
  leg11->AddEntry(&hfake21,"total fit","lf");
  leg11->AddEntry(&hfake31,"bkgd + non-prompt","lf"); 
  leg11->AddEntry(&hfake11,"background","lf");
  leg11->Draw("same");
/*
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_timefit_Log_wopull.pdf";
  c2b->SaveAs(titlestr.c_str());
  delete c2b;*/
  c2b->Update();
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange +  "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timefit_Log_wopull.pdf";
  c2b->SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange +  "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timefit_Log_wopull.root";
  c2b->SaveAs(titlestr.c_str());
  delete c2b;

}


void drawMassCtau2DPlots(RooWorkspace *ws, InputOpt &opt) {

  RooBinning rb(ws->var("Jpsi_Ct")->getBinning().numBins(), ws->var("Jpsi_Ct")->getBinning().array());

  string titlestr;
  RooAbsPdf *hist2D;
  if (opt.isPEE == 1)  hist2D = ws->pdf("totPDF_PEE");
  else hist2D = ws->pdf("totPDF");
  TH1* h2D = hist2D->createHistogram("2DHist",*(ws->var("Jpsi_Mass")),YVar(*(ws->var("Jpsi_Ct")),Binning(rb)));
  h2D->GetZaxis()->SetTitle("");
  h2D->GetZaxis()->SetTitleOffset(5.0);
  double avgBinWidth = rb.averageBinWidth();
//  h2D->GetZaxis()->SetTitle(Form("Counts / %.2f (mm)",avgBinWidth));
  gStyle->SetPadLeftMargin(0.2);
  gStyle->SetTitleSize(0.04,"xyz");
  gStyle->SetLabelSize(0.03,"xyz");
  TCanvas* c2c = new TCanvas("c2c","The Canvas",1000,1000);
  c2c->cd();
  h2D->Draw("SURF1");
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_2D_totFit.pdf";
  c2c->SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_2D_totFit.root";
  c2c->SaveAs(titlestr.c_str());
  delete c2c;
}

void drawCtauFitPlotsSignals(RooWorkspace *ws, RooDataSet *redDataCut, RooDataSet *redDataSB, RooDataSet *redMCCutNP, float NSigPR_fin, float NSigNP_fin, InputOpt &opt) {
  TLatex *t = new TLatex();  t->SetNDC();  t->SetTextAlign(12);
  string titlestr;

  RooBinning rb(ws->var("Jpsi_Ct")->getBinning().numBins(), ws->var("Jpsi_Ct")->getBinning().array());
  RooBinning rberr(ws->var("Jpsi_CtErr")->getBinning().numBins(), ws->var("Jpsi_CtErr")->getBinning().array());

  float lmin, lmax, errmin, errmax;
  getOptRange(opt.errrange,&errmin,&errmax);
  getOptRange(opt.lrange,&lmin,&lmax);

  // case 1) : sideband subtraction from full data distribution
  TH1D *sideDatahist = (TH1D*)redDataSB->createHistogram("sideDatahist",*(ws->var("Jpsi_Ct")),Binning(rb));
  TH2D *hist2DSB = (TH2D*)redDataSB->createHistogram("2DHistSB",*(ws->var("Jpsi_Ct")),Binning(rb),YVar(*(ws->var("Jpsi_CtErr")),Binning(rberr)));
  hist2DSB->Sumw2();
  float bc;
  if (!opt.mBkgFunct.compare("expFunct")) bc = ws->var("coefExp")->getVal();
  else if (!opt.mBkgFunct.compare("polFunct")) bc = ws->var("coefPol1")->getVal();
  float scaleF = (exp(2.6*bc)-exp(3.5*bc))/(exp(2.6*bc)-exp(2.9*bc)+exp(3.3*bc)-exp(3.5*bc));
  cout << "drawCtauFitPlotsSignals:: bc: " << bc << " scaleF: " << scaleF << endl;
  sideDatahist->Scale(scaleF);
  hist2DSB->Scale(scaleF);
  
  TCanvas canv;
  canv.cd();
  canv.SetRightMargin(0.17);
  canv.SetLeftMargin(0.12);
  hist2DSB->GetYaxis()->SetTitleOffset(1.2);
  hist2DSB->GetZaxis()->SetTitleOffset(1.35);
  hist2DSB->Draw("colz");
  canv.SaveAs("2D_sideband.png");

  /* case 2) : background lifetime function subtraction from full data distribution*/
  /*  RooDataHist *sideData = ws->pdf("totBKG")->generateBinned(*(ws->var("Jpsi_Ct")),1000000);
  TH1D *sideDatahist = (TH1D*)sideData->createHistogram("sideDatahist",*(ws->var("Jpsi_Ct")),Binning(rb));
  sideDatahist->Scale(NBkg_fin/1000000.0);*/

  // Full data distribution
  TH1D *binDatahist = (TH1D*)redDataCut->createHistogram("binDatahist",*(ws->var("Jpsi_Ct")),Binning(rb));
  TH1D *subtrSighist = new TH1D("subtrSighist","subtrSighist", ws->var("Jpsi_Ct")->getBinning().numBins(), ws->var("Jpsi_Ct")->getBinning().array());

  TH2D *hist2D = (TH2D*)redDataCut->createHistogram("2DHist",*(ws->var("Jpsi_Ct")),Binning(rb),YVar(*(ws->var("Jpsi_CtErr")),Binning(rberr)));
  TH2D *hist2DSig = (TH2D*)hist2D->Clone();
  hist2DSig->Reset();
  hist2DSig->Sumw2();
  hist2D->Sumw2();
  canv.Clear(); canv.cd();
  hist2D->GetYaxis()->SetTitleOffset(1.2);
  hist2D->GetZaxis()->SetTitleOffset(1.35);
  hist2D->Draw("colz");
  canv.SaveAs("2D_data.png");

  // Subtract background from full distribution in 1D
  for (int i=1; i <= binDatahist->GetNbinsX(); i++) {
    double all = binDatahist->GetBinContent(i);
    double side = sideDatahist->GetBinContent(i);
    double diff = all-side;
    if (diff<0) diff=0;
    subtrSighist->SetBinContent(i,diff);
  }

  // Subtract background from full distribution in 2D
  for (int i=1; i<= ws->var("Jpsi_Ct")->getBinning().numBins(); i++) {
    for (int j=1; j<=ws->var("Jpsi_CtErr")->getBinning().numBins(); j++) {
      int nbin = hist2D->GetBin(i,j);
      double all = hist2D->GetBinContent(nbin);
      double side = hist2DSB->GetBinContent(nbin);
      double diff = all-side;
      if (diff<0) diff=0;
      hist2DSig->SetBinContent(nbin,diff);
    }
  }
  
  RooCategory category("LifetimeVariables","LifetimeVariables");
  category.defineType("Jpsi_Ct");
  category.defineType("Jpsi_CtErr");

  // b-fraction is 2.3841e-01 +/- 2.84e-03, but scaledSig is very higher than full dataset
//  RooDataHist *scaledSig = new RooDataHist("scaledSig","scaledSig",RooArgList( *(ws->var("Jpsi_Ct")),*(ws->var("Jpsi_CtErr"))),Index(category),Import("Jpsi_Ct",*hist2DSig),Import("Jpsi_CtErr",*hist2DSig)); //signal only hist

  // Draws normal hist
//  RooDataHist *scaledSig = new RooDataHist("scaledSig","scaledSig",RooArgList( *(ws->var("Jpsi_Ct")),*(ws->var("Jpsi_CtErr"))),category,histlist); //signal only hist
  RooDataHist *scaledSig2 = new RooDataHist("scaledSig2","scaledSig2",RooArgList( *(ws->var("Jpsi_Ct")),*(ws->var("Jpsi_CtErr"))),Index(category),Import(*hist2DSig,kFALSE)); //signal only hist

  canv.Clear(); canv.cd();
  TH2D* h2D3 = (TH2D*)scaledSig2->createHistogram("2DHistSig",*(ws->var("Jpsi_Ct")),Binning(rb),YVar(*(ws->var("Jpsi_CtErr")),Binning(rberr)));
  h2D3->Sumw2();
  h2D3->GetYaxis()->SetTitleOffset(1.2);
  h2D3->GetZaxis()->SetTitleOffset(1.35);
  h2D3->Draw("colz");
  canv.SaveAs("2D_signalonly.png");

  TH1D* h2D1 = (TH1D*)scaledSig2->createHistogram("1DHistSig",*(ws->var("Jpsi_CtErr")),Binning(rberr));
  h2D1->Sumw2();
  canv.Clear(); canv.cd();
  canv.SetRightMargin(0.035);
  canv.SetLeftMargin(0.15);
  h2D1->Draw();
  canv.SaveAs("subtrData2.png");

  TH1D* h2D2 = (TH1D*)scaledSig2->createHistogram("1DHistSig2",*(ws->var("Jpsi_Ct")),Binning(rb));
  h2D2->Sumw2();
  canv.Clear(); canv.cd();
  h2D2->Draw();
  subtrSighist->SetMarkerColor(kRed);
  subtrSighist->SetLineColor(kRed);
  subtrSighist->Draw("same");
  canv.SaveAs("1D_signalonly.png");
  
//  RooDataHist *scaledSig = new RooDataHist("scaledSig","scaledSig",RooArgList( *(ws->var("Jpsi_Ct")),*(ws->var("Jpsi_CtErr"))),Index(category),Import("Jpsi_Ct",*subtrSighist),Import("Jpsi_CtErr",*h2D1)); //signal only hist
  RooDataHist *scaledSig = new RooDataHist("scaledSig","scaledSig",RooArgList( *(ws->var("Jpsi_Ct")),*(ws->var("Jpsi_CtErr"))),Index(category),Import(*hist2DSig,kFALSE)); //signal only hist
  TH2D* h2D4 = (TH2D*)scaledSig->createHistogram("2DHistSig2",*(ws->var("Jpsi_Ct")),Binning(rb),YVar(*(ws->var("Jpsi_CtErr")),Binning(rberr)));
  canv.Clear(); canv.cd();
  canv.SetRightMargin(0.17);
  canv.SetLeftMargin(0.12);
  h2D4->GetYaxis()->SetTitleOffset(1.2);
  h2D4->GetZaxis()->SetTitleOffset(1.35);
  h2D4->Draw("colz");
  canv.SaveAs("2D_signalonly2.png");


  RooDataHist *subtrData = new RooDataHist("subtrData2","subtrData2",RooArgList(*(ws->var("Jpsi_CtErr"))),h2D1);  ws->import(*subtrData);
  RooHistPdf *errPdfSig = new RooHistPdf("errPdfSig2","errPdfSig2",RooArgSet(*(ws->var("Jpsi_CtErr"))),*subtrData);  ws->import(*errPdfSig);
 
  RooDataHist* binMCCutNP = new RooDataHist("binMCCutNP","MC distribution for NP signal",RooArgSet(*(ws->var("Jpsi_CtTrue"))),*redMCCutNP);
  // per-event error 
/*  ws->factory("GaussModel::resGW2(Jpsi_Ct,meanResSigW2[0.,-0.01,0.01],sigmaResSigW2[2.3,1.3,3.5],one[1.0],Jpsi_CtErr)");
  ws->factory("GaussModel::resGN2(Jpsi_Ct,meanResSigW2,sigmaResSigN2[0.8,0.6,1.1],one,Jpsi_CtErr)");
  ws->factory("AddModel::sigPR2({resGW2,resGN2},{fracRes2[0.05,0.001,0.3]})");
  
  RooHistPdfConv sigNPW("sigNPW2","Non-prompt signal with wide gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW2")),*(ws->var("sigmaResSigW2")),*(ws->var("one")),*(ws->var("Jpsi_CtErr")),*binMCCutNP);  ws->import(sigNPW);
  RooHistPdfConv sigNPN("sigNPN2","Non-prompt signal with narrow gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW2")),*(ws->var("sigmaResSigN2")),*(ws->var("one")),*(ws->var("Jpsi_CtErr")),*binMCCutNP);  ws->import(sigNPN);
  RooAddPdf sigNP("sigNP2","Non-prompt signal",RooArgSet(*(ws->pdf("sigNPW2")),*(ws->pdf("sigNPN2"))),RooArgSet(*(ws->var("fracRes2"))));  ws->import(sigNP); 

  RooProdPdf totSIGPR2("totSIGPR2","PDF with PEE", *(ws->pdf("errPdfSig2")),
                         Conditional( *(ws->pdf("sigPR2")), RooArgList(*(ws->var("Jpsi_Ct"))) )
                         );  ws->import(totSIGPR2);
  RooProdPdf totSIGNP2("totSIGNP2","PDF with PEE", *(ws->pdf("errPdfSig2")),
                         Conditional( *(ws->pdf("sigNP2")), RooArgList(*(ws->var("Jpsi_Ct"))) )
                         );  ws->import(totSIGNP2);
  RooFitResult *sigres = ws->pdf("sigPDF_PEE")->fitTo(*scaledSig,Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8),ConditionalObservables(RooArgSet(*(ws->var("Jpsi_CtErr")))));
  sigres->Print("v");
*/

  // not per-event error
  opt.isPEE = 0;
  ws->factory("GaussModel::resGW2(Jpsi_Ct,meanResSigW2[0.,-0.1,0.1],sigmaResSigW2[0.07,0.01,0.9])");
  ws->factory("GaussModel::resGN2(Jpsi_Ct,meanResSigW2,sigmaResSigN2[0.04,0.01,0.3])");
  ws->factory("GaussModel::resGO2(Jpsi_Ct,meanResSigW2,sigmaResSigO2[0.2,0.05,1.0])");
  ws->factory("GaussModel::resGM2(Jpsi_Ct,meanResSigW2,sigmaResSigM2[0.4,0.04,2.0])");
  ws->factory("AddModel::sigPR2({resGW2,resGO2,resGM2,resGN2},{fracRes12[0.2,0.01,0.9],fracRes22[0.02,0.0,0.25],fracRes32[0.1,0.0,0.5]})");

/*  ws->factory("GaussModel::resGW2(Jpsi_Ct,meanResSigW2[0.,-0.1,0.1],sigmaResSigW2[0.07,0.01,0.9])");
  ws->factory("GaussModel::resGN2(Jpsi_Ct,meanResSigW2,sigmaResSigN2[0.04,0.01,0.3])");
  ws->factory("AddModel::sigPR2({resGW2,resGN2},{fracRes22[0.2,0.01,0.9]})");*/
  
  RooHistPdfConv sigNPW("sigNPW2","Non-prompt signal with wide gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW2")),*(ws->var("sigmaResSigW2")),*binMCCutNP ); ws->import(sigNPW);
  RooHistPdfConv sigNPO("sigNPO2","Non-prompt signal with outstanding gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW2")),*(ws->var("sigmaResSigO2")),*binMCCutNP ); ws->import(sigNPO);
  RooHistPdfConv sigNPM("sigNPM2","Non-prompt signal with mastodontic gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW2")),*(ws->var("sigmaResSigM2")),*binMCCutNP ); ws->import(sigNPM);
  RooHistPdfConv sigNPN("sigNPN2","Non-prompt signal with narrow gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW2")),*(ws->var("sigmaResSigN2")),*binMCCutNP ); ws->import(sigNPN);
  RooAddPdf sigNP("sigNP2","Non-prompt signal",RooArgSet(*(ws->pdf("sigNPW2")),*(ws->pdf("sigNPO2")),*(ws->pdf("sigNPM2")),*(ws->pdf("sigNPN2"))),RooArgSet(*(ws->var("fracRes12")),*(ws->var("fracRes22")),*(ws->var("fracRes32")))); ws->import(sigNP);
  
/*  RooHistPdfConv sigNPN("sigNPN2","Non-prompt signal with narrow gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW2")),*(ws->var("sigmaResSigN2")),*binMCCutNP ); ws->import(sigNPN);
  RooHistPdfConv sigNPW("sigNPW2","Non-prompt signal with wide gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW2")),*(ws->var("sigmaResSigW2")),*binMCCutNP ); ws->import(sigNPW);
  RooAddPdf sigNP("sigNP2","Non-prompt signal",RooArgSet(*(ws->pdf("sigNPW2")),*(ws->pdf("sigNPN2"))),RooArgSet(*(ws->var("fracRes22")))); ws->import(sigNP);*/

	ws->factory("SUM::sigPDF(Bfrac2[0.25,0.0,1.0]*sigNP2,sigPR2)");
	//ws->factory("SUM::sigPDF(Bfrac2[0.25,0.2,0.8]*sigNP2,sigPR2)");
  RooDataHist *sigHist = new RooDataHist("subHist","sigHist",RooArgList(*(ws->var("Jpsi_Ct"))),subtrSighist);
  RooFitResult *sigres = ws->pdf("sigPDF")->fitTo(*sigHist,Minos(0),Save(1),SumW2Error(kTRUE),NumCPU(8));
  sigres->Print("v");

  RooPlot *tframeSig = ws->var("Jpsi_Ct")->frame();
  double avgBinWidth = rb.averageBinWidth();
  tframeSig->GetYaxis()->SetTitle(Form("Counts / %.2f (mm)",avgBinWidth));
  tframeSig->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
  tframeSig->GetXaxis()->CenterTitle(1);

  RooHist *hpullsig;
  redDataCut->plotOn(tframeSig,DataError(RooAbsData::SumW2),Binning(rb),MarkerSize(0.3),MarkerColor(kBlue),LineColor(kBlue));  // full data
//  scaledSig->plotOn(tframeSig,DataError(RooAbsData::SumW2),Binning(rb),MarkerSize(1)); // subtracted signal*/
  sigHist->plotOn(tframeSig,DataError(RooAbsData::SumW2),Binning(rb),MarkerSize(1)); // subtracted signal*/

  float NSig_fin = NSigPR_fin + NSigNP_fin;
  NSigNP_fin = NSig_fin*ws->var("Bfrac2")->getVal();
  NSigPR_fin = NSig_fin*(1-ws->var("Bfrac2")->getVal());
  RooRealVar tmpVar1("tmpVar1","tmpVar1",NSigNP_fin);
  RooRealVar tmpVar3("tmpVar3","tmpVar3",NSigPR_fin);

  if (opt.isPEE == 1) {
//    ws->pdf("totPDF_PEE")->plotOn(tframeSig,LineColor(kBlue),LineWidth(2),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(NSig_fin,RooAbsReal::NumEvent));
    RooAddPdf tmpPDF2("tmpPDF2","tmpPDF2",RooArgList(*(ws->pdf("totSIGNP2")),*(ws->pdf("totSIGPR2"))),RooArgList(tmpVar1,tmpVar3)); //tmpVar1 = NSigNP, tmpVar3 = NSigPR
    tmpPDF2.plotOn(tframeSig,LineColor(kBlack),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*subtrData,kTRUE),NumCPU(8),Normalization(NSig_fin,RooAbsReal::NumEvent),LineWidth(2));
//    ws->pdf("totPDF_PEE")->plotOn(tframeSig,Components("totSIGNP"),LineColor(kRed),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErrSIG,kTRUE),NumCPU(8),Normalization(NSig_fin,RooAbsReal::NumEvent),LineStyle(kDashed));
//    ws->pdf("totPDF_PEE")->plotOn(tframeSig,Components("totSIGPR"),LineColor(kGreen),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErrSIG,kTRUE),NumCPU(8),Normalization(NSig_fin,RooAbsReal::NumEvent),LineStyle(kDashDotted));
    ws->pdf("totSIGNP2")->plotOn(tframeSig,LineColor(kRed),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*subtrData,kTRUE),NumCPU(8),Normalization(NSigNP_fin,RooAbsReal::NumEvent),LineStyle(kDashed));
    ws->pdf("totSIGPR2")->plotOn(tframeSig,LineColor(kGreen),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*subtrData,kTRUE),NumCPU(8),Normalization(NSigPR_fin,RooAbsReal::NumEvent),LineStyle(kDashDotted));
//    ws->pdf("totPDF_PEE")->plotOn(tframeSig,LineColor(kBlack),LineWidth(2),ProjWData(RooArgList(*(ws->var("Jpsi_CtErr"))),*binDataCtErr,kTRUE),NumCPU(8),Normalization(NSig_fin,RooAbsReal::NumEvent));
  } else {  //not pee
    ws->pdf("sigPDF")->plotOn(tframeSig,LineColor(kBlack),LineWidth(2),Normalization(NSig_fin,RooAbsReal::NumEvent));
    ws->pdf("sigPDF")->plotOn(tframeSig,Components("sigNP2"),LineColor(kRed),Normalization(NSig_fin,RooAbsReal::NumEvent),LineStyle(kDashed));
    ws->pdf("sigPDF")->plotOn(tframeSig,Components("sigPR2"),LineColor(kGreen),Normalization(NSig_fin,RooAbsReal::NumEvent),LineStyle(kDashDotted));
    ws->pdf("sigPDF")->plotOn(tframeSig,LineColor(kBlack),LineWidth(2),Normalization(NSig_fin,RooAbsReal::NumEvent));
  }


  opt.isPEE = 1;

  TCanvas* c21 = new TCanvas("c2","The Canvas",200,10,540,546);
  c21->Draw();

  TLegend * leg11 = new TLegend(0.63,0.5,0.92,0.64,NULL,"brNDC");
  leg11->SetFillStyle(0); leg11->SetBorderSize(0); leg11->SetShadowColor(0);
  leg11->SetMargin(0.2);
  leg11->AddEntry(gfake1,"data","p");
  leg11->AddEntry(&hfake41,"prompt","l"); 
  leg11->AddEntry(&hfake311,"non-prompt","l"); 
  leg11->Draw("same");

  tframeSig->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
  tframeSig->GetXaxis()->CenterTitle(1);
  tframeSig->SetMinimum(0.5); 
  tframeSig->Draw();

  t->SetTextSize(0.05);
  t->DrawLatex(0.19,0.9,opt.cmspre);
  t->DrawLatex(0.19,0.83,opt.beamEn); 
  t->SetTextSize(0.04);
  t->DrawLatex(0.19,0.76,opt.lumi); 
  t->SetTextSize(0.035);
  t->DrawLatex(0.60,0.76,opt.rapString);
  t->DrawLatex(0.60,0.71,opt.ptString);
  if (opt.isPbPb != 0) t->DrawLatex(0.60,0.67,opt.centString);
  t->DrawLatex(0.60,0.62,opt.dphiString);

  double tframeMax = tframeSig->GetMaximum();
  tframeSig->SetMaximum(400); 
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timefit_woBkg_Lin.pdf";
  c21->SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange + "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timefit_woBkg_Lin.root";
  c21->SaveAs(titlestr.c_str());

  tframeSig->SetMaximum(tframeMax*9); 
  c21->SetLogy(1);
  c21->Update();
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange +  "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timefit_woBkg_Log.pdf";
  c21->SaveAs(titlestr.c_str());
  titlestr = opt.dirPre + "_rap" + opt.yrange + "_pT" + opt.prange +  "_cent" + opt.crange + "_dPhi" + opt.phirange + "_ntrk" + inOpt.ntrrange + "_ET" + inOpt.etrange + "_timefit_woBkg_Log.root";
  c21->SaveAs(titlestr.c_str());
  delete c21;
}

