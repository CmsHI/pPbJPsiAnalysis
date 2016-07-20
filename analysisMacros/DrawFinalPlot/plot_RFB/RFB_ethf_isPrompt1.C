void RFB_ethf_isPrompt1()
{
//=========Macro generated from canvas: c1/c1
//=========  (Wed Jul 20 17:48:47 2016) by ROOT version6.04/02
   TCanvas *c1 = new TCanvas("c1", "c1",1920,155,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(-9.876542,0.3937107,51.85185,1.211321);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.16);
   c1->SetRightMargin(0.03);
   c1->SetTopMargin(0.075);
   c1->SetBottomMargin(0.13);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   
   Double_t gRFB_sys_0_fx3001[3] = {
   13.3,
   28.2,
   41.1};
   Double_t gRFB_sys_0_fy3001[3] = {
   0.9523555,
   0.7356661,
   0.67884};
   Double_t gRFB_sys_0_felx3001[3] = {
   0.65,
   0.65,
   0.65};
   Double_t gRFB_sys_0_fely3001[3] = {
   0.05046532,
   0.04528025,
   0.04558411};
   Double_t gRFB_sys_0_fehx3001[3] = {
   0.65,
   0.65,
   0.65};
   Double_t gRFB_sys_0_fehy3001[3] = {
   0.05046532,
   0.04528025,
   0.04558411};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(3,gRFB_sys_0_fx3001,gRFB_sys_0_fy3001,gRFB_sys_0_felx3001,gRFB_sys_0_fehx3001,gRFB_sys_0_fely3001,gRFB_sys_0_fehy3001);
   grae->SetName("gRFB_sys_0");
   grae->SetTitle("h1D_RFB_ETHF_0");

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#cc99ff");
   grae->SetFillColor(ci);
   grae->SetLineStyle(0);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_gRFB_sys_03001 = new TH1F("Graph_gRFB_sys_03001","h1D_RFB_ETHF_0",100,0,50);
   Graph_gRFB_sys_03001->SetMinimum(0.5);
   Graph_gRFB_sys_03001->SetMaximum(1.15);
   Graph_gRFB_sys_03001->SetDirectory(0);
   Graph_gRFB_sys_03001->SetStats(0);
   Graph_gRFB_sys_03001->SetLineStyle(0);
   Graph_gRFB_sys_03001->SetMarkerStyle(20);
   Graph_gRFB_sys_03001->GetXaxis()->SetTitle("E_{T}^{HF |#eta|>4} [GeV]");
   Graph_gRFB_sys_03001->GetXaxis()->CenterTitle(true);
   Graph_gRFB_sys_03001->GetXaxis()->SetLabelFont(42);
   Graph_gRFB_sys_03001->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_03001->GetXaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_03001->GetXaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_03001->GetXaxis()->SetTitleFont(42);
   Graph_gRFB_sys_03001->GetYaxis()->SetTitle("R_{FB}");
   Graph_gRFB_sys_03001->GetYaxis()->CenterTitle(true);
   Graph_gRFB_sys_03001->GetYaxis()->SetLabelFont(42);
   Graph_gRFB_sys_03001->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_03001->GetYaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_03001->GetYaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_03001->GetYaxis()->SetTitleOffset(1.2);
   Graph_gRFB_sys_03001->GetYaxis()->SetTitleFont(42);
   Graph_gRFB_sys_03001->GetZaxis()->SetLabelFont(42);
   Graph_gRFB_sys_03001->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_03001->GetZaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_03001->GetZaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_03001->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_gRFB_sys_03001);
   
   grae->Draw("a2");
   
   Double_t gRFB_sys_1_fx3002[3] = {
   12,
   26.9,
   39.8};
   Double_t gRFB_sys_1_fy3002[3] = {
   0.9902471,
   0.8195942,
   0.7830853};
   Double_t gRFB_sys_1_felx3002[3] = {
   0.65,
   0.65,
   0.65};
   Double_t gRFB_sys_1_fely3002[3] = {
   0.05038377,
   0.04460231,
   0.04630383};
   Double_t gRFB_sys_1_fehx3002[3] = {
   0.65,
   0.65,
   0.65};
   Double_t gRFB_sys_1_fehy3002[3] = {
   0.05038377,
   0.04460231,
   0.04630383};
   grae = new TGraphAsymmErrors(3,gRFB_sys_1_fx3002,gRFB_sys_1_fy3002,gRFB_sys_1_felx3002,gRFB_sys_1_fehx3002,gRFB_sys_1_fely3002,gRFB_sys_1_fehy3002);
   grae->SetName("gRFB_sys_1");
   grae->SetTitle("h1D_RFB_ETHF_1");

   ci = TColor::GetColor("#99ffcc");
   grae->SetFillColor(ci);
   grae->SetLineStyle(0);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_gRFB_sys_13002 = new TH1F("Graph_gRFB_sys_13002","h1D_RFB_ETHF_1",100,8.44,43.36);
   Graph_gRFB_sys_13002->SetMinimum(0.7063965);
   Graph_gRFB_sys_13002->SetMaximum(1.071016);
   Graph_gRFB_sys_13002->SetDirectory(0);
   Graph_gRFB_sys_13002->SetStats(0);
   Graph_gRFB_sys_13002->SetLineStyle(0);
   Graph_gRFB_sys_13002->SetMarkerStyle(20);
   Graph_gRFB_sys_13002->GetXaxis()->SetLabelFont(42);
   Graph_gRFB_sys_13002->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_13002->GetXaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_13002->GetXaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_13002->GetXaxis()->SetTitleFont(42);
   Graph_gRFB_sys_13002->GetYaxis()->SetLabelFont(42);
   Graph_gRFB_sys_13002->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_13002->GetYaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_13002->GetYaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_13002->GetYaxis()->SetTitleOffset(1.2);
   Graph_gRFB_sys_13002->GetYaxis()->SetTitleFont(42);
   Graph_gRFB_sys_13002->GetZaxis()->SetLabelFont(42);
   Graph_gRFB_sys_13002->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_13002->GetZaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_13002->GetZaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_13002->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_gRFB_sys_13002);
   
   grae->Draw("2");
   
   Double_t gRFB_sys_2_fx3003[3] = {
   10.7,
   25.6,
   38.5};
   Double_t gRFB_sys_2_fy3003[3] = {
   0.9327401,
   0.8700354,
   0.8321425};
   Double_t gRFB_sys_2_felx3003[3] = {
   0.65,
   0.65,
   0.65};
   Double_t gRFB_sys_2_fely3003[3] = {
   0.03489381,
   0.04313635,
   0.03517466};
   Double_t gRFB_sys_2_fehx3003[3] = {
   0.65,
   0.65,
   0.65};
   Double_t gRFB_sys_2_fehy3003[3] = {
   0.03489381,
   0.04313635,
   0.03517466};
   grae = new TGraphAsymmErrors(3,gRFB_sys_2_fx3003,gRFB_sys_2_fy3003,gRFB_sys_2_felx3003,gRFB_sys_2_fehx3003,gRFB_sys_2_fely3003,gRFB_sys_2_fehy3003);
   grae->SetName("gRFB_sys_2");
   grae->SetTitle("h1D_RFB_ETHF_2");

   ci = TColor::GetColor("#ffcccc");
   grae->SetFillColor(ci);
   grae->SetLineStyle(0);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_gRFB_sys_23003 = new TH1F("Graph_gRFB_sys_23003","h1D_RFB_ETHF_2",100,7.14,42.06);
   Graph_gRFB_sys_23003->SetMinimum(0.7799012);
   Graph_gRFB_sys_23003->SetMaximum(0.9847005);
   Graph_gRFB_sys_23003->SetDirectory(0);
   Graph_gRFB_sys_23003->SetStats(0);
   Graph_gRFB_sys_23003->SetLineStyle(0);
   Graph_gRFB_sys_23003->SetMarkerStyle(20);
   Graph_gRFB_sys_23003->GetXaxis()->SetLabelFont(42);
   Graph_gRFB_sys_23003->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_23003->GetXaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_23003->GetXaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_23003->GetXaxis()->SetTitleFont(42);
   Graph_gRFB_sys_23003->GetYaxis()->SetLabelFont(42);
   Graph_gRFB_sys_23003->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_23003->GetYaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_23003->GetYaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_23003->GetYaxis()->SetTitleOffset(1.2);
   Graph_gRFB_sys_23003->GetYaxis()->SetTitleFont(42);
   Graph_gRFB_sys_23003->GetZaxis()->SetLabelFont(42);
   Graph_gRFB_sys_23003->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_23003->GetZaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_23003->GetZaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_23003->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_gRFB_sys_23003);
   
   grae->Draw("2");
   
   Double_t gRFB_sys_3_fx3004[3] = {
   9.4,
   24.3,
   37.2};
   Double_t gRFB_sys_3_fy3004[3] = {
   0.9778473,
   0.9574874,
   0.9111968};
   Double_t gRFB_sys_3_felx3004[3] = {
   0.65,
   0.65,
   0.65};
   Double_t gRFB_sys_3_fely3004[3] = {
   0.04326974,
   0.04335503,
   0.048421};
   Double_t gRFB_sys_3_fehx3004[3] = {
   0.65,
   0.65,
   0.65};
   Double_t gRFB_sys_3_fehy3004[3] = {
   0.04326974,
   0.04335503,
   0.048421};
   grae = new TGraphAsymmErrors(3,gRFB_sys_3_fx3004,gRFB_sys_3_fy3004,gRFB_sys_3_felx3004,gRFB_sys_3_fehx3004,gRFB_sys_3_fely3004,gRFB_sys_3_fehy3004);
   grae->SetName("gRFB_sys_3");
   grae->SetTitle("h1D_RFB_ETHF_3");

   ci = TColor::GetColor("#ccccff");
   grae->SetFillColor(ci);
   grae->SetLineStyle(0);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_gRFB_sys_33004 = new TH1F("Graph_gRFB_sys_33004","h1D_RFB_ETHF_3",100,5.84,40.76);
   Graph_gRFB_sys_33004->SetMinimum(0.8469416);
   Graph_gRFB_sys_33004->SetMaximum(1.036951);
   Graph_gRFB_sys_33004->SetDirectory(0);
   Graph_gRFB_sys_33004->SetStats(0);
   Graph_gRFB_sys_33004->SetLineStyle(0);
   Graph_gRFB_sys_33004->SetMarkerStyle(20);
   Graph_gRFB_sys_33004->GetXaxis()->SetLabelFont(42);
   Graph_gRFB_sys_33004->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_33004->GetXaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_33004->GetXaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_33004->GetXaxis()->SetTitleFont(42);
   Graph_gRFB_sys_33004->GetYaxis()->SetLabelFont(42);
   Graph_gRFB_sys_33004->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_33004->GetYaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_33004->GetYaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_33004->GetYaxis()->SetTitleOffset(1.2);
   Graph_gRFB_sys_33004->GetYaxis()->SetTitleFont(42);
   Graph_gRFB_sys_33004->GetZaxis()->SetLabelFont(42);
   Graph_gRFB_sys_33004->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRFB_sys_33004->GetZaxis()->SetLabelSize(0.045);
   Graph_gRFB_sys_33004->GetZaxis()->SetTitleSize(0.055);
   Graph_gRFB_sys_33004->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_gRFB_sys_33004);
   
   grae->Draw("2");
   
   Double_t gRFB_0_fx3005[3] = {
   13.3,
   28.2,
   41.1};
   Double_t gRFB_0_fy3005[3] = {
   0.9523555,
   0.7356661,
   0.67884};
   Double_t gRFB_0_felx3005[3] = {
   0,
   0,
   0};
   Double_t gRFB_0_fely3005[3] = {
   0.05144817,
   0.04293285,
   0.05081927};
   Double_t gRFB_0_fehx3005[3] = {
   0,
   0,
   0};
   Double_t gRFB_0_fehy3005[3] = {
   0.05144817,
   0.04293285,
   0.05081927};
   grae = new TGraphAsymmErrors(3,gRFB_0_fx3005,gRFB_0_fy3005,gRFB_0_felx3005,gRFB_0_fehx3005,gRFB_0_fely3005,gRFB_0_fehy3005);
   grae->SetName("gRFB_0");
   grae->SetTitle("h1D_RFB_ETHF_0");

   ci = TColor::GetColor("#660099");
   grae->SetLineColor(ci);
   grae->SetLineStyle(0);

   ci = TColor::GetColor("#660099");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(23);
   grae->SetMarkerSize(2.1);
   
   TH1F *Graph_gRFB_03005 = new TH1F("Graph_gRFB_03005","h1D_RFB_ETHF_0",100,10.52,43.88);
   Graph_gRFB_03005->SetMinimum(0.5904425);
   Graph_gRFB_03005->SetMaximum(1.041382);
   Graph_gRFB_03005->SetDirectory(0);
   Graph_gRFB_03005->SetStats(0);
   Graph_gRFB_03005->SetLineStyle(0);
   Graph_gRFB_03005->SetMarkerStyle(20);
   Graph_gRFB_03005->GetXaxis()->SetLabelFont(42);
   Graph_gRFB_03005->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRFB_03005->GetXaxis()->SetLabelSize(0.045);
   Graph_gRFB_03005->GetXaxis()->SetTitleSize(0.055);
   Graph_gRFB_03005->GetXaxis()->SetTitleFont(42);
   Graph_gRFB_03005->GetYaxis()->SetLabelFont(42);
   Graph_gRFB_03005->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRFB_03005->GetYaxis()->SetLabelSize(0.045);
   Graph_gRFB_03005->GetYaxis()->SetTitleSize(0.055);
   Graph_gRFB_03005->GetYaxis()->SetTitleOffset(1.2);
   Graph_gRFB_03005->GetYaxis()->SetTitleFont(42);
   Graph_gRFB_03005->GetZaxis()->SetLabelFont(42);
   Graph_gRFB_03005->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRFB_03005->GetZaxis()->SetLabelSize(0.045);
   Graph_gRFB_03005->GetZaxis()->SetTitleSize(0.055);
   Graph_gRFB_03005->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_gRFB_03005);
   
   grae->Draw("p");
   
   Double_t gRFB_1_fx3006[3] = {
   12,
   26.9,
   39.8};
   Double_t gRFB_1_fy3006[3] = {
   0.9902471,
   0.8195942,
   0.7830853};
   Double_t gRFB_1_felx3006[3] = {
   0,
   0,
   0};
   Double_t gRFB_1_fely3006[3] = {
   0.04000334,
   0.08406761,
   0.03562562};
   Double_t gRFB_1_fehx3006[3] = {
   0,
   0,
   0};
   Double_t gRFB_1_fehy3006[3] = {
   0.04000334,
   0.08406761,
   0.03562562};
   grae = new TGraphAsymmErrors(3,gRFB_1_fx3006,gRFB_1_fy3006,gRFB_1_felx3006,gRFB_1_fehx3006,gRFB_1_fely3006,gRFB_1_fehy3006);
   grae->SetName("gRFB_1");
   grae->SetTitle("h1D_RFB_ETHF_1");

   ci = TColor::GetColor("#006600");
   grae->SetLineColor(ci);
   grae->SetLineStyle(0);

   ci = TColor::GetColor("#006600");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(33);
   grae->SetMarkerSize(2.5);
   
   TH1F *Graph_gRFB_13006 = new TH1F("Graph_gRFB_13006","h1D_RFB_ETHF_1",100,9.22,42.58);
   Graph_gRFB_13006->SetMinimum(0.7060542);
   Graph_gRFB_13006->SetMaximum(1.059723);
   Graph_gRFB_13006->SetDirectory(0);
   Graph_gRFB_13006->SetStats(0);
   Graph_gRFB_13006->SetLineStyle(0);
   Graph_gRFB_13006->SetMarkerStyle(20);
   Graph_gRFB_13006->GetXaxis()->SetLabelFont(42);
   Graph_gRFB_13006->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRFB_13006->GetXaxis()->SetLabelSize(0.045);
   Graph_gRFB_13006->GetXaxis()->SetTitleSize(0.055);
   Graph_gRFB_13006->GetXaxis()->SetTitleFont(42);
   Graph_gRFB_13006->GetYaxis()->SetLabelFont(42);
   Graph_gRFB_13006->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRFB_13006->GetYaxis()->SetLabelSize(0.045);
   Graph_gRFB_13006->GetYaxis()->SetTitleSize(0.055);
   Graph_gRFB_13006->GetYaxis()->SetTitleOffset(1.2);
   Graph_gRFB_13006->GetYaxis()->SetTitleFont(42);
   Graph_gRFB_13006->GetZaxis()->SetLabelFont(42);
   Graph_gRFB_13006->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRFB_13006->GetZaxis()->SetLabelSize(0.045);
   Graph_gRFB_13006->GetZaxis()->SetTitleSize(0.055);
   Graph_gRFB_13006->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_gRFB_13006);
   
   grae->Draw("p");
   
   Double_t gRFB_2_fx3007[3] = {
   10.7,
   25.6,
   38.5};
   Double_t gRFB_2_fy3007[3] = {
   0.9327401,
   0.8700354,
   0.8321425};
   Double_t gRFB_2_felx3007[3] = {
   0,
   0,
   0};
   Double_t gRFB_2_fely3007[3] = {
   0.04285784,
   0.02617857,
   0.02866611};
   Double_t gRFB_2_fehx3007[3] = {
   0,
   0,
   0};
   Double_t gRFB_2_fehy3007[3] = {
   0.04285784,
   0.02617857,
   0.02866611};
   grae = new TGraphAsymmErrors(3,gRFB_2_fx3007,gRFB_2_fy3007,gRFB_2_felx3007,gRFB_2_fehx3007,gRFB_2_fely3007,gRFB_2_fehy3007);
   grae->SetName("gRFB_2");
   grae->SetTitle("h1D_RFB_ETHF_2");

   ci = TColor::GetColor("#990033");
   grae->SetLineColor(ci);
   grae->SetLineStyle(0);

   ci = TColor::GetColor("#990033");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(21);
   grae->SetMarkerSize(1.7);
   
   TH1F *Graph_gRFB_23007 = new TH1F("Graph_gRFB_23007","h1D_RFB_ETHF_2",100,7.92,41.28);
   Graph_gRFB_23007->SetMinimum(0.7862642);
   Graph_gRFB_23007->SetMaximum(0.9928101);
   Graph_gRFB_23007->SetDirectory(0);
   Graph_gRFB_23007->SetStats(0);
   Graph_gRFB_23007->SetLineStyle(0);
   Graph_gRFB_23007->SetMarkerStyle(20);
   Graph_gRFB_23007->GetXaxis()->SetLabelFont(42);
   Graph_gRFB_23007->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRFB_23007->GetXaxis()->SetLabelSize(0.045);
   Graph_gRFB_23007->GetXaxis()->SetTitleSize(0.055);
   Graph_gRFB_23007->GetXaxis()->SetTitleFont(42);
   Graph_gRFB_23007->GetYaxis()->SetLabelFont(42);
   Graph_gRFB_23007->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRFB_23007->GetYaxis()->SetLabelSize(0.045);
   Graph_gRFB_23007->GetYaxis()->SetTitleSize(0.055);
   Graph_gRFB_23007->GetYaxis()->SetTitleOffset(1.2);
   Graph_gRFB_23007->GetYaxis()->SetTitleFont(42);
   Graph_gRFB_23007->GetZaxis()->SetLabelFont(42);
   Graph_gRFB_23007->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRFB_23007->GetZaxis()->SetLabelSize(0.045);
   Graph_gRFB_23007->GetZaxis()->SetTitleSize(0.055);
   Graph_gRFB_23007->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_gRFB_23007);
   
   grae->Draw("p");
   
   Double_t gRFB_3_fx3008[3] = {
   9.4,
   24.3,
   37.2};
   Double_t gRFB_3_fy3008[3] = {
   0.9778473,
   0.9574874,
   0.9111968};
   Double_t gRFB_3_felx3008[3] = {
   6.903537e-310,
   6.903537e-310,
   6.903537e-310};
   Double_t gRFB_3_fely3008[3] = {
   0.02450444,
   0.02694229,
   0.02984342};
   Double_t gRFB_3_fehx3008[3] = {
   6.903537e-310,
   6.903537e-310,
   6.903537e-310};
   Double_t gRFB_3_fehy3008[3] = {
   0.02450444,
   0.02694229,
   0.02984342};
   grae = new TGraphAsymmErrors(3,gRFB_3_fx3008,gRFB_3_fy3008,gRFB_3_felx3008,gRFB_3_fehx3008,gRFB_3_fely3008,gRFB_3_fehy3008);
   grae->SetName("gRFB_3");
   grae->SetTitle("h1D_RFB_ETHF_3");

   ci = TColor::GetColor("#3333cc");
   grae->SetLineColor(ci);
   grae->SetLineStyle(0);

   ci = TColor::GetColor("#3333cc");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(20);
   grae->SetMarkerSize(1.7);
   
   TH1F *Graph_gRFB_33008 = new TH1F("Graph_gRFB_33008","h1D_RFB_ETHF_3",100,6.62,39.98);
   Graph_gRFB_33008->SetMinimum(0.8692535);
   Graph_gRFB_33008->SetMaximum(1.014452);
   Graph_gRFB_33008->SetDirectory(0);
   Graph_gRFB_33008->SetStats(0);
   Graph_gRFB_33008->SetLineStyle(0);
   Graph_gRFB_33008->SetMarkerStyle(20);
   Graph_gRFB_33008->GetXaxis()->SetLabelFont(42);
   Graph_gRFB_33008->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRFB_33008->GetXaxis()->SetLabelSize(0.045);
   Graph_gRFB_33008->GetXaxis()->SetTitleSize(0.055);
   Graph_gRFB_33008->GetXaxis()->SetTitleFont(42);
   Graph_gRFB_33008->GetYaxis()->SetLabelFont(42);
   Graph_gRFB_33008->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRFB_33008->GetYaxis()->SetLabelSize(0.045);
   Graph_gRFB_33008->GetYaxis()->SetTitleSize(0.055);
   Graph_gRFB_33008->GetYaxis()->SetTitleOffset(1.2);
   Graph_gRFB_33008->GetYaxis()->SetTitleFont(42);
   Graph_gRFB_33008->GetZaxis()->SetLabelFont(42);
   Graph_gRFB_33008->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRFB_33008->GetZaxis()->SetLabelSize(0.045);
   Graph_gRFB_33008->GetZaxis()->SetTitleSize(0.055);
   Graph_gRFB_33008->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_gRFB_33008);
   
   grae->Draw("p");
   TLine *line = new TLine(0,1,50,1);
   line->SetLineStyle(7);
   line->Draw();
   
   TLegend *leg = new TLegend(0.2,0.16,0.45,0.36,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.032);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   TLegendEntry *entry=leg->AddEntry("NULL","6.5 < p_{T} < 30 GeV/c","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL","0 < |y_{CM}| < 0.9","lpf");

   ci = TColor::GetColor("#ccccff");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#3333cc");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#3333cc");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1.7);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL","0.9 < |y_{CM}| < 1.5","lpf");

   ci = TColor::GetColor("#ffcccc");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#990033");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#990033");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1.7);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL","1.5 < |y_{CM}| < 1.93","lpf");

   ci = TColor::GetColor("#99ffcc");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#006600");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#006600");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(33);
   entry->SetMarkerSize(2.5);
   entry->SetTextFont(42);
   leg->Draw();
   
   leg = new TLegend(0.53,0.16,0.78,0.26,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.032);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   entry=leg->AddEntry("NULL","5 < p_{T} < 6.5 GeV/c","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL","1.5 < |y_{CM}| < 1.93","lpf");

   ci = TColor::GetColor("#cc99ff");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#660099");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#660099");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(23);
   entry->SetMarkerSize(2.1);
   entry->SetTextFont(42);
   leg->Draw();
   TLatex *   tex = new TLatex(0.88,0.84,"Prompt J/#psi");
tex->SetNDC();
   tex->SetTextAlign(32);
   tex->SetTextFont(42);
   tex->SetTextSize(0.055);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.97,0.94,"34.6 nb^{-1} (pPb 5.02 TeV)");
tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04275);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.16,0.94,"CMS");
tex->SetNDC();
   tex->SetTextFont(61);
   tex->SetTextSize(0.0534375);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.2815,0.94,"Preliminary");
tex->SetNDC();
   tex->SetTextFont(52);
   tex->SetTextSize(0.04467375);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
