#include "../SONGKYO.h"

//string format(int prec, double d);

void read_cross_rap(int isPA=0, bool isPrompt=true)
{
	gROOT->Macro("../Style.C");

  TString szPA;
  if (isPA==0) szPA="pp";
  else if (isPA==1) szPA="pA";
  else {cout << "select among isPA = 0, or 1"<< endl; return; }
  TString szPrompt;
  if (isPrompt) szPrompt = "PR";
  else szPrompt = "NP"; 
  
  const int nRap = 8;

	// --- read-in file
	TFile * fIn = new TFile(Form("../DrawFinalPlot/plot_cross/%s_cross_rap_isPrompt%d_isScale0.root",szPA.Data(),(int)isPrompt),"READ");

  TGraphAsymmErrors* g_cross_lowpt = (TGraphAsymmErrors*)fIn->Get("g_cross_lowpt");
  TGraphAsymmErrors* g_cross_sys_lowpt = (TGraphAsymmErrors*)fIn->Get("g_cross_sys_lowpt");
  TGraphAsymmErrors* g_cross_highpt = (TGraphAsymmErrors*)fIn->Get("g_cross_highpt");
  TGraphAsymmErrors* g_cross_sys_highpt = (TGraphAsymmErrors*)fIn->Get("g_cross_sys_highpt");

  double xPnt_lowpt[nRap];
  double yPnt_lowpt[nRap];
  double yStat_lowpt[nRap];
  double ySyst_lowpt[nRap];
  double xPnt_highpt[nRap];
  double yPnt_highpt[nRap];
  double yStat_highpt[nRap];
  double ySyst_highpt[nRap];

  for (int iy=0; iy< nRap; iy++){
    g_cross_lowpt->GetPoint(iy,xPnt_lowpt[iy],yPnt_lowpt[iy]);
    yStat_lowpt[iy] = g_cross_lowpt->GetErrorY(iy);
    ySyst_lowpt[iy] = g_cross_sys_lowpt->GetErrorY(iy);
    g_cross_highpt->GetPoint(iy,xPnt_highpt[iy],yPnt_highpt[iy]);
    yStat_highpt[iy] = g_cross_highpt->GetErrorY(iy);
    ySyst_highpt[iy] = g_cross_sys_highpt->GetErrorY(iy);
  }
  
  //// for rap 
  int multi=8;
  double rapArrPP[10] = {-2.4, -1.93, -1.5, -0.9, 0, 0.9, 1.5, 1.93, 2.4};
  int rapPrePP[10] = {1,2,1,1,0,1,1,2,1}; //decimal
  
  double rapArrPA[10] = {-2.87,-2.4, -1.93, -1.5, -0.9, 0, 0.9, 1.5, 1.93};
  int rapPrePA[10] = {2,1,1,1,1,0,1,1,2}; //decimal

  ///////////////////////// ::: for AN table ::: /////////////////////////
  if (isPA==0){
    cout << "\\multirow{"<<multi<<"}{*}{$6.5<\\pt<10$}"<<endl;
    //for (int iy=0; iy< nRap; iy++){
    for (int iy=nRap-1; iy>=0; iy--){ // fw to bw ordering!
      cout <<  " & " << std::fixed << std::setprecision(rapPrePP[iy]) << rapArrPP[iy] << " & " << std::setprecision(rapPrePP[iy+1]) << rapArrPP[iy+1] << " & "; 
      cout << std::setprecision(5) << yPnt_lowpt[iy] << " & " <<  std::setprecision(5)<< yStat_lowpt[iy] << " & "<<  std::setprecision(5)<< ySyst_lowpt[iy] <<"\\\\" <<endl; 
    }
    cout<< "\\hline"<<endl;
    cout << "\\multirow{"<<multi<<"}{*}{$10<\\pt<30$}"<<endl;
    for (int iy=nRap-1; iy>=0; iy--){ // fw to bw ordering!
        cout <<  " & " << std::fixed << std::setprecision(rapPrePP[iy]) << rapArrPP[iy] << " & " << std::setprecision(rapPrePP[iy+1]) << rapArrPP[iy+1] << " & "; 
        cout << std::setprecision(5) << yPnt_highpt[iy] << " & " <<  std::setprecision(5)<< yStat_highpt[iy] << " & "<<  std::setprecision(5)<< ySyst_highpt[iy] <<"\\\\" <<endl; 
    }
    cout<< "\\hline"<<endl;
  }
  else {
    cout << "\\multirow{"<<multi<<"}{*}{$6.5<\\pt<10$}"<<endl;
    for (int iy=nRap-1; iy>=0; iy--){ // fw to bw ordering!
      cout <<  " & " << std::fixed << std::setprecision(rapPrePA[iy]) << rapArrPA[iy] << " & " << std::setprecision(rapPrePA[iy+1]) << rapArrPA[iy+1] << " & "; 
      cout << std::setprecision(3) << yPnt_lowpt[iy] << " & " <<  std::setprecision(3)<< yStat_lowpt[iy] << " & "<<  std::setprecision(3)<< ySyst_lowpt[iy] <<"\\\\" <<endl; 
    }
    cout<< "\\hline"<<endl;
    cout << "\\multirow{"<<multi<<"}{*}{$10<\\pt<30$}"<<endl;
    for (int iy=nRap-1; iy>=0; iy--){ // fw to bw ordering!
        cout <<  " & " << std::fixed << std::setprecision(rapPrePA[iy]) << rapArrPA[iy] << " & " << std::setprecision(rapPrePA[iy+1]) << rapArrPA[iy+1] << " & "; 
        cout << std::setprecision(3) << yPnt_highpt[iy] << " & " <<  std::setprecision(3)<< yStat_highpt[iy] << " & "<<  std::setprecision(3)<< ySyst_highpt[iy] <<"\\\\" <<endl; 
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
