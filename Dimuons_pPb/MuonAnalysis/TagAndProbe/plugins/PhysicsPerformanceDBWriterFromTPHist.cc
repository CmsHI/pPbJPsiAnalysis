#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondFormats/PhysicsToolsObjects/interface/PerformancePayloadFromTable.h"

#include "CondFormats/PhysicsToolsObjects/interface/PerformanceWorkingPoint.h"

#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>

using namespace std;

class PhysicsPerformanceDBWriterFromTPHist : public edm::EDAnalyzer
{
public:
  PhysicsPerformanceDBWriterFromTPHist(const edm::ParameterSet&);
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&) {}
  virtual void endJob() {}
  ~PhysicsPerformanceDBWriterFromTPHist() {}

private:
  std::vector<std::string> rec1;
  std::vector<std::string> rec2;
  unsigned long long iovBegin;
  unsigned long long iovEnd;  
  std::vector<std::string> inputHistoFiles;
  std::vector<std::string> inputHistogramNames; 
  std::vector<std::string> inputBinningVariables;
  std::vector<std::string> inputResultTypes;
  std::vector<std::string> inputAlgorithmNames;
  std::vector<double> inputDiscriminatorCuts;
  std::string inputConcreteClass; 

};

PhysicsPerformanceDBWriterFromTPHist::PhysicsPerformanceDBWriterFromTPHist
  (const edm::ParameterSet& p)
{
  rec1 = p.getParameter< std::vector <std::string> >("RecordPayloads");
  rec2 = p.getParameter< std::vector <std::string> >("RecordWPs");
  iovBegin = p.getParameter<unsigned long long>("IOVBegin"); 
  iovEnd = p.getParameter<unsigned long long>("IOVEnd"); 
  inputHistoFiles = p.getParameter< std::vector<std::string> >("inputHistoFiles");
  inputHistogramNames = p.getParameter< std::vector<std::string> >("inputHistogramNames"); 
  inputAlgorithmNames = p.getParameter< std::vector<std::string> >("inputAlgorithmNames");
  inputDiscriminatorCuts = p.getParameter< std::vector<double> >("inputDiscriminatorCuts");
  inputConcreteClass = p.getUntrackedParameter<std::string>("inputConcreteClass","PerformancePayloadFromTable");
  inputResultTypes = p.getParameter< std::vector<std::string> >("inputResultTypes");
  inputBinningVariables = p.getParameter< std::vector<std::string> >("inputBinningVariables");  

}

