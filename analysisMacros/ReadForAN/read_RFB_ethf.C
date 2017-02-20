#include "../SONGKYO.h"

//string format(int prec, double d);

void read_RFB_ethf(bool isPrompt=true)
{
	gROOT->Macro("../Style.C");

  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  
  const int nRap = 3;
  const int nPt = 2;
  const int nEt = 3;
  const int nHist = 4;

	// --- read-in file
	TFile * fIn = new TFile(Form("../DrawFinalPlot/plot_RFB/RFB_ethf_isPrompt%d.root",(int)isPrompt),"READ");

  TGraphAsymmErrors* gRFB[nHist];
  TGraphAsymmErrors* gRFB_sys[nHist];
  for (int inh=0; inh< nHist; inh++){
    gRFB[inh] = (TGraphAsymmErrors*)fIn->Get(Form("gRFB_%d",inh));
    gRFB_sys[inh] = (TGraphAsymmErrors*)fIn->Get(Form("gRFB_sys_%d",inh));
  }

  double xPnt[nHist][nEt];
  double yPnt[nHist][nEt];
  double yStat[nHist][nEt];
  double ySyst[nHist][nEt];

  for (int inh=0; inh< nHist; inh++){
    for (int iet=0; iet< nEt; iet++){
      gRFB[inh]->GetPoint(iet,xPnt[inh][iet],yPnt[inh][iet]);
      yStat[inh][iet] = gRFB[inh]->GetErrorY(iet);
      ySyst[inh][iet] = gRFB_sys[inh]->GetErrorY(iet);
    }
  }
  
  //// for ET 
  TString etArr[4]={"0","20","30","\\infty"};
 
  //// for pT 
  double ptArr[3]={5,6.5,30};
  int ptPre[3] = {0,1,0}; //decimal
  
  //// for rap 
  int multiPA[4] = {6,3,3};
  double rapArrPA[4] = {1.93, 1.5, 0.9, 0};
  int rapPrePA[4] = {2,1,1,0}; //decimal

  ///////////////////////// ::: for AN table ::: /////////////////////////
  for (int iy=nRap-1; iy>=0; iy--){ // fw to bw ordering!
    cout << "\\multirow{"<<multiPA[iy]<<"}{*}{$"<< std::fixed << std::setprecision(rapPrePA[iy+1]) <<rapArrPA[iy+1]<<" < |\\ycm| < "<< std::setprecision(rapPrePA[iy]) <<rapArrPA[iy] <<"$}"<<endl;
    for (int ipt=0; ipt< nPt; ipt++){
      if (iy>=1 && ipt<=0) {continue;}
      for (int iet=0; iet<nEt; iet++){
        cout <<  " & " << std::fixed << std::setprecision(ptPre[ipt]) << ptArr[ipt] << " & " << std::setprecision(ptPre[ipt+1]) << ptArr[ipt+1] << " & "; 
        if (iet==nEt-1) cout << "$[" << etArr[iet].Data() << ","<< etArr[iet+1].Data() << ")$ & "; 
        else cout << "$[" << etArr[iet].Data() << ","<< etArr[iet+1].Data() << "]$ & "; 
        //// set inh
        int inh;
        if (iy==2) inh=3; // pT 6.5-30, rap 0.0-0.9 
        else if (iy==1) inh=2; //pT 6.5-30, rap 0.9-1.5 
        else if (iy==0 && ipt==1) inh=1; // pT 6.5-30, rap 1.5-1.93
        else if (iy==0 && ipt==0) inh=0; // pT 5-6.5, rap 1.5-1.93
        cout << std::setprecision(2) << yPnt[inh][iet] << " & " <<  std::setprecision(2)<< yStat[inh][iet] << " & "<<  std::setprecision(2)<< ySyst[inh][iet] <<"\\\\" <<endl; 
      }
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
