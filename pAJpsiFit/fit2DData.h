#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

#include <TROOT.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraphErrors.h>
#include <TMath.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TRandom.h>

#include <RooFit.h>
#include <RooGlobalFunc.h>
#include <RooCategory.h>
#include <RooHistPdfConv.h>
#include <RooGenericPdf.h>
#include <RooFFTConvPdf.h>
#include <RooWorkspace.h>
#include <RooBinning.h>
#include <RooHistPdf.h>
#include <RooProdPdf.h>
#include <RooAddPdf.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooHist.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <RooConstVar.h>

using namespace std;
using namespace RooFit;

struct InputOpt {
  string FileName, FileName2, FileNameMC1, FileNameMC2;
  string mBkgFunct, mSigFunct;
	bool dataMerge;
  bool prefitMass;
  bool prefitSignalCTau;
  bool prefitBkg;
  bool fracfix;
  int isGG; //0:glb-glb 1:glb-trk 2:trk-trk ? not used now
  int isMB; //minBias
  int doWeight;
  int isMultiplicity; // 0: centrality, 1 : Ntracks, 2 : ET^{HF}
	string prange, lrange, yrange, crange, phirange, errrange;
	string etrange, ntrrange;
  string dirPre;
  string rpmethod;
  char cmspre[512], beamEn[512], lumi[512];
  char centString[512], rapString[512], ptString[512], dphiString[512];
	char etString[512], ntrkString[512];
  string psmin_latex, psmax_latex;
 
  bool analyticBlifetime;
  bool doBfit;
  bool narrowSideband;
  bool oneGaussianResol;
  bool fixResol2MC;
  bool usedPhi;
  int isPbPb; //0:pp, 1:PbPb, 2:Pbp 3:pPb - just for legend
  int isPEE;
  int is2Widths;
  int ctauBackground;

	int ctErrRange;
	string ctErrFile;

  double combinedWidth, combinedWidthErr; //CB + Gaus combined width/width error
  double PcombinedWidth, PcombinedWidthErr; //CB + Gaus combined width/width error, scaling for presentation
} inOpt;


// Global objects for drawing
TGraphErrors *gfake1;
TH1F hfake11, hfake21, hfake31, hfake311, hfake41;

// Input arguments, text parsing, formatting functions
void parseInputArg(int argc, char* argv[], InputOpt &opt);
void getOptRange(string &ran,float *min,float *max);
void formTitle(InputOpt &opt, float cmin, float cmax) ;
void formRapidity(InputOpt &opt, float ymin, float ymax) ;
void formPt(InputOpt &opt, float pmin, float pmax) ;
void formPhi(InputOpt &opt, float psmin, float psmax) ;
void formEt(InputOpt &opt, float etmin, float etmax) ;
void formNtrk(InputOpt &opt, float ntrmin, float ntrmax) ;

void getCtauErrRange(RooDataSet *data, InputOpt &opt, const char *reduceDSOrig, float lmin, float lmax, float *errmin, float *errmax);
int readCtauErrRange(InputOpt &opt, float *errmin, float *errmax);
//int readCtauErrRange(InputOpt &opt, float *errmin, float *errmax);

// Define essential fit functions
void setWSRange(RooWorkspace *ws, float lmin, float lmax, float errmin, float errmax);
RooBinning setCtBinning(float lmin,float lmax);
void defineMassBkg(RooWorkspace *ws);
void defineMassSig(RooWorkspace *ws, InputOpt &opt);
void getMCTrueLifetime(RooWorkspace *ws, RooDataSet *redMCCutNP, string titlestr, double lmax, InputOpt &opt);
void defineCTResol(RooWorkspace *ws, InputOpt &opt);
void defineCTBkg(RooWorkspace *ws, InputOpt &opt);
void defineCTSig(RooWorkspace *ws, RooDataSet *redMCCutNP, string titlestr, double lmax, InputOpt &opt);
RooDataHist* subtractSidebands(RooWorkspace* ws, RooDataHist* subtrData, RooDataHist* all, RooDataHist* side, float scalefactor, string varName);

