#include "../SONGKYO.h"

//string format(int prec, double d);

void read_RFB_pt(bool isPrompt=true)
{
	gROOT->Macro("../Style.C");

  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  
  const int nRap = 3;
  const int nPt = 6;

	// --- read-in file
	TFile * fIn = new TFile(Form("../DrawFinalPlot/plot_RFB/RFB_pt_isPrompt%d.root",(int)isPrompt),"READ");

  TGraphAsymmErrors* gRFB[nRap];
  TGraphAsymmErrors* gRFB_sys[nRap];
  for (int iy=0; iy< nRap; iy++){
    gRFB[iy] = (TGraphAsymmErrors*)fIn->Get(Form("gRFB_%d",iy));
    gRFB_sys[iy] = (TGraphAsymmErrors*)fIn->Get(Form("gRFB_sys_%d",iy));
  }

  double xPnt[nRap][nPt];
  double yPnt[nRap][nPt];
  double yStat[nRap][nPt];
  double ySyst[nRap][nPt];

  for (int iy=0; iy< nRap; iy++){
    for (int ipt=0; ipt< nPt; ipt++){
      gRFB[iy]->GetPoint(ipt,xPnt[iy][ipt],yPnt[iy][ipt]);
      yStat[iy][ipt] = gRFB[iy]->GetErrorY(ipt);
      ySyst[iy][ipt] = gRFB_sys[iy]->GetErrorY(ipt);
    }
  }
 
  //// for pT 
  double ptArr[7]={5,6.5,7.5,8.5,10,14,30};
  int ptPre[7] = {0,1,1,1,0,0,0}; //decimal
  
  //// for rap 
  int multiPA[4] = {6,5,5};
  double rapArrPA[4] = {1.93, 1.5, 0.9, 0};
  int rapPrePA[4] = {2,1,1,0}; //decimal

  ///////////////////////// ::: for AN table ::: /////////////////////////
  for (int iy=0; iy< nRap; iy++){
    cout << "\\multirow{"<<multiPA[iy]<<"}{*}{$"<< std::fixed << std::setprecision(rapPrePA[iy+1]) <<rapArrPA[iy+1]<<" < |\\ycm| < "<< std::setprecision(rapPrePA[iy]) <<rapArrPA[iy] <<"$}"<<endl;
    for (int ipt=0; ipt< nPt; ipt++){
      if (iy>=1 && ipt<=0) {continue;}
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
