#include <TROOT.h>
#include <TFile.h>
#include <iostream>
#include <TSystem.h>
#include <TTree.h>
#include <TKey.h>
#include <TH1.h>
#include <TH2.h>
#include <TPave.h>
#include <TText.h>
#include <sstream>
#include <string.h>
#include <TFile.h>

#include <TGraphAsymmErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>

#include <RooFit.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooArgSet.h>

#include <TStyle.h>
#include <TLatex.h>
#include <TDirectory.h>
#include <TCollection.h>
#include <TPostScript.h>

using namespace RooFit;
using namespace std;

// Function Define
TH2F *plotEff2D(RooDataSet *a, TString b);
TGraphAsymmErrors *plotEffPt(RooDataSet *a, int aa);
TGraphAsymmErrors *plotEffEta(RooDataSet *a, int aa);
void formatTH1F(TH1* a, int b, int c, int d);
void formatTGraph(TGraph* a, int b, int c, int d);
void formatTLeg(TLegend* a);
void TnPDrawMass_test_5eta4();

const int ptBins = 9; //1
const int ptBins1 = 9;//1
//const int ptBins = 11; //2
//const int ptBins1 = 11; //2
//const int ptBins = 13; //4
//const int ptBins1 = 13; //4

TString Cat1 = "RD"; // RD or RD
TString Cat2 = "MuIdtrg"; // Trg, Trk or MuId 
TString data_set = "pPb"; // ReReco, Prompt Reco or RD weighted

//TString outfile_ = "Jpsi_pPb_RD_v24_Mass_Calo_nominal_v7_eta2_nominal_v6";
TString outfile_ = "Jpsi_pPb_RD_v24_Mass_Calo_Eta4_nominal_v8.ps";
//TString outfile_ = "Jpsi_pPb_RD_v24_Mass_Calo_nominal_v7_Eta3_nominal_v6";
//TString outfile_ = "Jpsi_pPb_RD_v24_Mass_Calo_nominal_v7_Eta4_nominal_v6";
//TString outfile_ = "Jpsi_pPb_RD_v24_Mass_Calo_nominal_v7_Eta5_nominal_v6";