// Drawing functions: Plotting
void ctauErrCutCheck(RooWorkspace *ws, RooDataSet *redData, RooDataSet *redData_2, RooDataSet *redMC, RooDataSet *redMC_2, RooDataSet *redMC2, RooDataSet *redMC2_2, InputOpt &opt) ;
void sidebandLeftRightCheck(RooWorkspace *ws, RooDataSet *redDataSBL, RooDataSet *redDataSBR, InputOpt &opt);
void drawMassPlotsWithoutB(RooWorkspace *ws, RooDataSet* redDataCut, RooFitResult *fitM, InputOpt &opt);
void ctauErrDistCheck(RooWorkspace *ws, RooDataHist *binDataCtErrSB, RooDataHist *binDataCtErrSIG, RooDataHist *subtrData, RooDataHist *weightedBkg, InputOpt &opt);
void ctauResolFitCheck(RooWorkspace *ws, bool fitMC, RooPlot *tframePR, InputOpt &opt);
//void drawCtauSBPlots(RooWorkspace *ws, RooDataSet *redDataSB, RooDataSet *redDataSBL, RooDataSet *redDataSBR, RooDataHist *binDataCtErr, RooFitResult *fitSB, RooFitResult *fitSBL, RooFitResult *fitSBR, InputOpt &opt) ;
void drawCtauSBPlots(RooWorkspace *ws, RooDataSet *redDataSB, RooDataSet *redDataSBL, RooDataSet *redDataSBR, RooDataHist *binDataCtErr, RooFitResult *fitSB, RooFitResult *fitSBL, RooFitResult *fitSBR, InputOpt &opt, double* UnNormChi2_side_t, int* nFitParam_side_t, int* nFullBinsPull_side_t, int* Dof_side_t,double* Chi2_side_t) ; // KYO 0316
//void drawMassPlotsWithB(RooWorkspace *ws, RooDataSet* redDataCut, float NSigNP_fin, float NBkg_fin, RooFitResult *fitM, InputOpt &opt);
void drawMassPlotsWithB(RooWorkspace *ws, RooDataSet* redDataCut, float NSigNP_fin, float NBkg_fin, RooFitResult *fitM, InputOpt &opt, double* UnNormChi2_mass_t, int* nFitParam_mass_t, int* nFullBinsPull_mass_t, int* Dof_mass_t,double* Chi2_mass_t); // KYO 0316
//void drawCtauFitPlots(RooWorkspace *ws, RooDataSet *redDataCut, RooDataHist* binDataCtErr, float NSigNP_fin, float NBkg_fin, RooFitResult *fit2D, InputOpt &opt) ;
void drawCtauFitPlots(RooWorkspace *ws, RooDataSet *redDataCut, RooDataHist* binDataCtErr, float NSigNP_fin, float NBkg_fin, RooFitResult *fit2D, InputOpt &opt, double* UnNormChi2_time_t, int* nFitParam_time_t, int* nFullBinsPull_time_t, int* Dof_time_t,double* Chi2_time_t); // KYO 0316
void drawMassCtau2DPlots(RooWorkspace *ws, InputOpt &opt) ;
void drawCtauFitPlotsSignals(RooWorkspace *ws, RooDataSet *redDataCut, RooDataSet *redDataSB, RooDataSet *redMCCutNP, float NSigPR_fin, float NSigNP_fin, InputOpt &opt);




/////////////////////////////////////////////////////////
//////////////////// Sub-routines ///////////////////////
/////////////////////////////////////////////////////////
void defineMassBkg(RooWorkspace *ws) {
  // 1st order polynomial
  ws->factory("Polynomial::polFunct(Jpsi_Mass,{coefPol1[-0.05,-5.,5.]})");
  // Exponential
  ws->factory("Exponential::expFunct(Jpsi_Mass,coefExp[-1.,-3.,1.])");

  return;
}

void defineMassSig(RooWorkspace *ws, InputOpt &opt) {
  //////// Candidates for signal
  // Normal gaussians
  if (opt.is2Widths == 1) {
//    ws->factory("Gaussian::signalG1(Jpsi_Mass,meanSig1[3.0975,3.05,3.15],sigmaSig1[0.1,0.02,0.2])");
  //  ws->factory("Gaussian::signalG1(Jpsi_Mass,meanSig1[3.0975,3.05,3.15],sigmaSig1[0.02,0.005,0.15])");
  	  //ws->factory("Gaussian::signalG1(Jpsi_Mass,meanSig1[3.0975,3.05,3.15],sigmaSig1[0.02,0.01,0.15])");
  	  ws->factory("Gaussian::signalG1(Jpsi_Mass,meanSig1[3.0975,3.05,3.15],sigmaSig1[0.03,0.008,0.075])"); // KYO test02
  } else if (opt.is2Widths == 0) {
    ws->factory("Gaussian::signalG1(Jpsi_Mass,meanSig1[3.0975,3.05,3.15],sigmaSig1[0.02,0.008,0.2])");
  }

  // Crystall Ball
	// narrow enne
  //ws->factory("CBShape::signalCB(Jpsi_Mass,meanSig1,sigmaSig1,alpha[1.,0.,3.],enne[5.,1.,30.])");
  ws->factory("CBShape::signalCB(Jpsi_Mass,meanSig1,sigmaSig1,alpha[1.9,1.,3.],enne[5.,1.,30.])");//test02
 	//ws->factory("CBShape::signalCB2(Jpsi_Mass,meanSig1,sigmaSig2[0.03,0.01,0.06],alpha,enne)");
 	ws->factory("CBShape::signalCB2(Jpsi_Mass,meanSig1,sigmaSig2[0.03,0.008,0.075],alpha,enne)");//test02
  //ws->factory("CBShape::signalCBWN(Jpsi_Mass,meanSig1,sigmaSig1,alpha,enneW[5.,1.,50.])");
	// wide enneW
  //ws->factory("CBShape::signalCBWN(Jpsi_Mass,meanSig1,sigmaSig1,alpha,enneW[5.,1.,70.])");
  ws->factory("CBShape::signalCBWN(Jpsi_Mass,meanSig1,sigmaSig1,alpha,enneW[2.5,1.0,4.0])");
  ws->factory("CBShape::signalCB2WN(Jpsi_Mass,meanSig1,sigmaSig2,alpha,enneW)");
  ws->factory("CBShape::signalCB3WN(Jpsi_Mass,meanSig1,sigmaSig3[0.03,0.01,0.2],alpha,enneW)");

  //////// Sum of signal functions
  // Sum of gaussian 1 and a crystall ball
  if (opt.is2Widths == 1) {
//    ws->factory("SUM::sigCBG1(coeffGaus[0.1,0.0,1.0]*signalG1,signalCB)"); //Mihee
//    ws->factory("SUM::sigCBG1(coeffGaus[0.1,0.07,0.93]*signalG1,signalCB)");
    ws->factory("SUM::sigCBG1(coeffGaus[0.5,0.11,0.89]*signalG1,signalCB)"); //test06
  } else if (opt.is2Widths == 0) {
//    ws->factory("SUM::sigCBG1(coeffGaus[0.1,0.05,1.]*signalG1,signalCB)");
//    ws->factory("SUM::sigCBG1(coeffGaus[0.1,0.07,0.93]*signalG1,signalCB)");
    ws->factory("SUM::sigCBG1(coeffGaus[0.5,0.11,0.89]*signalG1,signalCB)"); //test06
  }
  // Sum of gaussian sigmaSig1 and crystall ball sigmaSig2
  ws->factory("SUM::sigCB2G1(coeffGaus*signalG1,signalCB2)");
  // Sum of gaussian sigmaSig1 and crystall ball sigmaSig1 with wide n
  ws->factory("SUM::sigCBWNG1(coeffGaus*signalG1,signalCBWN)");
  // Sum of gaussian sigmaSig1 and crystall ball sigmaSig2 with wide n
  ws->factory("SUM::sigCB2WNG1(coeffGaus*signalG1,signalCB2WN)");
  return;
}

