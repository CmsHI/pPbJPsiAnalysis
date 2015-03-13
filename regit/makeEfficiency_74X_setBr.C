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
#include <TPaletteAxis.h> 

#include <TMath.h>
#include <math.h>

#include <sstream>
#include <string>
#include <fstream>
#include "TString.h"

#include "KYOcommonOpt.h"


bool kineCut(float muPt, float muEta);
bool massCut(float lv_dimu_mass);
bool idCut(float t_npixel, float t_nmuon);
bool idRecoCut(float t_nmatch, float t_chi);
float getWeight(bool t_isPrompt, bool t_isPair, float t_pt, float t_eta);
	
//for pT-|y| weight for 73X
TFile *fWeightPR  = new TFile("./pTdist_diffY/pTdist_diffY_prompt.root");
TFile *fWeightNP = new TFile("./pTdist_diffY/pTdist_diffY_nonprompt.root");

TH1D* hPairRatio_PR_0 = (TH1D*)fWeightPR->Get("hPairRatio_0");
TH1D* hPairRatio_PR_1 = (TH1D*)fWeightPR->Get("hPairRatio_1");
TH1D* hPairRatio_PR_2 = (TH1D*)fWeightPR->Get("hPairRatio_2");
TH1D* hPairRatio_PR_3 = (TH1D*)fWeightPR->Get("hPairRatio_3");
TH1D* hTrkRatio_PR_0 = (TH1D*)fWeightPR->Get("hTrkRatio_0");
TH1D* hTrkRatio_PR_1 = (TH1D*)fWeightPR->Get("hTrkRatio_1");
TH1D* hTrkRatio_PR_2 = (TH1D*)fWeightPR->Get("hTrkRatio_2");
TH1D* hTrkRatio_PR_3 = (TH1D*)fWeightPR->Get("hTrkRatio_3");

TH1D* hPairRatio_NP_0 = (TH1D*)fWeightNP->Get("hPairRatio_0");
TH1D* hPairRatio_NP_1 = (TH1D*)fWeightNP->Get("hPairRatio_1");
TH1D* hPairRatio_NP_2 = (TH1D*)fWeightNP->Get("hPairRatio_2");
TH1D* hPairRatio_NP_3 = (TH1D*)fWeightNP->Get("hPairRatio_3");
TH1D* hTrkRatio_NP_0 = (TH1D*)fWeightNP->Get("hTrkRatio_0");
TH1D* hTrkRatio_NP_1 = (TH1D*)fWeightNP->Get("hTrkRatio_1");
TH1D* hTrkRatio_NP_2 = (TH1D*)fWeightNP->Get("hTrkRatio_2");
TH1D* hTrkRatio_NP_3 = (TH1D*)fWeightNP->Get("hTrkRatio_3");

