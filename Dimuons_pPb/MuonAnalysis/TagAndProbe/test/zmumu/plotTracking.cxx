/**
*/
#include <TCanvas.h>
#include <TPad.h>
#include "../jpsi/plotUtil.cxx"
TString prefix = "plots_dev/tracking/";
TString basedir  = "tpTreeSta";

TFile *ref = 0;

TCanvas *c1 = 0;
void plotTracking(TString scenario="data",TString match="dr030e030") {
    prefix = prefix+scenario+"/";
    gSystem->mkdir(prefix,true);

    gROOT->ProcessLine(".x /afs/cern.ch/user/g/gpetrucc/cpp/tdrstyle.cc");
    gStyle->SetOptStat(0);
    c1 = new TCanvas("c1","c1");

    if (gROOT->GetListOfFiles()->GetEntries() == 2) {
        ref = (TFile *) gROOT->GetListOfFiles()->At(1);
        ((TFile*) gROOT->GetListOfFiles()->At(0))->cd();
    }

    doRatioPlot = false;
    doDiffPlot  = false;
    doPdf = false;
    doSquare = true; yMin = 0.8; yMax = 1.1;
    datalbl = "Data";
    reflbl  = "Sim.";
    preliminary = ""; //CMS Preliminary, #sqrt{s} = 7 TeV";
    plotTracking_(match);
}

void plotTracking_(TString match) {
    const int nplots = 2;
    const char *plots[nplots] = { "eff_abseta",  "eff_vtx"       };
    const char * vars[nplots] = { "eta",         "tag_nVertices" };
    for (size_t i = 0; i < nplots; ++i) {
        TString plotname(plots[i]); plotname += "_"+match;
        TString varname(vars[i]);
        TDirectory *fit_0 = gFile->GetDirectory(basedir+"/"+plotname+"/");
        if (fit_0 == 0) { std::cerr << "Didn't find " << basedir+"/"+plotname+"/" << " in " << gFile->GetName() << std::endl; continue; }
        TDirectory *fit_1 = gFile->GetDirectory(basedir+"/"+plotname+"NoZ/");
        if (ref != 0) {
            TDirectory *ref_0 = ref->GetDirectory(basedir+"/"+plotname+"/");
            TDirectory *ref_1 = ref->GetDirectory(basedir+"/"+plotname+"NoZ/");
            yMin = 0.9; yMax = 1.019;
            retitle = "Raw efficiency";
            if (fit_0 && ref_0) refstack(fit_0, ref_0, plotname, varname+"_PLOT_");
            retitle = "Fake rate";
            yMin = 0.0; yMax = 1.1;
            if (fit_1 && ref_1) refstack(fit_1, ref_1, plotname+"_fake", varname+"_PLOT_");
            if (fit_0 && fit_1 && ref_0 && ref_1) {
                yMin = 0.9; yMax = 1.019; retitle = "Corrected efficiency";
                TGraphAsymmErrors *corr = corrsingle(fit_0, fit_1, plotname+"_corr",     varname+"_PLOT_", false);
                TGraphAsymmErrors *cref = corrsingle(ref_0, ref_1, plotname+"_corr_ref", varname+"_PLOT_", false);
                if (doFillMC) {
                    cref->SetLineColor(2);
                    cref->SetFillColor(208);
                    cref->SetLineStyle(0);
                    cref->SetMarkerColor(2);
                    cref->SetMarkerStyle(21);
                    cref->SetMarkerSize(0.4);
                } else {
                    cref->SetLineWidth(2);
                    cref->SetLineColor(kRed);
                    cref->SetMarkerColor(kRed);
                    cref->SetMarkerStyle(25);
                    cref->SetMarkerSize(2.0);
                }
                corr->SetLineWidth(2);
                corr->SetLineColor(kBlack);
                corr->SetMarkerColor(kBlack);
                corr->SetMarkerStyle(20);
                corr->SetMarkerSize(1.6);
                cref->Draw("AP");
                corr->Draw("P SAME");
                if (datalbl) doLegend(corr,cref,datalbl,reflbl);
                c1->Print(prefix+plotname+"_corr"+".png");
                if (doPdf) c1->Print(prefix+plotname+"_corr"+".pdf");
                autoScale = false; yMinR = 0.971; yMaxR = 1.029;
                doRatio(corr,cref,plotname+"_corr",corr->GetXaxis()->GetTitle()); 
            }
        } else {
            TDirectory *mc_pt_eta = 0;
            if (mc_pt_eta) {
                //datalbl = "T&P fit"; reflbl = "Sim. truth";
                //extraSpam = "        |#eta| < 1.2"; mcstack(fit_pt_eta, mc_pt_eta, idname+"_pt_barrel",  "pt_PLOT_abseta_bin0_");
                //extraSpam = "  1.2 < |#eta| < 2.4"; mcstack(fit_pt_eta, mc_pt_eta, idname+"_pt_endcaps", "pt_PLOT_abseta_bin1_");
            } else {
                yMin = 0.9; yMax = 1.019; retitle = "Raw efficiency";
                if (fit_0) single(fit_0, plotname, varname+"_PLOT_");
                yMin = 0.0; yMax = 1.1;  retitle = "Fake rate";
                if (fit_1) single(fit_1, plotname+"_fake1", varname+"_PLOT_");
                yMin = 0.9; yMax = 1.019; retitle = "Corrected efficiency";
                if (fit_0 && fit_1) corrsingle(fit_0, fit_1, plotname+"_corr", varname+"_PLOT_");
            }
        }

        if (ref == 0) {
            if (fit_0) doCanvas(fit_0, 1, 50, plotname+"_"+varname+"_%d", varname+"_bin%d__");
            if (fit_1) doCanvas(fit_1, 1, 50, plotname+"_fake_"+varname+"_%d", varname+"_bin%d__");
        }
    }
}