RooDataHist* subtractSidebands(RooWorkspace* ws, RooDataHist* subtrData, RooDataHist* all, RooDataHist* side, float scalefactor, string varName = "Jpsi_CtErr") {
  const RooArgSet* aRow;
  const RooArgSet* aRowS;
 
  if (all->numEntries() != side->numEntries()) {
    cout << "ERROR subtractSidebands : different binning!" << endl;
    return 0;
  }

  RooDataHist* scaledBkg = new RooDataHist("scaledBkg","Scale applied sideband data",RooArgSet(*(ws->var(varName.c_str())))); 
  RooDataHist* weightedBkg = new RooDataHist("weightedBkg","weighting applied sideband data",RooArgSet(*(ws->var(varName.c_str())))); 

  for (Int_t i=0; i<all->numEntries(); i++) {
    aRow = all->get(i);
    aRowS = side->get(i);
    RooRealVar* thisVar = (RooRealVar*)aRow->find(varName.c_str());
    ws->var(varName.c_str())->setVal(thisVar->getVal());
    float wBkg = side->weight(*aRowS,0,false);
    if (wBkg <= 0.1) wBkg = 0.1;
    float sfBkg = scalefactor*side->weight(*aRowS,0,false);
    float newWeight = all->weight(*aRow,0,false) - scalefactor*side->weight(*aRowS,0,false);
    if (newWeight <= 0.1) newWeight = 0.1;
    subtrData->add(RooArgSet(*(ws->var(varName.c_str()))),newWeight);
    scaledBkg->add(RooArgSet(*(ws->var(varName.c_str()))),sfBkg);
    weightedBkg->add(RooArgSet(*(ws->var(varName.c_str()))),wBkg);
  }

//  return scaledBkg;
  return weightedBkg;
}

void defineCTResol(RooWorkspace *ws, InputOpt &opt) {
  if (opt.isPEE == 1) {
    if (opt.oneGaussianResol) {
      ws->factory("GaussModel::sigPR(Jpsi_Ct,meanResSigW[0.,-0.01,0.01],sigmaResSigN[0.8,0.6,2.0],one[1.0],Jpsi_CtErr)");
    } else {
//      ws->factory("GaussModel::resGW(Jpsi_Ct,meanResSigW[0.,-0.02,0.03],sigmaResSigW[2.3,1.3,3.5],one[1.0],Jpsi_CtErr)");
//      ws->factory("GaussModel::resGN(Jpsi_Ct,meanResSigW[0.,-0.01,0.01],sigmaResSigN[0.8,0.6,1.1],one,Jpsi_CtErr)");
//      ws->factory("GaussModel::resGW(Jpsi_Ct,meanResSigW[0.,-0.01,0.01],sigmaResSigW[2.3,1.3,3.5],one[1.0],Jpsi_CtErr)");
      ws->factory("GaussModel::resGW(Jpsi_Ct,meanResSigW[0.,-0.01,0.01],sigmaResSigW[2.3,1.2,3.0],one[1.0],Jpsi_CtErr)");
     // ws->factory("GaussModel::resGN(Jpsi_Ct,meanResSigW,sigmaResSigN[0.8,0.6,1.1],one,Jpsi_CtErr)");
     //KYO
		  ws->factory("GaussModel::resGN(Jpsi_Ct,meanResSigN[0.,-0.01,0.01],sigmaResSigN[0.8,0.6,1.1],one,Jpsi_CtErr)");
      //ws->factory("AddModel::sigPR({resGW,resGN},{fracRes[0.05,0.001,0.3]})");
      ws->factory("AddModel::sigPR({resGW,resGN},{fracRes[0.05,0.005,0.4]})");
    }
  } else if (opt.isPEE == 0) {
//    ws->factory("GaussModel::resGW(Jpsi_Ct,meanResSigW[0.,-0.1,0.1],sigmaResSigW[0.07,0.01,0.9])");
    ws->factory("GaussModel::resGW(Jpsi_Ct,meanResSigW[0.,-0.1,0.1],sigmaResSigW[0.07,0.01,0.9])");
    ws->factory("GaussModel::resGN(Jpsi_Ct,meanResSigW,sigmaResSigN[0.04,0.01,0.3])");
    ws->factory("GaussModel::resGO(Jpsi_Ct,meanResSigW,sigmaResSigO[0.2,0.05,1.0])");
    ws->factory("GaussModel::resGM(Jpsi_Ct,meanResSigW,sigmaResSigM[0.4,0.04,2.0])");
    ws->factory("AddModel::sigPR({resGW,resGO,resGM,resGN},{fracRes[0.2,0.01,0.9],fracRes2[0.02,0.0,0.25],fracRes3[0.1,0.0,0.5]})");
  }

  return;
}

