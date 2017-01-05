#include "../SONGKYO.h"

int rootAna_ethf_minBias_selfnorm(){

	// # of event range
	const int nEntry = 8000;
		
	gROOT->Macro("./Style.C");

	TFile *fDataMB;
	fDataMB = new TFile("/home/songkyo/kyo/pPbDataSample/DataMinBias/MinBiasTree_pPb_211313-211631.root");
	TTree* treeDataMB = new TTree();
	treeDataMB->AddFriend("skimanalysis/HltTree",fDataMB);
	treeDataMB->AddFriend("hiEvtAnalyzer/HiTree",fDataMB);

	//// Event selection
	//TCut trigCut = "HltTree.Event == HiTree.evt && HltTree.HLT_PAZeroBiasPixel_SingleTrack_v1==1";
	TCut evCut = "HltTree.pPAcollisionEventSelection==1 && HltTree.pVertexFilterCutGplus==1";

	// define 1D hist
	//int etBin[] = {100,50,50,50,50,50,50,100,700};
	//double etmin[] = {0., 10., 15., 20., 25., 30., 35., 40., 50.};
	//double etmax[] = {10, 15, 20., 25., 30., 35., 40., 50., 120.};
	int etBin[] = {100,50,50,50,50,50,50,800};
	double etmin[] = {0., 10., 15., 20., 25., 30., 35., 40.};
	double etmax[] = {10, 15, 20., 25., 30., 35., 40., 120.};
	const Int_t nEtBin = sizeof(etBin)/sizeof(int);
	cout << "nEtBin = " << nEtBin << endl;
	
	TCanvas* c1 = new TCanvas("c1","c1",600,600);
	TLegend *legUR = new TLegend(0.60,0.70,0.87,0.87,NULL,"brNDC");
	TLegend *legUM = new TLegend(0.30,0.68,0.65,0.90,NULL,"brNDC");
	TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
	TLegend *legBM = new TLegend(0.30,0.20,0.65,0.42,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legUM);
	SetLegendStyle(legUL);
	SetLegendStyle(legBM);

	TH1D* hEt[nEtBin];
	for (Int_t i=0; i<nEtBin; i++){	
		hEt[i] = new TH1D(Form("hEt_%d",i),";E_{T}^{HF |#eta>4|};Events",etBin[i],etmin[i],etmax[i]);
		hEt[i]->Sumw2();
//		treeDataMB->Draw(Form("HiTree.hiHFeta4>>%s",hEt[i]->GetName()),evCut,"",nEntry);
		treeDataMB->Draw(Form("HiTree.hiHFeta4>>%s",hEt[i]->GetName()),evCut,"");
		std::cout << i<<"th hEt entries = " << hEt[i]->GetEntries() << std::endl;
		std::cout << i<<"th hEt integral = " << hEt[i]->Integral() << std::endl;
	}
		
	/// Save data as a root file
	TFile *outFile = new TFile("ethf_MB_selfnorm.root","RECREATE");
	outFile->cd();
	for (Int_t i=0; i<nEtBin; i++){	
		hEt[i]->Write();
	}
	outFile->Close();
	
	//// Draw normalized distributions to compare 1st & 2nd run
	c1->cd();
	for (Int_t i=0; i<nEtBin; i++){	
		hEt[i]->Scale(1/hEt[i]->Integral());	
		SetHistStyle(hEt[i],4,0);
		gPad->SetLogy(0);
		hEt[i]->Draw("pe");
		c1->SaveAs(Form("ethf_MB_selfnorm_%d.pdf",i));
		c1->Clear();
		legUR->Clear();
	}
	
	return 0;	

}

