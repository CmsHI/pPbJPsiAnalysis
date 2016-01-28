// Modified by Yeonju
//To corrected Acceptance applying weighting factors from DATA/MC fit fuction..

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>
#include <TH1.h>
#include <TF1.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TMath.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TAxis.h>
#include <cmath>
#include <TLorentzRotation.h>

const double shiftvar = -0.47; // conversion constant y=0(collision)==y=-0.47(LAB frame)  

//bool dimuCut(int, int lv_dmom0_Id, int lv_dgmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch);
//bool kineCut(bool, double muPt, double muEta, double muP);
bool dimuCut(int lv_dmom0_Id, int lv_dgmom0_Id, int lv_dkid0_ch, int lv_dkid1_ch);
bool kineCut(double muPt, double muEta, double muP);
bool massCut(double lv_dimu_mass);

void rootAna_acceptance_weight_kyo01(bool isPrompt=true, bool isPbp=true){

//    gROOT->Macro("rootlogon.C+");
    gStyle->SetCanvasDefW(800);

    char* sampleName;
    double minylab =-2.4;
    double maxylab=2.4;
    double minpt=0.0;
    double maxpt=30.0;

    //read-in tree
    TChain * ana1 = new TChain("DiAna");
    if (isPrompt){
        sampleName="PRMC_boosted";
        //		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_JPsiWithFSR_5TeV02_1st_KYOvtx_ntuple_20150126.root");
        //		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_JPsiWithFSR_5TeV02_1st_KYOvtx_ntuple_20150126_p2.root");
        ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140210.root"); //from Hyunchul
        ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/pythia6_PromptJpsi_boosted_wofilters_totevtin5M_new_MuonAna_20140211.root"); //from Hyunchul
    }
    else {
        sampleName="NPMC_boosted";
        //		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_inclBtoJPsiMuMu_5TeV02_1st_KYOvtx_ntuple_20150127.root");
        //		ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSampleKYOvtx/tot_GENonly_inclBtoJPsiMuMu_5TeV02_1st_KYOvtx_ntuple_20150127_p2.root");
        ana1->Add("/home/songkyo/kyo/pPbDataSample/AcceptanceSample/pythia6_nonPromptJpsi_boosted_wofilters_totevt100M_MuonAna_20140407.root"); //from Hyunchul
    }

    //	const char* strName = Form("%s_%s",strBinnig,sampleName);
    //	std::cout << "strName: " << strName << std::endl;

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
    //Double_t ptBinsArr[] = {0.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap9pt
    Double_t ptBinsArr[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap9pt2gev
    //Double_t ptBinsArr[] = {5.0, 6.5, 10.0, 30.0}; // 6rap3pt
    const Int_t nPtBins = sizeof(ptBinsArr)/sizeof(double)-1;
    cout << "nPtBins=" << nPtBins << endl;

    // --- y Bin //set to 1st run (For 2nd run, will be automatically changed later)
    Double_t yBinsArr[] = {-2.4, -1.97, -1.37, -0.47, 0.43, 1.03, 1.46, 1.93, 2.4}; // 8rap9pt
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
    cout << "nYBins2nd=" << nYBins2nd << endl;
    if (nYBins != nYBins2nd) { cout << " *** Error!! nYBins should be equal to nYBins2nd"<<endl; return; };

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
    //	std::cout << "strName: " << strName << std::endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // define 2D hist
    // with defined binning - double 1
    TH2D *h2D_NoCut_noWeight_pt_y_Pbp = new TH2D("h2D_NoCut_noWeight_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
    TH2D *h2D_Den_noWeight_pt_y_Pbp = new TH2D("h2D_Den_noWeight_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
    TH2D *h2D_Num_noWeight_pt_y_Pbp = new TH2D("h2D_Num_noWeight_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
    TH2D *h2D_Acc_noWeight_pt_y_Pbp = new TH2D("h2D_Acc_noWeight_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);

    //by Yeonju
    TH2D* h2D_NoCut_pt_y_Pbp = new TH2D("h2D_NoCut_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
    TH2D* h2D_Den_pt_y_Pbp = new TH2D("h2D_Den_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
    TH2D* h2D_Num_pt_y_Pbp = new TH2D("h2D_Num_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
    TH2D* h2D_Acc_pt_y_Pbp = new TH2D("h2D_Acc_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
    
	TH1D *h1D_Num_pt= new TH1D("h1D_Num_pt","",nPtBins,ptBinsArr);
    TH1D *h1D_Den_pt= new TH1D("h1D_Den_pt","",nPtBins,ptBinsArr);
    TH1D *h1D_Acc_pt= new TH1D("h1D_Acc_pt","",nPtBins,ptBinsArr);
    //	std::cout << "TH1, TH2 definition, strName: " << strName << std::endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //2D hist
    h2D_NoCut_noWeight_pt_y_Pbp->Sumw2();
    h2D_Den_noWeight_pt_y_Pbp->Sumw2();
    h2D_Num_noWeight_pt_y_Pbp->Sumw2();
    h2D_Acc_noWeight_pt_y_Pbp->Sumw2();

    //by Yeonju
    h2D_NoCut_pt_y_Pbp->Sumw2();
    h2D_Den_pt_y_Pbp->Sumw2();
    h2D_Num_pt_y_Pbp->Sumw2();
    h2D_Acc_pt_y_Pbp->Sumw2();

    h1D_Num_pt->Sumw2();
    h1D_Den_pt->Sumw2();
    h1D_Acc_pt->Sumw2();

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // String definition
    string runstring;
    if(isPbp) runstring="Pbp";
    else runstring="pPb";

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Outputs
    //TFile* toyFile = new TFile(Form("ToyGaussian_isPrompt%d_%s_kyo01.root",(int)isPrompt,runstring.c_str()));
    TFile* toyFile = new TFile("fitRatio_isPrompt1_Pbp_kyo01.root");
    TF1* Func[nYBins];
    for(int iy=0;iy<nYBins;iy++){
        Func[iy]=(TF1*) toyFile->Get(Form("func_%d",iy)); 
    }   

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //// Loop Start!

    for(int i=0; i<ana1->GetEntries(); i++){
        if(i%100000==0) cout << ">>>>> EVENT " << i << " / " << ana1->GetEntries() <<  endl;
        ana1->GetEntry(i);
        
        //weighting factor for Acc. Sys. by Yeonju
        double w_toy = 1.0;
        int yflag = 99;
        double ptThr = 0.0;
        string st_AccRange = "";
        // Funcy[yflag] is ordered from FW to BW in y_CM!! (kyo)   
						if(-2.40<= dimu_y && dimu_y<-1.97) {  yflag = 0; ptThr=0.0; }
            else if(-1.97<= dimu_y && dimu_y<-1.37) {  yflag = 1;  ptThr=3.0; }
            else if(-1.37<= dimu_y && dimu_y<-0.47) {  yflag = 2;  ptThr=6.5; }
            else if(-0.47<= dimu_y && dimu_y<0.43) {  yflag = 3;  ptThr=6.5; }
            else if(0.43<= dimu_y && dimu_y<1.03) {  yflag = 4;  ptThr=6.5; }
            else if(1.03<= dimu_y && dimu_y<1.46) {  yflag = 5;  ptThr=5.0; }
            else if(1.46<= dimu_y && dimu_y<1.93) {  yflag = 6;  ptThr=3.0; }
            else if(1.93<= dimu_y && dimu_y<2.40) {  yflag = 7;  ptThr=0.0; }
        if(yflag==99) continue;
        if(dimu_pt<ptThr) st_AccRange = "out of the range";
        w_toy = Func[yflag]->Eval(dimu_pt);
        if(i<100) cout << " pt : " << dimu_pt << ", y : " << dimu_y << ", weigt : " << w_toy << ", " << st_AccRange <<  endl;

        ////// --- cut01 : no cut
        h2D_NoCut_noWeight_pt_y_Pbp->Fill(dimu_y,dimu_pt);
        h2D_NoCut_pt_y_Pbp->Fill(dimu_y,dimu_pt,w_toy);

        if (!dimuCut(dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch)) continue;

        ////// --- cut02 : for denominator
        bool yn = false;
        if (dimuCut(dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch) && minpt<=dimu_pt && dimu_pt<maxpt && minylab<=dimu_y && dimu_y<maxylab) {yn = true;}
        if ( yn == true  ) {
            h2D_Den_noWeight_pt_y_Pbp->Fill(dimu_y,dimu_pt);
            h2D_Den_pt_y_Pbp->Fill(dimu_y,dimu_pt,w_toy);
            h1D_Den_pt->Fill(dimu_pt);

            ////// --- cut03 : for numerator
            bool yn2 = false;
            if (massCut(dimu_mass) && dimuCut(dmom0_Id,dgmom0_Id,dkid0_ch,dkid1_ch) && kineCut(dkid0_pt,dkid0_eta,dkid0_p) && kineCut(dkid1_pt,dkid1_eta,dkid1_p) && minpt<=dimu_pt && dimu_pt<maxpt && minylab<=dimu_y && dimu_y<maxylab) {yn2 = true;}
            if (yn2 == true){
                h2D_Num_noWeight_pt_y_Pbp->Fill(dimu_y,dimu_pt);
                h2D_Num_pt_y_Pbp->Fill(dimu_y,dimu_pt,w_toy);
                h1D_Num_pt->Fill(dimu_pt);
            } // end of yn2
        } // end of yn
    } //end of loop

    // (Num/Den) to get acceptance (B : binomial error)
    h2D_Acc_noWeight_pt_y_Pbp->Divide(h2D_Num_noWeight_pt_y_Pbp,h2D_Den_noWeight_pt_y_Pbp,1,1,"B");
    h2D_Acc_pt_y_Pbp->Divide(h2D_Num_pt_y_Pbp,h2D_Den_pt_y_Pbp,1,1,"B");
    h1D_Acc_pt->Divide(h1D_Num_pt,h1D_Den_pt);

		// difference and relative error calculation
    TH2D* h2D_Acc_diff_pt_y_Pbp;
    TH2D* h2D_Acc_err_pt_y_Pbp = new TH2D("h2D_Acc_err_pt_y_Pbp","",nYBins,yBinsArr,nPtBins,ptBinsArr);
    h2D_Acc_err_pt_y_Pbp->Sumw2();
    for(int ipt=0;ipt<nPtBins;ipt++){
        for(int iy=0;iy<nYBins;iy++){
            h2D_Acc_diff_pt_y_Pbp = (TH2D*) h2D_Acc_pt_y_Pbp->Clone("h2D_Acc_diff_pt_y_Pbp");
            h2D_Acc_diff_pt_y_Pbp->Add(h2D_Acc_noWeight_pt_y_Pbp,-1); 
            h2D_Acc_err_pt_y_Pbp->Divide(h2D_Acc_diff_pt_y_Pbp,h2D_Acc_pt_y_Pbp);
        }
    }

  ////////////////////////////////////////////////////////////////////////////
	// rapidity change, for 2nd run hist.
	TH2D *h2D_Acc_noWeight_pt_y_pPb = new TH2D("h2D_Acc_noWeight_pt_y_pPb","",nYBins2nd,yBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Den_pt_y_pPb = new TH2D("h2D_Den_pt_y_pPb","",nYBins2nd,yBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Num_pt_y_pPb = new TH2D("h2D_Num_pt_y_pPb","",nYBins2nd,yBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_pt_y_pPb = new TH2D("h2D_Acc_pt_y_pPb","",nYBins2nd,yBinsArr2nd,nPtBins,ptBinsArr);
	TH2D *h2D_Acc_err_pt_y_pPb = new TH2D("h2D_Acc_err_pt_y_pPb","",nYBins2nd,yBinsArr2nd,nPtBins,ptBinsArr);
	h2D_Acc_err_pt_y_pPb->Sumw2();
	h2D_Acc_pt_y_pPb->Sumw2();
	h2D_Acc_noWeight_pt_y_pPb->Sumw2();

	int tmpbin01Pbp, tmpbin01pPb; 
	int tmpbin02Pbp, tmpbin02pPb; 
	int tmpbin03Pbp, tmpbin03pPb; 
	int tmpbin04Pbp, tmpbin04pPb; 
	int tmpbin05Pbp, tmpbin05pPb; 
	float tmp01Val, tmp01Err;
	float tmp02Val, tmp02Err;
	float tmp03Val, tmp03Err;
	float tmp04Val, tmp04Err;
	float tmp05Val, tmp05Err;
	for (Int_t iy=0; iy<nYBins; iy++){
		for (Int_t ipt=0; ipt<nPtBins; ipt++) {
			tmpbin01Pbp = h2D_Acc_noWeight_pt_y_Pbp->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmp01Val = h2D_Acc_noWeight_pt_y_Pbp->GetBinContent(tmpbin01Pbp);
			tmp01Err = h2D_Acc_noWeight_pt_y_Pbp->GetBinError(tmpbin01Pbp);
			tmpbin01pPb = h2D_Acc_noWeight_pt_y_pPb->FindBin((yBinsArr2nd[nYBins-iy-1]+yBinsArr2nd[nYBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Acc_noWeight_pt_y_pPb->SetBinContent(tmpbin01pPb,tmp01Val);
			h2D_Acc_noWeight_pt_y_pPb->SetBinError(tmpbin01pPb,tmp01Err);
			
			tmpbin02Pbp = h2D_Den_pt_y_Pbp->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmp02Val = h2D_Den_pt_y_Pbp->GetBinContent(tmpbin02Pbp);
			tmp02Err = h2D_Den_pt_y_Pbp->GetBinError(tmpbin02Pbp);
			tmpbin02pPb = h2D_Den_pt_y_pPb->FindBin((yBinsArr2nd[nYBins-iy-1]+yBinsArr2nd[nYBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Den_pt_y_pPb->SetBinContent(tmpbin02pPb,tmp02Val);
			h2D_Den_pt_y_pPb->SetBinError(tmpbin02pPb,tmp02Err);
			
			
			tmpbin03Pbp = h2D_Num_pt_y_Pbp->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmp03Val = h2D_Num_pt_y_Pbp->GetBinContent(tmpbin03Pbp);
			tmp03Err = h2D_Num_pt_y_Pbp->GetBinError(tmpbin03Pbp);
			tmpbin03pPb = h2D_Num_pt_y_pPb->FindBin((yBinsArr2nd[nYBins-iy-1]+yBinsArr2nd[nYBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Num_pt_y_pPb->SetBinContent(tmpbin03pPb,tmp03Val);
			h2D_Num_pt_y_pPb->SetBinError(tmpbin03pPb,tmp03Err);
			
			
			tmpbin04Pbp = h2D_Acc_pt_y_Pbp->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmp04Val = h2D_Acc_pt_y_Pbp->GetBinContent(tmpbin04Pbp);
			tmp04Err = h2D_Acc_pt_y_Pbp->GetBinError(tmpbin04Pbp);
			tmpbin04pPb = h2D_Acc_pt_y_pPb->FindBin((yBinsArr2nd[nYBins-iy-1]+yBinsArr2nd[nYBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Acc_pt_y_pPb->SetBinContent(tmpbin04pPb,tmp04Val);
			h2D_Acc_pt_y_pPb->SetBinError(tmpbin04pPb,tmp04Err);
			
			tmpbin05Pbp = h2D_Acc_err_pt_y_Pbp->FindBin((yBinsArr[iy]+yBinsArr[iy+1])/2, (ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			tmp05Val = h2D_Acc_err_pt_y_Pbp->GetBinContent(tmpbin05Pbp);
			tmp05Err = h2D_Acc_err_pt_y_Pbp->GetBinError(tmpbin05Pbp);
			tmpbin05pPb = h2D_Acc_err_pt_y_pPb->FindBin((yBinsArr2nd[nYBins-iy-1]+yBinsArr2nd[nYBins-iy])/2,(ptBinsArr[ipt]+ptBinsArr[ipt+1])/2);
			h2D_Acc_err_pt_y_pPb->SetBinContent(tmpbin05pPb,tmp05Val);
			h2D_Acc_err_pt_y_pPb->SetBinError(tmpbin05pPb,tmp05Err);
		}
	}
		
    ////////////////////////////////////////////////////////////////////////////
    // Save the data!

    //TFile *outFile = new TFile(Form("OneAccAna_isPrompt%d_%s.root",(int)isPrompt,runstring.c_str()),"RECREATE");
    TFile *outFile = new TFile(Form("AccAna_isPrompt%d_%s_kyo01.root",(int)isPrompt,runstring.c_str()),"RECREATE");
    
		outFile->cd();

    h1D_Num_pt->Write();
    h1D_Den_pt->Write();
    h1D_Acc_pt->Write(); 
  
    h2D_Den_noWeight_pt_y_Pbp->Write();
    h2D_Num_noWeight_pt_y_Pbp->Write();
    h2D_Acc_noWeight_pt_y_Pbp->Write();
    h2D_Den_pt_y_Pbp->Write();
    h2D_Num_pt_y_Pbp->Write();
    h2D_Acc_pt_y_Pbp->Write();
    h2D_Acc_diff_pt_y_Pbp->Write();
    h2D_Acc_err_pt_y_Pbp->Write();
    h2D_Acc_noWeight_pt_y_pPb->Write();
    h2D_Den_pt_y_pPb->Write();
    h2D_Num_pt_y_pPb->Write();
    h2D_Acc_pt_y_pPb->Write();
    h2D_Acc_err_pt_y_pPb->Write();
   
    outFile->Close();

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