void defineCTBkg(RooWorkspace *ws, InputOpt &opt) {
  if (opt.ctauBackground == 0) {
    ws->factory("Decay::bkg2(Jpsi_Ct,lambdap[0.42,0.05,1.5],sigPR,RooDecay::SingleSided)");
    ws->factory("Decay::bkg3(Jpsi_Ct,lambdam[0.79,0.02,1.5],sigPR,RooDecay::Flipped)");
    ws->factory("Decay::bkg4(Jpsi_Ct,lambdasym[0.69,0.02,5.0],sigPR,RooDecay::DoubleSided)");
//    ws->factory("Decay::bkg2(Jpsi_Ct,lambdap[0.42,0.05,1.0],sigPR,RooDecay::SingleSided)");
//    ws->factory("Decay::bkg3(Jpsi_Ct,lambdam[0.05,0.005,0.5],sigPR,RooDecay::Flipped)");
//    ws->factory("Decay::bkg4(Jpsi_Ct,lambdasym[0.1,0.001,1.0],sigPR,RooDecay::DoubleSided)");
    //ws->factory("Decay::bkg5(Jpsi_Ct,lambdap2[2.0,1.0,5.5],sigPR,RooDecay::SingleSided)");

/*    ws->factory("SUM::bkgPart1(fpm[1.0,0.0,1.0]*bkg2,bkg5)");
    ws->factory("SUM::bkgPart2(fpm2[0.9,0.0,1.0]*bkgPart1,bkg3)");
    ws->factory("SUM::bkgPart3(fLiving[0.9,0.0,1.0]*bkgPart2,bkg4)");
    ws->factory("SUM::bkgCtTot(fbkgCtTot[0.29,0.0,1.0]*sigPR,bkgPart2)"); */

//    ws->factory("SUM::bkgPart1(fpm[1.0,0.0,1.0]*bkg2,bkg3)");
    ws->factory("SUM::bkgPart1(fpm[0.9,0.0,1.0]*bkg2,bkg3)");
    ws->factory("SUM::bkgPart2(fLiving[0.9,0.0,1.0]*bkgPart1,bkg4)");
    ws->factory("SUM::bkgCtTot(fbkgCtTot[0.29,0.0,1.0]*sigPR,bkgPart2)");
    //ws->factory("SUM::bkgCtTot(fbkgCtTot[0.29,0.01,1.0]*sigPR,bkgPart2)");
  
  } else if (opt.ctauBackground == 1 || opt.ctauBackground == 2) {
    ws->factory("Decay::bkg2L(Jpsi_Ct,lambdapL[0.42,0.05,1.5],sigPR,RooDecay::SingleSided)");
    ws->factory("Decay::bkg3L(Jpsi_Ct,lambdamL[0.79,0.02,1.5],sigPR,RooDecay::Flipped)");
    ws->factory("Decay::bkg4L(Jpsi_Ct,lambdasymL[0.69,0.02,5.0],sigPR,RooDecay::DoubleSided)");

    ws->factory("SUM::bkgPart1L(fpmL[1.0,0.0,1.0]*bkg2L,bkg3L)");
    ws->factory("SUM::bkgPart2L(fLivingL[0.9,0.0,1.0]*bkgPart1L,bkg4L)");
    ws->factory("SUM::bkgCtTotL(fbkgCtTotLeft[0.29,0.0,1.0]*sigPR,bkgPart2L)");

    ws->factory("Decay::bkg2R(Jpsi_Ct,lambdapR[0.42,0.05,1.5],sigPR,RooDecay::SingleSided)");
    ws->factory("Decay::bkg3R(Jpsi_Ct,lambdamR[0.79,0.02,1.5],sigPR,RooDecay::Flipped)");
    ws->factory("Decay::bkg4R(Jpsi_Ct,lambdasymR[0.69,0.02,5.0],sigPR,RooDecay::DoubleSided)");

    ws->factory("SUM::bkgPart1R(fpmR[1.0,0.0,1.0]*bkg2R,bkg3R)");
    ws->factory("SUM::bkgPart2R(fLivingR[0.9,0.0,1.0]*bkgPart1R,bkg4R)");
    ws->factory("SUM::bkgCtTotR(fbkgCtTotRight[0.29,0.0,1.0]*sigPR,bkgPart2R)");

    ws->factory("SUM::bkgCtTot(fbkgCtTotR[0.35,0.0,1.0]*bkgCtTotR,bkgCtTotL)");
  }

  return;
}

