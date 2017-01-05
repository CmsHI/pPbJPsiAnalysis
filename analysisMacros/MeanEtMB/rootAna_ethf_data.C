#include "../SONGKYO.h"

int rootAna_ethf_data(){

	//// # of event range
	const int nEntry = 5000;
		
	gROOT->Macro("../Style.C");

	//// read-in root file (data)
	TFile *fDataPbp1; // 210498<= runNb <= 210658 should be rejected
	TFile *fDataPbp2; // only 210498<= runNb <= 210658 will be used : 1st 7 run
	TFile *fDatapPb;
	fDataPbp1 = new TFile("/home/songkyo/kyo/pPbDataSample/Data/RD2013_pa_1st_run_210676-211256_GR_P_V43D_nocut.root");
	fDataPbp2 = new TFile("/home/songkyo/kyo/pPbDataSample/Data/RD2013_pa_1st_run_210498-210658_GR_P_V43F_nocut.root");
	fDatapPb = new TFile("/home/songkyo/kyo/pPbDataSample/Data/RD2013_pa_2nd_run_211313-211631_GR_P_V43D_nocut.root");
	TTree *treeDataPbp1 = (TTree*)fDataPbp1->Get("myTree");
	TTree *treeDataPbp2 = (TTree*)fDataPbp2->Get("myTree");
	TTree *treeDatapPb = (TTree*)fDatapPb->Get("myTree");

	//// cut definitions
	TCut runCut = "runNb >=210498 && runNb <= 210658"; // 1st 7 run 

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
//		treeDataPbp1->Draw(Form("SumET_HFplusEta4+SumET_HFminusEta4>>%s",hEt[i]->GetName()),(!runCut),"",nEntry);
//		treeDataPbp2->Draw(Form("SumET_HFplusEta4+SumET_HFminusEta4>>+%s",hEt[i]->GetName()),runCut,"",nEntry);
//		treeDatapPb->Draw(Form("SumET_HFplusEta4+SumET_HFminusEta4>>+%s",hEt[i]->GetName()),"","",nEntry);
		treeDataPbp1->Draw(Form("SumET_HFplusEta4+SumET_HFminusEta4>>%s",hEt[i]->GetName()),(!runCut),"");
		treeDataPbp2->Draw(Form("SumET_HFplusEta4+SumET_HFminusEta4>>+%s",hEt[i]->GetName()),runCut,"");
		treeDatapPb->Draw(Form("SumET_HFplusEta4+SumET_HFminusEta4>>+%s",hEt[i]->GetName()),"","");
		std::cout << i<<"th hEt entries = " << hEt[i]->GetEntries() << std::endl;
		std::cout << i<<"th hEt integral = " << hEt[i]->Integral() << std::endl;
	}
		
	//// Save data as a root file
	TFile *outFile = new TFile("ethf_data_6rap3pt.root","RECREATE");
	outFile->cd();
	for (Int_t i=0; i<nEtBin; i++){	
		hEt[i]->Write();
	}
	outFile->Close();
	
	c1->cd();
	for (Int_t i=0; i<nEtBin; i++){	
		SetHistStyle(hEt[i],4,0);
		gPad->SetLogy(0);
		hEt[i]->Draw("pe");
		c1->SaveAs(Form("ethf_data_%d.pdf",i));
		c1->Clear();
	}
	
	return 0;	

}