void TnPDrawMass_test_5eta4()
{
    char *infile;
    //char *files = "/afs/cern.ch/work/g/goni/cms760pre4/src/test_TNP/TnP/mc/out/gputEveryCut_mc_allDirection_v24_nominalCond_5etaBins_cutG_all_2G_v1.root";
    //char *files = "/afs/cern.ch/work/g/goni/cms760pre4/src/test_TNP/TnP/mc/out/gputEveryCut_mc_allDirection_v24_nominalCond_5etaBins_cutG_all_Multi_v1.root";
    //char *files = "/afs/cern.ch/work/g/goni/cms760pre4/src/test_TNP/TnP/mc/out/gputEveryCut_mc_allDirection_v24_nominalCond_5etaBins_cutG_all_PAMu7_v1.root";
    //char *files = "/afs/cern.ch/work/g/goni/cms760pre4/src/test_TNP/TnP/mc/out/gputEveryCut_mc_allDirection_v24_nominalCond_5etaBins_cutG_all_merge_v1.root";
    //char *files = "/afs/cern.ch/work/g/goni/cms760pre4/src/test_TNP/TnP/mc/out/gputEveryCut_mc_allDirection_v24_nominalCond_5etaBins_cutG_all_narrow_v1.root";
    char *files = "/afs/cern.ch/work/g/goni/cms760pre4/src/test_TNP/TnP/data/outputEveryCut_data_allDirection_v24_nominalCond_5etaBins_cutG_all_nominal_v8_oldeta.root";

    TString outname_in, outname_mid, outname_out;
    TString middle_name, middle_name2;

    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    gStyle->SetPaperSize(20,26);
    TCanvas *c1 = new TCanvas();
    Int_t type = 112;
    TString out1 = outfile_ + "[";
		c1->SetLogy();
    c1->Print(out1);

    infile = files;
    TFile *f = new TFile(infile);

    outname_in = "Jpsi_" + Cat1 + "_" + Cat2 + "_Mass_" + middle_name + ".ps[";
    outname_mid = "Jpsi_" + Cat1 + "_" + Cat2 + "_Mass_" + middle_name + ".ps";
    outname_out = "Jpsi_" + Cat1 + "_" + Cat2 + "_Mass_" + middle_name + ".ps]";


    cout<<" Out Name : "<<outname_in<<" "<<outname_mid<<" "<<outname_in<<endl;
        

    TString tot_dir; //dir_suffix;
    
    //TString ptbins[ptBins] = {"pt_bin0_", "pt_bin1_", "pt_bin2_", "pt_bin3_", "pt_bin4_",
		//								"pt_bin5_", "pt_bin6_", "pt_bin7_", "pt_bin8_",};//1
    //TString ptbins1[ptBins1] = {"3.3 - 3.8 GeV", "3.8 - 4.3  GeV", "4.3 - 5 GeV", "5 - 6 GeV", "6 - 7 GeV",
		//								"7 - 8 GeV", "8 - 10 GeV", "10 - 13 GeV", "13 - 30 GeV"};//1
  
    TString dir_pt[ptBins] = {"tpTree/ptBin_eta4_0/", "tpTree/ptBin_eta4_1/", "tpTree/ptBin_eta4_2/", "tpTree/ptBin_eta4_3/", "tpTree/ptBin_eta4_4/", "tpTree/ptBin_eta4_5/", "tpTree/ptBin_eta4_6/","tpTree/ptBin_eta4_7/","tpTree/ptBin_eta4_8/"};//2
    TString ptbins1[ptBins1] = {"1.3 - 1.6 GeV", "1.6 - 2.2 GeV", "2.2 - 2.5  GeV", "2.5 - 2.8 GeV", "2.8 - 3.5 GeV",
                    "3.5 - 4.5 GeV", "4.5 - 5.0 GeV", "5.0 - 6.0 GeV", "6.0 - 30 GeV"};//4


    //TString ptbins[ptBins] = {"pt_bin0_", "pt_bin1_", "pt_bin2_", "pt_bin3_", "pt_bin4_",
	  //								"pt_bin5_", "pt_bin6_", "pt_bin7_", "pt_bin8_", "pt_bin9_"};//3
    //TString ptbins1[ptBins1] = {"0 - 3.3 GeV", "3.3 - 3.8  GeV", "3.8 - 4.3 GeV", "4.3 - 5 GeV", "5 -6 GeV",
		//								"6 - 7 GeV", "7 - 8 GeV", "8 - 10 GeV", "10 - 13 GeV", "13 - 30 GeV"};//3
    
    //TString ptbins[ptBins] = {"pt_bin0_", "pt_bin1_", "pt_bin2_", "pt_bin3_", "pt_bin4_",
	  //								"pt_bin5_", "pt_bin6_", "pt_bin7_", "pt_bin8_", "pt_bin9_"};//4
    //TString ptbins1[ptBins1] = {"0 - 3.3 GeV", "3.3 - 3.8  GeV", "3.8 - 4.3 GeV", "4.3 - 5 GeV", "5 -6 GeV",
		//								"6 - 7 GeV", "7 - 8 GeV", "8 - 10 GeV", "10 - 13 GeV", "13 - 30 GeV"};//4
     
    //TString ptbins[ptBins] = {"pt_bin0_", "pt_bin1_", "pt_bin2_", "pt_bin3_", "pt_bin4_",
	  //								"pt_bin5_", "pt_bin6_", "pt_bin7_", "pt_bin8_", "pt_bin9_"};//5
    //TString ptbins1[ptBins1] = {"0 - 3.3 GeV", "3.3 - 3.8  GeV", "3.8 - 4.3 GeV", "4.3 - 5 GeV", "5 -6 GeV",
		//								"6 - 7 GeV", "7 - 8 GeV", "8 - 10 GeV", "10 - 13 GeV", "13 - 30 GeV"};//5

    //dir_pt = "tpTree/ptBin_eta1/";
    //dir_pt = "tpTree/ptBin_eta3/";
    //dir_pt = "tpTree/ptBin_eta3/";
    //dir_pt = "tpTree/ptBin_eta4/";
    //dir_pt = "tpTree/ptBin_eta5/";

    //dir_suffix = "_cbGaussPlusPoly1";
    //dir_suffix = "_cbGaussPlusPoly4";
    TString dir_suffix[ptBins]= {"_cbGaussPlusExpo4_0","_cbGaussPlusExpo4_1","_cbGaussPlusExpo4_2","_cbGaussPlusExpo4_3","_cbGaussPlusExpo4_4","_cbGaussPlusExpo4_5","_cbGaussPlusExpo4_6","_cbGaussPlusExpo4_7","_cbGaussPlusExpo4_8"};
    //dir_suffix = "_GaussPlusExpo";
    //dir_suffix = "_twoGaussPlusExpo";
    gStyle->SetPaperSize(20,26);

    c1->Print(outname_in);
    int cnt = 0;

    TCanvas *tmp1 = new TCanvas();
    tmp1->cd();

    TString title;
    title = "J/#psi Efficiency Heavy Ion TnP Result (" + middle_name2 + ")";

    c1 = (TCanvas *)tmp1->Clone();
    c1->Print(outname_mid);
    c1->Print(outfile_);

    TString mid_title = "Centrality : (" + middle_name2 + ")";
    TString leg_title = Cat2 + " Efficiency (" + middle_name2 + ")";

    // pt
    for(int j = 0; j < ptBins; j++){
        //TString tot_dir = dir_pt + "abseta_bin0__event_PrimaryVertex_z_bin0__" + ptbins[j] + "_tag_eta_bin0__tag_pt_bin0__Acc_JPsi_pass__Calo_pass__tag_PAMu3_standard_pass_" + dir_suffix;
        TString tot_dir = dir_pt[j] + "abseta_bin0__event_PrimaryVertex_z_bin0__pt_bin0__tag_eta_bin0__tag_pt_bin0__Acc_JPsiG_pass__Calo_pass__tag_PAMu3_standard_pass_" + dir_suffix[j];//2G,PAMu3,merge
        //TString tot_dir = dir_pt + "abseta_bin0__event_PrimaryVertex_z_bin0__" + ptbins[j] + "_tag_eta_bin0__tag_pt_bin0__Acc_JPsiG_pass__Calo_pass__tag_PAMu7_standard_pass_" + dir_suffix;//PAMu7
        //TString tot_dir = dir_pt + "abseta_bin0__event_PrimaryVertex_z_bin0__pair_probeMultiplicity_bin0__" + ptbins[j] + "_tag_eta_bin0__tag_pt_bin0__Acc_JPsiG_pass__Calo_pass__tag_PAMu3_standard_pass_" + dir_suffix;//multi
        //TString tot_dir = dir_pt + "abseta_bin0__event_PrimaryVertex_z_bin0__" + ptbins[j] + "_tag_eta_bin0__tag_pt_bin0__Acc_JPsi_pass__Calo_pass__tag_PAMu3_standard_pass_" + dir_suffix;//merge
        //
      
  
        f->cd(tot_dir);
        cout<<" tot_dir : "<<tot_dir<<endl;
        TDirectory *root_dir = gDirectory;
        TIter rootnextkey( root_dir->GetListOfKeys() );
        root_dir->cd();
        TKey *rootkey;
        TCanvas *ctmp = new TCanvas();
        ctmp->cd();

        TLatex l;
        l.SetTextAlign(13);
        l.SetTextSize(0.06);
        l.DrawLatex(0.1,0.8,mid_title);
        l.DrawLatex(0.1,0.6,"Bin : ");
        l.SetTextSize(0.04);
        //l.DrawLatex(0.1,0.5,tot_dir);
        l.DrawLatex(0.1,0.5,ptbins1[j]);
        

        ctmp->Update();
        c1 = (TCanvas *)ctmp->Clone();
        c1->Print(outname_mid);
        c1->Print(outfile_);

        while( rootkey = (TKey*)rootnextkey() )
        {
            TObject *rootobj = rootkey->ReadObj();
            TDirectory *rdir = gDirectory;
            TIter rdirnextkey(rdir->GetListOfKeys());
            rdir->cd();
            TKey *dir_key;
            while( dir_key = (TKey*)rdirnextkey())
            {
                if (rootobj->IsA()->InheritsFrom("TCanvas")){
                    c1 = (TCanvas *)rootobj;
		c1->SetLogy();
		c1->Modified();
		c1->Update();
                    c1->Print(outname_mid);
                    c1->Print(outfile_);
                    cnt++;
                    cout<<"Count : "<<cnt<<endl;
                    if(cnt > 0) break;
                }
            }
        }
    }
    c1->Print(outname_out);
    TString out2 = outfile_ + "]";
    c1->Print(out2);
}
void formatTH1F(TH1* a, int b, int c, int d){
    a->SetLineWidth(2);
    a->SetLineStyle(c);
    a->SetMarkerSize(2);
    a->SetLineColor(b);
    a->SetMarkerColor(b);
    a->GetYaxis()->SetTitle("Efficiency");
    if(d == 1){
        a->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    }else if(d == 2){
        a->GetXaxis()->SetTitle("#eta");
    }else if(d == 3){
        a->GetXaxis()->SetTitle("rapidity");
    }else if(d == 4){
        a->GetXaxis()->SetTitle("Centrality");
    }
    a->GetXaxis()->CenterTitle(true);
    a->GetXaxis()->SetLabelSize(0.05);
    a->GetXaxis()->SetTitleSize(0.05);
    a->GetXaxis()->SetTitleOffset(0.9);
    a->GetYaxis()->SetLabelSize(0.05);
    a->GetYaxis()->SetTitleSize(0.05);
    a->GetYaxis()->SetTitleOffset(0.8);

}

