# Auto generated configuration file
# using: 
# Revision: 1.232.2.6 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/EightTeV/PYTHIA6_JPsiWithFSR_tuneD6T_8TeV_cff.py -s GEN:ProductionFilterSequence --eventcontent RECOSIM --datatier GEN-SIM --conditions STARTHI53_V17HI::All --no_exec
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

#production info.
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Source: /local/reps/CMSSW/CMSSW/Configuration/GenProduction/python/EightTeV/PYTHIA6_JPsiWithFSR_tuneD6T_8TeV_cff.py,v $'),
    annotation = cms.untracked.string('Winter13: Pythia6 generation of prompt JPsi, 5.023TeV, D6T tune'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    output = cms.untracked.int32(500)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(
	wantSummary = cms.untracked.bool(True)
)

# seeds
#from IOMC.RandomEngine.RandomServiceHelper import RandomNumberServiceHelper
#randSvc = RandomNumberServiceHelper(process.RandomNumberGeneratorService)
#randSvc.populate()
#print process.RandomNumberGeneratorService.dumpConfig()

##from Configuration.AlCa.GlobalTag import GlobalTag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'STARTHI53_V27::All'

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'STARTHI53_V27::All', '')

from Configuration.Generator.PythiaUESettings_cfi import *

process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(5023.0),
    # PYTHIA xSec: 15.12 mb
    crossSection = cms.untracked.double(13775390),#8978000.0
    filterEfficiency = cms.untracked.double(0.0463),#0.0513
    maxEventsToPrint = cms.untracked.int32(0),
    ExternalDecays = cms.PSet(
        EvtGen = cms.untracked.PSet(
            operates_on_particles = cms.vint32(0),
            use_default_decay = cms.untracked.bool(False),
            decay_table = cms.FileInPath('GeneratorInterface/ExternalDecays/data/DECAY_NOLONGLIFE.DEC'),
            particle_property_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/evt.pdl'),
            user_decay_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/Onia_mumu.dec'),
            list_forced_decays = cms.vstring('MyJ/psi'),
       ),
        parameterSets = cms.vstring('EvtGen')
    ),
   PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        processParameters = cms.vstring('MSEL=61          ! Quarkonia', 
            'MDME(858,1) = 0  ! 0.060200    e-    e+', 
            'MDME(859,1) = 1  ! 0.060100    mu-  mu+', 
            'MDME(860,1) = 0  ! 0.879700    rndmflav        rndmflavbar', 
            'MSTP(142)=2      ! turns on the PYEVWT Pt re-weighting routine', 
            'PARJ(13)=0.750   ! probability that a c or b meson has S=1', 
            'PARJ(14)=0.162   ! probability that a meson with S=0 is produced with L=1, J=1', 
            'PARJ(15)=0.018   ! probability that a meson with S=1 is produced with L=1, J=0', 
            'PARJ(16)=0.054   ! probability that a meson with S=1 is produced with L=1, J=1', 
            'MSTP(145)=0      ! choice of polarization', 
            'MSTP(146)=0      ! choice of polarization frame ONLY when mstp(145)=1', 
            'MSTP(147)=0      ! particular helicity or density matrix component when mstp(145)=1', 
            'MSTP(148)=1      ! possibility to allow for final-state shower evolution, extreme case !', 
            'MSTP(149)=1      ! if mstp(148)=1, it determines the kinematics of the QQ~3S1(8)->QQ~3S1(8)+g branching', 
            'PARP(141)=1.16   ! New values for COM matrix elements', 
            'PARP(142)=0.0119 ! New values for COM matrix elements', 
            'PARP(143)=0.01   ! New values for COM matrix elements', 
            'PARP(144)=0.01   ! New values for COM matrix elements', 
            'PARP(145)=0.05   ! New values for COM matrix elements', 
            'PARP(146)=9.28   ! New values for COM matrix elements', 
            'PARP(147)=0.15   ! New values for COM matrix elements', 
            'PARP(148)=0.02   ! New values for COM matrix elements', 
            'PARP(149)=0.02   ! New values for COM matrix elements', 
            'PARP(150)=0.085  ! New values for COM matrix elements', 
            'BRAT(861)=0.202  ! chi_2c->J/psi gamma', 
            'BRAT(862)=0.798  ! chi_2c->rndmflav rndmflavbar', 
            'BRAT(1501)=0.013 ! chi_0c->J/psi gamma', 
            'BRAT(1502)=0.987 ! chi_0c->rndmflav rndmflavbar', 
            'BRAT(1555)=0.356 ! chi_1c->J/psi gamma', 
            'BRAT(1556)=0.644 ! chi_1c->rndmflav rndmflavbar'),
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters', 
            'CSAParameters'),
        CSAParameters = cms.vstring('CSAMODE = 6     ! cross-section reweighted quarkonia')
    )
)


## for filters

process.oniafilter = cms.EDFilter("PythiaFilter",
    Status = cms.untracked.int32(2),
    MaxEta = cms.untracked.double(1e100),
    MinEta = cms.untracked.double(-1e100),
    MinPt = cms.untracked.double(0.0),
    ParticleID = cms.untracked.int32(443)
)

process.mumugenfilter = cms.EDFilter("MCParticlePairFilter",
    Status = cms.untracked.vint32(1, 1),
    MinP = cms.untracked.vdouble(2.5, 2.5),
    MaxEta = cms.untracked.vdouble(2.5, 2.5),
    MinEta = cms.untracked.vdouble(-2.5, -2.5),
    ParticleCharge = cms.untracked.int32(-1),
    ParticleID1 = cms.untracked.vint32(13),
    ParticleID2 = cms.untracked.vint32(13)
)

#process.ProductionFilterSequence = cms.Sequence(process.generator*process.VtxSmeared*process.oniafilter*process.mumugenfilter)
process.ProductionFilterSequence = cms.Sequence(process.generator*process.oniafilter*process.VtxSmeared*process.mumugenfilter)


# Output definition
process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
	splitLevel = cms.untracked.int32(0),
	eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
	outputCommands = process.RAWSIMEventContent.outputCommands,
	fileName = cms.untracked.string('PromptJpsi_PYTHIA_1stBoost_502TeV_GENSIM.root'),
	dataset = cms.untracked.PSet(
		filterName = cms.untracked.string(''),
		dataTier = cms.untracked.string('GEN-SIM')
	),
	SelectEvents = cms.untracked.PSet(
		SelectEvents = cms.vstring('generation_step')
	)
)

# Path and EndPath definitions
process.generation_step = cms.Path( process.ProductionFilterSequence * process.genParticles)
process.simulation_step = cms.Path(process.psim)

process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# avoid problem with unknown pdg code
process.hiGenParticles.abortOnUnknownPDGCode = cms.untracked.bool(False)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)

# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq

