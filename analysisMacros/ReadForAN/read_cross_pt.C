#include "../SONGKYO.h"

//string format(int prec, double d);

void read_cross_pt(int isPA=0, bool isPrompt=true)
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
  const int nPt = 9;

	// --- read-in file
	TFile * fIn = new TFile(Form("../DrawFinalPlot/plot_cross/%s_cross_pt_isPrompt%d_isLog0_isScale0.root",szPA.Data(),(int)isPrompt),"READ");

  TGraphAsymmErrors* g_cross[nRap];
  TGraphAsymmErrors* g_cross_sys[nRap];
  for (int iy=0; iy< nRap; iy++){
    g_cross[iy] = (TGraphAsymmErrors*)fIn->Get(Form("g_cross_%d",iy));
    g_cross_sys[iy] = (TGraphAsymmErrors*)fIn->Get(Form("g_cross_sys_%d",iy));
	  //cout << "g_cross["<<iy<<"]: " << g_cross[iy]->GetName() << endl;
	  //cout << "g_cross_sys["<<iy<<"]: " << g_cross_sys[iy]->GetName() << endl;
  }

  double xPnt[nRap][nPt];
  double yPnt[nRap][nPt];
  double yStat[nRap][nPt];
  double ySyst[nRap][nPt];

  for (int iy=0; iy< nRap; iy++){
    for (int ipt=0; ipt< nPt; ipt++){
      g_cross[iy]->GetPoint(ipt,xPnt[iy][ipt],yPnt[iy][ipt]);
      yStat[iy][ipt] = g_cross[iy]->GetErrorY(ipt);
      ySyst[iy][ipt] = g_cross_sys[iy]->GetErrorY(ipt);
    }
  }
 
  //// for pT 
  double ptArr[10]={2,3,4,5,6.5,7.5,8.5,10,14,30};
  int ptPre[10] = {0,0,0,0,1,1,1,0,0,0}; //decimal
  
  //// for rap 
  int multiPP[8] = {9,7,5,5,5,5,7,9};
  double rapArrPP[10] = {2.4, 1.93, 1.5, 0.9, 0, -0.9, -1.5, -1.93, -2.4};
  int rapPrePP[10] = {1,2,1,1,0,1,1,2,1}; //decimal
  
  int multiPA[8] = {9,7,5,5,5,6,7,9};
  double rapArrPA[10] = {1.93, 1.5, 0.9, 0, -0.9, -1.5, -1.93, -2.4,-2.87};
  int rapPrePA[10] = {2,1,1,0,1,1,2,1,2}; //decimal

  ///////////////////////// ::: for AN table ::: /////////////////////////
  if (isPA==0){
    for (int iy=0; iy< nRap; iy++){
      cout << "\\multirow{"<<multiPP[iy]<<"}{*}{$"<< std::fixed << std::setprecision(rapPrePP[iy+1]) <<rapArrPP[iy+1]<<" < \\ycm < "<< std::setprecision(rapPrePP[iy]) <<rapArrPP[iy] <<"$}"<<endl;
      for (int ipt=0; ipt< nPt; ipt++){
        if (iy>=1 && iy<=6 && ipt<=1) {continue;}
        if (iy>=2 && iy<=5 && ipt<=3) {continue;}
        cout <<  " & " << std::fixed << std::setprecision(ptPre[ipt]) << ptArr[ipt] << " & " << std::setprecision(ptPre[ipt+1]) << ptArr[ipt+1] << " & "; 
        cout << std::setprecision(5) << yPnt[iy][ipt] << " & " <<  std::setprecision(6)<< yStat[iy][ipt] << " & "<<  std::setprecision(6)<< ySyst[iy][ipt] <<"\\\\" <<endl; 
        //cout << std::scientific << std::setprecision(2) << yPnt[iy][ipt]*1000 << " & " <<  std::setprecision(1)<< yStat[iy][ipt]*1000 << " & "<<  std::setprecision(1)<< ySyst[iy][ipt]*1000 <<"\\\\" <<endl; 
        //cout << format(15, yPnt[iy][ipt]) << endl;  
      }
    cout<< "\\hline"<<endl;
    }
  }
  else {
    for (int iy=0; iy< nRap; iy++){
      cout << "\\multirow{"<<multiPA[iy]<<"}{*}{$"<< std::fixed << std::setprecision(rapPrePA[iy+1]) <<rapArrPA[iy+1]<<" < \\ycm < "<< std::setprecision(rapPrePA[iy]) <<rapArrPA[iy] <<"$}"<<endl;
      for (int ipt=0; ipt< nPt; ipt++){
        if (iy>=1 && iy<=6 && ipt<=1) {continue;}
        if (iy>=2 && iy<=5 && ipt<=2) {continue;}
        if (iy>=2 && iy<=4 && ipt<=3) {continue;}
        cout <<  " & " << std::fixed << std::setprecision(ptPre[ipt]) << ptArr[ipt] << " & " << std::setprecision(ptPre[ipt+1]) << ptArr[ipt+1] << " & "; 
        cout << std::setprecision(3) << yPnt[iy][ipt] << " & " <<  std::setprecision(4)<< yStat[iy][ipt] << " & "<<  std::setprecision(4)<< ySyst[iy][ipt] <<"\\\\" <<endl; 
      }
    cout<< "\\hline"<<endl;
    }
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