void formatTLeg(TLegend* a){

    a->SetFillStyle(0);
    a->SetFillColor(0);
    a->SetBorderSize(0);
    a->SetTextSize(0.03);

}


void formatTGraph(TGraph* a, int b, int c, int d)
{

    a->SetMarkerStyle(c);
    a->SetMarkerColor(b);
    a->SetMarkerSize(1.0);
    a->SetLineColor(b);
    a->SetLineWidth(1);
    a->GetXaxis()->SetLabelSize(0.05);
    a->GetXaxis()->SetTitleSize(0.06);
    a->GetXaxis()->SetTitleOffset(0.9);
    a->GetYaxis()->SetTitle("Efficiency");
    a->GetXaxis()->CenterTitle();
    if(d == 1){ 
        a->GetXaxis()->SetTitle("p_{T} (GeV/c)"); 
    }else if(d == 2){ 
        a->GetXaxis()->SetTitle("eta"); 
    }else if(d == 3){ 
        a->GetXaxis()->SetTitle("rapidity"); 
    }else if(d == 4){
        a->GetXaxis()->SetTitle("Centrality");
    }   
    a->GetYaxis()->SetRangeUser(0,1);
    a->GetXaxis()->SetRangeUser(0,10);
    a->GetYaxis()->SetLabelSize(0.05);
    a->GetYaxis()->SetTitleSize(0.05);
    a->GetYaxis()->SetTitleOffset(0.9);


}

