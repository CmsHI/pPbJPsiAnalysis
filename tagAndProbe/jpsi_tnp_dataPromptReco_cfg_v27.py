import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring(),
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )    


process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

import os
if   "CMSSW_5_3_" in os.environ['CMSSW_VERSION']:
    #process.GlobalTag.globaltag = cms.string('START53_V27::All')  #for MC
    #process.GlobalTag.globaltag = cms.string( 'GR_P_V43F::All' ) #for data /PAMuon/HIRun2013-28Sep2013-v1/RECO
    process.GlobalTag.globaltag = cms.string( 'GR_P_V43D::All' ) #for data /PAMuon/HIRun2013-PromptReco-v1/RECO
    
    process.source.fileNames = [
        'root://xrootd.unl.edu//store/hidata/HIRun2013/PAMuon/RECO/PromptReco-v1/000/210/818/00000/707A9589-0369-E211-B7E1-0015C5FDE067.root'
        #'file:/afs/cern.ch/work/g/ginnocen/38291323-E567-E211-8DD9-5404A63886C5.root', #for data
    ]
elif "CMSSW_5_2_" in os.environ['CMSSW_VERSION']:
    process.GlobalTag.globaltag = cms.string('GR_P_V39_AN1::All')
    process.source.fileNames = [
        '/store/data/Run2012A/MuOnia/AOD/PromptReco-v1/000/191/226/645E9BE9-FC87-E111-9D30-5404A63886C5.root',
        '/store/data/Run2012A/MuOnia/AOD/PromptReco-v1/000/191/226/60C36C17-0188-E111-8B3D-5404A638868F.root',
        '/store/data/Run2012A/MuOnia/AOD/PromptReco-v1/000/191/226/5E384785-F087-E111-A8CB-BCAEC518FF80.root',
        '/store/data/Run2012A/MuOnia/AOD/PromptReco-v1/000/191/226/5C605E9F-F887-E111-A540-00237DDC5CB0.root',
        '/store/data/Run2012A/MuOnia/AOD/PromptReco-v1/000/191/226/5C0DCA92-CE87-E111-A539-5404A63886C6.root',
        '/store/data/Run2012A/MuOnia/AOD/PromptReco-v1/000/191/226/5A856F46-1888-E111-A3A5-BCAEC5329700.root',
        '/store/data/Run2012A/MuOnia/AOD/PromptReco-v1/000/191/226/5271187F-DF87-E111-ADDB-BCAEC518FF7C.root',
    ]
else: raise RuntimeError, "Unknown CMSSW version %s" % os.environ['CMSSW_VERSION']



# Common offline event selection
process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")

# Centrality for pPb
############### added by yongsun ################
process.load('RecoHI.HiCentralityAlgos.pACentrality_cfi')

from HeavyIonsAnalysis.Configuration.CommonFunctions_cff import *
overrideCentrality(process)

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFtowersTrunc"),
    nonDefaultGlauberModel = cms.string(""),
    centralitySrc = cms.InputTag("pACentrality"),
    pPbRunFlip = cms.untracked.uint32(211313)
    )
########################################




process.load("HLTrigger.HLTfilters.triggerResultsFilter_cfi")
process.triggerResultsFilter.triggerConditions = cms.vstring( 'HLT_PAMu3_v*' )
process.triggerResultsFilter.l1tResults = ''
process.triggerResultsFilter.throw = True
process.triggerResultsFilter.hltResults = cms.InputTag( "TriggerResults", "", "HLT" )
process.HLTMu   = process.triggerResultsFilter.clone(triggerConditions = [ 'HLT_PAMu3_v*' ])

## ==== Merge CaloMuons and Tracks into the collection of reco::Muons  ====
from RecoMuon.MuonIdentification.calomuons_cfi import calomuons;
process.mergedMuons = cms.EDProducer("CaloMuonMerger",
                                     mergeTracks = cms.bool(False),
                                     mergeCaloMuons = cms.bool(True), # AOD
                                     muons     = cms.InputTag("muons"), 
                                     caloMuons = cms.InputTag("calomuons"),
                                     tracks    = cms.InputTag("generalTracks"),
                                     minCaloCompatibility = calomuons.minCaloCompatibility,
                                     ## Apply some minimal pt cut
                                     muonsCut     = cms.string("track.isNonnull && pt > 1"),
                                     caloMuonsCut = cms.string("pt > 1"),
                                     tracksCut    = cms.string("pt > 1"),
                                     )