void PhysicsPerformanceDBWriterFromTPHist::beginJob()
{
  //
  // read object from file
  //

  //
  // File Format is
  // - tagger name
  // - cut
  // - concrete class name
  // - how many results and how many binning
  // - their values
  // - vector<float>
  //

  std::string tagger; 
  float cut;  
  std::string concreteType; 
  std::string comment; 
  std::vector<float> pl; 
  std::string infilename;
  std::string histname;
  int stride; 
  int nres, nbin;  
  int number = 0;
  std::string tmprec1, tmprec2;
  unsigned long long tmpiovBegin, tmpiovEnd;

  std::vector<PerformanceResult::ResultType> res;   
  std::vector<BinningVariables::BinningVariablesType> bin; 

  concreteType = inputConcreteClass;  

  nres = inputResultTypes.size();
  cout << nres << " result types" << endl;

  // First read the result types and binning variables
  for(unsigned int i = 0; i < inputResultTypes.size(); ++i)
    {
      int tmp = -1; 
      if(inputResultTypes[i] == "efficiency")
	tmp = 2001;
      else if(inputResultTypes[i] == "efficiency_symerr")
	tmp = 2002; 
      else
	cout << "Unknown efficeincy result type " << inputResultTypes[i] << endl;
      cout << "\tResult type = " << tmp << " (" << inputResultTypes[i] << ")" << endl;
      res.push_back((PerformanceResult::ResultType)(tmp)); 
    } 

  nbin = inputBinningVariables.size();
  cout << nbin << " binning variables" << endl;

  for(unsigned int i = 0; i < inputBinningVariables.size(); ++i) 
    {
      int tmp = -1; 
      if(inputBinningVariables[i] == "pt")
	tmp = 1001;
      else if(inputBinningVariables[i] == "charge")
	tmp = 1002;
      else if(inputBinningVariables[i] == "eta")
	tmp = 1003;
      else if(inputBinningVariables[i] == "phi")
	tmp = 1004;
      else 
	cout << "Unknown binning variable type " << inputBinningVariables[i] << endl; 
      cout << "\tBinning variable = " << tmp << " (" << inputBinningVariables[i] << ")" << endl; 

      bin.push_back((BinningVariables::BinningVariablesType)(tmp)); 
    } 

  stride = nres+nbin*2; 

  // Now read the acutal payload
  for(unsigned int i = 0; i < inputHistoFiles.size(); ++i)
    {
      number = 0; 
      pl.clear();

      infilename = inputHistoFiles[i]; 
      histname = inputHistogramNames[i]; 
      tagger = inputAlgorithmNames[i];
      cut = inputDiscriminatorCuts[i];    
      tmpiovBegin = iovBegin;
      tmpiovEnd = iovEnd;
      tmprec1 = rec1[i];
      tmprec2 = rec2[i];

      cout << "Reading from Tag-&-Probe file " << infilename << endl; 
      cout << "\tReading efficiencies from histograms named " << histname << endl;
      cout << "Algorithm name = " << tagger << endl;
      cout << "Discriminator cut = " << cut << endl;

      TFile *f = TFile::Open(infilename.c_str());
      f->cd();
      TH2F *heff = (TH2F *)f->Get(histname.c_str());

      int nbinx = heff->GetNbinsX();
      int nbiny = heff->GetNbinsY();

      float binlowedgex = -999.;
      float binhighedgex = -999.;
      float binlowedgey = -999.; 
      float binhighedgey = -999.; 
      float bincontent = -999.;
      float binerror = -999.;
      
      for(int j = 1; j < nbinx + 1;j++)
	{
	  binlowedgex = heff->GetXaxis()->GetBinLowEdge(j); 
	  binhighedgex = heff->GetXaxis()->GetBinUpEdge(j);  

	  for(int k = 1;k < nbiny + 1;k++)
	    {
	      binlowedgey = heff->GetYaxis()->GetBinLowEdge(k); 
              binhighedgey = heff->GetYaxis()->GetBinUpEdge(k);  

	      bincontent = heff->GetBinContent(j,k);
	      binerror = heff->GetBinError(j,k);

	      pl.push_back(binlowedgex);
	      pl.push_back(binhighedgex); 
              pl.push_back(binlowedgey); 
              pl.push_back(binhighedgey);  

	      cout << " Inserting " << bincontent << " in position " << number 
		   << " (" << binlowedgex << " < " << inputBinningVariables[0] << " < " << binhighedgex << ", " 
		   << binlowedgey << " < " << inputBinningVariables[1] << " < " << binhighedgey << ")" << endl; 
	      pl.push_back(bincontent); 
	      number++;
	      cout << " Inserting " << binerror << " in position " << number   
		   << " (" << binlowedgex << " < " << inputBinningVariables[0] << " < " << binhighedgex << ", "  
                   << binlowedgey << " < " << inputBinningVariables[1] << " < " << binhighedgey << ")" << endl;  
	      pl.push_back(binerror);  
	      number++;
	    }
	}

      f->Close();


      if (stride != nbin*2+nres)
	{ 
	  std::cout <<" Table not well formed"<<std::endl; 
	} 
      if ((number % stride) != 0)
	{ 
	  std::cout <<" Table not well formed"<<std::endl; 
	} 

      PerformanceWorkingPoint * wp = new PerformanceWorkingPoint(cut, tagger); 
      
      PerformancePayloadFromTable * mupl = 0; 
      
      if (concreteType == "PerformancePayloadFromTable")
	{ 
	  mupl = new PerformancePayloadFromTable(res, bin, stride, pl); 
	}
      else
	{ 
	  cout <<" Non existing request: " <<concreteType<<endl; 
	} 
   
      cout <<" Created the "<<concreteType <<" object"<<endl; 

      // 
      // now create pl etc etc 
      // 
 
      edm::Service<cond::service::PoolDBOutputService> s; 
      if (s.isAvailable()) 
	{ 
	  if (s->isNewTagRequest(tmprec1)) 
	    { 
	      s->createNewIOV<PerformancePayload>(mupl, 
						  tmpiovBegin, 
						  tmpiovEnd, 
						  tmprec1); 
	    } 
	  else 
	    { 
	      s->appendSinceTime<PerformancePayload>(mupl, 
						     tmpiovBegin, 
						     tmprec1); 
	    } 
	  cout << "Wrote payload" << endl;
	} 

      // write also the WP 
   
      if (s.isAvailable()) 
	{ 
	  if (s->isNewTagRequest(tmprec2)) 
	    { 
	      s->createNewIOV<PerformanceWorkingPoint>(wp, 
						       tmpiovBegin, 
						       tmpiovEnd, 
						       tmprec2); 
	    } 
	  else 
	    { 
           
	      s->appendSinceTime<PerformanceWorkingPoint>(wp, 
							  tmpiovBegin, 
							  tmprec2); 
	    } 
	  cout << "Wrote Working Point" << endl;
	}       

    }
}

DEFINE_FWK_MODULE(PhysicsPerformanceDBWriterFromTPHist);
