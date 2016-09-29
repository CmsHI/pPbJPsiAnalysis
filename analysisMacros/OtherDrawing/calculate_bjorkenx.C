#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TString.h>
#include <TROOT.h>
#include "TSystem.h"
#include <TMath.h>
#include <math.h>

void calculate_bjorkenx(Int_t isLHC=1,Int_t useQ = 0, Double_t q_square=13)
{
  /////////////////////////
  // every units in GeV!!
  /////////////////////////
  
  //Double_t ptArr[] = {2.5,17}; 
  //Double_t ptArr[] = {2,2.5,17,30}; 
  //Double_t ptArr[] = {5, 5.75, 6.5, 7, 17, 30}; 
  //Double_t ptArr[] = {7, 17}; 
  Double_t ptArr[] = {0.}; 
  //Double_t rapArr[] = { -2.87, 1.93};
  //Double_t rapArr[] = { -2.165};
  Double_t rapArr[] = {2.4};
  //Double_t rapArr[] = { -1.93, 1.93};
  //Double_t rapArr[] = { -2.4, 1.93};
  //Double_t rapArr[] = {-0.45,0.45};
  const Int_t nPtBins = sizeof(ptArr)/sizeof(double);
  const Int_t nYBins = sizeof(rapArr)/sizeof(double);
  cout << "nPtBins=" << nPtBins << endl;
  cout << "nYBins=" << nYBins << endl;

  // set parameters
  const Double_t  jpsiM = 3.097; // in GeV
  Double_t sqrt_sNN;
  Double_t sqrt_Q; // in GeV
  Double_t bjorkenX1[nYBins][nPtBins];
  Double_t bjorkenX2[nYBins];

  if (isLHC ==1) {
    sqrt_sNN = 5.02*1000; //5.02 TeV :: LHC
  } else if (isLHC==0) {
    sqrt_sNN = 200; // 200 GeV :: RHIC
  }

  // useQ option : 0=use pT // 1=use Q // 2=avg pT to get Q
  if (useQ==1) {
    sqrt_Q = TMath::Sqrt(q_square); // in GeV
  } else if (useQ==2) {
    for (Int_t ipt=0; ipt < nPtBins; ipt++) {
      sqrt_Q += ptArr[ipt];
    }
    sqrt_Q = sqrt_Q / nPtBins;
  } 

  //calculate bjorken x
  cout <<endl<< " ***** sqrt_sNN = "<< sqrt_sNN <<", use Q for calculation = "<<useQ <<" *****" <<endl<<endl;
  for (Int_t ir =0; ir < nYBins; ir++) {
    if (useQ==0) {
      for (Int_t ipt=0; ipt < nPtBins; ipt++) {
        bjorkenX1[ir][ipt] = (1/sqrt_sNN)*TMath::Sqrt(jpsiM*jpsiM + ptArr[ipt]*ptArr[ipt])*TMath::Exp(-rapArr[ir]);
      //cout << "In given pT = " <<ptArr[ipt] <<" (GeV/c), and rapidity y =" << rapArr[ir] << endl;
      //cout << " Q = sqrt( m^2 + pT^2 ) = " <<TMath::Sqrt(jpsiM*jpsiM + ptArr[ipt]*ptArr[ipt]) << endl;
      //cout << "Bjorken x = " << bjorkenX1[ir][ipt] << endl;
      cout << "y = "<<rapArr[ir]<< ", pT ="<<ptArr[ipt]<<" :: Bjorken x = " << bjorkenX1[ir][ipt] << endl;
      cout << " Q = sqrt( m^2 + pT^2 ) = " <<TMath::Sqrt(jpsiM*jpsiM + ptArr[ipt]*ptArr[ipt]) << endl;
      }
      cout << endl;
    } else if (useQ==1 || useQ ==2) {
        bjorkenX2[ir] = (1/sqrt_sNN)*sqrt_Q*TMath::Exp(-rapArr[ir]);
        cout << "In given sqrt_Q = " <<sqrt_Q <<" (GeV/c), and rapidity y =" << rapArr[ir] << endl;
        cout << "Bjorken x = " << bjorkenX2[ir] << endl;
    }
  }

  return;
}

