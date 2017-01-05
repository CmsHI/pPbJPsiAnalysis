#include "../SONGKYO.h"


int rootAna_ethf_minBias(){

	//// # of event range
	const int nEntry = 8000;
		
	gROOT->Macro("../Style.C");

	TFile *fDataMB;
	fDataMB = new TFile("/home/songkyo/kyo/pPbDataSample/DataMinBias/MinBiasTree_pPb_211313-211631.root");
	TTree* treeDataMB = new TTree();
	treeDataMB->AddFriend("skimanalysis/HltTree",fDataMB);
	treeDataMB->AddFriend("hiEvtAnalyzer/HiTree",fDataMB);

	//// Event selection
	//TCut trigCut = "HltTree.Event == HiTree.evt && HltTree.HLT_PAZeroBiasPixel_SingleTrack_v1==1";
	TCut evCut = "HltTree.pPAcollisionEventSelection==1 && HltTree.pVertexFilterCutGplus==1";

	//// define 1D hist
	int etBin[] = {120, 20, 10, 90};
	double etmin[] = {0., 0., 20., 30.};
	double etmax[] = {120, 20, 30, 120};
	const Int_t nEtBin = sizeof(etBin)/sizeof(int);
	cout << "nEtBin = " << nEtBin << endl;
	
	TCanvas* c1 = new TCanvas("c1","c1",600,600);

	TH1D* hEt[nEtBin];
	for (Int_t i=0; i<nEtBin; i++){	
		hEt[i] = new TH1D(Form("hEt_%d",i),";E_{T}^{HF |#eta>4|};Events",etBin[i],etmin[i],etmax[i]);
		hEt[i]->Sumw2();
//		treeDataMB->Draw(Form("HiTree.hiHFeta4>>%s",hEt[i]->GetName()),evCut,"",nEntry);
		treeDataMB->Draw(Form("HiTree.hiHFeta4>>%s",hEt[i]->GetName()),evCut,"");
		std::cout << i<<"th hEt entries = " << hEt[i]->GetEntries() << std::endl;
		std::cout << i<<"th hEt integral = " << hEt[i]->Integral() << std::endl;
	}
		
	//// Save data as a root file
	TFile *outFile = new TFile("ethf_MB_6rap3pt.root","RECREATE");
	outFile->cd();
	for (Int_t i=0; i<nEtBin; i++){	
		hEt[i]->Write();
	}
	outFile->Close();
	
	//// Draw normalized distributions to compare 1st & 2nd run
	c1->cd();
	for (Int_t i=0; i<nEtBin; i++){	
		SetHistStyle(hEt[i],4,0);
		gPad->SetLogy(0);
		hEt[i]->Draw("pe");
		c1->SaveAs(Form("ethf_MB_%d.pdf",i));
		c1->Clear();
	}
	
	return 0;	

}

