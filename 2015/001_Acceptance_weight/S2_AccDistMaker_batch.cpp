// modified by Yeonju
// for Acceptance systematic study
// pt-y 2D differential correction
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>
#include <string>
#include <math.h>

#include <TROOT.h>
#include "TSystem.h"
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TMath.h>
#include <math.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "TClonesArray.h"
#include <TAxis.h>
#include <cmath>
#include <TLorentzRotation.h>

#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TGraphErrors.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLine.h>
#include <TAxis.h>

#include <TRandom.h>
#include <TStopwatch.h>
#include <ctime>
//#include "hiForest.h"
//#include "CutAndBinCollection2012.h"
using namespace std;

const double shiftvar = -0.47; // conversion constant y=0(collision)==y=-0.47(LAB frame)  

//bool dimuCut(int, int lv_dmom0_Id, int lv_dgmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch);
//bool kineCut(bool, double muPt, double muEta, double muP);
bool dimuCut(int lv_dmom0_Id, int lv_dgmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch);
bool kineCut(double muPt, double muEta, double muP);
bool massCut(double lv_dimu_mass);

void jumSun(Double_t x1=0,Double_t y1=0,Double_t x2=1,Double_t y2=1,Int_t color=1, Double_t width=1);
void onSun(Double_t x1=0,Double_t y1=0,Double_t x2=1,Double_t y2=1,Int_t color=1, Double_t width=1);

