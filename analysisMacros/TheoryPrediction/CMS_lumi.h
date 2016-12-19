#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"

//
// Global variables
//

TString cmsText     = "CMS";
float cmsTextFont   = 61;  // default is helvetic-bold

bool writeExtraText = false;
//bool writeExtraText = true;
TString extraText   = "Preliminary";
float extraTextFont = 52;  // default is helvetica-italics

// text sizes and text offsets with respect to the top frame
// in unit of the top margin size
//float lumiTextSize     = 0.6;
//float lumiTextSize     = 0.6*0.75; // KYO
float lumiTextSize     = 0.6*0.95; // KYO
float lumiTextOffset   = 0.2;
//float cmsTextSize      = 0.75;
//float cmsTextSize      = 0.75*0.75; // KYO
float cmsTextSize      = 0.75*0.95; // KYO
float cmsTextOffset    = 0.1;  // only used in outOfFrame version

float relPosX    = 0.045;
float relPosY    = 0.035;
//float relPosX    = 0.065; //KYO
//float relPosY    = 0.055; //KYO
float relExtraDY = 1.2;
//float relExtraDY = 1.5; // KYO

// ratio of "CMS" and extra text size
float extraOverCmsTextSize  = 0.76;

TString lumi_13TeV = "20.1 fb^{-1}";
TString lumi_8TeV  = "19.7 fb^{-1}";
TString lumi_7TeV  = "5.1 fb^{-1}";
TString lumi_pPb502TeV  = "34.6 nb^{-1}";
TString lumi_pp502TeV  = "28.0 pb^{-1}";
TString lumi_PbPb502TeV  = "346 #mub^{-1}";

bool drawLogo      = false;

void CMS_lumi( TPad* pad, int iPeriod=3, int iPosX=10 );