## ==== Trigger matching
process.load("MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff")
## with some customization from Hyunchul
process.muonL1Info.maxDeltaR = 0.3
process.muonL1Info.fallbackToME1 = True
process.muonMatchHLTL1.maxDeltaR = 0.3
process.muonMatchHLTL1.fallbackToME1 = True
process.muonMatchHLTL2.maxDeltaR = 0.3
process.muonMatchHLTL2.maxDPtRel = 10.0
process.muonMatchHLTL3.maxDeltaR = 0.1
process.muonMatchHLTL3.maxDPtRel = 10.0
process.muonMatchHLTCtfTrack.maxDeltaR = 0.1
process.muonMatchHLTCtfTrack.maxDPtRel = 10.0
process.muonMatchHLTTrackMu.maxDeltaR = 0.1
process.muonMatchHLTTrackMu.maxDPtRel = 10.0

from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import *
changeRecoMuonInput(process, "mergedMuons")
useExtendedL1Match(process)
addHLTL1Passthrough(process)
changeTriggerProcessName(process, "*")

IN_ACCEPTANCE = "(abs(eta) < 2.4 && ((abs(eta) < 1.2 && pt >= 3.3) || (1.2 <= abs(eta) && abs(eta) < 1.6 && pt >= -(1.4/0.4)*abs(eta)+1.2*(1.4/0.4)+2.6) || (1.6 <= abs(eta) && abs(eta) < 2.1 && pt >= -(0.4/0.5)*abs(eta)+1.6*(0.4/0.5)+1.2) || (2.1 <= abs(eta) && pt >= 0.8)))"
IN_ACCEPTANCEB = "(abs(eta) < 2.4 && ((abs(eta) < 1.2 && pt >= 3.3) || (1.2 <= abs(eta) && abs(eta) < 1.6 && pt >= 2.0) || (1.6 <= abs(eta) && abs(eta) < 2.1 && pt >= -(.9/.5)*abs(eta)+1.6*(.9/.5)+2.0) || (2.1 <= abs(eta) && pt >= 1.1)))"
IN_ACCEPTANCED = "(abs(eta) < 2.4 && ((abs(eta) < 1.2 && pt >= 3.3) || (1.2 <= abs(eta) && abs(eta) < 1.6 && pt >= 2.0) || (1.6 <= abs(eta) && abs(eta) < 2.1 && pt >= -(.4/.5)*abs(eta)+1.6*(.4/.5)+2.0) || (2.1 <= abs(eta) && pt >= 1.3)))"
IN_ACCEPTANCEG = "(abs(eta) < 2.4 && ((abs(eta) < 1.2 && pt >= 3.3) || (1.2 <= abs(eta) && abs(eta) < 2.1 && pt >= -(1.0/.9)*abs(eta)+1.2*(1.0/.9)+2.6) || (2.1 <= abs(eta) && pt >= 1.3)))"
IN_ACCEPTANCEOLD = "(abs(eta) < 2.4 && ((abs(eta) < 1.3 && pt >= 3.3) || (1.3 <= abs(eta) && abs(eta) < 2.2 && p >= 2.9) || (2.2 <= abs(eta) && pt >= 0.8)))"
#old track and muon id cuts until Feb. 15th

#TRACK_CUTS = "isTrackerMuon && track.quality('highPurity')"
#MUONID = "muonID('TrackerMuonArbitrated') && muonID('TMOneStationTight') && (track.hitPattern.trackerLayersWithMeasurement > 5) && (track.normalizedChi2 < 100000) && (track.hitPattern.pixelLayersWithMeasurement > 0) && (abs(dB) < 0.3) && (abs(track.dz) < 20)"
TRACKaMUON = "isTrackerMuon && track.quality('highPurity') && muonID('TrackerMuonArbitrated') && muonID('TMOneStationTight') && (track.hitPattern.trackerLayersWithMeasurement > 5) && (track.normalizedChi2 < 100000) && (track.hitPattern.pixelLayersWithMeasurement > 0) && (abs(dB) < 0.3) && (abs(track.dz) < 20)"

from MuonAnalysis.TagAndProbe.common_variables_cff import *
process.load("MuonAnalysis.TagAndProbe.common_modules_cff")

process.tagMuons = cms.EDFilter("PATMuonSelector",
                                src = cms.InputTag("patMuonsWithTrigger"),
                                cut = cms.string(TRACKaMUON+ "&&"+ IN_ACCEPTANCEOLD + "&& (pt>3)"),
                                )

process.oneTag  = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("tagMuons"), minNumber = cms.uint32(1))

process.probeMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuonsWithTrigger"),
    #cut = cms.string("track.isNonnull" + " && " + IN_ACCEPTANCE + " && track.quality('highPurity')")
    cut = cms.string("track.isNonnull && track.quality('highPurity')")
)

process.tpPairs = cms.EDProducer("CandViewShallowCloneCombiner",
    cut = cms.string('2.6 < mass < 3.5 && abs(daughter(0).vz - daughter(1).vz) < 1'),
    decay = cms.string('tagMuons@+ probeMuons@-')
)
process.onePair = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("tpPairs"), minNumber = cms.uint32(1))

