// -*- C++ -*-
//
// Package:    InefficiencyCreator
// Class:      InefficiencyCreator
// 
/**\class InefficiencyCreator InefficiencyCreator.cc MuonAnalysis/TagAndProbe/src/InefficiencyCreator.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Nov 16 16:12 (lxplus231.cern.ch)
//         Created:  Sun Nov 16 16:14:09 CET 2008
// $Id: InefficiencyCreator.cc,v 1.1 2010/05/27 08:30:39 gpetrucc Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Common/interface/OwnVector.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandFlat.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "DataFormats/Candidate/interface/Candidate.h"

template<typename T> 
struct ValueVectorTrait { 
    typedef std::vector<T> type; 
};
template<> struct ValueVectorTrait<reco::Candidate> { 
    typedef edm::OwnVector<reco::Candidate> type; 
};
//
// class decleration
template<typename T>
class InefficiencyCreator : public edm::EDProducer {
    public:
        explicit InefficiencyCreator(const edm::ParameterSet&);
        ~InefficiencyCreator();

        typedef typename ValueVectorTrait<T>::type PlainVecT;
        typedef edm::Ref<PlainVecT>       RefT;
        typedef edm::RefVector<PlainVecT> RefVecT;
        typedef edm::RefToBaseVector<T>   RefBaseVecT;
    private:
        virtual void produce(edm::Event&, const edm::EventSetup&);

        /// The RECO objects
        edm::InputTag src_;

        typedef StringObjectFunction<T> Function;
        /// The Probability
        Function probability_;

        /// The Random number generator
        std::auto_ptr<CLHEP::RandFlat> flatDistribution_;

        enum OutputMode { Values, Refs, RefBases };
        OutputMode outputMode_;
};

template<typename T>
InefficiencyCreator<T>::InefficiencyCreator(const edm::ParameterSet &iConfig) :
    src_(iConfig.getParameter<edm::InputTag>("src")),
    probability_(iConfig.getParameter<std::string>("probability")),
    flatDistribution_(0)
{
    edm::Service<edm::RandomNumberGenerator> rng;
    if ( ! rng.isAvailable()) {
        throw cms::Exception("Configuration")
            << "XXXXXXX requires the RandomNumberGeneratorService\n"
            "which is not present in the configuration file.  You must add the service\n"
            "in the configuration file or remove the modules that require it.";
    }

    CLHEP::HepRandomEngine& engine = rng->getEngine();

    // engine MUST be a reference here, if a pointer is used the
    // distribution will destroy the engine in its destructor, a major
    // problem because the service owns the engine and will destroy it 
    flatDistribution_.reset(new CLHEP::RandFlat(engine, 0, 1));

    std::string outputMode = iConfig.getParameter<std::string>("outputMode");
    if (outputMode == "vector") {
        outputMode_ = Values;
        produces<PlainVecT>(); 
    } else if (outputMode == "RefVector") {
        outputMode_ = Refs;
        produces<RefVecT>(); 
    } else if (outputMode == "RefToBaseVector") {
        outputMode_ = RefBases;
        produces<RefBaseVecT>();
    }


}

template<typename T>
InefficiencyCreator<T>::~InefficiencyCreator() 
{
}

template<typename T>
void
InefficiencyCreator<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    edm::Handle<edm::View<T> > src; 
    iEvent.getByLabel(src_, src);

    // Prepare output
    std::auto_ptr<PlainVecT>   vec;
    std::auto_ptr<RefVecT>     refvec;
    std::auto_ptr<RefBaseVecT> rbvec;
    switch (outputMode_) {
        case Values:   vec.reset(new PlainVecT());      break;
        case Refs:     refvec.reset(new RefVecT());     break;
        case RefBases: rbvec.reset(new RefBaseVecT());  break;
    }

    // Filter and fill
    size_t i, n = src->size(); 
    typename edm::View<T>::const_iterator it;
    for (i = 0, it = src->begin(); i < n; ++i, ++it) {
        const T &t = *it;
        if (flatDistribution_->fire() <= probability_(t)) {
            switch (outputMode_) {
                case Values:   vec->push_back(t); break;
                case Refs:     refvec->push_back(src->refAt(i).template castTo<RefT>()); break;
                case RefBases: rbvec->push_back(src->refAt(i));  break;
            }
        }
    }

    /// Write out output
    switch (outputMode_) {
        case Values:   iEvent.put(vec);    break;
        case Refs:     iEvent.put(refvec); break;
        case RefBases: iEvent.put(rbvec);  break;
    }

}

//define this as a plug-in
typedef InefficiencyCreator<reco::Candidate>   CandidateInefficiencyCreator;
DEFINE_FWK_MODULE(CandidateInefficiencyCreator);

//#include "DataFormats/TrackReco/interface/Track.h"
//#include "DataFormats/PatCandidates/interface/Muon.h"
//typedef InefficiencyCreator<reco::Track>       TrackInefficiencyCreator;
//typedef InefficiencyCreator<pat::Muon>         PATMuonInefficiencyCreator;
//DEFINE_FWK_MODULE(TrackInefficiencyCreator);
//DEFINE_FWK_MODULE(PATMuonInefficiencyCreator);