void getMCTrueLifetime(RooWorkspace *ws, RooDataSet *redMCCutNP, string titlestr, double lmax, InputOpt &opt) {
  ws->pdf("bMCTrue")->fitTo(*redMCCutNP,Minos(0),SumW2Error(kTRUE),NumCPU(8));

  if (opt.isPbPb == 1) {
    ws->var("Gmc")->setConstant(kTRUE);
    ws->var("cutx")->setConstant(kTRUE);
    ws->var("bTau1")->setConstant(kTRUE);
    ws->var("bTau2")->setConstant(kTRUE);

    // *** Draw MC true Lifetime fit
    RooPlot *trueframef = ws->var("Jpsi_CtTrue")->frame(Bins(150));
    redMCCutNP->plotOn(trueframef);
    ws->pdf("bMCTrue")->plotOn(trueframef,LineColor(kBlue),Normalization(redMCCutNP->sumEntries(),RooAbsReal::NumEvent));

    char titlestr_lin[512];

    TLatex t;
    t.SetNDC(); t.SetTextAlign(22); t.SetTextSize(0.035);

    TCanvas c0f;

    c0f.cd(); c0f.SetLogy(0); trueframef->Draw();
    sprintf(titlestr_lin,"bTau1: %.2f #pm %.2f",ws->var("bTau1")->getVal(),ws->var("bTau1")->getError());
    t.DrawLatex(0.76,0.90,titlestr_lin);
    sprintf(titlestr_lin,"bTau2: %.2f #pm %.2f",ws->var("bTau2")->getVal(),ws->var("bTau2")->getError());
    t.DrawLatex(0.76,0.85,titlestr_lin);
    sprintf(titlestr_lin,"cutx: %.2f #pm %.2f",ws->var("cutx")->getVal(),ws->var("cutx")->getError());
    t.DrawLatex(0.76,0.80,titlestr_lin);
    sprintf(titlestr_lin,"Gmc: %.1e #pm %.1e",ws->var("Gmc")->getVal(),ws->var("Gmc")->getError());
    t.DrawLatex(0.76,0.75,titlestr_lin);
    sprintf(titlestr_lin,"%s_Lin.pdf",titlestr.c_str());
    c0f.SaveAs(titlestr_lin);

    c0f.Clear(); c0f.SetLogy(1); trueframef->Draw();
    sprintf(titlestr_lin,"bTau1: %.2f #pm %.2f",ws->var("bTau1")->getVal(),ws->var("bTau1")->getError());
    t.DrawLatex(0.76,0.90,titlestr_lin);
    sprintf(titlestr_lin,"bTau2: %.2f #pm %.2f",ws->var("bTau2")->getVal(),ws->var("bTau2")->getError());
    t.DrawLatex(0.76,0.85,titlestr_lin);
    sprintf(titlestr_lin,"cutx: %.2f #pm %.2f",ws->var("cutx")->getVal(),ws->var("cutx")->getError());
    t.DrawLatex(0.76,0.80,titlestr_lin);
    sprintf(titlestr_lin,"Gmc: %.1e #pm %.1e",ws->var("Gmc")->getVal(),ws->var("Gmc")->getError());
    t.DrawLatex(0.76,0.75,titlestr_lin);
    sprintf(titlestr_lin,"%s_Log.pdf",titlestr.c_str());
    c0f.SaveAs(titlestr_lin);

    delete trueframef;
  } else {
    // *** Draw MC true Lifetime fit
/*    ws->var("Gmc")->setConstant(kTRUE);
    ws->var("cutx")->setConstant(kTRUE);
    ws->var("bTau1")->setConstant(kTRUE);
    ws->var("bTau2")->setConstant(kTRUE);

    RooPlot *trueframef = ws->var("Jpsi_CtTrue")->frame(Bins(150));
    redMCCutNP->plotOn(trueframef);
    ws->pdf("bMCTrue")->plotOn(trueframef,LineColor(kBlue),Normalization(redMCCutNP->sumEntries(),RooAbsReal::NumEvent));

    char titlestr_lin[512];

    TLatex t;
    t.SetNDC(); t.SetTextAlign(22); t.SetTextSize(0.035);

    TCanvas c0f;

    c0f.cd(); c0f.SetLogy(0); trueframef->Draw();
    sprintf(titlestr_lin,"bTau1: %.2f #pm %.2f",ws->var("bTau1")->getVal(),ws->var("bTau1")->getError());
    t.DrawLatex(0.76,0.90,titlestr_lin);
    sprintf(titlestr_lin,"bTau2: %.2f #pm %.2f",ws->var("bTau2")->getVal(),ws->var("bTau2")->getError());
    t.DrawLatex(0.76,0.85,titlestr_lin);
    sprintf(titlestr_lin,"cutx: %.2f #pm %.2f",ws->var("cutx")->getVal(),ws->var("cutx")->getError());
    t.DrawLatex(0.76,0.80,titlestr_lin);
    sprintf(titlestr_lin,"Gmc: %.1e #pm %.1e",ws->var("Gmc")->getVal(),ws->var("Gmc")->getError());
    t.DrawLatex(0.76,0.75,titlestr_lin);
    sprintf(titlestr_lin,"%s_Lin.pdf",titlestr.c_str());
    c0f.SaveAs(titlestr_lin);

    c0f.Clear(); c0f.SetLogy(1); trueframef->Draw();
    sprintf(titlestr_lin,"bTau1: %.2f #pm %.2f",ws->var("bTau1")->getVal(),ws->var("bTau1")->getError());
    t.DrawLatex(0.76,0.90,titlestr_lin);
    sprintf(titlestr_lin,"bTau2: %.2f #pm %.2f",ws->var("bTau2")->getVal(),ws->var("bTau2")->getError());
    t.DrawLatex(0.76,0.85,titlestr_lin);
    sprintf(titlestr_lin,"cutx: %.2f #pm %.2f",ws->var("cutx")->getVal(),ws->var("cutx")->getError());
    t.DrawLatex(0.76,0.80,titlestr_lin);
    sprintf(titlestr_lin,"Gmc: %.1e #pm %.1e",ws->var("Gmc")->getVal(),ws->var("Gmc")->getError());
    t.DrawLatex(0.76,0.75,titlestr_lin);
    sprintf(titlestr_lin,"%s_Log.pdf",titlestr.c_str());
    c0f.SaveAs(titlestr_lin);

    delete trueframef;
*/
    // *** Draw MC true Lifetime fit
    RooPlot *trueframef = ws->var("Jpsi_CtTrue")->frame(Bins(150));
    redMCCutNP->plotOn(trueframef);
    ws->pdf("bMCTrue")->plotOn(trueframef,LineColor(kBlue),Normalization(redMCCutNP->sumEntries(),RooAbsReal::NumEvent));

    char titlestr_lin[512];

    TLatex t;
    t.SetNDC(); t.SetTextAlign(22); t.SetTextSize(0.035);

    TCanvas c0f;

    c0f.cd(); c0f.SetLogy(0); trueframef->Draw();
    sprintf(titlestr_lin,"bTau: %.2f #pm %.2f",ws->var("bTau")->getVal(),ws->var("bTau")->getError());
    t.DrawLatex(0.76,0.85,titlestr_lin);
    sprintf(titlestr_lin,"Gmc: %.1e #pm %.1e",ws->var("Gmc")->getVal(),ws->var("Gmc")->getError());
    t.DrawLatex(0.76,0.75,titlestr_lin);
    sprintf(titlestr_lin,"%s_Lin.pdf",titlestr.c_str());
    c0f.SaveAs(titlestr_lin);

    c0f.Clear(); c0f.SetLogy(1); trueframef->Draw();
    sprintf(titlestr_lin,"bTau: %.2f #pm %.2f",ws->var("bTau")->getVal(),ws->var("bTau")->getError());
    t.DrawLatex(0.76,0.85,titlestr_lin);
    sprintf(titlestr_lin,"Gmc: %.1e #pm %.1e",ws->var("Gmc")->getVal(),ws->var("Gmc")->getError());
    t.DrawLatex(0.76,0.75,titlestr_lin);
    sprintf(titlestr_lin,"%s_Log.pdf",titlestr.c_str());
    c0f.SaveAs(titlestr_lin);

    delete trueframef;

    ws->var("Gmc")->setConstant(kTRUE);
  }

  return ;
}