int main (int argc, char* argv[]){
//void S2_AccDistMaker_batch(bool isPrompt=true, bool isPbp=true){
//void S2_AccDistMaker(char *strBinnig = "8rap9pt", bool isPrompt=true, bool isPbp=true){
    gRandom->SetSeed(time(0));
    //gRandom->SetSeed(0);
    cout << "time(0) : " << time(0) << endl;
    TStopwatch timer;
    timer.Start();
//    float begin_rTime = timer.RealTime();   
//    cout << "begin time : " << begin_rTime << endl;
    TH1::SetDefaultSumw2();

    //gROOT->Macro("rootlogon.C+");
	gStyle->SetCanvasDefW(800);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Argument Setting

        bool isPrompt=0;
        bool isPbp=0;
        int fileNum=0;
	if (argc==1) {
                cout << "select option ( Usage : ./Executable isPrompt is1stRun)" << endl;
                return 0;
        }
	cout << "argv[1] : " << argv[1] <<  ", argv[2] : " << argv[2] << ", argv[3] : " << argv[3] << endl; 
        isPrompt =  atoi(argv[1]);
        isPbp =  atoi(argv[2]);
        fileNum =  atoi(argv[3]);

	cout << "isPrompt : " << isPrompt << ", isPbp : " << isPbp << ", fileNum : " << fileNum << endl; 
	char* sampleName;
	double minylab =-2.4;
	double maxylab=2.4;
	double minpt=0.0;
	double maxpt=30.0;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // read-in tree

	TChain * ana1 = new TChain("DiAna");// Rapidity is in LAB frame in default
	if (isPrompt){
		//sampleName="PRMC_boosted";
//		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_JPsiWithFSR_5TeV02_1st_KYOvtx_ntuple_20150126.root");
//		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_JPsiWithFSR_5TeV02_1st_KYOvtx_ntuple_20150126_p2.root");
//		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140210.root"); //from Hyunchul
//		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140211.root"); //from Hyunchul
		ana1->Add("root://eoscms//eos/cms/store/caf/user/lamia/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140210.root"); //from Hyunchul
		ana1->Add("root://eoscms//eos/cms/store/caf/user/lamia/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140211.root"); //from Hyunchul
	}
	else {
		//sampleName="NPMC_boosted";
//		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_inclBtoJPsiMuMu_5TeV02_1st_KYOvtx_ntuple_20150127.root");
//		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_inclBtoJPsiMuMu_5TeV02_1st_KYOvtx_ntuple_20150127_p2.root");
		ana1->Add("root://eoscms//eos/cms/store/caf/user/lamia/pythia6_nonPromptJpsi_boosted_wofilters_totevt100M_MuonAna_20140407.root"); //from Hyunchul
	}
	if (ana1->IsZombie()) { cout << "CANNOT open data root file\n"; return 1; }

	//const char* strName = Form("%s_%s",strBinnig,sampleName);
	//std::cout << "strName: " << strName << std::endl;

	// muons (pdg +13 or -13 already)
	int dkid0_Id, dkid0_ch, dkid0_st;	
	int dkid1_Id, dkid1_ch, dkid1_st;
	double dkid0_pt, dkid0_eta, dkid0_y, dkid0_px, dkid0_py, dkid0_pz, dkid0_phi, dkid0_mass, dkid0_p;
	double dkid1_pt, dkid1_eta, dkid1_y, dkid1_px, dkid1_py, dkid1_pz, dkid1_phi, dkid1_mass, dkid1_p;
	double dimu_pt, dimu_eta, dimu_y, dimu_px, dimu_py, dimu_pz, dimu_phi, dimu_mass, dimu_p;
	// J/psi
	int dmom0_Id, dmom0_ch, dmom0_st;	
	// dgmom (not used)
	int dgmom0_Id, dgmom0_ch, dgmom0_st;
	double dmom0_pt, dmom0_eta, dmom0_y, dmom0_px, dmom0_py, dmom0_pz, dmom0_phi, dmom0_mass, dmom0_p;
	double dgmom0_pt, dgmom0_eta, dgmom0_y, dgmom0_px, dgmom0_py, dgmom0_pz, dgmom0_phi, dgmom0_mass, dgmom0_p;

	using namespace std;

	// Definition of bin
	// --- pt Bin
	Double_t ptBinsArr[] = {0.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap9pt
	//Double_t ptBinsArr[] = {5.0, 6.5, 10.0, 30.0}; // 6rap3pt
    const Int_t nPtBins = sizeof(ptBinsArr)/sizeof(double)-1;
    cout << "nPtBins=" << nPtBins << endl;
    
    Double_t ptFineArrNum[] = {0.0, 0.6, 1.2, 1.8, 2.4, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.3, 5.6, 5.9, 6.2, 6.5, 6.7, 6.9, 7.1, 7.3, 7.5, 7.7, 7.9, 8.1, 8.3, 8.5, 8.8, 9.1, 9.4, 9.7, 10., 10.8, 11.6, 12.4, 13.2, 14., 17.2, 20.4, 23.6, 26.8, 30.};
    const Int_t nPtFine = sizeof(ptFineArrNum)/sizeof(Double_t)-1;
	
    //Double_t AccCentArr[] = {0.087, 0.089, 0.131, 0.196, 0.288, 0.370, 0.447, 0.539, 0.681}; // Acceptance central value

  // --- y Bin //set to 1st run (For 2nd run, will be automatically changed later)
    Double_t yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // 8rap9pt
    Double_t yBinsArr_pPb[] = {-2.4, -1.93, -1.46, -1.03, -0.43, 0.47, 1.37, 1.97, 2.4}; // 8rap9pt
  //Double_t yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46}; // 6rap3pt
	const Int_t nYBins = sizeof(yBinsArr)/sizeof(double)-1;
	cout << "nYBins=" << nYBins << endl;

	// for 2nd run
	Double_t yBinsArr2nd[nYBins+1] = {};
	for (Int_t i=0; i<nYBins+1; i++) {
		 yBinsArr2nd[i] = -1*yBinsArr[nYBins-i];
		cout <<"yBinsArr["<<i<<"] = " <<yBinsArr[i]<<endl;
		cout <<"yBinsArr2nd["<<i<<"] = " <<yBinsArr2nd[i]<<endl;
	}
	const Int_t nYBins2nd = sizeof(yBinsArr2nd)/sizeof(double)-1;

	//read-in branches
	ana1->SetBranchAddress("dimu_eta",	&dimu_eta);
	ana1->SetBranchAddress("dimu_mass",	&dimu_mass);
	ana1->SetBranchAddress("dimu_p",	&dimu_p);
	ana1->SetBranchAddress("dimu_phi",	&dimu_phi);
	ana1->SetBranchAddress("dimu_pt",	&dimu_pt);
	ana1->SetBranchAddress("dimu_px",	&dimu_px);
	ana1->SetBranchAddress("dimu_py",	&dimu_py);
	ana1->SetBranchAddress("dimu_pz",	&dimu_pz);
	ana1->SetBranchAddress("dimu_y",	&dimu_y);

	ana1->SetBranchAddress("dgmom0_Id",	&dgmom0_Id);
	ana1->SetBranchAddress("dgmom0_ch",	&dgmom0_ch);
	ana1->SetBranchAddress("dgmom0_eta",	&dgmom0_eta);
	ana1->SetBranchAddress("dgmom0_mass",	&dgmom0_mass);
	ana1->SetBranchAddress("dgmom0_p",	&dgmom0_p);
	ana1->SetBranchAddress("dgmom0_phi",	&dgmom0_phi);
	ana1->SetBranchAddress("dgmom0_pt",	&dgmom0_pt);
	ana1->SetBranchAddress("dgmom0_px",	&dgmom0_px);
	ana1->SetBranchAddress("dgmom0_py",	&dgmom0_py);
	ana1->SetBranchAddress("dgmom0_pz",	&dgmom0_pz);
	ana1->SetBranchAddress("dgmom0_st",	&dgmom0_st);
	ana1->SetBranchAddress("dgmom0_y",	&dgmom0_y);

	ana1->SetBranchAddress("dmom0_Id",	&dmom0_Id);
	ana1->SetBranchAddress("dmom0_ch",	&dmom0_ch);
	ana1->SetBranchAddress("dmom0_eta",	&dmom0_eta);
	ana1->SetBranchAddress("dmom0_mass",	&dmom0_mass);
	ana1->SetBranchAddress("dmom0_p",	&dmom0_p);
	ana1->SetBranchAddress("dmom0_phi",	&dmom0_phi);
	ana1->SetBranchAddress("dmom0_pt",	&dmom0_pt);
	ana1->SetBranchAddress("dmom0_px",	&dmom0_px);
	ana1->SetBranchAddress("dmom0_py",	&dmom0_py);
	ana1->SetBranchAddress("dmom0_pz",	&dmom0_pz);
	ana1->SetBranchAddress("dmom0_st",	&dmom0_st);
	ana1->SetBranchAddress("dmom0_y",	&dmom0_y);

	ana1->SetBranchAddress("dkid0_Id",	&dkid0_Id);
	ana1->SetBranchAddress("dkid0_ch",	&dkid0_ch);
	ana1->SetBranchAddress("dkid0_eta",	&dkid0_eta);
	ana1->SetBranchAddress("dkid0_mass",	&dkid0_mass);
	ana1->SetBranchAddress("dkid0_p",	&dkid0_p);
	ana1->SetBranchAddress("dkid0_phi",	&dkid0_phi);
	ana1->SetBranchAddress("dkid0_pt",	&dkid0_pt);
	ana1->SetBranchAddress("dkid0_px",	&dkid0_px);
	ana1->SetBranchAddress("dkid0_py",	&dkid0_py);
	ana1->SetBranchAddress("dkid0_pz",	&dkid0_pz);
	ana1->SetBranchAddress("dkid0_st",	&dkid0_st);
	ana1->SetBranchAddress("dkid0_y",	&dkid0_y);

	ana1->SetBranchAddress("dkid1_Id",	&dkid1_Id);
	ana1->SetBranchAddress("dkid1_ch",	&dkid1_ch);
	ana1->SetBranchAddress("dkid1_eta",	&dkid1_eta);
	ana1->SetBranchAddress("dkid1_mass",	&dkid1_mass);
	ana1->SetBranchAddress("dkid1_p",	&dkid1_p);
	ana1->SetBranchAddress("dkid1_phi",	&dkid1_phi);
	ana1->SetBranchAddress("dkid1_pt",	&dkid1_pt);
	ana1->SetBranchAddress("dkid1_px",	&dkid1_px);
	ana1->SetBranchAddress("dkid1_py",	&dkid1_py);
	ana1->SetBranchAddress("dkid1_pz",	&dkid1_pz);
	ana1->SetBranchAddress("dkid1_st",	&dkid1_st);
	ana1->SetBranchAddress("dkid1_y",	&dkid1_y);

	cout << "Entries of tree : " << ana1->GetEntries() << endl;
	//std::cout << "strName: " << strName << std::endl;


    ////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
    // Get Main Acceptance Value
    string runstring;
    if(isPbp) runstring="Pbp";
    else runstring="pPb";

    TString prom_st, run_st;
    if(isPrompt) prom_st = "PR"; else prom_st = "NP";
    if(isPbp) run_st = "Pbp"; else run_st = "pPb";
	//cout <<"runstring : " <<  runstring.Data() << endl;
    ////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
    // Toy Acceptance Calculation
      TFile* toyFile = new TFile(Form("/afs/cern.ch/work/y/ygo/private/PhotonAnalysis2015/150227_JPsiAccSys2ndBatch/ToyGaussian_isPrompt%d_%s.root",(int)isPrompt,runstring.c_str()));
//    TFile* toyFile = new TFile("/afs/cern.ch/work/y/ygo/private/PhotonAnalysis2015/150227_JPsiAccSys2ndBatch/ToyGaussian_isPrompt1_Pbp.root");
//    TFile* toyFile = new TFile(Form("ToyGaussian_isPrompt%d_%s.root",isPrompt,runstring.Data()));
    	TH1D* hWeight[nYBins];
    //TCanvas* ctemp = new TCanvas("ctemp", "",400,400);
    for(int iy=0;iy<nYBins;iy++){
        hWeight[iy]=(TH1D*) toyFile->Get(Form("hWeight_%s_%d",runstring.c_str(),iy)); 
        cout << hWeight[iy]->GetName() << endl;
	//if(iy==0){hWeight[iy]->Draw();}
	if(hWeight[iy]->IsZombie()) {cout << "CANNOT open hWeight histogram\n"; return 1; }
    }

    TFile* outFileAcc = new TFile(Form("/afs/cern.ch/work/y/ygo/private/PhotonAnalysis2015/150227_JPsiAccSys2ndBatch/isPrompt%disPbp%d/AccDist_isPrompt%d_%s_fileNum%d.root",(int)isPrompt,(int)isPbp,(int)isPrompt,runstring.c_str(),fileNum),"recreate");
    ////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	// define 1D hist
    // for Acc. Sys. study
    TH2D *h2D_Den_pt_y;
    TH2D *h2D_Num_pt_y;
    TH2D *h2D_Acc_pt_y;
    if(isPbp){
        h2D_Den_pt_y= new TH2D("h2D_Den_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
        h2D_Num_pt_y= new TH2D("h2D_Num_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
        h2D_Acc_pt_y= new TH2D("h2D_Acc_pt_y","",nYBins,yBinsArr,nPtBins,ptBinsArr);
    } else if(isPbp==0){
        h2D_Den_pt_y= new TH2D("h2D_Den_pt_y","",nYBins,yBinsArr_pPb,nPtBins,ptBinsArr);
        h2D_Num_pt_y= new TH2D("h2D_Num_pt_y","",nYBins,yBinsArr_pPb,nPtBins,ptBinsArr);
        h2D_Acc_pt_y= new TH2D("h2D_Acc_pt_y","",nYBins,yBinsArr_pPb,nPtBins,ptBinsArr);
    }
    TH1D *hAccCompBin[nYBins][nPtBins];

    TFile *accCentFile_PR = new TFile("/afs/cern.ch/work/y/ygo/private/PhotonAnalysis2015/150227_JPsiAccSys2ndBatch/AccAna_8rap9pt_PRMC_boosted.root");
    TFile *accCentFile_NP = new TFile("/afs/cern.ch/work/y/ygo/private/PhotonAnalysis2015/150227_JPsiAccSys2ndBatch/AccAna_8rap9pt_NPMC_boosted.root");
    TH1D* hAccCent;
    if(isPrompt==0 && isPbp==0){
        hAccCent = (TH1D*) accCentFile_NP->Get("h2D_Acc_pt_y_pPb");
    }else if(isPrompt==0 && isPbp==1){
        hAccCent = (TH1D*) accCentFile_NP->Get("h2D_Acc_pt_y_Pbp");
    }else if(isPrompt==1 && isPbp==0){
        hAccCent = (TH1D*) accCentFile_PR->Get("h2D_Acc_pt_y_pPb");
    }else if(isPrompt==1 && isPbp==1){
        hAccCent = (TH1D*) accCentFile_PR->Get("h2D_Acc_pt_y_Pbp");
    }
	if(hAccCent->IsZombie()) {cout << "CANNOT open hAccCent histogram\n"; return 1; }
    double AccCent[nYBins][nPtBins];

    for(int iy=0;iy<nYBins;iy++){
//        h1D_Num_pt[iy]= new TH1D(Form("h1D_Num_pt_y%d",iy),"h1D_Num",nPtBins,ptBinsArr);
//        h1D_Den_pt[iy]= new TH1D(Form("h1D_Den_pt_y%d",iy),"h1D_Den",nPtBins,ptBinsArr);
//        h1D_Acc_pt[iy]= new TH1D(Form("h1D_Acc_pt_y%d",iy),"h1D_Acc",nPtBins,ptBinsArr);
        for(int ipt=0;ipt<nPtBins;ipt++){
            AccCent[iy][ipt] = hAccCent->GetBinContent(iy+1,ipt+1);
            hAccCompBin[iy][ipt] = new TH1D(Form("hAccCompBin_y%d_pt%d",iy,ipt),Form("Acc. Dist. of %d ptbin for rapidity %dth bin;Acceptance;Entries",ipt,iy),2000, AccCent[iy][ipt]-0.1, AccCent[iy][ipt]+0.1);
            //hAccCompBin[iy][ipt] = new TH1D(Form("hAccCompBin_y%d_pt%d",iy,ipt),Form("Acc. Dist. of %d ptbin for rapidity %dth bin;Acceptance;Entries",ipt,iy),100, 0.0, 1.0);
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

    //// Loop Start!
/*  TCanvas * ctemp= new TCanvas("ctemp","ctemp",800,500);
    TCanvas * can[Ntoy];
    for(int itoy=0;itoy<Ntoy;itoy++){
        can[itoy] = new TCanvas(Form("can_%d",itoy),Form("can_%d",itoy),500,500);
    }
*/
    int Ntoy = 10;
    for(int itoy=0; itoy<Ntoy; itoy++){
        //ctemp->cd();
        if(itoy%100==0) cout << ">>>>> itoy " << itoy << " / " << Ntoy <<  endl;
            h2D_Num_pt_y-> Reset();
            h2D_Den_pt_y-> Reset();
            h2D_Acc_pt_y-> Reset();
        for(int i=0; i<ana1->GetEntries(); i++){
            // if(i%100000==0) cout << ">>>>> EVENT " << i << " / " << ana1->GetEntries() <<  endl;
            ana1->GetEntry(i);
            //weighting factor for Acc. Sys. by Yeonju
            double w_toy = 1.0;
            int yflag = 99;
            double ptThr = 0.0;
            string st_AccRange = "";
            if(isPbp){
                if(-2.40<= dimu_y && dimu_y<-1.97) {  yflag = 0; ptThr=0.0; }
                else if(-1.97<= dimu_y && dimu_y<-1.37) {  yflag = 1;  ptThr=3.0; } 
                else if(-1.37<= dimu_y && dimu_y<-0.47) {  yflag = 2;  ptThr=6.5; } 
                else if(-0.47<= dimu_y && dimu_y<0.43) {  yflag = 3;  ptThr=6.5; }
                else if(0.43<= dimu_y && dimu_y<1.03) {  yflag = 4;  ptThr=6.5; }
                else if(1.03<= dimu_y && dimu_y<1.46) {  yflag = 5;  ptThr=5.0; }
                else if(1.46<= dimu_y && dimu_y<1.93) {  yflag = 6;  ptThr=3.0; }
                else if(1.93<= dimu_y && dimu_y<2.40) {  yflag = 7;  ptThr=0.0; }
            } else if(isPbp==0){//pPb
                if(-2.40<= dimu_y && dimu_y<-1.93) {  yflag = 7; ptThr=0.0; }
                else if(-1.93<= dimu_y && dimu_y<-1.46) {  yflag = 6;  ptThr=3.0; } 
                else if(-1.46<= dimu_y && dimu_y<-1.03) {  yflag = 5;  ptThr=5.0; } 
                else if(-1.03<= dimu_y && dimu_y<-0.43) {  yflag = 4;  ptThr=6.5; }
                else if(-0.43<= dimu_y && dimu_y<0.47) {  yflag = 3;  ptThr=6.5; }
                else if(0.47<= dimu_y && dimu_y<1.37) {  yflag = 2;  ptThr=6.5; }
                else if(1.37<= dimu_y && dimu_y<1.97) {  yflag = 1;  ptThr=3.0; }
                else if(1.97<= dimu_y && dimu_y<2.40) {  yflag = 0;  ptThr=0.0; }
            }
//	cout << "c " << endl;
            if(yflag==99) continue;
            if(dimu_pt<ptThr) st_AccRange="out of the range";
       
	//cout << "d " << endl;
            cout << "yflag : " << yflag << ", pt : " << dimu_pt<< endl; 
		cout << "d"<<hWeight[yflag]<<endl;
		cout << "d"<<hWeight[yflag]->GetName()<<endl;
//            Int_t hBin = hWeight[yflag]->FindBin(dimu_pt);
            Int_t hBin = 0;
	//	cout << "hBin = " <<hBin << endl;
		hBin=hWeight[yflag]->FindBin(dimu_pt);
	//	cout << "hBin = " <<hBin << endl;
	//cout << "e" << endl;
            //cout << hBin << endl;
            double mean = hWeight[yflag]->GetBinContent(hBin);
            double sigma = hWeight[yflag]->GetBinError(hBin); 
            //cout << hBin << "   "  << mean <<"  " << sigma << endl;
            
	//cout << "h " << endl;
          //  if(i<100) cout <<"Before!!!!!! pt : " << dimu_pt << ", y : " << dimu_y << ", mean : " << mean << ", sigma : " << sigma << ", weighting : " << w_toy << ", " << st_AccRange << endl;
            if(sigma==0.000) continue;
	//cout << "i " << endl;
            w_toy = gRandom->Gaus(mean,sigma);
//	cout << "j " << endl;
            if(i<2) cout << "weighting factor : " << w_toy << endl;
          //  if(i<100) cout <<"After!!!!!!! pt : " << dimu_pt << ", y : " << dimu_y << ", mean : " << mean << ", sigma : " << sigma << ", weighting : " << w_toy << ", " << st_AccRange << endl;

            ////// --- cut01 : no cut
            if (!dimuCut(dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch)) continue;

            ////// --- cut02 : for denominator
            bool yn = false;
            if (dimuCut(dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch) && minpt<=dimu_pt && dimu_pt<maxpt && minylab<=dimu_y && dimu_y<maxylab) {yn = true;}
            if ( yn == true  ) {
                h2D_Den_pt_y->Fill(dimu_y,dimu_pt,w_toy); //yeonju
                ////// --- cut03 : for numerator
                bool yn2 = false;
                if (massCut(dimu_mass) && dimuCut(dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch) && kineCut(dkid0_pt,dkid0_eta,dkid0_p) && kineCut(dkid1_pt,dkid1_eta,dkid1_p) && minpt<=dimu_pt && dimu_pt<maxpt && minylab<=dimu_y && dimu_y<maxylab) {yn2 = true;}
                if (yn2 == true){
                    h2D_Num_pt_y->Fill(dimu_y,dimu_pt,w_toy);//yeonju
                } // end of yn2
            } // end of yn
        } //end of loop
        
        // (Num/Den) to get acceptance (B : binomial error)
        h2D_Acc_pt_y->Divide(h2D_Num_pt_y,h2D_Den_pt_y,1,1,"B");
        // can[itoy]->cd();
        // h2D_Acc_pt_y->DrawCopy("text colz");
        for(int iy=0;iy<nYBins;iy++){
            for(int ipt=0;ipt<nPtBins;ipt++){
                hAccCompBin[iy][ipt]->Fill(h2D_Acc_pt_y->GetBinContent(iy+1,ipt+1));
                // cout << "toy number: "<< itoy << ", ptbin : " << ipt+1<< ", Acceptance : " << h1D_Acc_pt->GetBinContent(ipt+1) << endl;
            }
        }
    } //end of itoy loop

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
	// Cosmetic!
    for(int iy=0;iy<nYBins;iy++){
        for(int ipt=0;ipt<nPtBins;ipt++){
           // double maxVal = hAccCompBin[iy][ipt]->GetBinContent(hAccCompBin[iy][ipt]->GetMaximumBin());
          //  hAccCompBin[iy][ipt]->GetYaxis()->SetRangeUser(0,maxVal*1.2);
          //  jumSun(AccCent[iy][ipt],0,AccCent[iy][ipt],maxVal*1.2,kBlue);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
	// Save the data!

    outFileAcc->cd();
    for(int iy=0;iy<nYBins;iy++){
        for(int ipt=0;ipt<nPtBins;ipt++){
            hAccCompBin[iy][ipt]->Write();
        }
    }
    hAccCent->Write();
    for(int itoy=0;itoy<Ntoy;itoy++){
        //can[itoy]->Write();
    }
    outFileAcc->Close();

    timer.Stop();
    double final_rTime = timer.RealTime();
    cout << "real time : " << final_rTime << endl;

	return 0;
} // end of main func

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

bool dimuCut(int lv_dmom0_Id, int lv_dgmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch) {
	return (TMath::Abs(lv_dmom0_Id)==443 && lv_dkid0_ch*lv_dkid1_ch<0);
}

bool kineCut(double muPt, double muEta, double muP) {
		return ( TMath::Abs(muEta) < 2.4 &&
						((TMath::Abs(muEta) < 1.3 && muPt >=3.3) ||
						 (1.3 <= TMath::Abs(muEta) && TMath::Abs(muEta) < 2.2 && muP >=2.9) ||
						 (2.2 <= TMath::Abs(muEta) && muPt >= 0.8)));
}

bool massCut(double lv_dimu_mass) {
	return ( 2.6 <= lv_dimu_mass && lv_dimu_mass < 3.5 );
}
void jumSun(Double_t x1,Double_t y1,Double_t x2,Double_t y2,Int_t color, Double_t width)
{
    TLine* t1 = new TLine(x1,y1,x2,y2);
    t1->SetLineWidth(width);
    t1->SetLineStyle(7);
    t1->SetLineColor(color);
    t1->Draw();
}


void onSun(Double_t x1,Double_t y1,Double_t x2,Double_t y2,Int_t color, Double_t width)
{
    TLine* t1 = new TLine(x1,y1,x2,y2);
    t1->SetLineWidth(width);
    t1->SetLineStyle(1);
    t1->SetLineColor(color);
    t1->Draw();
}