TGraphAsymmErrors *plotEffEta(RooDataSet *a, int aa){
    const RooArgSet *set = a->get();
    RooRealVar *xAx = (RooRealVar*)set->find("eta");
    RooRealVar *eff = (RooRealVar*)set->find("efficiency");

    const int nbins = xAx->getBinning().numBins();

    double tx[nbins], txhi[nbins], txlo[nbins];
    double ty[nbins], tyhi[nbins], tylo[nbins];

    for (int i=0; i<nbins; i++) {
        a->get(i);
        ty[i] = eff->getVal();
        tx[i] = xAx->getVal();
        txhi[i] = fabs(xAx->getErrorHi());
        txlo[i] = fabs(xAx->getErrorLo()); 
        tyhi[i] = fabs(eff->getErrorHi());
        tylo[i] = fabs(eff->getErrorLo()); 
    }

    cout<<"NBins : "<<nbins<<endl;

    const double *x = tx;
    const double *xhi = txhi;
    const double *xlo = txlo;
    const double *y = ty;
    const double *yhi = tyhi;
    const double *ylo = tylo;

    TGraphAsymmErrors *b = new TGraphAsymmErrors();
    if(aa == 1) {*b = TGraphAsymmErrors(nbins,x,y,xlo,xhi,ylo,yhi);}
    if(aa == 0) {*b = TGraphAsymmErrors(nbins,x,y,0,0,ylo,yhi);}
    b->SetMaximum(1.1);
    b->SetMinimum(0.0);
    b->SetMarkerStyle(20);
    b->SetMarkerColor(kRed+2);
    b->SetMarkerSize(1.0);
    b->SetTitle("");
    b->GetXaxis()->SetTitleSize(0.05);
    b->GetYaxis()->SetTitleSize(0.05);
    b->GetXaxis()->SetTitle("#eta");
    b->GetYaxis()->SetTitle("Efficiency");
    b->GetXaxis()->CenterTitle();
    //b->Draw("apz");

    for (int i=0; i<nbins; i++) {
        cout << x[i] << " " << y[i] << " " << yhi[i] << " " << ylo[i] << endl;
    }

    return b;

}

