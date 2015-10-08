/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 *    File: $Id: RooHistPdfConv.h,v 1.1 2011/02/01 15:56:33 miheejo Exp $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/
#ifndef ROO_HISTPDFCONV
#define ROO_HISTPDFCONV

#include <string>

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooDataHist.h"

class RooHistPdfConv : public RooAbsPdf {
public:

  // enum RooGaussBasis { histBasis=1 };

  // Constructors, assignment etc
  RooHistPdfConv() { }
  RooHistPdfConv(const char *name, const char *title, RooAbsReal& x, 
		RooAbsReal& mean, RooAbsReal& sigma, RooDataHist& datahist) ; 

  RooHistPdfConv(const char *name, const char *title, RooAbsReal& x, 
		RooAbsReal& mean, RooAbsReal& sigma, RooAbsReal& msSF, RooDataHist& datahist ) ; 

  RooHistPdfConv(const char *name, const char *title, RooAbsReal& x, 
		RooAbsReal& mean, RooAbsReal& sigma, RooAbsReal& meanSF, RooAbsReal& sigmaSF, RooDataHist& datahist) ; 

  RooHistPdfConv(const RooHistPdfConv& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooHistPdfConv(*this,newname) ; }
  inline virtual ~RooHistPdfConv() {}
  
  virtual Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
  virtual Double_t analyticalIntegral(Int_t code, const char* rangeName) const ;

  void init() const;
  Double_t cerfInt(Double_t xi) const;

  // Int_t getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t staticInitOK=kTRUE) const;
  // void generateEvent(Int_t code);

protected:

  virtual Double_t evaluate() const ;

  RooRealProxy xIn ;
  RooRealProxy mean ;
  RooRealProxy sigma ;
  RooRealProxy msf ;
  RooRealProxy ssf ;

  RooDataHist* _histpdf;
  std::string _variableName;

  //ClassDef(RooHistPdfConv,1) // Gaussian Resolution Model
};

#endif