void defineCTSig(RooWorkspace *ws, RooDataSet *redMCCutNP, string titlestr, double lmax, InputOpt &opt) {
  RooDataHist* binMCCutNP = new RooDataHist("binMCCutNP","MC distribution for NP signal",RooArgSet(*(ws->var("Jpsi_CtTrue"))),*redMCCutNP);
  
  if (opt.isPEE == 0) {
    RooHistPdfConv sigNPW("sigNPW","Non-prompt signal with wide gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW")),*(ws->var("sigmaResSigW")),*binMCCutNP ); ws->import(sigNPW);
    RooHistPdfConv sigNPO("sigNPO","Non-prompt signal with outstanding gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW")),*(ws->var("sigmaResSigO")),*binMCCutNP ); ws->import(sigNPO);
    RooHistPdfConv sigNPM("sigNPM","Non-prompt signal with mastodontic gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW")),*(ws->var("sigmaResSigM")),*binMCCutNP ); ws->import(sigNPM);
    RooHistPdfConv sigNPN("sigNPN","Non-prompt signal with narrow gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW")),*(ws->var("sigmaResSigN")),*binMCCutNP ); ws->import(sigNPN);

    RooAddPdf sigNP("sigNP","Non-prompt signal",RooArgSet(*(ws->pdf("sigNPW")),*(ws->pdf("sigNPO")),*(ws->pdf("sigNPM")),*(ws->pdf("sigNPN"))),RooArgSet(*(ws->var("fracRes")),*(ws->var("fracRes2")),*(ws->var("fracRes3")))); ws->import(sigNP);

  } else if (opt.isPEE == 1) {  
    if (opt.analyticBlifetime) {
      if (opt.isPbPb == 1) {
        RooRealVar cutx("cutx","cutx",0.353,0.25,0.45);  ws->import(cutx); //Region below than this cut will use bTau1, above than it will use bTau2. determined by free fit.
//        RooRealVar cutx("cutx1","cutx1",0.353,0.25,0.45);  ws->import(cutx); //Region below than this cut will use bTau1, above than it will use bTau2. determined by free fit.
//        RooRealVar cutx("cutx2","cutx2",1.10,1.50,0.90);  ws->import(cutx); //Region below than this cut will use bTau1, above than it will use bTau2. determined by free fit.
        RooRealVar bTau1("bTau1","bTau1",-4.0,-4.5,-1.0); ws->import(bTau1);
        RooRealVar bTau2("bTau2","bTau2",-5.8,-8.0,-4.5); ws->import(bTau2);
//        RooRealVar bTau3("bTau3","bTau3",-0.8,-5.0,-0.1); ws->import(bTau3);
        RooGenericPdf npTrue("npTrue","npTrue","( 0.0*(@0<=0.0) + exp(@0*@1)*(@0>0 && @0<@3) + exp(@0*@2 - @3*(@2-@1))*(@0>=@3) )", RooArgList(*(ws->var("Jpsi_CtTrue")),bTau1,bTau2,cutx)); ws->import(npTrue);
//        ws->factory("GaussModel::bresGTrue(Jpsi_CtTrue,mean[0.0],Gmc[0.002,0.00001,0.02])");
        ws->factory("GaussModel::bresGTrue(Jpsi_CtTrue,mean[0.0],Gmc[0.00002,0.000005,0.0001])");
        ws->factory("FCONV::bMCTrue(Jpsi_CtTrue,npTrue,bresGTrue)");

        getMCTrueLifetime(ws, redMCCutNP, titlestr, lmax, opt);
       
			 	// KYO gmc -> Gmc 
        //float GmcVal = ws->var("Gmc")->getVal();
        //RooRealVar gmc("gmc","Sigma of MC Gaussian",GmcVal);  ws->import(gmc);
        float bTauVal1 = ws->var("bTau1")->getVal();
        RooRealVar btauFix1("btauFix1","Slope of MC exponential",bTauVal1);   ws->import(btauFix1);
        float bTauVal2 = ws->var("bTau2")->getVal();
        RooRealVar btauFix2("btauFix2","Slope of MC exponential",bTauVal2);   ws->import(btauFix2);

        //RooFormulaVar bResSigN("bResSigN", "sqrt((@0*@1)**2+(@2)**2)", RooArgList(*(ws->var("sigmaResSigN")), *(ws->var("Jpsi_CtErr")),gmc));  ws->import(bResSigN);
        RooFormulaVar bResSigN("bResSigN", "sqrt((@0*@1)**2+(@2)**2)", RooArgList(*(ws->var("sigmaResSigN")), *(ws->var("Jpsi_CtErr")), *(ws->var("Gmc"))));  ws->import(bResSigN);
        if (opt.oneGaussianResol) {
          ws->factory("GaussModel::bresG(Jpsi_Ct,meanResSigW,bResSigN)");
        } else {
          //RooFormulaVar bResSigW("bResSigW", "sqrt((@0*@1)**2+(@2)**2)", RooArgList(*(ws->var("sigmaResSigW")), *(ws->var("Jpsi_CtErr")),gmc));  ws->import(bResSigW);
          RooFormulaVar bResSigW("bResSigW", "sqrt((@0*@1)**2+(@2)**2)", RooArgList(*(ws->var("sigmaResSigW")), *(ws->var("Jpsi_CtErr")), *(ws->var("Gmc"))));  ws->import(bResSigW);
          ws->factory("GaussModel::bresGN(Jpsi_Ct,meanResSigW,bResSigN)");
          ws->factory("GaussModel::bresGW(Jpsi_Ct,meanResSigW,bResSigW)");
          ws->factory("AddModel::bresG({bresGW,bresGN},{fracRes})");
        }
        
        RooGenericPdf npReal("npReal","npReal","( 0.0*(@0<=0.0) + exp(@0*@1)*(@0>0 && @0<@3) + exp(@0*@2 - @3*(@2-@1))*(@0>=@3) )", RooArgList(*(ws->var("Jpsi_Ct")),bTau1,bTau2,cutx)); ws->import(npReal);
        ws->factory("FCONV::sigNP(Jpsi_Ct,npReal,bresG)");
      } else {  // for pp && pPb
/*        RooRealVar cutx("cutx","cutx",1.8,1.4,2.5);  ws->import(cutx); //Region below than this cut will use bTau1, above than it will use bTau2. determined by free fit.
        RooRealVar bTau1("bTau1","bTau1",-5.8,-8.0,-4.5); ws->import(bTau1);
        RooRealVar bTau2("bTau2","bTau2",-4.0,-4.5,-1.0); ws->import(bTau2);
        RooGenericPdf npTrue("npTrue","npTrue","( 0.0*(@0<=0.0) + exp(@0*@1)*(@0>0 && @0<@3) + exp(@0*@2 - @3*(@2-@1))*(@0>=@3) )", RooArgList(*(ws->var("Jpsi_CtTrue")),bTau1,bTau2,cutx)); ws->import(npTrue);
        ws->factory("GaussModel::bresGTrue(Jpsi_CtTrue,mean[0.0],Gmc[0.002,0.00001,0.02])");
        ws->factory("FCONV::bMCTrue(Jpsi_CtTrue,npTrue,bresGTrue)");

        getMCTrueLifetime(ws, redMCCutNP, titlestr, lmax, opt);
        
        float GmcVal = ws->var("Gmc")->getVal();
        RooRealVar gmc("gmc","Sigma of MC Gaussian",GmcVal);  ws->import(gmc);
        float bTauVal1 = ws->var("bTau1")->getVal();
        RooRealVar btauFix1("btauFix1","Slope of MC exponential",bTauVal1);   ws->import(btauFix1);
        float bTauVal2 = ws->var("bTau2")->getVal();
        RooRealVar btauFix2("btauFix2","Slope of MC exponential",bTauVal2);   ws->import(btauFix2);

        RooFormulaVar bResSigN("bResSigN", "sqrt((@0*@1)**2+(@2)**2)", RooArgList(*(ws->var("sigmaResSigN")), *(ws->var("Jpsi_CtErr")),gmc));  ws->import(bResSigN);
        if (opt.oneGaussianResol) {
          ws->factory("GaussModel::bresG(Jpsi_Ct,meanResSigW,bResSigN)");
        } else {
          RooFormulaVar bResSigW("bResSigW", "sqrt((@0*@1)**2+(@2)**2)", RooArgList(*(ws->var("sigmaResSigW")), *(ws->var("Jpsi_CtErr")),gmc));  ws->import(bResSigW);
          ws->factory("GaussModel::bresGN(Jpsi_Ct,meanResSigW,bResSigN)");
          ws->factory("GaussModel::bresGW(Jpsi_Ct,meanResSigW,bResSigW)");
          ws->factory("AddModel::bresG({bresGW,bresGN},{fracRes})");
        }
        
        RooGenericPdf npReal("npReal","npReal","( 0.0*(@0<=0.0) + exp(@0*@1)*(@0>0 && @0<@3) + exp(@0*@2 - @3*(@2-@1))*(@0>=@3) )", RooArgList(*(ws->var("Jpsi_Ct")),bTau1,bTau2,cutx)); ws->import(npReal);
        ws->factory("FCONV::sigNP(Jpsi_Ct,npReal,bresG)");
*/        
        // for 1 exponential slope
        ws->factory("GaussModel::bresGTrue(Jpsi_CtTrue,mean[0.0],Gmc[0.002,0.00001,0.02])");
        ws->factory("Decay::bMCTrue(Jpsi_CtTrue,bTau[0.04,0.01,1.0],bresGTrue,RooDecay::SingleSided)");

        getMCTrueLifetime(ws, redMCCutNP, titlestr, lmax, opt);
        
//        float GmcVal = ws->var("Gmc")->getVal();
//        RooRealVar gmc("gmc","Sigma of MC Gaussian",GmcVal);  ws->import(gmc);
        float bTauVal = ws->var("bTau")->getVal();
        RooRealVar btauFix("btauFix","Slope of MC exponential",bTauVal);   ws->import(btauFix);

//        RooFormulaVar bResSigN("bResSigN", "sqrt((@0*@1)**2+(@2)**2)", RooArgList(*(ws->var("sigmaResSigN")), *(ws->var("Jpsi_CtErr")),gmc));  ws->import(bResSigN);
        RooFormulaVar bResSigN("bResSigN", "sqrt((@0*@1)**2+(@2)**2)", RooArgList(*(ws->var("sigmaResSigN")), *(ws->var("Jpsi_CtErr")),*(ws->var("Gmc"))));  ws->import(bResSigN);
        if (opt.oneGaussianResol) {
          ws->factory("GaussModel::bresG(Jpsi_Ct,meanResSigW,bResSigN)");
        } else {
//          RooFormulaVar bResSigW("bResSigW", "sqrt((@0*@1)**2+(@2)**2)", RooArgList(*(ws->var("sigmaResSigW")), *(ws->var("Jpsi_CtErr")),gmc));  ws->import(bResSigW);
          RooFormulaVar bResSigW("bResSigW", "sqrt((@0*@1)**2+(@2)**2)", RooArgList(*(ws->var("sigmaResSigW")), *(ws->var("Jpsi_CtErr")), *(ws->var("Gmc"))));  ws->import(bResSigW);
          ws->factory("GaussModel::bresGN(Jpsi_Ct,meanResSigW,bResSigN)");
          ws->factory("GaussModel::bresGW(Jpsi_Ct,meanResSigW,bResSigW)");
          ws->factory("AddModel::bresG({bresGW,bresGN},{fracRes})");
        }
        ws->factory("Decay::sigNP(Jpsi_Ct,bTau,bresG,RooDecay::SingleSided)");
      }
    // end of analytic B 
		} else {
      if (opt.oneGaussianResol) {
        RooHistPdfConv sigNP("sigNP","Non-prompt signal with narrow gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW")),*(ws->var("sigmaResSigN")),*(ws->var("one")),*(ws->var("Jpsi_CtErr")),*binMCCutNP);  ws->import(sigNP);
      } else {
        RooHistPdfConv sigNPW("sigNPW","Non-prompt signal with wide gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW")),*(ws->var("sigmaResSigW")),*(ws->var("one")),*(ws->var("Jpsi_CtErr")),*binMCCutNP);  ws->import(sigNPW);
        RooHistPdfConv sigNPN("sigNPN","Non-prompt signal with narrow gaussian",*(ws->var("Jpsi_Ct")),*(ws->var("meanResSigW")),*(ws->var("sigmaResSigN")),*(ws->var("one")),*(ws->var("Jpsi_CtErr")),*binMCCutNP);  ws->import(sigNPN);
        RooAddPdf sigNP("sigNP","Non-prompt signal",RooArgSet(*(ws->pdf("sigNPW")),*(ws->pdf("sigNPN"))),RooArgSet(*(ws->var("fracRes"))));  ws->import(sigNP); 
      }
    }

  }// end of isPEE == 1
  
  return;
}

