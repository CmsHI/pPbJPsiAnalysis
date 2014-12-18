# Auto generated configuration file
# using: 
# Revision: 1.381.2.28 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/HI/PYTHIA6_inclBtoPsiMuMu_5TeV02_cff.py --fileout file:HIN-pAWinter13-inclBtoPsiMuMu_5TeV02.root --mc --eventcontent RAWSIM --pileup NoPileUp --datatier GEN-SIM --conditions START53_V28::All --beamspot Realistic8TeVCollisionPPbBoost --step GEN,SIM --datamix NODATAMIXER --python_filename HIN-pAWinter13-inclBtoPsiMuMu_5TeV02_1_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 26
import FWCore.ParameterSet.Config as cms

process = cms.Process('SIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('GeneratorInterface.HiGenCommon.VtxSmearedRealisticPPbBoost8TeVCollision_cff')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    #input = cms.untracked.int32(2)
    #input = cms.untracked.int32(100)
    input = cms.untracked.int32(50)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('Winter13: Pythia6+EvtGen generation of B0->Jpsi->MuMu, 5.023TeV, Z2star tune'),
    name = cms.untracked.string('$Source: /local/reps/CMSSW/CMSSW/Configuration/GenProduction/python/EightTeV/PYTHIA6_inclB0toJpsiMuMu_8TeV_cff.py,v $')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
#    fileName = cms.untracked.string('file:HIN-pAWinter13-inclBtoPsiMuMu_5TeV02.root'),
    fileName = cms.untracked.string('file:test_np_Pbp.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V27::All', '')

#process.bfilter = cms.EDFilter("PythiaFilter",
#    ParticleID = cms.untracked.int32(5)
#)

#process.oniafilter = cms.EDFilter("MCSingleParticleFilter",
#    MaxEta = cms.untracked.vdouble(1000.0, 1000.0),
#    Status = cms.untracked.vint32(2, 2),
#    MinEta = cms.untracked.vdouble(-1000.0, -1000.0),
#    MinPt = cms.untracked.vdouble(0.0, 0.0),
#    ParticleID = cms.untracked.vint32(443, 100443)
#)

#process.generator = cms.EDFilter("Pythia6GeneratorFilter",
process.generator = cms.EDFilter("PyquenGeneratorFilter",
    ExternalDecays = cms.PSet(
        EvtGen = cms.untracked.PSet(
            use_default_decay = cms.untracked.bool(False),
            decay_table = cms.FileInPath('GeneratorInterface/ExternalDecays/data/DECAY_NOLONGLIFE.DEC'),
            particle_property_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/evt.pdl'),
            user_decay_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/incl_BtoJpsi_mumu.dec'),
            list_forced_decays = cms.vstring('MyB0', 
                'Myanti-B0', 
                'MyB+', 
                'MyB-', 
                'MyB_s0', 
                'Myanti-B_s0', 
                'MyLambda_b0', 
                'Myanti-Lambda_b0'),
            operates_on_particles = cms.vint32(0)
        ),
        parameterSets = cms.vstring('EvtGen')
    ),
    
		filterType = cms.untracked.string('PartonHadronDecayGenEvtSelector'),
		partons = cms.vint32(5),
		partonStatus = cms.vint32(2),
		partonEtaMax = cms.vdouble(999.),
		partonPtMin  = cms.vdouble(0.),
		hadrons = cms.vint32(443,100443),
		hadronStatus = cms.vint32(2,2),
		hadronEtaMin = cms.vdouble(-1000.0,-1000.0),
		hadronEtaMax = cms.vdouble(1000.0,1000.0),
		hadronPtMin = cms.vdouble(0.0,0.0),
		hadronPtMax = cms.vdouble(50.0,50.0),
		hadronPMin = cms.vdouble(0.0,0.0),
		decays = cms.int32(13),
		decayStatus = cms.int32(1),
		decayPtMin = cms.double(0.0),
		decayPtMax = cms.double(999),
		decayEtaMin = cms.double(-2.03),
		decayEtaMax = cms.double(2.97),
		decayPMin = cms.double(1.7),
#		decayEtaMin = cms.double(-2.5),
#		decayEtaMax = cms.double(2.5),
#		decayPMin = cms.double(2.5),
		decayNtrig = cms.int32(2),

		aBeamTarget = cms.double(208.0), ## beam/target atomic number
		angularSpectrumSelector = cms.int32(0), ## angular emitted gluon spectrum
		cFlag = cms.int32(0), ## centrality flag
		bMin = cms.double(0.0),  ## min impact param (fm); valid only if cflag_!=0
		bMax = cms.double(0.0),  ## max impact param (fm); valid only if cflag_!=0
		bFixed = cms.double(0.0), ## fixed impact param (fm); valid only if cflag_=0
		embeddingMode = cms.bool(False),
		doQuench = cms.bool(False),
		doIsospin = cms.bool(True),
		doCollisionalEnLoss = cms.bool(False),
		doRadiativeEnLoss = cms.bool(True),
		qgpNumQuarkFlavor = cms.int32(0), ## number of active quark flavors in qgp; allowed values: 0,1,2,3
		qgpProperTimeFormation = cms.double(0.1), ## proper time of QGP formation; allowed range [0.01,10.0]fm/c;
		qgpInitialTemperature = cms.double(1.0), ## initial temperature of QGP; allowed range [0.2,2.0]GeV;

		pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(0.0003114),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(5023.0),
    crossSection = cms.untracked.double(54402709.5),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring('MSTU(21)=1     ! Check on possible errors during program execution', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
            'MSTP(52)=2     ! work with LHAPDF', 
            'PARP(82)=1.921 ! pt cutoff for multiparton interactions', 
            'PARP(89)=1800. ! sqrts for which PARP82 is set', 
            'PARP(90)=0.227 ! Multiple interactions: rescaling power', 
            'MSTP(95)=6     ! CR (color reconnection parameters)', 
            'PARP(77)=1.016 ! CR', 
            'PARP(78)=0.538 ! CR', 
            'PARP(80)=0.1   ! Prob. colored parton from BBR', 
            'PARP(83)=0.356 ! Multiple interactions: matter distribution parameter', 
            'PARP(84)=0.651 ! Multiple interactions: matter distribution parameter', 
            'PARP(62)=1.025 ! ISR cutoff', 
            'MSTP(91)=1     ! Gaussian primordial kT', 
            'PARP(93)=10.0  ! primordial kT-max', 
            'MSTP(81)=21    ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model'),
        bbbarSettings = cms.vstring('MSEL = 1'),
        parameterSets = cms.vstring('pythiaUESettings', 
            'bbbarSettings')
    )
)


#process.mumugenfilter = cms.EDFilter("MCParticlePairFilter",
#    Status = cms.untracked.vint32(1, 1),
#    MaxEta = cms.untracked.vdouble(2.5, 2.5),
#    MinEta = cms.untracked.vdouble(-2.5, -2.5),
#    ParticleCharge = cms.untracked.int32(-1),
#    MinP = cms.untracked.vdouble(2.5, 2.5),
#    ParticleID1 = cms.untracked.vint32(13),
#    ParticleID2 = cms.untracked.vint32(13)
#)


#process.ProductionFilterSequence = cms.Sequence(process.generator+process.bfilter+process.oniafilter+process.mumugenfilter)
process.ProductionFilterSequence = cms.Sequence(process.generator)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# End of customisation functions
