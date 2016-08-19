//void nc_totalComp()
{
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat(".3f");
  //gStyle->SetPaintTextFormat(".5f");
  
  TFile* f01 = TFile::Open("EffAna_8rap9pt_pp_NP_newcut_Zvtx0_SF0_ptSpectraTest_gen.root");
  TFile* f02 = TFile::Open("EffAna_8rap9pt_pp_NP_newcut_Zvtx0_SF0_ptSpectraTest_dimu.root");
  //TFile* f01 = TFile::Open("EffAna_8rap9pt_pp_NP_newcut_Zvtx0_SF0_ptSpectraTest_gen.root");
  
  TCanvas* c1 = new TCanvas("c1","",600,600);
  TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Eff_pt_y");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Den_pt_y_fine");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Num_pt_y_fine");
  //TH2D* h2D_01 = (TH2D*)f01->Get("h2D_Eff_pt_y_fine");
  h2D_01->SetName("h2D_01");

  TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Eff_pt_y");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Den_pt_y_fine");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Num_pt_y_fine");
  //TH2D* h2D_02 = (TH2D*)f02->Get("h2D_Eff_pt_y_fine");
  h2D_02->SetName("h2D_02");
  
  //TH2D* h2D_03 = (TH2D*)f03->Get("h2D_Eff_pt_y");
  //TH2D* h2D_03 = (TH2D*)f03->Get("h2D_Den_pt_y_fine");
  //TH2D* h2D_03 = (TH2D*)f03->Get("h2D_Num_pt_y_fine");
  //TH2D* h2D_03 = (TH2D*)f03->Get("h2D_Eff_pt_y_fine");
  //h2D_03->SetName("h2D_03");
    
  TH1D* h1D_01 = h2D_01->ProjectionY("h1D_01",1,1);
  TH1D* h1D_02 = h2D_02->ProjectionY("h1D_02",1,1);
  //TH1D* h1D_03 = h2D_03->ProjectionY("h1D_03",1,1);

  h1D_01->SetLineColor(kRed);
  h1D_02->SetLineColor(kBlue);
  //h1D_03->SetLineColor(kGreen);
  
  //h1D_01->Rebin(5);
  //h1D_02->Rebin(5);
  //h1D_03->Rebin(5);

  h1D_01->SetMaximum(1.1);
  h1D_01->Draw("hist");
  h1D_02->Draw("hist same");
  //h1D_03->Draw("hist same");
  
  TLine* t1 = new TLine(0,1,30,1);
  t1->SetLineStyle(7);
  t1->Draw();

//  TLegend *leg = new TLegend(0.55,0.70,0.90,0.90,NULL,"brNDC");
  TLegend *leg = new TLegend(0.15,0.70,0.50,0.90,NULL,"brNDC");
  //leg->AddEntry(h1D_01,"/gen","l");
  //leg->AddEntry(h1D_02,"/dimu","l");
  leg->AddEntry(h1D_01,"Den gen","l");
  leg->AddEntry(h1D_02,"Den dimu","l");
//  leg->AddEntry(h1D_01,"Jpsi/Jpsi","l");
//  leg->AddEntry(h1D_02,"dimu/Jpsi","l");
//  leg->AddEntry(h1D_03,"dimu/dimu","l");
  leg->Draw();

  //h2D_01->Divide(h2D_02);
  //h2D_01->SetMaximum(1.3);  
  //h2D_01->SetMaximum(1.7);  
  //h2D_01->Draw("text e colz");

  TCanvas* c2 = new TCanvas("c2","",600,600);
  TH1D* h1D_03 = new TH1D("h1D_03","",100,0,30);
  //TH1D* h1D_03 = new TH1D("h1D_03","",20,0,30);
  //TH1D* h1D_03 = new TH1D("h1D_03","",1,0,30);
  h1D_03->SetName("h1D_03");
  h1D_03->Divide(h1D_01,h1D_02,1,1);
  h1D_03->GetYaxis()->SetRangeUser(0.8,1.2);
  h1D_03->SetMarkerStyle(kOpenCircle);
  h1D_03->Draw("pe");
  t1->Draw();

}