#process.tagMuonsMCMatch = cms.EDProducer("MCTruthDeltaRMatcherNew",
#    src = cms.InputTag("tagMuons"),
#    matched = cms.InputTag("genParticles"),
#    pdgId = cms.vint32(13),
#    distMin = cms.double(0.3),
#)
#process.probeMuonsMCMatch = process.tagMuonsMCMatch.clone(src = "probeMuons")

from MuonAnalysis.TagAndProbe.muon.tag_probe_muon_extraIso_cff import ExtraIsolationVariables

process.tpTree = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                addEventVariablesInfo = cms.bool(True),
                                # choice of tag and probe pairs, and arbitration
                                tagProbePairs = cms.InputTag("tpPairs"),
                                arbitration   = cms.string("None"),
                                # probe variables: all useful ones
                                variables = cms.PSet(
        KinematicVariables,
        IsolationVariables,
        MuonIDVariables,
        TrackQualityVariables,
        GlobalTrackQualityVariables,
        TriggerVariables,
        dxyBS = cms.InputTag("muonDxyPVdzmin","dxyBS"),
        dxyPVdzmin = cms.InputTag("muonDxyPVdzmin","dxyPVdzmin"),
        dzPV = cms.InputTag("muonDxyPVdzmin","dzPV"),
        nSplitTk  = cms.InputTag("splitTrackTagger"),
        
        ),
                                flags = cms.PSet(
        #        TrackQualityFlags,
        HFHIPhysicsFlagsTrigger,
        MuonIDFlags,
        Acc_JPsiOLD = cms.string(IN_ACCEPTANCEOLD),
        TRACK_MUON= cms.string(TRACKaMUON),
        ),
                                tagVariables = cms.PSet(
        pt  = cms.string('pt'),
        eta = cms.string('eta'),
        phi = cms.string('phi'),
        nValHits = cms.string('outerTrack.numberOfValidHits'),
        nVertices = cms.InputTag("nverticesModule"),
        l1rate = cms.InputTag("l1rate"),
        bx     = cms.InputTag("l1rate","bx"),
        ),
                                tagFlags     = cms.PSet(
        TrackQualityFlags,
        MuonIDFlags,
        HFHIPhysicsFlagsTrigger,
        ),
                                pairVariables = cms.PSet(
        pt = cms.string("pt"),
        dphiVtxTimesQ = cms.InputTag("tagProbeSeparation", "dphiVtxTimesQ"),
        drM1          = cms.InputTag("tagProbeSeparation", "drM1"),
        dphiM1        = cms.InputTag("tagProbeSeparation", "dphiM1"),
        distM1        = cms.InputTag("tagProbeSeparation", "distM1"),
        drM2          = cms.InputTag("tagProbeSeparation", "drM2"),
        dphiM2        = cms.InputTag("tagProbeSeparation", "dphiM2"),
        distM2        = cms.InputTag("tagProbeSeparation", "distM2"),
        drVtx         = cms.InputTag("tagProbeSeparation", "drVtx"),
        dz            = cms.string("daughter(0).vz - daughter(1).vz"),
        probeMultiplicity = cms.InputTag("probeMultiplicity"),
    ),
    pairFlags = cms.PSet(),
    isMC           = cms.bool(False),
    #tagMatches       = cms.InputTag("tagMuonsMCMatch"),
    #probeMatches     = cms.InputTag("probeMuonsMCMatch"),
    #motherPdgId      = cms.vint32(443),
    #makeMCUnbiasTree       = cms.bool(False),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes              = cms.InputTag("probeMuons"),
)

process.load("MuonAnalysis.TagAndProbe.muon.tag_probe_muon_extraIso_cfi")

process.tnpSimpleSequence = cms.Sequence(
    #process.tagMuons   * process.tagMuonsMCMatch   +
    process.tagMuons    +
    process.oneTag     +
    #process.probeMuons * process.probeMuonsMCMatch +
    process.probeMuons +
    process.tpPairs    +
    process.onePair    +
    process.muonDxyPVdzmin +
    process.nverticesModule +
    process.tagProbeSeparation +
    process.computeCorrectedIso + 
    process.probeMultiplicity + 
    process.splitTrackTagger +
    process.l1rate +
    process.tpTree
)

process.tagAndProbe = cms.Path( 
    process.PAcollisionEventSelection *
    process.siPixelRecHits * 
    process.pACentrality *
    process.HLTMu    +
    process.mergedMuons                 *
    process.patMuonsWithTriggerSequence *
    process.tnpSimpleSequence
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("tnpJPsi_dataPromtReco.root"))
