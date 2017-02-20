#include "../SONGKYO.h"

//string format(int prec, double d);

void read_RpPb_pt(bool isPrompt=true)
{
	gROOT->Macro("../Style.C");

  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  
  const int nRap = 7;
  const int nPt = 9;

	// --- read-in file
	TFile * fIn = new TFile(Form("../DrawFinalPlot/plot_RpPb/RpPb_pt_isPrompt%d.root",(int)isPrompt),"READ");

  TGraphAsymmErrors* g_RpPb[nRap];
  TGraphAsymmErrors* g_RpPb_sys[nRap];
  for (int iy=0; iy< nRap; iy++){
    g_RpPb[iy] = (TGraphAsymmErrors*)fIn->Get(Form("g_RpPb_%d",iy));
    g_RpPb_sys[iy] = (TGraphAsymmErrors*)fIn->Get(Form("g_RpPb_sys_%d",iy));
  }

  double xPnt[nRap][nPt];
  double yPnt[nRap][nPt];
  double yStat[nRap][nPt];
  double ySyst[nRap][nPt];

  for (int iy=0; iy< nRap; iy++){
    for (int ipt=0; ipt< nPt; ipt++){
      g_RpPb[iy]->GetPoint(ipt,xPnt[iy][ipt],yPnt[iy][ipt]);
      yStat[iy][ipt] = g_RpPb[iy]->GetErrorY(ipt);
      ySyst[iy][ipt] = g_RpPb_sys[iy]->GetErrorY(ipt);
    }
  }
 
  //// for pT 
  double ptArr[10]={2,3,4,5,6.5,7.5,8.5,10,14,30};
  int ptPre[10] = {0,0,0,0,1,1,1,0,0,0}; //decimal
  
  //// for rap 
  int multiPA[7] = {7,5,5,5,5,6,7};
  double rapArrPA[8] = {1.93, 1.5, 0.9, 0, -0.9, -1.5, -1.93, -2.4};
  int rapPrePA[8] = {2,1,1,0,1,1,2,1}; //decimal

  ///////////////////////// ::: for AN table ::: /////////////////////////
  for (int iy=0; iy< nRap; iy++){
    cout << "\\multirow{"<<multiPA[iy]<<"}{*}{$"<< std::fixed << std::setprecision(rapPrePA[iy+1]) <<rapArrPA[iy+1]<<" < \\ycm < "<< std::setprecision(rapPrePA[iy]) <<rapArrPA[iy] <<"$}"<<endl;
    for (int ipt=0; ipt< nPt; ipt++){
      if (ipt<=1) {continue;}
      if (iy>=1 && iy<=5 && ipt<=2) {continue;}
      if (iy>=1 && iy<=4 && ipt<=3) {continue;}
      cout <<  " & " << std::fixed << std::setprecision(ptPre[ipt]) << ptArr[ipt] << " & " << std::setprecision(ptPre[ipt+1]) << ptArr[ipt+1] << " & "; 
      cout << std::setprecision(2) << yPnt[iy][ipt] << " & " <<  std::setprecision(2)<< yStat[iy][ipt] << " & "<<  std::setprecision(2)<< ySyst[iy][ipt] <<"\\\\" <<endl; 
    }
  cout<< "\\hline"<<endl;
  }
  
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