void makeEfficiency_74X_setBr(bool isPrompt=false, bool isPair=true, bool isEmbedded=true, bool isPtCut = false, bool doWeight = false)
{
	gROOT->Macro("./JpsiStyle.C");
	gStyle->SetPaintTextFormat(".3f"); // for text colz
	
	//string strDir = "eff_74X_setBr"; // directory name
	//string strDir = "eff_74X_setBr_wDetached"; // directory name
	string strDir = "eff_74X_noRegitMuDetached"; // directory name

	int initev =0;
	int nevt = -1; //all
	//int nevt = 1000;

	int ptbin = 11.;
	double ptmin = 0.;
	double ptmax = 11.;
	if (isEmbedded && !isPrompt) { ptbin = 22.; ptmax = 22.; }
	int rapbin = 12;
	double rapmin = -2.4;
	double rapmax = 2.4;
	int lxybin = 10;
	double lxymin = 0.;
	double lxymax = 1.;

	string strPrompt;
	string strEmbd;
	string strPair;
	if(isPrompt){ strPrompt = "prompt"; }
	else { strPrompt = "nonprompt"; }
	if(isEmbedded){ strEmbd = "embd";	}
	else { strEmbd = "nonembd"; }
	if(isPair){ strPair = "Pair";	}
	else { strPair = "Trk"; }
	cout << "strPrompt = " << strPrompt.c_str() << endl;
	cout << "strEmbd = " << strEmbd.c_str() << endl;
	cout << "strPair = " << strPair.c_str() << endl;

	////// read-in file & tree
	TFile * refFile;
	if (isEmbedded){
		////// 740pre3 -embd
		//if (isPrompt){	refFile = new TFile("/home/songkyo/kyo/regitSample/tot_Pythiagun_PromptJpsi_step2nMatch_KYO_regit_embd_20150211.root");}
		//else { refFile = new TFile("/home/songkyo/kyo/regitSample/tot_Pythiagun_B2JpsiMuMu_step2nMatch_KYO_regit_embd_20150211.root");}
		////// 740pre3 -embd with HiDetachedTripletStep
		//if (isPrompt){	refFile = new TFile("/home/songkyo/kyo/regitSample/tot_Pythiagun_PromptJpsi_step2nMatch_KYO_regit_wDetached_embd_20150224.root");}
		//else { refFile = new TFile("/home/songkyo/kyo/regitSample/tot_Pythiagun_B2JpsiMuMu_step2nMatch_KYO_regit_wDetached_embd_20150224.root");}
		////// 740pre3 - remove some regit : noRegitMuDetached
		if (isPrompt){	refFile = new TFile("/home/songkyo/kyo/regitSample/tot_Pythiagun_PromptJpsi_step2nMatch_KYO_regit_noRegitMuDetached_embd_20150310.root");}
		else { refFile = new TFile("/home/songkyo/kyo/regitSample/tot_Pythiagun_B2JpsiMuMu_step2nMatch_KYO_regit_noRegitMuDetached_embd_20150310.root");}
	}
	else {
		//730pre2 -nonembd
		if (isPrompt){	refFile = new TFile("/home/songkyo/kyo/regitSample/tot_Pythiagun_PromptJpsi_step2nMatch_KYO_regit_20141121.root");}
		else { refFile = new TFile("/home/songkyo/kyo/regitSample/tot_Pythiagun_B2Jpsi_step2nMatch_KYO_regit_20141203_no9_32_90.root");}
	}
	
	TTree *tr = new TTree(); 
	if(isPair) { tr = (TTree*)refFile->Get("mcmatchanalysis/pnSimRecoPairType1");} 
	else { tr = (TTree*)refFile->Get("mcmatchanalysis/pnSimRecoTrkType1"); }

/*
	//for pT-|y| weight
	TFile *fWeight;
	if (isPrompt){ fWeight = new TFile("./pTdist_diffY/pTdist_diffY_prompt.root");}
	else {fWeight = new TFile("./pTdist_diffY/pTdist_diffY_nonprompt.root");}

	const int nYBin = 4;	
	TH1D* hWeight[nYBin];
	for (int i=0; i< nYBin; i++){
		if (isPair) { hWeight[i] = (TH1D*)fWeight->Get(Form("hPairRatio_%d",i)); }
		else { hWeight[i] = (TH1D*)fWeight->Get(Form("hTrkRatio_%d",i)); };
		//cout << i << "th hWeight = " << hWeight[i] << endl;
		cout << i << "th hWeight Name= " << hWeight[i]->GetName() << endl;
	}
*/

	///////////////////////////////////////
	////// Pair //////
	//gen
	Float_t			pt;	
	Float_t			y;	
	Float_t			pt1;	
	Float_t			eta1;	
	Float_t			npixelhits1;	
	Float_t			nmuonhits1;	
	Float_t			pt2;	
	Float_t			eta2;	
	Float_t			npixelhits2;	
	Float_t			nmuonhits2;	
	//reco
	Float_t			ptreco;	
	Float_t			yreco;	
	Float_t			minvreco;	
	Float_t			ptreco1;	
	Float_t			etareco1;	
	Float_t			nvalidpixelhitsreco1;	
	Float_t			nvalidmuonhitsreco1;	
	Float_t			nmatch1;	
	Float_t			chi2ndofreco1;	
	Float_t			ptreco2;	
	Float_t			etareco2;	
	Float_t			nvalidpixelhitsreco2;	
	Float_t			nvalidmuonhitsreco2;	
	Float_t			nmatch2;	
	Float_t			chi2ndofreco2;	
	// for lxy
	Float_t			vtxz1;
	Float_t			vtxy1;
	Float_t			bx;
	Float_t			by;
		
	////// Trk (additional)//////
	//gen
	Float_t			eta;	
	Float_t			idparent;	
	Float_t			npixelhits;	
	Float_t			nmuonhits;	
	//reco
	Float_t			etareco;	
	Float_t			nvalidpixelhitsreco;	
	Float_t			nvalidmuonhitsreco;	
	Float_t			nmatch;	
	Float_t			chi2ndofreco;	
	// for lxy
	Float_t			vtxz;
	Float_t			vtxy;

	///////////////////////////////////////
	//Branches
	///////////////////////////////////////
	
	TBranch			*b_pt;	
	TBranch			*b_y;	
	TBranch			*b_pt1;	
	TBranch			*b_eta1;	
	TBranch			*b_npixelhits1;	
	TBranch			*b_nmuonhits1;	
	TBranch			*b_pt2;	
	TBranch			*b_eta2;	
	TBranch			*b_npixelhits2;	
	TBranch			*b_nmuonhits2;	
	//reco
	TBranch			*b_ptreco;	
	TBranch			*b_yreco;	
	TBranch			*b_minvreco;	
	TBranch			*b_ptreco1;	
	TBranch			*b_etareco1;	
	TBranch			*b_nvalidpixelhitsreco1;	
	TBranch			*b_nvalidmuonhitsreco1;	
	TBranch			*b_nmatch1;	
	TBranch			*b_chi2ndofreco1;	
	TBranch			*b_ptreco2;	
	TBranch			*b_etareco2;	
	TBranch			*b_nvalidpixelhitsreco2;	
	TBranch			*b_nvalidmuonhitsreco2;	
	TBranch			*b_nmatch2;	
	TBranch			*b_chi2ndofreco2;	
	// for lxy
	TBranch			*b_vtxz1;
	TBranch			*b_vtxy1;
	TBranch			*b_bx;
	TBranch			*b_by;
		
	////// Trk (additional)//////
	//gen
	TBranch			*b_eta;	
	TBranch			*b_idparent;	
	TBranch			*b_npixelhits;	
	TBranch			*b_nmuonhits;	
	//reco
	TBranch			*b_etareco;	
	TBranch			*b_nvalidpixelhitsreco;	
	TBranch			*b_nvalidmuonhitsreco;	
	TBranch			*b_nmatch;	
	TBranch			*b_chi2ndofreco;	
	// for lxy
	TBranch			*b_vtxz;
	TBranch			*b_vtxy;

	///////////////////////////////////////
	//BranchAddress
	///////////////////////////////////////

	tr->SetBranchAddress("pt",&pt,&b_pt);
	tr->SetBranchAddress("ptreco",&ptreco,&b_ptreco);
	tr->SetBranchAddress("bx",&bx,&b_bx);
	tr->SetBranchAddress("by",&by,&b_by);
	if (isPair) {
		tr->SetBranchAddress("y",&y,&b_y);
		tr->SetBranchAddress("pt1",&pt1,&b_pt1);
		tr->SetBranchAddress("eta1",&eta1,&b_eta1);
		tr->SetBranchAddress("npixelhits1",&npixelhits1,&b_npixelhits1);
		tr->SetBranchAddress("nmuonhits1",&nmuonhits1,&b_nmuonhits1);
		tr->SetBranchAddress("pt2",&pt2,&b_pt2);
		tr->SetBranchAddress("eta2",&eta2,&b_eta2);
		tr->SetBranchAddress("npixelhits2",&npixelhits2,&b_npixelhits2);
		tr->SetBranchAddress("nmuonhits2",&nmuonhits2,&b_nmuonhits2);
		tr->SetBranchAddress("yreco",&yreco,&b_yreco);
		tr->SetBranchAddress("minvreco",&minvreco,&b_minvreco);
		tr->SetBranchAddress("ptreco1",&ptreco1,&b_ptreco1);
		tr->SetBranchAddress("etareco1",&etareco1,&b_etareco1);
		tr->SetBranchAddress("nvalidpixelhitsreco1",&nvalidpixelhitsreco1,&b_nvalidpixelhitsreco1);
		tr->SetBranchAddress("nvalidmuonhitsreco1",&nvalidmuonhitsreco1,&b_nvalidmuonhitsreco1);
		tr->SetBranchAddress("nmatch1",&nmatch1,&b_nmatch1);
		tr->SetBranchAddress("chi2ndofreco1",&chi2ndofreco1,&b_chi2ndofreco1);
		tr->SetBranchAddress("ptreco2",&ptreco2,&b_ptreco2);
		tr->SetBranchAddress("etareco2",&etareco2,&b_etareco2);
		tr->SetBranchAddress("nvalidpixelhitsreco2",&nvalidpixelhitsreco2,&b_nvalidpixelhitsreco2);
		tr->SetBranchAddress("nvalidmuonhitsreco2",&nvalidmuonhitsreco2,&b_nvalidmuonhitsreco2);
		tr->SetBranchAddress("nmatch2",&nmatch2,&b_nmatch2);
		tr->SetBranchAddress("chi2ndofreco2",&chi2ndofreco2,&b_chi2ndofreco2);
		tr->SetBranchAddress("vtxz1",&vtxz1,&b_vtxz1);
		tr->SetBranchAddress("vtxy1",&vtxy1,&b_vtxy1);
	}	
	else {
		tr->SetBranchAddress("eta",&eta,&b_eta);
		tr->SetBranchAddress("etareco",&etareco,&b_etareco);
		tr->SetBranchAddress("idparent",&idparent,&b_idparent);
		tr->SetBranchAddress("npixelhits",&npixelhits,&b_npixelhits);
		tr->SetBranchAddress("nmuonhits",&nmuonhits,&b_nmuonhits);
		tr->SetBranchAddress("nvalidpixelhitsreco",&nvalidpixelhitsreco,&b_nvalidpixelhitsreco);
		tr->SetBranchAddress("nvalidmuonhitsreco",&nvalidmuonhitsreco,&b_nvalidmuonhitsreco);
		tr->SetBranchAddress("nmatch",&nmatch,&b_nmatch);
		tr->SetBranchAddress("chi2ndofreco",&chi2ndofreco,&b_chi2ndofreco);
		tr->SetBranchAddress("vtxz",&vtxz,&b_vtxz);
		tr->SetBranchAddress("vtxy",&vtxy,&b_vtxy);
	}

	//2D hist. just check
	TH2D* hGenPtY = new TH2D("hGenPtY","hGenPtY",50,rapmin,rapmax,50,ptmin,ptmax);
	TH2D* hRecoPtY = new TH2D("hRecoPtY","hRecoPtY",50,rapmin,rapmax,50,ptmin,ptmax);
	TH2D* hEffPtY = new TH2D("hEffPtY","hEffPtY",50,rapmin,rapmax,50,ptmin,ptmax);
	
	TH1D* hGenPt = new TH1D("hGenPt","hGenPt",ptbin,ptmin,ptmax);
	TH1D* hRecoPt = new TH1D("hRecoPt","hRecoPt",ptbin,ptmin,ptmax);
	TH1D* hEffPt = new TH1D("hEffPt","hEffPt",ptbin,ptmin,ptmax);
	TH1D* hGenRap = new TH1D("hGenRap","hGenRap",rapbin,rapmin,rapmax);
	TH1D* hRecoRap = new TH1D("hRecoRap","hRecoRap",rapbin,rapmin,rapmax);
	TH1D* hEffRap = new TH1D("hEffRap","hEffRap",rapbin,rapmin,rapmax);
	TH1D* hGenLxy = new TH1D("hGenLxy","hGenLxy",lxybin,lxymin,lxymax);
	TH1D* hRecoLxy = new TH1D("hRecoLxy","hRecoLxy",lxybin,lxymin,lxymax);
	TH1D* hEffLxy = new TH1D("hEffLxy","hEffLxy",lxybin,lxymin,lxymax);

	hGenPtY->Sumw2();
	hRecoPtY->Sumw2();
	hEffPtY->Sumw2();
	hGenPt->Sumw2();
	hRecoPt->Sumw2();
	hEffPt->Sumw2();
	hGenRap->Sumw2();
	hRecoRap->Sumw2();
	hEffRap->Sumw2();
	hGenLxy->Sumw2();
	hRecoLxy->Sumw2();
	hEffLxy->Sumw2();

	//event loop start
	if(nevt == -1) nevt = tr->GetEntries();	
	for (int iev=initev; iev<nevt; iev++){
		tr->GetEntry(iev);	
		
		bool yngen = false;	
		bool ynreco = false;	
	  float weightF = 1.;
		float lxy = -531.;

		/// give a cut to Gen and Reco separately 
		if (isPair) {
			if (isPtCut) { if ( !(pt<15.) ) continue; }	
			/// define IP
			lxy = TMath::Sqrt((vtxz1-bx)*(vtxz1-bx)+(vtxy1-by)*(vtxy1-by));	
			
			if (kineCut(pt1,eta1) && kineCut(pt2,eta2) 
			&& idCut(npixelhits1,nmuonhits1) 
			&& idCut(npixelhits2,nmuonhits2) )
				{ yngen=true; }
			if (kineCut(ptreco1,etareco1) && kineCut(ptreco2,etareco2) 
			&& idCut(nvalidpixelhitsreco1,nvalidmuonhitsreco1)
			&& idCut(nvalidpixelhitsreco2,nvalidmuonhitsreco2)
			//&& idRecoCut(nmatch1, chi2ndofreco1)
			//&& idRecoCut(nmatch2, chi2ndofreco2)
			&& chi2ndofreco1 < 4.
			&& chi2ndofreco2 < 4.
			&& massCut(minvreco) ) 
				{ ynreco=true;}
		}
		else {
			if (isPtCut) { if (!(pt >1. && pt<15. ) ) continue; }	
			/// define IP
			lxy = TMath::Sqrt((vtxz-bx)*(vtxz-bx)+(vtxy-by)*(vtxy-by));	
			
			if ( (!isEmbedded)
		 	&& TMath::Abs(eta) < 2.4
			&& TMath::Abs(idparent)>442 
			&& TMath::Abs(idparent)<550 
			&& idCut(npixelhits,nmuonhits) )
				{ yngen=true; }
			else if ( isEmbedded
		 	&& TMath::Abs(eta) < 2.4
			//&& TMath::Abs(idparent)>442 
			//&& TMath::Abs(idparent)<550 
			&& idCut(npixelhits,nmuonhits) )
				{ yngen=true; }
			if ( (!isEmbedded) 
			//TMath::Abs(etareco) < 2.4
			&& TMath::Abs(eta) < 2.4
			&& TMath::Abs(idparent)>442 
			&& TMath::Abs(idparent)<550 
			&& idCut(nvalidpixelhitsreco,nvalidmuonhitsreco)
			&& idRecoCut(nmatch, chi2ndofreco) )
				{ ynreco=true;}
			else if ( isEmbedded 
			//TMath::Abs(etareco) < 2.4
			&& TMath::Abs(eta) < 2.4
			//&& TMath::Abs(idparent)>442 
			//&& TMath::Abs(idparent)<550 
			&& idCut(nvalidpixelhitsreco,nvalidmuonhitsreco)
			&& idRecoCut(nmatch, chi2ndofreco) )
				{ ynreco=true;}
		}

		/// weight if "doWeight" && fill the histogram
		if (isPair){
			if (doWeight) {weightF = getWeight(isPrompt, isPair, pt,y);}
//				cout << "weightF = " <<weightF << endl;
			if (yngen)	{
				hGenPtY->Fill(y,pt,weightF);
				hGenPt->Fill(pt,weightF);
				hGenRap->Fill(y,weightF); 
				hGenLxy->Fill(lxy,weightF); 
			}
			if (ynreco) {
				hRecoPtY->Fill(yreco,ptreco,weightF);
				hRecoPt->Fill(ptreco, weightF);
				hRecoRap->Fill(yreco, weightF); 
				hRecoLxy->Fill(lxy, weightF); 
			}
		}else { 
			if (doWeight) {weightF = getWeight(isPrompt, isPair, pt,eta);}
//				cout << "weightF = " <<weightF << endl;
			if (yngen)	{
				hGenPtY->Fill(eta,pt,weightF);
				hGenPt->Fill(pt,weightF);
				hGenRap->Fill(eta,weightF); 
				hGenLxy->Fill(lxy,weightF); 
			}
			if (ynreco) {
				hRecoPtY->Fill(etareco,ptreco,weightF);
				hRecoPt->Fill(ptreco,weightF);
				hRecoRap->Fill(etareco,weightF);
				hRecoLxy->Fill(lxy, weightF); 
			}
		}
	}// end of event loop

	cout << "hGenPt integral = " << hGenPt->Integral() << endl;
	cout << "hRecoPt integral = " << hRecoPt->Integral() << endl;
	cout << "hGenRap integral = " << hGenRap->Integral() << endl;
	cout << "hRecoRap integral = " << hRecoRap->Integral() << endl;
	cout << "hGenLxy integral = " << hGenLxy->Integral() << endl;
	cout << "hRecoLxy integral = " << hRecoLxy->Integral() << endl;

	//calculate efficiency
	hEffPtY->Divide(hRecoPtY,hGenPtY,1,1,"b");
	hEffPt->Divide(hRecoPt,hGenPt,1,1,"b");
	hEffRap->Divide(hRecoRap,hGenRap,1,1,"b");
	hEffLxy->Divide(hRecoLxy,hGenLxy,1,1,"b");

	//Legend
	TLegend *legUR = new TLegend(0.56,0.68,0.90,0.90,NULL,"brNDC");
	TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
	TLegend *legBM = new TLegend(0.40,0.20,0.75,0.35,NULL,"brNDC");
	TLegend *legBR = new TLegend(0.56,0.20,0.90,0.42,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legUL);
	SetLegendStyle(legBM);
	SetLegendStyle(legBR);

	TCanvas *c1 = new TCanvas("c1","c1",600,600); 
	
	//draw 2D Pt-y
	TPaletteAxis* pal; 
	hEffPtY->SetMinimum(0.0);
	hEffPtY->SetMaximum(1.0);
	if (isPair) hEffPtY->GetXaxis()->SetTitle("y");
	else hEffPtY->GetXaxis()->SetTitle("#eta");
	hEffPtY->GetXaxis()->CenterTitle();
	hEffPtY->GetYaxis()->SetTitle("p_{T} (GeV)");
	hEffPtY->Draw("colz");
	c1->Update();
	pal = (TPaletteAxis*)hEffPtY->GetListOfFunctions()->FindObject("palette"); 
	pal->SetX2NDC(0.92);
	c1->Modified();
	c1->Update();
	c1->SaveAs(Form("%s/EffPtY_isPtCut%d_doWeight%d_isEmbedded%d_%s_%s_%s.pdf",strDir.c_str(),(int)isPtCut,(int)doWeight,(int)isEmbedded,strPrompt.c_str(),strEmbd.c_str(),strPair.c_str()));
	c1->Clear();

	//draw EffPt
	SetHistStyle(hEffPt,3,0);
	hEffPt->SetMinimum(0.0);
	hEffPt->SetMaximum(1.0);
	if (isPair) hEffPt->GetXaxis()->SetTitle("Pair p_{T} (GeV)");
	else hEffPt->GetXaxis()->SetTitle("Trk p_{T} (GeV)");
	hEffPt->GetYaxis()->SetTitle("Efficiency");
	hEffPt->Draw("pe");
	c1->SaveAs(Form("%s/EffPt_isPtCut%d_doWeight%d_isEmbedded%d_%s_%s_%s.pdf",strDir.c_str(),(int)isPtCut,(int)doWeight,(int)isEmbedded,strPrompt.c_str(),strEmbd.c_str(),strPair.c_str()));
	c1->Clear();

	//draw EffRap
	SetHistStyle(hEffRap,3,0);
	hEffRap->SetMinimum(0.0);
	hEffRap->SetMaximum(1.0);
	if (isPair) hEffRap->GetXaxis()->SetTitle("y");
	else hEffRap->GetXaxis()->SetTitle("#eta");
	hEffRap->GetXaxis()->CenterTitle();
	hEffRap->GetYaxis()->SetTitle("Efficiency");
	hEffRap->Draw("pe");
	c1->SaveAs(Form("%s/EffRap_isPtCut%d_doWeight%d_isEmbedded%d_%s_%s_%s.pdf",strDir.c_str(),(int)isPtCut,(int)doWeight,(int)isEmbedded,strPrompt.c_str(),strEmbd.c_str(),strPair.c_str()));
	c1->Clear();

	//draw EffLxy
	SetHistStyle(hEffLxy,3,0);
	hEffLxy->SetMinimum(0.0);
	hEffLxy->SetMaximum(1.0);
	hEffLxy->GetXaxis()->SetTitle("Lxy");
	hEffLxy->GetYaxis()->SetTitle("Efficiency");
	hEffLxy->Draw("pe");
	c1->SaveAs(Form("%s/EffLxy_isPtCut%d_doWeight%d_isEmbedded%d_%s_%s_%s.pdf",strDir.c_str(),(int)isPtCut,(int)doWeight,(int)isEmbedded,strPrompt.c_str(),strEmbd.c_str(),strPair.c_str()));
	c1->Clear();

	//save as a root file
	TFile* outFile = new TFile(Form("%s/Eff_isPtCut%d_doWeight%d_isEmbedded%d_%s_%s_%s.root",strDir.c_str(),(int)isPtCut,(int)doWeight,(int)isEmbedded,strPrompt.c_str(),strEmbd.c_str(),strPair.c_str()),"RECREATE");	
	outFile->cd();
	hGenPtY->Write();
	hRecoPtY->Write();
	hEffPtY->Write();
	
	hGenPt->Write();
	hGenRap->Write();
	hGenLxy->Write();
	hRecoPt->Write();
	hRecoRap->Write();
	hRecoLxy->Write();
	hEffPt->Write();
	hEffRap->Write();
	hEffLxy->Write();
	outFile->Close();	
	
	return;

}

