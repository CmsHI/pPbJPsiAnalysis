void cms_jps_ppb_pt_y045_fb()
{
//=========Macro generated from canvas: w6/Two Graphs
//=========  (Tue Jul 21 12:15:03 2015) by ROOT version6.02/08
   TCanvas *w6 = new TCanvas("w6", "Two Graphs",0,0,600,500);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   w6->Range(-2.944785,-0.197647,21.59509,1.449412);
   w6->SetFillColor(0);
   w6->SetBorderMode(0);
   w6->SetBorderSize(0);
   w6->SetTickx(1);
   w6->SetTicky(1);
   w6->SetLeftMargin(0.12);
   w6->SetRightMargin(0.065);
   w6->SetTopMargin(0.03);
   w6->SetBottomMargin(0.12);
   w6->SetFrameBorderMode(0);
   w6->SetFrameBorderMode(0);
   
   TH2F *hr13 = new TH2F("hr13","",10,0,20,20,0,1.4);
   hr13->SetStats(0);
   hr13->SetMarkerStyle(20);
   hr13->SetMarkerSize(0.8);
   hr13->GetXaxis()->SetTitle("p_{T} (GeV)");
   hr13->GetXaxis()->SetLabelFont(42);
   hr13->GetXaxis()->SetTitleSize(0.048);
   hr13->GetXaxis()->SetTitleOffset(1.15);
   hr13->GetXaxis()->SetTitleFont(42);
   hr13->GetYaxis()->SetTitle("R_{F/B}(p_{T})");
   hr13->GetYaxis()->SetLabelFont(42);
   hr13->GetYaxis()->SetTitleSize(0.048);
   hr13->GetYaxis()->SetTitleOffset(1.2);
   hr13->GetYaxis()->SetTitleFont(42);
   hr13->GetZaxis()->SetLabelFont(42);
   hr13->GetZaxis()->SetTitleSize(0.048);
   hr13->GetZaxis()->SetTitleFont(42);
   hr13->Draw("");
   TLine *line = new TLine(0,1,20,1);
   line->SetLineStyle(2);
   line->SetLineWidth(2);
   line->Draw();
   
   Double_t Graph0_fx10[202] = {
   0,
   0.2,
   0.4,
   0.6,
   0.8,
   1,
   1.2,
   1.4,
   1.6,
   1.8,
   2,
   2.2,
   2.4,
   2.6,
   2.8,
   3,
   3.2,
   3.4,
   3.6,
   3.8,
   4,
   4.2,
   4.4,
   4.6,
   4.8,
   5,
   5.2,
   5.4,
   5.6,
   5.8,
   6,
   6.2,
   6.4,
   6.6,
   6.8,
   7,
   7.2,
   7.4,
   7.6,
   7.8,
   8,
   8.2,
   8.4,
   8.6,
   8.8,
   9,
   9.2,
   9.4,
   9.6,
   9.8,
   10,
   10.2,
   10.4,
   10.6,
   10.8,
   11,
   11.2,
   11.4,
   11.6,
   11.8,
   12,
   12.2,
   12.4,
   12.6,
   12.8,
   13,
   13.2,
   13.4,
   13.6,
   13.8,
   14,
   14.2,
   14.4,
   14.6,
   14.8,
   15,
   15.2,
   15.4,
   15.6,
   15.8,
   16,
   16.2,
   16.4,
   16.6,
   16.8,
   17,
   17.2,
   17.4,
   17.6,
   17.8,
   18,
   18.2,
   18.4,
   18.6,
   18.8,
   19,
   19.2,
   19.4,
   19.6,
   19.8,
   20,
   20,
   19.8,
   19.6,
   19.4,
   19.2,
   19,
   18.8,
   18.6,
   18.4,
   18.2,
   18,
   17.8,
   17.6,
   17.4,
   17.2,
   17,
   16.8,
   16.6,
   16.4,
   16.2,
   16,
   15.8,
   15.6,
   15.4,
   15.2,
   15,
   14.8,
   14.6,
   14.4,
   14.2,
   14,
   13.8,
   13.6,
   13.4,
   13.2,
   13,
   12.8,
   12.6,
   12.4,
   12.2,
   12,
   11.8,
   11.6,
   11.4,
   11.2,
   11,
   10.8,
   10.6,
   10.4,
   10.2,
   10,
   9.8,
   9.6,
   9.4,
   9.2,
   9,
   8.8,
   8.6,
   8.4,
   8.2,
   8,
   7.8,
   7.6,
   7.4,
   7.2,
   7,
   6.8,
   6.6,
   6.4,
   6.2,
   6,
   5.8,
   5.6,
   5.4,
   5.2,
   5,
   4.8,
   4.6,
   4.4,
   4.2,
   4,
   3.8,
   3.6,
   3.4,
   3.2,
   3,
   2.8,
   2.6,
   2.4,
   2.2,
   2,
   1.8,
   1.6,
   1.4,
   1.2,
   1,
   0.8,
   0.6,
   0.4,
   0.2,
   0};
   Double_t Graph0_fy10[202] = {
   0.9142149,
   0.914938,
   0.9165757,
   0.9196849,
   0.9235736,
   0.9284955,
   0.9339967,
   0.940225,
   0.94727,
   0.9548998,
   0.9634742,
   0.972404,
   0.9810699,
   0.9890505,
   0.9964119,
   1.003013,
   1.008831,
   1.013978,
   1.018193,
   1.021545,
   1.024606,
   1.027106,
   1.028722,
   1.03047,
   1.031391,
   1.032231,
   1.032655,
   1.032701,
   1.032624,
   1.032162,
   1.031762,
   1.031092,
   1.030433,
   1.029727,
   1.028904,
   1.028051,
   1.027119,
   1.026296,
   1.025341,
   1.024379,
   1.023477,
   1.02269,
   1.021657,
   1.020787,
   1.020023,
   1.019152,
   1.018089,
   1.017189,
   1.016547,
   1.015728,
   1.015078,
   1.014411,
   1.013939,
   1.013458,
   1.012777,
   1.012134,
   1.011665,
   1.011097,
   1.010582,
   1.012395,
   1.012055,
   1.011714,
   1.01143,
   1.008282,
   1.010862,
   1.010471,
   1.010097,
   1.009624,
   1.009419,
   1.005577,
   1.0052,
   1.004901,
   1.004537,
   1.004236,
   1.004136,
   1.003774,
   1.003509,
   1.003404,
   1.003535,
   1.003337,
   1.003165,
   1.002732,
   1.002631,
   1.002288,
   1.002188,
   1.0019,
   1.001806,
   1.001564,
   1.001237,
   1.001125,
   1.000999,
   1.00113,
   1.000911,
   1.000903,
   1.000691,
   1.000567,
   1.000511,
   1.000371,
   1.000257,
   1.00026,
   1.000128,
   0.9968568,
   0.996829,
   0.9969605,
   0.9970623,
   0.997163,
   0.9972535,
   0.9972366,
   0.9974336,
   0.9975556,
   0.9976563,
   0.9973478,
   0.9975393,
   0.9975927,
   0.9977869,
   0.9978875,
   0.9979946,
   0.9982032,
   0.9982762,
   0.9984979,
   0.9986893,
   0.9991346,
   0.9992588,
   0.9995749,
   0.9985201,
   0.9987907,
   0.999087,
   0.9994192,
   0.9997043,
   1.000061,
   0.9973911,
   0.9977822,
   0.9979629,
   0.9982586,
   0.9985073,
   0.998785,
   0.9999785,
   1.000364,
   1.003179,
   1.004479,
   1.004764,
   1.007465,
   1.007827,
   1.005068,
   1.005502,
   1.005637,
   1.006045,
   1.006492,
   1.007022,
   1.007422,
   1.00801,
   1.00833,
   1.008843,
   1.00944,
   1.009577,
   1.010125,
   1.010555,
   1.011172,
   1.011792,
   1.012398,
   1.013078,
   1.013823,
   1.014324,
   1.015016,
   1.015416,
   1.015901,
   1.0165,
   1.016885,
   1.017173,
   1.017427,
   1.017694,
   1.017807,
   1.017748,
   1.017764,
   1.017353,
   1.01687,
   1.016093,
   1.015067,
   1.013836,
   1.011802,
   1.00975,
   1.007434,
   1.004548,
   1.001689,
   0.9979299,
   0.9936021,
   0.9886292,
   0.9831261,
   0.9768533,
   0.9697797,
   0.961871,
   0.9531718,
   0.9439886,
   0.9347142,
   0.9256612,
   0.9167025,
   0.9086522,
   0.9015067,
   0.8951483,
   0.8908379,
   0.8882346,
   0.8872674};
   TGraph *graph = new TGraph(202,Graph0_fx10,Graph0_fy10);
   graph->SetName("Graph0");
   graph->SetTitle("Graph");
   graph->SetFillColor(2);
   graph->SetFillStyle(3004);
   graph->SetLineColor(2);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(0.8);
   
   TH1F *Graph_Graph10 = new TH1F("Graph_Graph10","Graph",202,0,22);
   Graph_Graph10->SetMinimum(0.872724);
   Graph_Graph10->SetMaximum(1.047245);
   Graph_Graph10->SetDirectory(0);
   Graph_Graph10->SetStats(0);
   Graph_Graph10->SetMarkerStyle(20);
   Graph_Graph10->SetMarkerSize(0.8);
   Graph_Graph10->GetXaxis()->SetLabelFont(42);
   Graph_Graph10->GetXaxis()->SetTitleSize(0.048);
   Graph_Graph10->GetXaxis()->SetTitleOffset(1.15);
   Graph_Graph10->GetXaxis()->SetTitleFont(42);
   Graph_Graph10->GetYaxis()->SetLabelFont(42);
   Graph_Graph10->GetYaxis()->SetTitleSize(0.048);
   Graph_Graph10->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph10->GetYaxis()->SetTitleFont(42);
   Graph_Graph10->GetZaxis()->SetLabelFont(42);
   Graph_Graph10->GetZaxis()->SetTitleSize(0.048);
   Graph_Graph10->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph10);
   
   graph->Draw("f");
   
   Double_t Graph1_fx11[101] = {
   0,
   0.2,
   0.4,
   0.6,
   0.8,
   1,
   1.2,
   1.4,
   1.6,
   1.8,
   2,
   2.2,
   2.4,
   2.6,
   2.8,
   3,
   3.2,
   3.4,
   3.6,
   3.8,
   4,
   4.2,
   4.4,
   4.6,
   4.8,
   5,
   5.2,
   5.4,
   5.6,
   5.8,
   6,
   6.2,
   6.4,
   6.6,
   6.8,
   7,
   7.2,
   7.4,
   7.6,
   7.8,
   8,
   8.2,
   8.4,
   8.6,
   8.8,
   9,
   9.2,
   9.4,
   9.6,
   9.8,
   10,
   10.2,
   10.4,
   10.6,
   10.8,
   11,
   11.2,
   11.4,
   11.6,
   11.8,
   12,
   12.2,
   12.4,
   12.6,
   12.8,
   13,
   13.2,
   13.4,
   13.6,
   13.8,
   14,
   14.2,
   14.4,
   14.6,
   14.8,
   15,
   15.2,
   15.4,
   15.6,
   15.8,
   16,
   16.2,
   16.4,
   16.6,
   16.8,
   17,
   17.2,
   17.4,
   17.6,
   17.8,
   18,
   18.2,
   18.4,
   18.6,
   18.8,
   19,
   19.2,
   19.4,
   19.6,
   19.8,
   20};
   Double_t Graph1_fy11[101] = {
   0.9142149,
   0.914938,
   0.9165757,
   0.9196849,
   0.9235736,
   0.9284955,
   0.9339967,
   0.940225,
   0.94727,
   0.9548998,
   0.9634742,
   0.972404,
   0.9810699,
   0.9890505,
   0.9964119,
   1.003013,
   1.008831,
   1.013978,
   1.018193,
   1.021545,
   1.024606,
   1.027106,
   1.028722,
   1.03047,
   1.031391,
   1.032231,
   1.032655,
   1.032701,
   1.032624,
   1.032162,
   1.031762,
   1.031092,
   1.030433,
   1.029727,
   1.028904,
   1.028051,
   1.027119,
   1.026296,
   1.025341,
   1.024379,
   1.023477,
   1.02269,
   1.021657,
   1.020787,
   1.020023,
   1.019152,
   1.018089,
   1.017189,
   1.016547,
   1.015728,
   1.015078,
   1.014411,
   1.013939,
   1.013458,
   1.012777,
   1.012134,
   1.011665,
   1.011097,
   1.010582,
   1.012395,
   1.012055,
   1.011714,
   1.01143,
   1.008282,
   1.010862,
   1.010471,
   1.010097,
   1.009624,
   1.009419,
   1.005577,
   1.0052,
   1.004901,
   1.004537,
   1.004236,
   1.004136,
   1.003774,
   1.003509,
   1.003404,
   1.003535,
   1.003337,
   1.003165,
   1.002732,
   1.002631,
   1.002288,
   1.002188,
   1.0019,
   1.001806,
   1.001564,
   1.001237,
   1.001125,
   1.000999,
   1.00113,
   1.000911,
   1.000903,
   1.000691,
   1.000567,
   1.000511,
   1.000371,
   1.000257,
   1.00026,
   1.000128};
   graph = new TGraph(101,Graph1_fx11,Graph1_fy11);
   graph->SetName("Graph1");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);
   graph->SetLineColor(2);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(0.8);
   
   TH1F *Graph_Graph11 = new TH1F("Graph_Graph11","Graph",101,0,22);
   Graph_Graph11->SetMinimum(0.9023662);
   Graph_Graph11->SetMaximum(1.04455);
   Graph_Graph11->SetDirectory(0);
   Graph_Graph11->SetStats(0);
   Graph_Graph11->SetMarkerStyle(20);
   Graph_Graph11->SetMarkerSize(0.8);
   Graph_Graph11->GetXaxis()->SetLabelFont(42);
   Graph_Graph11->GetXaxis()->SetTitleSize(0.048);
   Graph_Graph11->GetXaxis()->SetTitleOffset(1.15);
   Graph_Graph11->GetXaxis()->SetTitleFont(42);
   Graph_Graph11->GetYaxis()->SetLabelFont(42);
   Graph_Graph11->GetYaxis()->SetTitleSize(0.048);
   Graph_Graph11->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph11->GetYaxis()->SetTitleFont(42);
   Graph_Graph11->GetZaxis()->SetLabelFont(42);
   Graph_Graph11->GetZaxis()->SetTitleSize(0.048);
   Graph_Graph11->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph11);
   
   graph->Draw("c");
   
   Double_t Graph2_fx12[101] = {
   0,
   0.2,
   0.4,
   0.6,
   0.8,
   1,
   1.2,
   1.4,
   1.6,
   1.8,
   2,
   2.2,
   2.4,
   2.6,
   2.8,
   3,
   3.2,
   3.4,
   3.6,
   3.8,
   4,
   4.2,
   4.4,
   4.6,
   4.8,
   5,
   5.2,
   5.4,
   5.6,
   5.8,
   6,
   6.2,
   6.4,
   6.6,
   6.8,
   7,
   7.2,
   7.4,
   7.6,
   7.8,
   8,
   8.2,
   8.4,
   8.6,
   8.8,
   9,
   9.2,
   9.4,
   9.6,
   9.8,
   10,
   10.2,
   10.4,
   10.6,
   10.8,
   11,
   11.2,
   11.4,
   11.6,
   11.8,
   12,
   12.2,
   12.4,
   12.6,
   12.8,
   13,
   13.2,
   13.4,
   13.6,
   13.8,
   14,
   14.2,
   14.4,
   14.6,
   14.8,
   15,
   15.2,
   15.4,
   15.6,
   15.8,
   16,
   16.2,
   16.4,
   16.6,
   16.8,
   17,
   17.2,
   17.4,
   17.6,
   17.8,
   18,
   18.2,
   18.4,
   18.6,
   18.8,
   19,
   19.2,
   19.4,
   19.6,
   19.8,
   20};
   Double_t Graph2_fy12[101] = {
   0.8872674,
   0.8882346,
   0.8908379,
   0.8951483,
   0.9015067,
   0.9086522,
   0.9167025,
   0.9256612,
   0.9347142,
   0.9439886,
   0.9531718,
   0.961871,
   0.9697797,
   0.9768533,
   0.9831261,
   0.9886292,
   0.9936021,
   0.9979299,
   1.001689,
   1.004548,
   1.007434,
   1.00975,
   1.011802,
   1.013836,
   1.015067,
   1.016093,
   1.01687,
   1.017353,
   1.017764,
   1.017748,
   1.017807,
   1.017694,
   1.017427,
   1.017173,
   1.016885,
   1.0165,
   1.015901,
   1.015416,
   1.015016,
   1.014324,
   1.013823,
   1.013078,
   1.012398,
   1.011792,
   1.011172,
   1.010555,
   1.010125,
   1.009577,
   1.00944,
   1.008843,
   1.00833,
   1.00801,
   1.007422,
   1.007022,
   1.006492,
   1.006045,
   1.005637,
   1.005502,
   1.005068,
   1.007827,
   1.007465,
   1.004764,
   1.004479,
   1.003179,
   1.000364,
   0.9999785,
   0.998785,
   0.9985073,
   0.9982586,
   0.9979629,
   0.9977822,
   0.9973911,
   1.000061,
   0.9997043,
   0.9994192,
   0.999087,
   0.9987907,
   0.9985201,
   0.9995749,
   0.9992588,
   0.9991346,
   0.9986893,
   0.9984979,
   0.9982762,
   0.9982032,
   0.9979946,
   0.9978875,
   0.9977869,
   0.9975927,
   0.9975393,
   0.9973478,
   0.9976563,
   0.9975556,
   0.9974336,
   0.9972366,
   0.9972535,
   0.997163,
   0.9970623,
   0.9969605,
   0.996829,
   0.9968568};
   graph = new TGraph(101,Graph2_fx12,Graph2_fy12);
   graph->SetName("Graph2");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);
   graph->SetLineColor(2);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(0.8);
   
   TH1F *Graph_Graph12 = new TH1F("Graph_Graph12","Graph",101,0,22);
   Graph_Graph12->SetMinimum(0.8742134);
   Graph_Graph12->SetMaximum(1.030861);
   Graph_Graph12->SetDirectory(0);
   Graph_Graph12->SetStats(0);
   Graph_Graph12->SetMarkerStyle(20);
   Graph_Graph12->SetMarkerSize(0.8);
   Graph_Graph12->GetXaxis()->SetLabelFont(42);
   Graph_Graph12->GetXaxis()->SetTitleSize(0.048);
   Graph_Graph12->GetXaxis()->SetTitleOffset(1.15);
   Graph_Graph12->GetXaxis()->SetTitleFont(42);
   Graph_Graph12->GetYaxis()->SetLabelFont(42);
   Graph_Graph12->GetYaxis()->SetTitleSize(0.048);
   Graph_Graph12->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph12->GetYaxis()->SetTitleFont(42);
   Graph_Graph12->GetZaxis()->SetLabelFont(42);
   Graph_Graph12->GetZaxis()->SetTitleSize(0.048);
   Graph_Graph12->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph12);
   
   graph->Draw("c");
   
   TLegend *leg = new TLegend(0.15,0.2,0.5,0.35,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.045);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(10);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("Graph0","E. loss  ","f");
   entry->SetFillColor(2);
   entry->SetFillStyle(3004);
   entry->SetLineColor(2);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph","E. loss + EPS09","f");
   entry->SetFillColor(4);
   entry->SetFillStyle(3004);
   entry->SetLineColor(4);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   
   Double_t Graph3_fx13[202] = {
   0,
   0.2,
   0.4,
   0.6,
   0.8,
   1,
   1.2,
   1.4,
   1.6,
   1.8,
   2,
   2.2,
   2.4,
   2.6,
   2.8,
   3,
   3.2,
   3.4,
   3.6,
   3.8,
   4,
   4.2,
   4.4,
   4.6,
   4.8,
   5,
   5.2,
   5.4,
   5.6,
   5.8,
   6,
   6.2,
   6.4,
   6.6,
   6.8,
   7,
   7.2,
   7.4,
   7.6,
   7.8,
   8,
   8.2,
   8.4,
   8.6,
   8.8,
   9,
   9.2,
   9.4,
   9.6,
   9.8,
   10,
   10.2,
   10.4,
   10.6,
   10.8,
   11,
   11.2,
   11.4,
   11.6,
   11.8,
   12,
   12.2,
   12.4,
   12.6,
   12.8,
   13,
   13.2,
   13.4,
   13.6,
   13.8,
   14,
   14.2,
   14.4,
   14.6,
   14.8,
   15,
   15.2,
   15.4,
   15.6,
   15.8,
   16,
   16.2,
   16.4,
   16.6,
   16.8,
   17,
   17.2,
   17.4,
   17.6,
   17.8,
   18,
   18.2,
   18.4,
   18.6,
   18.8,
   19,
   19.2,
   19.4,
   19.6,
   19.8,
   20,
   20,
   19.8,
   19.6,
   19.4,
   19.2,
   19,
   18.8,
   18.6,
   18.4,
   18.2,
   18,
   17.8,
   17.6,
   17.4,
   17.2,
   17,
   16.8,
   16.6,
   16.4,
   16.2,
   16,
   15.8,
   15.6,
   15.4,
   15.2,
   15,
   14.8,
   14.6,
   14.4,
   14.2,
   14,
   13.8,
   13.6,
   13.4,
   13.2,
   13,
   12.8,
   12.6,
   12.4,
   12.2,
   12,
   11.8,
   11.6,
   11.4,
   11.2,
   11,
   10.8,
   10.6,
   10.4,
   10.2,
   10,
   9.8,
   9.6,
   9.4,
   9.2,
   9,
   8.8,
   8.6,
   8.4,
   8.2,
   8,
   7.8,
   7.6,
   7.4,
   7.2,
   7,
   6.8,
   6.6,
   6.4,
   6.2,
   6,
   5.8,
   5.6,
   5.4,
   5.2,
   5,
   4.8,
   4.6,
   4.4,
   4.2,
   4,
   3.8,
   3.6,
   3.4,
   3.2,
   3,
   2.8,
   2.6,
   2.4,
   2.2,
   2,
   1.8,
   1.6,
   1.4,
   1.2,
   1,
   0.8,
   0.6,
   0.4,
   0.2,
   0};
   Double_t Graph3_fy13[202] = {
   0.9395375,
   0.9401008,
   0.9408615,
   0.9426838,
   0.9449524,
   0.9485143,
   0.9527629,
   0.9571404,
   0.9613671,
   0.964928,
   0.9687421,
   0.972595,
   0.9765731,
   0.9791371,
   0.9822544,
   0.9868669,
   0.9907584,
   0.9943217,
   0.9981685,
   1.000999,
   1.003818,
   1.006505,
   1.008745,
   1.010533,
   1.012885,
   1.014066,
   1.014584,
   1.013862,
   1.01312,
   1.012196,
   1.011739,
   1.010938,
   1.010607,
   1.010366,
   1.009725,
   1.009239,
   1.008876,
   1.008778,
   1.007919,
   1.007057,
   1.005799,
   1.00504,
   1.003462,
   1.00249,
   1.001722,
   1.001315,
   1.000338,
   0.9995556,
   0.9986961,
   1.000266,
   0.9998424,
   0.995643,
   0.9967855,
   0.9951688,
   0.9945968,
   1.003788,
   1.000633,
   0.9985853,
   1.003929,
   0.9984409,
   0.9962562,
   0.9963331,
   0.9890661,
   0.9885726,
   0.9900058,
   0.9911102,
   0.9904868,
   0.9925824,
   0.9917574,
   0.9915157,
   0.9911146,
   0.9911137,
   0.9905627,
   0.9900166,
   0.9892899,
   0.988735,
   0.9879328,
   0.9876413,
   0.9871628,
   0.9867423,
   0.9858265,
   0.9859629,
   0.9856518,
   0.9856967,
   0.9852421,
   0.9851729,
   0.985202,
   0.9852762,
   0.9849671,
   0.9849116,
   0.9847008,
   0.9848141,
   0.9870728,
   0.9872658,
   0.9875085,
   0.9878796,
   0.9844215,
   0.984521,
   0.9858946,
   0.9834002,
   0.9844456,
   0.9416121,
   0.9431997,
   0.9454544,
   0.9477599,
   0.9483424,
   0.9521486,
   0.9529598,
   0.9536659,
   0.9541102,
   0.9540974,
   0.9538962,
   0.9544461,
   0.9541721,
   0.9545205,
   0.9540851,
   0.9545238,
   0.9545572,
   0.9552398,
   0.9555685,
   0.9561121,
   0.9562686,
   0.9572985,
   0.9576517,
   0.9581693,
   0.9586411,
   0.9590432,
   0.9599262,
   0.9603001,
   0.9609203,
   0.961309,
   0.9615943,
   0.9620908,
   0.9627079,
   0.9634425,
   0.9639169,
   0.9620052,
   0.9626094,
   0.9628158,
   0.9614292,
   0.9685299,
   0.9690488,
   0.9689281,
   0.969962,
   0.9634583,
   0.9637112,
   0.9644593,
   0.9601865,
   0.96113,
   0.9655172,
   0.9686714,
   0.9750948,
   0.9771247,
   0.9769483,
   0.9768696,
   0.9758607,
   0.9759898,
   0.9770212,
   0.9778525,
   0.97905,
   0.9807906,
   0.9815755,
   0.9824626,
   0.9833433,
   0.9837357,
   0.9832559,
   0.9838086,
   0.9846764,
   0.9854985,
   0.9865418,
   0.987425,
   0.9883639,
   0.9893606,
   0.9907566,
   0.9917847,
   0.9923961,
   0.9923035,
   0.9919356,
   0.9903353,
   0.9892633,
   0.9880787,
   0.9877851,
   0.9871768,
   0.9864267,
   0.9838076,
   0.981341,
   0.9786207,
   0.9765399,
   0.9722515,
   0.9663591,
   0.9592334,
   0.951768,
   0.9440707,
   0.9363644,
   0.9282552,
   0.9206683,
   0.9135723,
   0.9073108,
   0.9028508,
   0.8993846,
   0.8976107,
   0.8968704};
   graph = new TGraph(202,Graph3_fx13,Graph3_fy13);
   graph->SetName("Graph3");
   graph->SetTitle("Graph");
   graph->SetFillColor(4);
   graph->SetFillStyle(3004);
   graph->SetLineColor(4);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(0.8);
   
   TH1F *Graph_Graph13 = new TH1F("Graph_Graph13","Graph",202,0,22);
   Graph_Graph13->SetMinimum(0.885099);
   Graph_Graph13->SetMaximum(1.026355);
   Graph_Graph13->SetDirectory(0);
   Graph_Graph13->SetStats(0);
   Graph_Graph13->SetMarkerStyle(20);
   Graph_Graph13->SetMarkerSize(0.8);
   Graph_Graph13->GetXaxis()->SetLabelFont(42);
   Graph_Graph13->GetXaxis()->SetTitleSize(0.048);
   Graph_Graph13->GetXaxis()->SetTitleOffset(1.15);
   Graph_Graph13->GetXaxis()->SetTitleFont(42);
   Graph_Graph13->GetYaxis()->SetLabelFont(42);
   Graph_Graph13->GetYaxis()->SetTitleSize(0.048);
   Graph_Graph13->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph13->GetYaxis()->SetTitleFont(42);
   Graph_Graph13->GetZaxis()->SetLabelFont(42);
   Graph_Graph13->GetZaxis()->SetTitleSize(0.048);
   Graph_Graph13->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph13);
   
   graph->Draw("f");
   
   Double_t Graph4_fx14[101] = {
   0,
   0.2,
   0.4,
   0.6,
   0.8,
   1,
   1.2,
   1.4,
   1.6,
   1.8,
   2,
   2.2,
   2.4,
   2.6,
   2.8,
   3,
   3.2,
   3.4,
   3.6,
   3.8,
   4,
   4.2,
   4.4,
   4.6,
   4.8,
   5,
   5.2,
   5.4,
   5.6,
   5.8,
   6,
   6.2,
   6.4,
   6.6,
   6.8,
   7,
   7.2,
   7.4,
   7.6,
   7.8,
   8,
   8.2,
   8.4,
   8.6,
   8.8,
   9,
   9.2,
   9.4,
   9.6,
   9.8,
   10,
   10.2,
   10.4,
   10.6,
   10.8,
   11,
   11.2,
   11.4,
   11.6,
   11.8,
   12,
   12.2,
   12.4,
   12.6,
   12.8,
   13,
   13.2,
   13.4,
   13.6,
   13.8,
   14,
   14.2,
   14.4,
   14.6,
   14.8,
   15,
   15.2,
   15.4,
   15.6,
   15.8,
   16,
   16.2,
   16.4,
   16.6,
   16.8,
   17,
   17.2,
   17.4,
   17.6,
   17.8,
   18,
   18.2,
   18.4,
   18.6,
   18.8,
   19,
   19.2,
   19.4,
   19.6,
   19.8,
   20};
   Double_t Graph4_fy14[101] = {
   0.9395375,
   0.9401008,
   0.9408615,
   0.9426838,
   0.9449524,
   0.9485143,
   0.9527629,
   0.9571404,
   0.9613671,
   0.964928,
   0.9687421,
   0.972595,
   0.9765731,
   0.9791371,
   0.9822544,
   0.9868669,
   0.9907584,
   0.9943217,
   0.9981685,
   1.000999,
   1.003818,
   1.006505,
   1.008745,
   1.010533,
   1.012885,
   1.014066,
   1.014584,
   1.013862,
   1.01312,
   1.012196,
   1.011739,
   1.010938,
   1.010607,
   1.010366,
   1.009725,
   1.009239,
   1.008876,
   1.008778,
   1.007919,
   1.007057,
   1.005799,
   1.00504,
   1.003462,
   1.00249,
   1.001722,
   1.001315,
   1.000338,
   0.9995556,
   0.9986961,
   1.000266,
   0.9998424,
   0.995643,
   0.9967855,
   0.9951688,
   0.9945968,
   1.003788,
   1.000633,
   0.9985853,
   1.003929,
   0.9984409,
   0.9962562,
   0.9963331,
   0.9890661,
   0.9885726,
   0.9900058,
   0.9911102,
   0.9904868,
   0.9925824,
   0.9917574,
   0.9915157,
   0.9911146,
   0.9911137,
   0.9905627,
   0.9900166,
   0.9892899,
   0.988735,
   0.9879328,
   0.9876413,
   0.9871628,
   0.9867423,
   0.9858265,
   0.9859629,
   0.9856518,
   0.9856967,
   0.9852421,
   0.9851729,
   0.985202,
   0.9852762,
   0.9849671,
   0.9849116,
   0.9847008,
   0.9848141,
   0.9870728,
   0.9872658,
   0.9875085,
   0.9878796,
   0.9844215,
   0.984521,
   0.9858946,
   0.9834002,
   0.9844456};
   graph = new TGraph(101,Graph4_fx14,Graph4_fy14);
   graph->SetName("Graph4");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);
   graph->SetLineColor(4);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(0.8);
   
   TH1F *Graph_Graph14 = new TH1F("Graph_Graph14","Graph",101,0,22);
   Graph_Graph14->SetMinimum(0.9320328);
   Graph_Graph14->SetMaximum(1.022088);
   Graph_Graph14->SetDirectory(0);
   Graph_Graph14->SetStats(0);
   Graph_Graph14->SetMarkerStyle(20);
   Graph_Graph14->SetMarkerSize(0.8);
   Graph_Graph14->GetXaxis()->SetLabelFont(42);
   Graph_Graph14->GetXaxis()->SetTitleSize(0.048);
   Graph_Graph14->GetXaxis()->SetTitleOffset(1.15);
   Graph_Graph14->GetXaxis()->SetTitleFont(42);
   Graph_Graph14->GetYaxis()->SetLabelFont(42);
   Graph_Graph14->GetYaxis()->SetTitleSize(0.048);
   Graph_Graph14->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph14->GetYaxis()->SetTitleFont(42);
   Graph_Graph14->GetZaxis()->SetLabelFont(42);
   Graph_Graph14->GetZaxis()->SetTitleSize(0.048);
   Graph_Graph14->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph14);
   
   graph->Draw("c");
   
   Double_t Graph5_fx15[101] = {
   0,
   0.2,
   0.4,
   0.6,
   0.8,
   1,
   1.2,
   1.4,
   1.6,
   1.8,
   2,
   2.2,
   2.4,
   2.6,
   2.8,
   3,
   3.2,
   3.4,
   3.6,
   3.8,
   4,
   4.2,
   4.4,
   4.6,
   4.8,
   5,
   5.2,
   5.4,
   5.6,
   5.8,
   6,
   6.2,
   6.4,
   6.6,
   6.8,
   7,
   7.2,
   7.4,
   7.6,
   7.8,
   8,
   8.2,
   8.4,
   8.6,
   8.8,
   9,
   9.2,
   9.4,
   9.6,
   9.8,
   10,
   10.2,
   10.4,
   10.6,
   10.8,
   11,
   11.2,
   11.4,
   11.6,
   11.8,
   12,
   12.2,
   12.4,
   12.6,
   12.8,
   13,
   13.2,
   13.4,
   13.6,
   13.8,
   14,
   14.2,
   14.4,
   14.6,
   14.8,
   15,
   15.2,
   15.4,
   15.6,
   15.8,
   16,
   16.2,
   16.4,
   16.6,
   16.8,
   17,
   17.2,
   17.4,
   17.6,
   17.8,
   18,
   18.2,
   18.4,
   18.6,
   18.8,
   19,
   19.2,
   19.4,
   19.6,
   19.8,
   20};
   Double_t Graph5_fy15[101] = {
   0.8968704,
   0.8976107,
   0.8993846,
   0.9028508,
   0.9073108,
   0.9135723,
   0.9206683,
   0.9282552,
   0.9363644,
   0.9440707,
   0.951768,
   0.9592334,
   0.9663591,
   0.9722515,
   0.9765399,
   0.9786207,
   0.981341,
   0.9838076,
   0.9864267,
   0.9871768,
   0.9877851,
   0.9880787,
   0.9892633,
   0.9903353,
   0.9919356,
   0.9923035,
   0.9923961,
   0.9917847,
   0.9907566,
   0.9893606,
   0.9883639,
   0.987425,
   0.9865418,
   0.9854985,
   0.9846764,
   0.9838086,
   0.9832559,
   0.9837357,
   0.9833433,
   0.9824626,
   0.9815755,
   0.9807906,
   0.97905,
   0.9778525,
   0.9770212,
   0.9759898,
   0.9758607,
   0.9768696,
   0.9769483,
   0.9771247,
   0.9750948,
   0.9686714,
   0.9655172,
   0.96113,
   0.9601865,
   0.9644593,
   0.9637112,
   0.9634583,
   0.969962,
   0.9689281,
   0.9690488,
   0.9685299,
   0.9614292,
   0.9628158,
   0.9626094,
   0.9620052,
   0.9639169,
   0.9634425,
   0.9627079,
   0.9620908,
   0.9615943,
   0.961309,
   0.9609203,
   0.9603001,
   0.9599262,
   0.9590432,
   0.9586411,
   0.9581693,
   0.9576517,
   0.9572985,
   0.9562686,
   0.9561121,
   0.9555685,
   0.9552398,
   0.9545572,
   0.9545238,
   0.9540851,
   0.9545205,
   0.9541721,
   0.9544461,
   0.9538962,
   0.9540974,
   0.9541102,
   0.9536659,
   0.9529598,
   0.9521486,
   0.9483424,
   0.9477599,
   0.9454544,
   0.9431997,
   0.9416121};
   graph = new TGraph(101,Graph5_fx15,Graph5_fy15);
   graph->SetName("Graph5");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);
   graph->SetLineColor(4);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(0.8);
   
   TH1F *Graph_Graph15 = new TH1F("Graph_Graph15","Graph",101,0,22);
   Graph_Graph15->SetMinimum(0.8873178);
   Graph_Graph15->SetMaximum(1.001949);
   Graph_Graph15->SetDirectory(0);
   Graph_Graph15->SetStats(0);
   Graph_Graph15->SetMarkerStyle(20);
   Graph_Graph15->SetMarkerSize(0.8);
   Graph_Graph15->GetXaxis()->SetLabelFont(42);
   Graph_Graph15->GetXaxis()->SetTitleSize(0.048);
   Graph_Graph15->GetXaxis()->SetTitleOffset(1.15);
   Graph_Graph15->GetXaxis()->SetTitleFont(42);
   Graph_Graph15->GetYaxis()->SetLabelFont(42);
   Graph_Graph15->GetYaxis()->SetTitleSize(0.048);
   Graph_Graph15->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph15->GetYaxis()->SetTitleFont(42);
   Graph_Graph15->GetZaxis()->SetLabelFont(42);
   Graph_Graph15->GetZaxis()->SetTitleSize(0.048);
   Graph_Graph15->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph15);
   
   graph->Draw("c");
   
   leg = new TLegend(0.15,0.2,0.5,0.35,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.045);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(10);
   leg->SetFillStyle(1001);
   entry=leg->AddEntry("Graph0","E. loss  ","f");
   entry->SetFillColor(2);
   entry->SetFillStyle(3004);
   entry->SetLineColor(2);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph3","E. loss + EPS09","f");
   entry->SetFillColor(4);
   entry->SetFillStyle(3004);
   entry->SetLineColor(4);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   
   leg = new TLegend(0.5,0.15,0.92,0.4,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.057);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(10);
   leg->SetFillStyle(1001);
   entry=leg->AddEntry("Graph4","J/#psi  p-Pb ","");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph4","#sqrt{s} = 5 TeV ","");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph4","|y|=0.45","");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   w6->Modified();
   w6->cd();
   w6->SetSelected(w6);
}
