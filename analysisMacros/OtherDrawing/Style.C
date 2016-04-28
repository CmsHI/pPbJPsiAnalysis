{
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  
  gStyle->SetTitleFillColor(0);
  gStyle->SetStatColor(0);
  
  gStyle->SetPaintTextFormat(".3f");

  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameLineColor(kBlack);

  gStyle->SetCanvasColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasBorderSize(0);

  gStyle->SetPadColor(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBorderSize(0);

  gStyle->SetTextSize(0.04);
  gStyle->SetTextFont(42);
  gStyle->SetLabelFont(42,"xyz");
  gStyle->SetLabelSize(0.04,"xyz");
  gStyle->SetTitleFont(42,"xyz");
  gStyle->SetTitleSize(0.048,"xyz");
  gStyle->SetPadTopMargin(0.03);
  //gStyle->SetPadTopMargin(0.075); //KYO
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadRightMargin(0.04); //KYO
  //gStyle->SetPadRightMargin(0.07);
  //gStyle->SetPadLeftMargin(0.17); //KYO
  gStyle->SetPadLeftMargin(0.12);
  //gStyle->SetTitleXOffset(1.0); //KYO
  gStyle->SetTitleXOffset(1.15);
  //gStyle->SetTitleYOffset(1.11); //KYO
  gStyle->SetTitleYOffset(1.2);

  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  gStyle->SetCanvasDefH(600);
  gStyle->SetCanvasDefW(600);

  gStyle->SetHistMinimumZero(kTRUE);

//  gStyle->SetErrorX(0); // disable if you want to draw horizontal error bars, e.g. when having variable bin size
  gStyle->SetEndErrorSize(0);

  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(0.8);

  gROOT->ForceStyle();
}