///////////////////////////////////////////////////////

//bool kineCut(float muPt, float muEta, float muP) {
//	return ( TMath::Abs(muEta) < 2.4 &&
//			((TMath::Abs(muEta) < 1.3 && muPt >=3.3) ||
//			(1.3 <= TMath::Abs(muEta) && TMath::Abs(muEta) < 2.2 && muP >=2.9) ||
//			(2.2 <= TMath::Abs(muEta) && muPt >= 0.8)));
//}

bool kineCut(float muPt, float muEta) {
	return ( TMath::Abs(muEta) < 2.4 &&
			((TMath::Abs(muEta) < 1.3 && muPt >=3.3) ||
			(1.3 <= TMath::Abs(muEta) && TMath::Abs(muEta) < 2.2 && (muPt/sin(2*atan(exp(-muEta))))>2.9) ||
			(2.2 <= TMath::Abs(muEta) && muPt >= 0.8)));
}

bool massCut(float lv_dimu_mass) {
	 return ( 2.6 <= lv_dimu_mass && lv_dimu_mass < 3.5 );
}

bool idCut(float t_npixel, float t_nmuon) {
	return (t_npixel > 2. 
					&& t_nmuon > 0.);
}

bool idRecoCut(float t_nmatch, float t_chi) {
	 return ( t_nmatch > 0. 
	         && t_chi < 4. );
}

