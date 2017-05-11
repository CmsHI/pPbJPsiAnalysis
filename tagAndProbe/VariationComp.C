//Headers{{{
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TLegend.h>
#include <iostream>
#include <fstream>
#include <TLatex.h>
//}}}

void VariationComp()
{
	const Int_t NOV = 5;//Number Of Variation
	TCanvas *c1 = new TCanvas("c1", "", 0, 0, 1500, 400);
	TH1F *heta1[NOV];
	TH1F *heta2[NOV];
	TH1F *heta3[NOV];
	TH1F *heta4[NOV];
	TH1F *heta5[NOV];
	Double_t Binning_eta1[10] = {3.3, 3.8, 4.3, 5, 6, 7, 8, 10, 13, 30};
	Double_t Binning_eta2[11] = {3.3, 3.5, 3.9, 4.3, 5, 6, 7, 8, 10, 13, 30};
	Double_t Binning_eta3[11] = {2.15, 3.0, 3.6, 4.3, 5, 6, 7, 8, 10, 13, 30};
	Double_t Binning_eta4[11] = {1.6, 2.2, 2.5, 2.8, 3.5, 4.5, 5, 6, 10, 12, 30};
	Double_t Binning_eta5[8] = {1.3, 1.6, 2.2, 3.4, 4.8, 6.0, 9.0, 30.0};
//kind of diff, pt bin

//Define variable{{{
	Char_t ptchar1;
	Char_t ptchar2;
	Char_t varchar1;
	Char_t varchar2;
	Char_t varchar3;
	Char_t varchar4;
	Char_t varchar5;
	Char_t varchar6;
	Char_t varchar7;
	Double_t ptmin;
	Double_t ptmax;
	Double_t varnarrow;
	Double_t var2G;
	Double_t varpol1;
	Double_t varMulti;
	Double_t varPAMu7;
	Double_t varstat;
	Double_t vartotal;
//}}}

//Define histogram{{{
	for(Int_t ih = 0; ih < NOV; ih++)
	{
		heta1[ih] = new TH1F(Form("heta1_%d", ih+1), "", 9, Binning_eta1);
		heta2[ih] = new TH1F(Form("heta2_%d", ih+1), "", 10, Binning_eta2);
		heta3[ih] = new TH1F(Form("heta3_%d", ih+1), "", 10, Binning_eta3);
		heta4[ih] = new TH1F(Form("heta4_%d", ih+1), "", 10, Binning_eta4);
		heta5[ih] = new TH1F(Form("heta5_%d", ih+1), "", 7, Binning_eta5);
	}
//}}}

	ifstream Variationfile;
	Variationfile.open("SingleMuon_error_v24_smear_v3.txt");
	{
		Variationfile.ignore(100000000000000, '\n');

//eta1{{{
		Variationfile.ignore(20, '\n');
		for(Int_t ipt = 0; ipt < 9; ipt++)
		{
			Variationfile >> ptchar1 >> ptmin >> ptchar2 >> ptmax >> varchar1 >> varnarrow >> varchar2 >> var2G >> varchar3 >> varpol1 >> varchar4 >> varMulti >> varchar5 >> varPAMu7 >> varchar6 >> varstat >> varchar7 >> vartotal; 

			cout << ptmin << " " << ptmax << " " << varnarrow << " " << var2G << " " << varpol1 << " " << varMulti << " " << varPAMu7 << endl;
			heta1[0]->SetBinContent(ipt+1, varnarrow);
			heta1[1]->SetBinContent(ipt+1, var2G);
			heta1[2]->SetBinContent(ipt+1, varpol1);
			heta1[3]->SetBinContent(ipt+1, varMulti);
			heta1[4]->SetBinContent(ipt+1, varPAMu7);
		}
//}}}

//eta2{{{
		Variationfile.ignore(20, '\n');
		Variationfile.ignore(20, '\n');
		for(Int_t ipt = 0; ipt < 10; ipt++)
		{
			Variationfile >> ptchar1 >> ptmin >> ptchar2 >> ptmax >> varchar1 >> varnarrow >> varchar2 >> var2G >> varchar3 >> varpol1 >> varchar4 >> varMulti >> varchar5 >> varPAMu7 >> varchar6 >> varstat >> varchar7 >> vartotal; 

			cout << ptmin << " " << ptmax << " " << varnarrow << " " << var2G << " " << varpol1 << " " << varMulti << " " << varPAMu7 << endl;
			heta2[0]->SetBinContent(ipt+1, varnarrow);
			heta2[1]->SetBinContent(ipt+1, var2G);
			heta2[2]->SetBinContent(ipt+1, varpol1);
			heta2[3]->SetBinContent(ipt+1, varMulti);
			heta2[4]->SetBinContent(ipt+1, varPAMu7);
		}
//}}}

//eta3{{{
		Variationfile.ignore(20, '\n');
		Variationfile.ignore(20, '\n');
		for(Int_t ipt = 0; ipt < 10; ipt++)
		{
			Variationfile >> ptchar1 >> ptmin >> ptchar2 >> ptmax >> varchar1 >> varnarrow >> varchar2 >> var2G >> varchar3 >> varpol1 >> varchar4 >> varMulti >> varchar5 >> varPAMu7 >> varchar6 >> varstat >> varchar7 >> vartotal; 

			cout << ptmin << " " << ptmax << " " << varnarrow << " " << var2G << " " << varpol1 << " " << varMulti << " " << varPAMu7 << endl;
			heta3[0]->SetBinContent(ipt+1, varnarrow);
			heta3[1]->SetBinContent(ipt+1, var2G);
			heta3[2]->SetBinContent(ipt+1, varpol1);
			heta3[3]->SetBinContent(ipt+1, varMulti);
			heta3[4]->SetBinContent(ipt+1, varPAMu7);
		}
//}}}

//eta4{{{
		Variationfile.ignore(20, '\n');
		Variationfile.ignore(20, '\n');
		for(Int_t ipt = 0; ipt < 10; ipt++)
		{
			Variationfile >> ptchar1 >> ptmin >> ptchar2 >> ptmax >> varchar1 >> varnarrow >> varchar2 >> var2G >> varchar3 >> varpol1 >> varchar4 >> varMulti >> varchar5 >> varPAMu7 >> varchar6 >> varstat >> varchar7 >> vartotal; 

			cout << ptmin << " " << ptmax << " " << varnarrow << " " << var2G << " " << varpol1 << " " << varMulti << " " << varPAMu7 << endl;
			heta4[0]->SetBinContent(ipt+1, varnarrow);
			heta4[1]->SetBinContent(ipt+1, var2G);
			heta4[2]->SetBinContent(ipt+1, varpol1);
			heta4[3]->SetBinContent(ipt+1, varMulti);
			heta4[4]->SetBinContent(ipt+1, varPAMu7);
		}
//}}}

//eta5{{{
		Variationfile.ignore(20, '\n');
		Variationfile.ignore(20, '\n');
		for(Int_t ipt = 0; ipt < 7; ipt++)
		{
			Variationfile >> ptchar1 >> ptmin >> ptchar2 >> ptmax >> varchar1 >> varnarrow >> varchar2 >> var2G >> varchar3 >> varpol1 >> varchar4 >> varMulti >> varchar5 >> varPAMu7 >> varchar6 >> varstat >> varchar7 >> vartotal; 

			cout << ptmin << " " << ptmax << " " << varnarrow << " " << var2G << " " << varpol1 << " " << varMulti << " " << varPAMu7 << endl;
			heta5[0]->SetBinContent(ipt+1, varnarrow);
			heta5[1]->SetBinContent(ipt+1, var2G);
			heta5[2]->SetBinContent(ipt+1, varpol1);
			heta5[3]->SetBinContent(ipt+1, varMulti);
			heta5[4]->SetBinContent(ipt+1, varPAMu7);
		}
//}}}

	}
	Variationfile.close();

	gStyle->SetOptStat(0000);
	c1->SetLogy();
	c1->Divide(5, 1, 0.05, 0.0);

//Draw eta1{{{
	c1->cd(1);
	heta1[0]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	heta1[0]->GetYaxis()->SetTitle("diff");
	//heta1[0]->SetMaximum(0.05);//high pt
	heta1[0]->SetMaximum(0.05);//low pt
	//heta1[0]->SetMaximum(0.03);//without PAMu7
	//heta1[0]->GetXaxis()->SetRange(3.3, 5.0);//low pt
	//heta1[0]->GetXaxis()->SetRange(5.0, 30.0);//high pt
	heta1[0]->SetLineColor(2);
	heta1[0]->SetLineWidth(2);
	heta1[0]->Draw();
	heta1[1]->SetLineColor(4);
	heta1[1]->SetLineWidth(2);
	heta1[1]->Draw("same");
	heta1[2]->SetLineColor(210);
	heta1[2]->SetLineWidth(2);
	heta1[2]->Draw("same");
	heta1[3]->SetLineColor(1);
	heta1[3]->SetLineWidth(2);
	heta1[3]->Draw("same");
	heta1[4]->SetLineColor(6);
	heta1[4]->SetLineWidth(2);
	heta1[4]->Draw("same");

	TLatex *texeta1 = new TLatex(0.3,0.8, "     |#eta| < 0.9");
	texeta1->SetNDC();
	texeta1->Draw();
//}}}

//Draw eta2{{{
	c1->cd(2);
	heta2[0]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	heta2[0]->GetYaxis()->SetTitle("diff");
	heta2[0]->SetMaximum(0.1);//high pt
	//heta2[0]->SetMaximum(0.1);//low pt
	//heta2[0]->SetMaximum(0.03);//without PAMu7
	//heta2[0]->GetXaxis()->SetRange(3.3, 5.0);//low pt
	//heta2[0]->GetXaxis()->SetRange(5.0, 30.0);//high pt
	heta2[0]->SetLineColor(2);
	heta2[0]->SetLineWidth(2);
	heta2[0]->Draw();
	heta2[1]->SetLineColor(4);
	heta2[1]->SetLineWidth(2);
	heta2[1]->Draw("same");
	heta2[2]->SetLineColor(210);
	heta2[2]->SetLineWidth(2);
	heta2[2]->Draw("same");
	heta2[3]->SetLineColor(1);
	heta2[3]->SetLineWidth(2);
	heta2[3]->Draw("same");
	heta2[4]->SetLineColor(6);
	heta2[4]->SetLineWidth(2);
	heta2[4]->Draw("same");

	TLatex *texeta2 = new TLatex(0.3,0.8,"0.9 < |#eta| < 1.2");
	texeta2->SetNDC();
	texeta2->Draw();
//}}}

//Draw eta3{{{
	c1->cd(3);
	heta3[0]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	heta3[0]->GetYaxis()->SetTitle("diff");
	heta3[0]->SetMaximum(0.1);//high pt
	//heta3[0]->SetMaximum(0.1);//low pt
	//heta3[0]->SetMaximum(0.03);//without PAMu7
	//heta3[0]->GetXaxis()->SetRange(2.16, 5.0);//low pt
	//heta3[0]->GetXaxis()->SetRange(5.0, 30.0);//high pt
	heta3[0]->SetLineColor(2);
	heta3[0]->SetLineWidth(2);
	heta3[0]->Draw();
	heta3[1]->SetLineColor(4);
	heta3[1]->SetLineWidth(2);
	heta3[1]->Draw("same");
	heta3[2]->SetLineColor(210);
	heta3[2]->SetLineWidth(2);
	heta3[2]->Draw("same");
	heta3[3]->SetLineColor(1);
	heta3[3]->SetLineWidth(2);
	heta3[3]->Draw("same");
	heta3[4]->SetLineColor(6);
	heta3[4]->SetLineWidth(2);
	heta3[4]->Draw("same");

	TLatex *texeta3 = new TLatex(0.3,0.8,"1.2 < |#eta| < 1.6");
	texeta3->SetNDC();
	texeta3->Draw();
//}}}

//Draw eta4{{{
	c1->cd(4);
	heta4[0]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	heta4[0]->GetYaxis()->SetTitle("diff");
	heta4[0]->SetMaximum(0.1);//high pt
	//heta4[0]->SetMaximum(0.1);//low pt
	//heta4[0]->SetMaximum(0.05);//without PAMu7
	//heta4[0]->GetXaxis()->SetRange(1.6, 5.0);//low pt
	//heta4[0]->GetXaxis()->SetRange(5.0, 30.0);//higt pt
	heta4[0]->SetLineColor(2);
	heta4[0]->SetLineWidth(2);
	heta4[0]->Draw();
	heta4[1]->SetLineColor(4);
	heta4[1]->SetLineWidth(2);
	heta4[1]->Draw("same");
	heta4[2]->SetLineColor(210);
	heta4[2]->SetLineWidth(2);
	heta4[2]->Draw("same");
	heta4[3]->SetLineColor(1);
	heta4[3]->SetLineWidth(2);
	heta4[3]->Draw("same");
	heta4[4]->SetLineColor(6);
	heta4[4]->SetLineWidth(2);
	heta4[4]->Draw("same");

	TLatex *texeta4 = new TLatex(0.3,0.8,"1.6 < |#eta| < 2.1");
	texeta4->SetNDC();
	texeta4->Draw();
//}}}

//Draw eta5{{{
	c1->cd(5);
	heta5[0]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	heta5[0]->GetYaxis()->SetTitle("diff");
	//heta5[0]->SetMaximum(0.1);//high pt
	heta5[0]->SetMaximum(0.25);//low pt
	//heta5[0]->SetMaximum(0.05);//high pt without PAMu7
	//heta5[0]->SetMaximum(0.15);//low pt without PAMu7
	//heta5[0]->GetXaxis()->SetRange(1.3, 5.0);//low pt
	//heta5[0]->GetXaxis()->SetRange(5.0, 30.0);//high pt
	heta5[0]->SetLineColor(2);
	heta5[0]->SetLineWidth(2);
	heta5[0]->Draw();
	heta5[1]->SetLineColor(4);
	heta5[1]->SetLineWidth(2);
	heta5[1]->Draw("same");
	heta5[2]->SetLineColor(210);
	heta5[2]->SetLineWidth(2);
	heta5[2]->Draw("same");
	heta5[3]->SetLineColor(1);
	heta5[3]->SetLineWidth(2);
	heta5[3]->Draw("same");
	heta5[4]->SetLineColor(6);
	heta5[4]->SetLineWidth(2);
	heta5[4]->Draw("same");

	TLatex* texeta5 = new TLatex(0.3,0.8,"2.1 < |#eta| < 2.4");
	texeta5->SetNDC();
	texeta5->Draw();
//}}}

	TLegend *leg = new TLegend(0.6, 0.7, 0.9, 0.95);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->AddEntry(heta5[0], "mass range", "l");
	leg->AddEntry(heta5[1], "Signal", "l");
	leg->AddEntry(heta5[2], "Background", "l");
	leg->AddEntry(heta5[3], "Multiplicity", "l");
	leg->AddEntry(heta5[4], "TagTrigger", "l");
	leg->Draw();
	c1->SaveAs("VariationComp_v24_5eta_v3.png");
}
