#include "../SONGKYO.h"

//string format(int prec, double d);

void read_RFB_rap(bool isPrompt=true)
{
	gROOT->Macro("../Style.C");

  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  
  const int nRap = 3;

	// --- read-in file
	TFile * fIn = new TFile(Form("../DrawFinalPlot/plot_RFB/RFB_rap_isPrompt%d.root",(int)isPrompt),"READ");

  TGraphAsymmErrors* gRFB_lowpt = (TGraphAsymmErrors*)fIn->Get("gRFB_lowpt");
  TGraphAsymmErrors* gRFB_sys_lowpt = (TGraphAsymmErrors*)fIn->Get("gRFB_sys_lowpt");
  TGraphAsymmErrors* gRFB_highpt = (TGraphAsymmErrors*)fIn->Get("gRFB_highpt");
  TGraphAsymmErrors* gRFB_sys_highpt = (TGraphAsymmErrors*)fIn->Get("gRFB_sys_highpt");

  double xPnt_lowpt[nRap];
  double yPnt_lowpt[nRap];
  double yStat_lowpt[nRap];
  double ySyst_lowpt[nRap];
  double xPnt_highpt[nRap];
  double yPnt_highpt[nRap];
  double yStat_highpt[nRap];
  double ySyst_highpt[nRap];

  for (int iy=0; iy< nRap; iy++){
    gRFB_lowpt->GetPoint(iy,xPnt_lowpt[iy],yPnt_lowpt[iy]);
    yStat_lowpt[iy] = gRFB_lowpt->GetErrorY(iy);
    ySyst_lowpt[iy] = gRFB_sys_lowpt->GetErrorY(iy);
    gRFB_highpt->GetPoint(iy,xPnt_highpt[iy],yPnt_highpt[iy]);
    yStat_highpt[iy] = gRFB_highpt->GetErrorY(iy);
    ySyst_highpt[iy] = gRFB_sys_highpt->GetErrorY(iy);
  }
  
  //// for rap 
  int multi=3;
  double rapArrPP[4] = {1.93, 1.5, 0.9, 0};
  int rapPrePP[4] = {2,1,1,0}; //decimal
  
  ///////////////////////// ::: for AN table ::: /////////////////////////
  cout << "\\multirow{"<<multi<<"}{*}{$6.5<\\pt<10$}"<<endl;
  //for (int iy=0; iy< nRap; iy++){
  for (int iy=nRap-1; iy>=0; iy--){ // fw to bw ordering!
    cout <<  " & " << std::fixed << std::setprecision(rapPrePP[iy]) << rapArrPP[iy] << " & " << std::setprecision(rapPrePP[iy+1]) << rapArrPP[iy+1] << " & "; 
    cout << std::setprecision(2) << yPnt_lowpt[iy] << " & " <<  std::setprecision(2)<< yStat_lowpt[iy] << " & "<<  std::setprecision(2)<< ySyst_lowpt[iy] <<"\\\\" <<endl; 
  }
  cout<< "\\hline"<<endl;
  cout << "\\multirow{"<<multi<<"}{*}{$10<\\pt<30$}"<<endl;
  for (int iy=nRap-1; iy>=0; iy--){ // fw to bw ordering!
    cout <<  " & " << std::fixed << std::setprecision(rapPrePP[iy]) << rapArrPP[iy] << " & " << std::setprecision(rapPrePP[iy+1]) << rapArrPP[iy+1] << " & "; 
    cout << std::setprecision(2) << yPnt_highpt[iy] << " & " <<  std::setprecision(2)<< yStat_highpt[iy] << " & "<<  std::setprecision(2)<< ySyst_highpt[iy] <<"\\\\" <<endl; 
  }
  cout<< "\\hline"<<endl;
  
  return;

}

/*
string format(int prec, double d) {
    stringstream ss;
    ss << d;
    ss.str("");
    ss << std::setprecision(prec) << std::fixed << d;

    string str;
    ss >> str;
    string::size_type s;
    for(s=str.length() - 1; s > 0; --s)
    {
        if(str[s] == '0') str.erase(s,1);
        else break;
    }
    if(str[s] == '.') str.erase(s,1);
    return str;
}
*/