float getWeight(bool t_isPrompt, bool t_isPair, float t_pt, float t_eta) {

	TH1D* hTmp;
	if (t_isPrompt) {
		if (t_isPair) { 
			if ( TMath::Abs(t_eta) < 0.8) hTmp = hPairRatio_PR_0;
			else if ( TMath::Abs(t_eta) < 1.2) hTmp = hPairRatio_PR_1;  
			else if ( TMath::Abs(t_eta) < 2.0) hTmp = hPairRatio_PR_2; 
			else hTmp = hPairRatio_PR_3; 
		} else {
			if ( TMath::Abs(t_eta) < 0.8) hTmp = hTrkRatio_PR_0;
			else if ( TMath::Abs(t_eta) < 1.2) hTmp = hTrkRatio_PR_1;  
			else if ( TMath::Abs(t_eta) < 2.0) hTmp = hTrkRatio_PR_2; 
			else hTmp = hTrkRatio_PR_3; 
		}		
	}else {
		if (t_isPair) { 
			if ( TMath::Abs(t_eta) < 0.8) hTmp = hPairRatio_NP_0;
			else if ( TMath::Abs(t_eta) < 1.2) hTmp = hPairRatio_NP_1;  
			else if ( TMath::Abs(t_eta) < 2.0) hTmp = hPairRatio_NP_2; 
			else hTmp = hPairRatio_NP_3; 
		} else {
			if ( TMath::Abs(t_eta) < 0.8) hTmp = hTrkRatio_NP_0;
			else if ( TMath::Abs(t_eta) < 1.2) hTmp = hTrkRatio_NP_1;  
			else if ( TMath::Abs(t_eta) < 2.0) hTmp = hTrkRatio_NP_2; 
			else hTmp = hTrkRatio_NP_3; 
		}		
	}

	int tmpbin = hTmp->FindBin(t_pt);
	float tmpW = hTmp->GetBinContent(tmpbin);
	return tmpW;

}
