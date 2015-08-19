import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("HIOnia")

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

# setup any defaults you want
#options.outputFile = "PRMC_eff_Histos.root"
options.outputFile = "inclBtoJPsiMuMu_pa_1st_run_STARTHI53_V27-v1_noAccCut_sglTrig_genMatch-v2.root"
options.secondaryOutputFile = "PRMC_eff_DataSet.root"
#options.inputFiles = 'file:/afs/cern.ch/work/k/kyolee/private/cms538HI/src/HiAnalysis/HiOnia/test/onia2MuMuPAT_MC_1000_1_PR.root'
options.inputFiles = '/store/user/kyolee/PromptJpsi_PYTHIAboosted_1st_GENSIM_STARTHI53_V27/PromptJpsi_PYTHIAboosted_1st_SKIM_STARTHI53_V27/108b77595cb4cf4dfdd6f615def31b81/PromptJpsi_PYTHIAboosted_1st_SKIM_STARTHI53_V27_100_1_hdE.root'
#options.maxEvents = 1000 # -1 means all events
options.maxEvents = -1 # -1 means all events

# get and parse the command line arguments
options.parseArguments()

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR_E_V33::All' # express reco
#process.GlobalTag.globaltag = 'GR_P_V43D::All' # prompt reco
process.GlobalTag.globaltag = 'STARTHI53_V27::All' # PRMC & NPMC for efficiency

from HeavyIonsAnalysis.Configuration.CommonFunctions_cff import *
overrideCentrality(process)

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFtowersPlusTrunc"),
    nonDefaultGlauberModel = cms.string(""),
    centralitySrc = cms.InputTag("pACentrality"),
    pPbRunFlip = cms.untracked.uint32(211313)
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        options.inputFiles
    )
)

process.hltDblMuOpen = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                 HLTPaths = cms.vstring("HLT_PAL1DoubleMuOpen_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hltDblMu0 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                 HLTPaths = cms.vstring("HLT_PAL1DoubleMu0_HighQ_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hltDblMu3 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                 HLTPaths = cms.vstring("HLT_PAL2DoubleMu3_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hltMu3 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                 HLTPaths = cms.vstring("HLT_PAMu3_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hltMu7 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                 HLTPaths = cms.vstring("HLT_PAMu7_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hltMu12 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                 HLTPaths = cms.vstring("HLT_PAMu12_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.hltMult100DblMu3 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                 HLTPaths = cms.vstring("HLT_PAPixelTrackMultiplicity100_L2DoubleMu3_v*"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

#process.hionia = cms.EDAnalyzer('HiOniaAnalyzer',
#process.hionia = cms.EDAnalyzer('MyEffAnalyzer',
process.hionia = cms.EDAnalyzer('MyEffAnalyzer_noAccCut_sglTrig_genMatch',
                                srcMuon = cms.InputTag("patMuonsWithTrigger"),
                                srcMuonNoTrig = cms.InputTag("patMuonsWithoutTrigger"),
                                src = cms.InputTag("onia2MuMuPatTrkTrk"),
                                srcTracks = cms.InputTag("generalTracks"),
                                genParticles = cms.InputTag("genParticles"), #KYO - pp
#                                genParticles = cms.InputTag("hiGenParticles"), #KYO - embedded
                                primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
                                triggerResultsLabel = cms.InputTag("TriggerResults","","HLT"),
                                srcCentrality = cms.InputTag("pACentrality"),
                                
                                #-- Reco Details
                                useBeamSpot = cms.bool(False),
                                useRapidity = cms.bool(True),
                                
                                #--
                                maxAbsZ = cms.double(24.0),
                                
                                pTBinRanges = cms.vdouble(0.0, 6.0, 8.0, 9.0, 10.0, 12.0, 15.0, 40.0),
                                etaBinRanges = cms.vdouble(0.0, 2.5),
                                centralityRanges = cms.vdouble(20,40,100),

                                onlyTheBest = cms.bool(False),		
                                applyCuts = cms.bool(True),
                                storeEfficiency = cms.bool(False),
                      
                                removeSignalEvents = cms.untracked.bool(False),
                                removeTrueMuons = cms.untracked.bool(False),
                                storeSameSign = cms.untracked.bool(True),
                                
                                muonLessPV = cms.bool(False), # KYO for pp
                                #muonLessPV = cms.bool(True), # KYO for embedded and pPbData

                                #-- Gen Details
                                oniaPDG = cms.int32(443),
                                isHI = cms.untracked.bool(False),
                                isPA = cms.untracked.bool(False),
                                isMC = cms.untracked.bool(True),
                                isPromptMC = cms.untracked.bool(False), #PR or NP KYO

                                #-- Histogram configuration
                                combineCategories = cms.bool(False),
                                fillRooDataSet = cms.bool(False),
                                fillTree = cms.bool(True),
                                fillHistos = cms.bool(False),
                                doGenMatching = cms.untracked.bool(True), # KYO
                                minimumFlag = cms.bool(False),  # KYO
                                fillSingleMuons = cms.bool(True),
                                fillRecoTracks = cms.bool(True),
                                histFileName = cms.string(options.outputFile),		
                                dataSetName = cms.string(options.secondaryOutputFile),
                                
                                #--
                                # NumberOfTriggers = cms.uint32(8),
                                dblTriggerPathNames = cms.vstring("HLT_PAL1DoubleMuOpen_v1",
                                                                  "HLT_PAL1DoubleMu0_HighQ_v1",
                                                                  "HLT_PAL2DoubleMu3_v1",
                                                                  "HLT_PAPixelTrackMultiplicity100_L2DoubleMu3_v1"),
                                dblTriggerFilterNames = cms.vstring("hltL1fL1sPAL1DoubleMuOpenL1Filtered0",
                                                                    "hltL1fL1sPAL1DoubleMu0HighQL1FilteredHighQ",
                                                                    "hltL2fL1sPAL2DoubleMu3L2Filtered3",
                                                                    "hltL2fL1sPAL2DoubleMu3L2Filtered3"),
                                sglTriggerPathNames = cms.vstring("HLT_PAMu3_v1",
                                                                  "HLT_PAMu7_v1",
                                                                  "HLT_PAMu12_v1"),
                                sglTriggerFilterNames = cms.vstring("hltL3fL2sMu3L3Filtered3",
                                                                    "hltL3fL2sMu7L3Filtered7",
                                                                    "hltL3fL2sMu12L3Filtered12")


                                )


#process.p = cms.Path(process.hltDblMuOpen*process.hionia)
process.p = cms.Path(process.hionia)
