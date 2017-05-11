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
void TnPDrawMass_test_5eta1();

const int etaBins = 18; //1
const int etaBins1 = 18;//1

TString Cat1 = "MC"; // RD or RD
TString Cat2 = "MuIdtrg"; // Trg, Trk or MuId 
TString data_set = "pPb"; // ReReco, Prompt Reco or RD weighted

TString outfile_ = "Jpsi_pPb_MC_v24_Mass_Calo_nominal_v9.ps";

void TnPDrawMass_test_5eta1()
{
    char *infile;
    char *files = "/home/kisoo/work/pPb_Jpsi/tagAndProbe/DataFile/outputEveryCut_mc_allDirection_v24_nominalCond_5etaBins_cutG_all_nominal_v9.root";

    TString outname_in, outname_mid, outname_out;
    TString middle_name, middle_name2;

    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    gStyle->SetPaperSize(20,26);
    TCanvas *c1 = new TCanvas();
    Int_t type = 112;
    TString out1 = outfile_ + "[";
    c1->Print(out1);

    infile = files;
    TFile *f = new TFile(infile);

    outname_in = "Jpsi_" + Cat1 + "_" + Cat2 + "_Mass_" + middle_name + ".ps[";
    outname_mid = "Jpsi_" + Cat1 + "_" + Cat2 + "_Mass_" + middle_name + ".ps";
    outname_out = "Jpsi_" + Cat1 + "_" + Cat2 + "_Mass_" + middle_name + ".ps]";

    cout<<" Out Name : "<<outname_in<<" "<<outname_mid<<" "<<outname_in<<endl;
        
    TString tot_dir; //dir_suffix;
    
    TString dir_eta[etaBins] = {"tpTree/etaBin0/", "tpTree/etaBin1/", "tpTree/etaBin2/", "tpTree/etaBin3/", "tpTree/etaBin4/","tpTree/etaBin5/", "tpTree/etaBin6/", "tpTree/etaBin7/", "tpTree/etaBin8/", "tpTree/etaBin9/", "tpTree/etaBin10/", "tpTree/etaBin11/", "tpTree/etaBin12/", "tpTree/etaBin13/", "tpTree/etaBin14/", "tpTree/etaBin15/", "tpTree/etaBin16/", "tpTree/etaBin17/"};//2
    TString etabins1[etaBins1] = {"-2.4 - -2.2", "-2.2 - -2.1", "-2.1 - -1.8", "-1.8 - -1.6", "-1.6 - -1.3", "-1.3 - -1.2", "-1.2 - -0.9", "-0.9 - -0.6", "-0.6 - 0.0", "0.0 - 0.6", "0.6 - 0.9", "0.9 - 1.2", "1.2 - 1.3", "1.3 - 1.6", "1.6 - 1.8", "1.8 - 2.1", "2.1 - 2.2", "2.2 - 2.4"};//1

    TString dir_suffix[etaBins]= {"_cbGaussPlusExpo0","_cbGaussPlusExpo1","_cbGaussPlusExpo2","_cbGaussPlusExpo3","_cbGaussPlusExpo4","_cbGaussPlusExpo5","_cbGaussPlusExpo6","_cbGaussPlusExpo7","_cbGaussPlusExpo8","_cbGaussPlusExpo9","_cbGaussPlusExpo10","_cbGaussPlusExpo11","_cbGaussPlusExpo12","_cbGaussPlusExpo13","_cbGaussPlusExpo14","_cbGaussPlusExpo15","_cbGaussPlusExpo16","_cbGaussPlusExpo17"};

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

    // eta
    for(int j = 0; j < etaBins; j++){
        TString tot_dir = dir_eta[j] + "eta_bin0__event_PrimaryVertex_z_bin0__tag_eta_bin0__tag_pt_bin0__Acc_JPsiG_pass__Calo_pass__tag_PAMu3_standard_pass_" + dir_suffix[j];//2G,PAMu3,merge
  
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
        l.DrawLatex(0.1,0.5,etabins1[j]);
        

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