TGraphAsymmErrors* corrsingle(TDirectory *fit, TDirectory *fake, TString alias, TString fitname, bool print=true) {
    if (fake == 0 || fit == 0) return;
    TCanvas *pfake = getFromPrefix(fake->GetDirectory("fit_eff_plots"), fitname);
    if (pfake == 0) {
        std::cerr << "NOT FOUND: " << "fit_eff_plots/"+fitname << " in " << fake->GetName() << std::endl;
        return;
    }
    RooHist *hfake = (RooHist *) pfake->FindObject("hxy_fit_eff");

    TCanvas *pfit = getFromPrefix(fit->GetDirectory("fit_eff_plots"), fitname);
    if (pfit == 0) {
        std::cerr << "NOT FOUND: " << "fit_eff_plots/"+fitname << " in " << fit->GetName() << std::endl;
        return;
    }
    RooHist *hfit = (RooHist *) pfit->FindObject("hxy_fit_eff");

    TGraphAsymmErrors *out = new TGraphAsymmErrors();
    for (int i = 0, n = hfit->GetN(), k = 0; i < n; ++i) {
        double x = hfit->GetX()[i], y = hfit->GetY()[i], eyh = hfit->GetErrorYhigh(i), eyl = hfit->GetErrorYlow(i);
        int j = findBin(hfake, x); if (j == -1) continue;
        double yf  = hfake->GetY()[j], eyhf = hfake->GetErrorYhigh(j), eylf = hfake->GetErrorYlow(j);
        double ycorr =    (   y   -    yf   )/(1-    yf   );
        double ycorr_hi = ((y+eyh)-(yf-eylf))/(1-(yf-eylf));
        double ycorr_lo = ((y-eyl)-(yf+eyhf))/(1-(yf+eyhf));
        /*
        std::cout << "x = " << x << " [" << (x-hfit->GetErrorXlow(i)) << ", " << (x+hfit->GetErrorXhigh(i)) << "] \t" <<
                     "y  = " << y  << " -"<<eyl <<"/+"<<eyh << " \t " <<
                     "yf = " << yf << " -"<<eylf<<"/+"<<eyhf<< " \t " <<
                     "yc = " << ycorr << "[ " << ycorr_lo << ", " << ycorr_hi << "]" << std::endl;
        */
        out->Set(k+1);
        out->SetPoint(k, x, ycorr);
        out->SetPointError(k, hfit->GetErrorXlow(i), hfit->GetErrorXhigh(i), ycorr - ycorr_lo, ycorr_hi - ycorr );
        k++;
    }

    c1->Clear(); c1->cd();
    out->SetLineWidth(2);
    out->SetLineColor(kBlack);
    out->SetMarkerColor(kBlack);
    out->SetMarkerStyle(20);
    out->SetMarkerSize(1.6);
    out->Draw("AP");
    out->GetXaxis()->SetTitle(getXtitle(pfit)); 
    out->GetXaxis()->SetMoreLogLabels(1);
    out->GetXaxis()->SetRangeUser(out->GetX()[0]-out->GetErrorXlow(0), out->GetX()[out->GetN()-1]+out->GetErrorXhigh(out->GetN()-1));
    out->GetYaxis()->SetRangeUser(yMin, yMax);
    out->GetYaxis()->SetDecimals(true);
    out->GetYaxis()->SetTitleOffset(1.3);
    if (retitle != "") out->GetYaxis()->SetTitle(retitle);
    
    if (doSquare) squareCanvas(c1);
    if (preliminary != "") cmsprelim();

    if (print) {
        c1->Print(prefix+alias+".png");
        if (doPdf) c1->Print(prefix+alias+".pdf");
    }

    return out;
}