TGraphAsymmErrors *plotEffPt(RooDataSet *a, int aa){
    const RooArgSet *set = a->get();
    RooRealVar *xAx = (RooRealVar*)set->find("pt");
    RooRealVar *eff = (RooRealVar*)set->find("efficiency");

    const int nbins = xAx->getBinning().numBins();

    double tx[nbins], txhi[nbins], txlo[nbins];
    double ty[nbins], tyhi[nbins], tylo[nbins];

    for (int i=0; i<nbins; i++) {
        a->get(i);
        ty[i] = eff->getVal();
        tx[i] = xAx->getVal();
        txhi[i] = fabs(xAx->getErrorHi());
        txlo[i] = fabs(xAx->getErrorLo()); 
        tyhi[i] = fabs(eff->getErrorHi());
        tylo[i] = fabs(eff->getErrorLo()); 
    }

    cout<<"NBins : "<<nbins<<endl;

    const double *x = tx;
    const double *xhi = txhi;
    const double *xlo = txlo;
    const double *y = ty;
    const double *yhi = tyhi;
    const double *ylo = tylo;

    TGraphAsymmErrors *b = new TGraphAsymmErrors();
    if(aa == 1) {*b = TGraphAsymmErrors(nbins,x,y,xlo,xhi,ylo,yhi);}
    if(aa == 0) {*b = TGraphAsymmErrors(nbins,x,y,0,0,ylo,yhi);}
    b->SetMaximum(1.1);
    b->SetMinimum(0.0);
    b->SetMarkerStyle(20);
    b->SetMarkerColor(kRed+2);
    b->SetMarkerSize(1.0);
    b->SetTitle("");
    b->GetXaxis()->SetTitleSize(0.05);
    b->GetYaxis()->SetTitleSize(0.05);
    b->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    b->GetYaxis()->SetTitle("Efficiency");
    b->GetXaxis()->CenterTitle();
    //b->Draw("apz");

    for (int i=0; i<nbins; i++) {
        cout << x[i] << " " << y[i] << " " << yhi[i] << " " << ylo[i] << endl;
    }

    return b;

}

TH2F *plotEff2D(RooDataSet *a, TString b){
    const RooArgSet *set = a->get();
    RooRealVar *yAx = (RooRealVar*)set->find("pt");
    RooRealVar *xAx = (RooRealVar*)set->find("eta");
    RooRealVar *eff = (RooRealVar*)set->find("efficiency");

//    const int xnbins = xAx->getBinning().numBins();
//    const int ynbins = yAx->getBinning().numBins();

    //double xbins[] = {-2.4, -1.6, -0.8, 0.0, 0.8, 1.6, 2.4};
    //double ybins[] = {0, 2, 3, 5, 8, 10, 20};

    const double *xvbins = xAx->getBinning().array();
    const double *yvbins = yAx->getBinning().array();

    TH2F* h = new TH2F(b, "", xAx->getBinning().numBins(), xvbins, yAx->getBinning().numBins(), yvbins);

    gStyle->SetPaintTextFormat("5.2f");
    gStyle->SetPadRightMargin(0.12);
    gStyle->SetPalette(1);
    h->SetOption("colztexte");
    h->GetZaxis()->SetRangeUser(-0.001,1.001);
    h->SetStats(kFALSE);
    h->GetYaxis()->SetTitle("p_{T} [GeV/c]");
    h->GetXaxis()->SetTitle("#eta");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetTitleOffset(0.8);
    h->GetXaxis()->SetTitleOffset(0.9);
    for(int i=0; i<a->numEntries(); i++){
        a->get(i);
        h->SetBinContent(h->FindBin(xAx->getVal(), yAx->getVal()), eff->getVal());
        h->SetBinError(h->FindBin(xAx->getVal(), yAx->getVal()), (eff->getErrorHi()-eff->getErrorLo())/2.);
    }

    return h;

}

